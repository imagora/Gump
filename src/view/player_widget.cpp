// Copyright (c) 2014-2019 winking324
//

#include "view/player_widget.h"

#include <log4cplus/log4cplus.h>
#include <QtAVWidgets/global.h>
#include <QTimer>
#include <QScreen>
#include <QPalette>
#include <QHBoxLayout>
#include <QApplication>
#include <QStackedLayout>
#include <QDesktopWidget>

#include "commons/definations.h"


namespace gump {


const static std::map<uint32_t, std::string> kStatusStr = {
  { QtAV::UnknownMediaStatus, "Unknown" },
  { QtAV::NoMedia, "No Media" },
  { QtAV::LoadingMedia, "Loading Media" },
  { QtAV::LoadedMedia, "Loaded Media" },
  { QtAV::StalledMedia, "Stalled Media" },
  { QtAV::BufferingMedia, "Buffering Media" },
  { QtAV::BufferedMedia, "Buffered Media" },
  { QtAV::EndOfMedia, "End Of Media" },
  { QtAV::InvalidMedia, "Invalid Media" },
};


PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent) {
  is_show_details_ = false;

  QDesktopWidget *desktop = QApplication::desktop();
  current_screen_number_ = desktop->screenNumber(this);
  if (current_screen_number_ < 0) {
    current_screen_number_ = 0;
    current_screen_ratio_ = 2;
  } else {
    current_screen_ratio_ =
        desktop->screen(current_screen_number_)->devicePixelRatio();
  }

  QHBoxLayout *main_layout = new QHBoxLayout();
  QStackedLayout *stacked_layout = new QStackedLayout();
  stacked_layout->setMargin(2);
  stacked_layout->setStackingMode(QStackedLayout::StackAll);

  video_output_ = new QtAV::VideoOutput(QtAV::VideoRendererId_Widget, this);
  if (!video_output_->widget()) {
    LOG4CPLUS_ERROR_STR(kLoggerName,
                        "Error: can not create video renderer retina");
    return;
  }

  player_ = new QtAV::AVPlayer(this);
  buffered_player_ = new QtAV::AVPlayer(this);

  player_->addVideoRenderer(video_output_);
  player_status_ = new QLabel(PlayerStatus(), this);
  QPalette pe;
  pe.setColor(QPalette::WindowText, Qt::red);
  player_status_->setPalette(pe);
  player_status_->setAlignment(Qt::AlignLeft|Qt::AlignTop);

  stacked_layout->addWidget(player_status_);
  stacked_layout->addWidget(video_output_->widget());
  setFixedWidth(300);

  main_layout->addLayout(stacked_layout);
  setLayout(main_layout);
  player_status_->raise();

  connect(player_, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this,
          SLOT(OnMediaStatusChanged(QtAV::MediaStatus)));
  connect(player_, SIGNAL(error(QtAV::AVError)), this,
          SLOT(OnPlayerError(QtAV::AVError)));
  connect(buffered_player_, SIGNAL(error(QtAV::AVError)), this,
          SLOT(OnPlayerError(QtAV::AVError)));

  QTimer::singleShot(2000, this, SLOT(RefreshMediaInfoTimer()));
}

PlayerWidget::~PlayerWidget() {
}

void PlayerWidget::PlayStream(const std::string &stream) {
  if (stream.empty() && !stream_.empty()) {
    player_->pause(false);
    return;
  }

  if (stream_ == stream) return;

  if (stream == buffered_stream_) {
    stream_ = buffered_stream_;
    buffered_stream_.clear();

    StopStream();
    player_->clearVideoRenderers();

    buffered_player_->addVideoRenderer(video_output_);
    std::swap(player_, buffered_player_);

    auto *audio = player_->audio();
    audio->setMute(false);
    return;
  }

  player_->play(QString::fromStdString(stream));
  stream_ = stream;
}

void PlayerWidget::BufferStream(const std::string &stream) {
  if (buffered_stream_ == stream) return;

  if (buffered_player_->isPlaying()) {
    buffered_player_->stop();
  }

  buffered_player_->play(QString::fromStdString(stream));
  auto *audio = buffered_player_->audio();
  audio->setMute();

  buffered_stream_ = stream;
}

void PlayerWidget::StopStream() {
  if (player_->isPlaying()) {
    player_->stop();
  }
}

void PlayerWidget::StartStream() {
  if (!player_->isPlaying() && !stream_.empty()) {
    player_->play(QString::fromStdString(stream_));
    return;
  }

  if (player_->isPaused()) {
    player_->pause(false);
    return;
  }
}

void PlayerWidget::PauseStream() {
  player_->pause();
}

void PlayerWidget::ShowDetails() {
  is_show_details_ = !is_show_details_;
}

void PlayerWidget::WindowMove() {
  QDesktopWidget *desktop = QApplication::desktop();
  int screen_number = desktop->screenNumber(this);
  if (screen_number == current_screen_number_) return;
  current_screen_number_ = screen_number;

  int screen_ratio =
      desktop->screen(current_screen_number_)->devicePixelRatio();
  if (screen_ratio == current_screen_ratio_) return;
  current_screen_ratio_ = screen_ratio;

  LOG4CPLUS_WARN_FMT(kLoggerName, "screen changed, current number: %d, "
                                  "current ratio: %d, change video output",
                     current_screen_number_, current_screen_ratio_);
}

void PlayerWidget::OnMediaStatusChanged(QtAV::MediaStatus status) {
  player_status_->setText(PlayerStatus(status));
  player_status_->raise();
}

void PlayerWidget::OnPlayerError(const QtAV::AVError &e) {
  LOG4CPLUS_ERROR_FMT(kLoggerName, "player error[%d]: %s", e.error(),
                      e.string().toLatin1().data());
}

void PlayerWidget::RefreshMediaInfoTimer() {
  QTimer::singleShot(2000, this, SLOT(RefreshMediaInfoTimer()));
  if (!player_->isPlaying()) {
    return;
  }

  QString player_status = PlayerStatus();
  if (is_show_details_) {
    const QtAV::Statistics &avstat = player_->statistics();
    if (avstat.audio.available) {
      player_status += "\nAudio:";
      player_status +=
          QString::fromLatin1("\n\tCodec: %1").arg(avstat.audio.codec);
      player_status +=
          QString::fromLatin1("\n\tBitrate: %1").arg(avstat.audio.bit_rate);
      player_status +=
          QString::fromLatin1("\n\tSamplerate: %1").arg(
            avstat.audio_only.sample_rate);
      player_status +=
          QString::fromLatin1("\n\tAudioChannels: %1").arg(
            avstat.audio_only.channels);
    }

    if (avstat.video.available) {
      player_status += "\nVideo:";
      player_status +=
          QString::fromLatin1("\n\tCodec: %1").arg(avstat.video.codec);
      player_status +=
          QString::fromLatin1("\n\tFPS: %1").arg(
            avstat.video_only.currentDisplayFPS());
      player_status +=
          QString::fromLatin1("\n\tBitrate: %1").arg(avstat.video.bit_rate);
      player_status +=
          QString::fromLatin1("\n\tResolution: %1x%2").arg(
            avstat.video_only.width).arg(avstat.video_only.height);
    }
  }

  player_status_->setText(player_status);
  player_status_->raise();
}

QString PlayerWidget::PlayerStatus(uint32_t status) {
  if (status == QtAV::UnknownMediaStatus) {
    status = player_->mediaStatus();
  }

  auto iter = kStatusStr.find(status);
  if (iter == kStatusStr.end()) {
    iter = kStatusStr.begin();
  }
  return iter->second.c_str();
}


}  // namespace gump

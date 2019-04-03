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
#include "commons/singleton.h"
#include "controller/player_controller.h"


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

//  player_status_ = new QLabel(PlayerStatus(), this);
  player_status_ = new QLabel("TODO", this);
  QPalette pe;
  pe.setColor(QPalette::WindowText, Qt::red);
  player_status_->setPalette(pe);
  player_status_->setAlignment(Qt::AlignLeft|Qt::AlignTop);

  stacked_layout->addWidget(player_status_);
  stacked_layout->addWidget(video_output_->widget());
//  setFixedWidth(30);

  main_layout->addLayout(stacked_layout);
  setLayout(main_layout);
  player_status_->raise();

//  QTimer::singleShot(2000, this, SLOT(RefreshMediaInfoTimer()));
}

void PlayerWidget::PlayStream(const QString &stream) {
  Singleton<PlayerController>::Instance()->PlayStream(stream, video_output_);
}

void PlayerWidget::BufferStream(const QString &stream) {
  Singleton<PlayerController>::Instance()->BufferStream(stream);
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
//  player_status_->setText(PlayerStatus(status));
//  player_status_->raise();
}


}  // namespace gump

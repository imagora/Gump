#include "player_widget.h"
#include "commons/definations.h"
#include <log4cplus/log4cplus.h>
#include <QHBoxLayout>
#include <QtAVWidgets/global.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>


using namespace gump;


PlayerWidget::PlayerWidget(QWidget *parent/* = nullptr*/)
  : QWidget(parent)
{
  player_ = new QtAV::AVPlayer(this);

  QHBoxLayout *layout = new QHBoxLayout();
  setLayout(layout);

  video_output_ = new QtAV::VideoOutput(QtAV::VideoRendererId_Widget, this);
  if (!video_output_->widget()) {
    LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Error: can not create video renderer retina");
    return;
  }

  QDesktopWidget *desktop = QApplication::desktop();
  current_screen_number_ = desktop->screenNumber(this);
  if (current_screen_number_ < 0) {
    current_screen_number_ = 0;
    current_screen_ratio_ = 2;
  } else {
    current_screen_ratio_ = desktop->screen(current_screen_number_)->devicePixelRatio();
  }

  player_->addVideoRenderer(video_output_);
  layout->addWidget(video_output_->widget());
  setFixedWidth(300);
  layout->setMargin(2);
}

void PlayerWidget::PlayStream(const std::string &stream)
{
  if (stream.empty() && !stream_.empty()) {
    player_->pause(false);
    return;
  }

  if (stream_ == stream) return;
  player_->play(QString::fromStdString(stream));
  stream_ = stream;
}

void PlayerWidget::StopStream()
{
  if (player_->isPlaying()) {
    player_->stop();
  }
}

void PlayerWidget::StartStream()
{
  if (!player_->isPlaying() && !stream_.empty()) {
    player_->play(QString::fromStdString(stream_));
    return;
  }

  if (player_->isPaused()) {
    player_->pause(false);
    return;
  }
}

void PlayerWidget::PauseStream()
{
  player_->pause();
}

void PlayerWidget::WindowMove()
{
  QDesktopWidget *desktop = QApplication::desktop();
  int screen_number = desktop->screenNumber(this);
  if (screen_number == current_screen_number_) return;
  current_screen_number_ = screen_number;

  int screen_ratio = desktop->screen(current_screen_number_)->devicePixelRatio();
  if (screen_ratio == current_screen_ratio_) return;
  current_screen_ratio_ = screen_ratio;

  LOG4CPLUS_WARN_FMT(LOGGER_NAME, "screen changed, current number: %d, "
                                  "current ratio: %d, change video output",
                     current_screen_number_, current_screen_ratio_);
}



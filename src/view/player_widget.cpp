#include "player_widget.h"
#include "commons/definations.h"
#include <log4cplus/log4cplus.h>
#include <QHBoxLayout>
#include <QtAVWidgets/global.h>


using namespace gump;


PlayerWidget::PlayerWidget(QWidget *parent/* = nullptr*/)
  : QWidget(parent)
{
  player_ = new QtAV::AVPlayer(this);

  QHBoxLayout *layout = new QHBoxLayout();
  setLayout(layout);

  video_output_ = new QtAV::VideoOutput(QtAV::VideoRendererId_GLWidget2, this);
  if (!video_output_->widget()) {
    LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Error: can not create video renderer");
    return;
  }
  video_output_->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);

  player_->setRenderer(video_output_);
  layout->addWidget(video_output_->widget());
  setFixedSize(360, 640);
}

void PlayerWidget::PlayStream(const std::string &stream)
{
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



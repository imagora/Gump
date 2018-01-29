#pragma once
#include <QWidget>
#include <QtAV/QtAV>


namespace gump {


class PlayerWidget : public QWidget
{
  Q_OBJECT
 public:
  PlayerWidget(QWidget *parent = nullptr);

  void PlayStream(const std::string &stream);

  void StopStream();

  void StartStream();

  void PauseStream();

  void WindowMove();

 private:
  int current_screen_number_;
  int current_screen_ratio_;
  QtAV::VideoOutput *video_output_;
  QtAV::VideoOutput *video_output_retina_;
  QtAV::AVPlayer *player_;
  std::string stream_;
};


}

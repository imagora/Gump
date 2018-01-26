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

 private:
  QtAV::VideoOutput *video_output_;
  QtAV::AVPlayer *player_;
  std::string stream_;
};


}

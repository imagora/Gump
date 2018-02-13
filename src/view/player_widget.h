#pragma once
#include <QWidget>
#include <QtAV/QtAV>


class QLabel;


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

 private slots:
  void OnMediaStatusChanged(QtAV::MediaStatus status);

  void OnPlayerError(const QtAV::AVError& e);

  void RefreshMediaInfoTimer();

 private:
  QString PlayerStatus(uint32_t status = 0);

 private:
  int current_screen_number_;
  int current_screen_ratio_;
  QtAV::VideoOutput *video_output_;
  QtAV::AVPlayer *player_;
  QLabel *player_status_;
  std::string stream_;
};


}

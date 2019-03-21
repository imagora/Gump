// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QLabel>
#include <QString>
#include <QWidget>
#include <QtAV/QtAV>

#include <string>


namespace gump {


class PlayerWidget :
    public QWidget {
  Q_OBJECT

 public:
  explicit PlayerWidget(QWidget *parent = nullptr);

  virtual ~PlayerWidget();

  void PlayStream(const std::string &stream);

  void BufferStream(const std::string &stream);

  void StopStream();

  void StartStream();

  void PauseStream();

  void ShowDetails();

  void WindowMove();

 private slots:
  void OnMediaStatusChanged(QtAV::MediaStatus status);

  void OnPlayerError(const QtAV::AVError& e);

  void RefreshMediaInfoTimer();

 private:
  QString PlayerStatus(uint32_t status = 0);

 private:
  bool is_show_details_;
  int current_screen_number_;
  int current_screen_ratio_;
  QtAV::VideoOutput *video_output_;
  QtAV::AVPlayer *player_;
  QtAV::AVPlayer *buffered_player_;
  QLabel *player_status_;
  std::string stream_;
  std::string buffered_stream_;
};


}  // namespace gump

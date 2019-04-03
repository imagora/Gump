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

  void PlayStream(const QString &stream);

  void BufferStream(const QString &stream);

  void ShowDetails();

  void WindowMove();

 private slots:
  void OnMediaStatusChanged(QtAV::MediaStatus status);

 private:
  bool is_show_details_;
  int current_screen_number_;
  int current_screen_ratio_;
  QtAV::VideoOutput *video_output_;

  QLabel *player_status_;
};


}  // namespace gump

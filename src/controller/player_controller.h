// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <list>
#include <QObject>
#include <QString>
#include <QtAV/QtAV>


namespace gump {


class PlayerController : public QObject {
  Q_OBJECT
 public:
  explicit PlayerController(QObject *parent = nullptr);

  void PlayStream(const QString &stream, QtAV::VideoOutput *renderer);

  void BufferStream(const QString &stream);

 signals:
  void StatusChangeEvent(QString status);

 public slots:
  void OnMediaStatusChanged(QtAV::MediaStatus status);

  void ReleasePlayers();

 private:
  QString GetPlayerStatus(QtAV::AVPlayer *player, uint32_t status = 0);

  void ResizeBuffer();

 private:
  struct BufferedPlayer {
    QString stream;
    QtAV::AVPlayer *player;
  };

 private:
  uint32_t max_size_;
  std::list<BufferedPlayer> buffered_players_;
};


}  // namespace gump


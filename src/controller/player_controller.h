// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <QString>
#include <QtAV/QtAV>
#include <list>

namespace gump {

class PlayerController : public QObject {
  Q_OBJECT
 public:
  explicit PlayerController(QObject *parent = nullptr);

  void GetStream(const QUrl &url);

  void PlayStream(const QString &stream);

  void BufferStream(const QString &stream);

  void SetRenderer(QtAV::VideoOutput *renderer);

  QString GetCurrentStream();

  QString GetCurrentStatus();

 signals:
  void StatusChangeEvent(QString status);

 public slots:
  void OnMediaStatusChanged(QtAV::MediaStatus status);

  void OnNetworkReply(QNetworkReply *reply);

  void ReleasePlayers();

 private:
  QString GetPlayerStatus(QtAV::AVPlayer *player, uint32_t status = 0);

  void ResizeBuffer();

 private:
  struct BufferedPlayer {
    QString stream;
    QtAV::AVPlayer *player = nullptr;
  };

 private:
  QtAV::VideoOutput *renderer_;
  std::list<BufferedPlayer> old_players_;
  std::list<BufferedPlayer> new_players_;
  QNetworkRequest config_request_;
  QNetworkAccessManager *network_manager_;
};

}  // namespace gump

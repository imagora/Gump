// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include <QString>
#include <QtAV/QtAV>

namespace gump {

class PlayerController : public QObject {
  Q_OBJECT

 private:
  struct StreamInfo {
    QString id;
    QString vendor;
    QString cname;
    QString uid;
    QString ip;
    QString url;
    QString play_url;
    QString prev;
    QString next;
    QtAV::AVPlayer *player = nullptr;
  };

 public:
  explicit PlayerController(QObject *parent = nullptr);

  void GetStream(const QUrl &url);

  void SearchStream(const QString &search_info);

  void SetRenderer(QtAV::VideoOutput *renderer);

  QString GetCurrentStatus();

 signals:
  void StatusChangeEvent(QString status);

 public slots:
  void OnMediaStatusChanged(QtAV::MediaStatus status);

  void OnNetworkReply(QNetworkReply *reply);

  void OnReleasePlayers();

  void OnStop();

  void OnPrev();

  void OnNext();

  void OnMute(bool mute);

 private:
  void StopRequest(QNetworkReply **reply);

  void HttpRequest(const QUrl &url, QNetworkReply **reply);

  void PlayStream(const QString &stream);

  void PlayStreamById(const QString &id);

  void BufferStreamById(const QString &id);

  QString GetPlayerStatus(QtAV::AVPlayer *player, uint32_t status = 0);

  void ResizeBuffer();

 private:
  bool is_mute_;
  QString current_stream_;
  QMap<QString, StreamInfo> streams_;

  QtAV::VideoOutput *renderer_;
  QNetworkReply *network_reply_;
  QNetworkReply *buffer_network_reply_;
  QNetworkAccessManager *network_manager_;
};

}  // namespace gump

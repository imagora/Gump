// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <map>
#include <QUrl>
#include <QTimer>
#include <QObject>
#include <QString>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "commons/stream_info.h"


namespace gump {


class OnlineController : public QObject {
  Q_OBJECT
 public:
  explicit OnlineController(QObject *parent = nullptr);

  void UpdateInfo(const QString &name, const QString &info);

  bool QueryStream(const QString &stream, QString *url, Stream *info);

  bool GetNextStream(const QString &stream, QString *url, Stream *info);

 signals:
  void RemoveAllEvent();

  void InsertStreamEvent(QString stream);

  void RemoveStreamEvent(QString stream);

 public slots:
  void OnUpdateUrl(QString url);

  void OnNetworkReply(QNetworkReply *reply);

  void OnRequestOnline();

 private:
  void RequestOnline();

  void UpdateList(const Streams &streams);

  void RemoveQuit(const Streams &streams);

  void InsertJoin(const Streams &streams);

 private:
  int refresh_timer_;
  Streams streams_;

  QUrl online_url_;
  QUrlQuery query_info_;

  QTimer *request_timer_;
  QNetworkReply *network_reply_;
  QNetworkAccessManager *network_manager_;
};


}  // namespace gump

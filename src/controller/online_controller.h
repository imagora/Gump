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

 signals:
  void UpdateList(MultiStreams streams);

 public slots:
  void OnUpdateUrl(QString url);

  void OnNetworkReply(QNetworkReply *reply);

  void OnRequestOnline();

 private:
  void RequestOnline();

 private:
  int refresh_timer_;
  QUrl online_url_;
  QUrlQuery query_info_;
  QTimer *request_timer_;
  QNetworkReply *network_reply_;
  QNetworkAccessManager *network_manager_;
};


}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <map>
#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>


namespace gump {


class ConfigController : public QObject {
  Q_OBJECT
 public:
  explicit ConfigController(QObject *parent = nullptr);

  void AutoRefreshConfig(int refresh_timer = 300);

 signals:
  void OnlineUrlEvent(QString url);

  void TracerUrlEvent(QString url);

  void TagsEvent(std::map<QString, QString> tags);

 public slots:
  void OnNetworkReply(QNetworkReply *reply);

  void RequestConfig();

 private:
  int refresh_timer_;
  QNetworkRequest config_request_;
  QNetworkAccessManager *network_manager_;
};


}  // namespace gump

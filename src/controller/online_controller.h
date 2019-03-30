// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

namespace gump {

class OnlineController : public QObject {
  Q_OBJECT
 public:
  explicit OnlineController(QObject *parent = nullptr);

  void UpdateInfo(const QString &name, const QString &info);

 signals:
  void UpdateList();

 public slots:
  void OnUpdateUrl(QString url);

  void OnNetworkReply(QNetworkReply *reply);

 private:
  void RequestOnline();

 private:
  int refresh_timer_;
  QString online_url_;
  QNetworkAccessManager *network_manager_;
};


}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QNetworkReply>
#include <QObject>
#include <QTime>
#include <QTimer>
#include <QUrlQuery>

#include "controller/auth_controller.h"

namespace gump {

class UserController : public QObject {
  Q_OBJECT
 public:
  explicit UserController(QObject *parent = nullptr);

  void Login(const QString &user);

  void Logout();

 public slots:
  void OnCheckTimer();

  void OnAuth(AuthStatus status);

  void OnNetworkReply(QNetworkReply *reply);

 signals:
  void LoginEvent(QString message);

  void LogoutEvent();

  void OfflineEvent();

 private:
  bool HttpRequest(const QString &name, QUrlQuery *query);

  void Ping();

  void OnLogin(const QString &message);

  void OnLogout(QNetworkReply::NetworkError code, const QByteArray &data);

  void OnPong(QNetworkReply::NetworkError code, const QByteArray &data);

 private:
  QString user_;
  QTime last_active_ts_;
  QTimer ping_timer_;

  AuthController *auth_controller_;
  QNetworkAccessManager *network_manager_;
  QMap<QNetworkReply *, QTime> network_replies_;
};

}  // namespace gump

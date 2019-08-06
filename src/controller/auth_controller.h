// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QOAuth2AuthorizationCodeFlow>


namespace gump {


enum class AuthStatus {
  kAuthNotAuthenticated,

  kAuthIdentifySuccess,
  kAuthIdentifyFailed,

  kAuthOAuthSuccess,
  kAuthOAuthFailed,
  kAuthOAuthTimeout,
  kAuthOAuthNotAuthenticated,
};


class AuthController :
    public QObject {
  Q_OBJECT
 public:
  explicit AuthController(QObject *parent = nullptr);

  void RequestIdentifier(const QString &username);

  void RequestOAuthToken();

  QString GetToken();

 signals:
  void Status(AuthStatus status);

 public slots:
  void OnIdentifierReply(QNetworkReply *reply);

  void OnOAuthReply(QNetworkReply *reply);

  void OnOAuthResponse();

 private:
  void InitOAuth();

  void BufferToken();

 private:
  AuthStatus status_;
  QOAuth2AuthorizationCodeFlow *oauth2_;
  QNetworkAccessManager *network_manager_;
};


}  // namespace gump


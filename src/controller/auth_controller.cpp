// Copyright (c) 2014-2019 winking324
//

#include "controller/auth_controller.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkRequest>
#include <QOAuthHttpServerReplyHandler>
#include <QSettings>
#include <QUrl>
#include <QUrlQuery>
#include <QtGlobal>

namespace gump {

static const QUrl kUserInfoUrl("https://oauth.agoralab.co/api/userInfo");

AuthController::AuthController(QObject *parent) : QObject(parent) {
  status_ = AuthStatus::kAuthNotAuthenticated;

  InitOAuth();

  network_manager_ = new QNetworkAccessManager(this);

  connect(oauth2_, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
          &QDesktopServices::openUrl);
  connect(oauth2_, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(OnOAuthReply(QNetworkReply *)));
  connect(oauth2_, SIGNAL(statusChanged(Status)), this,
          SLOT(OnOAuthResponse()));
  connect(network_manager_, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(OnIdentifierReply(QNetworkReply *)));
}

void AuthController::RequestIdentifier(const QString &username) {
  QUrlQuery query_info;
  query_info.addQueryItem("username", username);
  QUrl login_url("http://gump2.imagora.net:36767/auth/login");
  login_url.setQuery(query_info);

  qInfo() << "request identifier:" << login_url;
  network_manager_->get(QNetworkRequest(login_url));
}

void AuthController::RequestOAuthToken() {
  // 1. grant new token if empty
  if (oauth2_->token().isEmpty()) {
    oauth2_->grant();
  }

  // 2. identify user
  oauth2_->get(kUserInfoUrl);
}

QString AuthController::GetToken() {
  if (status_ != AuthStatus::kAuthOAuthSuccess) {
    return "";
  }

  return oauth2_->token();
}

void AuthController::RemoveToken() {
  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  settings.remove("token");
  settings.remove("refresh_token");
  settings.endGroup();
}

void AuthController::OnIdentifierReply(QNetworkReply *reply) {
  status_ = AuthStatus::kAuthIdentifyFailed;

  qInfo() << "on reply:" << reply->url();

  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    qCritical() << "cannot parse identifier response:" << data;
    emit Status(status_);
    return;
  }

  if (doc.isObject()) {
    QJsonObject obj = doc.object();

    auto id_iter = obj.find("id");
    if (id_iter.value().type() != QJsonValue::String ||
        id_iter.value().toString().isEmpty()) {
      qCritical() << "request identifier failed, id error";
      emit Status(status_);
      return;
    }

    auto key_iter = obj.find("key");
    if (key_iter.value().type() != QJsonValue::String ||
        key_iter.value().toString().isEmpty()) {
      qCritical() << "request identifier failed, key error";
      emit Status(status_);
      return;
    }

    status_ = AuthStatus::kAuthIdentifySuccess;
    qInfo() << "request identifier success";
    oauth2_->setClientIdentifier(id_iter.value().toString());
    oauth2_->setClientIdentifierSharedKey(key_iter.value().toString());
  }

  emit Status(status_);
}

void AuthController::OnOAuthReply(QNetworkReply *reply) {
  if (reply->url() == kUserInfoUrl) {
    auto code = reply->error();
    if (code != QNetworkReply::NoError) {
      qInfo() << "request user info failed:" << code;
      oauth2_->grant();
      return;
    }

    status_ = AuthStatus::kAuthOAuthSuccess;
    emit Status(status_);
    return;
  }
}

void AuthController::OnOAuthResponse() {
  switch (oauth2_->status()) {
    case QAbstractOAuth::Status::NotAuthenticated:
      status_ = AuthStatus::kAuthOAuthNotAuthenticated;
      qCritical() << "request oauth failed, not authenticated";
      emit Status(status_);
      break;
    case QAbstractOAuth::Status::TemporaryCredentialsReceived:
      qInfo() << "request oauthing";
      break;
    case QAbstractOAuth::Status::Granted:
      status_ = AuthStatus::kAuthOAuthSuccess;
      qInfo() << "request oauth success";
      BufferToken();
      emit Status(status_);
      break;
    case QAbstractOAuth::Status::RefreshingToken:
      qInfo() << "request oauth refresh";
      oauth2_->refreshAccessToken();
      break;
  }
}

void AuthController::InitOAuth() {
  static const QUrl kAuthUrl("https://oauth.agoralab.co/oauth/authorize");
  static const QUrl kTokenUrl("https://oauth.agoralab.co/oauth/token");

  oauth2_ = new QOAuth2AuthorizationCodeFlow(this);

  auto reply_handler = new QOAuthHttpServerReplyHandler(37676, this);
  reply_handler->setCallbackPath("auth_callback");

  oauth2_->setReplyHandler(reply_handler);
  oauth2_->setScope("read");
  oauth2_->setAuthorizationUrl(kAuthUrl);
  oauth2_->setAccessTokenUrl(kTokenUrl);

  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  QString token = settings.value("token").toString();
  QString refresh_token = settings.value("refresh_token").toString();
  settings.endGroup();

  if (!token.isEmpty()) {
    oauth2_->setToken(token);
  }

  if (!refresh_token.isEmpty()) {
    oauth2_->setRefreshToken(refresh_token);
  }
}

void AuthController::BufferToken() {
  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  settings.setValue("token", oauth2_->token());
  settings.setValue("refresh_token", oauth2_->refreshToken());
  settings.endGroup();
}

}  // namespace gump

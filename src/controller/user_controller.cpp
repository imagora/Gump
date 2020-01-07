// Copyright (c) 2014-2019 winking324
//

#include "controller/user_controller.h"

#include <QDebug>
#include <QMap>
#include <QSettings>

#include "commons/definations.h"

namespace gump {

static const int kPingInterval = 5 * 1000;
static const QMap<AuthStatus, QString> kAuthMessages = {
    {AuthStatus::kAuthNotAuthenticated, "not authenticated"},
    {AuthStatus::kAuthIdentifySuccess, "need oauth"},
    {AuthStatus::kAuthIdentifyFailed, "identify failed"},
    {AuthStatus::kAuthOAuthSuccess, ""},
    {AuthStatus::kAuthOAuthFailed, "oauth failed"},
    {AuthStatus::kAuthOAuthTimeout, "oauth timeout"},
    {AuthStatus::kAuthOAuthNotAuthenticated, "oauth not authenticated"},
};

using kNetworkReplyHandler =
    void (UserController::*)(QNetworkReply::NetworkError, const QByteArray &);

UserController::UserController(QObject *parent) : QObject(parent) {
  auth_controller_ = new AuthController(this);
  network_manager_ = new QNetworkAccessManager(this);

  ping_timer_.setInterval(kPingInterval);

  connect(&ping_timer_, SIGNAL(timeout()), this, SLOT(OnCheckTimer()));
  connect(auth_controller_, SIGNAL(Status(AuthStatus)), this,
          SLOT(OnAuth(AuthStatus)));
  connect(network_manager_, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(OnNetworkReply(QNetworkReply *)));
}

void UserController::Login(const QString &user) {
  if (!user_.isEmpty()) {
    qWarning() << "user:" << user_ << "online, ignore:" << user;
    return;
  }

  user_ = user;
  qInfo() << "user:" << user_ << "login";
  auth_controller_->RequestIdentifier(user);
}

void UserController::Logout() {
  QUrlQuery query_info;
  HttpRequest("logout", &query_info);
}

void UserController::OnCheckTimer() {
  auto now_ts = QTime::currentTime();
  auto iter = network_replies_.begin();
  while (iter != network_replies_.end()) {
    int delta = iter.value().msecsTo(now_ts);
    if (delta > 5 * 1000) {
      qWarning() << "user:" << user_ << "request" << iter.key()->url().path()
                 << "timeout, remove it";
      iter.key()->abort();
      iter.key()->deleteLater();
      iter = network_replies_.erase(iter);
      continue;
    }
    ++iter;
  }

  Ping();
}

void UserController::OnAuth(AuthStatus status) {
  if (status == AuthStatus::kAuthIdentifySuccess) {
    qInfo() << "user:" << user_
            << "request identifier success, start request oauth token";
    auth_controller_->RequestOAuthToken();
    return;
  }

  if (status == AuthStatus::kAuthOAuthSuccess) {
    last_active_ts_ = QTime::currentTime();
    ping_timer_.start();
  }

  QString message = "exception";
  auto msg_it = kAuthMessages.find(status);
  if (msg_it != kAuthMessages.end()) {
    message = msg_it.value();
  }

  OnLogin(message);
}

void UserController::OnNetworkReply(QNetworkReply *reply) {
  static const QMap<QString, kNetworkReplyHandler> kNetworkReplyHandlers = {
      {"logout", &UserController::OnLogout},
      {"ping", &UserController::OnPong},
  };

  auto url = reply->url();
  auto handler_it = kNetworkReplyHandlers.find(url.fileName());
  if (handler_it == kNetworkReplyHandlers.end()) {
    qWarning() << "user:" << user_ << "response:" << url.path()
               << "not found handler";
    return;
  }

  auto code = reply->error();
  auto data = reply->readAll();
  (this->*(handler_it.value()))(code, data);

  auto iter = network_replies_.find(reply);
  if (iter == network_replies_.end()) {
    qWarning() << "not found thie request history, path:" << url.path();
  } else {
    network_replies_.erase(iter);
  }
  reply->deleteLater();
}

bool UserController::HttpRequest(const QString &name, QUrlQuery *query) {
  if (network_replies_.size() > 10) {
    qWarning() << "ignore this request, too many request:"
               << network_replies_.size();
    return false;
  }

  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  auto token = settings.value("token").toString();
  settings.endGroup();

  query->addQueryItem("username", user_);

  QUrl request_url(kGumpServerBaseUrl + name);
  request_url.setQuery(*query);

  QNetworkRequest request(request_url);
  request.setRawHeader("Accesstoken", token.toLocal8Bit());

  auto *reply = network_manager_->get(request);
  network_replies_.insert(reply, QTime::currentTime());
  return true;
}

void UserController::Ping() {
  QUrlQuery query_info;
  HttpRequest("ping", &query_info);
}

void UserController::OnLogin(const QString &message) {
  qInfo() << "user:" << user_ << "login message:" << message;
  emit LoginEvent(message);
}

void UserController::OnLogout(QNetworkReply::NetworkError code,
                              const QByteArray &data) {
  (void)data;
  qInfo() << "user:" << user_ << "logout, code:" << code << ", data:" << data;
  user_.clear();
  ping_timer_.stop();
  auth_controller_->RemoveToken();
  emit LogoutEvent();
}

void UserController::OnPong(QNetworkReply::NetworkError code,
                            const QByteArray &data) {
  (void)data;
  qDebug() << "user:" << user_ << "pong, code:" << code << ", data:" << data;
  last_active_ts_ = QTime::currentTime();
}

}  // namespace gump

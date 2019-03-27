// Copyright (c) 2014-2019 winking324
//

#include "controller/oauth_controller.h"
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>

namespace gump {


OAuthController::OAuthController(QObject *parent)
  : QObject(parent) {
  QUrl authenticate_url("https://oauth.agoralab.co/oauth/authorize");
  QUrl access_token_url("https://oauth.agoralab.co/oauth/token");

  access_manager_ = new QNetworkAccessManager(this);
  oauth2_ = new QOAuth2AuthorizationCodeFlow(this);

  oauth2_->setReplyHandler(new QOAuthHttpServerReplyHandler(8163, this));
  oauth2_->setAuthorizationUrl(authenticate_url);
  oauth2_->setAccessTokenUrl(access_token_url);
}


}  // namespace gump

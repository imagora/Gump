// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QObject>
#include <QNetworkAccessManager>
#include <QOAuth2AuthorizationCodeFlow>


namespace gump {


class OAuthController :
    public QObject {
  Q_OBJECT
 public:
  explicit OAuthController(QObject *parent = nullptr);

 signals:

 public slots:

 private:
  QOAuth2AuthorizationCodeFlow *oauth2_;
  QNetworkAccessManager *access_manager_;
};


}  // namespace gump


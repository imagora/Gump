// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QWidget>
#include <QStackedLayout>

#include "view/login_widget.h"
#include "view/logging_widget.h"
#include "view/playlist_widget.h"
#include "view/play_widget.h"

#include "controller/auth_controller.h"
#include "controller/config_controller.h"


namespace gump {


class CentralWidget :
    public QWidget {
  Q_OBJECT
 public:
  explicit CentralWidget(QWidget *parent = nullptr);

 signals:

 public slots:
  void OnLogin(QString username);

  void OnAuthStatus(AuthStatus status);

  void OnPlaylistSelected();

  void OnPlayQuit();

 private:
  LoginWidget *login_widget_;
  LoggingWidget *logging_widget_;
  PlaylistWidget *playlist_widget_;
  PlayWidget *play_widget_;

  QStackedLayout *stacked_layout_;
  AuthController *auth_controller_;
  ConfigController *config_controller_;
};


}  // namespace gump

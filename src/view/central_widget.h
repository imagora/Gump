// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QStackedLayout>
#include <QWidget>

#include "controller/auth_controller.h"
#include "view/logging_widget.h"
#include "view/login_widget.h"
#include "view/menubar.h"
#include "view/play_widget.h"

namespace gump {

class CentralWidget : public QWidget {
  Q_OBJECT
 public:
  explicit CentralWidget(QWidget *parent = nullptr);

  void SearchAndPlay(const QString &info);

 signals:

 public slots:
  void OnLogin(QString username);

  void OnAuthStatus(AuthStatus status);

 private:
  MenuBar *menu_bar_;

  LoginWidget *login_widget_;
  LoggingWidget *logging_widget_;
  PlayWidget *play_widget_;

  QStackedLayout *stacked_layout_;
  AuthController *auth_controller_;
};

}  // namespace gump

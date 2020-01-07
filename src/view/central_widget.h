// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QStackedLayout>
#include <QSystemTrayIcon>
#include <QWidget>

#include "controller/user_controller.h"
#include "view/logging_widget.h"
#include "view/login_widget.h"
#include "view/play_widget.h"
#include "view/tray_menu.h"

namespace gump {

class CentralWidget : public QWidget {
  Q_OBJECT
 public:
  explicit CentralWidget(QWidget *parent = nullptr);

  void SearchAndPlay(const QString &info);

  void Close();

 signals:

 public slots:
  void OnShow(QSystemTrayIcon::ActivationReason);

  void OnStayOnTop(bool enable);

  void OnExit();

  void OnLogin(QString username);

  void OnLoginResponse(QString message);

  void OnLogout();

  void OnLogoutResponse();

  void OnSearch(QString search_info);

 private:
  QStackedLayout *stacked_layout_;
  QSystemTrayIcon *tray_icon_;
  TrayMenu *tray_menu_;

  LoginWidget *login_widget_;
  LoggingWidget *logging_widget_;
  PlayWidget *play_widget_;

  UserController *user_controller_;
};

}  // namespace gump

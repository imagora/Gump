// Copyright (c) 2014-2019 winking324
//

#include "view/central_widget.h"

#include <QApplication>
#include <QSettings>
#include <QtGlobal>

#include "commons/singleton.h"
#include "controller/player_controller.h"

namespace gump {

enum class StackedWidgetIndex : int {
  kLogin,
  kLogging,
  kPlay,
};

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
  login_widget_ = new LoginWidget(this);
  logging_widget_ = new LoggingWidget(this);
  play_widget_ = new PlayWidget(this);

  stacked_layout_ = new QStackedLayout(this);
  stacked_layout_->insertWidget(static_cast<int>(StackedWidgetIndex::kLogin),
                                login_widget_);
  stacked_layout_->insertWidget(static_cast<int>(StackedWidgetIndex::kLogging),
                                logging_widget_);
  stacked_layout_->insertWidget(static_cast<int>(StackedWidgetIndex::kPlay),
                                play_widget_);

  setLayout(stacked_layout_);

  tray_menu_ = new TrayMenu(this);
  tray_icon_ = new QSystemTrayIcon(QIcon(":/icon.png"), this);
  tray_icon_->setContextMenu(tray_menu_);
  tray_icon_->show();
  auto menus = static_cast<int>(MenuItem::kMenuLogout) |
               static_cast<int>(MenuItem::kMenuSearch) |
               static_cast<int>(MenuItem::kMenuTools);
  tray_menu_->SetMenuEnable(menus, false);

  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  QString username = settings.value("username").toString();
  settings.endGroup();
  if (!username.isEmpty()) {
    login_widget_->SetUsername(username);
  }

  user_controller_ = new UserController(this);
  connect(user_controller_, &UserController::LoginEvent, this,
          &CentralWidget::OnLoginResponse);
  connect(user_controller_, &UserController::LogoutEvent, this,
          &CentralWidget::OnLogoutResponse);

  connect(login_widget_, SIGNAL(Login(QString)), this, SLOT(OnLogin(QString)));

  connect(tray_icon_, &QSystemTrayIcon::activated, this,
          &CentralWidget::OnShow);

  connect(tray_menu_, &TrayMenu::SearchEvent, this, &CentralWidget::OnSearch);
  connect(tray_menu_, &TrayMenu::ExitEvent, this, &CentralWidget::OnExit);
  connect(tray_menu_, &TrayMenu::LogoutEvent, this, &CentralWidget::OnLogout);
  connect(tray_menu_, &TrayMenu::StayOnTopEvent, this,
          &CentralWidget::OnStayOnTop);

  auto *player_controller = Singleton<PlayerController>::Instance();
  connect(tray_menu_, &TrayMenu::StopEvent, player_controller,
          &PlayerController::OnStop);
  connect(tray_menu_, &TrayMenu::PrevEvent, player_controller,
          &PlayerController::OnPrev);
  connect(tray_menu_, &TrayMenu::NextEvent, player_controller,
          &PlayerController::OnNext);
  connect(tray_menu_, &TrayMenu::MuteEvent, player_controller,
          &PlayerController::OnMute);
}

void CentralWidget::SearchAndPlay(const QString &info) {
  qInfo() << "search and play for:" << info;
  auto *player_controller = Singleton<PlayerController>::Instance();
  player_controller->GetStream(info);
}

void CentralWidget::Close() {
  auto *player_controller = Singleton<PlayerController>::Instance();
  player_controller->OnMute(true);
  // tray_menu_->SetMenuEnable(static_cast<int>(MenuItem::kMenuTools), false);
}

void CentralWidget::OnShow(QSystemTrayIcon::ActivationReason) {
  parentWidget()->show();
  auto *player_controller = Singleton<PlayerController>::Instance();
  player_controller->OnMute(false);
}

void CentralWidget::OnStayOnTop(bool enable) {
  parentWidget()->setWindowFlag(Qt::WindowStaysOnTopHint, enable);
  parentWidget()->show();
  parentWidget()->raise();
}

void CentralWidget::OnExit() {
  qInfo() << "Gump exit";
  QApplication::setQuitOnLastWindowClosed(true);
  parentWidget()->close();
}

void CentralWidget::OnLogin(QString username) {
  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  settings.setValue("username", username);
  settings.endGroup();

  logging_widget_->SetUsername(username);
  user_controller_->Login(username);

  stacked_layout_->setCurrentIndex(
      static_cast<int>(StackedWidgetIndex::kLogging));
}

void CentralWidget::OnLoginResponse(QString message) {
  if (!message.isEmpty()) {
    login_widget_->SetErrorMessage(QString("Login failed: (%1)").arg(message));
    stacked_layout_->setCurrentIndex(
        static_cast<int>(StackedWidgetIndex::kLogin));
    return;
  }

  stacked_layout_->setCurrentIndex(static_cast<int>(StackedWidgetIndex::kPlay));

  auto menus = static_cast<int>(MenuItem::kMenuLogout) |
               static_cast<int>(MenuItem::kMenuSearch);
  tray_menu_->SetMenuEnable(menus, true);
}

void CentralWidget::OnLogout() {
  auto *player_controller = Singleton<PlayerController>::Instance();
  player_controller->OnStop();

  user_controller_->Logout();

  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  settings.remove("username");
  settings.endGroup();
}

void CentralWidget::OnLogoutResponse() {
  stacked_layout_->setCurrentIndex(
      static_cast<int>(StackedWidgetIndex::kLogin));
  auto menus = static_cast<int>(MenuItem::kMenuLogout) |
               static_cast<int>(MenuItem::kMenuSearch) |
               static_cast<int>(MenuItem::kMenuTools);
  tray_menu_->SetMenuEnable(menus, false);
}

void CentralWidget::OnSearch(QString search_info) {
  auto *player_controller = Singleton<PlayerController>::Instance();
  player_controller->SearchStream(search_info);
}

}  // namespace gump

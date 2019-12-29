// Copyright (c) 2014-2019 winking324
//

#include "view/central_widget.h"

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
  menu_bar_ = new MenuBar(this);

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

  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  QString username = settings.value("username").toString();
  settings.endGroup();
  if (!username.isEmpty()) {
    login_widget_->SetUsername(username);
  }

  auth_controller_ = new AuthController(this);
  connect(auth_controller_, SIGNAL(Status(AuthStatus)), this,
          SLOT(OnAuthStatus(AuthStatus)));

  connect(login_widget_, SIGNAL(Login(QString)), this, SLOT(OnLogin(QString)));
}

void CentralWidget::SearchAndPlay(const QString &info) {
  qInfo() << "search and play for: " << info;
  auto *player_controller = Singleton<PlayerController>::Instance();
  player_controller->GetStream(info);
}

void CentralWidget::OnLogin(QString username) {
  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  settings.setValue("username", username);
  settings.endGroup();

  logging_widget_->SetUsername(username);
  auth_controller_->RequestIdentifier(username);

  stacked_layout_->setCurrentIndex(
      static_cast<int>(StackedWidgetIndex::kLogging));
}

void CentralWidget::OnAuthStatus(AuthStatus status) {
  if (status == AuthStatus::kAuthIdentifySuccess) {
    auth_controller_->RequestOAuthToken();
    return;
  }

  if (status == AuthStatus::kAuthOAuthSuccess) {
    stacked_layout_->setCurrentIndex(
        static_cast<int>(StackedWidgetIndex::kPlay));
    return;
  }

  login_widget_->SetErrorMessage(
      QString("Error(%1): login failed").arg(static_cast<uint32_t>(status)));
  stacked_layout_->setCurrentIndex(
      static_cast<int>(StackedWidgetIndex::kLogin));
}

}  // namespace gump

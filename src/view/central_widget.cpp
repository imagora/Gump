// Copyright (c) 2014-2019 winking324
//

#include "view/central_widget.h"

#include <QtGlobal>
#include <QSettings>

#include "commons/singleton.h"
#include "controller/online_controller.h"
#include "controller/tracer_controller.h"


namespace gump {


enum class StackedWidgetIndex : int {
  kLogin = 0,
  kLogging = 1,
  kPlaylist = 2,
  kPlay = 3,
  kConfig = 4,
};


CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
  login_widget_ = new LoginWidget(this);
  logging_widget_ = new LoggingWidget(this);
  playlist_widget_ = new PlaylistWidget(this);
  play_widget_ = new PlayWidget(this);

  stacked_layout_ = new QStackedLayout(this);
  stacked_layout_->insertWidget(
        static_cast<int>(StackedWidgetIndex::kLogin), login_widget_);
  stacked_layout_->insertWidget(
        static_cast<int>(StackedWidgetIndex::kLogging), logging_widget_);
  stacked_layout_->insertWidget(
        static_cast<int>(StackedWidgetIndex::kPlaylist), playlist_widget_);
  stacked_layout_->insertWidget(
        static_cast<int>(StackedWidgetIndex::kPlay), play_widget_);

  setLayout(stacked_layout_);

  auth_controller_ = new AuthController(this);

  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  QString username = settings.value("username").toString();
  settings.endGroup();

  if (!username.isEmpty()) {
    login_widget_->SetUsername(username);
    stacked_layout_->setCurrentIndex(
          static_cast<int>(StackedWidgetIndex::kLogging));
    auth_controller_->RequestIdentifier(username);
  }

  config_controller_ = new ConfigController(this);
  auto *online_controller = Singleton<OnlineController>::Instance();
  auto *tracer_controller = Singleton<TracerController>::Instance();

  connect(login_widget_, SIGNAL(Login(QString)),
          this, SLOT(OnLogin(QString)));
  connect(auth_controller_, SIGNAL(Status(AuthStatus)),
          this, SLOT(OnAuthStatus(AuthStatus)));

  connect(config_controller_, SIGNAL(OnlineUrlEvent(QString)),
          online_controller, SLOT(OnUpdateUrl(QString)));
  connect(config_controller_, SIGNAL(TracerUrlEvent(QString)),
          tracer_controller, SLOT(OnUpdateUrl(QString)));
  connect(config_controller_, SIGNAL(TagsEvent(std::map<QString, QString>)),
          playlist_widget_, SLOT(OnTags(std::map<QString, QString>)));
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
          static_cast<int>(StackedWidgetIndex::kPlaylist));
    config_controller_->AutoRefreshConfig();
    return;
  }

  login_widget_->SetErrorMessage(
        QString("Error(%1): login failed").arg(static_cast<uint32_t>(status)));
  stacked_layout_->setCurrentIndex(
        static_cast<int>(StackedWidgetIndex::kLogin));
}


}  // namespace gump


// Copyright (c) 2014-2019 winking324
//

#include "view/central_widget.h"


namespace gump {


enum class StackedWidgetIndex : int {
  kLogin = 0,
  kLogging = 1,
  kPlaylist = 2,
  kPlayer = 3,
  kConfig = 4,
};


CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
  login_widget_ = new LoginWidget(this);
  logging_widget_ = new LoggingWidget(this);
  playlist_widget_ = new PlaylistWidget(this);

  stacked_layout_ = new QStackedLayout(this);
  stacked_layout_->insertWidget(
        static_cast<int>(StackedWidgetIndex::kLogin), login_widget_);
  stacked_layout_->insertWidget(
        static_cast<int>(StackedWidgetIndex::kLogging), logging_widget_);
  stacked_layout_->insertWidget(
        static_cast<int>(StackedWidgetIndex::kPlaylist), playlist_widget_);

  setLayout(stacked_layout_);

  connect(login_widget_, SIGNAL(Login(QString, QString)), this, SLOT(OnLogin(QString, QString)));
}

void CentralWidget::OnLogin(QString username, QString password) {
  logging_widget_->SetUsername(username);
  stacked_layout_->setCurrentIndex(
        static_cast<int>(StackedWidgetIndex::kPlaylist));
}


}  // namespace gump


// Copyright (c) 2014-2019 winking324
//


#include "view/logging_widget.h"
#include <QIcon>
#include <QDebug>
#include <QGridLayout>


namespace gump {


LoggingWidget::LoggingWidget(QWidget *parent)
  : QWidget(parent) {
  auto *layout = new QGridLayout(this);
  layout->setAlignment(Qt::AlignCenter);

  icon_label_ = new QLabel(this);
  icon_label_->setAlignment(Qt::AlignCenter);
  icon_label_->setPixmap(QIcon(":/username.png").pixmap(32, 32));

  username_label_ = new QLabel(this);
  username_label_->setAlignment(Qt::AlignCenter);

  layout->addWidget(icon_label_, 0, 0);
  layout->addWidget(username_label_, 1, 0);

  setLayout(layout);

  current_index_ = 0;
  update_timer_ = new QTimer(this);
  update_timer_->setInterval(500);

  connect(update_timer_, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void LoggingWidget::SetUsername(const QString &username) {
  username_label_->setText(username);
}

void LoggingWidget::OnTimer() {
  if (current_index_ == 0) {
    current_index_ = 1;
    icon_label_->setPixmap(QIcon(":/username2.png").pixmap(32, 32));
  } else {
    current_index_ = 0;
    icon_label_->setPixmap(QIcon(":/username.png").pixmap(32, 32));
  }
}

void LoggingWidget::hideEvent(QHideEvent *) {
  qInfo() << "hide event";
  update_timer_->stop();
}

void LoggingWidget::showEvent(QShowEvent *event) {
  qInfo() << "show event";
  update_timer_->start();
}


}  // namespace gump

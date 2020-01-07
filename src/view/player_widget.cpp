// Copyright (c) 2014-2019 winking324
//

#include "view/player_widget.h"

#include <QtAVWidgets/global.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QPalette>
#include <QScreen>
#include <QStackedLayout>
#include <QTimer>

#include "commons/definations.h"
#include "commons/singleton.h"
#include "controller/player_controller.h"

namespace gump {

PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent) {
  is_show_details_ = false;

  QHBoxLayout *main_layout = new QHBoxLayout();
  QStackedLayout *stacked_layout = new QStackedLayout();
  stacked_layout->setMargin(2);
  stacked_layout->setStackingMode(QStackedLayout::StackAll);

  video_output_ = new QtAV::VideoOutput(QtAV::VideoRendererId_Widget, this);
  if (!video_output_->widget()) {
    qCritical() << "Error: can not create video renderer retina";
    return;
  }

  //  player_status_ = new QLabel(PlayerStatus(), this);
  player_status_ = new QLabel("", this);
  QPalette pe;
  pe.setColor(QPalette::WindowText, Qt::red);
  player_status_->setPalette(pe);
  player_status_->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  stacked_layout->addWidget(player_status_);
  stacked_layout->addWidget(video_output_->widget());
  //  setFixedWidth(30);

  main_layout->addLayout(stacked_layout);
  setLayout(main_layout);
  player_status_->raise();

  //  QTimer::singleShot(2000, this, SLOT(RefreshMediaInfoTimer()));
}

void PlayerWidget::ShowDetails() { is_show_details_ = !is_show_details_; }

void PlayerWidget::OnStatusChanged(QString status) {
  player_status_->setText(status);
  //  player_status_->raise();
}

void PlayerWidget::showEvent(QShowEvent *) {
  auto *player_controller = Singleton<PlayerController>::Instance();
  player_controller->SetRenderer(video_output_);
  OnStatusChanged(player_controller->GetCurrentStatus());
  connect(player_controller, SIGNAL(StatusChangeEvent(QString)), this,
          SLOT(OnStatusChanged(QString)));
}

void PlayerWidget::hideEvent(QHideEvent *) {
  auto *player_controller = Singleton<PlayerController>::Instance();
  disconnect(player_controller, SIGNAL(StatusChangeEvent(QString)), this,
             SLOT(OnStatusChanged(QString)));
}

}  // namespace gump

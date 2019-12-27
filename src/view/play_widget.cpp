// Copyright (c) 2014-2019 winking324
//

#include "view/play_widget.h"

#include <QDebug>
#include <QVBoxLayout>

#include "commons/singleton.h"
#include "controller/player_controller.h"

namespace gump {

PlayWidget::PlayWidget(QWidget *parent) : QWidget(parent) {
  player_ = new PlayerWidget(this);
  player_->setFixedSize(QSize(360, 640));
}

void PlayWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    emit QuitPlayEvent();
  }
}

void PlayWidget::mouseDoubleClickEvent(QMouseEvent *) { emit QuitPlayEvent(); }

void PlayWidget::showEvent(QShowEvent *) {}

}  // namespace gump

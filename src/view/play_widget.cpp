// Copyright (c) 2014-2019 winking324
//

#include "view/play_widget.h"

#include <QDebug>

#include "commons/singleton.h"
#include "controller/player_controller.h"
#include "controller/online_controller.h"
#include "controller/stream_rule_controller.h"


namespace gump {


PlayWidget::PlayWidget(QWidget *parent) : QWidget(parent) {
  player_ = new PlayerWidget(this);
  player_->setFixedSize(QSize(360, 640));
}

void PlayWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    emit QuitPlayEvent();
  }

  if (event->key() == Qt::Key_Down) {
    auto *player_controller = Singleton<PlayerController>::Instance();
    QString next_stream;

    QString stream = player_controller->GetCurrentStream();
    Singleton<OnlineController>::Instance()->GetNextStream(
          stream, &next_stream, nullptr);
    if (!next_stream.isEmpty()) {
      player_controller->PlayStream(
            Singleton<StreamRuleController>::Instance()->ConvertToPlayUrl(
              next_stream));
    }

    stream = player_controller->GetCurrentStream();
    Singleton<OnlineController>::Instance()->GetNextStream(
          stream, &next_stream, nullptr);
    if (!next_stream.isEmpty()) {
      player_controller->BufferStream(
            Singleton<StreamRuleController>::Instance()->ConvertToPlayUrl(
              next_stream));
    }
  }
}

void PlayWidget::mouseDoubleClickEvent(QMouseEvent *) {
  emit QuitPlayEvent();
}

void PlayWidget::showEvent(QShowEvent *) {
}


}  // namespace gump

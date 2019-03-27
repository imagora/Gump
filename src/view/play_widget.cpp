// Copyright (c) 2014-2019 winking324
//

#include "view/play_widget.h"


namespace gump {


PlayWidget::PlayWidget(QWidget *parent) : QWidget(parent) {
  player_ = new PlayerWidget(this);
  player_->setFixedSize(QSize(360, 640));
}


}  // namespace gump

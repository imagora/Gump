// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QWidget>
#include "view/player_widget.h"


namespace gump {


class PlayWidget : public QWidget {
  Q_OBJECT
 public:
  explicit PlayWidget(QWidget *parent = nullptr);

 signals:

 public slots:

 private:
  PlayerWidget *player_;
};


}  // namespace gump

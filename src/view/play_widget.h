// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QMouseEvent>
#include <QWidget>

#include "view/player_widget.h"

namespace gump {

class PlayWidget : public QWidget {
  Q_OBJECT
 public:
  explicit PlayWidget(QWidget *parent = nullptr);

 signals:
  void QuitPlayEvent();

  void PrevEvent();

  void NextEvent();

 public slots:

 protected:
  virtual void keyReleaseEvent(QKeyEvent *event) override;

 private:
  PlayerWidget *player_;
};

}  // namespace gump

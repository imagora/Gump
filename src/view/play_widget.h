// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QWidget>
#include <QMouseEvent>

#include "view/player_widget.h"


namespace gump {


class PlayWidget : public QWidget {
  Q_OBJECT
 public:
  explicit PlayWidget(QWidget *parent = nullptr);

 signals:
  void QuitPlayEvent();

 public slots:

 protected:
  virtual void keyReleaseEvent(QKeyEvent *event) override;

  virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

  virtual void showEvent(QShowEvent *event) override;

 private:
  PlayerWidget *player_;
};


}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QMainWindow>

#include "view/central_widget.h"
#include "view/system_tray.h"

namespace gump {

class GumpWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit GumpWindow(QWidget *parent = nullptr);

 protected:
  virtual bool event(QEvent *event) override;

 private:
  CentralWidget *central_widget_;
  SystemTray *system_tray_;
};

}  // namespace gump

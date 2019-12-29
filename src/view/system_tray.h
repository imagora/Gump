// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

namespace gump {

class SystemTray : public QWidget {
  Q_OBJECT
 public:
  SystemTray(QWidget *parent = nullptr);

 public slots:
  void OnExit();

 private:
  QAction *exit_action_;

  QMenu *tray_menu_;
  QSystemTrayIcon *tray_icon_;
};

}  // namespace gump

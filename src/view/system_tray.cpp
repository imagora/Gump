// Copyright (c) 2014-2019 winking324
//

#include "view/system_tray.h"

#include <QApplication>

namespace gump {

SystemTray::SystemTray(QWidget *parent) : QWidget(parent) {
  tray_icon_ = new QSystemTrayIcon(QIcon(":/icon.png"), this);
  tray_menu_ = new QMenu(this);
  exit_action_ = new QAction("Exit", this);

  tray_icon_->setContextMenu(tray_menu_);
  tray_menu_->addAction(exit_action_);

  connect(tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          parent, SLOT(show()));
  connect(exit_action_, SIGNAL(triggered()), this, SLOT(OnExit()));

  tray_icon_->show();
}

void SystemTray::OnExit() {
  QApplication::setQuitOnLastWindowClosed(true);
  parentWidget()->close();
}

}  // namespace gump

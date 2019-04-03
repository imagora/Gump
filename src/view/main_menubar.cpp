// Copyright (c) 2014-2019 winking324
//

#include "view/main_menubar.h"


namespace gump {


enum MenuType {
  kMenuWindow = 2,
  kMenuHelp = 3,
};


static const std::map<uint32_t, QString> kMenuName {
  {kMenuWindow, "Window"},
  {kMenuHelp, "Help"},
};


MainMenuBar::MainMenuBar(QWidget *parent)
    : QMenuBar(parent) {
  for (auto &menu_info : kMenuName) {
    menus_.insert(std::make_pair(menu_info.first,
                                 new QMenu(menu_info.second, this)));
  }

  InitWindowMenu();
  InitHelpMenu();

  for (auto &menuItem : menus_) {
    addMenu(menuItem.second);
  }
}

MainMenuBar::~MainMenuBar() {
}

void MainMenuBar::InitWindowMenu() {
  QMenu *windowMenu = menus_[kMenuWindow];
  windowMenu->addAction("Minimize", this, SLOT(showMinimized()));
}

void MainMenuBar::InitHelpMenu() {
  QMenu *helpMenu = menus_[kMenuHelp];
  helpMenu->addAction("Help", this, SLOT(OpenAboutDlg()));
  helpMenu->addAction("About", this, SLOT(OpenAboutDlg()));
}

void MainMenuBar::OpenAboutDlg() {
  about_dlg_ = new AboutDialog(this);
  about_dlg_->show();
}


}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#include "view/menubar.h"

namespace gump {

using MenuItemInitFuncPtr = void (MenuBar::*)(QMenu *);
struct MenuItemInfo {
  QString item_name;
  MenuItemInitFuncPtr init_func;
};

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent) {
  static const QMap<MenuItem, MenuItemInfo> kMenuItems{
      {MenuItem::kMenuUser, {"User", &MenuBar::InitUserMenu}},
      {MenuItem::kMenuWindow, {"Window", &MenuBar::InitWindowMenu}},
      {MenuItem::kMenuHelp, {"Help", &MenuBar::InitHelpMenu}},
  };

  for (auto iter = kMenuItems.begin(); iter != kMenuItems.end(); ++iter) {
    auto *new_menu = new QMenu(iter.value().item_name, this);
    menus_.insert(iter.key(), new_menu);
    (this->*(iter.value().init_func))(new_menu);
    addMenu(new_menu);
  }
}

MenuBar::~MenuBar() {}

void MenuBar::InitUserMenu(QMenu *menu) {
  menu->addAction("Logout", this, SLOT(OnUserQuit()));
}

void MenuBar::InitWindowMenu(QMenu *menu) {
  menu->addAction("Minimize", this, SLOT(showMinimized()));
}

void MenuBar::InitHelpMenu(QMenu *menu) {
  menu->addAction("About", this, SLOT(OnOpenAboutDlg()));
}

void MenuBar::OnUserQuit() {}

void MenuBar::OnOpenAboutDlg() {
  about_dlg_ = new AboutDialog(this);
  about_dlg_->show();
}

}  // namespace gump

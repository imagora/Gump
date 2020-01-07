// Copyright (c) 2014-2019 winking324
//

#include "view/tray_menu.h"

#include <QDebug>

namespace gump {

using MenuItemInitFuncPtr = void (TrayMenu::*)(QAction *);
struct MenuItemInfo {
  QString item_name;
  MenuItemInitFuncPtr init_func;
};

TrayMenu::TrayMenu(QWidget *parent) : QMenu(parent) {
  static const QMap<MenuItem, MenuItemInfo> kMenuItems{
      {MenuItem::kMenuStayOnTop, {"Stay On Top", &TrayMenu::InitStayOnTopMenu}},
      {MenuItem::kMenuSearch, {"Search", &TrayMenu::InitSearchMenu}},
      {MenuItem::kMenuPlayControl,
       {"Play Control", &TrayMenu::InitPlayControlMenu}},
      {MenuItem::kMenuTools, {"Tools", &TrayMenu::InitToolsMenu}},
      {MenuItem::kMenuLogout, {"Logout", &TrayMenu::InitLogoutMenu}},
      {MenuItem::kMenuAbout, {"About", &TrayMenu::InitAboutMenu}},
      {MenuItem::kMenuExit, {"Exit", &TrayMenu::InitExitMenu}},
  };

  for (auto iter = kMenuItems.begin(); iter != kMenuItems.end(); ++iter) {
    auto *new_action = new QAction(iter.value().item_name, this);
    menus_.insert(iter.key(), new_action);
    (this->*(iter.value().init_func))(new_action);
    addAction(new_action);
  }
}

TrayMenu::~TrayMenu() {}

void TrayMenu::SetMenuEnable(int menus, bool enable) {
  for (auto iter = menus_.begin(); iter != menus_.end(); ++iter) {
    if (static_cast<int>(iter.key()) & menus) {
      iter.value()->setEnabled(enable);
      qInfo() << "set menu:" << iter.value()->text()
              << (enable ? "enable" : "disable");
    }
  }
}

void TrayMenu::InitStayOnTopMenu(QAction *action) {
  action->setCheckable(true);
  connect(action, &QAction::toggled, this, &TrayMenu::StayOnTopEvent);
}

void TrayMenu::InitLogoutMenu(QAction *action) {
  connect(action, &QAction::triggered, this, &TrayMenu::LogoutEvent);
}

void TrayMenu::InitSearchMenu(QAction *action) {
  auto *menu = new QMenu(this);
  menu->addAction("Search ...", this,
                  [this]() { OpenSearchDlg(SearchType::kSearchAll); });
  menu->addAction("Search Channel Name", this,
                  [this]() { OpenSearchDlg(SearchType::kSearchCName); });
  menu->addAction("Search User ID", this,
                  [this]() { OpenSearchDlg(SearchType::kSearchUid); });
  menu->addAction("Search Server IP", this,
                  [this]() { OpenSearchDlg(SearchType::kSearchIp); });
  menu->addAction("Search Stream Url", this,
                  [this]() { OpenSearchDlg(SearchType::kSearchUrl); });
  action->setMenu(menu);
}

void TrayMenu::InitPlayControlMenu(QAction *action) {
  auto *menu = new QMenu(this);
  menu->addAction("Stop", this, [this]() {});
  menu->addAction("Next", this, [this]() {});
  menu->addAction("Prev", this, [this]() {});
  auto mute_action = menu->addAction("Mute", this, [this]() {});
  mute_action->setChecked(true);
  auto media_action = menu->addAction("Media Info", this, [this]() {});
  media_action->setChecked(true);
  action->setMenu(menu);
}

void TrayMenu::InitToolsMenu(QAction *action) {
  auto *menu = new QMenu(this);
  menu->addAction("Argus", this, &TrayMenu::OpenArgusEvent);
  menu->addAction("Stream Info", this, []() {});
  action->setMenu(menu);
}

void TrayMenu::InitAboutMenu(QAction *action) {
  connect(action, &QAction::triggered, this, [this]() { OpenAboutDlg(); });
}

void TrayMenu::InitExitMenu(QAction *action) {
  connect(action, &QAction::triggered, this, &TrayMenu::ExitEvent);
}

void TrayMenu::OpenAboutDlg() {
  about_dlg_ = new AboutDialog(parentWidget());
  about_dlg_->show();
}

void TrayMenu::OpenSearchDlg(SearchType type) {
  SearchDialog dialog(type, parentWidget());
  if (dialog.exec() == QDialog::Accepted) {
    QString search_info = dialog.SearchInfo();
    if (!search_info.isEmpty()) {
      qInfo() << "search for:" << static_cast<int>(type)
              << ", detail:" << search_info;
      emit SearchEvent(search_info);
    }
  }
}

}  // namespace gump

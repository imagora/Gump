// Copyright (c) 2014-2019 winking324
//

#include "view/main_menubar.h"

#include "view/main_window.h"

namespace gump {

enum MenuType {
  kMenuSettings = 1,
  kMenuWindow = 2,
  kMenuHelp = 3,
};

static const std::map<uint32_t, QString> kMenuName{
    {kMenuSettings, "Settings"},
    {kMenuWindow, "Window"},
    {kMenuHelp, "Help"},
};

MainMenuBar::MainMenuBar(QWidget *parent) : QMenuBar(parent) {
  for (auto &menu_info : kMenuName) {
    menus_.insert(
        std::make_pair(menu_info.first, new QMenu(menu_info.second, this)));
  }

  InitSettingsMenu();
  InitWindowMenu();
  InitHelpMenu();

  for (auto &menuItem : menus_) {
    addMenu(menuItem.second);
  }
}

MainMenuBar::~MainMenuBar() {}

void MainMenuBar::InitSettingsMenu() {
  QMenu *settingsMenu = menus_[kMenuSettings];
  settingsMenu->addAction("preferences", this, SLOT(OpenPreferencesDlg()));
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

void MainMenuBar::OpenPreferencesDlg() {
  preferences_dlg_ = new PreferencesDialog(this);
  connect(preferences_dlg_, SIGNAL(UpdatePreferences()),
          dynamic_cast<MainWindow *>(parentWidget()),
          SLOT(UpdatePreferences()));
  preferences_dlg_->show();
}

}  // namespace gump

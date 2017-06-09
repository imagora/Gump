#include "main_menubar.h"
#include "about_dialog.h"
#include "preferences_dialog.h"
#include "main_window.h"
#include <QMenu>


using namespace gump;


static const std::map<uint32_t, QString> MENU_NAME {
  {MENU_SETTINGS, "Settings"},
  {MENU_WINDOW, "Window"},
  {MENU_HELP, "Help"},
};


MainMenuBar::MainMenuBar(QWidget *parent/* = nullptr*/)
  : QMenuBar(parent)
{
  for (auto &menuInfo : MENU_NAME) {
    menus_.insert(std::make_pair(menuInfo.first, new QMenu(menuInfo.second, this)));
  }

  InitSettingsMenu();
  InitWindowMenu();
  InitHelpMenu();

  for (auto &menuItem : menus_) {
    addMenu(menuItem.second);
  }
}


MainMenuBar::~MainMenuBar()
{

}


void MainMenuBar::InitSettingsMenu()
{
  QMenu *settingsMenu = menus_[MENU_SETTINGS];
  settingsMenu->addAction("preferences", this, SLOT(OpenPreferencesDlg()));
}


void MainMenuBar::InitWindowMenu()
{
  QMenu *windowMenu = menus_[MENU_WINDOW];
  windowMenu->addAction("Minimize", this, SLOT(showMinimized()));
}


void MainMenuBar::InitHelpMenu()
{
  QMenu *helpMenu = menus_[MENU_HELP];
  helpMenu->addAction("Help", this, SLOT(OpenAboutDlg()));
  helpMenu->addAction("About", this, SLOT(OpenAboutDlg()));
}

void MainMenuBar::OpenAboutDlg()
{
  about_dlg_ = new AboutDialog(this);
  about_dlg_->show();
}

void MainMenuBar::OpenPreferencesDlg()
{
  preferences_dlg_ = new PreferencesDialog(this);
  connect(preferences_dlg_, SIGNAL(UpdatePreferences()),
          dynamic_cast<MainWindow *>(parentWidget()), SLOT(UpdatePreferences()));
  preferences_dlg_->show();
}


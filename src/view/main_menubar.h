#pragma once
#include <map>
#include <QMenuBar>


class QMenu;


namespace gump {


class AboutDialog;
class PreferencesDialog;


enum MenuType {
  MENU_SETTINGS   = 1,
  MENU_WINDOW     = 2,
  MENU_HELP       = 3,
};


class MainMenuBar : public QMenuBar
{
  Q_OBJECT

 public:
  MainMenuBar(QWidget *parent = nullptr);
  ~MainMenuBar();

 private:
  void InitSettingsMenu();
  void InitWindowMenu();
  void InitHelpMenu();

 private slots:
  void OpenAboutDlg();
  void OpenPreferencesDlg();

 private:
  std::map<uint32_t, QMenu*>   menus_;
  AboutDialog *about_dlg_;
  PreferencesDialog *preferences_dlg_;
};


}


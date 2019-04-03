// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <map>
#include <QMenu>
#include <QMenuBar>

#include "view/about_dialog.h"
//#include "view/preferences_dialog.h"


namespace gump {


class MainMenuBar : public QMenuBar {
  Q_OBJECT

 public:
  explicit MainMenuBar(QWidget *parent = nullptr);

  virtual ~MainMenuBar();

 private:
  void InitWindowMenu();

  void InitHelpMenu();

 private slots:
  void OpenAboutDlg();

 private:
  std::map<uint32_t, QMenu*> menus_;
  AboutDialog *about_dlg_;
};


}


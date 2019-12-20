// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QMap>
#include <QMenu>
#include <QMenuBar>

#include "view/about_dialog.h"

namespace gump {

enum class MenuItem : int {
  kMenuUser,
  kMenuWindow,
  kMenuHelp,
};

class MenuBar : public QMenuBar {
  Q_OBJECT

 public:
  explicit MenuBar(QWidget *parent = nullptr);

  virtual ~MenuBar();

 private:
  void InitUserMenu(QMenu *menu);

  void InitWindowMenu(QMenu *menu);

  void InitHelpMenu(QMenu *menu);

 private slots:
  void OnUserQuit();

  void OnOpenAboutDlg();

 private:
  QMap<MenuItem, QMenu *> menus_;
  AboutDialog *about_dlg_;
};

}  // namespace gump

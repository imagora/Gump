// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QMap>
#include <QMenu>

#include "view/about_dialog.h"
#include "view/search_dialog.h"

namespace gump {

enum class MenuItem : int {
  kMenuStayOnTop = (1 << 0),
  kMenuSearch = (1 << 1),
  kMenuPlayControl = (1 << 2),
  kMenuTools = (1 << 3),
  kMenuLogout = (1 << 4),
  kMenuAbout = (1 << 5),
  kMenuExit = (1 << 6),
};

class TrayMenu : public QMenu {
  Q_OBJECT

 public:
  explicit TrayMenu(QWidget *parent = nullptr);

  virtual ~TrayMenu();

  void SetMenuEnable(int menus, bool enable);

 signals:
  void StayOnTopEvent(bool enable);

  void OpenArgusEvent();

  void SearchEvent(QString search_info);

  void LogoutEvent();

  void ExitEvent();

 private:
  void InitStayOnTopMenu(QAction *action);

  void InitLogoutMenu(QAction *action);

  void InitSearchMenu(QAction *action);

  void InitPlayControlMenu(QAction *action);

  void InitToolsMenu(QAction *action);

  void InitAboutMenu(QAction *action);

  void InitExitMenu(QAction *action);

  void OpenAboutDlg();

  void OpenSearchDlg(SearchType type);

 private slots:

 private:
  QMap<MenuItem, QAction *> menus_;
  AboutDialog *about_dlg_;
};

}  // namespace gump

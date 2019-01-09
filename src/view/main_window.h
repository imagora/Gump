// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QMainWindow>

#include "view/log_dock.h"
#include "view/main_menubar.h"
#include "view/main_toolbar.h"
#include "view/main_central_widget.h"
#include "controller/log_dock_appender.h"


namespace gump {


class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);

  virtual ~MainWindow();

 public slots:
  void UpdatePreferences();

  void SearchItem(std::string vid, std::string cname, std::string stream);

  void PlayStream();

  void PauseStream();

  void StopStream();

  void ShowDetails();

 protected:
  virtual void closeEvent(QCloseEvent *event) override;

  virtual void moveEvent(QMoveEvent *event) override;

 private:
  LogDock *log_dock_;
  log4cplus::SharedAppenderPtr log_appender_;
  MainMenuBar *menu_;
  MainToolBar *toolbar_;
  MainCentralWidget *central_widget_;
};


}  // namespace gump

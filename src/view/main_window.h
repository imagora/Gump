#pragma once
#include <QMainWindow>
#include "controller/log_dock_appender.h"


namespace gump {


class LogDock;
class LogDockAppender;
class MainMenuBar;
class MainToolBar;
class MainCentralWidget;


class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

 public slots:
  void UpdatePreferences();

  void SearchItem(std::string vid, std::string cname, std::string stream);

  void PlayStream();

  void PauseStream();

  void StopStream();

  void ShowDetails();

protected:
  void closeEvent(QCloseEvent *event) override;

  void moveEvent(QMoveEvent *event) override;

 private:
  LogDock *log_dock_;
  log4cplus::SharedAppenderPtr log_appender_;
  MainMenuBar *menu_;
  MainToolBar *toolbar_;
  MainCentralWidget *central_widget_;
};


}

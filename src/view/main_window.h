#pragma once
#include <QMainWindow>


namespace gump {


class MainMenuBar;
class MainCentralWidget;


class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

 public slots:
  void UpdatePreferences();

 private:
  MainMenuBar *menu_;
  MainCentralWidget *central_widget_;
};


}

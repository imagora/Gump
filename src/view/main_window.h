#pragma once
#include <QMainWindow>


namespace gump {


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

 private:
  MainMenuBar *menu_;
  MainToolBar *toolbar_;
  MainCentralWidget *central_widget_;
};


}

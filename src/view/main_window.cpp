#include "main_window.h"
#include "main_menubar.h"
#include "main_central_widget.h"
#include <QIcon>


using namespace gump;


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  setWindowTitle("Gump");
  setWindowIcon(QIcon(":/icon.png"));

  menu_ = new MainMenuBar(this);
  central_widget_ = new MainCentralWidget();
  setCentralWidget(central_widget_);

  sizeHint();
  //setWindowState(Qt::WindowMaximized);
}


MainWindow::~MainWindow()
{

}

void MainWindow::UpdatePreferences()
{
  central_widget_->UpdateStreamRule();
}



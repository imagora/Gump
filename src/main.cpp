#include <QApplication>
#include "view/main_window.h"
#include <QtAVWidgets/QtAVWidgets>


int main(int argc, char *argv[])
{
  QtAV::Widgets::registerRenderers();
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/icon.png"));

  gump::MainWindow w;
  w.show();
  return a.exec();
}

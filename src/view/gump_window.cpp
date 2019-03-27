// Copyright (c) 2014-2019 winking324
//

#include "view/gump_window.h"


namespace gump {


GumpWindow::GumpWindow(QWidget *parent) : QMainWindow(parent) {
  central_widget_ = new CentralWidget(this);

  setCentralWidget(central_widget_);
  setFixedHeight(640);
  setFixedWidth(360);
}


}  // namespace gump

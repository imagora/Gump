// Copyright (c) 2014-2019 winking324
//

#include "view/gump_window.h"

#include "controller/search_event.h"

namespace gump {

GumpWindow::GumpWindow(QWidget *parent) : QMainWindow(parent) {
  central_widget_ = new CentralWidget(this);

  setCentralWidget(central_widget_);
  setFixedHeight(640);
  setFixedWidth(360);
}

bool GumpWindow::event(QEvent *event) {
  if (event->type() == kSearchEventType) {
    SearchEvent *search_event = dynamic_cast<SearchEvent *>(event);
    central_widget_->SearchAndPlay(search_event->GetSearchInfo());
    return true;
  }

  return QMainWindow::event(event);
}

}  // namespace gump

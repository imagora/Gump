// Copyright (c) 2014-2019 winking324
//

#include "view/gump_window.h"

#include "commons/singleton.h"
#include "controller/search_event.h"
#include "controller/update_controller.h"

namespace gump {

GumpWindow::GumpWindow(QWidget *parent) : QMainWindow(parent) {
  central_widget_ = new CentralWidget(this);

  setCentralWidget(central_widget_);
  setFixedHeight(640);
  setFixedWidth(360);

  auto *update_controller = Singleton<UpdateController>::Instance();
  update_controller->StartCheckForUpdatesTimer();
}

bool GumpWindow::event(QEvent *event) {
  if (event->type() == kSearchEventType) {
    SearchEvent *search_event = dynamic_cast<SearchEvent *>(event);
    central_widget_->SearchAndPlay(search_event->GetSearchInfo());
    return true;
  }

  if (event->type() == QEvent::Close) {
    central_widget_->Close();
    hide();
    return true;
  }

  return QMainWindow::event(event);
}

}  // namespace gump

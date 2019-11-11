// Copyright (c) 2014-2019 winking324
//

#include "controller/application.h"
#include <QFileOpenEvent>
#include <QDebug>

#include "controller/search_event.h"


namespace gump {


Application::Application(int &argc, char **argv) : QApplication (argc, argv) {
  event_receiver_ = nullptr;
}

void Application::SetEventReceiver(QObject *obj) {
  event_receiver_ = obj;
}

bool Application::event(QEvent *event) {
  if (event->type() == QEvent::FileOpen && event_receiver_ != nullptr) {
    QFileOpenEvent *open_event = dynamic_cast<QFileOpenEvent *>(event);
    SearchEvent search_event(open_event->url());

    if (notify(event_receiver_, &search_event)) {
      return true;
    }
  }

  return QApplication::event(event);
}


}  // namespace gump

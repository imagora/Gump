// Copyright (c) 2014-2019 winking324
//

#include "controller/tracer_controller.h"
#include <QDesktopServices>


namespace gump {


TracerController::TracerController(QObject *parent) : QObject(parent) {
}

void TracerController::OpenTracer(const QString &id, const QString &name) {
  QString url = tracer_url_ + "?vids=" + id + "&cname=" + name;
  QDesktopServices::openUrl(QUrl(url));
}

void TracerController::OnUpdateUrl(QString url) {
  tracer_url_ = url;
}


}  // namespace gump

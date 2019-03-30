// Copyright (c) 2014-2019 winking324
//

#include "controller/online_controller.h"

#include <QDebug>


namespace gump {


OnlineController::OnlineController(QObject *parent)
  : QObject(parent) {
}

void OnlineController::UpdateInfo(const QString &name, const QString &info) {
  qInfo() << "request online: " << name << ", " << info;
}

void OnlineController::OnUpdateUrl(QString url) {
  online_url_ = url;
}

void OnlineController::OnNetworkReply(QNetworkReply *reply) {

}


}  // namespace gump

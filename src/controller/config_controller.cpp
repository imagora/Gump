// Copyright (c) 2014-2019 winking324
//

#include "controller/config_controller.h"

#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include "commons/singleton.h"
#include "controller/stream_rule_controller.h"


namespace gump {


static const QUrl kConfigUrl("https://gump.agoralab.co/gump/update");


ConfigController::ConfigController(QObject *parent)
  : QObject(parent), config_request_(kConfigUrl) {
  network_manager_ = new QNetworkAccessManager(this);

  connect(network_manager_, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(OnNetworkReply(QNetworkReply*)));
}

void ConfigController::AutoRefreshConfig(int refresh_timer) {
  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  QString token = settings.value("token").toString();
  settings.endGroup();

  refresh_timer_ = refresh_timer;
  config_request_.setRawHeader("Accesstoken", token.toLocal8Bit());
  RequestConfig();
}

void ConfigController::OnNetworkReply(QNetworkReply *reply) {
  QTimer::singleShot(refresh_timer_ * 1000, this, SLOT(RequestConfig()));

  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError || !doc.isObject()) {
    qCritical() << "cannot parse config response" << data;
  } else {
    QJsonObject obj = doc.object();
    auto status_iter = obj.find("status");
    if (status_iter.value().type() != QJsonValue::String ||
        status_iter.value().toString() != "success") {
      qCritical() << "request config failed, status error";
      return;
    }

    auto rules_iter = obj.find("rules");
    if (rules_iter.value().type() != QJsonValue::Object) {
      qCritical() << "request config failed, rules error";
    } else {
      Singleton<StreamRuleController>::Instance()->UpdateRules(
            rules_iter.value().toObject());
    }

    auto tags_iter = obj.find("tags");
    if (tags_iter.value().type() != QJsonValue::Object) {
      qCritical() << "request config failed, tags error";
    } else {
      QJsonObject tags_obj = tags_iter.value().toObject();
      std::map<QString, QString> tags;
      for (auto tag_it = tags_obj.begin(); tag_it != tags_obj.end(); ++tag_it) {
        tags[tag_it.key()] = tag_it.value().toString();
      }
      emit TagsEvent(tags);
    }

    auto online_iter = obj.find("online_url");
    if (online_iter.value().type() != QJsonValue::String ||
        online_iter.value().toString().isEmpty()) {
      qCritical() << "request config failed, online url error";
    } else {
      emit OnlineUrlEvent(online_iter.value().toString());
    }

    auto tracer_iter = obj.find("tracer_url");
    if (tracer_iter.value().type() != QJsonValue::String ||
        tracer_iter.value().toString().isEmpty()) {
      qCritical() << "request config failed, tracer url error";
    } else {
      emit TracerUrlEvent(tracer_iter.value().toString());
    }
  }
}

void ConfigController::RequestConfig() {
  network_manager_->get(config_request_);
}


}  // namespace gump

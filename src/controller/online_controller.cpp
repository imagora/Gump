// Copyright (c) 2014-2019 winking324
//

#include "controller/online_controller.h"

#include <QDebug>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


namespace gump {


OnlineController::OnlineController(QObject *parent)
  : QObject(parent) {
  network_reply_ = nullptr;
  network_manager_ = new QNetworkAccessManager(this);
  request_timer_ = new QTimer();

  connect(network_manager_, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(OnNetworkReply(QNetworkReply*)));
  connect(request_timer_, SIGNAL(timeout()), this, SLOT(OnRequestOnline()));
}

void OnlineController::UpdateInfo(const QString &name, const QString &info) {
  qInfo() << "request online: " << name << "[" << info << "]";
  query_info_.clear();
  if (info.isEmpty()) {
    request_timer_->stop();
    emit UpdateList(MultiStreams());
    return;
  }

  query_info_.addQueryItem("page_size", "10000");
  query_info_.addQueryItem("vid", info);

  if (!request_timer_->isActive()) {
    request_timer_->start(60 * 1000);
  }

  RequestOnline();
}

void OnlineController::OnUpdateUrl(QString url) {
  online_url_.setUrl(url);
}

void OnlineController::OnNetworkReply(QNetworkReply *reply) {
  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    qCritical() << "parse online response error";
    return;
  }

  MultiStreams multi_streams;
  if (doc.isObject()) {
    QJsonObject obj = doc.object();
    auto success_iter = obj.find("success");
    if (success_iter.value().type() != QJsonValue::Bool ||
        !success_iter.value().toBool()) {
      qCritical() << "request online failed, no success";
      return;
    }

    auto data_iter = obj.find("data");
    if (data_iter.value().type() != QJsonValue::Object) {
      qCritical() << "request online failed, no data";
      return;
    }

    QJsonObject data = data_iter.value().toObject();
    int total_count = data.find("total_count").value().toInt();
    qInfo() << "request online success, stream count: " << total_count;

    QJsonArray streams = data.find("streams").value().toArray();
    foreach (QJsonValue stream_value, streams) {
      QJsonObject stream = stream_value.toObject();
      QString cname = stream.find("cname").value().toString();

      Stream stream_info;
      stream_info.ip =
          static_cast<uint32_t>(stream.find("lbes_ip").value().toInt());
      stream_info.mode =
          static_cast<uint32_t>(stream.find("mode").value().toInt());
      stream_info.status =
          static_cast<uint32_t>(stream.find("status").value().toInt());
      stream_info.user_id =
          static_cast<uint32_t>(stream.find("uid").value().toInt());
      stream_info.create_ts =
          static_cast<uint32_t>(stream.find("create").value().toInt());
      stream_info.stream_url = QUrl(stream.find("url").value().toString());

      multi_streams[cname].push_back(stream_info);
    }
  }

  emit UpdateList(multi_streams);
}

void OnlineController::OnRequestOnline() {
  if (network_reply_ != nullptr && network_reply_->isRunning()) {
    return;
  }

  RequestOnline();
}

void OnlineController::RequestOnline() {
  if (network_reply_ != nullptr && network_reply_->isRunning()) {
    network_reply_->abort();
    network_reply_ = nullptr;
  }

  online_url_.setQuery(query_info_);
  network_reply_ = network_manager_->get(QNetworkRequest(online_url_));
}


}  // namespace gump

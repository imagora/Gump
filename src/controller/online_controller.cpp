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
    emit RemoveAllEvent();
    return;
  }

  static const int kAppidLength = 32;
  if (info.length() == kAppidLength) {
    UpdateByAppid(info);
  } else {
    UpdateByVid(info);
  }
}

bool OnlineController::QueryStream(const QString &stream, QString *url,
                                   Stream *info) {
  auto stream_it = streams_.find(stream);
  if (stream_it == streams_.end()) return false;

  *url = stream_it->second.url.toString();

  if (info != nullptr) {
    *info = stream_it->second;
  }
  return true;
}

bool OnlineController::GetNextStream(const QString &stream, QString *url,
                                     Stream *info) {
  if (streams_.empty())
    return false;

  auto stream_it = streams_.find(stream);
  if (stream_it == streams_.end() || std::next(stream_it) == streams_.end()) {
    stream_it = streams_.begin();
  } else {
    ++stream_it;
  }

  *url = stream_it->second.url.toString();

  if (info != nullptr) {
    *info = stream_it->second;
  }
  return true;
}

void OnlineController::OnUpdateUrl(QString url) {
  online_url_.setUrl(url);
}

void OnlineController::OnNetworkReply(QNetworkReply *reply) {
  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    qCritical() << "parse online response error" << data;
    return;
  }

  Streams multi_streams;
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

    auto streams = data.find("streams").value().toArray();
    foreach (QJsonValue stream_value, streams) {
      QJsonObject stream = stream_value.toObject();

      Stream stream_info;
      stream_info.ip =
          static_cast<uint32_t>(stream.find("lbes_ip").value().toInt());
      stream_info.user_id =
          static_cast<uint32_t>(stream.find("uid").value().toInt());
      stream_info.create_ts =
          static_cast<uint32_t>(stream.find("create").value().toInt());
      stream_info.name = stream.find("cname").value().toString();
      stream_info.url = QUrl(stream.find("url").value().toString());

      multi_streams[stream_info.url.fileName()] = stream_info;
    }

    auto members = data.find("members").value().toArray();
    foreach (QJsonValue member_value, members) {
      QJsonObject member = member_value.toObject();

      Stream stream_info;
      stream_info.ip =
          static_cast<uint32_t>(member.find("ip").value().toInt());
      stream_info.user_id =
          static_cast<uint32_t>(member.find("uid").value().toInt());
      stream_info.create_ts =
          static_cast<uint32_t>(member.find("create").value().toInt());
      stream_info.name = member.find("cname").value().toString();
      stream_info.url = QUrl(member.find("url").value().toString());

      multi_streams[stream_info.url.fileName()] = stream_info;
    }

    auto cursor = data.find("cursor").value().toInt(0);
    if (cursor != 0) {
      query_info_.removeQueryItem("cursor");
      query_info_.addQueryItem("cursor", QString::number(cursor));

      RequestOnline();
    }
  }

  UpdateList(multi_streams);
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

  online_url_.setPath(online_path_);
  online_url_.setQuery(query_info_);
  qInfo() << "request: " << online_url_;
  network_reply_ = network_manager_->get(QNetworkRequest(online_url_));
}

void OnlineController::UpdateList(const Streams &streams) {
  RemoveQuit(streams);

  InsertJoin(streams);

  Q_ASSERT_X(streams.size() == streams_.size(), "online", "streams size error");
}

void OnlineController::RemoveQuit(const Streams &streams) {
  auto old_stream_it = streams_.begin();
  while (old_stream_it != streams_.end()) {
    auto new_stream_it = streams.find(old_stream_it->first);
    if (new_stream_it == streams.end()) {
      emit RemoveStreamEvent(old_stream_it->first);
      old_stream_it = streams_.erase(old_stream_it);
      continue;
    }
    ++old_stream_it;
  }
}

void OnlineController::InsertJoin(const Streams &streams) {
  auto new_stream_it = streams.begin();
  while (new_stream_it != streams.end()) {
    auto old_stream_it = streams_.find(new_stream_it->first);
    if (old_stream_it == streams_.end()) {
      emit InsertStreamEvent(new_stream_it->first);
      streams_[new_stream_it->first] = new_stream_it->second;
    }

    ++new_stream_it;
  }
}

void OnlineController::UpdateByVid(const QString &vid) {
  online_path_ = "/v1/online/stream/listStreamsByKey";

  query_info_.addQueryItem("page_size", "10000");
  query_info_.addQueryItem("vid", vid);

  if (!request_timer_->isActive()) {
    request_timer_->start(60 * 1000);
  }

  RequestOnline();
}

void OnlineController::UpdateByAppid(const QString &appid) {
  online_path_ = "/v1/online/uap/scanIndexMembers";

  query_info_.addQueryItem("app", "mix_streaming");
  query_info_.addQueryItem("indexKeyObject", "vendor");
  query_info_.addQueryItem("indexMemberObject", "worker");
  query_info_.addQueryItem("indexType", "sortedSet");
  query_info_.addQueryItem("indexValue", "create");
  query_info_.addQueryItem("indexKey", appid);
  query_info_.addQueryItem("cursor", "0");

  if (!request_timer_->isActive()) {
    request_timer_->start(60 * 1000);
  }

  RequestOnline();
}


}  // namespace gump

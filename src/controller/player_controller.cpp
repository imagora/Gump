// Copyright (c) 2014-2019 winking324
//

#include "controller/player_controller.h"

#include <unistd.h>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>

#include "commons/definations.h"

namespace gump {

PlayerController::PlayerController(QObject *parent) : QObject(parent) {
  is_mute_ = false;
  renderer_ = nullptr;
  network_reply_ = nullptr;
  buffer_network_reply_ = nullptr;

  network_manager_ = new QNetworkAccessManager(this);
  connect(network_manager_, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(OnNetworkReply(QNetworkReply *)));
}

void PlayerController::GetStream(const QUrl &url) {
  qInfo() << "get stream:" << url;
  HttpRequest(url, &network_reply_);
}

void PlayerController::SearchStream(const QString &search_info) {
  QString url = kGumpServerBaseUrl + "search?" + search_info;
  GetStream(url);
}

void PlayerController::SetRenderer(QtAV::VideoOutput *renderer) {
  renderer_ = renderer;

  if (current_stream_.isEmpty()) {
    qInfo() << "current stream empty, ignore set renderer";
    return;
  }

  auto iter = streams_.find(current_stream_);
  if (iter == streams_.end()) {
    qCritical() << "not found stream:" << current_stream_
                << ", set renderer failed";
    return;
  }

  auto *player = iter.value().player;
  player->clearVideoRenderers();
  player->addVideoRenderer(renderer_);
}

QString PlayerController::GetCurrentStatus() {
  if (current_stream_.isEmpty()) {
    qWarning() << "current stream empty, has no status";
    return "";
  }

  auto iter = streams_.find(current_stream_);
  if (iter == streams_.end()) {
    qCritical() << "not found stream:" << current_stream_
                << ", has no media status";

    return "";
  }

  return GetPlayerStatus(iter.value().player, QtAV::UnknownMediaStatus);
}

void PlayerController::OnReleasePlayers() {
  qInfo() << "waiting for release players";
  OnStop();
}

void PlayerController::StopRequest(QNetworkReply **reply) {
  if (*reply != nullptr && !(*reply)->isFinished()) {
    qInfo() << "last requst for:" << (*reply)->url() << "not finished, abort";
    (*reply)->abort();
    (*reply)->deleteLater();
    *reply = nullptr;
  }
}

void PlayerController::OnStop() {
  qInfo() << "stop all players";
  StopRequest(&network_reply_);
  StopRequest(&buffer_network_reply_);

  current_stream_.clear();
  auto iter = streams_.begin();
  while (iter != streams_.end()) {
    auto *player = iter.value().player;
    player->clearVideoRenderers();
    player->audio()->setMute();
    player->stop();
    disconnect(player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this,
               SLOT(OnMediaStatusChanged(QtAV::MediaStatus)));

    player->deleteLater();
    iter = streams_.erase(iter);
  }
}

void PlayerController::OnPrev() {
  if (current_stream_.isEmpty()) {
    qInfo() << "current stream empty, ignore get prev stream";
    return;
  }

  auto iter = streams_.find(current_stream_);
  if (iter == streams_.end()) {
    qCritical() << "not found stream:" << current_stream_ << ", prev failed";
    return;
  }

  const auto &prev = iter.value().prev;
  if (prev.isEmpty()) {
    qInfo() << "stream:" << current_stream_ << "has no prev";
    return;
  }

  PlayStreamById(prev);
}

void PlayerController::OnNext() {
  if (current_stream_.isEmpty()) {
    qInfo() << "current stream empty, ignore get next stream";
    return;
  }

  auto iter = streams_.find(current_stream_);
  if (iter == streams_.end()) {
    qCritical() << "not found stream:" << current_stream_ << ", next failed";

    return;
  }

  const auto &next = iter.value().next;
  if (next.isEmpty()) {
    qInfo() << "stream:" << current_stream_ << "has no next";
    return;
  }

  PlayStreamById(next);
}

void PlayerController::OnMute(bool mute) {
  is_mute_ = mute;
  if (current_stream_.isEmpty()) {
    qInfo() << "current stream empty, ignore mute";
    return;
  }

  auto iter = streams_.find(current_stream_);
  if (iter == streams_.end()) {
    qCritical() << "not found stream:" << current_stream_ << ", mute failed";
    return;
  }

  qInfo() << "set stream:" << current_stream_ << (mute ? "mute" : "unmute");
  iter.value().player->audio()->setMute(is_mute_);
}

void PlayerController::HttpRequest(const QUrl &url, QNetworkReply **reply) {
  qInfo() << "new request for:" << url;
  StopRequest(reply);

  QSettings settings("agora.io", "gump");
  settings.beginGroup("auth");
  QString token = settings.value("token").toString();
  settings.endGroup();

  QNetworkRequest request(url);
  request.setRawHeader("Accesstoken", token.toLocal8Bit());
  *reply = network_manager_->get(request);
}

void PlayerController::OnMediaStatusChanged(QtAV::MediaStatus status) {
  if (current_stream_.isEmpty()) {
    qWarning() << "current stream empty, media status:" << status
               << "changed error";
    return;
  }

  auto iter = streams_.find(current_stream_);
  if (iter == streams_.end()) {
    qCritical() << "not found stream:" << current_stream_
                << ", media status changed error";

    return;
  }

  auto *player = iter.value().player;
  emit StatusChangeEvent(GetPlayerStatus(player, status));
}

void PlayerController::OnNetworkReply(QNetworkReply *reply) {
  if (reply != network_reply_ && reply != buffer_network_reply_) {
    qWarning() << "ignore network reply:" << reply->url();
    reply->deleteLater();
    return;
  }

  auto parser = [&](QString *id, StreamInfo *stream_info) {
    QByteArray data = reply->readAll();
    QJsonParseError parse_err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parse_err);
    if (parse_err.error != QJsonParseError::NoError || !doc.isObject()) {
      qCritical() << "cannot parse config response:" << data;
      return false;
    }

    auto obj = doc.object();
    auto status_iter = obj.find("status");
    if (status_iter.value().toString("") != "success") {
      qCritical() << "request failed, status:" << status_iter.value();
      return false;
    }

    auto stream_iter = obj.find("stream");
    if (stream_iter.value().type() != QJsonValue::Object) {
      qCritical() << "request failed, parse stream object failed";
      return false;
    }

    auto stream_obj = stream_iter.value().toObject();
    *id = stream_obj.find("id")->toString("");
    if (id->isEmpty()) {
      qCritical() << "request failed, parse stream id failed";
      return false;
    }

    stream_info->vendor = stream_obj.find("vendor")->toString("");
    stream_info->cname = stream_obj.find("cname")->toString("");
    stream_info->uid = stream_obj.find("uid")->toString("");
    stream_info->ip = stream_obj.find("ip")->toString("");
    stream_info->url = stream_obj.find("url")->toString("");
    stream_info->play_url = stream_obj.find("play_url")->toString("");
    stream_info->prev = stream_obj.find("prev")->toString("");
    stream_info->next = stream_obj.find("next")->toString("");

    return true;
  };

  QString id;
  StreamInfo stream_info;
  if (parser(&id, &stream_info)) {
    auto iter = streams_.find(id);
    if (iter != streams_.end()) {
      qWarning() << "already exist stream:" << id << ", drop it";
      return;
    }

    stream_info.player = new QtAV::AVPlayer();
    if (!stream_info.play_url.isEmpty()) {
      stream_info.player->play(stream_info.play_url);
      stream_info.player->audio()->setMute();
    }

    streams_.insert(id, stream_info);
  }

  if (reply == network_reply_) {
    network_reply_->deleteLater();
    network_reply_ = nullptr;
    PlayStreamById(id);
  } else {
    buffer_network_reply_->deleteLater();
    buffer_network_reply_ = nullptr;
  }
}

void PlayerController::PlayStreamById(const QString &id) {
  if (id.isEmpty()) {
    return;
  }

  auto iter = streams_.find(id);
  if (iter == streams_.end()) {
    qWarning() << "not found stream:" << id << ", request from server";

    QString search_info = "id=" + id.toLocal8Bit().toBase64();
    SearchStream(search_info);
    return;
  }

  if (!current_stream_.isEmpty()) {
    auto current_stream_iter = streams_.find(current_stream_);
    if (current_stream_iter == streams_.end()) {
      qCritical() << "not found stream:" << current_stream_ << ", ignore";
    } else {
      auto *playing_player = current_stream_iter.value().player;
      playing_player->audio()->setMute();
      playing_player->clearVideoRenderers();
      disconnect(playing_player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),
                 this, SLOT(OnMediaStatusChanged(QtAV::MediaStatus)));
    }
    current_stream_.clear();
  }

  current_stream_ = id;
  auto *player = iter.value().player;
  player->audio()->setMute(false);
  player->addVideoRenderer(renderer_);
  connect(player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this,
          SLOT(OnMediaStatusChanged(QtAV::MediaStatus)));
  emit StatusChangeEvent(GetCurrentStatus());

  ResizeBuffer();
}

void PlayerController::BufferStreamById(const QString &id) {
  if (id.isEmpty()) {
    qWarning() << "buffer stream id empty, ignore";
    return;
  }

  qInfo() << "buffer stream:" << id;
  auto iter = streams_.find(id);
  if (iter != streams_.end()) {
    qWarning() << "stream:" << current_stream_ << "already buffered, ignore";
    return;
  }

  StopRequest(&buffer_network_reply_);
  QString url = kGumpServerBaseUrl + "search?id=" + id.toLocal8Bit().toBase64();
  HttpRequest(url, &buffer_network_reply_);
}

QString PlayerController::GetPlayerStatus(QtAV::AVPlayer *player,
                                          uint32_t status) {
  const static QMap<int, QString> kPlayerStatusInfo = {
      {QtAV::UnknownMediaStatus, "Unknown"},
      {QtAV::NoMedia, "No Media"},
      {QtAV::LoadingMedia, "Loading Media"},
      {QtAV::LoadedMedia, "Loaded Media"},
      {QtAV::StalledMedia, "Stalled Media"},
      {QtAV::BufferingMedia, "Buffering Media"},
      {QtAV::BufferedMedia, "Buffered Media"},
      {QtAV::EndOfMedia, "End Of Media"},
      {QtAV::InvalidMedia, "Invalid Media"},
  };

  if (status == QtAV::UnknownMediaStatus) {
    status = player->mediaStatus();
  }

  auto iter = kPlayerStatusInfo.find(status);
  if (iter == kPlayerStatusInfo.end()) {
    iter = kPlayerStatusInfo.begin();
  }
  return iter.value();
}

void PlayerController::ResizeBuffer() {
  if (current_stream_.isEmpty()) {
    qInfo() << "current stream empty, clear all buffer";
    OnStop();
    return;
  }

  auto iter = streams_.find(current_stream_);
  if (iter == streams_.end()) {
    qCritical() << "not found stream:" << current_stream_
                << ", clear all buffer";
    OnStop();
    return;
  }

  bool found_prev = false;
  bool found_next = false;
  const auto &prev = iter.value().prev;
  const auto &next = iter.value().next;
  iter = streams_.begin();
  while (iter != streams_.end()) {
    auto const &id = iter.key();
    if (id == prev) {
      found_prev = true;
      ++iter;
      continue;
    }

    if (id == next) {
      found_next = true;
      ++iter;
      continue;
    }

    if (id == current_stream_) {
      ++iter;
      continue;
    }

    qInfo() << "clear expired stream:" << id;
    auto *player = iter.value().player;
    player->clearVideoRenderers();
    player->audio()->setMute();
    player->stop();
    player->deleteLater();
    iter = streams_.erase(iter);
  }

  if (!found_prev) {
    BufferStreamById(prev);
  }

  if (!found_next) {
    BufferStreamById(next);
  }
}

}  // namespace gump

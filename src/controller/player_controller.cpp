// Copyright (c) 2014-2019 winking324
//

#include "controller/player_controller.h"

#include <unistd.h>

#include <map>
#include <QDebug>


namespace gump {


static const uint32_t kOldMaxBufferSize = 3;
static const uint32_t kNewMaxBufferSize = 3;


PlayerController::PlayerController(QObject *parent)
  : QObject(parent) {
  renderer_ = nullptr;
}

void PlayerController::PlayStream(const QString &stream_url) {
  QString stream = QUrl(stream_url).fileName().section('.', 0, 0);
  qInfo() << "play stream: " << stream_url << ", name: " << stream;

  if (!new_players_.empty()) {
    if (stream == new_players_.front().stream)
      return;

    auto *playing_player = new_players_.front().player;
    playing_player->audio()->setMute();
    playing_player->clearVideoRenderers();
    disconnect(playing_player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),
               this, SLOT(OnMediaStatusChanged(QtAV::MediaStatus)));
  }

  auto it = std::find_if(new_players_.begin(), new_players_.end(),
                         [&](const BufferedPlayer &player) {
    return player.stream == stream;
  });

  QtAV::AVPlayer *player;
  if (it != new_players_.end()) {
    player = it->player;
    new_players_.splice(new_players_.begin(), new_players_, it);
  } else {
    BufferedPlayer new_player;
    new_player.stream = stream;
    new_player.player = new QtAV::AVPlayer();
    new_player.player->play(stream_url);

    player = new_player.player;

    new_players_.push_front(std::move(new_player));
  }

  player->audio()->setMute(false);
  player->addVideoRenderer(renderer_);
  connect(player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),
          this, SLOT(OnMediaStatusChanged(QtAV::MediaStatus)));
  emit StatusChangeEvent(GetCurrentStatus());
  ResizeBuffer();
}

void PlayerController::BufferStream(const QString &stream_url) {
  QString stream = QUrl(stream_url).fileName().section('.', 0, 0);
  qInfo() << "buffer stream: " << stream_url << ", name: " << stream;

  auto it = std::find_if(new_players_.begin(), new_players_.end(),
                         [&](const BufferedPlayer &player) {
    return player.stream == stream;
  });

  if (it != new_players_.end()) {
    return;
  }

  BufferedPlayer new_player;
  new_player.stream = stream;
  new_player.player = new QtAV::AVPlayer();
  new_player.player->play(stream_url);
  new_player.player->audio()->setMute();
  new_players_.insert(std::next(new_players_.begin()),
                           std::move(new_player));

  ResizeBuffer();
}

void PlayerController::SetRenderer(QtAV::VideoOutput *renderer) {
  renderer_ = renderer;

  if (new_players_.size() > 0) {
    auto *player = new_players_.front().player;
    player->clearVideoRenderers();
    player->addVideoRenderer(renderer_);
  }
}

QString PlayerController::GetCurrentStream() {
  if (new_players_.empty()) return "";

  return new_players_.front().stream;
}

QString PlayerController::GetCurrentStatus() {
  if (new_players_.empty()) return "";

  return GetPlayerStatus(new_players_.front().player, QtAV::UnknownMediaStatus);
}

void PlayerController::ReleasePlayers() {
  qInfo() << "waiting for release players";
  for (auto &player : new_players_) {
    player.player->clearVideoRenderers();
    player.player->audio()->setMute();
    player.player->stop();
    player.player->deleteLater();
  }

  new_players_.clear();
}

void PlayerController::OnMediaStatusChanged(QtAV::MediaStatus status) {
  auto *player = new_players_.front().player;
  emit StatusChangeEvent(GetPlayerStatus(player, status));
}

QString PlayerController::GetPlayerStatus(QtAV::AVPlayer *player,
                                          uint32_t status) {
  const static std::map<uint32_t, QString> kPlayerStatusInfo = {
    { QtAV::UnknownMediaStatus, "Unknown" },
    { QtAV::NoMedia, "No Media" },
    { QtAV::LoadingMedia, "Loading Media" },
    { QtAV::LoadedMedia, "Loaded Media" },
    { QtAV::StalledMedia, "Stalled Media" },
    { QtAV::BufferingMedia, "Buffering Media" },
    { QtAV::BufferedMedia, "Buffered Media" },
    { QtAV::EndOfMedia, "End Of Media" },
    { QtAV::InvalidMedia, "Invalid Media" },
  };

  if (status == QtAV::UnknownMediaStatus) {
    status = player->mediaStatus();
  }

  auto iter = kPlayerStatusInfo.find(status);
  if (iter == kPlayerStatusInfo.end()) {
    iter = kPlayerStatusInfo.begin();
  }
  return iter->second;
}

void PlayerController::ResizeBuffer() {
  while (new_players_.size() > kNewMaxBufferSize) {
    auto *delete_player = new_players_.back().player;
    if (delete_player != nullptr && delete_player->isPlaying()) {
      delete_player->stop();
      delete_player->deleteLater();
    }
    new_players_.pop_back();
  }
}


}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#include "controller/player_controller.h"

#include <unistd.h>

#include <map>
#include <QDebug>


namespace gump {


static const uint32_t kMaxSize = 4;


PlayerController::PlayerController(QObject *parent)
  : QObject(parent) {
  max_size_ = kMaxSize;
}

void PlayerController::PlayStream(const QString &stream,
                                  QtAV::VideoOutput *renderer) {
  qInfo() << "play stream: " << stream;
  if (!buffered_players_.empty()) {
    if (stream == buffered_players_.front().stream)
      return;

    auto *playing_player = buffered_players_.front().player;
    playing_player->audio()->setMute();
    playing_player->clearVideoRenderers();
    disconnect(playing_player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),
               this, SLOT(OnMediaStatusChanged(QtAV::MediaStatus)));
  }

  auto it = std::find_if(buffered_players_.begin(), buffered_players_.end(),
                         [&](const BufferedPlayer &player) {
    return player.stream == stream;
  });

  QtAV::AVPlayer *player;
  if (it != buffered_players_.end()) {
    player = it->player;
    buffered_players_.splice(buffered_players_.begin(), buffered_players_, it);
  } else {
    BufferedPlayer new_player;
    new_player.stream = stream;
    new_player.player = new QtAV::AVPlayer();
    new_player.player->play(stream);

    player = new_player.player;

    buffered_players_.push_front(std::move(new_player));
  }

  player->audio()->setMute(false);
  player->addVideoRenderer(renderer);
  connect(player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),
          this, SLOT(OnMediaStatusChanged(QtAV::MediaStatus)));

  ResizeBuffer();
}

void PlayerController::BufferStream(const QString &stream) {
  auto it = std::find_if(buffered_players_.begin(), buffered_players_.end(),
                         [&](const BufferedPlayer &player) {
    return player.stream == stream;
  });

  if (it != buffered_players_.end()) {
    return;
  }

  BufferedPlayer new_player;
  new_player.stream = stream;
  new_player.player = new QtAV::AVPlayer();
  new_player.player->play(stream);
  new_player.player->audio()->setMute();
  buffered_players_.insert(std::next(buffered_players_.begin()), std::move(new_player));

  ResizeBuffer();
}

void PlayerController::ReleasePlayers() {
  qInfo() << "release...";
  for (auto &player : buffered_players_) {
    player.player->clearVideoRenderers();
    player.player->audio()->setMute();
    player.player->stop();
    player.player->deleteLater();
  }

  buffered_players_.clear();
}

void PlayerController::OnMediaStatusChanged(QtAV::MediaStatus status) {
  auto *player = buffered_players_.front().player;
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
  if (buffered_players_.size() <= kMaxSize)
    return;

  auto *delete_player = buffered_players_.back().player;
  if (delete_player->isPlaying()) {
    delete_player->stop();
    delete_player->deleteLater();
  }
  buffered_players_.pop_back();
}


}  // namespace gump

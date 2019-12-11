// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <map>
#include <string>

#include "commons/definations.h"

namespace gump {

class PreferencesManager : public QObject {
  Q_OBJECT

 public:
  explicit PreferencesManager(QObject *parent = nullptr);

  virtual ~PreferencesManager();

  void PlayStream(const std::string &command);

  std::string ConvertToPlayUrl(const std::string &url);

  std::string ConvertToToolTip(const StreamInfo &stream);

  std::string QueryConfig(const std::string &key);

  void UpdatePreferences();

 signals:
  void Refresh(ChannelStreams channel_streams);

 private:
  void RefreshChannelStreams();

 private slots:
  void RefreshChannelStreamsTimer();

  void FinishConfigRequest(QNetworkReply *reply);

  void FinishOnlineRequest(QNetworkReply *reply);

 private:
  std::map<std::string, std::string> configs_;
  std::map<std::string, std::pair<std::string, std::string>> rules_;
  QNetworkAccessManager *config_mgr_;
  QNetworkAccessManager *online_mgr_;
};

}  // namespace gump

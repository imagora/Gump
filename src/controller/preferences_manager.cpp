// Copyright (c) 2014-2019 winking324
//

#include "controller/preferences_manager.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <log4cplus/log4cplus.h>

#include <QTimer>
#include <QDateTime>
#include <QSettings>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>

#include <sstream>


namespace gump {


PreferencesManager::PreferencesManager(QObject *parent)
    : QObject(parent) {
  config_mgr_ = new QNetworkAccessManager(this);
  online_mgr_ = new QNetworkAccessManager(this);
  connect(config_mgr_, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(FinishConfigRequest(QNetworkReply*)));
  connect(online_mgr_, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(FinishOnlineRequest(QNetworkReply*)));
  QTimer::singleShot(1000, this, SLOT(RefreshChannelStreamsTimer()));

  UpdatePreferences();
}

PreferencesManager::~PreferencesManager() {
}

void PreferencesManager::PlayStream(const std::string &command) {
  FILE* fp = popen(command.c_str(), "r");
  if (fp == nullptr) {
    LOG4CPLUS_WARN(kLoggerName, "Failed to run cmd: " << command);
  }
}

std::string PreferencesManager::ConvertToPlayUrl(const std::string &url) {
  std::pair<std::string, std::string> re{"", ""};
  for (auto host : rules_) {
    if (url.find(host.first) == std::string::npos) {
      continue;
    }
    re = host.second;
    break;
  }

  if (re.first.empty()) return "";
  QString stream(re.second.c_str());

  QRegExp qre(re.first.c_str());
  qre.setMinimal(false);
  int pos = qre.indexIn(url.c_str());
  if (pos > -1) {
    int cap_count = qre.captureCount();
    switch (cap_count) {
      case 1:
        stream = stream.arg(qre.cap(1));
      break;
      case 2:
        stream = stream.arg(qre.cap(1), qre.cap(2));
      break;
      case 3:
        stream = stream.arg(qre.cap(1), qre.cap(2), qre.cap(3));
      break;
      default:
      break;
    }
  }
  return stream.toStdString();
}

std::string PreferencesManager::ConvertToToolTip(const StreamInfo &stream) {
  QDateTime date_time;
  date_time.setTime_t(stream.create_ts);

  in_addr ip_addr = {0};
  ip_addr.s_addr = ntohl(stream.ip);
  std::string ip = inet_ntoa(ip_addr);

  std::stringstream ss;
  ss << "uid: " << stream.user_id << "\n"
     << "mode: " << (stream.mode == 1 ? "raw" : "mix") << "\n"
     << "status: " << stream.status << "\n"
     << "create at: " << date_time.toString("yyyy-MM-dd hh:mm:ss").toStdString()
     << "\n"
     << "from server: " << ip;
  return ss.str();
}

std::string PreferencesManager::QueryConfig(const std::string &key) {
  auto it = configs_.find(key);
  if (it == configs_.end()) return "";

  return it->second;
}

void PreferencesManager::UpdatePreferences() {
  QSettings settings("agora.io", "gump");
  settings.beginGroup("preferences");
  QString config_url = settings.value("config_url").toString();
  QString username = settings.value("config_username").toString();
  QString password = settings.value("config_password").toString();
  settings.endGroup();

  if (config_url.isEmpty()) {
    LOG4CPLUS_WARN(kLoggerName, "cannot get config url");
    return;
  }

  QString concatenated = username + ":" + password;
  QByteArray data = concatenated.toLocal8Bit().toBase64();
  QString headerData = "Basic " + data;

  QNetworkRequest request = QNetworkRequest(QUrl(config_url));
  request.setRawHeader("Authorization", headerData.toLocal8Bit());
  config_mgr_->get(request);
}

void PreferencesManager::RefreshChannelStreams() {
  QString url = QString::fromStdString(QueryConfig("online_url"));
  if (url.isEmpty()) return;

  QSettings settings("agora.io", "gump");
  settings.beginGroup("preferences");
  QString vid = settings.value("vid").toString();
  QString max = settings.value("max").toString();
  settings.endGroup();

  url += "?page_size=" + max;
  if (vid != "0") {
    url += "&vid=" + vid;
  }

  online_mgr_->get(QNetworkRequest(QUrl(url)));
}

void PreferencesManager::RefreshChannelStreamsTimer() {
  RefreshChannelStreams();
  QTimer::singleShot(5 * 60 * 1000, this, SLOT(RefreshChannelStreamsTimer()));
}

void PreferencesManager::FinishConfigRequest(QNetworkReply *reply) {
  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument rule_json = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    LOG4CPLUS_ERROR(kLoggerName, "Parse rule(json) error");
    return;
  }

  std::map<std::string, std::string> new_configs;
  std::map<std::string, std::pair<std::string, std::string>> new_rules;
  if (rule_json.isObject()) {
    QJsonObject obj = rule_json.object();
    for (QJsonObject::Iterator iter = obj.begin(); iter != obj.end(); ++iter) {
      std::string key = iter.key().toStdString();

      if (key != "rules") {
        new_configs.insert(
              std::make_pair(key, iter.value().toString().toStdString()));
        continue;
      }

      QJsonObject rules_obj = iter.value().toObject();
      for (auto rule_it = rules_obj.begin(); rule_it != rules_obj.end();
           ++rule_it) {
        std::string rule_key = rule_it.key().toStdString();

        if (!rule_it.value().isObject()) {
          LOG4CPLUS_ERROR(kLoggerName, "Cannot get rule for key: " << key);
          continue;
        }

        QJsonObject rule_obj = rule_it.value().toObject();
        QJsonValue re_val = rule_obj["re"];
        QJsonValue url_val = rule_obj["url"];
        if (re_val == QJsonValue::Undefined ||
            url_val == QJsonValue::Undefined) {
          LOG4CPLUS_ERROR(kLoggerName,
                          "Cannot get re and url from value: " << key);
          continue;
        }

        new_rules[rule_key] = std::make_pair(re_val.toString().toStdString(),
                                             url_val.toString().toStdString());
      }
    }
  }

  new_configs.swap(configs_);
  new_rules.swap(rules_);
}

void PreferencesManager::FinishOnlineRequest(QNetworkReply *reply) {
  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    LOG4CPLUS_ERROR(kLoggerName, "Parse stream info error");
    return;
  }

  ChannelStreams channel_streams;
  if (doc.isObject()) {
    QJsonObject obj = doc.object();
    auto success_iter = obj.find("success");
    if (success_iter.value().type() != QJsonValue::Bool ||
        !success_iter.value().toBool()) {
      LOG4CPLUS_ERROR(kLoggerName, "Request streams failed, success error");
      return;
    }

    auto data_iter = obj.find("data");
    if (data_iter.value().type() != QJsonValue::Object) {
      LOG4CPLUS_ERROR(kLoggerName, "Request streams failed, no data");
      return;
    }
    QJsonObject data = data_iter.value().toObject();
    int total_count = data.find("total_count").value().toInt();
    LOG4CPLUS_INFO(kLoggerName, "Total stream count: " << total_count);

    QJsonArray streams = data.find("streams").value().toArray();
    foreach (QJsonValue stream_value, streams) {
      QJsonObject stream = stream_value.toObject();
      uint32_t vid = stream.find("vid").value().toInt();
      QString cname = stream.find("cname").value().toString();

      StreamInfo stream_info;
      stream_info.ip = stream.find("lbes_ip").value().toInt();
      stream_info.mode = stream.find("mode").value().toInt();
      stream_info.status = stream.find("status").value().toInt();
      stream_info.user_id = stream.find("uid").value().toInt();
      stream_info.create_ts = stream.find("create").value().toInt();
      stream_info.stream_url =
          stream.find("url").value().toString().toStdString();
      channel_streams[std::make_pair(std::to_string(vid), cname.toStdString())].
          push_back(stream_info);
    }
  }

  emit Refresh(channel_streams);
}


}  // namespace gump


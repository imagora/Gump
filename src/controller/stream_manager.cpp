#include "stream_manager.h"
#include <sstream>
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
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>


using namespace gump;


StreamManager::StreamManager(QObject *parent)
  : QObject(parent)
{
  rule_mgr_ = new QNetworkAccessManager(this);
  online_mgr_ = new QNetworkAccessManager(this);
  connect(rule_mgr_, SIGNAL(finished(QNetworkReply*)), this, SLOT(FinishRuleRequest(QNetworkReply*)));
  connect(online_mgr_, SIGNAL(finished(QNetworkReply*)), this, SLOT(FinishOnlineRequest(QNetworkReply*)));
  QTimer::singleShot(1000, this, SLOT(RefreshChannelStreamsTimer()));

  UpdateStreamRule();
}

StreamManager::~StreamManager()
{
}

void StreamManager::PlayStream(const std::string &command)
{
  FILE* fp = popen(command.c_str(), "r");
  if (fp == NULL) {
    LOG4CPLUS_WARN_FMT(LOGGER_NAME, "Failed to run cmd: %s", command.c_str());
  }
}

std::string StreamManager::ConvertToPlayUrl(const std::string &url)
{
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

std::string StreamManager::ConvertToToolTip(const StreamInfo &stream)
{
  QDateTime date_time;
  date_time.setTime_t(stream.create_ts);

  in_addr ip_addr = {0};
  ip_addr.s_addr = ntohl(stream.ip);
  std::string ip = inet_ntoa(ip_addr);

  std::stringstream ss;
  ss << "uid: " << stream.user_id << "\n"
     << "mode: " << (stream.mode == 1 ? "raw" : "mix") << "\n"
     << "status: " << stream.status << "\n"
     << "create at: " << date_time.toString("yyyy-MM-dd hh:mm:ss").toStdString() << "\n"
     << "from server: " << ip;
  return ss.str();
}

void StreamManager::UpdateStreamRule()
{
  QSettings settings("agora.io", "gump");
  settings.beginGroup("preferences");
  QString rule_url = settings.value("rule_url").toString();
  QString username = settings.value("rule_username").toString();
  QString password = settings.value("rule_password").toString();
  settings.endGroup();

  if (rule_url.isEmpty()) return;

  QString concatenated = username + ":" + password;
  QByteArray data = concatenated.toLocal8Bit().toBase64();
  QString headerData = "Basic " + data;

  QNetworkRequest request = QNetworkRequest(QUrl(rule_url));
  request.setRawHeader("Authorization", headerData.toLocal8Bit());
  rule_mgr_->get(request);
}

void StreamManager::RefreshChannelStreams()
{
  QSettings settings("agora.io", "gump");
  settings.beginGroup("preferences");
  QString url = settings.value("online_url").toString();
  settings.endGroup();

  if (url.isEmpty()) return;

  online_mgr_->get(QNetworkRequest(QUrl(url)));
}

void StreamManager::RefreshChannelStreamsTimer()
{
  RefreshChannelStreams();
  QTimer::singleShot(5 * 60 * 1000, this, SLOT(RefreshChannelStreamsTimer()));
}

void StreamManager::FinishRuleRequest(QNetworkReply *reply)
{
  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument rule_json = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Parse rule(json) error");
    return;
  }

  Rules new_rules;
  if (rule_json.isObject()) {
    QJsonObject obj = rule_json.object();
    for (QJsonObject::Iterator iter = obj.begin(); iter != obj.end(); ++iter) {
      RuleKey key = iter.key().toStdString();

      if (!iter.value().isObject()) {
        LOG4CPLUS_ERROR_FMT(LOGGER_NAME, "Cannot get rule for key: %s", key.c_str());
        continue;
      }

      QJsonObject rule_obj = iter.value().toObject();
      QJsonValue re_val = rule_obj["re"];
      QJsonValue url_val = rule_obj["url"];
      if (re_val == QJsonValue::Undefined || url_val == QJsonValue::Undefined) {
        LOG4CPLUS_ERROR_FMT(LOGGER_NAME, "Cannot get re and url from value: %s", key.c_str());
        continue;
      }

      new_rules[key] = std::make_pair(re_val.toString().toStdString(),
                                      url_val.toString().toStdString());
    }
  }

  new_rules.swap(rules_);
}

void StreamManager::FinishOnlineRequest(QNetworkReply *reply)
{
  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Parse stream info error");
    return;
  }

  ChannelStreams channel_streams;
  if (doc.isObject()) {
    QJsonObject obj = doc.object();
    auto success_iter = obj.find("success");
    if (success_iter.value().type() != QJsonValue::Bool ||
        !success_iter.value().toBool()) {
      LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Request streams failed, success error");
      return;
    }

    auto data_iter = obj.find("data");
    if (data_iter.value().type() != QJsonValue::Object) {
      LOG4CPLUS_ERROR_STR(LOGGER_NAME, "Request streams failed, no data");
      return;
    }
    QJsonObject data = data_iter.value().toObject();
    int total_count = data.find("total_count").value().toInt();
    LOG4CPLUS_INFO_FMT(LOGGER_NAME, "Total stream count: %d", total_count);

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
      stream_info.stream_url = stream.find("url").value().toString().toStdString();
      channel_streams[std::make_pair(std::to_string(vid), cname.toStdString())].
          push_back(stream_info);
    }
  }

  emit Refresh(channel_streams);
}


#include "stream_manager.h"
#include <iostream>
#include <QTimer>
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
  UpdateStreamRule();
  network_mgr_ = new QNetworkAccessManager(this);
  connect(network_mgr_, SIGNAL(finished(QNetworkReply*)), this, SLOT(FinishRequest(QNetworkReply*)));
  QTimer::singleShot(1000, this, SLOT(RefreshChannelStreamsTimer()));
}

StreamManager::~StreamManager()
{
  delete network_mgr_;
  network_mgr_ = nullptr;
}

void StreamManager::PlayStream(const std::string &command)
{
  FILE* fp = popen(command.c_str(), "r");
  if (fp == NULL) {
    std::cout << "failed to run command '" << command << "'" << std::endl;
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

void StreamManager::UpdateStreamRule()
{
  QSettings settings("agora.io", "gump");
  settings.beginGroup("preferences");
  std::string rule = settings.value("rule").toString().toStdString();
  settings.endGroup();

  QJsonParseError parse_err;
  QJsonDocument rule_json = QJsonDocument::fromJson(rule.c_str(), &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    std::cout << "parse rule(json) error: " << rule << std::endl;
    return;
  }

  Rules new_rules;
  if (rule_json.isObject()) {
    QJsonObject obj = rule_json.object();
    for (QJsonObject::Iterator iter = obj.begin(); iter != obj.end(); ++iter) {
      RuleKey key = iter.key().toStdString();

      if (!iter.value().isObject()) {
        std::cout << "cannot get rule for key: " << key << std::endl;
        continue;
      }

      QJsonObject rule_obj = iter.value().toObject();
      QJsonValue re_val = rule_obj["re"];
      QJsonValue url_val = rule_obj["url"];
      if (re_val == QJsonValue::Undefined || url_val == QJsonValue::Undefined) {
        std::cout << "cannot get re and url from value: " << key << std::endl;
        continue;
      }

      new_rules[key] = std::make_pair(re_val.toString().toStdString(),
                                      url_val.toString().toStdString());
    }
  }

  new_rules.swap(rules_);
}

void StreamManager::RefreshChannelStreams()
{
  QSettings settings("agora.io", "gump");
  settings.beginGroup("preferences");
  std::string url = settings.value("url").toString().toStdString();
  settings.endGroup();

  network_mgr_->get(QNetworkRequest(QUrl(url.c_str())));
}

void StreamManager::RefreshChannelStreamsTimer()
{
  RefreshChannelStreams();
  QTimer::singleShot(60 * 1000, this, SLOT(RefreshChannelStreamsTimer()));
}

void StreamManager::FinishRequest(QNetworkReply *reply)
{
  QByteArray data = reply->readAll();
  QJsonParseError parse_err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parse_err);
  if (parse_err.error != QJsonParseError::NoError) {
    std::cout << "parse json error" << std::endl;
    return;
  }

  ChannelStreams channel_streams;
  if (doc.isObject()) {
    QJsonObject obj = doc.object();
    for (QJsonObject::Iterator iter = obj.begin(); iter != obj.end(); ++iter) {
      QString vid = iter.key().section(":", 0, 0);
      QString cname = iter.key().section(":", 1);

      if (iter.value().type() != QJsonValue::Array) {
        std::cout << "cannot get steams type: " << iter.value().type() << std::endl;
        continue;
      }

      Streams &streams = channel_streams[std::make_pair(vid.toStdString(), cname.toStdString())];

      QJsonArray raw_streams = iter.value().toArray();
      foreach (QJsonValue stream, raw_streams) {
        streams.push_back(stream.toString().toStdString());
      }
    }
  }

  emit Refresh(channel_streams);
}


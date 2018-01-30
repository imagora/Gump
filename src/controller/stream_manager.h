#pragma once
#include <QObject>
#include "commons/definations.h"


class QNetworkReply;
class QNetworkAccessManager;


namespace gump {


class StreamManager : public QObject
{
  Q_OBJECT

  typedef std::string RuleKey;
  typedef std::pair<std::string, std::string> Rule;
  typedef std::map<RuleKey, Rule> Rules;

 public:
  StreamManager(QObject *parent = nullptr);

  ~StreamManager();

  void PlayStream(const std::string& command);

  std::string ConvertToPlayUrl(const std::string &url);

  std::string ConvertToToolTip(const StreamInfo &stream);

  void UpdateStreamRule();

 signals:
  void Refresh(ChannelStreams channel_streams);

 private:
  void RefreshChannelStreams();

 private slots:
  void RefreshChannelStreamsTimer();

  void FinishRuleRequest(QNetworkReply *reply);

  void FinishOnlineRequest(QNetworkReply *reply);

 private:
  Rules rules_;
  QNetworkAccessManager *rule_mgr_;
  QNetworkAccessManager *online_mgr_;
};


}


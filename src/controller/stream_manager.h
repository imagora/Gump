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

  void UpdateStreamRule();

 signals:
  void Refresh(ChannelStreams channel_streams);

 private:
  void RefreshChannelStreams();

 private slots:
  void RefreshChannelStreamsTimer();

  void FinishRequest(QNetworkReply *reply);

 private:
  Rules rules_;
  QNetworkAccessManager *network_mgr_;
};


}


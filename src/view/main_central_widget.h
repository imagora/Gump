#pragma once
#include <string>
#include <QWidget>
#include "commons/definations.h"


class QTableWidget;
class QHBoxLayout;
class QTableWidgetItem;


namespace gump {


class StreamManager;


class MainCentralWidget : public QWidget
{
  Q_OBJECT

 public:
  MainCentralWidget(QWidget *parent = nullptr);

  ~MainCentralWidget();

  void UpdateStreamRule();

  void SearchItem(const std::string &vid, const std::string &cname, const std::string &stream);

 private:
  void InsertRow(const std::string &vid, const std::string &cname, const std::string &stream);

  void RemoveRow(const std::string &vid, const std::string &cname, const std::string &stream);

  void DiffChannelStreams(const ChannelStreams &channel_streams);

 private slots:
  void OnPlay(QTableWidgetItem *item);
  void RefreshStreams(ChannelStreams channel_streams);

 private:
  QHBoxLayout *layout_;
  QTableWidget *stream_table_;
  StreamManager *stream_mgr_;
  ChannelStreams channel_streams_;
};


}


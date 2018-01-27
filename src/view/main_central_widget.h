#pragma once
#include <string>
#include <QWidget>
#include "commons/definations.h"


class QTableWidget;
class QTableWidgetItem;


namespace gump {


class PlayerWidget;
class StreamManager;


class MainCentralWidget : public QWidget
{
  Q_OBJECT

 public:
  MainCentralWidget(QWidget *parent = nullptr);

  ~MainCentralWidget();

  void UpdateStreamRule();

  void SearchItem(const std::string &vid, const std::string &cname, const std::string &stream);

  void PlayStream();

  void PauseStream();

  void StopStream();

 protected:
  void keyReleaseEvent(QKeyEvent *event) override;

 private:
  void InsertRow(const std::string &vid, const std::string &cname, const StreamInfo &stream);

  void RemoveRow(const std::string &vid, const std::string &cname, const std::string &stream);

  void DiffChannelStreams(const ChannelStreams &channel_streams);

 private slots:
  void OnPlay(QTableWidgetItem *item);

  void OnPlay(bool);

  void OnCopyStream(bool);

  void OnCopyPlayStream(bool);

  void OnCopyStreamInfo(bool);

  void OnMenu(const QPoint &pos);

  void RefreshStreams(ChannelStreams channel_streams);

 private:
  PlayerWidget *player_widget_;
  QTableWidget *stream_table_;
  StreamManager *stream_mgr_;
  ChannelStreams channel_streams_;
};


}


// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <string>

#include "view/player_widget.h"
#include "commons/definations.h"
#include "controller/preferences_manager.h"


namespace gump {


class MainCentralWidget :
    public QWidget {
  Q_OBJECT

 public:
  explicit MainCentralWidget(QWidget *parent = nullptr);

  virtual ~MainCentralWidget();

  void UpdatePreferences();

 public slots:
  void SearchItem(const std::string &vid, const std::string &cname,
                  const std::string &stream);

  void PlayStream();

  void PauseStream();

  void StopStream();

  void ShowDetails();

  void WindowMove();

 protected:
  void keyReleaseEvent(QKeyEvent *event) override;

 private:
  void InsertRow(const std::string &vid, const std::string &cname,
                 const StreamInfo &stream);

  void RemoveRow(const std::string &vid, const std::string &cname,
                 const std::string &stream);

  void DiffChannelStreams(const ChannelStreams &channel_streams);

 private slots:
  void OnPlay(QTableWidgetItem *item);

  void OnPlay(bool);

  void OnTracer(bool);

  void OnCopyStream(bool);

  void OnCopyPlayStream(bool);

  void OnCopyStreamInfo(bool);

  void OnMenu(const QPoint &pos);

  void RefreshStreams(ChannelStreams channel_streams);

 private:
  PlayerWidget *player_widget_;
  QTableWidget *stream_table_;
  PreferencesManager *config_mgr_;
  ChannelStreams channel_streams_;
};


}  // namespace gump


// Copyright (c) 2014-2019 winking324
//

#include "view/main_central_widget.h"

#include <log4cplus/log4cplus.h>

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMenu>
#include <QRegExp>
#include <QSettings>
#include <QVBoxLayout>
#include <map>

namespace gump {

MainCentralWidget::MainCentralWidget(QWidget *parent) : QWidget(parent) {
  config_mgr_ = new PreferencesManager(this);

  QHBoxLayout *all_layout = new QHBoxLayout();
  setLayout(all_layout);

  QVBoxLayout *table_layout = new QVBoxLayout();
  all_layout->addLayout(table_layout);

  stream_table_ = new QTableWidget(this);
  stream_table_->setContextMenuPolicy(Qt::CustomContextMenu);
  stream_table_->setColumnCount(3);
  stream_table_->setHorizontalHeaderLabels(
      QStringList{"VID", "CNAME", "STREAM"});
  stream_table_->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::ResizeToContents);
  stream_table_->horizontalHeader()->setSectionResizeMode(2,
                                                          QHeaderView::Stretch);
  table_layout->addWidget(stream_table_);

  player_widget_ = new PlayerWidget(this);
  all_layout->addWidget(player_widget_);

  connect(stream_table_, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this,
          SLOT(OnPlay(QTableWidgetItem *)));
  connect(stream_table_, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(OnMenu(QPoint)));
  connect(config_mgr_, SIGNAL(Refresh(ChannelStreams)), this,
          SLOT(RefreshStreams(ChannelStreams)));
}

MainCentralWidget::~MainCentralWidget() {}

void MainCentralWidget::UpdatePreferences() {
  config_mgr_->UpdatePreferences();
}

void MainCentralWidget::SearchItem(const std::string &search) {
  QList<QTableWidgetItem *> find_items =
      stream_table_->findItems(search.c_str(), Qt::MatchContains);
  if (find_items.empty()) return;

  stream_table_->selectRow((*find_items.begin())->row());
}

void MainCentralWidget::PlayStream() { player_widget_->PlayStream(""); }

void MainCentralWidget::PauseStream() { player_widget_->PauseStream(); }

void MainCentralWidget::StopStream() { player_widget_->StopStream(); }

void MainCentralWidget::ShowDetails() { player_widget_->ShowDetails(); }

void MainCentralWidget::WindowMove() { player_widget_->WindowMove(); }

void MainCentralWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() != Qt::Key_Up && event->key() != Qt::Key_Down) {
    return;
  }

  int col = stream_table_->currentColumn();
  if (col != 2) return;

  int row = stream_table_->currentRow();
  QTableWidgetItem *item = stream_table_->item(row, 2);
  OnPlay(item);
}

void MainCentralWidget::InsertRow(const std::string &vid,
                                  const std::string &cname,
                                  const StreamInfo &stream) {
  stream_table_->insertRow(stream_table_->rowCount());
  int row = stream_table_->rowCount() - 1;
  QTableWidgetItem *vid_item = new QTableWidgetItem(vid.c_str());
  QTableWidgetItem *cname_item = new QTableWidgetItem(cname.c_str());
  QTableWidgetItem *stream_item =
      new QTableWidgetItem(stream.stream_url.c_str());

  vid_item->setFlags(stream_item->flags() & ~Qt::ItemIsEditable);
  cname_item->setFlags(stream_item->flags() & ~Qt::ItemIsEditable);
  stream_item->setFlags(stream_item->flags() & ~Qt::ItemIsEditable);

  cname_item->setToolTip(cname.c_str());
  stream_item->setToolTip(config_mgr_->ConvertToToolTip(stream).c_str());

  stream_table_->setItem(row, 0, vid_item);
  stream_table_->setItem(row, 1, cname_item);
  stream_table_->setItem(row, 2, stream_item);
}

void MainCentralWidget::RemoveRow(const std::string &vid,
                                  const std::string &cname,
                                  const std::string &stream) {
  if (stream.empty()) {
    QList<QTableWidgetItem *> find_items =
        stream_table_->findItems(cname.c_str(), Qt::MatchExactly);
    foreach (QTableWidgetItem *item, find_items) {
      int row = item->row();
      if (stream_table_->item(row, 0)->text().toStdString() != vid) continue;
      stream_table_->removeRow(row);
    }
    return;
  }

  QList<QTableWidgetItem *> find_items =
      stream_table_->findItems(stream.c_str(), Qt::MatchExactly);
  foreach (QTableWidgetItem *item, find_items) {
    int row = item->row();
    if (stream_table_->item(row, 0)->text().toStdString() != vid) continue;
    if (stream_table_->item(row, 1)->text().toStdString() != cname) continue;
    stream_table_->removeRow(item->row());
  }
}

void MainCentralWidget::DiffChannelStreams(
    const ChannelStreams &channel_streams) {
  // 1. check channel quit/change
  for (auto channel : channel_streams_) {
    auto iter = channel_streams.find(channel.first);
    if (iter == channel_streams.end()) {
      RemoveRow(channel.first.first, channel.first.second, "");
      continue;
    }

    for (auto stream : channel.second) {
      auto stream_iter =
          std::find_if(iter->second.begin(), iter->second.end(),
                       [&](const StreamInfo &item) {
                         return item.stream_url == stream.stream_url;
                       });
      if (stream_iter == iter->second.end()) {
        RemoveRow(channel.first.first, channel.first.second, stream.stream_url);
      }
    }
  }

  // 2. check channel join
  for (auto channel : channel_streams) {
    auto iter = channel_streams_.find(channel.first);
    if (iter == channel_streams_.end()) {
      for (auto stream : channel.second) {
        InsertRow(channel.first.first, channel.first.second, stream);
      }
      continue;
    }

    for (auto stream : channel.second) {
      auto stream_iter =
          std::find_if(iter->second.begin(), iter->second.end(),
                       [&](const StreamInfo &item) {
                         return item.stream_url == stream.stream_url;
                       });
      if (stream_iter == iter->second.end()) {
        InsertRow(channel.first.first, channel.first.second, stream);
      }
    }
  }
}

void MainCentralWidget::OnPlay(QTableWidgetItem *item) {
  std::string cmd;
  std::string stream = item->text().toStdString();
  if (stream.empty() || stream.find("rtmp") == std::string::npos) {
    return;
  }
  stream = config_mgr_->ConvertToPlayUrl(stream);
  if (stream.empty()) {
    LOG4CPLUS_WARN_STR(kLoggerName, "Cannot convert stream to play url");
    return;
  }

  LOG4CPLUS_INFO_FMT(kLoggerName, "Play stream: %s", stream.c_str());
  player_widget_->StopStream();
  player_widget_->PlayStream(stream);

  if (item->row() + 1 < stream_table_->rowCount()) {
    auto *buffered_item = stream_table_->item(item->row() + 1, 2);
    std::string buffer_stream = buffered_item->text().toStdString();
    if (!buffer_stream.empty() &&
        buffer_stream.find("rtmp") != std::string::npos) {
      buffer_stream = config_mgr_->ConvertToPlayUrl(buffer_stream);
    }

    if (!buffer_stream.empty()) {
      LOG4CPLUS_INFO(kLoggerName, "Buffer stream: " << buffer_stream);
      player_widget_->BufferStream(buffer_stream);
    }
  }

  QSettings settings("agora.io", "gump");
  settings.beginGroup("preferences");
  cmd = settings.value("external_player").toString().toStdString();
  settings.endGroup();
  if (cmd.empty()) return;
  if (cmd.find("ffplay") != std::string::npos) {
    cmd += " -v quiet ";
  }

  cmd += stream;

  LOG4CPLUS_INFO_FMT(kLoggerName, "Run cmd: %s", cmd.c_str());
  config_mgr_->PlayStream(cmd);
}

void MainCentralWidget::OnPlay(bool) {
  int row = stream_table_->currentRow();
  QTableWidgetItem *item = stream_table_->item(row, 2);
  OnPlay(item);
}

void MainCentralWidget::OnTracer(bool) {
  QString url = QString::fromStdString(config_mgr_->QueryConfig("tracer_url"));
  if (url.isEmpty()) return;

  int row = stream_table_->currentRow();
  QTableWidgetItem *vid_item = stream_table_->item(row, 0);
  QString vid = vid_item->text();
  QTableWidgetItem *cname_item = stream_table_->item(row, 1);
  QString cname = cname_item->text();

  url += ("?vids=" + vid + "&cname=" + cname);
  QDesktopServices::openUrl(QUrl(url));
}

void MainCentralWidget::OnCopyStream(bool) {
  int row = stream_table_->currentRow();
  QTableWidgetItem *item = stream_table_->item(row, 2);
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(item->text());
}

void MainCentralWidget::OnCopyPlayStream(bool) {
  int row = stream_table_->currentRow();
  QTableWidgetItem *item = stream_table_->item(row, 2);
  std::string stream = item->text().toStdString();
  if (stream.empty() || stream.find("rtmp") == std::string::npos) {
    LOG4CPLUS_INFO_FMT(kLoggerName, "Copy play stream %s failed",
                       stream.c_str());
    return;
  }

  stream = config_mgr_->ConvertToPlayUrl(stream);
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(stream.c_str());
}

void MainCentralWidget::OnCopyStreamInfo(bool) {
  int row = stream_table_->currentRow();
  QTableWidgetItem *item = stream_table_->item(row, 2);
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(item->toolTip());
}

void MainCentralWidget::OnMenu(const QPoint &pos) {
  QMenu *menu = new QMenu(this);

  QAction *play_action = new QAction("Play Stream", menu);
  QAction *open_tracer_action = new QAction("Open Tracer", menu);
  QAction *url_action = new QAction("Copy Stream Url", menu);
  QAction *play_url_action = new QAction("Copy Stream Play Url", menu);
  QAction *info_action = new QAction("Copy Stream Detail Info", menu);

  connect(play_action, SIGNAL(triggered(bool)), SLOT(OnPlay(bool)));
  connect(open_tracer_action, SIGNAL(triggered(bool)), SLOT(OnTracer(bool)));
  connect(url_action, SIGNAL(triggered(bool)), SLOT(OnCopyStream(bool)));
  connect(play_url_action, SIGNAL(triggered(bool)),
          SLOT(OnCopyPlayStream(bool)));
  connect(info_action, SIGNAL(triggered(bool)), SLOT(OnCopyStreamInfo(bool)));

  menu->addAction(play_action);
  menu->addAction(open_tracer_action);
  menu->addAction(url_action);
  menu->addAction(play_url_action);
  menu->addAction(info_action);

  menu->popup(stream_table_->viewport()->mapToGlobal(pos));
}

void MainCentralWidget::RefreshStreams(ChannelStreams channel_streams) {
  DiffChannelStreams(channel_streams);
  channel_streams_.swap(channel_streams);
}

}  // namespace gump

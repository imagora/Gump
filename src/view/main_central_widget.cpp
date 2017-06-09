#include "main_central_widget.h"
#include "controller/stream_manager.h"
#include <QTableWidget>
#include <QHBoxLayout>
#include <QSettings>
#include <QRegExp>
#include <iostream>
#include <map>


using namespace gump;


MainCentralWidget::MainCentralWidget(QWidget *parent)
  : QWidget(parent)
{
  stream_mgr_ = new StreamManager(this);

  layout_ = new QHBoxLayout(this);
  setLayout(layout_);

  stream_table_ = new QTableWidget(this);
  stream_table_->setColumnCount(3);
  stream_table_->setHorizontalHeaderLabels(QStringList{"VID", "CNAME", "STREAM"});

  layout_->addWidget(stream_table_);
  connect(stream_table_, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(OnPlay(QTableWidgetItem*)));
  connect(stream_mgr_, SIGNAL(Refresh(ChannelStreams)), this, SLOT(RefreshStreams(ChannelStreams)));
}

MainCentralWidget::~MainCentralWidget()
{
}

void MainCentralWidget::UpdateStreamRule()
{
  stream_mgr_->UpdateStreamRule();
}

void MainCentralWidget::InsertRow(const std::string &vid, const std::string &cname, const std::string &stream)
{
  stream_table_->insertRow(stream_table_->rowCount());
  int row = stream_table_->rowCount() - 1;
  QTableWidgetItem *vid_item = new QTableWidgetItem(vid.c_str());
  QTableWidgetItem *cname_item = new QTableWidgetItem(cname.c_str());
  QTableWidgetItem *stream_item = new QTableWidgetItem(stream.c_str());

  stream_table_->setItem(row, 0, vid_item);
  stream_table_->setItem(row, 1, cname_item);
  stream_table_->setItem(row, 2, stream_item);
}

void MainCentralWidget::RemoveRow(const std::string &vid, const std::string &cname, const std::string &stream)
{
  if (stream.empty()) {
    QList<QTableWidgetItem *> find_items = stream_table_->findItems(cname.c_str(), Qt::MatchExactly);
    foreach (QTableWidgetItem *item, find_items) {
      int row = item->row();
      if (stream_table_->item(row, 0)->text().toStdString() != vid) continue;
      stream_table_->removeRow(row);
    }
    return;
  }

  QList<QTableWidgetItem *> find_items = stream_table_->findItems(stream.c_str(), Qt::MatchExactly);
  foreach (QTableWidgetItem *item, find_items) {
    int row = item->row();
    if (stream_table_->item(row, 0)->text().toStdString() != vid) continue;
    if (stream_table_->item(row, 1)->text().toStdString() != cname) continue;
    stream_table_->removeRow(item->row());
  }
}

void MainCentralWidget::DiffChannelStreams(const ChannelStreams &channel_streams)
{
  // 1. check channel quit/change
  for (auto channel : channel_streams_) {
    auto iter = channel_streams.find(channel.first);
    if (iter == channel_streams.end()) {
      RemoveRow(channel.first.first, channel.first.second, "");
      continue;
    }

    for (auto stream : channel.second) {
      auto stream_iter = std::find(iter->second.begin(), iter->second.end(), stream);
      if (stream_iter == iter->second.end()) {
        RemoveRow(channel.first.first, channel.first.second, stream);
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
      auto stream_iter = std::find(iter->second.begin(), iter->second.end(), stream);
      if (stream_iter == iter->second.end()) {
        InsertRow(channel.first.first, channel.first.second, stream);
      }
    }
  }
}

void MainCentralWidget::OnPlay(QTableWidgetItem *item)
{
  std::string cmd;
  std::string stream = item->text().toStdString();
  if (stream.empty() || stream.find("rtmp") == std::string::npos) {
    return;
  }
  stream = stream_mgr_->ConvertToPlayUrl(stream);

  QSettings settings("agora.io", "gump");
  settings.beginGroup("preferences");
  cmd = settings.value("player").toString().toStdString();
  settings.endGroup();
  if (cmd.empty()) return;
  if (cmd.find("ffplay") != std::string::npos) {
    cmd += " -v quiet ";
  }

  cmd += stream;
  std::cout << cmd << std::endl;
  stream_mgr_->PlayStream(cmd);
}

void MainCentralWidget::RefreshStreams(ChannelStreams channel_streams)
{
  DiffChannelStreams(channel_streams);
  channel_streams_.swap(channel_streams);
}

// Copyright (c) 2014-2019 winking324
//


#include "view/playlist_widget.h"

#include <set>
#include <QIcon>
#include <QHeaderView>

#include "commons/singleton.h"
#include "controller/online_controller.h"
#include "controller/stream_rule_controller.h"


namespace gump {


PlaylistWidget::PlaylistWidget(QWidget *parent)
  : QWidget(parent) {
  search_edit_ = new LineEdit(QIcon(":/search_16px.png"), QSize(16, 16), this);

  tag_group_ = new QButtonGroup(this);

  stream_table_ = new QTableWidget(this);
  stream_table_->setStyleSheet("color:gray");
  stream_table_->setContextMenuPolicy(Qt::CustomContextMenu);
  stream_table_->setColumnCount(2);
  stream_table_->setHorizontalHeaderLabels(
        QStringList{"NAME", "STREAM"});
  stream_table_->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::ResizeToContents);
  stream_table_->horizontalHeader()->setSectionResizeMode(
        1, QHeaderView::Stretch);

  tag_layout_ = new QHBoxLayout();
  tag_layout_->setSpacing(5);
  tag_layout_->addStretch();

  layout_ = new QVBoxLayout(this);
  layout_->addWidget(search_edit_);
  layout_->addItem(tag_layout_);
  layout_->addWidget(stream_table_);
  layout_->setMargin(5);
  layout_->setSpacing(5);
  setLayout(layout_);

  player_ = new PlayerWidget(this);
  player_->setGeometry(360 - 140, 640 - 180, 120, 160);

  connect(tag_group_, SIGNAL(buttonToggled(QAbstractButton*, bool)),
          this, SLOT(OnTagToggled(QAbstractButton*, bool)));
  connect(stream_table_, SIGNAL(itemClicked(QTableWidgetItem*)), this,
          SLOT(OnItemSelected(QTableWidgetItem*)));
}

void PlaylistWidget::OnTags(std::map<QString, QString> tags) {
  bool refresh = true;
  auto all_tags = tag_group_->buttons();
  if (all_tags.size() == static_cast<int>(tags.size())) {
    refresh = false;
    for (auto *tag : all_tags) {
      if (tags.find(tag->text()) == tags.end()) {
        refresh = true;
        break;
      }
    }
  }

  if (!refresh) return;

  QString checked_name;
  auto *checked_tag = dynamic_cast<Tag *>(tag_group_->checkedButton());
  if (checked_tag != nullptr) {
    checked_name = checked_tag->text();
  }

  for (auto *tag : all_tags) {
    tag_group_->removeButton(tag);
    tag_layout_->removeWidget(tag);
    tag->deleteLater();
  }

  auto count = 0;
  static const int kMaxShowCount = 6;
  for (const auto &tag_info : tags) {
    auto *tag = new Tag(tag_info.first, tag_info.second, this);

    if (checked_name == tag->text()) {
      tag->setChecked(true);
    }

    tag_group_->addButton(tag);
    tag_layout_->insertWidget(0, tag);
    if (count < kMaxShowCount) tag->show();
    ++count;
  }

  checked_tag = dynamic_cast<Tag *>(tag_group_->checkedButton());
  if (checked_tag == nullptr) {
    auto *online_controller = Singleton<OnlineController>::Instance();
    online_controller->UpdateInfo("", "");
  }
}

void PlaylistWidget::OnTagToggled(QAbstractButton *tag, bool checked) {
  if (checked) {
    auto *checked_tag = dynamic_cast<Tag *>(tag);
    auto *online_controller = Singleton<OnlineController>::Instance();
    online_controller->UpdateInfo(checked_tag->text(),
                                  checked_tag->GetDetails());
  }
}

void PlaylistWidget::OnUpdateList(MultiStreams streams) {

  auto current_row = stream_table_->currentRow();
  QString selected_stream;
  if (current_row > 0) {
    selected_stream = stream_table_->item(current_row, 1)->text();
  }

  for (auto channel : streams_) {
    auto iter = streams.find(channel.first);
    if (iter == streams.end()) {
      RemoveTableRow(channel.first, QUrl(""));
      continue;
    }

    for (auto stream : channel.second) {
      auto stream_iter = std::find_if(iter->second.begin(), iter->second.end(),
                                      [&](const Stream &item){
        return item.stream_url == stream.stream_url;
      });
      if (stream_iter == iter->second.end()) {
        RemoveTableRow(channel.first, stream.stream_url);
      }
    }
  }

  for (auto channel : streams) {
    auto iter = streams_.find(channel.first);
    if (iter == streams_.end()) {
      for (auto stream : channel.second) {
        InsertTableRow(channel.first, stream.stream_url);
      }
      continue;
    }

    for (auto stream : channel.second) {
      auto stream_iter = std::find_if(iter->second.begin(), iter->second.end(),
                                      [&](const Stream &item){
        return item.stream_url == stream.stream_url;
      });
      if (stream_iter == iter->second.end()) {
        InsertTableRow(channel.first, stream.stream_url);
      }
    }
  }

  if (!selected_stream.isEmpty()) {
    OnSearchItem(selected_stream);
  }

  streams_.swap(streams);
}

void PlaylistWidget::OnSearchItem(const QString &search) {
  QList<QTableWidgetItem *> find_items =
      stream_table_->findItems(search, Qt::MatchContains);
  if (find_items.empty()) return;

  stream_table_->selectRow((*find_items.begin())->row());
}

void PlaylistWidget::OnItemSelected(QTableWidgetItem *item) {
  stream_table_->selectRow(item->row());

  QString play_url = GetPlayUrl(item->row());
  if (play_url.isEmpty()) return;

  player_->PlayStream(play_url);

  if (item->row() > 0) {
    QString buffer_url = GetPlayUrl(item->row() - 1);
    if (!buffer_url.isEmpty()) {
      player_->BufferStream(buffer_url);
    }
  }

  if (item->row() + 1 < stream_table_->rowCount()) {
    QString buffer_url = GetPlayUrl(item->row() + 1);
    if (!buffer_url.isEmpty()) {
      player_->BufferStream(buffer_url);
    }
  }
}

void PlaylistWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() != Qt::Key_Up &&
      event->key() != Qt::Key_Down) {
    return;
  }

  int row = stream_table_->currentRow();
  QTableWidgetItem *item = stream_table_->item(row, 1);
  OnItemSelected(item);
}

void PlaylistWidget::InsertTableRow(const QString &name,
                                    const QUrl &stream) {
  stream_table_->insertRow(stream_table_->rowCount());
  int row = stream_table_->rowCount() - 1;
  QTableWidgetItem *name_item = new QTableWidgetItem(name);
  QTableWidgetItem *stream_item = new QTableWidgetItem(stream.fileName());

  name_item->setFlags(stream_item->flags() & ~Qt::ItemIsEditable);
  stream_item->setFlags(stream_item->flags() & ~Qt::ItemIsEditable);

  name_item->setToolTip(name);
//  stream_item->setToolTip(config_mgr_->ConvertToToolTip(stream).c_str());

  stream_table_->setItem(row, 0, name_item);
  stream_table_->setItem(row, 1, stream_item);
}

void PlaylistWidget::RemoveTableRow(const QString &name,
                                    const QUrl &stream) {
  if (stream.isEmpty()) {
    QList<QTableWidgetItem *> find_items =
        stream_table_->findItems(name, Qt::MatchExactly);

    std::set<int> row_to_remove;
    foreach (QTableWidgetItem *item, find_items) {
      row_to_remove.insert(item->row());
    }

    for (auto it = row_to_remove.rbegin(); it != row_to_remove.rend(); ++it) {
      stream_table_->removeRow(*it);
    }
    return;
  }

  QList<QTableWidgetItem *> find_items =
      stream_table_->findItems(stream.fileName(), Qt::MatchExactly);
  foreach (QTableWidgetItem *item, find_items) {
    int row = item->row();
    if (stream_table_->item(row, 0)->text() != name) continue;
    stream_table_->removeRow(item->row());
  }
}

QString PlaylistWidget::GetPlayUrl(int row) {
  QString name = stream_table_->item(row, 0)->text();
  QString stream = stream_table_->item(row, 1)->text();

  auto channel_it = streams_.find(name);
  if (channel_it == streams_.end()) return "";

  auto stream_it =
      std::find_if(channel_it->second.begin(), channel_it->second.end(),
                   [&](const Stream &detail){
    return stream == detail.stream_url.fileName();
  });
  if (stream_it == channel_it->second.end()) return "";

  return Singleton<StreamRuleController>::Instance()->ConvertToPlayUrl(
        stream_it->stream_url.toString());
}


}  // namespace gump

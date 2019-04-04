// Copyright (c) 2014-2019 winking324
//


#include "view/playlist_widget.h"

#include <set>
#include <QIcon>
#include <QHeaderView>

#include "commons/singleton.h"
#include "controller/online_controller.h"
#include "controller/player_controller.h"
#include "controller/stream_rule_controller.h"


namespace gump {


PlaylistWidget::PlaylistWidget(QWidget *parent)
  : QWidget(parent) {
  search_edit_ = new LineEdit(QIcon(":/search_16px.png"), QSize(16, 16), this);

  tag_group_ = new QButtonGroup(this);

  stream_table_ = new QTableWidget(this);
  stream_table_->setStyleSheet("color:gray");
  stream_table_->setContextMenuPolicy(Qt::CustomContextMenu);
  stream_table_->setColumnCount(1);
  stream_table_->setHorizontalHeaderLabels(QStringList{"STREAM"});
  stream_table_->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::Stretch);

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
  connect(stream_table_, SIGNAL(itemClicked(QTableWidgetItem*)),
          this, SLOT(OnItemClicked(QTableWidgetItem*)));
  connect(stream_table_, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
          this, SLOT(OnItemDoubleClicked(QTableWidgetItem*)));

  auto *online_controller = Singleton<OnlineController>::Instance();
  connect(online_controller, SIGNAL(RemoveAllEvent()),
          this, SLOT(OnRemoveAll()));
  connect(online_controller, SIGNAL(InsertStreamEvent(QString)),
          this, SLOT(OnInsertStream(QString)));
  connect(online_controller, SIGNAL(RemoveStreamEvent(QString)),
          this, SLOT(OnRemoveStream(QString)));
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

void PlaylistWidget::OnRemoveAll() {
  for (int i = stream_table_->rowCount(); i > 0; --i) {
    stream_table_->removeRow(i - 1);
  }
}

void PlaylistWidget::OnInsertStream(QString stream) {
  stream_table_->insertRow(stream_table_->rowCount());
  int row = stream_table_->rowCount() - 1;

  QTableWidgetItem *stream_item = new QTableWidgetItem(stream);

  stream_item->setFlags(stream_item->flags() & ~Qt::ItemIsEditable);

//  stream_item->setToolTip(config_mgr_->ConvertToToolTip(stream).c_str());

  stream_table_->setItem(row, 0, stream_item);
}

void PlaylistWidget::OnRemoveStream(QString stream) {
  QList<QTableWidgetItem *> find_items =
      stream_table_->findItems(stream, Qt::MatchExactly);
  foreach (QTableWidgetItem *item, find_items) {
    stream_table_->removeRow(item->row());
  }
}

void PlaylistWidget::OnSearchItem(const QString &search) {
  QList<QTableWidgetItem *> find_items =
      stream_table_->findItems(search, Qt::MatchContains);
  if (find_items.empty()) return;

  stream_table_->selectRow((*find_items.begin())->row());
}

void PlaylistWidget::OnItemClicked(QTableWidgetItem *item) {
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

void PlaylistWidget::OnItemDoubleClicked(QTableWidgetItem *) {
  emit ItemSelectedEvent();
}

void PlaylistWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() != Qt::Key_Up &&
      event->key() != Qt::Key_Down) {
    return;
  }

  OnItemClicked(stream_table_->currentItem());
}

void PlaylistWidget::showEvent(QShowEvent *) {
  auto *player_controller = Singleton<PlayerController>::Instance();

  QString stream = player_controller->GetCurrentStream();
  if (stream.isEmpty()) return;

  OnSearchItem(stream);
}

QString PlaylistWidget::GetPlayUrl(int row) {
  QString stream = stream_table_->item(row, 0)->text();

  QString stream_url;
  if (!Singleton<OnlineController>::Instance()->QueryStream(stream, &stream_url,
                                                            nullptr)) {
    return "";
  }

  return Singleton<StreamRuleController>::Instance()->ConvertToPlayUrl(
        stream_url);
}


}  // namespace gump

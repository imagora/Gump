// Copyright (c) 2014-2019 winking324
//


#include "view/playlist_widget.h"

#include <QIcon>
#include <QHeaderView>

#include "commons/singleton.h"
#include "controller/online_controller.h"


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


}  // namespace gump

// Copyright (c) 2014-2019 winking324
//


#include "view/playlist_widget.h"

#include <QIcon>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace gump {


PlaylistWidget::PlaylistWidget(QWidget *parent)
  : QWidget(parent) {
  auto *layout = new QVBoxLayout(this);

  search_edit_ = new LineEdit(QIcon(":/search_16px.png"), QSize(16, 16), this);

  stream_table_ = new QTableWidget(this);
  stream_table_->setStyleSheet("color:gray");
  stream_table_->setContextMenuPolicy(Qt::CustomContextMenu);
  stream_table_->setColumnCount(3);
  stream_table_->setHorizontalHeaderLabels(
        QStringList{"VID", "NAME", "STREAM"});
  stream_table_->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::ResizeToContents);
  stream_table_->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::Stretch);

  auto *tag = new Tag("one", "1", this);
  auto *tag2 = new Tag("two", "2", this);
  auto tag_layout = new QHBoxLayout(this);
  tag_layout->addWidget(tag, 0, Qt::AlignLeft);
  tag_layout->addWidget(tag2, 0, Qt::AlignLeft);
  tag_layout->setSpacing(5);
  tag_layout->addStretch();

  layout->addWidget(search_edit_);
  layout->addItem(tag_layout);
  layout->addWidget(stream_table_);
  layout->setMargin(5);
  layout->setSpacing(5);
  setLayout(layout);

  player_ = new PlayerWidget(this);
  player_->setGeometry(360 - 140, 640 - 180, 120, 160);
}


}  // namespace gump

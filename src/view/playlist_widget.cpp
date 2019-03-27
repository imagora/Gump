// Copyright (c) 2014-2019 winking324
//


#include "view/playlist_widget.h"

#include <QIcon>
#include <QHeaderView>
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

  layout->addWidget(search_edit_);
  layout->addWidget(stream_table_);
  setLayout(layout);

  player_ = new PlayerWidget(this);
  player_->setGeometry(360 - 140, 640 - 180, 120, 160);
}


}  // namespace gump

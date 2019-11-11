// Copyright (c) 2014-2019 winking324
//

#include "view/logged_widget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

#include "commons/singleton.h"
#include "controller/online_controller.h"


namespace gump {


LoggedWidget::LoggedWidget(QWidget *parent) : QWidget(parent) {
  stream_table_ = new QTableWidget(this);
  stream_table_->setContextMenuPolicy(Qt::CustomContextMenu);
  stream_table_->setColumnCount(3);
  stream_table_->setHorizontalHeaderLabels(
        QStringList{"VID", "CNAME", "STREAM"});
  stream_table_->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::ResizeToContents);
  stream_table_->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::Stretch);

  player_widget_ = new PlayerWidget(this);

  QHBoxLayout *layout = new QHBoxLayout();
  QVBoxLayout *table_layout = new QVBoxLayout();
  table_layout->addWidget(stream_table_);
  layout->addLayout(table_layout);
  layout->addWidget(player_widget_);
  setLayout(layout);

  connect(stream_table_, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this,
          SLOT(OnPlay(QTableWidgetItem*)));
  connect(stream_table_, SIGNAL(customContextMenuRequested(QPoint)), this,
          SLOT(OnMenu(QPoint)));

  auto *online_controller = Singleton<OnlineController>::Instance();
  connect(online_controller, SIGNAL(RemoveAllEvent()),
            this, SLOT(OnRemoveAll()));
  connect(online_controller, SIGNAL(InsertStreamEvent(QString)),
          this, SLOT(OnInsertStream(QString)));
  connect(online_controller, SIGNAL(RemoveStreamEvent(QString)),
          this, SLOT(OnRemoveStream(QString)));
}


}  // namespace gump


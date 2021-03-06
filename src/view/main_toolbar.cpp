// Copyright (c) 2014-2019 winking324
//

#include "view/main_toolbar.h"

#include <log4cplus/log4cplus.h>

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QSize>
#include <QToolButton>

#include "commons/definations.h"

namespace gump {

MainToolBar::MainToolBar(QWidget *parent) : QToolBar(parent) {
  setMovable(false);

  QWidget *tool_bar = new QWidget(this);
  QHBoxLayout *tool_bar_layout = new QHBoxLayout(tool_bar);

  QLabel *search_label = new QLabel("Search:", tool_bar);
  search_edit_ = new QLineEdit(tool_bar);
  QToolButton *search_btn = new QToolButton(tool_bar);
  search_btn->setIcon(QIcon(":/search.png"));
  QToolButton *play_btn = new QToolButton(tool_bar);
  play_btn->setIcon(QIcon(":/play.png"));
  QToolButton *pause_btn = new QToolButton(tool_bar);
  pause_btn->setIcon(QIcon(":/pause.png"));
  QToolButton *stop_btn = new QToolButton(tool_bar);
  stop_btn->setIcon(QIcon(":/stop.png"));
  QToolButton *details_btn = new QToolButton(tool_bar);
  details_btn->setIcon(QIcon(":/details.png"));

  tool_bar_layout->addWidget(search_label);
  tool_bar_layout->addWidget(search_edit_);
  tool_bar_layout->addWidget(search_btn);

  tool_bar_layout->addWidget(play_btn);
  tool_bar_layout->addWidget(pause_btn);
  tool_bar_layout->addWidget(stop_btn);

  tool_bar_layout->addWidget(details_btn);

  tool_bar->setLayout(tool_bar_layout);
  addWidget(tool_bar);

  connect(search_edit_, SIGNAL(returnPressed()), this, SLOT(OnSearch()));
  connect(search_btn, SIGNAL(released()), this, SLOT(OnSearch()));
  connect(play_btn, SIGNAL(released()), this, SLOT(OnPlay()));
  connect(pause_btn, SIGNAL(released()), this, SLOT(OnPause()));
  connect(stop_btn, SIGNAL(released()), this, SLOT(OnStop()));
  connect(details_btn, SIGNAL(released()), this, SLOT(OnDetails()));
}

MainToolBar::~MainToolBar() {}

void MainToolBar::OnSearch() {
  emit SearchItem(search_edit_->text().toStdString());
}

void MainToolBar::OnPlay() { emit PlayStream(); }

void MainToolBar::OnPause() { emit PauseStream(); }

void MainToolBar::OnStop() { emit StopStream(); }

void MainToolBar::OnDetails() { emit ShowDetails(); }

}  // namespace gump

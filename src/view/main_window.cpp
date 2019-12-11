// Copyright (c) 2014-2019 winking324
//

#include "view/main_window.h"

#include <log4cplus/log4cplus.h>

#include <QFileOpenEvent>
#include <QIcon>

#include "controller/search_event.h"

namespace gump {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Gump");
  setWindowIcon(QIcon(":/icon.png"));

  log_dock_ = new LogDock(this);
  addDockWidget(Qt::BottomDockWidgetArea, log_dock_);

  menu_ = new MainMenuBar(this);
  toolbar_ = new MainToolBar(this);
  central_widget_ = new MainCentralWidget(this);

  addToolBar(toolbar_);
  setCentralWidget(central_widget_);

  log_appender_ = log4cplus::SharedAppenderPtr(new LogDockAppender(log_dock_));
  log4cplus::Logger::getInstance(kLoggerName).addAppender(log_appender_);

  setWindowState(Qt::WindowMaximized);
  connect(toolbar_, SIGNAL(SearchItem(std::string)), this,
          SLOT(SearchItem(std::string)));
  connect(toolbar_, SIGNAL(PlayStream()), this, SLOT(PlayStream()));
  connect(toolbar_, SIGNAL(PauseStream()), this, SLOT(PauseStream()));
  connect(toolbar_, SIGNAL(StopStream()), this, SLOT(StopStream()));
  connect(toolbar_, SIGNAL(ShowDetails()), this, SLOT(ShowDetails()));
}

MainWindow::~MainWindow() {}

void MainWindow::UpdatePreferences() { central_widget_->UpdatePreferences(); }

void MainWindow::SearchItem(std::string search) {
  central_widget_->SearchItem(search);
}

void MainWindow::PlayStream() { central_widget_->PlayStream(); }

void MainWindow::PauseStream() { central_widget_->PauseStream(); }

void MainWindow::StopStream() { central_widget_->StopStream(); }

void MainWindow::ShowDetails() { central_widget_->ShowDetails(); }

bool MainWindow::event(QEvent *event) {
  if (event->type() == kSearchEventType) {
    SearchEvent *search_event = dynamic_cast<SearchEvent *>(event);
    SearchItem(search_event->GetSearchInfo().toStdString());
    return true;
  }

  return QMainWindow::event(event);
}

void MainWindow::closeEvent(QCloseEvent *) {
  log_appender_->destructorImpl();
  log4cplus::Logger::getInstance(kLoggerName).removeAllAppenders();
}

void MainWindow::moveEvent(QMoveEvent *) {
  if (this->isVisible()) {
    central_widget_->WindowMove();
  }
}

}  // namespace gump

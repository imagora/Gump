// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QWidget>
#include <QTableWidget>
#include <QButtonGroup>
#include <QTableWidgetItem>
#include "view/player_widget.h"


namespace gump {


class LoggedWidget : public QWidget {
  Q_OBJECT
 public:
  explicit LoggedWidget(QWidget *parent = nullptr);

 signals:
  void ItemSelectedEvent();

 public slots:
  void OnRemoveAll();

  void OnInsertStream(QString stream);

  void OnRemoveStream(QString stream);

  void OnSearchItem(const QString &search);

  void OnItemClicked(QTableWidgetItem *item);

  void OnItemDoubleClicked(QTableWidgetItem *item);

 protected:
  virtual void keyReleaseEvent(QKeyEvent *event) override;

  virtual void showEvent(QShowEvent *event) override;

 private:
  PlayerWidget *player_widget_;
  QTableWidget *stream_table_;
};


}  // namespace gump

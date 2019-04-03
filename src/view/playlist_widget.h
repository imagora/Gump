// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QUrl>
#include <vector>
#include <QWidget>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QButtonGroup>

#include "commons/stream_info.h"

#include "view/tag.h"
#include "view/line_edit.h"
#include "view/player_widget.h"


namespace gump {


class PlaylistWidget : public QWidget {
  Q_OBJECT
 public:
  explicit PlaylistWidget(QWidget *parent = nullptr);

 signals:
  void TagChangeEvent(QString tag_name, QString tag_info);

 public slots:
  void OnTags(std::map<QString, QString> tags);

  void OnTagToggled(QAbstractButton *tag, bool checked);

  void OnUpdateList(MultiStreams streams);

  void OnSearchItem(const QString &search);

  void OnItemSelected(QTableWidgetItem *item);

 protected:
 void keyReleaseEvent(QKeyEvent *event) override;

 private:
  void InsertTableRow(const QString &name, const QUrl &stream);

  void RemoveTableRow(const QString &name, const QUrl &stream);

  QString GetPlayUrl(int row);

 private:
  PlayerWidget *player_;
  LineEdit *search_edit_;
  QTableWidget *stream_table_;
  QButtonGroup *tag_group_;

  QVBoxLayout *layout_;
  QHBoxLayout *tag_layout_;

  MultiStreams streams_;
};


}  // namespace gump


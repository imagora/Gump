// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <vector>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QButtonGroup>

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

 private:
  PlayerWidget *player_;
  LineEdit *search_edit_;
  QTableWidget *stream_table_;
  QButtonGroup *tag_group_;

  QVBoxLayout *layout_;
  QHBoxLayout *tag_layout_;
};


}  // namespace gump


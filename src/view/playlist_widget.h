// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QWidget>
#include <QTableWidget>

#include "view/line_edit.h"
#include "view/player_widget.h"


namespace gump {


class PlaylistWidget : public QWidget {
  Q_OBJECT
public:
  explicit PlaylistWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
  PlayerWidget *player_;
  LineEdit *search_edit_;
  QTableWidget *stream_table_;
};


}  // namespace gump


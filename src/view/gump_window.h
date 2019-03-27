// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QMainWindow>
#include "view/central_widget.h"

namespace gump {


class GumpWindow :
    public QMainWindow {
  Q_OBJECT
 public:
  explicit GumpWindow(QWidget *parent = nullptr);

 signals:

 public slots:

 private:
  CentralWidget *central_widget_;
};


}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QWidget>
#include <QStackedLayout>

#include "view/login_widget.h"
#include "view/logging_widget.h"
#include "view/playlist_widget.h"


namespace gump {


class CentralWidget :
    public QWidget {
  Q_OBJECT
 public:
  explicit CentralWidget(QWidget *parent = nullptr);

 signals:

 public slots:
  void OnLogin(QString username, QString password);

 private:
  LoginWidget *login_widget_;
  LoggingWidget *logging_widget_;
  PlaylistWidget *playlist_widget_;
  QStackedLayout *stacked_layout_;
};


}  // namespace gump

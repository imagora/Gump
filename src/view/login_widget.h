// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QLabel>
#include <QWidget>
#include <QString>

#include "view/line_edit.h"


namespace gump {


class LoginWidget :
    public QWidget {
  Q_OBJECT
 public:
  explicit LoginWidget(QWidget *parent = nullptr);

  void SetUsername(const QString &username);

  void SetErrorMessage(const QString &msg);

 signals:
  void Login(QString username);

 public slots:
  void OnLogin();

 private:
  LineEdit *username_input_;
  QLabel *error_label_;
};


}  // namespace gump

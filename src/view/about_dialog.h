// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QDialog>


namespace gump {


class AboutDialog :
    public QDialog {

 public:
  explicit AboutDialog(QWidget *parent = nullptr);

  virtual ~AboutDialog();
};


}  // namespace gump

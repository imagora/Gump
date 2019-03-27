// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QIcon>
#include <QLineEdit>


namespace gump {


class LineEdit :
    public QLineEdit {
  Q_OBJECT
 public:
  explicit LineEdit(const QIcon &icon, const QSize &size,
                    QWidget *parent = nullptr);
};


}  // namespace gump

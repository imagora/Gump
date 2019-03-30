// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QWidget>
#include <QString>
#include <QPushButton>


namespace gump {


class Tag : public QPushButton {
  Q_OBJECT
 public:
  explicit Tag(const QString &text, const QString &details,
               QWidget *parent = nullptr);

  QString GetDetails() const;

  void SetDetails(const QString &details);

 private:
  QString details_;
};


}  // namespace gump

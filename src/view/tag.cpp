// Copyright (c) 2014-2019 winking324
//

#include "view/tag.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QFontMetrics>

namespace gump {


Tag::Tag(const QString &text, const QString &details, QWidget *parent)
  : QPushButton(text, parent) {
  details_ = details;

  setCheckable(true);
  setAutoExclusive(true);

  QString style("QPushButton{border:2px groove gray;"
                "border-radius:6px;height:12px;color:gray;} "
                "QPushButton:hover{color:white;} "
                "QPushButton:checked{color:white;background:gray;}"
                "QPushButton:pressed{color:white;background:gray;}");

  setStyleSheet(style);
  setFixedWidth(sizeHint().width() + 6);
  setFixedHeight(sizeHint().height() + 2);
}

QString Tag::GetDetails() const {
  return details_;
}

void Tag::SetDetails(const QString &details) {
  details_ = details;
}


}  // namespace gump

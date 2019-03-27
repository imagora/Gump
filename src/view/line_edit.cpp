// Copyright (c) 2014-2019 winking324
//

#include "view/line_edit.h"

#include <QLabel>
#include <QHBoxLayout>


namespace gump {


LineEdit::LineEdit(const QIcon &icon, const QSize &size, QWidget *parent)
  : QLineEdit (parent) {
  if (icon.isNull()) {
    return;
  }

  auto *label = new QLabel(this);
  label->setPixmap(icon.pixmap(size));

  QString style("QLineEdit{padding-left:%1px;border-radius:%2px;height:%3px;}");

  int height = size.height();
  int radius = height / 2;
  int padding_left = height + height / 4;
  style = style.arg(QString::number(padding_left), QString::number(radius),
                    QString::number(height));
  setStyleSheet(style);

  setFrame(false);
  setAttribute(Qt::WA_MacShowFocusRect, 0);
}


}  // namespace gump

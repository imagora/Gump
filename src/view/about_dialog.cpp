// Copyright (c) 2014-2019 winking324
//
#include "about_dialog.h"

#include <QIcon>
#include <QLabel>
#include <QGridLayout>

#include "commons/definations.h"


namespace gump {


AboutDialog::AboutDialog(QWidget *parent/* = nullptr*/)
  : QDialog(parent) {
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(":/icon.png"));
  setWindowTitle("About");
  setMinimumSize(400, 200);
  setMaximumSize(400, 200);

  QGridLayout *layout = new QGridLayout(this);
  QLabel *icon_label = new QLabel(this);
  icon_label->setPixmap(QPixmap(":/icon.png"));
  layout->addWidget(icon_label, 0, 0, 1, 1);

  QLabel *about_label =
      new QLabel(tr("Life was like a box of chocolates,\n") +
                 tr("you never know what you’re gonna get."),
                 this);

  QLabel *version_label =
      new QLabel(tr("Version: ") + tr(BUILD_VERSION) +
                 tr("\nBuilt: ") + tr(BUILD_DATE_TIME) +
                 tr("\nHuiShan, DongxuCheng, ZhengchangSun") +
                 tr("\nCopyright 2017 agora.io"), this);

  layout->addWidget(about_label, 0, 1, 1, 1);
  layout->addWidget(version_label, 1, 1, 1, 1);
  setLayout(layout);
}


}  // namespace gump

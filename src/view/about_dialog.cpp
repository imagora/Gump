#include <QIcon>
#include <QLabel>
#include <QGridLayout>
#include "about_dialog.h"


using namespace gump;


AboutDialog::AboutDialog(QWidget *parent/* = nullptr*/)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/icon.png"));
    setWindowTitle("About");
    setMinimumSize(400, 200);
    setMaximumSize(400, 200);

    QGridLayout *layout = new QGridLayout(this);
    QLabel *icon_label = new QLabel(this);
    icon_label->setPixmap(QPixmap(":/icon.png"));
    layout->addWidget(icon_label, 0, 0, 1, 1);


    QLabel *about_label = new QLabel(tr("Life was like a box of chocolates,\n") +
                                     tr("you never know what you’re gonna get."),
                                     this);

    QLabel *version_label = new QLabel("Version: 0.1 by shanhui&chengdongxu\n"
                                       "Copyright 2017 Agora.io", this);

    layout->addWidget(about_label, 0, 1, 1, 1);
    layout->addWidget(version_label, 1, 1, 1, 1);
    setLayout(layout);
}


AboutDialog::~AboutDialog()
{

}


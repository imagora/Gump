#pragma once
#include <QDialog>


namespace gump {


class AboutDialog : public QDialog
{
public:
    AboutDialog(QWidget *parent = nullptr);


    virtual ~AboutDialog();
};


}

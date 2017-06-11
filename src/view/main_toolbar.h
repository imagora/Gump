#pragma once
#include <QToolBar>


class QLineEdit;


namespace gump {


class MainToolBar : public QToolBar
{
  Q_OBJECT

 public:
  MainToolBar(QWidget *parent = nullptr);

  ~MainToolBar();

 signals:
  void SearchItem(std::string vid, std::string cname, std::string stream);

 private slots:
  void OnSearch();

 private:
  QLineEdit *search_edit_;

};


}


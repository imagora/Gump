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

  void PlayStream();

  void PauseStream();

  void StopStream();

  void ShowDetails();

 private slots:
  void OnSearch();

  void OnPlay();

  void OnPause();

  void OnStop();

  void OnDetails();

 private:
  QLineEdit *search_edit_;

};


}


// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QToolBar>
#include <QLineEdit>

#include <string>


namespace gump {


class MainToolBar :
    public QToolBar {
  Q_OBJECT

 public:
  explicit MainToolBar(QWidget *parent = nullptr);

  virtual ~MainToolBar();

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


}  // namespace gump


// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>

namespace gump {

enum class SearchType : int {
  kSearchCName,
  kSearchIp,
  kSearchUrl,
};

class SearchDialog : public QDialog {
 public:
  explicit SearchDialog(SearchType type, QWidget *parent = nullptr);

  QString SearchInfo();

 private:
  void InitVendorWidgets(int *row);

  void InitCNameWidgets(int *row);

  void InitIpWidgets(int *row);

  void InitUrlWidgets(int *row);

  bool CheckInput();

 private:
  SearchType type_;

  QLineEdit *vendor_;
  QLineEdit *cname_;
  QLineEdit *ip_;
  QLineEdit *url_;

  QDialogButtonBox *confirm_btns_;
  QGridLayout *layout_;
};

}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>

namespace gump {

enum class SearchType : int {
  kSearchCName = 0x1,
  kSearchUid = 0x2,
  kSearchIp = 0x4,
  kSearchUrl = 0x8,

  kSearchAll = 0xF,
};

class SearchDialog : public QDialog {
 public:
  explicit SearchDialog(SearchType type = SearchType::kSearchAll,
                        QWidget *parent = nullptr);

  QString SearchInfo();

 private:
  void InitVendorWidgets(int *row);

  void InitCNameWidgets(int *row);

  void InitUidWidgets(int *row);

  void InitIpWidgets(int *row);

  void InitUrlWidgets(int *row);

 private:
  QLineEdit *vendor_;
  QLineEdit *cname_;
  QLineEdit *uid_;
  QLineEdit *ip_;
  QLineEdit *url_;

  QDialogButtonBox *confirm_btns_;
  QGridLayout *layout_;
};

}  // namespace gump

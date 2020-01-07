// Copyright (c) 2014-2019 winking324
//

#include "view/search_dialog.h"

#include <QLabel>

namespace gump {

SearchDialog::SearchDialog(SearchType type, QWidget *parent) : QDialog(parent) {
  vendor_ = nullptr;
  cname_ = nullptr;
  uid_ = nullptr;
  ip_ = nullptr;
  url_ = nullptr;

  layout_ = new QGridLayout(this);

  confirm_btns_ = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

  int row = 0;
  if (static_cast<int>(type) & static_cast<int>(SearchType::kSearchCName)) {
    InitVendorWidgets(&row);
    InitCNameWidgets(&row);
  }

  if (static_cast<int>(type) & static_cast<int>(SearchType::kSearchUid)) {
    InitVendorWidgets(&row);
    InitUidWidgets(&row);
  }

  if (static_cast<int>(type) & static_cast<int>(SearchType::kSearchIp)) {
    InitIpWidgets(&row);
  }

  if (static_cast<int>(type) & static_cast<int>(SearchType::kSearchUrl)) {
    InitUrlWidgets(&row);
  }

  layout_->addWidget(confirm_btns_, row++, 1, 1, 2);
  setLayout(layout_);

  connect(confirm_btns_, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(confirm_btns_, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString SearchDialog::SearchInfo() {
  QString search_info;
  if (vendor_ != nullptr && !vendor_->text().isEmpty()) {
    search_info += "&vendor=" + vendor_->text().trimmed();
  }

  if (cname_ != nullptr && !cname_->text().isEmpty()) {
    search_info += "&cname=" + cname_->text().trimmed();
  }

  if (uid_ != nullptr && !uid_->text().isEmpty()) {
    search_info += "&uid=" + uid_->text().trimmed();
  }

  if (ip_ != nullptr && !ip_->text().isEmpty()) {
    search_info += "&ip=" + ip_->text().trimmed();
  }

  if (url_ != nullptr && !url_->text().isEmpty()) {
    search_info += "&url=" + url_->text().trimmed();
  }

  if (!search_info.isEmpty()) {
    search_info.remove(0, 1);
  }

  return search_info;
}

void SearchDialog::InitVendorWidgets(int *row) {
  vendor_ = new QLineEdit(this);
  layout_->addWidget(new QLabel(tr("Vendor"), this), *row, 0, 1, 1);
  layout_->addWidget(vendor_, *row, 1, 1, 2);
  ++(*row);
}

void SearchDialog::InitCNameWidgets(int *row) {
  cname_ = new QLineEdit(this);
  layout_->addWidget(new QLabel(tr("ChannelName"), this), *row, 0, 1, 1);
  layout_->addWidget(cname_, *row, 1, 1, 2);
  ++(*row);
}

void SearchDialog::InitUidWidgets(int *row) {
  uid_ = new QLineEdit(this);
  layout_->addWidget(new QLabel(tr("Uid"), this), *row, 0, 1, 1);
  layout_->addWidget(uid_, *row, 1, 1, 2);
  ++(*row);
}

void SearchDialog::InitIpWidgets(int *row) {
  ip_ = new QLineEdit(this);
  layout_->addWidget(new QLabel(tr("ServerIp"), this), *row, 0, 1, 1);
  layout_->addWidget(ip_, *row, 1, 1, 2);
  ++(*row);
}

void SearchDialog::InitUrlWidgets(int *row) {
  url_ = new QLineEdit(this);
  layout_->addWidget(new QLabel(tr("StreamUrl"), this), *row, 0, 1, 1);
  layout_->addWidget(url_, *row, 1, 1, 2);
  ++(*row);
}

}  // namespace gump

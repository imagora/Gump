// Copyright (c) 2014-2019 winking324
//

#include "view/search_dialog.h"

#include <QLabel>

namespace gump {

SearchDialog::SearchDialog(SearchType type, QWidget *parent) : QDialog(parent) {
  type_ = type;

  vendor_ = nullptr;
  cname_ = nullptr;
  ip_ = nullptr;
  url_ = nullptr;

  layout_ = new QGridLayout(this);

  confirm_btns_ = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

  int row = 0;
  switch (type_) {
    case SearchType::kSearchCName:
      InitVendorWidgets(&row);
      InitCNameWidgets(&row);
      break;
    case SearchType::kSearchIp:
      InitIpWidgets(&row);
      break;
    case SearchType::kSearchUrl:
      InitUrlWidgets(&row);
      break;
  }

  layout_->addWidget(confirm_btns_, row++, 1, 1, 2);
  setLayout(layout_);

  connect(confirm_btns_, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(confirm_btns_, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString SearchDialog::SearchInfo() {
  QString search_info;
  if (!CheckInput()) {
    return search_info;
  }

  if (vendor_ != nullptr && !vendor_->text().isEmpty()) {
    search_info += "&vendor=" + vendor_->text().trimmed();
  }

  if (cname_ != nullptr && !cname_->text().isEmpty()) {
    search_info +=
        "&cname=" + cname_->text().trimmed().toLocal8Bit().toBase64();
  }

  if (ip_ != nullptr && !ip_->text().isEmpty()) {
    search_info += "&ip=" + ip_->text().trimmed();
  }

  if (url_ != nullptr && !url_->text().isEmpty()) {
    search_info += "&url=" + url_->text().trimmed().toLocal8Bit().toBase64();
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

bool SearchDialog::CheckInput() {
  if (type_ == SearchType::kSearchCName &&
      (vendor_->text().isEmpty() || cname_->text().isEmpty())) {
    return false;
  }

  if (type_ == SearchType::kSearchIp && ip_->text().isEmpty()) {
    return false;
  }

  if (type_ == SearchType::kSearchUrl && url_->text().isEmpty()) {
    return false;
  }

  return true;
}

}  // namespace gump

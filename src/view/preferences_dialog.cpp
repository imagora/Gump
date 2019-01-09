// Copyright (c) 2014-2019 winking324
//

#include "view/preferences_dialog.h"

#include <QLabel>
#include <QPushButton>


namespace gump {


PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent) {
  QLabel *config_url_label = new QLabel("Rule URL", this);
  config_url_input_ = new QLineEdit(this);

  QLabel *username_label = new QLabel("Username", this);
  config_username_input_ = new QLineEdit(this);

  QLabel *password_label = new QLabel("Password", this);
  config_password_input_ = new QLineEdit(this);
  config_password_input_->setEchoMode(QLineEdit::PasswordEchoOnEdit);

  QLabel *vendor_id_label = new QLabel("VID", this);
  vendor_id_input_ = new QLineEdit(this);

  QLabel *max_load_label = new QLabel("Max", this);
  max_load_input_ = new QLineEdit(this);

  QLabel *player_label = new QLabel("External Player", this);
  external_player_input_ = new QLineEdit(this);

  QPushButton *ok_btn = new QPushButton("OK", this);
  QPushButton *cancel_btn = new QPushButton("Cancel", this);

  int row = 0;
  layout_ = new QGridLayout(this);
  layout_->addWidget(config_url_label, row, 0, 1, 1);
  layout_->addWidget(config_url_input_, row, 1, 1, 3);
  ++row;
  layout_->addWidget(username_label, row, 0, 1, 1);
  layout_->addWidget(config_username_input_, row, 1, 1, 1);
  ++row;
  layout_->addWidget(password_label, row, 0, 1, 1);
  layout_->addWidget(config_password_input_, row, 1, 1, 1);
  ++row;
  layout_->addWidget(vendor_id_label, row, 0, 1, 1);
  layout_->addWidget(vendor_id_input_, row, 1, 1, 3);
  ++row;
  layout_->addWidget(max_load_label, row, 0, 1, 1);
  layout_->addWidget(max_load_input_, row, 1, 1, 3);
  ++row;
  layout_->addWidget(player_label, row, 0, 1, 1);
  layout_->addWidget(external_player_input_, row, 1, 1, 3);
  ++row;
  layout_->addWidget(cancel_btn, row, 2, 1, 1);
  layout_->addWidget(ok_btn, row, 3, 1, 1);

  settings_ = new QSettings("agora.io", "gump", this);
  settings_->beginGroup("preferences");
  config_url_input_->setText(settings_->value("config_url").toString());
  config_username_input_->setText(
        settings_->value("config_username").toString());
  config_password_input_->setText(
        settings_->value("config_password").toString());
  vendor_id_input_->setText(settings_->value("vid", 0).toString());
  max_load_input_->setText(settings_->value("max", 100).toString());
  external_player_input_->setText(
        settings_->value("external_player").toString());
  settings_->endGroup();

  connect(ok_btn, SIGNAL(released()), this, SLOT(OnOk()));
  connect(cancel_btn, SIGNAL(released()), this, SLOT(OnCancel()));
}

PreferencesDialog::~PreferencesDialog() {
}

void PreferencesDialog::OnOk() {
  settings_->beginGroup("preferences");
  settings_->setValue("config_url", config_url_input_->text());
  settings_->setValue("config_username", config_username_input_->text());
  settings_->setValue("config_password", config_password_input_->text());
  settings_->setValue("vid", vendor_id_input_->text());
  settings_->setValue("max", max_load_input_->text());
  settings_->setValue("external_player", external_player_input_->text());
  settings_->endGroup();

  emit UpdatePreferences();
  close();
}

void PreferencesDialog::OnCancel() {
  close();
}


}  // namespace gump

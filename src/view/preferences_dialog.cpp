#include "preferences_dialog.h"
#include <QGridLayout>
#include <QSettings>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>


using namespace gump;


PreferencesDialog::PreferencesDialog(QWidget *parent)
  : QDialog(parent)
{
  QLabel *rule_url_label = new QLabel("Rule URL", this);
  rule_url_input_ = new QLineEdit(this);

  QLabel *username_label = new QLabel("Username", this);
  rule_username_input_ = new QLineEdit(this);

  QLabel *password_label = new QLabel("Password", this);
  rule_password_input_ = new QLineEdit(this);
  rule_password_input_->setEchoMode(QLineEdit::PasswordEchoOnEdit);

  QLabel *online_url_label = new QLabel("Online URL", this);
  online_url_input_ = new QLineEdit(this);

  QLabel *tracer_url_label = new QLabel("Tracer URL", this);
  tracer_url_input_ = new QLineEdit(this);

  QLabel *player_label = new QLabel("External Player", this);
  external_player_input_ = new QLineEdit(this);

  QPushButton *ok_btn = new QPushButton("OK", this);
  QPushButton *cancel_btn = new QPushButton("Cancel", this);

  int row = 0;
  layout_ = new QGridLayout(this);
  layout_->addWidget(rule_url_label, row, 0, 1, 1);
  layout_->addWidget(rule_url_input_, row, 1, 1, 3);
  ++row;
  layout_->addWidget(username_label, row, 0, 1, 1);
  layout_->addWidget(rule_username_input_, row, 1, 1, 1);
  ++row;
  layout_->addWidget(password_label, row, 0, 1, 1);
  layout_->addWidget(rule_password_input_, row, 1, 1, 1);
  ++row;
  layout_->addWidget(online_url_label, row, 0, 1, 1);
  layout_->addWidget(online_url_input_, row, 1, 1, 3);
  ++row;
  layout_->addWidget(tracer_url_label, row, 0, 1, 1);
  layout_->addWidget(tracer_url_input_, row, 1, 1, 3);
  ++row;
  layout_->addWidget(player_label, row, 0, 1, 1);
  layout_->addWidget(external_player_input_, row, 1, 1, 3);
  ++row;
  layout_->addWidget(cancel_btn, row, 2, 1, 1);
  layout_->addWidget(ok_btn, row, 3, 1, 1);

  settings_ = new QSettings("agora.io", "gump", this);
  settings_->beginGroup("preferences");
  rule_url_input_->setText(settings_->value("rule_url").toString());
  rule_username_input_->setText(settings_->value("rule_username").toString());
  rule_password_input_->setText(settings_->value("rule_password").toString());
  online_url_input_->setText(settings_->value("online_url").toString());
  tracer_url_input_->setText(settings_->value("tracer_url").toString());
  external_player_input_->setText(settings_->value("external_player").toString());
  settings_->endGroup();

  connect(ok_btn, SIGNAL(released()), this, SLOT(OnOk()));
  connect(cancel_btn, SIGNAL(released()), this, SLOT(OnCancel()));
}


PreferencesDialog::~PreferencesDialog()
{

}

void PreferencesDialog::OnOk()
{
  settings_->beginGroup("preferences");
  settings_->setValue("rule_url", rule_url_input_->text());
  settings_->setValue("rule_username", rule_username_input_->text());
  settings_->setValue("rule_password", rule_password_input_->text());
  settings_->setValue("online_url", online_url_input_->text());
  settings_->setValue("tracer_url", tracer_url_input_->text());
  settings_->setValue("external_player", external_player_input_->text());
  settings_->endGroup();

  emit UpdatePreferences();
  close();
}

void PreferencesDialog::OnCancel()
{
  close();
}

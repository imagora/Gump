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
  settings_ = new QSettings("agora.io", "gump", this);
  layout_ = new QGridLayout(this);

  QLabel *get_url_label = new QLabel("Request URL", this);
  url_input_ = new QLineEdit(this);
  QLabel *player_label = new QLabel("Player", this);
  player_input_ = new QLineEdit(this);
  QLabel *rule_label = new QLabel("Stream Rule", this);
  stream_rule_ = new QTextEdit(this);
  QPushButton *ok_btn = new QPushButton("OK", this);
  QPushButton *cancel_btn = new QPushButton("Cancel", this);

  layout_->addWidget(get_url_label, 0, 0, 1, 1);
  layout_->addWidget(url_input_, 0, 1, 1, 3);
  layout_->addWidget(player_label, 1, 0, 1, 1);
  layout_->addWidget(player_input_, 1, 1, 1, 3);
  layout_->addWidget(rule_label, 2, 0, 1, 1);
  layout_->addWidget(stream_rule_, 2, 1, 3, 3);
  layout_->addWidget(cancel_btn, 5, 2, 1, 1);
  layout_->addWidget(ok_btn, 5, 3, 1, 1);

  settings_->beginGroup("preferences");
  url_input_->setText(settings_->value("url").toString());
  player_input_->setText(settings_->value("player").toString());
  stream_rule_->setText(settings_->value("rule").toString());
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
  settings_->setValue("url", url_input_->text());
  settings_->setValue("player", player_input_->text());
  settings_->setValue("rule", stream_rule_->toPlainText());
  settings_->endGroup();

  emit UpdatePreferences();
  close();
}

void PreferencesDialog::OnCancel()
{
  close();
}

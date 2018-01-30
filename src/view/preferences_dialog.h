#pragma once
#include <QDialog>


class QLineEdit;
class QSettings;
class QGridLayout;


namespace gump {


class PreferencesDialog : public QDialog
{
  Q_OBJECT

 public:
  PreferencesDialog(QWidget *parent = nullptr);

  ~PreferencesDialog();

 signals:
  void UpdatePreferences();

 private slots:
  void OnOk();
  void OnCancel();

 private:
  QLineEdit *online_url_input_;
  QLineEdit *external_player_input_;
  QLineEdit *rule_url_input_;
  QLineEdit *rule_username_input_;
  QLineEdit *rule_password_input_;
  QLineEdit *tracer_url_input_;
  QGridLayout *layout_;
  QSettings *settings_;
};


}

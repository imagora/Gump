// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QDialog>
#include <QLineEdit>
#include <QSettings>
#include <QTextEdit>
#include <QGridLayout>


namespace gump {


class PreferencesDialog : public QDialog {
  Q_OBJECT

 public:
  explicit PreferencesDialog(QWidget *parent = nullptr);

  virtual ~PreferencesDialog();

 signals:
  void UpdatePreferences();

 private slots:
  void OnOk();
  void OnCancel();

 private:
  QLineEdit *external_player_input_;
  QLineEdit *vendor_id_input_;
  QLineEdit *max_load_input_;
  QLineEdit *config_url_input_;
  QLineEdit *config_username_input_;
  QLineEdit *config_password_input_;
  QGridLayout *layout_;
  QSettings *settings_;
};


}  // namespace gump

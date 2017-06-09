#pragma once
#include <QDialog>


class QLineEdit;
class QTextEdit;
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
  QLineEdit *url_input_;
  QLineEdit *player_input_;
  QTextEdit *stream_rule_;
  QGridLayout *layout_;
  QSettings *settings_;
};


}

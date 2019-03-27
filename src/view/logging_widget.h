// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QTimer>
#include <QLabel>
#include <QWidget>


namespace gump {


class LoggingWidget
    : public QWidget {
  Q_OBJECT
 public:
  explicit LoggingWidget(QWidget *parent = nullptr);

  void SetUsername(const QString &username);

 private slots:
  void OnTimer();

 protected:
  virtual void hideEvent(QHideEvent *event) override;

  virtual void showEvent(QShowEvent *event) override;

 private:
  uint32_t current_index_;
  QLabel *icon_label_;
  QLabel *username_label_;
  QTimer *update_timer_;
};


}  // namespace gump

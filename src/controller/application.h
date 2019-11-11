// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QApplication>


namespace gump {


class Application : public QApplication {
 public:
  explicit Application(int &argc, char **argv);

  void SetEventReceiver(QObject *obj);

 protected:
  virtual bool event(QEvent *event) override;

 private:
  QObject *event_receiver_;
};


}  // namespace gump

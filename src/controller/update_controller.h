// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QObject>
#include <QTimer>

namespace gump {

class UpdateController : public QObject {
  Q_OBJECT

 public:
  void StartCheckForUpdatesTimer();

 public slots:
  void OnTimer();

 private:
  void Init();

  void CheckForUpdates();

 private:
  bool has_init_ = false;
  QTimer check_timer_;
};

}  // namespace gump

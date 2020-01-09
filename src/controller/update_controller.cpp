// Copyright (c) 2014-2019 winking324
//

#include "controller/update_controller.h"

#include <QSimpleUpdater.h>

#include <QDebug>

#include "commons/definations.h"

namespace gump {

static const int kCheckUpdateInterval = 5 * 60 * 1000;
static const char kCheckUpdateUrl[] =
    "http://gump-update.imagora.net:36770/updates.json";

void UpdateController::StartCheckForUpdatesTimer() {
  if (has_init_) {
    return;
  }

  has_init_ = true;
  Init();
  CheckForUpdates();
}

void UpdateController::OnTimer() { CheckForUpdates(); }

void UpdateController::Init() {
  qInfo() << "init update controller, current version:" << BUILD_VERSION
          << ", built:" << BUILD_DATE_TIME;

  auto *updater = QSimpleUpdater::getInstance();
  updater->setModuleVersion(kCheckUpdateUrl, BUILD_VERSION);
  // updater->setNotifyOnFinish(kCheckUpdateUrl, true);
  updater->setNotifyOnUpdate(kCheckUpdateUrl, true);
  updater->setDownloaderEnabled(kCheckUpdateUrl, true);
  updater->setMandatoryUpdate(kCheckUpdateUrl, true);

  connect(&check_timer_, SIGNAL(timeout()), this, SLOT(OnTimer()));
  check_timer_.setInterval(kCheckUpdateInterval);
  check_timer_.start();
}

void UpdateController::CheckForUpdates() {
  qInfo() << "check for updates";
  auto *updater = QSimpleUpdater::getInstance();
  updater->checkForUpdates(kCheckUpdateUrl);
}

}  // namespace gump

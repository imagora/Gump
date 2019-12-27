// Copyright (c) 2014-2019 winking324
//

#include <QSimpleUpdater.h>
#include <log4cplus/log4cplus.h>

#include <QApplication>
#include <QtAVWidgets/QtAVWidgets>
#include <QtGlobal>

#include "commons/definations.h"
#include "view/application.h"
#include "view/gump_window.h"

void QMessageOutput(QtMsgType type, const QMessageLogContext &context,
                    const QString &msg) {
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
    case QtDebugMsg:
      LOG4CPLUS_DEBUG_FMT(gump::kLoggerName, "(%s:%u, %s) %s", context.file,
                          context.line, context.function, localMsg.constData());
      break;
    case QtInfoMsg:
      LOG4CPLUS_INFO_FMT(gump::kLoggerName, "(%s:%u, %s) %s", context.file,
                         context.line, context.function, localMsg.constData());
      break;
    case QtWarningMsg:
      LOG4CPLUS_WARN_FMT(gump::kLoggerName, "(%s:%u, %s) %s", context.file,
                         context.line, context.function, localMsg.constData());
      break;
    case QtCriticalMsg:
      LOG4CPLUS_ERROR_FMT(gump::kLoggerName, "(%s:%u, %s) %s", context.file,
                          context.line, context.function, localMsg.constData());
      break;
    case QtFatalMsg:
      LOG4CPLUS_FATAL_FMT(gump::kLoggerName, "(%s:%u, %s) %s", context.file,
                          context.line, context.function, localMsg.constData());
      abort();
  }
}

int main(int argc, char *argv[]) {
  QtAV::Widgets::registerRenderers();
  gump::Application a(argc, argv);

  QApplication::setApplicationName("Gump");
  QApplication::setApplicationVersion(BUILD_VERSION);
  QApplication::setWindowIcon(QIcon(":/icon.png"));
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  auto *updater = QSimpleUpdater::getInstance();
  updater->setModuleVersion(gump::kCheckUpdateUrl, BUILD_VERSION);
  updater->setNotifyOnFinish(gump::kCheckUpdateUrl, true);
  updater->setNotifyOnUpdate(gump::kCheckUpdateUrl, true);
  updater->setDownloaderEnabled(gump::kCheckUpdateUrl, true);
  updater->setMandatoryUpdate(gump::kCheckUpdateUrl, true);
  // updater->checkForUpdates(gump::kCheckUpdateUrl);

  // Start
  gump::GumpWindow w;
  a.SetEventReceiver(&w);
  w.show();

  // qInstallMessageHandler(QMessageOutput);
  return a.exec();
}

// Copyright (c) 2014-2019 winking324
//

#include <log4cplus/log4cplus.h>
#include <QtAVWidgets/QtAVWidgets>

#include <QtGlobal>
#include <QApplication>


#include "view/main_window.h"
#include "commons/definations.h"


void QMessageOutput(QtMsgType type, const QMessageLogContext &context,
                    const QString &msg) {
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
    case QtDebugMsg:
//      LOG4CPLUS_DEBUG_FMT(gump::LOGGER_NAME, "(%s:%u, %s) %s", context.file,
//                          context.line, context.function,
//                          localMsg.constData());
    break;
    case QtInfoMsg:
      LOG4CPLUS_INFO_FMT(gump::kLoggerName,  "(%s:%u, %s) %s", context.file,
                         context.line, context.function,
                         localMsg.constData());
    break;
    case QtWarningMsg:
      LOG4CPLUS_WARN_FMT(gump::kLoggerName,  "(%s:%u, %s) %s", context.file,
                         context.line, context.function,
                         localMsg.constData());
    break;
    case QtCriticalMsg:
      LOG4CPLUS_ERROR_FMT(gump::kLoggerName,  "(%s:%u, %s) %s", context.file,
                          context.line, context.function,
                          localMsg.constData());
    break;
    case QtFatalMsg:
      LOG4CPLUS_FATAL_FMT(gump::kLoggerName,  "(%s:%u, %s) %s", context.file,
                          context.line, context.function,
                          localMsg.constData());
      abort();
  }
}


int main(int argc, char *argv[]) {
  QtAV::Widgets::registerRenderers();
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/icon.png"));
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  gump::MainWindow w;
  w.show();

  qInstallMessageHandler(QMessageOutput);
  return a.exec();
}

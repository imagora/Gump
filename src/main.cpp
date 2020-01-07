// Copyright (c) 2014-2019 winking324
//

#include <log4cplus/log4cplus.h>

#include <QApplication>
#include <QFileInfo>
#include <QtAVWidgets/QtAVWidgets>
#include <QtGlobal>

#include "commons/definations.h"
#include "view/application.h"
#include "view/gump_window.h"

static const char kLoggerName[] = "GUMP";

void QMessageOutput(QtMsgType type, const QMessageLogContext &context,
                    const QString &msg) {
  QByteArray localMsg = msg.toLocal8Bit();
  QFileInfo context_file(context.file);
  switch (type) {
    case QtDebugMsg:
      LOG4CPLUS_DEBUG_FMT(kLoggerName, "(%s:%u, %s) %s", context.file,
                          context.line, context.function, localMsg.constData());
      break;
    case QtInfoMsg:
      LOG4CPLUS_INFO_FMT(kLoggerName, "[%s:%u] %s",
                         context_file.fileName().toLocal8Bit().constData(),
                         context.line, localMsg.constData());
      break;
    case QtWarningMsg:
      LOG4CPLUS_WARN_FMT(kLoggerName, "[%s:%u] %s",
                         context_file.fileName().toLocal8Bit().constData(),
                         context.line, localMsg.constData());
      break;
    case QtCriticalMsg:
      LOG4CPLUS_ERROR_FMT(kLoggerName, "[%s:%u] %s",
                          context_file.fileName().toLocal8Bit().constData(),
                          context.line, localMsg.constData());
      break;
    case QtFatalMsg:
      LOG4CPLUS_FATAL_FMT(kLoggerName, "[%s:%u] %s",
                          context_file.fileName().toLocal8Bit().constData(),
                          context.line, localMsg.constData());
      abort();
  }
}

void InitLog() {
  QFile log_properties_file(":/log.properties");
  if (!log_properties_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }

  log4cplus::helpers::Properties log_properties;
  QTextStream text_stream(&log_properties_file);
  while (!text_stream.atEnd()) {
    auto line = text_stream.readLine();
    auto properties = line.split("=");
    if (properties.size() != 2) {
      continue;
    }

    auto key = properties[0].trimmed();
    auto value = properties[1].trimmed();
    log_properties.setProperty(key.toStdString(), value.toStdString());
  }

  log4cplus::PropertyConfigurator config(log_properties);
  config.configure();
}

int main(int argc, char *argv[]) {
  QtAV::Widgets::registerRenderers();
  gump::Application a(argc, argv);

  QApplication::setApplicationName("Gump");
  QApplication::setApplicationVersion(BUILD_VERSION);
  QApplication::setWindowIcon(QIcon(":/icon.png"));
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication::setQuitOnLastWindowClosed(false);

  InitLog();
  qInstallMessageHandler(QMessageOutput);

  gump::GumpWindow w;
  a.SetEventReceiver(&w);
  w.show();

  return a.exec();
}

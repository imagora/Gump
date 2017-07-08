#include "log_dock_appender.h"
#include "view/log_dock.h"
#include "commons/definations.h"
#include <log4cplus/layout.h>
#include <log4cplus/log4cplus.h>


using namespace gump;


LogDockAppender::LogDockAppender(LogDock *log_widget)
  : log_widget_(log_widget)
{
}

LogDockAppender::~LogDockAppender()
{

}

void LogDockAppender::close()
{
  LOG4CPLUS_DEBUG_STR(LOGGER_NAME, "Entering LogDockAppender::close()..");
  closed = true;
}

void LogDockAppender::append(const log4cplus::spi::InternalLoggingEvent &event)
{
  if (log_widget_ == nullptr) return;

  if (!layout) {
    layout.reset(new log4cplus::PatternLayout("%d [%t] %-5p %c - %m%n"));
    //setLayout(layout);
  }

  // TODO: Expose log4cplus' internal TLS to use here.
  log4cplus::tostringstream oss;
  layout->formatAndAppend(oss, event);
  log4cplus::LogLevel ll = event.getLogLevel();
  log_widget_->InsertLog(LOG4CPLUS_TSTRING_TO_STRING(oss.str()).c_str(), ll/10000);
}

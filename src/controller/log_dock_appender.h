#pragma once
#include <log4cplus/appender.h>


namespace gump {

class LogDock;

class LogDockAppender : public log4cplus::Appender
{
 public:
  LogDockAppender(LogDock *log_widget);

  virtual ~LogDockAppender();

  virtual void close();

 protected:
  virtual void append(const log4cplus::spi::InternalLoggingEvent& event);

 private:
  LogDock *log_widget_;
};

}


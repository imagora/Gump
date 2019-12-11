// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <log4cplus/appender.h>

#include "view/log_dock.h"

namespace gump {

class LogDockAppender : public log4cplus::Appender {
 public:
  explicit LogDockAppender(LogDock *log_widget);

  virtual ~LogDockAppender();

  virtual void close() override;

 protected:
  virtual void append(
      const log4cplus::spi::InternalLoggingEvent &event) override;

 private:
  LogDock *log_widget_;
};

}  // namespace gump

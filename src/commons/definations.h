// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <map>
#include <string>
#include <vector>

#define BUILD_DATE_TIME __DATE__ " " __TIME__

#if !defined(BUILD_VERSION)
#define BUILD_VERSION "Unknown"
#endif

namespace gump {

enum LogLevel {
  kTrace = 0,
  kDebug = 1,
  kInfo = 2,
  kWarn = 3,
  kError = 4,
  kFatal = 5,
  kLogButt = 6,
};

static const char kLoggerName[] = "GUMP";
static const char kCheckUpdateUrl[] =
    "http://gump-update.imagora.net:36770/updates.json";

}  // namespace gump

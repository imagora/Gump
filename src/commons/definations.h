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

struct StreamInfo {
  uint32_t ip;
  uint32_t mode;
  uint32_t status;
  uint32_t user_id;
  uint32_t create_ts;
  std::string stream_url;
};

typedef std::pair<std::string, std::string> ChannelKey;
typedef std::vector<StreamInfo> Streams;
typedef std::map<ChannelKey, Streams> ChannelStreams;

}  // namespace gump

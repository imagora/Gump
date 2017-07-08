#pragma once
#include <map>
#include <vector>
#include <string>


namespace gump {


enum LogLevel {
  TRACE_LOG_LEVEL   = 0,
  DEBUG_LOG_LEVEL   = 1,
  INFO_LOG_LEVEL    = 2,
  WARN_LOG_LEVEL    = 3,
  ERROR_LOG_LEVEL   = 4,
  FATAL_LOG_LEVEL   = 5,
  LOG_LEVEL_BUTT    = 6,
};

const static std::string LOGGER_NAME("GUMP");

typedef std::pair<std::string, std::string> ChannelKey;
typedef std::vector<std::string> Streams;
typedef std::map<ChannelKey, Streams> ChannelStreams;

}

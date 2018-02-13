#pragma once
#include <map>
#include <vector>
#include <string>

#define BUILD_DATE_TIME __DATE__ " " __TIME__

#if !defined(BUILD_VERSION)
#define BUILD_VERSION "Unknown"
#endif


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

struct StreamInfo
{
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

}

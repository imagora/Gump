#pragma once
#include <map>
#include <vector>
#include <string>


namespace gump {

typedef std::pair<std::string, std::string> ChannelKey;
typedef std::vector<std::string> Streams;
typedef std::map<ChannelKey, Streams> ChannelStreams;

}

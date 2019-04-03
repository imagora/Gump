// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <map>
#include <vector>

#include <QUrl>


namespace gump {


struct Stream {
  uint32_t ip;
  uint32_t mode;
  uint32_t status;
  uint32_t user_id;
  uint32_t create_ts;
  QUrl stream_url;
};


using StreamDetails = std::vector<Stream>;
using MultiStreams = std::map<QString, StreamDetails>;


}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <map>
#include <vector>
#include <QUrl>
#include <QString>


namespace gump {


struct Stream {
  uint32_t ip;
  uint32_t user_id;
  uint32_t create_ts;

  QString name;
  QUrl url;
};


using Streams = std::map<QString, Stream>;


}  // namespace gump

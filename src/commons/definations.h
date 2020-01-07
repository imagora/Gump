// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QString>

#define BUILD_DATE_TIME __DATE__ " " __TIME__

#if !defined(BUILD_VERSION)
#define BUILD_VERSION "Unknown"
#endif

namespace gump {

static const QString kGumpServerBaseUrl = "https://gump.agoralab.co/gump/";

}  // namespace gump

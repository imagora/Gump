// Copyright (c) 2014-2019 winking324
//

#include "controller/search_event.h"

namespace gump {

SearchEvent::SearchEvent(const QUrl &info) : QEvent(kSearchEventType) {
  info_ = info.adjusted(QUrl::RemoveScheme).toString().remove(0, 2);
  info_.push_front("https://");
}

QString SearchEvent::GetSearchInfo() { return info_; }

}  // namespace gump

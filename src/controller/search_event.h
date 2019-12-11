// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QEvent>
#include <QString>
#include <QUrl>

namespace gump {

static const QEvent::Type kSearchEventType = static_cast<QEvent::Type>(1001);

class SearchEvent : public QEvent {
 public:
  SearchEvent(const QUrl &info);

  QString GetSearchInfo();

 private:
  QString info_;
};

}  // namespace gump

// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <map>
#include <QString>
#include <QJsonObject>


namespace gump {

class StreamRuleController {
 public:
  void UpdateRules(const QJsonObject &rules);

  QString ConvertToPlayUrl(const QString &url);

 private:
  std::map<QString, std::pair<QString, QString>> rules_;
};


}  // namespace gump


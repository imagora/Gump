// Copyright (c) 2014-2019 winking324
//

#include "stream_rule_controller.h"
#include <QDebug>
#include <QRegExp>

namespace gump {


void StreamRuleController::UpdateRules(const QJsonObject &rules) {
  rules_.clear();

  for (auto rule_it = rules.begin(); rule_it != rules.end(); ++rule_it) {
    QString rule_key = rule_it.key();

    if (!rule_it.value().isObject()) {
      qWarning() << "cannot get rule for key: " << rule_key;
      continue;
    }

    QJsonObject rule_obj = rule_it.value().toObject();
    QJsonValue re_val = rule_obj["re"];
    QJsonValue url_val = rule_obj["url"];
    if (re_val == QJsonValue::Undefined || url_val == QJsonValue::Undefined) {
      qWarning() << "cannot get regex or url for key: " << rule_key;
      continue;
    }

    rules_[rule_key] = std::make_pair(re_val.toString(), url_val.toString());
  }

  qInfo() << "update stream rules count: " << rules_.size();
}

QString StreamRuleController::ConvertToPlayUrl(const QString &url) {
  std::pair<QString, QString> regex_rule{"", ""};
  for (const auto &rule : rules_) {
    if (url.indexOf(rule.first) == -1) continue;

    regex_rule = rule.second;
    break;
  }
  if (regex_rule.first.isEmpty()) return "";

  QRegExp qre(regex_rule.first);
  qre.setMinimal(false);
  int pos = qre.indexIn(url);
  if (pos > -1) {
    int cap_count = qre.captureCount();
    switch (cap_count) {
      case 1:
      return regex_rule.second.arg(qre.cap(1));
      case 2:
      return regex_rule.second.arg(qre.cap(1), qre.cap(2));
      case 3:
      return regex_rule.second.arg(qre.cap(1), qre.cap(2), qre.cap(3));
      default:
      break;
    }
  }
  return "";
}


}  // namespace gump

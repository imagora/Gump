// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

namespace gump {

class TracerController : public QObject {
  Q_OBJECT
 public:
  explicit TracerController(QObject *parent = nullptr);

  void OpenTracer(const QString &id, const QString &name);

 signals:

 public slots:
  void OnUpdateUrl(QString url);

 private:
  QString tracer_url_;
};


}  // namespace gump

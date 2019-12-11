// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)

#include <QDockWidget>
#include <QString>
#include <QTextCursor>
#include <QTextEdit>

namespace gump {

class LogDock : public QDockWidget {
 public:
  explicit LogDock(QWidget *parent = nullptr);

  virtual ~LogDock();

  void InsertLog(const QString &log_info, int log_level);

  void TraceLogFormat(QTextCursor *text_cursor);

  void DebugLogFormat(QTextCursor *text_cursor);

  void InfoLogFormat(QTextCursor *text_cursor);

  void WarnLogFormat(QTextCursor *text_cursor);

  void ErrorLogFormat(QTextCursor *text_cursor);

  void FatalLogFormat(QTextCursor *text_cursor);

 private:
  void LogColorFormat(QTextCursor *text_cursor, Qt::GlobalColor color);

 private:
  QTextEdit *log_text_edit_;
};

}  // namespace gump

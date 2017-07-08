#pragma once
#include <QtWidgets/QDockWidget>


class QTextEdit;
class QTextCursor;


namespace gump {


class LogDock : public QDockWidget {
 public:
  LogDock(QWidget *parent = nullptr);

  ~LogDock();

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


}


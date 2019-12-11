// Copyright (c) 2014-2019 winking324
//

#include "view/log_dock.h"

#include "commons/definations.h"

namespace gump {

void (LogDock::*kLogFormatHandler[kLogButt])(QTextCursor *) = {
    &LogDock::TraceLogFormat, &LogDock::DebugLogFormat,
    &LogDock::InfoLogFormat,  &LogDock::WarnLogFormat,
    &LogDock::ErrorLogFormat, &LogDock::FatalLogFormat,
};

LogDock::LogDock(QWidget *parent) : QDockWidget(parent) {
  log_text_edit_ = new QTextEdit(this);
  log_text_edit_->document()->setMaximumBlockCount(100);
  log_text_edit_->setWordWrapMode(QTextOption::NoWrap);
  setWidget(log_text_edit_);
}

LogDock::~LogDock() {}

void LogDock::InsertLog(const QString &log_info, int log_level) {
  log_text_edit_->moveCursor(QTextCursor::End);
  QTextCursor text_cursor(log_text_edit_->textCursor());
  (this->*kLogFormatHandler[log_level])(&text_cursor);
  text_cursor.insertText(log_info);
}

void LogDock::TraceLogFormat(QTextCursor *text_cursor) {
  LogColorFormat(text_cursor, Qt::gray);
}

void LogDock::DebugLogFormat(QTextCursor *text_cursor) {
  LogColorFormat(text_cursor, Qt::gray);
}

void LogDock::InfoLogFormat(QTextCursor *text_cursor) {
  LogColorFormat(text_cursor, Qt::black);
}

void LogDock::WarnLogFormat(QTextCursor *text_cursor) {
  LogColorFormat(text_cursor, Qt::darkYellow);
}

void LogDock::ErrorLogFormat(QTextCursor *text_cursor) {
  LogColorFormat(text_cursor, Qt::red);
}

void LogDock::FatalLogFormat(QTextCursor *text_cursor) {
  LogColorFormat(text_cursor, Qt::red);
}

void LogDock::LogColorFormat(QTextCursor *text_cursor, Qt::GlobalColor color) {
  QTextCharFormat format;
  format.setForeground(QBrush(color));
  text_cursor->setCharFormat(format);
}

}  // namespace gump

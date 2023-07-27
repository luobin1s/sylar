#pragma once
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
namespace sylar {
class Logger;
// 日志事件
class LogEvent {
 public:
  typedef std::shared_ptr<LogEvent> ptr;
  LogEvent(const char *file, int32_t m_line, uint32_t elapse,
           uint32_t thread_id, uint32_t fiber_id, uint64_t time);
  ~LogEvent() {};
  const char *getFile() const { return m_file; }
  int32_t getLine() const { return m_line; }
  uint32_t getElapse() const { return m_elapse; }
  uint32_t getThreadId() const { return m_threadId; }
  uint32_t getFiber() const { return m_fiberId; }
  uint64_t getTime() const { return m_time; }
  std::string getContent() const { return m_ss.str(); }
  std::stringstream &getSS() { return m_ss; }

 private:
  const char *m_file = nullptr;  // 文件名
  int32_t m_line = 0;            // 行号
  uint32_t m_elapse = 0;         // 程序运行时间
  uint32_t m_threadId = 0;       // 线程id
  uint32_t m_fiberId = 0;        // 协程id
  uint64_t m_time = 0;           // 时间戳
 // std::string m_content;         // 内容
  std::stringstream m_ss;
};

// 日志等级
class LogLevel {
 public:
  enum Level {
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5,
  };
  static const char *ToString(LogLevel ::Level level);
};

// 日志格式器
class LogFormatter {
 public:
  typedef std::shared_ptr<LogFormatter> ptr;
  //%t  %thread_id%m%n
  std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level,
                     LogEvent::ptr event);
  LogFormatter(const std::string &pattern);
  class FormatItem {
   public:
    typedef std::shared_ptr<FormatItem> ptr;
    FormatItem(const std::string &fmt = "") {}
    virtual ~FormatItem() {}
    virtual void format(std::ostream &os, std::shared_ptr<Logger> logger,
                        LogLevel::Level level, LogEvent::ptr event) = 0;
  };
  void init();

 private:
  std::string m_pattern;
  std::vector<FormatItem::ptr> m_items;
};

// 日志输出地
class LogAppender {
 public:
  typedef std::shared_ptr<LogAppender> ptr;
  virtual void log(std::shared_ptr<Logger> logger, LogLevel ::Level level,
                   LogEvent::ptr event) = 0;
  void setFormatter(LogFormatter::ptr var) { m_formatter = var; }
  LogFormatter::ptr getFormatter() { return m_formatter; }
  virtual ~LogAppender(){};

 protected:
  LogLevel ::Level m_level = LogLevel::DEBUG;
  LogFormatter::ptr m_formatter;
};

// 日志器
class Logger : public std::enable_shared_from_this<Logger> {
 public:
  typedef std::shared_ptr<Logger> ptr;
  Logger(const std::string &name = "root");
  void log(LogLevel ::Level level, LogEvent::ptr event);
  void debug(LogEvent::ptr event);
  void info(LogEvent::ptr event);
  void warn(LogEvent::ptr event);
  void error(LogEvent::ptr event);
  void fatal(LogEvent::ptr event);

  void addAppender(LogAppender::ptr appender);
  void delAppender(LogAppender::ptr appender);
  LogLevel::Level getLevel() const { return m_level; }
  void setLevel(LogLevel::Level val) { m_level = val; }
  const std::string getName() const { return m_name; }

 private:
  std::list<LogAppender::ptr> m_appenders;
  std::string m_name;
  LogLevel::Level m_level;
  LogFormatter::ptr m_formatter;
};

// 输出到控制台的appender
class StdoutLogAppender : public LogAppender {
 public:
  typedef std::shared_ptr<StdoutLogAppender> ptr;
  virtual void log(std::shared_ptr<Logger> logger, LogLevel ::Level level,
                   LogEvent::ptr event) override;
};

// 输出到文件的appender
class FileLogAppender : public LogAppender {
 public:
  typedef std::shared_ptr<StdoutLogAppender> ptr;
  FileLogAppender(const std::string &filename);
  virtual void log(std::shared_ptr<Logger> logger, LogLevel ::Level level,
                   LogEvent::ptr event) override;
  bool reopen();

 private:
  std::string m_filename;
  std::ofstream m_filestream;
};

}  // namespace sylar

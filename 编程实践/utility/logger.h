#pragma once

#include <fstream>
#include <stdarg.h>
#include <string.h>
#include <time.h>

using namespace std;

namespace lynx {
namespace utility {
#define debug(format, ...)                                                     \
  logger::instance()->log(logger::DEBUG, __FILE__, __LINE__, format,           \
                          ##__VA_ARGS__)
#define info(format, ...)                                                      \
  logger::instance()->log(logger::INFO, __FILE__, __LINE__, format,            \
                          ##__VA_ARGS__)
#define warn(format, ...)                                                      \
  logger::instance()->log(logger::WARN, __FILE__, __LINE__, format,            \
                          ##__VA_ARGS__)
#define error(format, ...)                                                     \
  logger::instance()->log(logger::ERROR, __FILE__, __LINE__, format,           \
                          ##__VA_ARGS__)
#define fatal(format, ...)                                                     \
  logger::instance()->log(logger::FATAL, __FILE__, __LINE__, format,           \
                          ##__VA_ARGS__)

class logger {
public:
  enum LEVEL { DEBUG = 0, INFO, WARN, ERROR, FATAL, LEVEL_COUNT };
  static logger *instance();
  void open(const string &fname);
  void close();
  void log(LEVEL, const char *file, int line, const char *format, ...);
  void level(LEVEL level);
  void max(int bytes);
  void rotate();

private:
  logger();
  string m_fname;
  ofstream m_fout;
  LEVEL m_level;
  int m_max;
  int m_len;
  static const char *s_level[LEVEL_COUNT];
  static logger *m_instance;
  ~logger();
};

} // namespace utility
} // namespace lynx

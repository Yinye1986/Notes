#include "logger.h"

using namespace lynx::utility;

const char *logger::s_level[LEVEL_COUNT] = {"DEBUG", "INFO", "WARN", "ERROR",
                                            "FATAL"};

logger::logger() : m_level(DEBUG), m_max(0), m_len(0) {}
logger::~logger() { close(); }
logger *logger::m_instance = NULL;

logger *logger::instance() {
  if (m_instance == NULL) {
    m_instance = new logger();
  }
  return m_instance;
}
void logger::level(LEVEL level) { m_level = level; }
void logger::open(const string &fname) {
  m_fname = fname;
  m_fout.open(fname, ios::app);
  if (m_fout.fail()) {
    throw std::logic_error("open file failed" + fname);
  }
  m_fout.seekp(0, ios::end);
  m_len = m_fout.tellp();
}
void logger::max(int bytes) { m_max = bytes; }
void logger::close() { m_fout.close(); }

void logger::log(LEVEL level, const char *file, int line, const char *format,
                 ...) {
  if (m_level > level) {
    return;
  }
  if (m_fout.fail()) {
    throw std::logic_error("open file failed" + m_fname);
  }

  time_t ticks = time(NULL);
  struct tm *ptm = localtime(&ticks);
  char timestamp[32];
  memset(timestamp, 0, sizeof(timestamp));
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);

  const char *fmt = "%s %s %s:%d";
  int size = snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);
  if (size > 0) {
    char *buffer = new char[size + 1];
    snprintf(buffer, size + 1, fmt, timestamp, s_level[level], file, line);
    buffer[size] = 0;
    m_fout << buffer;
    m_len += size;
    delete[] buffer;
  }
  va_list arg_ptr;
  va_start(arg_ptr, format);
  size = vsnprintf(NULL, 0, format, arg_ptr);
  va_end(arg_ptr);
  if (size > 0) {
    char *content = new char[size + 1];
    va_start(arg_ptr, format);
    vsnprintf(content, size + 1, format, arg_ptr);
    va_end(arg_ptr);
    m_fout << content;
    m_len += size;
    delete[] content;
  }
  m_fout << '\n';
  m_fout.flush();
  if (m_len >= m_max && m_max > 0) {
    rotate();
  }
}

void logger::rotate() {
  close();
  time_t ticks = time(NULL);
  struct tm *ptm = localtime(&ticks);
  char timestamp[32];
  memset(timestamp, 0, sizeof(timestamp));
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);
  string filename = m_fname + timestamp;
  if (rename(m_fname.c_str(), filename.c_str()) != 0) {
    throw std::logic_error("rename log file failed:" + string(strerror(errno)));
  }
}

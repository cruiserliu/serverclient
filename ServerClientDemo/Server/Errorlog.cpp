#include "Errorlog.h"

void initLogger(const string &info_log_filename,
                const string &warn_log_filename,
                const string &error_log_filename)
{
   Logger::m_info_log_file.open(info_log_filename.c_str());
   Logger::m_warn_log_file.open(warn_log_filename.c_str());
   Logger::m_error_log_file.open(error_log_filename.c_str());
}

ostream &Logger::getStream(log_rank_t log_rank)
{
   return (INFO == log_rank) ? (m_info_log_file.is_open() ? m_info_log_file : cout) : (WARNING == log_rank ? (m_warn_log_file.is_open() ? m_warn_log_file : cerr) : (m_error_log_file.is_open() ? m_error_log_file : cerr));
}

ostream &Logger::start(log_rank_t log_rank,
                       const int32_t line,
                       const string &function)
{
   time_t tm;
   time(&tm);
   char time_string[128];
   ctime_r(&tm, time_string);
   return getStream(log_rank) << time_string
                              << " function ( " << function << " )"
                              << " line " << line
                              << flush;
}

Logger::~Logger()
{
   getStream(m_log_rank) << endl
                         << flush;

   if (FATAL == m_log_rank)
   {
      m_info_log_file.close();
      m_info_log_file.close();
      m_info_log_file.close();
      abort();
   }
}
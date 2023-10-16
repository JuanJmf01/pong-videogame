// logger.h
#ifndef LOG_LEVELS_H
#define LOG_LEVELS_H

#define LOG_INFO 0
#define LOG_WARNING 1
#define LOG_ERROR 2
#define LOG_DEBUG 3


void log_message(int severity, const char *message);

#endif


#ifndef LOGGING_H
#define LOGGING_H

enum loglevel {
  LOG_ERROR  = 0,
  LOG_WARN   = 1,
  LOG_INFO   = 2,
  LOG_DEBUG  = 3,
};

void logwarn(const char *msg, ...);
void logdebug(const char *msg, ...);

#endif /* LOGGING_H */


#include "logging.h"
#include <stdio.h>

enum loglevel g_loglevel = LOG_DEBUG;

static const char * level_names[] = {
  "error",
  "warn ",
  "info ",
  "debug"
};

void log_(enum loglevel loglevel, const char *msg, va_list args) {
  const char *logname = level_names[loglevel];
  printf("%s: ", logname);
  vprintf(msg, args);
}

void logwarn(const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  log_(LOG_WARN, msg, args);
  va_end(args);
}

void logdebug(const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  log_(LOG_DEBUG, msg, args);
  va_end(args);
}

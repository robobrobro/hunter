#ifndef _HUNTER_DEBUG_H_
#define _HUNTER_DEBUG_H_

#include <stdio.h>

#define debug(fmt, ...)
#define error(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

#endif  /* _HUNTER_DEBUG_H_ */

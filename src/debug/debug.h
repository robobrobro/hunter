#ifndef _HUNTER_DEBUG_H_
#define _HUNTER_DEBUG_H_

#include <stdio.h>

#define debug(fmt, ...) printf("[DEBUG][%s:%s:%d] " fmt, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
#define error(fmt, ...) fprintf(stderr, "[ERROR][%s:%s:%d] " fmt, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)

#endif  /* _HUNTER_DEBUG_H_ */

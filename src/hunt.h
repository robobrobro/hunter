#ifndef _HUNTER_HUNT_H_
#define _HUNTER_HUNT_H_

#include <stddef.h>
#include <stdint.h>

typedef void (*hunt_cb_t)(size_t offset, void *arg);
void hunt(uint8_t *file, size_t file_len, uint8_t *flag, size_t flag_len, hunt_cb_t cb, void *arg);

#endif  /* _HUNTER_HUNT_H_ */

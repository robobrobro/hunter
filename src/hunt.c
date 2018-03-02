#include "hunt.h"

#include <string.h>

void hunt(uint8_t *file, size_t file_len, uint8_t *flag, size_t flag_len, hunt_cb_t cb, void *arg)
{
    for (size_t idx = 0; idx < file_len - flag_len; ++idx) {
        if (memcmp(&file[idx], flag, flag_len) == 0) {
            cb(idx, arg);
        }
    }
}

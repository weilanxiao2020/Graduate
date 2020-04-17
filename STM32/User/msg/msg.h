#ifndef __MSG_H
#define __MSG_H

#include "constants.h"
#include "utils.h"
#include <stdlib.h>

static boolean msg_bulid_header(byte *msg, uint8_t cnt, const byte *header);
static boolean msg_bulid_data(byte *msg, uint8_t cnt, const byte *data, uint8_t size);

#endif

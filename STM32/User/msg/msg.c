#include "msg.h"
#include "string.h"

void msg_create_error() 
{
    byte head[4] = {0xfe,0xf1};
    uint64_t timestamp = 1032546845;
    int size = sizeof(timestamp);
    head[2] = (size>>8) & 0xff;
    head[3] = size & 0xff;
    byte *data = (byte *)malloc(4+size);
    msg_bulid_header(data, 4, head);
    byte time[8];
    u64_to_u8(timestamp,time);
    msg_bulid_data(data+4, size, time, size);
}

static boolean msg_bulid_header(byte *msg, uint8_t cnt, const byte *header)
{
    uint8_t i;
    if(cnt<4) {
        return false;
    }
    for(i=0;i<4;i++) {
        *(msg+i) = *(header+i);
    }
    return true;
}

static boolean msg_bulid_data(byte *msg, uint8_t cnt, const byte *data, uint8_t size)
{
    uint8_t i;
    if(cnt<size) {
        return false;
    }
    for(i=0;i<size;i++) {
        *(msg+i) = *(data+i);
    }
    return true;
}
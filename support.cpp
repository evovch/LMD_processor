#include "support.h"

unsigned int DecodeBuf(unsigned char* p_buf)
{
    return ((p_buf[0] << 24) & 0xff000000) |
           ((p_buf[1] << 16) & 0x00ff0000) |
           ((p_buf[2] <<  8) & 0x0000ff00) |
           ((p_buf[3] <<  0) & 0x000000ff);
}

unsigned int DecodeBuf2(unsigned char* p_buf)
{
    return ((p_buf[3] << 24) & 0xff000000) |
           ((p_buf[2] << 16) & 0x00ff0000) |
           ((p_buf[1] <<  8) & 0x0000ff00) |
           ((p_buf[0] <<  0) & 0x000000ff);
}

//! Expanded timestamp for 250 MHz * 14 bit epochs
uint64_t FullTimeStamp(uint32_t epoch, uint16_t stamp)
{ return ((uint64_t) epoch << 14) | (stamp & 0x3fff); }

//! Expanded timestamp for 250/8*5 MHz * 19 bit epochs
uint64_t FullTimeStamp2(uint32_t epoch, uint32_t stamp)
{ return ((uint64_t) epoch << 19) | (stamp & 0x7ffff); }

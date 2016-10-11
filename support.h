#ifndef SUPPORT_H
#define SUPPORT_H

#include <utility>

#include "Rtypes.h"

typedef std::pair<uint64_t, uint64_t> triggerWindow_t;

unsigned int DecodeBuf(unsigned char* p_buf);

unsigned int DecodeBuf2(unsigned char* p_buf);

//! Expanded timestamp for 250 MHz * 14 bit epochs
uint64_t FullTimeStamp(uint32_t epoch, uint16_t stamp);

//! Expanded timestamp for 250/8*5 MHz * 19 bit epochs
uint64_t FullTimeStamp2(uint32_t epoch, uint32_t stamp);

#endif // SUPPORT_H

#ifndef CLS_EVENT_H
#define CLS_EVENT_H

#include <cstdint>
#include <utility>
#include <map>

class cls_Event
{
public:
    cls_Event();
    ~cls_Event();

    void AddHit(uint64_t p_time, uint8_t p_ch, uint16_t p_adcVal);

    unsigned int GetNhits(void) { return fEventTimeAdcMap.size(); }
    void Clear(void) { fEventTimeAdcMap.clear(); }

    void Dump(void);

public:

    // The main data container - hits of the event.
    // pair: full time - channel:adc
    std::multimap< uint64_t, std::pair<uint8_t, uint16_t> > fEventTimeAdcMap;

};

#endif // CLS_EVENT_H

#ifndef CLS_EVENT_H
#define CLS_EVENT_H

#include <utility>
#include <map>

#include <Rtypes.h>

class cls_Event
{
public:
    cls_Event();
    ~cls_Event();

    void AddHit(ULong64_t p_time, UChar_t p_ch, UShort_t p_adcVal);

    unsigned int GetNhits(void) { return fEventTimeAdcMap.size(); }
    void Clear(void) { fEventTimeAdcMap.clear(); }

    void Dump(void);

public:

    // The main data container - hits of the event.
    // pair: full time - channel:adc
    std::multimap< ULong64_t, std::pair<UChar_t, UShort_t> > fEventTimeAdcMap;

};

#endif // CLS_EVENT_H

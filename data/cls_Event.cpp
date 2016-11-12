#include "cls_Event.h"

/*
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
*/

#include <cstdio>

cls_Event::cls_Event()
{
}

cls_Event::~cls_Event()
{
}

void cls_Event::AddHit(ULong64_t p_time, UChar_t p_ch, UShort_t p_adcVal)
{
    std::pair<UChar_t, UShort_t> v_curChAdcPair;
    v_curChAdcPair = std::make_pair(p_ch, p_adcVal);
    fEventTimeAdcMap.insert(std::pair<uint32_t, std::pair<UChar_t, UShort_t> >(p_time, v_curChAdcPair));
}

void cls_Event::Dump(void)
{
    printf ("EVENT (%d)=====================================================================\n", this->GetNhits());

    std::multimap< ULong64_t, std::pair<UChar_t, UShort_t> >::iterator v_mainIter;
    for (v_mainIter = fEventTimeAdcMap.begin(); v_mainIter != fEventTimeAdcMap.end(); ++v_mainIter)
    {
        ULong64_t v_currentTimestamp = (*v_mainIter).first;
        std::pair<UChar_t, UShort_t> v_currentHit = (*v_mainIter).second;

        printf ("ts: %lld\t\tch: %d\tadc: %d\n", v_currentTimestamp, v_currentHit.first, v_currentHit.second);

    }
}

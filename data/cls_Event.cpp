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

void cls_Event::AddHit(uint64_t p_time, uint8_t p_ch, uint16_t p_adcVal)
{
    std::pair<uint8_t, uint16_t> v_curChAdcPair;
    v_curChAdcPair = std::make_pair(p_ch, p_adcVal);
    fEventTimeAdcMap.insert(std::pair<uint32_t, std::pair<uint8_t, uint16_t> >(p_time, v_curChAdcPair));
}

void cls_Event::Dump(void)
{
    printf ("EVENT (%d)=====================================================================\n", this->GetNhits());

    std::multimap< uint64_t, std::pair<uint8_t, uint16_t> >::iterator v_mainIter;
    for (v_mainIter = fEventTimeAdcMap.begin(); v_mainIter != fEventTimeAdcMap.end(); ++v_mainIter)
    {
        uint64_t v_currentTimestamp = (*v_mainIter).first;
        std::pair<uint8_t, uint16_t> v_currentHit = (*v_mainIter).second;

        printf ("ts: %ld\t\tch: %d\tadc: %d\n", v_currentTimestamp, v_currentHit.first, v_currentHit.second);

    }
}

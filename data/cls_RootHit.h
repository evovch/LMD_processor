#ifndef CLS_ROOTHIT_H
#define CLS_ROOTHIT_H

/*
 * AdcVal is actually pedestal minus raw adc val.
 * This value has physical meaning.
 */

#include <TObject.h>

class cls_RootHit : public TObject
{
public: // methods

    cls_RootHit();
    cls_RootHit(uint64_t p_ts, uint8_t p_ch, uint16_t p_rawAdc, int32_t p_adc);
    // Copy constructor
    cls_RootHit(cls_RootHit* p_sourceHit);

    void Clear(Option_t *option="");

    cls_RootHit &operator=(const cls_RootHit &orig);

private: // data members

    uint64_t fTimestamp;
    //uint8_t fFebID;       // hooks for further development
    //uint8_t fChipID;      // hooks for further development
    uint8_t fChannel;
    uint16_t fRawAdcVal;
    int32_t fAdcVal;      // (pedestal -raw adc val)

public: // switch off this shit for time being
    ClassDef(cls_RootHit,1)
};

#endif // CLS_ROOTHIT_H

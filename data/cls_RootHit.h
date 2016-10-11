#ifndef CLS_ROOTHIT_H
#define CLS_ROOTHIT_H

/*
 * AdcVal is actually pedestal minus raw adc val.
 * This value has physical meaning.
 */

#include <Rtypes.h>
#include <TObject.h>

class cls_RootHit : public TObject
{
public: // methods

    cls_RootHit();
    cls_RootHit(ULong64_t p_ts, UChar_t p_ch, UShort_t p_rawAdc, Int_t p_adc);
    // Copy constructor
    cls_RootHit(cls_RootHit* p_sourceHit);

    void Clear(Option_t *option="");

    cls_RootHit &operator=(const cls_RootHit &orig);

private: // data members

    ULong64_t fTimestamp;
    //UChar_t fFebID;       // hooks for further development
    //UChar_t fChipID;      // hooks for further development
    UChar_t fChannel;
    UShort_t fRawAdcVal;
    Int_t fAdcVal;      // (pedestal -raw adc val)

public: // switch off this shit for time being
    ClassDef(cls_RootHit,1)
};

#endif // CLS_ROOTHIT_H

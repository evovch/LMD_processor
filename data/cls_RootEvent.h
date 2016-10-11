#ifndef CLS_ROOTEVENT_H
#define CLS_ROOTEVENT_H

#include <TObject.h>

#include <TClonesArray.h>

class cls_RootHit;

class cls_RootEvent : public TObject
{
public: // methods

    cls_RootEvent();

    cls_RootHit* AddHit(cls_RootHit* p_sourceHit);
    cls_RootHit* AddHit(uint64_t p_ts, uint8_t p_ch, uint16_t p_rawAdc, int32_t p_adc);
    void Clear(Option_t *option ="");

private: // data members

    TClonesArray* fHits; // TClonesArray of cls_RootHit objects
    uint16_t fNumOfHits; // Number of hits - actually the size of the 'fHits'

public: // switch off this shit for time being
    ClassDef(cls_RootEvent,1)
};

#endif // CLS_ROOTEVENT_H

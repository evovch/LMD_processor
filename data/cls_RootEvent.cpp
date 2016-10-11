#include "cls_RootEvent.h"

// switch off this shit for time being
ClassImp(cls_RootEvent)

#include <TClonesArray.h>

#include "cls_RootHit.h"

cls_RootEvent::cls_RootEvent() :
    fNumOfHits(0)
{
    fHits = new TClonesArray("cls_RootHit", 5);
}

/* https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html#example-4-a-tree-with-an-event-class
 * Section 14.17
 */
cls_RootHit* cls_RootEvent::AddHit(cls_RootHit* p_sourceHit)
{
    TClonesArray &hits = *fHits;
    cls_RootHit *hit = new(hits[fNumOfHits++]) cls_RootHit(p_sourceHit);
    return hit;
}

cls_RootHit* cls_RootEvent::AddHit(uint64_t p_ts, uint8_t p_ch, uint16_t p_rawAdc, int32_t p_adc)
{
    TClonesArray &hits = *fHits;
    cls_RootHit *hit = new(hits[fNumOfHits++]) cls_RootHit(p_ts, p_ch, p_rawAdc, p_adc);
    return hit;
}

void cls_RootEvent::Clear(Option_t * /*option*/)
{
    fHits->Clear("C");
}

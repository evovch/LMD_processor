#include "cls_RootHit.h"

// switch off this shit for time being
ClassImp(cls_RootHit)

// Default constructor should not be used
cls_RootHit::cls_RootHit()
{

}

cls_RootHit::cls_RootHit(uint64_t p_ts, uint8_t p_ch, uint16_t p_rawAdc, int32_t p_adc) :
    fTimestamp(p_ts),
    fChannel(p_ch),
    fRawAdcVal(p_rawAdc),
    fAdcVal(p_adc)
{

}

cls_RootHit::cls_RootHit(cls_RootHit* p_sourceHit) :
    fTimestamp(p_sourceHit->fTimestamp),
    fChannel(p_sourceHit->fChannel),
    fRawAdcVal(p_sourceHit->fRawAdcVal),
    fAdcVal(p_sourceHit->fAdcVal)
{

}

void cls_RootHit::Clear(Option_t * /*option*/)
{
   TObject::Clear();
}

cls_RootHit &cls_RootHit::operator=(const cls_RootHit &orig)
{
   TObject::operator=(orig);

   fTimestamp = orig.fTimestamp;
   fChannel = orig.fChannel;
   fRawAdcVal = orig.fRawAdcVal;
   fAdcVal = orig.fAdcVal;

   return *this;
}

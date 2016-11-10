#ifndef CLS_PIXELMAP_H
#define CLS_PIXELMAP_H

#include <TString.h>

class cls_pixelMap
{
public:
    cls_pixelMap();
    ~cls_pixelMap();

    void Import(TString p_filename);

public:

    // Return -1 if no neighbor exists
    Int_t GetTopNeighbor(UInt_t p_pixelID);
    Int_t GetBottomNeighbor(UInt_t p_pixelID);
    Int_t GetLeftNeighbor(UInt_t p_pixelID);
    Int_t GetRightNeighbor(UInt_t p_pixelID);

private:
    UInt_t fMap[8][8];

};

#endif // CLS_PIXELMAP_H

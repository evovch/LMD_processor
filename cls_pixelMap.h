#ifndef CLS_PIXELMAP_H
#define CLS_PIXELMAP_H

#include <TString.h>

class cls_pixelMap
{
public:
    cls_pixelMap();
    ~cls_pixelMap();

    void Import(TString p_filename);

    Int_t GetPixelFromFebCh(UInt_t p_febCh);
    Int_t GetFebChFromPixel(UInt_t p_pixelId);

    // Return 1 - OK, 0 - FAIL
    Int_t GetPixelCoords(UInt_t p_febCh, UInt_t* o_row, UInt_t* o_col);

    // Return -1 if no neighbor exists
    Int_t GetTopNeighbor(UInt_t p_febCh);
    Int_t GetBottomNeighbor(UInt_t p_febCh);
    Int_t GetLeftNeighbor(UInt_t p_febCh);
    Int_t GetRightNeighbor(UInt_t p_febCh);

private: // data members

    // FEB channels are from 0 (incl.) to 63 (incl.)
    // Pixel IDs are from 1 (incl.) to 64 (incl.)

    UInt_t fPixelFromFebCh[64];
    UInt_t fFebChFromPixel[64+1];   // 0-th element not used

};

#endif // CLS_PIXELMAP_H

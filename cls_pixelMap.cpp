#include "cls_pixelMap.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::cerr;
using std::endl;

cls_pixelMap::cls_pixelMap()
{
    // Default map
    for (UInt_t i=0; i<8; i++) {
        fPixelFromFebCh[i*8+0] = i*8+1;
        fPixelFromFebCh[i*8+1] = i*8+5;
        fPixelFromFebCh[i*8+2] = i*8+3;
        fPixelFromFebCh[i*8+3] = i*8+7;
        fPixelFromFebCh[i*8+4] = i*8+2;
        fPixelFromFebCh[i*8+5] = i*8+6;
        fPixelFromFebCh[i*8+6] = i*8+4;
        fPixelFromFebCh[i*8+7] = i*8+8;

        fFebChFromPixel[i*8+1] = i*8+0;
        fFebChFromPixel[i*8+5] = i*8+1;
        fFebChFromPixel[i*8+3] = i*8+2;
        fFebChFromPixel[i*8+7] = i*8+3;
        fFebChFromPixel[i*8+2] = i*8+4;
        fFebChFromPixel[i*8+6] = i*8+5;
        fFebChFromPixel[i*8+4] = i*8+6;
        fFebChFromPixel[i*8+8] = i*8+7;
    }
}

cls_pixelMap::~cls_pixelMap()
{
}

void cls_pixelMap::Import(TString p_filename)
{
    std::ifstream infile(p_filename.Data());

    UInt_t globch;
    UInt_t roc;
    UInt_t feb;
    UInt_t febch;
    UInt_t mapmt;
    UInt_t pixel;
    Double_t xcoord;
    Double_t ycoord;
    UInt_t xbin;
    UInt_t ybin;
    UInt_t trb;
    UInt_t trbch;

    //FIXME - implement #comment checking
    // By now simply skip first two lines
    std::string str;
    getline(infile, str);
    getline(infile, str);

    while ((infile >> globch >> roc >> feb >> febch >> mapmt >> pixel >> xcoord >> ycoord >> xbin >> ybin >> trb >> trbch)) {
        //printf ("%d\t%d\t%d\t%d\t%d\t%d\t%f\t%f\t%d\t%d\t%d\t%d\n",
        //        globch, roc, feb, febch, mapmt, pixel, xcoord, ycoord, xbin, ybin, trb, trbch);

        if (roc == 0 && feb == 0 && febch < 64 && mapmt==1 ) {
            fPixelFromFebCh[febch] = pixel;
            fFebChFromPixel[pixel] = febch;
        }

        // Implement whatever needed

    }
}

Int_t cls_pixelMap::GetPixelCoords(UInt_t p_febCh, UInt_t* o_row, UInt_t* o_col)
{
    if (p_febCh >= 0 && p_febCh < 64) {
        UInt_t v_pixelID = fPixelFromFebCh[p_febCh];

        *o_row = (v_pixelID-1)/8;
        *o_col = (v_pixelID-1)%8;
        return 1; // OK

    } else {
        return 0; // FAIL
    }

}

Int_t cls_pixelMap::GetPixelFromFebCh(UInt_t p_febCh)
{
    if (p_febCh < 0 || p_febCh > 63) {
        return -1; // FAIL
    } else {
        return fPixelFromFebCh[p_febCh];
    }

}

Int_t cls_pixelMap::GetFebChFromPixel(UInt_t p_pixelId)
{
    if (p_pixelId < 1 || p_pixelId > 64) {
        return -1; // FAIL
    } else {
        return fFebChFromPixel[p_pixelId];
    }
}

// FEB channel as input and as output
Int_t cls_pixelMap::GetTopNeighbor(UInt_t p_febCh)
{
    if (p_febCh < 0 || p_febCh > 63) {
        return -1; // FAIL
    } else {
        UInt_t curPixel = fPixelFromFebCh[p_febCh];
        UInt_t neighPixel = curPixel - 8;

        if (neighPixel < 1 || neighPixel > 64) {
            return -1; // FAIL
        } else {
            return fFebChFromPixel[neighPixel]; // OK
        }
    }
}

// FEB channel as input and as output
Int_t cls_pixelMap::GetBottomNeighbor(UInt_t p_febCh)
{
    if (p_febCh < 0 || p_febCh > 63) {
        return -1; // FAIL
    } else {
        UInt_t curPixel = fPixelFromFebCh[p_febCh];
        UInt_t neighPixel = curPixel + 8;

        if (neighPixel < 1 || neighPixel > 64) {
            return -1; // FAIL
        } else {
            return fFebChFromPixel[neighPixel]; // OK
        }
    }
}

// FEB channel as input and as output
Int_t cls_pixelMap::GetLeftNeighbor(UInt_t p_febCh)
{
    if (p_febCh < 0 || p_febCh > 63) {
        return -1; // FAIL
    } else {
        UInt_t curPixel = fPixelFromFebCh[p_febCh];
        UInt_t neighPixel = curPixel - 1;

        if (neighPixel < 1 || neighPixel > 64) {
            return -1; // FAIL
        } else {
            return fFebChFromPixel[neighPixel]; // OK
        }
    }
}

// FEB channel as input and as output
Int_t cls_pixelMap::GetRightNeighbor(UInt_t p_febCh)
{
    if (p_febCh < 0 || p_febCh > 63) {
        return -1; // FAIL
    } else {
        UInt_t curPixel = fPixelFromFebCh[p_febCh];
        UInt_t neighPixel = curPixel + 1;

        if (neighPixel < 1 || neighPixel > 64) {
            return -1; // FAIL
        } else {
            return fFebChFromPixel[neighPixel]; // OK
        }
    }
}

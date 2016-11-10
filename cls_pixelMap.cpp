#include "cls_pixelMap.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::cerr;
using std::endl;

cls_pixelMap::cls_pixelMap()
{

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

    UInt_t counterRow=0;
    UInt_t counterCol=0;

    while ((infile >> globch >> roc >> feb >> febch >> mapmt >> pixel >> xcoord >> ycoord >> xbin >> ybin >> trb >> trbch)) {
        //printf ("%d\t%d\t%d\t%d\t%d\t%d\t%f\t%f\t%d\t%d\t%d\t%d\n",
        //        globch, roc, feb, febch, mapmt, pixel, xcoord, ycoord, xbin, ybin, trb, trbch);

        fMap[counterRow][counterCol] = pixel;
        counterCol++;
        if (counterCol == 8) {
            counterRow++;
            counterCol = 0;
        }

        if (counterRow == 8) break;

        // Implement whatever needed

    }
}

Int_t cls_pixelMap::GetTopNeighbor(UInt_t p_pixelID)
{
    for (UInt_t counterRow=0; counterRow<8; counterRow++) {
        for (UInt_t counterCol=0; counterCol<8; counterCol++) {
            if (fMap[counterRow][counterCol] == p_pixelID) {

                if (counterRow > 0) return fMap[counterRow-1][counterCol];
                else return -1;

                break;
            }
        }
    }
    return -2;
}

Int_t cls_pixelMap::GetBottomNeighbor(UInt_t p_pixelID)
{
    for (UInt_t counterRow=0; counterRow<8; counterRow++) {
        for (UInt_t counterCol=0; counterCol<8; counterCol++) {
            if (fMap[counterRow][counterCol] == p_pixelID) {

                if (counterRow < 7) return fMap[counterRow+1][counterCol];
                else return -1;

                break;
            }
        }
    }
    return -2;
}

Int_t cls_pixelMap::GetLeftNeighbor(UInt_t p_pixelID)
{
    for (UInt_t counterRow=0; counterRow<8; counterRow++) {
        for (UInt_t counterCol=0; counterCol<8; counterCol++) {
            if (fMap[counterRow][counterCol] == p_pixelID) {

                if (counterCol > 0) return fMap[counterRow][counterCol-1];
                else return -1;

                break;
            }
        }
    }
    return -2;
}

Int_t cls_pixelMap::GetRightNeighbor(UInt_t p_pixelID)
{
    for (UInt_t counterRow=0; counterRow<8; counterRow++) {
        for (UInt_t counterCol=0; counterCol<8; counterCol++) {
            if (fMap[counterRow][counterCol] == p_pixelID) {

                if (counterCol < 7) return fMap[counterRow][counterCol+1];
                else return -1;

                break;
            }
        }
    }
    return -2;
}

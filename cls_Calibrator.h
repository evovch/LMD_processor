#ifndef CLS_CALIBRATOR_H
#define CLS_CALIBRATOR_H

#include <TString.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TFile.h>

class cls_Calibrator
{
public:
    cls_Calibrator();
    ~cls_Calibrator();

    void ImportGraphs(TString p_filename);

    void GenerateLUTs(void);
    unsigned int ExportLUTs(void);

    // Input value - (ped - rawVal)
    Float_t GetCalibratedVal(UInt_t ch, Float_t val);

private: // data members
//    TFile *fInput;
    TGraph* fGraph[64];
    TH1D* fLUT[64];

};

#endif // CLS_CALIBRATOR_H

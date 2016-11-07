#ifndef CLS_CALIBRATOR_H
#define CLS_CALIBRATOR_H

#include <TString.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TFile.h>

class cls_Calibrator
{
private: // Singleton standard
    cls_Calibrator(void);
    ~cls_Calibrator(void);
    cls_Calibrator(cls_Calibrator const&); // no implementation needed
    cls_Calibrator& operator= (cls_Calibrator const&); // no implementation needed

public: // Singleton standard
    static cls_Calibrator& Instance(void);

public: // methods
    unsigned int ImportGraphs(TString p_filename);
    unsigned int ExportLUTs(TString p_filename="LUTs.root");

    // Input value - (ped - rawVal)
    Float_t GetCalibratedVal(UInt_t ch, Float_t val);

private: // methods
    void GenerateDummyLUTs(void);
    void GenerateLUTs(void);
    void Deconstruct(void);

private: // data members
    TGraph* fGraph[64];
    TH1D* fLUT[64];

    Bool_t fConstructed; // Defines whether the histograms have been allocated

};

#endif // CLS_CALIBRATOR_H

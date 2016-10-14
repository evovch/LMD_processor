#include "cls_Calibrator.h"
//#include <TROOT.h>
//#include <TObject.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

cls_Calibrator::cls_Calibrator()
{

}

cls_Calibrator::~cls_Calibrator()
{

}

Float_t cls_Calibrator::GetCalibratedVal(UInt_t ch, Float_t val)
{
    return fLUT[ch]->GetBinContent((UInt_t)val);
}

void cls_Calibrator::ImportGraphs(TString p_filename)
{

    TFile fr(p_filename.Data());

    TList *gList = fr.GetListOfKeys();
    const Int_t nEntries = gList->GetEntries();
    if (nEntries!=64) {
        cerr << "[cls_Calibrator::ImportGraphs] Something wrong, only " << nEntries << " graphs found instead of 64" << endl;
        return;
    }

    TString gName;
    for (Int_t i = 0; i < nEntries; i++) {
        gName.Form("gcal%d", i);
        fGraph[i] = (TGraph*)fr.Get(gName.Data());
        if (fGraph[i]==0) {
            cerr << "[cls_Calibrator::ImportGraphs] Graph " << gName.Data() << " was not found in file " << fr.GetName() << endl;
            fr.Close();
            return;
        }
    }

    fr.Close();

    this->GenerateLUTs();

    this->ExportLUTs();
}

void cls_Calibrator::GenerateLUTs(void)
{

    TString hName;
    TString hTitle;
    const Double_t minX = -200.;     //minimal channel treated
    const Double_t maxX = 4096.;    //maximal channel treated

    //auxilliary variables for graph range
    Double_t xmin = 0;
    Double_t xmax = 0;
    Double_t ymin = 0;
    Double_t ymax = 0;

    for (Int_t i = 0; i < 64; i++) {
        hName.Form("calHist%d", i);
        hTitle.Form("do we need it for channel %d?", i);
        fLUT[i] = new TH1D(hName.Data(), hTitle.Data(), maxX - minX, minX, maxX);
        fGraph[i]->ComputeRange(xmin, ymin, xmax, ymax);
       // cout << xmin << "\t" << xmax << endl;
        for (Int_t j = xmin; j <= xmax; j++) {
            fLUT[i]->SetBinContent(j, fGraph[i]->Eval(j) );
        }
    }//for

}

unsigned int cls_Calibrator::ExportLUTs(void)
{
    TString p_filename("LUTs.root");

    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    TFile v_outputFile(p_filename, "RECREATE");

    if (v_outputFile.IsZombie()) {
        cerr << "Error opening file " << p_filename.Data() << endl;
        gDirectory = prevDir;
        gFile = prevFile;
        return 1; // FAIL
    }


    for (Int_t i = 0; i < 64; i++) {
        fLUT[i]->Write();
    }


    v_outputFile.Close();

    cout << "Successfully exported analysis histograms into " << p_filename.Data() << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

/***
 * Check
 * 1) Singleton implementation
 * 2) Memory management
 */

#include "cls_Calibrator.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

cls_Calibrator& cls_Calibrator::Instance(void)
{
    static cls_Calibrator instance;
    return instance;
}

cls_Calibrator::cls_Calibrator(void)
{
    //TODO init data members
    fConstructed = kFALSE;
}

cls_Calibrator::~cls_Calibrator(void)
{
    //TODO Free memory if any
    this->Deconstruct();
}

void cls_Calibrator::Deconstruct(void)
{
    if (fConstructed) {
        //for (Int_t i = 0; i < 64; i++) {
        //    delete fLUT[i];                       // This causes double-free error. Why!?
        //}
        fConstructed = kFALSE;
    }
}

Float_t cls_Calibrator::GetCalibratedVal(UInt_t ch, Float_t val)
{
    if (!fConstructed) this->GenerateDummyLUTs();

    return fLUT[ch]->GetBinContent((UInt_t)val);
}

// Return value: 0 - ok, 1 - error
unsigned int cls_Calibrator::ImportGraphs(TString p_filename)
{
    //TODO check - check that there is no memory already allocated.
    //TODO If already allocated - delete! Next allocatation is invoked in GenerateLUTs().
    if (fConstructed) {
        this->Deconstruct();
    }

    TFile fr(p_filename.Data()); //TODO Specify access type

    //TODO check that the file is opened successfully and manage fr.Close() correspondingly

    TList *gList = fr.GetListOfKeys();
    const Int_t nEntries = gList->GetEntries();
    if (nEntries != 64) {
        cerr << "[cls_Calibrator::ImportGraphs] Something wrong, only " << nEntries << " graphs found instead of 64." << endl;
        return 1;
    }

    TString gName;
    for (Int_t i = 0; i < nEntries; i++) {
        gName.Form("gcal%d", i);
        fGraph[i] = (TGraph*)fr.Get(gName.Data());
        if (fGraph[i] == 0) {
            cerr << "[cls_Calibrator::ImportGraphs] Graph " << gName.Data() << " was not found in file " << fr.GetName() << endl;
            fr.Close();
            return 1;
        }
    }

    fr.Close();

    this->GenerateLUTs();

    this->ExportLUTs();

    return 0;
}

// This method should be called only from ImportGraphs()!
// So no need to check for fConstructed.
void cls_Calibrator::GenerateLUTs(void)
{
    TString hName;
    TString hTitle;
    const Double_t minX = -200.;     //minimal channel treated
    const Double_t maxX = 4096.;    //maximal channel treated

    // Auxilliary variables for graph range
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
            fLUT[i]->SetBinContent(j, fGraph[i]->Eval(j));
        }
    }//for

    fConstructed = kTRUE;
}

void cls_Calibrator::GenerateDummyLUTs(void)
{
    TString hName;
    TString hTitle;
    const Double_t minX = -200.;     //minimal channel treated
    const Double_t maxX = 4096.;    //maximal channel treated

    // Auxilliary variables for graph range
    Double_t xmin = 0;
    Double_t xmax = 0;
    Double_t ymin = 0;
    Double_t ymax = 0;

    for (Int_t i = 0; i < 64; i++) {
        hName.Form("calHist%d", i);
        hTitle.Form("do we need it for channel %d?", i);

        fLUT[i] = new TH1D(hName.Data(), hTitle.Data(), maxX - minX, minX, maxX);

        for (Int_t j = xmin; j <= xmax; j++) {
            fLUT[i]->SetBinContent(j, 1.);
        }
    }//for

    fConstructed = kTRUE;
}

unsigned int cls_Calibrator::ExportLUTs(TString p_filename)
{
    if (!fConstructed) return 1; // FAIL

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

#include "cls_Calibrator.h"
//#include <TROOT.h>
//#include <TObject.h>

cls_Calibrator::cls_Calibrator()
{

}

cls_Calibrator::~cls_Calibrator()
{

}

void cls_Calibrator::ImportGraphs(TString p_filename)
{

    TFile fr(p_filename.Data());

    TList *gList = fr.GetListOfKeys();
    const Int_t nEntries = gList->GetEntries();
    if (nEntries!=64) {
        gList->Error("cls_Calibrator::ImportGraphs", "Something wrong, only %d graphs found instead of 64", nEntries);
        return;
    }

    TString gName;
    for (Int_t i = 0; i < nEntries; i++) {
        gName.Form("gcal%d", i);
        fGraph[i] = (TGraph*)gList->FindObject(gName.Data());
        if (fGraph[i]==0) {
            fGraph[i]->Error("cls_Calibrator::ImportGraphs", "Graph %s was not found in file %s", gName.Data(), fr.GetName());
            fr.Close();
            return;
        }
    }

    this->GenerateLUTs();
}

void cls_Calibrator::GenerateLUTs(void)
{

    for (Int_t i = 0; i < 64; i++) {

    }

}

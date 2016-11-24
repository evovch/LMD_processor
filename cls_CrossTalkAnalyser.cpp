#include "cls_CrossTalkAnalyser.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "Rtypes.h"
#include <TFile.h>

#include "cls_pixelMap.h"

cls_CrossTalkAnalyser::cls_CrossTalkAnalyser()
{
    this->InitHistos();
}

cls_CrossTalkAnalyser::~cls_CrossTalkAnalyser()
{
    this->DeleteHistos();
}

void cls_CrossTalkAnalyser::InitHistos(void)
{
    TString histoName;
    TString histoTitle;

    for (UInt_t i=0; i<64; i++) {
        histoName.Form("adcInEventNoNeighbours_ch%d", i);
        histoTitle.Form("ADC spectra in events - no neighbours;channel;ADC value");
        fhAdcInEventNoNeighbours[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1024, 0., 4096.);

        histoName.Form("adcInEventWithTopNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events - with top neighbour;channel;ADC value");
        fhAdcInEventWithTopNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1024, 0., 4096.);

        histoName.Form("adcInEventWithBottomNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events - with bottom neighbour;channel;ADC value");
        fhAdcInEventWithBottomNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1024, 0., 4096.);

        histoName.Form("adcInEventWithLeftNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events - with left neighbour;channel;ADC value");
        fhAdcInEventWithLeftNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1024, 0., 4096.);

        histoName.Form("adcInEventWithRightNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events - with right neighbour;channel;ADC value");
        fhAdcInEventWithRightNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1024, 0., 4096.);

        histoName.Form("adcInEventWithAtLeastOneNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events - with at least one neighbour;channel;ADC value");
        fhAdcInEventWithAtLeastOneNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1024, 0., 4096.);

        histoName.Form("adcInEventWoBaselineNoNeighbours_ch%d", i);
        histoTitle.Form("ADC spectra in events without baseline - no neighbours;channel;ADC value");
        fhAdcInEventWoBaselineNoNeighbours[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1074*2, -200., 4096.);

        histoName.Form("adcInEventWoBaselineWithTopNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events without baseline - with top neighbour;channel;ADC value");
        fhAdcInEventWoBaselineWithTopNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1074*2, -200., 4096.);

        histoName.Form("adcInEventWoBaselineWithBottomNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events without baseline - with bottom neighbour;channel;ADC value");
        fhAdcInEventWoBaselineWithBottomNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1074*2, -200., 4096.);

        histoName.Form("adcInEventWoBaselineWithLeftNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events without baseline - with left neighbour;channel;ADC value");
        fhAdcInEventWoBaselineWithLeftNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1074*2, -200., 4096.);

        histoName.Form("adcInEventWoBaselineWithRightNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events without baseline - with right neighbour;channel;ADC value");
        fhAdcInEventWoBaselineWithRightNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1074*2, -200., 4096.);

        histoName.Form("adcInEventWoBaselineWithAtLeastOneNeighbour_ch%d", i);
        histoTitle.Form("ADC spectra in events without baseline - with at least one neighbour;channel;ADC value");
        fhAdcInEventWoBaselineWithAtLeastOneNeighbour[i] = new TH2D(histoName, histoTitle, 128, 0., 128., 1074*2, -200., 4096.);
    }
}

void cls_CrossTalkAnalyser::DeleteHistos(void)
{
    for (UInt_t i=0; i<64; i++) {
        delete fhAdcInEventNoNeighbours[i];
        delete fhAdcInEventWithTopNeighbour[i];
        delete fhAdcInEventWithBottomNeighbour[i];
        delete fhAdcInEventWithLeftNeighbour[i];
        delete fhAdcInEventWithRightNeighbour[i];
        delete fhAdcInEventWithAtLeastOneNeighbour[i];
        delete fhAdcInEventWoBaselineNoNeighbours[i];
        delete fhAdcInEventWoBaselineWithTopNeighbour[i];
        delete fhAdcInEventWoBaselineWithBottomNeighbour[i];
        delete fhAdcInEventWoBaselineWithLeftNeighbour[i];
        delete fhAdcInEventWoBaselineWithRightNeighbour[i];
        delete fhAdcInEventWoBaselineWithAtLeastOneNeighbour[i];
    }
}

unsigned int cls_CrossTalkAnalyser::ExportHistos(TString p_filename)
{
    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    TFile v_outputFile(p_filename, "RECREATE");

    if (v_outputFile.IsZombie()) {
        cerr << "Error opening file " << p_filename.Data() << endl;
        gDirectory = prevDir;
        gFile = prevFile;
        return 1; // FAIL
    }

    // ----------------------------------------

    gDirectory->mkdir("CrossTalk");
    gDirectory->cd("CrossTalk");

    for (UInt_t i=0; i<64; i++) {
        fhAdcInEventNoNeighbours[i]->Write();
        fhAdcInEventWithTopNeighbour[i]->Write();
        fhAdcInEventWithBottomNeighbour[i]->Write();
        fhAdcInEventWithLeftNeighbour[i]->Write();
        fhAdcInEventWithRightNeighbour[i]->Write();
        fhAdcInEventWithAtLeastOneNeighbour[i]->Write();
        fhAdcInEventWoBaselineNoNeighbours[i]->Write();
        fhAdcInEventWoBaselineWithTopNeighbour[i]->Write();
        fhAdcInEventWoBaselineWithBottomNeighbour[i]->Write();
        fhAdcInEventWoBaselineWithLeftNeighbour[i]->Write();
        fhAdcInEventWoBaselineWithRightNeighbour[i]->Write();
        fhAdcInEventWoBaselineWithAtLeastOneNeighbour[i]->Write();
    }

    gDirectory->cd("..");

    // ----------------------------------------

    v_outputFile.Close();

    cout << "Successfully exported analysis histograms into " << p_filename.Data() << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

void cls_CrossTalkAnalyser::ProcessEvent(std::vector<cls_Event>::iterator p_eventsIter)
{
    // The current event object
    cls_Event v_curEvent = *p_eventsIter;

    // Iterator over the hits of the event
    std::multimap< ULong64_t, std::pair<UChar_t, UShort_t> >::iterator v_eventHitsIter;

    // Analyse only if there are more than 1 hit in the event
    /*if (p_eventsIter->GetNhits() <= 1) {
        return;
    }*/

    /*cout << "CH:\t";
    for (v_eventHitsIter = v_curEvent.fEventTimeAdcMap.begin(); v_eventHitsIter != v_curEvent.fEventTimeAdcMap.end(); ++v_eventHitsIter) {
        cout << (UInt_t)(*v_eventHitsIter).second.first << "\t";
    }
    cout << endl;
    cout << "PX:\t";
    for (v_eventHitsIter = v_curEvent.fEventTimeAdcMap.begin(); v_eventHitsIter != v_curEvent.fEventTimeAdcMap.end(); ++v_eventHitsIter) {
        cout << fPixelMap->GetPixelFromFebCh((*v_eventHitsIter).second.first) << "\t";
    }
    cout << endl;*/

    // Loop over the studied pixels
    for (UInt_t curChPixel=0; curChPixel<64; curChPixel++) {

        Bool_t curPixelFound = kFALSE;
        Bool_t topNeighborFound = kFALSE;
        Bool_t bottomNeighborFound = kFALSE;
        Bool_t leftNeighborFound = kFALSE;
        Bool_t rightNeighborFound = kFALSE;

        //cout << "Searching for channel " << curChPixel << "\t(pixel " <<  fPixelMap->GetPixelFromFebCh(curChPixel) << ")" << endl;

        // Loop over the hits of the event
        // First we determine if there are pairs of hits to be analysed in this event
        for (v_eventHitsIter = v_curEvent.fEventTimeAdcMap.begin(); v_eventHitsIter != v_curEvent.fEventTimeAdcMap.end(); ++v_eventHitsIter) {

            // Extract channel number from the hit
            uint8_t v_curHitChannel = (*v_eventHitsIter).second.first;

            if ((UInt_t)v_curHitChannel == curChPixel) curPixelFound = kTRUE;
            if ((Int_t)v_curHitChannel == fPixelMap->GetTopNeighbor(curChPixel)) topNeighborFound = kTRUE;
            if ((Int_t)v_curHitChannel == fPixelMap->GetBottomNeighbor(curChPixel)) bottomNeighborFound = kTRUE;
            if ((Int_t)v_curHitChannel == fPixelMap->GetLeftNeighbor(curChPixel)) leftNeighborFound = kTRUE;
            if ((Int_t)v_curHitChannel == fPixelMap->GetRightNeighbor(curChPixel)) rightNeighborFound = kTRUE;
        }

        /*if (curPixelFound) cout << "\t-\tFound!" << endl;
        if (topNeighborFound) cout << "\t-\tFound top" << endl;
        if (bottomNeighborFound) cout << "\t-\tFound bottom" << endl;
        if (leftNeighborFound) cout << "\t-\tFound left" << endl;
        if (rightNeighborFound) cout << "\t-\tFound right" << endl;*/

        // Loop over the hits of the event
        // Fill the histograms depending on what pairs are found
        for (v_eventHitsIter = v_curEvent.fEventTimeAdcMap.begin(); v_eventHitsIter != v_curEvent.fEventTimeAdcMap.end(); ++v_eventHitsIter) {

            // Extract time, channel and adc values from the hit
            //uint64_t v_curHitTime = (*v_eventHitsIter).first;
            uint8_t v_curHitChannel = (*v_eventHitsIter).second.first;
            uint16_t v_curHitAdc = (*v_eventHitsIter).second.second;

            // Extract REAL ADC value by subtracting the pedestal
            //TODO check data types and casting...
            Double_t v_realADCval = (Double_t)fPedestals[v_curHitChannel] - (Double_t)v_curHitAdc;

            if (curPixelFound && !topNeighborFound && !bottomNeighborFound && !leftNeighborFound && !rightNeighborFound) {
                fhAdcInEventNoNeighbours[curChPixel]->Fill(v_curHitChannel, v_curHitAdc);
                fhAdcInEventWoBaselineNoNeighbours[curChPixel]->Fill(v_curHitChannel, v_realADCval);
            }
            if (curPixelFound && topNeighborFound && !bottomNeighborFound && !leftNeighborFound && !rightNeighborFound) {
                fhAdcInEventWithTopNeighbour[curChPixel]->Fill(v_curHitChannel, v_curHitAdc);
                fhAdcInEventWoBaselineWithTopNeighbour[curChPixel]->Fill(v_curHitChannel, v_realADCval);
            }
            if (curPixelFound && !topNeighborFound && bottomNeighborFound && !leftNeighborFound && !rightNeighborFound) {
                fhAdcInEventWithBottomNeighbour[curChPixel]->Fill(v_curHitChannel, v_curHitAdc);
                fhAdcInEventWoBaselineWithBottomNeighbour[curChPixel]->Fill(v_curHitChannel, v_realADCval);
            }
            if (curPixelFound && !topNeighborFound && !bottomNeighborFound && leftNeighborFound && !rightNeighborFound) {
                fhAdcInEventWithLeftNeighbour[curChPixel]->Fill(v_curHitChannel, v_curHitAdc);
                fhAdcInEventWoBaselineWithLeftNeighbour[curChPixel]->Fill(v_curHitChannel, v_realADCval);
            }
            if (curPixelFound && !topNeighborFound && !bottomNeighborFound && !leftNeighborFound && rightNeighborFound) {
                fhAdcInEventWithRightNeighbour[curChPixel]->Fill(v_curHitChannel, v_curHitAdc);
                fhAdcInEventWoBaselineWithRightNeighbour[curChPixel]->Fill(v_curHitChannel, v_realADCval);
            }
            if (curPixelFound && (topNeighborFound || bottomNeighborFound || leftNeighborFound || rightNeighborFound)) {
                fhAdcInEventWithAtLeastOneNeighbour[curChPixel]->Fill(v_curHitChannel, v_curHitAdc);
                fhAdcInEventWoBaselineWithAtLeastOneNeighbour[curChPixel]->Fill(v_curHitChannel, v_realADCval);
            }


        } // End of the loop over the hits of the event

    } // End of the loop over the studied pixels

}

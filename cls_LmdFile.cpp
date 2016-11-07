#include "cls_LmdFile.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::cerr;
using std::endl;

#include <cstdio>

#include <TFile.h>
#include <TTree.h>

#include "external/LmdFile.h"
#include "support.h"
#include "HistoWidget.h"
#include "cls_RootEvent.h"
#include "cls_Calibrator.h"

//#define DEBUGMODE

// =============================================================================================================================
// =============================================================================================================================

cls_LmdFile::cls_LmdFile() :
    mRawData(nullptr)
{
    this->InitHistos();

    for (unsigned int i=0; i<128; i++) fEffCalib[i] = 0.;
    for (unsigned int i=0; i<128; i++) fPedestals[i] = 0.;
}

// =============================================================================================================================
// =============================================================================================================================

cls_LmdFile::~cls_LmdFile()
{
    if (mRawData) delete [] mRawData; mRawData = nullptr;

    this->DeleteHistos();
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::InitHistos(void)
{
    // Raw data analysis
    fhAdcAll = new TH2D("adcAll", "ADC spectra all;channel;ADC value", 128, 0., 128., 1024, 0., 4096.);
    fhAdcAllWoBaseline = new TH2D("adcAllWoBaseline", "ADC spectra all without baseline;channel;ADC value",
                                            128, 0., 128., 1074*2, -200., 4096.);
    fhAdcAllSumWoBaseline = new TH1D("adcAllSum", "adcAllSum;ADC value;Entries", 1074*2, -200., 4096.);

    // Callibrated data analysis
    fhCalAdcAllWoBaseline1e = new TH2D("cal1eAdcAllWoBaseline", "ADC spectra all without baseline; channel; 1e value",  64, 0., 64., 1074*2, -1.5, 20.);
    fhCalAdcAllSumWoBaseline = new TH1D("calAdcAllSum", "adcAllSum;ADC value;Entries;calibrated", 1074*2, -2., 50.);

    fhCalAdcAllWoBaselineNonLinear = new TH2D("fhCalAdcAllWoBaselineNonLinear", "fhCalAdcAllWoBaselineNonLinear", 128, 0., 128., 1074*2, -200., 4096.);
    fhCalAdcAllSumWoBaselineNonLinear = new TH1D("fhCalAdcAllSumWoBaselineNonLinear", "fhCalAdcAllSumWoBaselineNonLinear", 1074*2, -200., 4096.);

    // Processed data analysis
    fhAdcInEvent = new TH2D("adcInEvent", "ADC spectra in events;channel;ADC value", 128, 0., 128., 1024, 0., 4096.);
    fhAdcBaseline = new TH2D("adcBaseline", "baseline;channel;ADC value", 128, 0., 128., 1074*2, -200., 4096.);
    fhAdcInEventWoBaseline = new TH2D("adcInEventWoBaseline", "ADC spectra in events without baseline;channel;ADC value",
                                            128, 0., 128., 1074*2, -200., 4096.);

    fhAdcSumPerEvent = new TH1D("adcSumPerEvent", "adcSumPerEvent", 1074*5, -2000., 40960.);
    fhAdcSumPerEventSingleHitEvents = new TH1D("adcSumPerEventSingleHitEvents", "adcSumPerEventSingleHitEvents", 1074*5, -2000., 40960.);
    // Timing
    fhMinusFirstInTime = new TH1D("minusFirstInTime", "minusFirstInTime;ns;Entries", 120, 0., 120.);
    fhDistanceBetwEvents = new TH1D("distanceBetwEvents", "distanceBetwEvents;ns;Entries", 120, 0., 120.);

    // Event building
    fhNumOfHitInEvent = new TH1D("numOfHitInEvent", "Number of hits in event;N;Entries", 150, 0., 150.);
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::DeleteHistos(void)
{
    // Raw data analysis
    delete fhAdcAll;
    delete fhAdcAllWoBaseline;
    delete fhAdcAllSumWoBaseline;

    // Callibrated data analysis
    delete fhCalAdcAllWoBaseline1e;
    delete fhCalAdcAllSumWoBaseline;

    delete fhCalAdcAllWoBaselineNonLinear;
    delete fhCalAdcAllSumWoBaselineNonLinear;

    // Processed data analysis
    delete fhAdcInEvent;
    delete fhAdcBaseline;
    delete fhAdcInEventWoBaseline;
    delete fhAdcSumPerEvent;
    delete fhAdcSumPerEventSingleHitEvents;
    // Timing
    delete fhMinusFirstInTime;
    delete fhDistanceBetwEvents;

    // Event building
    delete fhNumOfHitInEvent;
}

// =============================================================================================================================
// =============================================================================================================================

unsigned int cls_LmdFile::ExportHistos(void)
{
    // Export event building histograms  -- ????? wat?

    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    TFile v_outputFile(mOutputHistoFilename, "RECREATE");

    if (v_outputFile.IsZombie()) {
        cerr << "Error opening file " << mOutputHistoFilename.Data() << endl;
        gDirectory = prevDir;
        gFile = prevFile;
        return 1; // FAIL
    }

    // ----------------------------------------

    // Raw data analysis
    fhAdcAll->Write();
    fhAdcAllWoBaseline->Write();
    fhAdcAllSumWoBaseline->Write();

    // Callibrated data analysis
    fhCalAdcAllWoBaseline1e->Write();
    fhCalAdcAllSumWoBaseline->Write();

    fhCalAdcAllWoBaselineNonLinear->Write();
    fhCalAdcAllSumWoBaselineNonLinear->Write();

    // Processed data analysis
    fhAdcInEvent->Write();
    fhAdcBaseline->Write();
    fhAdcInEventWoBaseline->Write();
    fhAdcSumPerEvent->Write();
    fhAdcSumPerEventSingleHitEvents->Write();
    // Timing
    fhMinusFirstInTime->Write();
    fhDistanceBetwEvents->Write();

    // Event building
    fhNumOfHitInEvent->Write();

    // ----------------------------------------

    v_outputFile.Close();

    cout << "Successfully exported analysis histograms into " << mOutputHistoFilename.Data() << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::ShowHistos(void)
{
    /*cls_HistoWidget* v_histoW = new cls_HistoWidget(fhAdcSumPerEvent);
    v_histoW->show();
    */

    //TODO implement what you want here
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::ImportPedestals(QString p_filename)
{
    std::ifstream infile(p_filename.toStdString());

    int a, b, ch;
    float pedestal, smth;

    for (unsigned int i=0; i<128; i++) {
        infile >> a >> b >> ch >> pedestal >> smth;
        fPedestals[ch] = pedestal;
    }
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::ImportEffCalib(QString p_filename)
{
    std::ifstream infile(p_filename.toStdString());

    int ch;
    float val;

    //TODO by now only 64
    for (unsigned int i=0; i<64; i++) {
        infile >> ch >> val;
        fEffCalib[ch] = val;
    }
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::ImportGraphsFile(QString p_filename)
{
    TString v_filename(p_filename.toStdString());
    cls_Calibrator::Instance().ImportGraphs(v_filename);
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::StartProcessing(QString p_filename)
{
    mFilename = p_filename.toStdString();

    this->RunUnpacking();
    this->RunEventBuilding();
    this->ExportEventsRootTree();
    this->RunEventsAnalysis();
    this->ExportHistos();
    this->ShowHistos();
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::RunUnpacking(void)
{
    mbs::LmdFile f;
    if (!f.OpenRead(mFilename.Data())) {
       cout << "Error opening file '" << mFilename.Data() << "'" << endl;
    }
    cout << "Successfully opened file '" << mFilename.Data() << "'" << endl;

    // Declarations
    uint32_t currentEpoch = 0;
    uint64_t hitFullTime = 0;
    std::pair<uint8_t, uint16_t> curChAdcPair;

    mbs::EventHeader* hdr = 0;
    mbs::SubeventHeader* subEventHdr = 0;

    unsigned int eventCounter = 0;
    unsigned int subEventCounter = 0;

    uint16_t* subEvData = 0;

    // Loop over events in the LMD file
    while ((hdr = f.ReadEvent()) != 0)
    {
#ifdef DEBUGMODE
       printf("%d: Event %u length %u num %u\n", eventCounter, hdr->EventNumber(), hdr->SubEventsSize(), hdr->NumSubevents());
#endif

       subEventHdr = 0;
       subEventCounter = 0;

       // Loop over subevents in the current event
       while ((subEventHdr = hdr->NextSubEvent(subEventHdr)) != 0)
       {

#ifdef DEBUGMODE
          uint32_t subEvNwords = subEventHdr->iWords; // subsubevent<(subEvNwords-8)/3
          printf ("\t%d: Subevent\tiWords=%d\tRawSize=% 4d\tRawSize/6=% 4d\n", subEventCounter, subEvNwords, subEventHdr->RawDataSize(), subEventHdr->RawDataSize()/6);
#endif
          subEvData = (uint16_t*)(subEventHdr->RawData());

          for (uint32_t subsubevent=0; subsubevent<subEventHdr->RawDataSize()/6; subsubevent++)
          {
             uint8_t type; // 3 bits used
#ifdef DEBUGMODE
             uint8_t roc; // 3 bits used
             uint8_t nx; // 2 bits used
             uint8_t ltsmsb; // 3 bits used
#endif
             uint16_t timestamp; // 14 bits
             uint8_t ch; // 7 bits used
             uint16_t adc; // 12 bits used
             uint32_t epoch; // 32 bits used
#ifdef DEBUGMODE
             uint32_t evnum; // 24 bits used
             uint8_t flag; // 2 bits used
#endif
             uint32_t bufferCursor = subsubevent*3;
             type = subEvData[bufferCursor+0] & 0x7; // 3 bits

             Double_t effCalibratedADC;
             Double_t calibratedVal;
             Double_t pedMinusAdcVal;

             switch (type) {
             case 0: //============================================================================================================================
#ifdef DEBUGMODE
                printf ("NOP");// printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
                printf ("\n");
#endif
                break;

             case 1: //============================================================================================================================
                timestamp = (((subEvData[bufferCursor+1] & 0x01FF) << 5) & 0x3FE0) | ((subEvData[bufferCursor+0] >> 11) & 0x001F);
                ch = (subEvData[bufferCursor+1] >> 9) & 0x007F; // 7 bits
                adc = (subEvData[bufferCursor+2] >> 1) & 0x0FFF; // 12 bits

                // Take into account 'last epoch' flag
                if (((subEvData[bufferCursor+2] >> 15) & 0x1) == 1)
                   hitFullTime = FullTimeStamp(currentEpoch-1, timestamp);
                else
                   hitFullTime = FullTimeStamp(currentEpoch, timestamp);

#ifdef DEBUGMODE
                printf ("HIT "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
                roc = (subEvData[bufferCursor+0] >> 3) & 0x7; // 3 bits
                nx = (subEvData[bufferCursor+0] >> 6) & 0x3; // 2 bits
                ltsmsb = (subEvData[bufferCursor+0] >> 8) & 0x7; // 3 bits
                printf ("HIT "); printf ("Roc:%d Nx:%d Chn:%02x Ts:%04x Adc:%03x\n", roc, nx, ch, hitFullTime, adc); //FIXME printf types

                // FullTime:%ld - hitFullTime
                printf("Msg:1 Roc:%d Nx:%d Chn:%02x Ts:%04x Last:%d Msb:%d Adc:%03x Pup:%d Oflw:%d\n",
                                        roc, nx, ch, timestamp,
                                        (subEvData[bufferCursor+2] >> 15) & 0x1,  // 1 bit last epoch
                                        ltsmsb, adc,
                                        (subEvData[bufferCursor+2] >> 13) & 0x1,   // 1 bit pile-up
                                        (subEvData[bufferCursor+2] >> 14) & 0x1   // 1 bit overflow
                                        );
#endif

                // Raw
                fhAdcAll->Fill(ch, adc);
                pedMinusAdcVal = fPedestals[ch] - adc;
                fhAdcAllWoBaseline->Fill(ch, pedMinusAdcVal);
                fhAdcAllSumWoBaseline->Fill(pedMinusAdcVal);
                // Eff calibrated
                effCalibratedADC = (fPedestals[ch] - adc)/fEffCalib[ch];
                fhCalAdcAllWoBaseline1e->Fill(ch, effCalibratedADC);
                fhCalAdcAllSumWoBaseline->Fill(effCalibratedADC);

                // Calibrated using LUTs
                calibratedVal = cls_Calibrator::Instance().GetCalibratedVal(ch, pedMinusAdcVal);
                fhCalAdcAllWoBaselineNonLinear->Fill(ch, calibratedVal);
                fhCalAdcAllSumWoBaselineNonLinear->Fill(calibratedVal);

                curChAdcPair = std::make_pair(ch, adc);
                fTimeAdcMap.insert(std::pair<uint32_t, std::pair<uint8_t, uint16_t> >(hitFullTime, curChAdcPair));
                break;

             case 2: //============================================================================================================================
                epoch = ((subEvData[bufferCursor+2] & 0x00FF) << 24) | (subEvData[bufferCursor+1] << 8) | ((subEvData[bufferCursor+0] >> 8) & 0x00FF);
#ifdef DEBUGMODE
                roc = (subEvData[bufferCursor+0] >> 3) & 0x7; // 3 bits
                printf ("EPO "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
                printf ("EPO %x\n", epoch);
                printf("Msg:2 Roc:%d Epoch:%x Missed:00\n", roc, epoch);
#endif
                // Save epoch and use it later in hit/aux unpacking for full time determination
                currentEpoch = epoch;
                break;

             case 3: //============================================================================================================================
                ch =  (subEvData[bufferCursor+0] >> 6) & 0x3; // 2 bits
///                timestamp = (((subEvData[bufferCursor+1] /*& 0x3F*/) << 8) & 0x3F00 ) | ((subEvData[bufferCursor+0] >> 8) & 0xFF); //TODO check and fix
                timestamp = (((subEvData[bufferCursor+1] /*& 0x3F*/) << 9) & 0x3E00 ) | ((subEvData[bufferCursor+0] >> 7) & 0x01FF);
                hitFullTime = FullTimeStamp(currentEpoch-1, timestamp);
#ifdef DEBUGMODE
                roc = (subEvData[bufferCursor+0] >> 3) & 0x7; // 3 bits
                printf ("SYN "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
                evnum = ((subEvData[bufferCursor+2] & 0x3FFF) << 10) | ((subEvData[bufferCursor+1] >> 6) & 0x03FF);
                flag = (subEvData[bufferCursor+2] >> 14) & 0x3;
                printf ("SYN %04x\n", hitFullTime);
                printf("Msg:3 Roc:%d SyncChn:%d EpochLSB:0 Ts:%04x Data:%x Flag:%x\n", roc, ch, timestamp, evnum, flag);
#endif
                break;

             case 4: //============================================================================================================================
                ch =  (subEvData[bufferCursor+0] >> 6) & 0x7F; // 7 bits
///                timestamp = (((subEvData[bufferCursor+1] << 3) & 0x3FF8) | ((subEvData[bufferCursor+0] >> 13) & 0x7)) & 0x03FFF; //TODO check and fix
                timestamp = (((subEvData[bufferCursor+1] << 4) & 0x3FF0) | ((subEvData[bufferCursor+0] >> 12) & 0xF)) & 0x03FFF;

                // Take into account 'epoch lsb'
                if (((subEvData[bufferCursor+1] >> 10) & 0x1) == (currentEpoch & 0x1))
                   hitFullTime = FullTimeStamp(currentEpoch, timestamp);
                else
                   hitFullTime = FullTimeStamp(currentEpoch-1, timestamp);

                fTimeAuxMap.insert(std::pair<uint32_t, uint8_t>(hitFullTime, ch));

#ifdef DEBUGMODE
                roc = (subEvData[bufferCursor+0] >> 3) & 0x7; // 3 bits
                printf ("AUX "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
                // FullTime:%ld hitFullTime
                printf("Msg:4 Roc:%d AuxChn:%02d EpochLSB:%d Ts:%04x Falling:%d Overflow:%d\n", roc, ch, (subEvData[bufferCursor+1] >> 10) & 0x1, timestamp, 0, 0);
#endif
                break;

             case 7: //============================================================================================================================
#ifdef DEBUGMODE
                printf ("SYS "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
                printf ("\n");
#endif
                break;

             default: //============================================================================================================================
#ifdef DEBUGMODE
                printf ("--- "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
                printf ("\n");
#endif
                break;
             }
          }
          subEventCounter++;
       }

       eventCounter++;
       //TODO - you can set the limit on the number of imported DAQ-events
       //if (eventCounter > 100000) break;
    }

}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::RunEventBuilding(void)
{
    // Last hit timestamp
    uint64_t v_lastTimestamp = 0;

    // Current event object
    cls_Event v_curEvent;

    // Loop over all the recorded hits
    std::multimap< uint64_t, std::pair<uint8_t, uint16_t> >::iterator v_mainIter;
    for (v_mainIter = fTimeAdcMap.begin(); v_mainIter != fTimeAdcMap.end(); ++v_mainIter)
    {
        uint64_t v_currentTimestamp = (*v_mainIter).first;
        std::pair<uint8_t, uint16_t> v_currentHit = (*v_mainIter).second;

        //TODO set the distance between the events here!
        if (v_currentTimestamp > (v_lastTimestamp + 30)) {

            // if the distance is more than the threshold -
            // push the last event (if it is not empty)
            // and start a new event
            if (v_curEvent.GetNhits() > 0) {
                fEvents.push_back(v_curEvent);

                fhNumOfHitInEvent->Fill(v_curEvent.GetNhits());

                v_curEvent.Clear();
            }

            // This is the first hit of the event
            v_curEvent.AddHit(v_currentTimestamp, v_currentHit.first, v_currentHit.second);
            v_lastTimestamp = v_currentTimestamp;

        } else {
            // The current hit is not so far from the last hit
            v_curEvent.AddHit(v_currentTimestamp, v_currentHit.first, v_currentHit.second);
        }

    }

    // Push the last event
    if (v_curEvent.GetNhits() > 0) {
        fEvents.push_back(v_curEvent);
        v_curEvent.Clear();
    }
}

// =============================================================================================================================
// =============================================================================================================================

unsigned int cls_LmdFile::ExportEventsRootTree()
{
    TFile theTreeFile(mOutputTreeFilename, "RECREATE");
    TTree theTree("theTree", "A tree with events");

    cls_RootEvent* v_event = new cls_RootEvent();

    theTree.Branch("theBranch", "cls_RootEvent", v_event);

    ULong_t eventsCounter = 0;

    // Loop over the events
    for (std::vector<cls_Event>::iterator v_eventsIter = fEvents.begin(); v_eventsIter != fEvents.end(); ++v_eventsIter)
    {
        // The current event object
        cls_Event v_curEvent = *v_eventsIter;

        //cout << "================================================================================================" << endl;

        // Loop over the hits of the current event
        std::multimap< ULong64_t, std::pair<UChar_t, UShort_t> >::iterator v_eventHitsIter;
        for (v_eventHitsIter = v_curEvent.fEventTimeAdcMap.begin(); v_eventHitsIter != v_curEvent.fEventTimeAdcMap.end(); ++v_eventHitsIter) {

            // Extract time, channel and adc values from the hit
            ULong64_t v_curHitTime = (*v_eventHitsIter).first;
            UChar_t v_curHitChannel = (*v_eventHitsIter).second.first;
            UShort_t v_curHitAdc = (*v_eventHitsIter).second.second;

            // Extract REAL ADC value by subtracting the pedestal
            //TODO check data types and casting...
            Int_t v_realADCval = (int32_t)fPedestals[v_curHitChannel] - (int32_t)v_curHitAdc;

            //printf ("%ld\t%d\t%d\t%d\n", v_curHitTime, v_curHitChannel, v_curHitAdc, v_realADCval);

            v_event->AddHit(v_curHitTime, v_curHitChannel, v_curHitAdc, v_realADCval);

        }
        v_event->SetID(eventsCounter++);

        theTree.Fill();      // Fill the tree
        v_event->Clear();    // Clear before reloading event
    }

    theTreeFile.Write();     // Write the file header
    //theTree.Print();       // Print the tree contents

    delete v_event;

    cout << "Successfully exported root tree into " << mOutputTreeFilename.Data() << "." << endl;

    return 0; // OK
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::RunEventsAnalysis(void)
{
    // Fill the baseline histograms from the pedestals
    for (unsigned int i=0; i<128; i++) {
        fhAdcBaseline->Fill(i, fPedestals[i]);
    }

    // The timestamp of the first hit in the previous event
    uint64_t v_lastEventStartTime = 0;

    // Loop over the events
    for (std::vector<cls_Event>::iterator v_eventsIter = fEvents.begin(); v_eventsIter != fEvents.end(); ++v_eventsIter)
    {
        //v_eventsIter->Dump(); // for debugging

        // The current event object
        cls_Event v_curEvent = *v_eventsIter;

        // The event contains at least one hit so here we store this first (or possibly only) hit of the current event.
        std::multimap< ULong64_t, std::pair<UChar_t, UShort_t> >::iterator v_eventHitsIter = v_curEvent.fEventTimeAdcMap.begin();

        // The timestamp of the first hit
        ULong64_t v_curEventStartTime = (*v_eventHitsIter).first;
        // Fill into the histogram the distance between the first hit of the current event and the first hit of the previous event
        if (v_lastEventStartTime != 0) {
            fhDistanceBetwEvents->Fill(v_curEventStartTime - v_lastEventStartTime);
        }
        v_lastEventStartTime = v_curEventStartTime;


        // The event contains just one hit
        if (v_eventsIter->GetNhits() == 1)
        {
            // v_eventHitsIter is the only hit

            // Its parameters
            //uint64_t v_curHitTime = (*v_eventHitsIter).first;
            uint8_t v_curHitChannel = (*v_eventHitsIter).second.first;
            uint16_t v_curHitAdc = (*v_eventHitsIter).second.second;

            // Extract REAL ADC value by subtracting the pedestal
            //TODO check data types and casting...
            double v_realADCval = (double)fPedestals[v_curHitChannel] - (double)v_curHitAdc;

            // Consider this only hit of the event as the real hit of the real event
            //TODO set the threshold
            /*if (v_realADCval > 350.)*/
            {
                fhAdcInEvent->Fill(v_curHitChannel, v_curHitAdc);
                fhAdcInEventWoBaseline->Fill(v_curHitChannel, v_realADCval);
                fhAdcSumPerEvent->Fill(v_realADCval);
                fhAdcSumPerEventSingleHitEvents->Fill(v_realADCval);
            }
        } // end of case of a single-hit event

        // The event contains more than 1 hit
        else if (v_eventsIter->GetNhits() > 1)
        {
            double v_adc_sum = 0.;

            // Process first hit in the event -----------------------------------------------------
            //v_eventHitsIter = v_curEvent.fEventTimeAdcMap.begin(); // this is not really needed
            uint64_t v_firstTime = (*v_eventHitsIter).first;

            // Extract time, channel and adc values from the hit
            uint64_t v_curHitTime = (*v_eventHitsIter).first;
            uint8_t v_curHitChannel = (*v_eventHitsIter).second.first;
            uint16_t v_curHitAdc = (*v_eventHitsIter).second.second;

            // Extract REAL ADC value by subtracting the pedestal
            //TODO check data types and casting...
            double v_realADCval = (double)fPedestals[v_curHitChannel] - (double)v_curHitAdc;

            // Sum the ADC values inside the event
            v_adc_sum += v_realADCval;

            // Fill histos
            fhAdcInEvent->Fill(v_curHitChannel, v_curHitAdc);
            fhAdcInEventWoBaseline->Fill(v_curHitChannel, v_realADCval);

            // ------------------------------------------------------------------------------------
            // Go to the next hit (from the first)
            ++v_eventHitsIter;

            // Process the rest of the hits in the event
            for (/* Nothing here*/; v_eventHitsIter != v_curEvent.fEventTimeAdcMap.end(); ++v_eventHitsIter) {

                // Extract time, channel and adc values from the hit
                v_curHitTime = (*v_eventHitsIter).first;
                v_curHitChannel = (*v_eventHitsIter).second.first;
                v_curHitAdc = (*v_eventHitsIter).second.second;

                // Extract REAL ADC value by subtracting the pedestal
                //TODO check data types and casting...
                double v_realADCval = (double)fPedestals[v_curHitChannel] - (double)v_curHitAdc;

                // Sum the ADC values inside the event
                v_adc_sum += v_realADCval;

                // Fill histos
                fhAdcInEvent->Fill(v_curHitChannel, v_curHitAdc);
                fhAdcInEventWoBaseline->Fill(v_curHitChannel, v_realADCval);
                fhMinusFirstInTime->Fill(v_curHitTime - v_firstTime);
            } // end of loop over the hits of the event

            fhAdcSumPerEvent->Fill(v_adc_sum);

        } // end of case of multi-hit event

    }

}

// =============================================================================================================================
// =============================================================================================================================

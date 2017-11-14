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
#include "cls_pixelMap.h"
#include "cls_CrossTalkAnalyser.h"

//#define DEBUGMODE

//#define DO_CROSSTALK

// =============================================================================================================================
// =============================================================================================================================

cls_LmdFile::cls_LmdFile() :
    mRawData(nullptr),
    fNhitsUnpacked(0),
    fNauxUnpacked(0),
    fNsyncUnpacked(0),
    fNhitsPushedIntoEvents(0),
    fPixelMap(nullptr),
    fShowHistograms(kFALSE)
{
    this->InitHistos();

    for (unsigned int i=0; i<128; i++) fEffCalib[i] = 0.;
    for (unsigned int i=0; i<128; i++) fPedestals[i] = 0.;

    fPedestalsCorrection[0] = 25;
    fPedestalsCorrection[1] = 19;
    fPedestalsCorrection[2] = 23;
    fPedestalsCorrection[3] = 21;
    fPedestalsCorrection[4] = 21;
    fPedestalsCorrection[5] = 19;
    fPedestalsCorrection[6] = 27;
    fPedestalsCorrection[7] =	25;
    fPedestalsCorrection[8] =	23;
    fPedestalsCorrection[9] =	-3;
    fPedestalsCorrection[10] =	27;
    fPedestalsCorrection[11] =	23;
    fPedestalsCorrection[12] =	1;
    fPedestalsCorrection[13] =	1;
    fPedestalsCorrection[14] =	3;
    fPedestalsCorrection[15] =	23;
    fPedestalsCorrection[16] =	27;
    fPedestalsCorrection[17] =	19;
    fPedestalsCorrection[18] =	25;
    fPedestalsCorrection[19] =	17;
    fPedestalsCorrection[20] =	19;
    fPedestalsCorrection[21] =	29;
    fPedestalsCorrection[22] =	15;
    fPedestalsCorrection[23] =	21;
    fPedestalsCorrection[24] =	19;
    fPedestalsCorrection[25] =	25;
    fPedestalsCorrection[26] =	1;
    fPedestalsCorrection[27] =	21;
    fPedestalsCorrection[28] =	21;
    fPedestalsCorrection[29] =	19;
    fPedestalsCorrection[30] =	17;
    fPedestalsCorrection[31] =	3;
    fPedestalsCorrection[32] =	17;
    fPedestalsCorrection[33] =	21;
    fPedestalsCorrection[34] =	21;
    fPedestalsCorrection[35] =	25;
    fPedestalsCorrection[36] =	21;
    fPedestalsCorrection[37] =	23;
    fPedestalsCorrection[38] =	15;
    fPedestalsCorrection[39] =	23;
    fPedestalsCorrection[40] =	15;
    fPedestalsCorrection[41] =	19;
    fPedestalsCorrection[42] =	-1;
    fPedestalsCorrection[43] =	15;
    fPedestalsCorrection[44] =	15;
    fPedestalsCorrection[45] =	-3;
    fPedestalsCorrection[46] =	19;
    fPedestalsCorrection[47] =	11;
    fPedestalsCorrection[48] =	23;
    fPedestalsCorrection[49] =	19;
    fPedestalsCorrection[50] =	19;
    fPedestalsCorrection[51] =	15;
    fPedestalsCorrection[52] =	19;
    fPedestalsCorrection[53] =	3;
    fPedestalsCorrection[54] =	21;
    fPedestalsCorrection[55] =	25;
    fPedestalsCorrection[56] =	27;
    fPedestalsCorrection[57] =	17;
    fPedestalsCorrection[58] =	19;
    fPedestalsCorrection[59] =	17;
    fPedestalsCorrection[60] =	-3;
    fPedestalsCorrection[61] =	17;
    fPedestalsCorrection[62] =	13;
    fPedestalsCorrection[63] =	21;


#ifdef DO_CROSSTALK
    fCrossTalkAnalyser = new cls_CrossTalkAnalyser();
#endif
}

// =============================================================================================================================
// =============================================================================================================================

cls_LmdFile::~cls_LmdFile()
{
    if (mRawData) delete [] mRawData; mRawData = nullptr;
    //if (fPixelMap) delete fPixelMap; fPixelMap = nullptr;  // crash. Why?

    this->DeleteHistos();

#ifdef DO_CROSSTALK
    delete fCrossTalkAnalyser;
#endif
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
    fhHitsPerChannel = new TH1D("fhHitsPerChannel", "Number of HIT messages per channel;Channel;Entries", 128, 0., 128.);

    // Callibrated data analysis
    fhCalAdcAllWoBaseline1e = new TH2D("calAdcAll", "ADC spectra all without baseline; channel; 1e value",  64, 0., 64., 1074*2, -1.5, 50.);
    fhCalAdcAllSumWoBaseline = new TH1D("calAdcAllSum", "adcAllSum;ADC value;Entries;calibrated", 1074*2, -2., 50.);

    // corrected data for non-linearity
    fhAdcAllWoBaselineNonLinear = new TH2D("fhAdcAllWoBaselineNonLinear", "Amplitudes corrected for non-linearity", 128, 0., 128., 1074*2, -200., 4096.);
    fhAdcAllSumWoBaselineNonLinear = new TH1D("fhAdcAllSumWoBaselineNonLinear", "Amplitudes corrected for non-linearity: sum", 1074*2, -200., 4096.);
//    fhCalAdcAllWoBaselineNonLinear = new TH2D("cal1eAdcWoBaselineNonLinear", "1e corrected for non-linearity", 64, 0., 64., 1074*2, -0.5, 20.);
//    fhAdcAllSumWoBaselineNonLinear = new TH1D("cal1eAdcSumWoBaselineNonLinear", "1e corrected for non-linearity", 1074*2, -0.5, 20.);

    //corrected for NL pedestals
    fhAdcAllWoBaselineNLcorr = new TH2D("fhAdcAllWoBaselineNLcorr", "Two star amplitudes", 64, 0., 64., 1074*2, -200., 4096.);
    fhAdcAllSumWoBaselineNLcorr = new TH1D("fhAdcAllSumWoBaselineNLcorr", "Two star amplitudes: sum", 1074*2, -200., 4096.);

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

    // Heat map
    fhHeatMap = new TH2D("heatMap", "Heat map", 8, 0., 8., 8, 0., 8.);

    // Event building
    fhNumOfHitInEvent = new TH1D("numOfHitInEvent", "Number of hits in event;N;Entries", 150, 0., 150.);

    // Triggered event building
    fhAuxPeriod = new TH1D("fhAuxPeriod", "fhAuxPeriod;ns;Entries", 1000, 0., 5000000.);

    fhTriggerCorrelation = new TH1D("fhTriggerCorrelation", "fhTriggerCorrelation;ns;Entries", 3000, 500., 3500.);
    fhTriggerCorrelationInEvent = new TH1D("fhTriggerCorrelationInEvent", "fhTriggerCorrelationInEvent;ns;Entries", 3000, 500., 3500.);
    fhTriggerCorrelationLarge = new TH1D("fhTriggerCorrelationLarge", "fhTriggerCorrelationLarge;ns;Entries", 2000, 0., 200000.);
    fhTriggerCorrelationInNoiseWin = new TH1D("fhTriggerCorrelationInNoiseWin", "fhTriggerCorrelationInNoiseWin;ns;Entries", 2000, 0., 200000.);

    fhNumOfHitInTriggeredEvent = new TH1D("numOfHitInTriggeredEvent", "Number of hits in triggered event;N;Entries", 150, 0., 150.);
    fhHitsPerChannelInTriggeredEvent = new TH1D("hitsPerChannelInTriggeredEvent", "Number of hits per channel in the triggered event;Channel;Entries", 128, 0., 128.);
    fhAdcInTriggeredEvent = new TH2D("adcInTriggeredEvent", "ADC spectra in triggered events;channel;ADC value", 128, 0., 128., 1024, 0., 4096.);
    fhAdcInTriggeredEventWoBaseline = new TH2D("adcInTriggeredEventWoBaseline", "ADC spectra in triggered events without baseline;channel;ADC value",
                                            128, 0., 128., 1074*2, -200., 4096.);
    fhAdcSumPerTriggeredEvent = new TH1D("adcSumPerTriggeredEvent", "adcSumPerTriggeredEvent", 1074*5, -2000., 40960.);

    fhHitsPerChannelInNoiseEvent = new TH1D("hitsPerChannelInNoiseEvent", "Number of hits per channel in noise win;Channel;Entries", 128, 0., 128.);
    fhAdcInNoiseEvent = new TH2D("adcInNoiseEvent", "ADC spectra in noise events;channel;ADC value", 128, 0., 128., 1024, 0., 4096.);
    fhAdcInNoiseEventWoBaseline = new TH2D("adcInNoiseEventWoBaseline", "ADC spectra in noise events without baseline;channel;ADC value",
                                            128, 0., 128., 1074*2, -200., 4096.);
    fhAdcSumPerNoiseEvent = new TH1D("adcSumPerNoiseEvent", "adcSumPerNoiseEvent", 1074*5, -2000., 40960.);

}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::DeleteHistos(void)
{
    // Raw data analysis
    delete fhAdcAll;
    delete fhAdcAllWoBaseline;
    delete fhAdcAllSumWoBaseline;
    delete fhHitsPerChannel;

    // Callibrated data analysis
    delete fhCalAdcAllWoBaseline1e;
    delete fhCalAdcAllSumWoBaseline;

    delete fhAdcAllWoBaselineNonLinear;
    delete fhAdcAllSumWoBaselineNonLinear;

    delete fhAdcAllWoBaselineNLcorr;
    delete fhAdcAllSumWoBaselineNLcorr;

//    delete fhCalAdcAllWoBaselineNonLinear;
//    delete fhAdcAllSumWoBaselineNonLinear;

    // Processed data analysis
    delete fhAdcInEvent;
    delete fhAdcBaseline;
    delete fhAdcInEventWoBaseline;
    delete fhAdcSumPerEvent;
    delete fhAdcSumPerEventSingleHitEvents;
    // Timing
    delete fhMinusFirstInTime;
    delete fhDistanceBetwEvents;

    // Heat map
    delete fhHeatMap;

    // Event building
    delete fhNumOfHitInEvent;

    // Triggered event building
    delete fhAuxPeriod;

    delete fhTriggerCorrelation;
    delete fhTriggerCorrelationInEvent;
    delete fhTriggerCorrelationLarge;
    delete fhTriggerCorrelationInNoiseWin;

    delete fhNumOfHitInTriggeredEvent;
    delete fhHitsPerChannelInTriggeredEvent;
    delete fhAdcInTriggeredEvent;
    delete fhAdcInTriggeredEventWoBaseline;
    delete fhAdcSumPerTriggeredEvent;

    delete fhHitsPerChannelInNoiseEvent;
    delete fhAdcInNoiseEvent;
    delete fhAdcInNoiseEventWoBaseline;
    delete fhAdcSumPerNoiseEvent;
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
    fhHitsPerChannel->Write();

    // Callibrated data analysis
    fhCalAdcAllWoBaseline1e->Write();
    fhCalAdcAllSumWoBaseline->Write();

    //corrected data
    fhAdcAllWoBaselineNonLinear->Write();
    fhAdcAllSumWoBaselineNonLinear->Write();

    fhAdcAllWoBaselineNLcorr->Write();
    fhAdcAllSumWoBaselineNLcorr->Write();
//    fhCalAdcAllWoBaselineNonLinear->Write();
//    fhAdcAllSumWoBaselineNonLinear->Write();

    // Processed data analysis
    fhAdcInEvent->Write();
    fhAdcBaseline->Write();
    fhAdcInEventWoBaseline->Write();
    fhAdcSumPerEvent->Write();
    fhAdcSumPerEventSingleHitEvents->Write();
    // Timing
    fhMinusFirstInTime->Write();
    fhDistanceBetwEvents->Write();

    // Heat map
    fhHeatMap->Write();

    // Event building
    fhNumOfHitInEvent->Write();

    // Triggered event building
    gDirectory->mkdir("TrigEventBuilding");
    gDirectory->cd("TrigEventBuilding");
    fhAuxPeriod->Write();
    fhTriggerCorrelation->Write();
    fhTriggerCorrelationInEvent->Write();
    fhTriggerCorrelationLarge->Write();
    fhTriggerCorrelationInNoiseWin->Write();

    fhNumOfHitInTriggeredEvent->Write();
    fhHitsPerChannelInTriggeredEvent->Write();
    fhAdcInTriggeredEvent->Write();
    fhAdcInTriggeredEventWoBaseline->Write();
    fhAdcSumPerTriggeredEvent->Write();

    fhHitsPerChannelInNoiseEvent->Write();
    fhAdcInNoiseEvent->Write();
    fhAdcInNoiseEventWoBaseline->Write();
    fhAdcSumPerNoiseEvent->Write();
    gDirectory->cd("..");

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
    cls_HistoWidget* v_histoW = new cls_HistoWidget(fhAdcSumPerEvent);
    v_histoW->show();


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

    //TODO - make it nice
#ifdef DO_CROSSTALK
    if (fCrossTalkAnalyser) fCrossTalkAnalyser->fPedestals = fPedestals;
#endif
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::ImportPixelMap(QString p_filename)
{
    if (!fPixelMap) fPixelMap = new cls_pixelMap();
    TString v_filename;
    v_filename.Form("%s", p_filename.toStdString().data());
    fPixelMap->Import(v_filename);

    //TODO - make it nice
#ifdef DO_CROSSTALK
    if (fCrossTalkAnalyser) fCrossTalkAnalyser->fPixelMap = fPixelMap;
#endif
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::ImportEffCalib(QString p_filename)
{
    if (p_filename.toStdString().length()==0) {
        Warning("cls_LmdFile::ImportEffCalib", "File with 1e calibration parameters was not set.");
        return;
    }

    Info("cls_LmdFile::ImportEffCalib", "Importing calibration parameters from file %s.", p_filename.toStdString().data());

    std::ifstream infile(p_filename.toStdString());

    int ch;
    float val;

    //TODO by now only 64
    for (unsigned int i=0; i<64; i++) {
        infile >> ch >> val;
        fEffCalib[ch] = val/f1ePosCorrection;
    }
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::ImportGraphsFile(QString p_filename)
{
    if (p_filename.toStdString().length()==0) {
        Warning("cls_LmdFile::ImportGraphsFile", "File with graphs for non-linearity correction was not set.");
        return;
    }

    Info("cls_LmdFile::ImportGraphsFile", "Importing graphs for non-linearity correction from file %s.", p_filename.toStdString().data());

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
    this->RunTriggeredEventBuilding();
    this->PrintDebugInfo();

    if (mOutputTreeFilename.Length() != 0) {
        this->ExportEventsRootTree();
    } else {
        Warning("cls_LmdFile::StartProcessing", "Output tree filename was not set; file wasn't created.");
    }

    if (mOutputHistoFilename.Length() != 0) {
        this->RunEventsAnalysis();
        this->ExportHistos();
    } else {
        Warning("cls_LmdFile::StartProcessing", "Output histograms filename was not set; file wasn't created.");
    }

#ifdef DO_CROSSTALK
    this->fCrossTalkAnalyser->ExportHistos(mOutputCrossTalkFilename);
#endif
    if (fShowHistograms) {
        this->ShowHistos();
    }

    Info("cls_LmdFile::StartProcessing", "Processing succesfully finished.");
}

// =============================================================================================================================
// =============================================================================================================================

void cls_LmdFile::PrintDebugInfo(void)
{
    cout << "Total " << fNauxUnpacked << " AUX messages unpacked." << endl;
    cout << "Total " << fNsyncUnpacked << " SYNC messages unpacked." << endl;
    cout << "Total " << fNhitsUnpacked << " HIT messages unpacked." << endl;
    cout << "Total " << fNhitsPushedIntoEvents << " hits pushed into events." << endl;
    cout << "Mismatch = " << fNhitsUnpacked-fNhitsPushedIntoEvents << endl;
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

    cout << "Unpacking..." << endl;

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
             Double_t correctedVal;
             Double_t correctedValPed;
             Double_t pedMinusAdcVal;
//             Float_t effCalibratedADC;
//             Float_t correctedVal;
//             Float_t correctedValPed;
//             Float_t pedMinusAdcVal;

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
//                if (mOutputHistoFilename.Length() != 0) { continue; }
                // Raw
                fhAdcAll->Fill(ch, adc);
                pedMinusAdcVal = fPedestals[ch] - adc;
                fhAdcAllWoBaseline->Fill(ch, pedMinusAdcVal);
                fhAdcAllSumWoBaseline->Fill(pedMinusAdcVal);
                fhHitsPerChannel->Fill(ch);

                // corrected using LUTs
                correctedVal = cls_Calibrator::Instance().GetCalibratedVal(ch, pedMinusAdcVal);
//                correctedVal = cls_Calibrator::Instance().GetCalibratedVal(ch, pedMinusAdcVal+calRandom.Uniform(-0.5, 0.5));
                fhAdcAllWoBaselineNonLinear->Fill(ch, correctedVal);
                fhAdcAllSumWoBaselineNonLinear->Fill(correctedVal);

                // corrected with regard to NL pedestals
                correctedValPed = correctedVal - fPedestalsCorrection[ch];
                fhAdcAllWoBaselineNLcorr->Fill(ch, correctedValPed);
                fhAdcAllSumWoBaselineNLcorr->Fill(correctedValPed);

                // Eff calibrated
//                effCalibratedADC = (correctedValPed)/fEffCalib[ch];
                effCalibratedADC = (correctedValPed+calRandom.Uniform(-0.5, 0.5))/fEffCalib[ch];
                fhCalAdcAllWoBaseline1e->Fill(ch, effCalibratedADC);
                fhCalAdcAllSumWoBaseline->Fill(effCalibratedADC);

                //we calculate corrected calibration coefficient each time but it may be performed only one time for all events
//                correctedCalCoef = cls_Calibrator::Instance().GetCalibratedVal(ch, fEffCalib[ch]);
//                fhCalAdcAllWoBaselineNonLinear->Fill(ch, correctedVal/correctedCalCoef);
//                fhAdcAllSumWoBaselineNonLinear->Fill(correctedVal/correctedCalCoef);

                curChAdcPair = std::make_pair(ch, adc);
                fTimeAdcMap.insert(std::pair<uint64_t, std::pair<uint8_t, uint16_t> >(hitFullTime, curChAdcPair));

                fNhitsUnpacked++;
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

                fNsyncUnpacked++;

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

                fTimeAuxMap.insert(std::pair<uint64_t, uint8_t>(hitFullTime, ch));

                fNauxUnpacked++;

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
    cout << "Building events..." << endl;

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
            fNhitsPushedIntoEvents++;
            v_lastTimestamp = v_currentTimestamp;

        } else {
            // The current hit is not so far from the last hit
            v_curEvent.AddHit(v_currentTimestamp, v_currentHit.first, v_currentHit.second);
            fNhitsPushedIntoEvents++;
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

void cls_LmdFile::RunTriggeredEventBuilding(void)
{
    cout << "Building events around triggers..." << endl;

    // These are defines for the event building algorithms
    uint64_t leftHistoWinSize = -500;
    uint64_t rightHistoWinSize = 3500;

    uint64_t leftHistoLargeWinSize = 0;
    uint64_t rightHistoLargeWinSize = 200000;

    uint64_t leftWinSize = -880;   // -940
    uint64_t rightWinSize = 980;  //  1010

    uint64_t leftNoiseWinSize = -40000;
    uint64_t rightNoiseWinSize= 140000;


    UInt_t v_auxCounter = 0;
    uint64_t v_lastAuxTs = 0;

    std::multimap< uint64_t, std::pair<uint8_t, uint16_t> >::iterator v_lastHitsIter;

    std::multimap< uint64_t, uint8_t >::iterator v_TimeAuxMapIter;

    // Run 1/4

    v_lastHitsIter = fTimeAdcMap.begin();
    // Loop over events
    for (v_TimeAuxMapIter=fTimeAuxMap.begin(); v_TimeAuxMapIter!=fTimeAuxMap.end(); ++v_TimeAuxMapIter)
    {
        uint64_t v_currentAuxTimestamp = (*v_TimeAuxMapIter).first;
        std::multimap< uint64_t, std::pair<uint8_t, uint16_t> >::iterator v_hitsIter;
        for (v_hitsIter=v_lastHitsIter; v_hitsIter!=fTimeAdcMap.begin(); --v_hitsIter) {
            uint64_t v_currentHitTimestamp = (*v_hitsIter).first;
            // As soon as we get one step left from the left window boundary - stop searching
            if (v_currentHitTimestamp < v_currentAuxTimestamp-leftHistoLargeWinSize) {
                break;
            }
        }
        // after this loop the v_hitsIter iterator is before the trigger window
        for (/*no action here*/; v_hitsIter!=fTimeAdcMap.end(); ++v_hitsIter) {
            uint64_t v_currentHitTimestamp = (*v_hitsIter).first;
            if (v_currentHitTimestamp < v_currentAuxTimestamp-leftHistoLargeWinSize) {   // before the window - skip
                continue;
            }
            if (v_currentHitTimestamp > v_currentAuxTimestamp+rightHistoLargeWinSize) {   // after the window - skip
                // store the first unused hit in the map after the right window bound
                v_lastHitsIter = v_hitsIter;
                break;
            }
            // in the window - account
            fhTriggerCorrelationLarge->Fill(v_currentHitTimestamp-v_currentAuxTimestamp);
        }

        // Get the laser period
        if (v_auxCounter>0) {
            uint64_t lastAuxTimeDiff = v_currentAuxTimestamp - v_lastAuxTs;
            fhAuxPeriod->Fill(lastAuxTimeDiff);
        }
        v_lastAuxTs = v_currentAuxTimestamp;
        v_auxCounter++;
    }

    // Run 2/4

    v_lastHitsIter = fTimeAdcMap.begin();
    // Loop over events
    for (v_TimeAuxMapIter=fTimeAuxMap.begin(); v_TimeAuxMapIter!=fTimeAuxMap.end(); ++v_TimeAuxMapIter)
    {
        uint64_t v_currentAuxTimestamp = (*v_TimeAuxMapIter).first;
        std::multimap< uint64_t, std::pair<uint8_t, uint16_t> >::iterator v_hitsIter;
        for (v_hitsIter=v_lastHitsIter; v_hitsIter!=fTimeAdcMap.begin(); --v_hitsIter) {
            uint64_t v_currentHitTimestamp = (*v_hitsIter).first;
            // As soon as we get one step left from the left window boundary - stop searching
            if (v_currentHitTimestamp < v_currentAuxTimestamp-leftHistoWinSize) {
                break;
            }
        }
        // after this loop the v_hitsIter iterator is before the trigger window
        for (/*no action here*/; v_hitsIter!=fTimeAdcMap.end(); ++v_hitsIter) {
            uint64_t v_currentHitTimestamp = (*v_hitsIter).first;
            if (v_currentHitTimestamp < v_currentAuxTimestamp-leftHistoWinSize) {   // before the window - skip
                continue;
            }
            if (v_currentHitTimestamp > v_currentAuxTimestamp+rightHistoWinSize) {   // after the window - skip
                // store the first unused hit in the map after the right window bound
                v_lastHitsIter = v_hitsIter;
                break;
            }
            // in the window - account
            fhTriggerCorrelation->Fill(v_currentHitTimestamp-v_currentAuxTimestamp);
        }
    }

    // Run 3/4

    v_lastHitsIter = fTimeAdcMap.begin();
    // Loop over events
    for (v_TimeAuxMapIter=fTimeAuxMap.begin(); v_TimeAuxMapIter!=fTimeAuxMap.end(); ++v_TimeAuxMapIter)
    {
        UInt_t v_counterOfHitsInEvent=0;

        uint64_t v_currentAuxTimestamp = (*v_TimeAuxMapIter).first;
        std::multimap< uint64_t, std::pair<uint8_t, uint16_t> >::iterator v_hitsIter;
        for (v_hitsIter=v_lastHitsIter; v_hitsIter!=fTimeAdcMap.begin(); --v_hitsIter) {
            uint64_t v_currentHitTimestamp = (*v_hitsIter).first;
            // As soon as we get one step left from the left window boundary - stop searching
            if (v_currentHitTimestamp < v_currentAuxTimestamp-leftWinSize) {
                break;
            }
        }
        // after this loop the v_hitsIter iterator is before the trigger window
        for (/*no action here*/; v_hitsIter!=fTimeAdcMap.end(); ++v_hitsIter) {
            uint64_t v_currentHitTimestamp = (*v_hitsIter).first;
            if (v_currentHitTimestamp < v_currentAuxTimestamp-leftWinSize) {   // before the window - skip
                continue;
            }
            if (v_currentHitTimestamp > v_currentAuxTimestamp+rightWinSize) {   // after the window - skip
                // store the first unused hit in the map after the right window bound
                v_lastHitsIter = v_hitsIter;
                break;
            }
            // in the window - account
            fhTriggerCorrelationInEvent->Fill(v_currentHitTimestamp-v_currentAuxTimestamp);
            v_counterOfHitsInEvent++;

            uint8_t v_curHitChannel = (*v_hitsIter).second.first;
            uint16_t v_curHitAdc = (*v_hitsIter).second.second;

            // Extract REAL ADC value by subtracting the pedestal
            //TODO check data types and casting...
            double v_realADCval = (double)fPedestals[v_curHitChannel] - (double)v_curHitAdc;

            fhAdcInTriggeredEvent->Fill(v_curHitChannel, v_curHitAdc);
            fhAdcInTriggeredEventWoBaseline->Fill(v_curHitChannel, v_realADCval);
            fhAdcSumPerTriggeredEvent->Fill(v_realADCval);
            fhHitsPerChannelInTriggeredEvent->Fill(v_curHitChannel);
        }
        fhNumOfHitInTriggeredEvent->Fill(v_counterOfHitsInEvent);
    }

    // Run 4/4

    v_lastHitsIter = fTimeAdcMap.begin();
    // Loop over events
    for (v_TimeAuxMapIter=fTimeAuxMap.begin(); v_TimeAuxMapIter!=fTimeAuxMap.end(); ++v_TimeAuxMapIter)
    {
        uint64_t v_currentAuxTimestamp = (*v_TimeAuxMapIter).first;
        std::multimap< uint64_t, std::pair<uint8_t, uint16_t> >::iterator v_hitsIter;
        for (v_hitsIter=v_lastHitsIter; v_hitsIter!=fTimeAdcMap.begin(); --v_hitsIter) {
            uint64_t v_currentHitTimestamp = (*v_hitsIter).first;
            // As soon as we get one step left from the left window boundary - stop searching
            if (v_currentHitTimestamp < v_currentAuxTimestamp-leftNoiseWinSize) {
                break;
            }
        }
        // after this loop the v_hitsIter iterator is before the trigger window
        for (/*no action here*/; v_hitsIter!=fTimeAdcMap.end(); ++v_hitsIter) {
            uint64_t v_currentHitTimestamp = (*v_hitsIter).first;
            if (v_currentHitTimestamp < v_currentAuxTimestamp-leftNoiseWinSize) {   // before the window - skip
                continue;
            }
            if (v_currentHitTimestamp > v_currentAuxTimestamp+rightNoiseWinSize) {   // after the window - skip
                // store the first unused hit in the map after the right window bound
                v_lastHitsIter = v_hitsIter;
                break;
            }
            // in the window - account
            fhTriggerCorrelationInNoiseWin->Fill(v_currentHitTimestamp-v_currentAuxTimestamp);

            uint8_t v_curHitChannel = (*v_hitsIter).second.first;
            uint16_t v_curHitAdc = (*v_hitsIter).second.second;

            // Extract REAL ADC value by subtracting the pedestal
            //TODO check data types and casting...
            double v_realADCval = (double)fPedestals[v_curHitChannel] - (double)v_curHitAdc;

            fhAdcInNoiseEvent->Fill(v_curHitChannel, v_curHitAdc);
            fhAdcInNoiseEventWoBaseline->Fill(v_curHitChannel, v_realADCval);
            fhAdcSumPerNoiseEvent->Fill(v_realADCval);
            fhHitsPerChannelInNoiseEvent->Fill(v_curHitChannel);
        }
    }
}

// =============================================================================================================================
// =============================================================================================================================

unsigned int cls_LmdFile::ExportEventsRootTree()
{
    TFile theTreeFile(mOutputTreeFilename, "RECREATE");
    TTree theTree("theTree", "A tree with events");

    cls_RootEvent* v_event = new cls_RootEvent();

    theTree.Branch("theBranch.", "cls_RootEvent", v_event);

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

            //calibration to 1e units

            // corrected using LUTs
//            Float_t correctedVal = cls_Calibrator::Instance().GetCalibratedVal(v_curHitChannel, v_realADCval);

            // corrected with regard to NL pedestals
//            Float_t correctedValPed = correctedVal - fPedestalsCorrection[v_curHitChannel];

            // Eff calibrated
//            Float_t calibratedAdc = (correctedValPed+calRandom.Uniform(-0.5, 0.5))/fEffCalib[v_curHitChannel];
            Float_t calibratedAdc = 0;
//            Float_t calibratedAdc = cls_Calibrator::Instance().GetCalibratedVal(v_curHitChannel, v_realADCval);
//            effCalibratedADC = (correctedValPed+calRandom.Uniform(-0.5, 0.5))/fEffCalib[ch];
//            fhCalAdcAllWoBaseline1e->Fill(ch, effCalibratedADC);
//            fhCalAdcAllSumWoBaseline->Fill(effCalibratedADC);


//

            v_event->AddHit(v_curHitTime, v_curHitChannel, v_curHitAdc, v_realADCval, calibratedAdc);

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
    cout << "Performing events analysis..." << endl;

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


        // Fill heat map
        // Loop over the hits of the event
        for (v_eventHitsIter = v_curEvent.fEventTimeAdcMap.begin(); v_eventHitsIter != v_curEvent.fEventTimeAdcMap.end(); ++v_eventHitsIter) {
            // Extract channel number from the hit
            uint8_t v_curHitChannel = (*v_eventHitsIter).second.first;
            UInt_t v_row;
            UInt_t v_col;

            if (fPixelMap->GetPixelCoords(v_curHitChannel, &v_row, &v_col) == 0)  {
                //cout << "Error " << "ch=" << (UInt_t)(v_curHitChannel) << "\trow=" << v_row << "\tcol=" << v_col << endl;
            } else {
                fhHeatMap->Fill(v_col, 7-v_row);
            }
        }

        // Cross-talk analysis
        // You may comment this section if you don't need it
#ifdef DO_CROSSTALK
        fCrossTalkAnalyser->ProcessEvent(v_eventsIter);
#endif


    } // End of loop over events

}

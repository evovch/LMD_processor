#ifndef CLS_LMDFILE_H
#define CLS_LMDFILE_H

#include <map>
#include <QString>
#include "cls_Event.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TError.h>

class cls_pixelMap;
class cls_CrossTalkAnalyser;

class cls_LmdFile
{
public:
    cls_LmdFile();
    ~cls_LmdFile();

    void ImportPixelMap(QString p_filename);
    void ImportPedestals(QString p_filename);
    void ImportEffCalib(QString p_filename);
    void ImportGraphsFile(QString p_filename);
    void StartProcessing(QString p_filename);

    void SetOutputHistoFile(QString p_filename) { mOutputHistoFilename = p_filename.toStdString(); }
    void SetOutputTreeFile(QString p_filename) { mOutputTreeFilename = p_filename.toStdString(); }
    void SetOutputCrossTalkFile(QString p_filename) { mOutputCrossTalkFilename = p_filename.toStdString(); }
    void SetShowHistograms(Bool_t p_value) { fShowHistograms = p_value; }

private: // methods

    void RunUnpacking(void);
    void RunEventBuilding(void);
    void RunTriggeredEventBuilding(void);
    void RunEventsAnalysis(void);

    void InitHistos(void);
    void DeleteHistos(void);
    unsigned int ExportHistos(void);

    unsigned int ExportEventsRootTree(void);

    void ShowHistos(void);

    void PrintDebugInfo(void);

private: // data members

    // Input LMD file name
    TString mFilename;

    // Output analysis histograms root file name
    TString mOutputHistoFilename;

    // Output tree root file name
    TString mOutputTreeFilename;

    // Output root file for cross-talk analysis
    TString mOutputCrossTalkFilename;

    // Storage and cursor for the raw data
    char* mRawData;
    std::streampos mSize;

    // Two containders for the unpacking stage ------------------------------------------
    // buffer for hits
    // pair full time - channel:adc
    std::multimap< uint64_t, std::pair<uint8_t, uint16_t> > fTimeAdcMap;
    // buffer for aux messages
    // pair full time - aux channel
    std::multimap< uint64_t, uint8_t > fTimeAuxMap;
    // ----------------------------------------------------------------------------------

    // Debug counters for the event building
    UInt_t fNhitsUnpacked;
    UInt_t fNauxUnpacked;
    UInt_t fNsyncUnpacked;
    UInt_t fNhitsPushedIntoEvents;

    // Container for the data after event building
    std::vector<cls_Event> fEvents;

    cls_pixelMap* fPixelMap;

    cls_CrossTalkAnalyser* fCrossTalkAnalyser;

    float fPedestals[128];
    float fEffCalib[128];

    Bool_t fShowHistograms;

private: // histos - move to another class

    // Raw data analysis
    TH2D* fhAdcAll;
    TH2D* fhAdcAllWoBaseline;
    TH1D* fhAdcAllSumWoBaseline;
    TH1D* fhHitsPerChannel;

    // Callibrated data analysis
    TH2D* fhCalAdcAllWoBaseline1e;
    TH1D* fhCalAdcAllSumWoBaseline;
//    TH2D* fhCalAdcAllSumWoBaseline

    // With calibration using LUTs
    TH2D* fhCalAdcAllWoBaselineNonLinear;
    TH1D* fhCalAdcAllSumWoBaselineNonLinear;

    // Processed data analysis
    TH2D* fhAdcInEvent;
    TH2D* fhAdcBaseline;
    TH2D* fhAdcInEventWoBaseline;
    TH1D* fhAdcSumPerEvent;
    TH1D* fhAdcSumPerEventSingleHitEvents;
    // Timing
    TH1D* fhMinusFirstInTime;
    TH1D* fhDistanceBetwEvents;

    // Heat map
    TH2D* fhHeatMap;

    // Event building
    TH1D* fhNumOfHitInEvent;

    // Triggered event building
    TH1D* fhAuxPeriod;
    TH1D* fhTriggerCorrelation;
    TH1D* fhTriggerCorrelationInEvent;
    TH1D* fhTriggerCorrelationLarge;
    TH1D* fhTriggerCorrelationInNoiseWin;

    TH1D* fhNumOfHitInTriggeredEvent;
    TH1D* fhHitsPerChannelInTriggeredEvent;
    TH2D* fhAdcInTriggeredEvent;
    TH2D* fhAdcInTriggeredEventWoBaseline;
    TH1D* fhAdcSumPerTriggeredEvent;

    TH1D* fhHitsPerChannelInNoiseEvent;
    TH2D* fhAdcInNoiseEvent;
    TH2D* fhAdcInNoiseEventWoBaseline;
    TH1D* fhAdcSumPerNoiseEvent;

};

#endif // CLS_LMDFILE_H

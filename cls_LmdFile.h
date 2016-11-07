#ifndef CLS_LMDFILE_H
#define CLS_LMDFILE_H

#include <map>
#include <QString>
#include "cls_Event.h"

#include <TH1D.h>
#include <TH2D.h>

class cls_LmdFile
{
public:
    cls_LmdFile();
    ~cls_LmdFile();

    void ImportPedestals(QString p_filename);
    void ImportEffCalib(QString p_filename);
    void ImportGraphsFile(QString p_filename);
    void StartProcessing(QString p_filename);

    void SetOutputHistoFile(QString p_filename) { mOutputHistoFilename = p_filename.toStdString(); }
    void SetOutputTreeFile(QString p_filename) { mOutputTreeFilename = p_filename.toStdString(); }

private: // methods

    void RunUnpacking(void);
    void RunEventBuilding(void);
    void RunEventsAnalysis(void);

    void InitHistos(void);
    void DeleteHistos(void);
    unsigned int ExportHistos(void);

    unsigned int ExportEventsRootTree(void);

    void ShowHistos(void);

private: // data members

    // Input LMD file name
    TString mFilename;

    // Output analysis histograms root file name
    TString mOutputHistoFilename;

    // Output tree root file name
    TString mOutputTreeFilename;

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

    // Container for the data after event building
    std::vector<cls_Event> fEvents;

    float fPedestals[128];
    float fEffCalib[128];

private: // histos - move to another class

    // Raw data analysis
    TH2D* fhAdcAll;
    TH2D* fhAdcAllWoBaseline;
    TH1D* fhAdcAllSumWoBaseline;

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

    // Event building
    TH1D* fhNumOfHitInEvent;

};

#endif // CLS_LMDFILE_H

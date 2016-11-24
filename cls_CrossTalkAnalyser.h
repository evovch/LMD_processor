#ifndef CLS_CROSSTALKANALYSER_H
#define CLS_CROSSTALKANALYSER_H

#include <vector>

#include <TH2D.h>
#include <TString.h>

/*#include <QString>*/

#include "cls_Event.h"

class cls_pixelMap;

class cls_CrossTalkAnalyser
{
public:
    cls_CrossTalkAnalyser();
    ~cls_CrossTalkAnalyser();

    void InitHistos(void);
    void DeleteHistos(void);

    unsigned int ExportHistos(TString p_filename);
    /*unsigned int ExportHistos(QString p_qfilename);*/

    void ProcessEvent(std::vector<cls_Event>::iterator p_eventsIter);

    float* fPedestals;
    cls_pixelMap* fPixelMap;

private:
    TH2D* fhAdcInEventNoNeighbours[64];
    TH2D* fhAdcInEventWithTopNeighbour[64];
    TH2D* fhAdcInEventWithBottomNeighbour[64];
    TH2D* fhAdcInEventWithLeftNeighbour[64];
    TH2D* fhAdcInEventWithRightNeighbour[64];
    TH2D* fhAdcInEventWithAtLeastOneNeighbour[64];
    TH2D* fhAdcInEventWoBaselineNoNeighbours[64];
    TH2D* fhAdcInEventWoBaselineWithTopNeighbour[64];
    TH2D* fhAdcInEventWoBaselineWithBottomNeighbour[64];
    TH2D* fhAdcInEventWoBaselineWithLeftNeighbour[64];
    TH2D* fhAdcInEventWoBaselineWithRightNeighbour[64];
    TH2D* fhAdcInEventWoBaselineWithAtLeastOneNeighbour[64];

};

#endif // CLS_CROSSTALKANALYSER_H

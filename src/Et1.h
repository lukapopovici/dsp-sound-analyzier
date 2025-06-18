/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: OnPanelCallback */
#define  PANEL_LOADBUTTON                 2       /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_COMMANDBUTTON_2            3       /* control type: command, callback function: OnPrevButtonCB */
#define  PANEL_COMMANDBUTTON              4       /* control type: command, callback function: OnNextButtonCB */
#define  PANEL_GRAPH_2                    5       /* control type: graph, callback function: (none) */
#define  PANEL_GRAPH                      6       /* control type: graph, callback function: (none) */
#define  PANEL_COMMANDBUTTON_3            7       /* control type: command, callback function: OnApplyButtonCB */
#define  PANEL_STRING_2                   8       /* control type: numeric, callback function: (none) */
#define  PANEL_STRING                     9       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC                    10      /* control type: numeric, callback function: (none) */
#define  PANEL_RING                       11      /* control type: ring, callback function: OnRingChangeCB */
#define  PANEL_BINARYSWITCH               12      /* control type: binary, callback function: OnBinarySwitchCB */
#define  PANEL_ALPHA                      13      /* control type: numeric, callback function: (none) */
#define  PANEL_STRING_3                   14      /* control type: numeric, callback function: (none) */
#define  PANEL_BINARYSWITCH_2             15      /* control type: binary, callback function: (none) */
#define  PANEL_COMMANDBUTTON_4            16      /* control type: command, callback function: OnSavePlotsButtonCB */
#define  PANEL_COMMANDBUTTON_5            17      /* control type: command, callback function: OnSaveAnalysisCB */
#define  PANEL_SKEW                       18      /* control type: numeric, callback function: (none) */
#define  PANEL_KURT                       19      /* control type: numeric, callback function: (none) */
#define  PANEL_COMMANDBUTTON_6            20      /* control type: command, callback function: OnSwitchPanelCB */

#define  WAVE_PANEL                       2       /* callback function: OnPanelCallback2 */
#define  WAVE_PANEL_TIMER                 2       /* control type: timer, callback function: FreqTmr */
#define  WAVE_PANEL_POWER_PEAK            3       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_FREQUENCY_PEAK        4       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_NUMERIC               5       /* control type: scale, callback function: (none) */
#define  WAVE_PANEL_GRAPHBLACKMAN         6       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_GRAPHHANNING          7       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_FSW                   8       /* control type: binary, callback function: (none) */
#define  WAVE_PANEL_PHASE_GRAPH           9       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_COMMANDBUTTON         10      /* control type: command, callback function: OnSwitchPanelCB */
#define  WAVE_PANEL_COMMANDBUTTON_2       11      /* control type: command, callback function: SAVE */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK FreqTmr(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnApplyButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnBinarySwitchCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNextButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelCallback2(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPrevButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnRingChangeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSaveAnalysisCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSavePlotsButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SAVE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
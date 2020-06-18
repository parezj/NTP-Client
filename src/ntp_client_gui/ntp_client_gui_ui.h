
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//
//  "ntp_client_gui_ui.h"
//
//  Declaration and include file for UI build functions
//

//  Date: 06-02-2020
//

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#ifndef _NTP_CLIENT_GUI_UI_INCLUDED
#define _NTP_CLIENT_GUI_UI_INCLUDED

#ifdef __cplusplus
    extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

//----------------------------------------------------------------------------------------------------
//  Include required headers
//----------------------------------------------------------------------------------------------------

#include <cvidef.h> // definition of CVICALLBACK

//----------------------------------------------------------------------------------------------------
//  Prototype UI build functions.
//----------------------------------------------------------------------------------------------------

int MainPANEL (int hParentPanel);

//-------------------------------------------------------------------------------------------------
// Prototype callbacks for Panel: PANEL
// ------------------------------------------------------------------------------------------------

// Panel
int CVICALLBACK panelCB (int panelHandle, int event, void *callbackData, int eventData1, int eventData2);

// Control: PANEL_CTRL_ENABLE
int CVICALLBACK callEnable (int panelHandle, int control, int event, void *callbackData, int eventData1, int eventData2);

//-------------------------------------------------------------------------------------------------
// Declare identifiers for Panel:  PANEL
// ------------------------------------------------------------------------------------------------

extern int PANEL_CTRL_IP                            ;     // control identifier
extern int PANEL_CTRL_ENABLE                        ;     // control identifier
extern int PANEL_DELAYLBL                           ;     // control identifier
extern int PANEL_JITTERLBL                          ;     // control identifier
extern int PANEL_OFFSETLBL                          ;     // control identifier
extern int PANEL_DELTALBL                           ;     // control identifier
extern int PANEL_CTRL_JITTER                        ;     // control identifier
extern int PANEL_DECORATION                         ;     // control identifier
extern int PANEL_CTRL_DELTA                         ;     // control identifier
extern int PANEL_CTRL_INTERVAL                      ;     // control identifier
extern int PANEL_STATUSLBL                          ;     // control identifier
extern int PANEL_DECORATION_2                       ;     // control identifier
extern int PANEL_TEXTMSG                            ;     // control identifier
extern int PANEL_CTRL_TIME                          ;     // control identifier
extern int PANEL_TEXTMSG_2                          ;     // control identifier
extern int PANEL_TIMELBL                            ;     // control identifier
extern int PANEL_CTRL_OFFSET                        ;     // control identifier
extern int PANEL_CTRL_STATUS                        ;     // control identifier
extern int PANEL_CTRL_DELAY                         ;     // control identifier
extern int PANEL_STRIPCHART                         ;     // control identifier

#ifdef __cplusplus
    }
#endif

#endif // _NTP_CLIENT_GUI_UI_INCLUDED
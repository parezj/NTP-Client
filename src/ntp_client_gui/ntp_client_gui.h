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

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_CTRL_IP                    2       /* control type: string, callback function: (none) */
#define  PANEL_CTRL_ENABLE                3       /* control type: textButton, callback function: callEnable */
#define  PANEL_DELAYLBL                   4       /* control type: textMsg, callback function: (none) */
#define  PANEL_JITTERLBL                  5       /* control type: textMsg, callback function: (none) */
#define  PANEL_OFFSETLBL                  6       /* control type: textMsg, callback function: (none) */
#define  PANEL_DELTALBL                   7       /* control type: textMsg, callback function: (none) */
#define  PANEL_CTRL_JITTER                8       /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION                 9       /* control type: deco, callback function: (none) */
#define  PANEL_CTRL_DELTA                 10      /* control type: textMsg, callback function: (none) */
#define  PANEL_CTRL_INTERVAL              11      /* control type: numeric, callback function: (none) */
#define  PANEL_STATUSLBL                  12      /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION_2               13      /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG                    14      /* control type: textMsg, callback function: (none) */
#define  PANEL_CTRL_TIME                  15      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  16      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMELBL                    17      /* control type: textMsg, callback function: (none) */
#define  PANEL_CTRL_OFFSET                18      /* control type: textMsg, callback function: (none) */
#define  PANEL_CTRL_STATUS                19      /* control type: textMsg, callback function: (none) */
#define  PANEL_CTRL_DELAY                 20      /* control type: textMsg, callback function: (none) */
#define  PANEL_STRIPCHART                 21      /* control type: strip, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK callEnable(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
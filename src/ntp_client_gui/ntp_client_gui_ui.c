
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//
//  "ntp_client_gui_ui.c"
//
//  Implementation file for UI build functions
//
//  Date: 06-02-2020
//
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//  Include header files
//----------------------------------------------------------------------------------------------------

#include "ntp_client_gui_ui.h"
#include <Utility.h> // definition of NULL
#include "toolbox.h" // definition of errChk macro

extern int CVIFUNC_C GetUserBitmapFromText (char *imageText, int version, int *bitmapId); // needed if there are bitmaps
extern int CVIFUNC_C NewActiveXCtrlFromPersistence (int panel, const char *ctrlLabel,
                                                    int top, int left, char *defaultIID,
                                                    int binaryFormat, char *persistenceText,
                                                    HRESULT *activeXError); // needed if there are ActiveX controls

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

int PANEL_CTRL_IP                            ;     // control identifier
int PANEL_CTRL_ENABLE                        ;     // control identifier
int PANEL_DELAYLBL                           ;     // control identifier
int PANEL_JITTERLBL                          ;     // control identifier
int PANEL_OFFSETLBL                          ;     // control identifier
int PANEL_DELTALBL                           ;     // control identifier
int PANEL_CTRL_JITTER                        ;     // control identifier
int PANEL_DECORATION                         ;     // control identifier
int PANEL_CTRL_DELTA                         ;     // control identifier
int PANEL_CTRL_INTERVAL                      ;     // control identifier
int PANEL_STATUSLBL                          ;     // control identifier
int PANEL_DECORATION_2                       ;     // control identifier
int PANEL_TEXTMSG                            ;     // control identifier
int PANEL_CTRL_TIME                          ;     // control identifier
int PANEL_TEXTMSG_2                          ;     // control identifier
int PANEL_TIMELBL                            ;     // control identifier
int PANEL_CTRL_OFFSET                        ;     // control identifier
int PANEL_CTRL_STATUS                        ;     // control identifier
int PANEL_CTRL_DELAY                         ;     // control identifier
int PANEL_STRIPCHART                         ;     // control identifier

//----------------------------------------------------------------------------------------------------
// MainPANEL ():  Build UI object PANEL; return handle or standard UI error code.
//----------------------------------------------------------------------------------------------------
int MainPANEL (int hParentPanel)
{
    int hPanel;
    int error = 0;
    int bitmapId = 0; // needed if there are bitmaps 
    char *dataPtr = NULL; 
    int hMenubar;
    int screenHeight;
    int screenWidth;
    int resAdjustment;
    int monitorId;
    float SFv;
    float SFh;
    int hTabPanel; // needed if there are tab controls 
    ColorMapEntry colorRamp[255]; // needed if there are controls with color ramps

    // Create the panel
    errChk(hPanel = NewPanel (hParentPanel, "NTP Client", 24, 3, 508, 769));

    // Set the panel's attributes
    errChk(SetPanelAttribute (hPanel, ATTR_CONSTANT_NAME, "PANEL"));
    errChk(SetPanelAttribute (hPanel, ATTR_CALLBACK_FUNCTION_POINTER, panelCB));
//  errChk(SetPanelAttribute (hPanel, ATTR_DIMMED, 0));
    if (hParentPanel)
        errChk(SetPanelAttribute (hPanel, ATTR_FRAME_COLOR, 0XF0F0F0));
//  errChk(SetPanelAttribute (hPanel, ATTR_SCROLL_BARS, VAL_NO_SCROLL_BARS));
    errChk(SetPanelAttribute (hPanel, ATTR_SCROLL_BAR_COLOR, 0XF0F0F0));
//  errChk(SetPanelAttribute (hPanel, ATTR_HSCROLL_OFFSET, 0));
    errChk(SetPanelAttribute (hPanel, ATTR_BACKCOLOR, 0XF0F0F0));
//  errChk(SetPanelAttribute (hPanel, ATTR_TITLEBAR_VISIBLE, 1));
    if (hParentPanel)
        errChk(SetPanelAttribute (hPanel, ATTR_TITLEBAR_THICKNESS, 18));
    if (hParentPanel)
        errChk(SetPanelAttribute (hPanel, ATTR_TITLEBAR_STYLE, VAL_WINDOWS_STYLE));
    if (hParentPanel)
        errChk(SetPanelAttribute (hPanel, ATTR_SCROLL_BAR_STYLE, VAL_LAB_STYLE));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_COLOR, VAL_BLACK));
    if (hParentPanel)
        errChk(SetPanelAttribute (hPanel, ATTR_TITLE_BACKCOLOR, 0XF0F0F0));
    if (hParentPanel)
        errChk(SetPanelAttribute (hPanel, ATTR_FRAME_STYLE, VAL_BEVELLED_FRAME));
    errChk(SetPanelAttribute (hPanel, ATTR_SIZABLE, 0));
//  errChk(SetPanelAttribute (hPanel, ATTR_MOVABLE, 1));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_FONT, "VAL_DIALOG_META_FONT"));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_ITALIC, 0));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_UNDERLINE, 0));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_STRIKEOUT, 0));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_POINT_SIZE, 11));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_BOLD, 0));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_PARENT_SHARES_SHORTCUT_KEYS, 1));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_ACTIVATE_WHEN_CLICKED_ON, 1));
//  errChk(SetPanelAttribute (hPanel, ATTR_CAN_MINIMIZE, 1));
    errChk(SetPanelAttribute (hPanel, ATTR_CAN_MAXIMIZE, 0));
    errChk(SetPanelAttribute (hPanel, ATTR_MIN_HEIGHT_FOR_SCALING, 0));
    errChk(SetPanelAttribute (hPanel, ATTR_MIN_WIDTH_FOR_SCALING, 0));
//  if (!hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_FLOATING, VAL_FLOAT_NEVER));
//  errChk(SetPanelAttribute (hPanel, ATTR_CLOSE_ITEM_VISIBLE, 1));
//  errChk(SetPanelAttribute (hPanel, ATTR_SYSTEM_MENU_VISIBLE, 1));
//  if (hParentPanel)
    //  errChk(SetPanelAttribute (hPanel, ATTR_TITLE_SIZE_TO_FONT, 1));
//  errChk(SetPanelAttribute (hPanel, ATTR_HAS_TASKBAR_BUTTON, 0));
//  errChk(SetPanelAttribute (hPanel, ATTR_VSCROLL_OFFSET, 0));
    if (hParentPanel)
        errChk(SetPanelAttribute (hPanel, ATTR_FRAME_THICKNESS, 1));
// ****WARNING****  Not equipped to set panel's ATTR_MOUSE_CURSOR = VAL_DEFAULT_CURSOR

    // Build control: PANEL_CTRL_IP
    errChk(PANEL_CTRL_IP = NewCtrl (hPanel, CTRL_STRING_LS, "NTP Server IP:", 428, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_CONSTANT_NAME, "CTRL_IP"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_FRAME_COLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_CTRL_MODE, VAL_HOT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_WIDTH, 75));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_HEIGHT, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_BGCOLOR, 0XF0F0F0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_POINT_SIZE, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_BGCOLOR, VAL_WHITE));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_DFLT_VALUE, "195.113.144.201"));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_IP));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_MAX_ENTRY_LENGTH, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_NO_EDIT_TEXT, 0));
// ****WARNING****  Skipped ATTR_MASK_CHARACTER = 42
// ****WARNING****  Skipped ATTR_ENABLE_CHARACTER_MASKING = 0
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_WIDTH, 132));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_TOP, 410));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_LEFT, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_LABEL_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_HSCROLL_OFFSET, 0));
//  errChk(DSBindCtrl (hPanel, PANEL_CTRL_IP, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_CTRL_ENABLE
    errChk(PANEL_CTRL_ENABLE = NewCtrl (hPanel, CTRL_SQUARE_TEXT_BUTTON_LS, "Enable:", 472, 81));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_CONSTANT_NAME, "CTRL_ENABLE"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_VISIBLE, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_CALLBACK_FUNCTION_POINTER, callEnable));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_CTRL_MODE, VAL_HOT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_WIDTH, 39));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_HEIGHT, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_BGCOLOR, 0XF0F0F0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TEXT_POINT_SIZE, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TEXT_BOLD, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_DFLT_VALUE, 0));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_ENABLE));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_AUTO_SIZING, VAL_GROW_ONLY));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_ON_COLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_OFF_COLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_ON_TEXT, "STOP"));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_OFF_TEXT, "START"));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_HEIGHT, 28));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_WIDTH, 64));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_TOP, 455));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_LEFT, 81));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_LABEL_SIZE_TO_TEXT, 1));

    // Build control: PANEL_DELAYLBL
    errChk(PANEL_DELAYLBL = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 441, 479));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_CONSTANT_NAME, "DELAYLBL"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_POINT_SIZE, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_DFLT_VALUE, "Delay:"));
    errChk(DefaultCtrl (hPanel, PANEL_DELAYLBL));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_WIDTH, 44));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_DELAYLBL, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_JITTERLBL
    errChk(PANEL_JITTERLBL = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 468, 484));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_CONSTANT_NAME, "JITTERLBL"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_POINT_SIZE, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_DFLT_VALUE, "Jitter: "));
    errChk(DefaultCtrl (hPanel, PANEL_JITTERLBL));
//  errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_WIDTH, 43));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_JITTERLBL, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_OFFSETLBL
    errChk(PANEL_OFFSETLBL = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 441, 605));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_CONSTANT_NAME, "OFFSETLBL"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_POINT_SIZE, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_DFLT_VALUE, "Offset: "));
    errChk(DefaultCtrl (hPanel, PANEL_OFFSETLBL));
//  errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_WIDTH, 51));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_OFFSETLBL, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_DELTALBL
    errChk(PANEL_DELTALBL = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 468, 611));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_CONSTANT_NAME, "DELTALBL"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_POINT_SIZE, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_DFLT_VALUE, "Delta: "));
    errChk(DefaultCtrl (hPanel, PANEL_DELTALBL));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_WIDTH, 45));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_DELTALBL, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_CTRL_JITTER
    errChk(PANEL_CTRL_JITTER = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 469, 530));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_CONSTANT_NAME, "CTRL_JITTER"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_POINT_SIZE, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_BOLD, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_DFLT_VALUE, "0"));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_JITTER));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_WIDTH, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_CTRL_JITTER, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_DECORATION
    errChk(PANEL_DECORATION = NewCtrl (hPanel, CTRL_RECESSED_BOX_LS, "", 428, 469));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_CONSTANT_NAME, "DECORATION"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_FRAME_COLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_CTRL_MODE, VAL_INDICATOR));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_HEIGHT, 72));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_WIDTH, 284));

    // Build control: PANEL_CTRL_DELTA
    errChk(PANEL_CTRL_DELTA = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 469, 659));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_CONSTANT_NAME, "CTRL_DELTA"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_POINT_SIZE, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_BOLD, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_DFLT_VALUE, "0"));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_DELTA));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_WIDTH, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_CTRL_DELTA, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_CTRL_INTERVAL
    errChk(PANEL_CTRL_INTERVAL = NewCtrl (hPanel, CTRL_NUMERIC_LS, "Interval [ms]:", 472, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_CONSTANT_NAME, "CTRL_INTERVAL"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_VISIBLE, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_DATA_TYPE, VAL_INTEGER));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_FRAME_COLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_CTRL_MODE, VAL_HOT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_WIDTH, 63));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_HEIGHT, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_BGCOLOR, 0XF0F0F0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_POINT_SIZE, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_BGCOLOR, VAL_WHITE));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_MAX_VALUE, 2147483647));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_MIN_VALUE, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_INCR_VALUE, 500));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_FORMAT, VAL_DECIMAL_FORMAT));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_PRECISION, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_CHECK_RANGE, VAL_COERCE));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_PADDING, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_SHOW_RADIX, 0));
// ****WARNING****  Skipped ATTR_DISABLE_RADIX = False
// ****WARNING****  Skipped ATTR_INCDEC_WIDTH = 9
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_DFLT_VALUE, 100));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_INTERVAL));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_SHOW_INCDEC_ARROWS, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_NO_EDIT_TEXT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_WIDTH, 63));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_TOP, 455));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_LEFT, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_LABEL_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_CTRL_INTERVAL, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_STATUSLBL
    errChk(PANEL_STATUSLBL = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 471, 182));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_CONSTANT_NAME, "STATUSLBL"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_POINT_SIZE, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_DFLT_VALUE, "Status:"));
    errChk(DefaultCtrl (hPanel, PANEL_STATUSLBL));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_WIDTH, 49));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_STATUSLBL, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_DECORATION_2
    errChk(PANEL_DECORATION_2 = NewCtrl (hPanel, CTRL_RAISED_BOX_LS, "", 427, 153));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_CONSTANT_NAME, "DECORATION_2"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_FRAME_COLOR, 0XE5E5E5));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_CTRL_MODE, VAL_INDICATOR));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_HEIGHT, 74));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_WIDTH, 310));

    // Build control: PANEL_TEXTMSG
    errChk(PANEL_TEXTMSG = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 412, 155));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_CONSTANT_NAME, "TEXTMSG"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_DFLT_VALUE, "Main Display:"));
    errChk(DefaultCtrl (hPanel, PANEL_TEXTMSG));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_HEIGHT, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_WIDTH, 66));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_TEXTMSG, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_CTRL_TIME
    errChk(PANEL_CTRL_TIME = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 434, 234));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_CONSTANT_NAME, "CTRL_TIME"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_POINT_SIZE, 27));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_BOLD, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_DFLT_VALUE, "00:00:00.000"));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_TIME));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_HEIGHT, 33));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_WIDTH, 174));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_CTRL_TIME, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_TEXTMSG_2
    errChk(PANEL_TEXTMSG_2 = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 412, 472));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_CONSTANT_NAME, "TEXTMSG_2"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_DFLT_VALUE, "Metrics [ms]:"));
    errChk(DefaultCtrl (hPanel, PANEL_TEXTMSG_2));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_HEIGHT, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_WIDTH, 62));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_TEXTMSG_2, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_TIMELBL
    errChk(PANEL_TIMELBL = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 442, 162));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_CONSTANT_NAME, "TIMELBL"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_POINT_SIZE, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_DFLT_VALUE, "Sys Time:"));
    errChk(DefaultCtrl (hPanel, PANEL_TIMELBL));
//  errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_WIDTH, 69));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_TIMELBL, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_CTRL_OFFSET
    errChk(PANEL_CTRL_OFFSET = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 441, 659));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_CONSTANT_NAME, "CTRL_OFFSET"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_POINT_SIZE, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_BOLD, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_DFLT_VALUE, "0"));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_OFFSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_WIDTH, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_CTRL_OFFSET, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_CTRL_STATUS
    errChk(PANEL_CTRL_STATUS = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 471, 238));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_CONSTANT_NAME, "CTRL_STATUS"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_POINT_SIZE, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_BOLD, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_DFLT_VALUE, "Disabled"));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_STATUS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_HEIGHT, 21));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_WIDTH, 226));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_SIZE_TO_TEXT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_CTRL_STATUS, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_CTRL_DELAY
    errChk(PANEL_CTRL_DELAY = NewCtrl (hPanel, CTRL_TEXT_MSG, "", 441, 530));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_CONSTANT_NAME, "CTRL_DELAY"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_FONT, VAL_DIALOG_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_STRIKEOUT, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_POINT_SIZE, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_BOLD, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_DFLT_VALUE, "0"));
    errChk(DefaultCtrl (hPanel, PANEL_CTRL_DELAY));
//  errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_HEIGHT, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_WIDTH, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_SIZE_TO_TEXT, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_TEXT_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(DSBindCtrl (hPanel, PANEL_CTRL_DELAY, "", VAL_DS_READ, NULL, NULL));

    // Build control: PANEL_STRIPCHART
    errChk(PANEL_STRIPCHART = NewCtrl (hPanel, CTRL_STRIP_CHART_LS, "", 10, 10));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_CONSTANT_NAME, "STRIPCHART"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_CALLBACK_FUNCTION_POINTER, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_SHORTCUT_KEY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_DIMMED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_TOOLTIP_TEXT, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_TOOLTIP_DELAY, 1000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_TOOLTIP_HIDE_DELAY, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_DISABLE_CTRL_TOOLTIP, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_CTRL_MODE, VAL_INDICATOR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_JUSTIFY, VAL_LEFT_JUSTIFIED));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_ANGLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_WIDTH, 9));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_HEIGHT, 15));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_RAISED, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_GRID_COLOR, 0XB3B3B3));
// ****WARNING****  Skipped ATTR_GRID_STYLE = 0
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_PLOT_BGCOLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XYNAME_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XYNAME_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_FONT, VAL_DIALOG_META_FONT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_COLOR, VAL_BLACK));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_COLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XNAME, "Time"));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XGRID_VISIBLE, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XMINORGRID_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_VISIBLE, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XFORMAT, VAL_ABSOLUTE_TIME_FORMAT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XDIVISIONS, VAL_AUTO));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XPRECISION, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XPADDING, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XENG_UNITS, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XUSE_LABEL_STRINGS, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XAXIS_GAIN, 1.000000));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XAXIS_OFFSET, 0.000000));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YNAME, "Value [ms]"));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YNAME, ""));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YGRID_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YGRID_VISIBLE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YMINORGRID_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YMINORGRID_VISIBLE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_VISIBLE, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_VISIBLE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YMAP_MODE, VAL_LINEAR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YMAP_MODE, VAL_LINEAR));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YFORMAT, VAL_FLOATING_PT_FORMAT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YFORMAT, VAL_FLOATING_PT_FORMAT));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YDIVISIONS, VAL_AUTO));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YDIVISIONS, VAL_AUTO));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YPRECISION, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YPRECISION, 1));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YPADDING, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YPADDING, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YENG_UNITS, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YENG_UNITS, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YREVERSE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YREVERSE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YUSE_LABEL_STRINGS, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YUSE_LABEL_STRINGS, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YAXIS_GAIN, 1.000000));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YAXIS_GAIN, 1.000000));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YAXIS_OFFSET, 0.000000));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YAXIS_OFFSET, 0.000000));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_EDGE_STYLE, VAL_RAISED_EDGE));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_BORDER_VISIBLE, 1));
// ****WARNING****  Skipped ATTR_FIXED_PLOT_AREA = False
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XYNAME_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XYNAME_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XYNAME_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XYNAME_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XYNAME_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XYNAME_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_BOLD, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_CHARACTER_SET, VAL_NATIVE_CHARSET));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_ANGLE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_ANGLE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_ANGLE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_ITALIC, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_UNDERLINE, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_STRIKEOUT, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_XLABEL_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_POINT_SIZE, 11));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLABEL_POINT_SIZE, 11));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_GRAPH_BGCOLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ANTI_ALIASED_PLOTS, 1));
// ****WARNING****  Skipped ATTR_INNER_MARKERS_VISIBLE = False
// ****WARNING****  Skipped ATTR_PLOT_AREA_TOP = 8
// ****WARNING****  Skipped ATTR_PLOT_AREA_LEFT = 58
// ****WARNING****  Skipped ATTR_PLOT_AREA_WIDTH = 658
// ****WARNING****  Skipped ATTR_PLOT_AREA_HEIGHT = 328
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLOOSE_FIT_AUTOSCALING, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLOOSE_FIT_AUTOSCALING, 0));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLOOSE_FIT_AUTOSCALING_UNIT, 2));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_RIGHT_YAXIS));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_YLOOSE_FIT_AUTOSCALING_UNIT, 2));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_POINTS_PER_SCREEN, 200));
//  errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_SCROLL_MODE, VAL_CONTINUOUS));
// ****WARNING****  Skipped ATTR_SHOW_CHART_DIVISION_LABELS = True
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ENABLE_ANTI_ALIASING, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_HEIGHT, 394));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_WIDTH, 744));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_TOP, -11));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_LEFT, 87));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_TOP, 23));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_LEFT, 613));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LABEL_SIZE_TO_TEXT, 1));
//  errChk(DSBindCtrl (hPanel, PANEL_STRIPCHART, "", VAL_DS_READ, NULL, NULL));
    errChk(SetAxisScalingMode (hPanel, PANEL_STRIPCHART, VAL_RIGHT_YAXIS, VAL_MANUAL, 0.000000, 100.000000));
    errChk(SetAxisScalingMode (hPanel, PANEL_STRIPCHART, VAL_RIGHT_YAXIS, VAL_AUTOSCALE, 0.000000, 100.000000));
    errChk(SetAxisScalingMode (hPanel, PANEL_STRIPCHART, VAL_LEFT_YAXIS, VAL_MANUAL, 0.000000, 10.000000));
    errChk(InsertAxisItem (hPanel, PANEL_STRIPCHART, VAL_LEFT_YAXIS, -1, "", 0.000000));
//  errChk(SetAxisTimeFormat (hPanel, PANEL_STRIPCHART, VAL_BOTTOM_XAXIS, VAL_ABSOLUTE_TIME_FORMAT, "%X\n%x"));
//  errChk(SetAxisTimeFormat (hPanel, PANEL_STRIPCHART, VAL_LEFT_YAXIS, VAL_ABSOLUTE_TIME_FORMAT, "%X\n%x"));
//  errChk(SetAxisTimeFormat (hPanel, PANEL_STRIPCHART, VAL_RIGHT_YAXIS, VAL_ABSOLUTE_TIME_FORMAT, "%X\n%x"));
//  errChk(SetAxisTimeFormat (hPanel, PANEL_STRIPCHART, VAL_BOTTOM_XAXIS, VAL_RELATIVE_TIME_FORMAT, "%H:%M:%S"));
//  errChk(SetAxisTimeFormat (hPanel, PANEL_STRIPCHART, VAL_LEFT_YAXIS, VAL_RELATIVE_TIME_FORMAT, "%H:%M:%S"));
//  errChk(SetAxisTimeFormat (hPanel, PANEL_STRIPCHART, VAL_RIGHT_YAXIS, VAL_RELATIVE_TIME_FORMAT, "%H:%M:%S"));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ACTIVE_YAXIS, VAL_LEFT_YAXIS));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_NUM_TRACES, 3));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 1, ATTR_PLOT_STYLE, VAL_THIN_LINE));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 1, ATTR_TRACE_COLOR, 0X00CC33));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 1, ATTR_TRACE_POINT_STYLE, VAL_NO_POINT));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 1, ATTR_LINE_STYLE, VAL_SOLID));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 1, ATTR_TRACE_VISIBLE, 1));
	errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 1, ATTR_TRACE_LG_VISIBLE, 1));
	errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 1, ATTR_TRACE_LG_TEXT, "Delta [ms]"));
	
	errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 2, ATTR_PLOT_STYLE, VAL_THIN_LINE));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 2, ATTR_TRACE_COLOR, 0XFCFC03));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 2, ATTR_TRACE_POINT_STYLE, VAL_NO_POINT));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 2, ATTR_LINE_STYLE, VAL_SOLID));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 2, ATTR_TRACE_VISIBLE, 1));
	errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 2, ATTR_TRACE_LG_VISIBLE, 1));
	errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 2, ATTR_TRACE_LG_TEXT, "Delta avg"));
	
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 3, ATTR_PLOT_STYLE, VAL_THIN_LINE));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 3, ATTR_TRACE_COLOR, VAL_RED));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 3, ATTR_TRACE_POINT_STYLE, VAL_EMPTY_SQUARE));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 3, ATTR_LINE_STYLE, VAL_SOLID));
    errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 3, ATTR_TRACE_VISIBLE, 1));
	errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 3, ATTR_TRACE_LG_VISIBLE, 1));
	errChk(SetTraceAttribute (hPanel, PANEL_STRIPCHART, 3, ATTR_TRACE_LG_TEXT, "Jitter [ms]"));
	errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_VISIBLE, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_WIDTH, 100));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_HEIGHT, 100));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_FRAME_COLOR, 0XF0F0F0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_PLOT_BGCOLOR, VAL_BLACK));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_AUTO_SIZE, 1));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_INTERACTIVE, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_LEGEND_SHOW_SAMPLES, 1));

    // Set up ZPLANE order if necessary
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG_2, ATTR_ZPLANE_POSITION, 2));
    errChk(SetCtrlAttribute (hPanel, PANEL_TEXTMSG, ATTR_ZPLANE_POSITION, 3));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_INTERVAL, ATTR_ZPLANE_POSITION, 4));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_TIME, ATTR_ZPLANE_POSITION, 5));
    errChk(SetCtrlAttribute (hPanel, PANEL_TIMELBL, ATTR_ZPLANE_POSITION, 6));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELTA, ATTR_ZPLANE_POSITION, 7));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_OFFSET, ATTR_ZPLANE_POSITION, 8));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_JITTER, ATTR_ZPLANE_POSITION, 9));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_DELAY, ATTR_ZPLANE_POSITION, 10));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELTALBL, ATTR_ZPLANE_POSITION, 11));
    errChk(SetCtrlAttribute (hPanel, PANEL_OFFSETLBL, ATTR_ZPLANE_POSITION, 12));
    errChk(SetCtrlAttribute (hPanel, PANEL_JITTERLBL, ATTR_ZPLANE_POSITION, 13));
    errChk(SetCtrlAttribute (hPanel, PANEL_DELAYLBL, ATTR_ZPLANE_POSITION, 14));
    errChk(SetCtrlAttribute (hPanel, PANEL_STATUSLBL, ATTR_ZPLANE_POSITION, 15));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_STATUS, ATTR_ZPLANE_POSITION, 16));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_ZPLANE_POSITION, 17));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_ZPLANE_POSITION, 18));
    errChk(SetCtrlAttribute (hPanel, PANEL_STRIPCHART, ATTR_ZPLANE_POSITION, 19));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION, ATTR_ZPLANE_POSITION, 20));
    errChk(SetCtrlAttribute (hPanel, PANEL_DECORATION_2, ATTR_ZPLANE_POSITION, 21));

    // Set up TAB order if necessary
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_IP, ATTR_CTRL_TAB_POSITION, 0));
    errChk(SetCtrlAttribute (hPanel, PANEL_CTRL_ENABLE, ATTR_CTRL_TAB_POSITION, 1));

    // Set up splitter attachments if necessary

    // Finalize panel colors, positioning, and sizing
    errChk(SetPanelAttribute (hPanel, ATTR_CONFORM_TO_SYSTEM, 0));
    errChk(SetPanelAttribute (hPanel, ATTR_CONFORM_TO_SYSTEM_THEME, 0));
    errChk(SetPanelAttribute (hPanel, ATTR_SCALE_CONTENTS_ON_RESIZE, 1));
    errChk(GetSystemAttribute (ATTR_RESOLUTION_ADJUSTMENT, &resAdjustment));
    if (resAdjustment == VAL_USE_PANEL_SETTING)
        resAdjustment = 0;
    if (resAdjustment)
        {
        errChk(GetMonitorFromPanel (hPanel, &monitorId));
        errChk(GetMonitorAttribute (monitorId, ATTR_HEIGHT, &screenHeight));
        errChk(GetMonitorAttribute (monitorId, ATTR_WIDTH, &screenWidth));
        SFv = (((float)screenHeight/1400)-1)*0.01*resAdjustment+1;
        errChk(SetPanelAttribute (hPanel, ATTR_HEIGHT, (int)(508*SFv)));
        SFh = (((float)screenWidth/3440)-1)*0.01*resAdjustment+1;
        errChk(SetPanelAttribute (hPanel, ATTR_WIDTH, (int)(769*SFh)));
        errChk(SetPanelAttribute (hPanel, ATTR_TOP, (int)(24*SFv)));
        errChk(SetPanelAttribute (hPanel, ATTR_LEFT, (int)(3*SFh)));
        }
    errChk(SetPanelAttribute (hPanel, ATTR_TOP, VAL_AUTO_CENTER));
    errChk(SetPanelAttribute (hPanel, ATTR_LEFT, VAL_AUTO_CENTER));
    errChk(SetPanelAttribute (hPanel, ATTR_SCALE_CONTENTS_ON_RESIZE, 0));

    //We're done!
    return hPanel;
Error:
    if (bitmapId)
        DiscardBitmap (bitmapId);
    return error;
}

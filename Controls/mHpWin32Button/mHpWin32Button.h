/*****************************************************************************\
mHpWin32Button.h - Interface for the mHelp Win32 Button Control Management
Plateform - Win32, MFC & VB6
Version - 1.0
Date - 10/17/2012

Copyright(c) - Unknown Corporation. All rights reserved.
\*****************************************************************************/

#ifndef MHPWIN32BUTTON_H
#define MHPWIN32BUTTON_H

#include <mHpCtrlMgmt.h> // mHelp control management interface
#include <stdio.h>

#define BUTTON_STATE_NAME           "CheckState" // Only available for CheckBox button style

// --------------------------------
// Button class names
//
#define CLASS_WIN32_BUTTON          "Button"                    // Win32 & MFC class
#define CLASS_VB6_BUTTON            "ThunderRT6CommandButton"   // VB6 Button class
#define CLASS_VB6_CHECKBOX          "ThunderRT6CheckBox"        // VB6 CheckBox class
#define CLASS_VB6_RADIOBOX          "ThunderRT6OptionButton"    // VB6 RadioBox class

#define BUTTON_CLASS                CLASS_SEP(CLASS_WIN32_BUTTON) CLASS_VB6_BUTTON
#define CHECKBOX_CLASS              CLASS_SEP(CLASS_WIN32_BUTTON) CLASS_VB6_CHECKBOX
#define RADIOBOX_CLASS              CLASS_SEP(CLASS_WIN32_BUTTON) CLASS_VB6_RADIOBOX

// --------------------------------
// Button style IDs
//
#define ID_STYLE_BUTTON             0
#define ID_STYLE_CHECKBOX           1
#define ID_STYLE_CHK3STATE          2
#define ID_STYLE_RADIOBOX           3

// --------------------------------
// Captured message ID
//
#define ID_MESSAGE_CLICKED          0   // WM_COMMAND with BN_CLICKED notification (Left click)

// --------------------------------
// Declaration
void SetDllVersion(char*,UINT);

class CmHpWin32Button : ImHpCtrlMgmt //////////////////////////////////////////
{
public:
    CmHpWin32Button(UINT nIdx) : ImHpCtrlMgmt(nIdx,CData::IntArray)
    {
        // Set control names
        pName=new CCtrlName*[3];

        pName[0]=new CCtrlName("Button",CData::Undefined);
        pName[1]=new CCtrlName("CheckBox",CData::Undefined);
        pName[2]=new CCtrlName("RadioBox",CData::Undefined);

        // Set DLL version
        SetDllVersion(szDllVersion,DLL_VERSION_SIZE);

        // Set default state bound description (only available for CheckBox)
        strncpy(szStateBound,"[0;2]",STATE_BOUND_SIZE);
    }
    virtual ~CmHpWin32Button()
    {   
        // Name
        delete pName[0];
        delete pName[1];
        delete pName[2];
        delete [] pName;

        // State
        if(pState)
        {   // Delete state class instance
            delete pState[0];
            delete [] pState;
        }

        // Config
        if(pCfg)
        {   // Delete config class instance
            delete pCfg[0];
            delete [] pCfg;
        }
    }

    // --------------------------------
    // Properties
    //

    ////// Class name
    BOOL IsCtrlClass(const char* lpClass) const
    {
        // Check button class name
        if((!strncmp(lpClass,CLASS_WIN32_BUTTON,sizeof(CLASS_WIN32_BUTTON)-1)) ||
           (!strncmp(lpClass,CLASS_VB6_BUTTON,sizeof(CLASS_VB6_BUTTON)-1)) ||
           (!strncmp(lpClass,CLASS_VB6_CHECKBOX,sizeof(CLASS_VB6_CHECKBOX)-1)) ||
           (!strncmp(lpClass,CLASS_VB6_RADIOBOX,sizeof(CLASS_VB6_RADIOBOX)-1)))
        {   return TRUE;
        }
        return FALSE;
    }
    const char* GetCtrlClass(UINT iClassIdx) const
    {
        // Return all managed button class name
        if(iClassIdx<3)
        {   switch(iClassIdx)
            {   case 0: return BUTTON_CLASS;    // Button
                case 1: return CHECKBOX_CLASS;  // CheckBox
                case 2: return RADIOBOX_CLASS;  // RadioBox
            }
        }
        return NULL;
    }

    ////// Name
    const char* GetCtrlName(int iNameIdx = NO_VALUE) const
    {
        // Return the name of the control...
        if(iNameIdx<=NO_VALUE) // ...according control style
        {   switch(iCtrlStyleID)
            {   case ID_STYLE_BUTTON: { return pName[0]->GetName(); }
                case ID_STYLE_CHECKBOX:
                case ID_STYLE_CHK3STATE:
                {   return pName[1]->GetName();
                }
                case ID_STYLE_RADIOBOX: { return pName[2]->GetName(); }
                default: { return NULL; }
            }
        }

        // ...according index parameter
        if(iNameIdx<3) // 3 controls name are available
            return pName[iNameIdx]->GetName();

        return NULL;
    }

    ////// Info
    void GetCtrlVers(char* lpVers,UINT iMaxSize) const;
    void GetCtrlDesc(char* lpDesc,UINT iMaxSize) const;
    HICON GetIconList(UINT iIconIdx) const;
    int GetIconIdx() const;

    ////// User Interface (UI)
    HWND CreateUI(HWND hUIParent,HWND hSaveBtn) const;
    void SaveUI(HWND hUI) const;

    ////// Style
    BOOL SetStyle(int iStyleID)
    {
        // Check valid style
        switch(iStyleID)
        {   case ID_STYLE_BUTTON:
            case ID_STYLE_CHECKBOX:
            case ID_STYLE_CHK3STATE:
            case ID_STYLE_RADIOBOX:
            {   break;
            }
            default:
            {   assert(NULL); // Invalid button control style
                return FALSE;
            }
        }

        // Assign control style
        iCtrlStyleID=iStyleID;

        // Define state & config class instance according control style
        if((!pState)&&(!pCfg))
        {   switch(iCtrlStyleID)
            {   case ID_STYLE_BUTTON:
                {   pCfg=new CCtrlCfg*[1];
                    pCfg[0]=new CCtrlCfg("ButtonLabel",CData::String);
                    break;
                }
                case ID_STYLE_CHECKBOX:
                case ID_STYLE_CHK3STATE:
                {   pState=new CCtrlState*[1];
                    pState[0]=new CCtrlState(BUTTON_STATE_NAME,CData::Integer);
                    break;
                }
                case ID_STYLE_RADIOBOX:
                {   pCfg=new CCtrlCfg*[1];
                    pCfg[0]=new CCtrlCfg("RadioLabel",CData::String);
                    break;
                }
            }
        }
        return TRUE;
    }

    ////// State
    UINT GetStateCount() const
    {
        // Return state count according control style
        switch(iCtrlStyleID)
        {   case ID_STYLE_CHECKBOX:
            case ID_STYLE_CHK3STATE:
            {   return 1;
            }
            case NO_VALUE: // Control style should be defined!
            {   assert(NULL);
                break;
            }
        }
        return 0;
    }
    CCtrlState* GetState(UINT iStateIdx)
    {
        // Return state according control style
        switch(iCtrlStyleID)
        {   case ID_STYLE_CHECKBOX:
            case ID_STYLE_CHK3STATE:
            {
                assert(iStateIdx==0); // One state is available for CheckBox control!
                if(!iStateIdx)
                    return pState[0];
                break;
            }
            case NO_VALUE: // Control style should be defined!
            {   assert(NULL);
                break;
            }
        }
        return NULL;
    }
    BOOL NeedToCheckState() const
    {   return FALSE; // No message could be displayd if process failed to set state
                      // -> State of CheckBox is fixed and can always be set successfully
    }
    const char* DisplayState();

    ////// Config
    UINT GetCfgCount() const
    {
        // Return config count according control style
        switch(iCtrlStyleID)
        {   case ID_STYLE_BUTTON:
            case ID_STYLE_RADIOBOX:
            {   return 1; // RadioBox or Button label
            }
            case NO_VALUE: // Control style should be defined!
            {   assert(NULL);
                break;
            }
        }
        return 0;
    }
    CCtrlCfg* GetConfig(UINT iCfgIdx)
    {
        // Return state according control style
        switch(iCtrlStyleID)
        {   case ID_STYLE_BUTTON:
            case ID_STYLE_RADIOBOX:
            {
                assert(iCfgIdx==0); // One config is available for RadioBox control!
                if(!iCfgIdx)
                    return pCfg[0];
                break;
            }
            case NO_VALUE: // Control style should be defined!
            {   assert(NULL);
                break;
            }
        }
        return NULL;
    }

    ////// HTML tags
    void GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const;

    // --------------------------------
    // Capture processus
    //
    BOOL SetCaptMsgID(int iMsgID)
    {
        // Check valid captured message ID
        switch(iMsgID)
        {   case ID_MESSAGE_CLICKED: // WM_COMMAND with BN_CLICKED notification (Left click)
            {
                // Assign captured message ID
                iCaptMsgID=iMsgID;
                break;
            }
            default: // Error: Invalid message ID!
            {   assert(NULL);
                iCaptMsgID=NO_VALUE;
                return FALSE;
            }
        }
        return TRUE;
    }
    BOOL CaptureWin32(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);

    // --------------------------------
    // Reproduce processus
    //
    BOOL GetCtrlPos(HWND hDlg,LONG &x,LONG &y,LONG &cx,LONG &cy) const;
    BOOL CheckState(HWND hDlg) const { return TRUE; }

    ActType NextAction(HWND hDlg);

    BOOL Start(HWND hDlg);
    BOOL End(HWND hDlg) { return TRUE; }

    BOOL MoveTo(HWND hDlg,int &xPos,int &yPos);
    BOOL MoveEvent(HWND hDlg) { return TRUE; }
    BOOL WaitUntil(HWND hDlg);

    BOOL LeftPressEvent(HWND hDlg);
    BOOL LeftUnpressEvent(HWND hDlg);
    BOOL RightPressEvent(HWND hDlg) { return TRUE; }
    BOOL RightUnpressEvent(HWND hDlg) { return TRUE; }
    BOOL EditEvent(HWND hDlg) { return TRUE; }

};

//////
__declspec(dllexport) void* CreateNewInstance(UINT,LPCSTR);

#endif /* MHPWIN32BUTTON_H */

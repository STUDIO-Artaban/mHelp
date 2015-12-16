/*****************************************************************************\
mHpWin32Slider.h - Interface for the mHelp Win32 Slider Control Management
Plateform - Win32, MFC & VB6
Version - 1.0
Date - 12/10/2012

Copyright(c) - Unknown Corporation. All rights reserved.
\*****************************************************************************/

#ifndef MHPWIN32SLIDER_H
#define MHPWIN32SLIDER_H

#include <mHpCtrlMgmt.h> // mHelp control management interface
#include <stdio.h>

#define SLIDER_STATE_NAME           "StatePos"

// --------------------------------
// Slider class names
//
#define CLASS_WIN32_SLIDER          "msctls_trackbar32"         // Win32 & MFC class
#define CLASS_VB6_SLIDER1           "SliderWndClass"            // VB6 Slider class
#define CLASS_VB6_SLIDER2           "Slider20WndClass"          // VB6 Slider class

#define SLIDER_CLASS                CLASS_SEP(CLASS_SEP(CLASS_WIN32_SLIDER) CLASS_VB6_SLIDER1) CLASS_VB6_SLIDER2

// --------------------------------
// Slider style IDs
//
#define ID_STYLE_SLIDER             0

// --------------------------------
// Captured message ID
//
#define ID_MESSAGE_SCROLL          0   // WM_HSCROLL | WM_VSCROLL
#define ID_MESSAGE_NOTIFY          1   // WM_NOTIFY with NM_RELEASEDCAPTURE notification code

// --------------------------------
// Declaration
void SetDllVersion(char*,UINT);

class CmHpWin32Slider : ImHpCtrlMgmt //////////////////////////////////////////
{
private:
    BOOL bVertOrientation;  // Orientation of the Slider (TRUE: Vertical; FALSE: Horizontal)
    int iTrackGap;          // Gap between each track position

public:
    CmHpWin32Slider(UINT nIdx) : ImHpCtrlMgmt(nIdx,CData::IntArray), bVertOrientation(FALSE), iTrackGap(0)
    {
        // Set control name(s)
        pName=new CCtrlName*[1];
        pName[0]=new CCtrlName("Slider/TrackBar",CData::Undefined);
        
        // Set DLL version
        SetDllVersion(szDllVersion,DLL_VERSION_SIZE);

        // Set default state bound description
        strncpy(szStateBound,"[;]",STATE_BOUND_SIZE);
    }
    virtual ~CmHpWin32Slider()
    {   
        // Name
        delete pName[0];
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
            delete pCfg[1];
            delete [] pCfg;
        }
    }

    // --------------------------------
    // Properties
    //

    ////// Win32 Class name
    BOOL IsCtrlClass(const char* lpCaptClass) const
    {
        // Check Slider class name
        if((!strncmp(lpCaptClass,CLASS_WIN32_SLIDER,sizeof(CLASS_WIN32_SLIDER)-1)) ||
           (!strncmp(lpCaptClass,CLASS_VB6_SLIDER1,sizeof(CLASS_VB6_SLIDER1)-1)) ||
           (!strncmp(lpCaptClass,CLASS_VB6_SLIDER2,sizeof(CLASS_VB6_SLIDER2)-1)))
        {   return TRUE;
        }
        return FALSE;
    }
    const char* GetCtrlClass(UINT iClassIdx) const
    {
        // Return Slider class name
        if(iClassIdx<1)
            return SLIDER_CLASS;    // Slider

        return NULL;
    }

    ////// Name
    const char* GetCtrlName(int iNameIdx = NO_VALUE) const
    {
        // Return the name of the control...
        if(iNameIdx<=NO_VALUE) // ...on which the user has acted
            return pName[0]->GetName();

        // ...according index parameter
        if(iNameIdx<1) // Only one name is available
            return pName[0]->GetName();

        return NULL;
    }

    ////// Info
    void GetCtrlVers(char* lpVers,UINT iMaxSize) const;
    void GetCtrlDesc(char* lpDesc,UINT iMaxSize) const;
    HICON GetIconList(UINT iIconIdx) const;
    int GetIconIdx() const;

    ////// User Interface (UI)
    HWND CreateUI(HWND hParentUI,HWND hSaveBtn) const;
    void SaveUI(HWND hUI) const;

    ////// Style
    BOOL SetStyle(int iStyleID)
    {
        // Check valid style
        if(iStyleID!=ID_STYLE_SLIDER)
        {   assert(NULL); // Invalid slider control style
            return FALSE;
        }

        // Assign control style
        iCtrlStyleID=iStyleID;

        // Define state & config class instance
        if(!pState)
        {
            // Define configuration to store min & max settings
            pCfg=new CCtrlCfg*[2];
            pCfg[0]=new CCtrlCfg("MaxPos",CData::Integer);
            pCfg[1]=new CCtrlCfg("MinPos",CData::Integer);

            // Define state to store Slider position
            pState=new CCtrlState*[1];
            pState[0]=new CCtrlState(SLIDER_STATE_NAME,CData::Integer);
        }
        return TRUE;
    }

    ////// State
    UINT GetStateCount() const
    {
        // Return Slider state count
        return 1;
    }
    CCtrlState* GetState(UINT iStateIdx)
    {
        // Return Slider state
        assert(iStateIdx==0); // One state is available for Slider control!
        if(!iStateIdx)
            return pState[0];

        return NULL;
    }
    BOOL NeedToCheckState() const
    {   return TRUE;    // A message can be displayed if process failed to set state
                        // -> The Slider position to set is not in its range
    }
    const char* DisplayState();

    ////// Config
    UINT GetCfgCount() const
    {
        // Return config count
        return 2;   // Min & max config
    }
    CCtrlCfg* GetConfig(UINT iCfgIdx)
    {
        // Return config
        assert(iCfgIdx<2); // Two config are availables for Slider control!
        if(iCfgIdx<2)
            return pCfg[iCfgIdx];

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
        {   case ID_MESSAGE_SCROLL: // WM_HSCROLL | WM_VSCROLL
            case ID_MESSAGE_NOTIFY: // WM_NOTIFY with NM_RELEASEDCAPTURE notification code
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

private:
    BOOL CaptureSliderMsg(HWND hParent,int iMsgID);

public:
    BOOL CaptureWin32(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);

    // --------------------------------
    // Reproduce processus
    //

    BOOL GetCtrlPos(HWND hDlg,LONG &x,LONG &y,LONG &cx,LONG &cy) const;
    BOOL CheckState(HWND hDlg) const;

    ActType NextAction(HWND hDlg);

    BOOL Start(HWND hDlg);
    BOOL End(HWND hDlg) { return TRUE; }

    BOOL MoveTo(HWND hDlg,int &xPos,int &yPos);
    BOOL MoveEvent(HWND hDlg);
    BOOL WaitUntil(HWND hDlg) { return TRUE; }

    BOOL LeftPressEvent(HWND hDlg);
    BOOL LeftUnpressEvent(HWND hDlg) { return TRUE; }
    BOOL RightPressEvent(HWND hDlg) { return TRUE; }
    BOOL RightUnpressEvent(HWND hDlg) { return TRUE; }
    BOOL EditEvent(HWND hDlg) { return TRUE; }

};

//////
__declspec(dllexport) void* CreateNewInstance(UINT,LPCSTR);

#endif /* MHPWIN32SLIDER_H */

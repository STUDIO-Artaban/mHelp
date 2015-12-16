// Copyright (c) Unknown Corporation.  All rights reserved.
//

#include "mHpWin32Slider.h"
#include "resource.h"

//---------------------------------
// Globals
HINSTANCE hInstCtrlMgmt=NULL;

///////////////////////////////////////////////////////////////////////////////

#define IsWndStyle(hWnd,Style)      ((BOOL)(GetWindowLongPtr(hWnd,GWL_STYLE)&(LONG_PTR)Style)==(LONG_PTR)Style)

void SetDllVersion(char* lpVersion,UINT iMaxSize)
{   
    // Set default version
    strncpy(lpVersion,"1.0.0.0",iMaxSize);

    // Get DLL version from current DLL file
    char lpDllFileName[MAX_PATH]={0};
    if(!GetModuleFileName((HMODULE)hInstCtrlMgmt,lpDllFileName,MAX_PATH))
        return;

    DWORD dwSize=GetFileVersionInfoSize(lpDllFileName,NULL);
    if(!dwSize)
        return;

    BYTE* pVersionInfo=NULL;
    pVersionInfo=new BYTE[dwSize];
    if(!GetFileVersionInfo(lpDllFileName,0,dwSize,pVersionInfo))
    {   delete [] pVersionInfo;
        return;
    }

    VS_FIXEDFILEINFO* pFileInfo=NULL;
    UINT iLenFileInfo=0;
    if(!VerQueryValue(pVersionInfo,"\\",(LPVOID*)&pFileInfo,&iLenFileInfo))
    {   delete [] pVersionInfo;
        return;
    }

    //////
    _snprintf(lpVersion,iMaxSize,"%d.%d.%d.%d",HIWORD(pFileInfo->dwFileVersionMS),
                                               LOWORD(pFileInfo->dwFileVersionMS),
                                               HIWORD(pFileInfo->dwFileVersionLS),
                                               LOWORD(pFileInfo->dwFileVersionLS));
}

LRESULT CALLBACK UIDlgProc(HWND hWndUI,UINT Msg,WPARAM wParam,LPARAM lParam)
{	static HWND hWndSave=NULL;
    switch(Msg)
	{	case WM_INITDIALOG:
        {   hWndSave=(HWND)lParam;
            return TRUE;
        }
        case WM_NOTIFY:
        {	switch(((LPNMHDR)lParam)->idFrom)
			{	case IDC_SLIDER_CFG:
				{	switch(((LPNMHDR)lParam)->code)
					{	case NM_RELEASEDCAPTURE:
						{	char lpTextPos[64];
                            _snprintf(lpTextPos,64,"%d",(int)SendMessage(((LPNMHDR)lParam)->hwndFrom,TBM_GETPOS,0,0));
							SetWindowText(GetDlgItem(hWndUI,IDC_EDIT_POSITION),lpTextPos);
							EnableWindow(hWndSave,TRUE);
							break;
						}
					}
					break;
				}
				default:
				{	return DefWindowProc(hWndUI,Msg,wParam,lParam);
                }
			}
			break;
        }
	}
	return FALSE;
}

//*****************************************************************************

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{  switch(ul_reason_for_call)
   {   case DLL_PROCESS_ATTACH:
		{   hInstCtrlMgmt=(HINSTANCE)hModule;
            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        {   break;
        }
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ImHpCtrlMgmt class implemetation

// --------------------------------
// Properties
//
void CmHpWin32Slider::GetCtrlVers(char* lpVers,UINT iMaxSize) const
{   LoadString(hInstCtrlMgmt,IDS_SLIDER_VERSION,lpVers,iMaxSize);
}

void CmHpWin32Slider::GetCtrlDesc(char* lpDesc,UINT iMaxSize) const
{   LoadString(hInstCtrlMgmt,IDS_SLIDER_DESCRIPTION,lpDesc,iMaxSize);
}

const char* CmHpWin32Slider::DisplayState()
{
    // Check display state string allocation
    if(!szDisplayState)
        szDisplayState=new char[128];

    // Return Slider state representation
    char lpDispPos[64];
    LoadString(hInstCtrlMgmt,IDS_DISPLAY_POSITION,lpDispPos,64);
    const int* iDispPos=(const int*)pState[0]->GetValue();
    _snprintf(szDisplayState,128,"%d%s",(*iDispPos),lpDispPos);

    return szDisplayState;
}

HICON CmHpWin32Slider::GetIconList(UINT iIconIdx) const
{
    // Return icon according a specific index
    if(iIconIdx < 1)
        return LoadIcon(hInstCtrlMgmt,MAKEINTRESOURCE(IDI_ICON_SLIDER));     // Slider icon
    return NULL;
}

int CmHpWin32Slider::GetIconIdx() const
{
    // Return icon index
    return 0;
}

HWND CmHpWin32Slider::CreateUI(HWND hParentUI,HWND hSaveBtn) const
{
    HWND hwndUI=NULL;

    // Create dialog handle to set slider position
    hwndUI=CreateDialogParam(hInstCtrlMgmt,
                             MAKEINTRESOURCE(IDD_SLIDER_UI),
                             hParentUI,
                             (DLGPROC)UIDlgProc,
                             (LPARAM)hSaveBtn);

    // Check valid UI handle
    if(hwndUI!=NULL)
    {
        // Set the range according slider configuration
        const int* iMaxPos=(const int*)pCfg[0]->GetValue(); // Max position
        const int* iMinPos=(const int*)pCfg[1]->GetValue(); // Min position

        	HWND hWndSliderCfg=GetDlgItem(hwndUI,IDC_SLIDER_CFG);
	    SendMessage(hWndSliderCfg,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)(*iMaxPos));
	    SendMessage(hWndSliderCfg,TBM_SETRANGEMIN,(WPARAM)TRUE,(LPARAM)(*iMinPos));

        // Select current Slider position
        const int* iCurPos=(const int*)pState[0]->GetValue();
	    SendMessage(hWndSliderCfg,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(*iCurPos));

        char lpTextPos[64];
	    _snprintf(lpTextPos,64,"%d",(*iCurPos));
	    SetWindowText(GetDlgItem(hwndUI,IDC_EDIT_POSITION),lpTextPos);
    }

    return hwndUI;
}

void CmHpWin32Slider::SaveUI(HWND hUI) const
{
    // Assign new position
    pState[0]->SetValue((int)SendMessage(GetDlgItem(hUI,IDC_SLIDER_CFG),TBM_GETPOS,0,0));
}

void CmHpWin32Slider::GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const
{
    assert(iCtrlStyleID!=NO_VALUE);

    // Get config and state
    const int* iMaxPos=(const int*)pCfg[0]->GetValue();
    const int* iMinPos=(const int*)pCfg[1]->GetValue();

    const int* iStatePos=(const int*)pState[0]->GetValue();

    _snprintf(lpTags,
              iMaxSize,
              "\t<input type=\"range\" name=\"%d-%s\" min=\"%d\" max=\"%d\" step=\"1\" value=\"%d\"/>\r\n",
              iTaskIdx,             // Task index (needed)
              SLIDER_STATE_NAME,    // State name
              (int)(*iMinPos),      // Min Slider position
              (int)(*iMaxPos),      // Max Slider position
              (int)(*iStatePos));   // State

    // eg. <input type="range" name="%s" min="1" max="50" step="1" value="4"/>
    // -> "range" input type available for Chrome 8, Safari 5, Opera 11 (when not supported display a "text" input)
}

// --------------------------------
// Capture processus
//
BOOL CmHpWin32Slider::CaptureSliderMsg(HWND hParent,int iMsgID)
{
    assert(hControl);   // hControl should be defined!

    // Check Slider class name
    char lpSliderClass[CTRL_CLASS_SIZE]={0};
    GetClassName(hControl,lpSliderClass,CTRL_CLASS_SIZE);
    if(IsCtrlClass(lpSliderClass)) // Always check class name in CaptureMessage() method!
    {
        // Set unique control ID
        SetWin32CtrlID(hParent);

        // Set control class
        strncpy(szClass,lpSliderClass,CTRL_CLASS_SIZE);

        // Set Slider style
        SetStyle(ID_STYLE_SLIDER);

        // Set captured message ID
        iCaptMsgID=iMsgID;

        // Set state, config & state bound
        pState[0]->SetValue((int)SendMessage(hControl,TBM_GETPOS,0,0));

        pCfg[0]->SetValue((int)SendMessage(hControl,TBM_GETRANGEMAX,0,0));
        pCfg[1]->SetValue((int)SendMessage(hControl,TBM_GETRANGEMIN,0,0));

        _snprintf(szStateBound,
                  STATE_BOUND_SIZE,
                  "[%d;%d]",
                  (int)SendMessage(hControl,TBM_GETRANGEMIN,0,0),
                  (int)SendMessage(hControl,TBM_GETRANGEMAX,0,0));

        return TRUE; // Slider message
    }
    return FALSE; // Not a Slider message
}

BOOL CmHpWin32Slider::CaptureWin32(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
    // Switch on window message
    switch(Message)
    {   case WM_HSCROLL:
		case WM_VSCROLL:
		{	if(lParam!=NULL)
            {
                // Get Slider control handle
                hControl=(HWND)lParam;

                return CaptureSliderMsg(hwnd,ID_MESSAGE_SCROLL);
            }
            break;
        }
		case WM_NOTIFY:
		{	if(((LPNMHDR)lParam)->hwndFrom!=NULL)
            {
                // Get Slider control handle
                hControl=((LPNMHDR)lParam)->hwndFrom;

                return CaptureSliderMsg(hwnd,ID_MESSAGE_NOTIFY);
            }
            break;
        }
        default: { break; }
    }
    return FALSE; // Not a Slider message
}

// --------------------------------
// Reproduce processus
//

//////
BOOL CmHpWin32Slider::GetCtrlPos(HWND hDlg,LONG &x,LONG &y,LONG &cx,LONG &cy) const
{
    assert(hControl!=NULL); // Start method has been called & succeeded!

    // Get screen upper-left corner position of the control
    RECT SliderRect;
    if(GetWindowRect(hControl,&SliderRect))
    {
        x=SliderRect.left;
        y=SliderRect.top;
        cx=SliderRect.right-SliderRect.left;
        cy=SliderRect.bottom-SliderRect.top;
        return TRUE;
    }
    return FALSE;
}

BOOL CmHpWin32Slider::CheckState(HWND hDlg) const
{
    assert(hControl!=NULL);

    // Check if position to set is in current Slider range
    const int* iNewPos=(const int*)pState[0]->GetValue();

    if(((*iNewPos)>((int)SendMessage(hControl,TBM_GETRANGEMAX,0,0)))||
       ((*iNewPos)<((int)SendMessage(hControl,TBM_GETRANGEMIN,0,0))))
    {   return FALSE; // Warning: State not in range!
    }
    return TRUE;
}

ImHpCtrlMgmt::ActType CmHpWin32Slider::NextAction(HWND hDlg)
{
    assert(hControl!=NULL);

    // Next action step
    iActStep++;

    // Switch on message to simulate
    switch(iCaptMsgID)
    {   case ID_MESSAGE_SCROLL:
        case ID_MESSAGE_NOTIFY:
        {
            // Switch on current action step
            switch(iActStep)
            {   case 1: { return ActMove; } // Move mHelp cursor over the Slider thumb
                case 2:
                {
                    assert(pState!=NULL); // State should be declared!
                    if(!pState)
                        return ActError;

                    // Check control state to know if already positionned
                    const int* iNewPos=(const int*)pState[0]->GetValue();
                    if((*iNewPos)==((int)SendMessage(hControl,TBM_GETPOS,0,0)))
                        return ActEnd; // Finish

                    // Initialize track gap
                    RECT rectChl;
                    SendMessage(hControl,TBM_GETCHANNELRECT,0,(LPARAM)&rectChl);

                    int iRange=(int)SendMessage(hControl,TBM_GETRANGEMAX,0,0);
                    iRange-=(int)SendMessage(hControl,TBM_GETRANGEMIN,0,0);
                    if(bVertOrientation)
                        iTrackGap=((int)(rectChl.bottom-rectChl.top))/iRange;
                    else
                        iTrackGap=((int)(rectChl.right-rectChl.left))/iRange;

                    // Initialize gap direction
                    int iCurPos=(int)SendMessage(hControl,TBM_GETPOS,0,0);
                    if(iCurPos>(*iNewPos))
                        iTrackGap*=(-1); // Decrease next position
                                         // -> Add negative == substract (see 'MoveTo' step 3)

                    // Press left mHelp cursor button
                    return ActLeftPress;
                }
                case 3:
                {
                    // Check control state to know if in new position
                    const int* iNewPos=(const int*)pState[0]->GetValue();
                    if((*iNewPos)==((int)SendMessage(hControl,TBM_GETPOS,0,0)))
                        return ActLeftUnpress; // Left unpress

                    // Move to next position
                    return ActMove;
                }
                case 4:
                {
                    // Finish
                    return ActEnd;
                }
            }
            break;
        }
        default:
        {
            // Error: Wrong message ID!
            assert(NULL);
            break;
        }
    }

    return ActError; // Error
}

BOOL CmHpWin32Slider::Start(HWND hDlg)
{
    // Get Slider control handle using control ID
    GetWin32CtrlHandle(hDlg);

    if((!IsWindow(hControl))||           // Check if hControl is a valid window
       (!IsWindowVisible(hControl))||    // Check control visible
       (!IsWindowEnabled(hControl)))     // Check control enabled
    {   hControl=NULL;
        return FALSE;
    }

    // Check if class name match with current class name
    char lpCurClass[CTRL_CLASS_SIZE];
    GetClassName(hControl,lpCurClass,CTRL_CLASS_SIZE);
    if(!IsCtrlClass(lpCurClass))
    {   hControl=NULL;
        return FALSE;
    }

    // Check if control style match with current style (no specific style for this control)
    if(iCtrlStyleID!=ID_STYLE_SLIDER)
    {   hControl=NULL;
        return FALSE;
    }

    // Check valid message to simulate
    switch(iCaptMsgID)
    {   case ID_MESSAGE_SCROLL:
        case ID_MESSAGE_NOTIFY:
        {   break;
        }
        default: // Error: Invalid message ID!
        {   return FALSE;
        }
    }

    // Initialize Slider orientation
    bVertOrientation=IsWndStyle(hControl,TBS_VERT);

    // Reset action step
    iActStep=0;

    return TRUE;
}

BOOL CmHpWin32Slider::MoveTo(HWND hDlg,int &xPos,int &yPos)
{
    assert(hControl!=NULL);

    // Move according action step
    switch(iActStep)
    {   case 1:
        {
            // Move to the thumb of the Slider control...
            RECT rectCtrl,rectThumb;

            GetWindowRect(hControl,&rectCtrl);
            SendMessage(hControl,TBM_GETTHUMBRECT,0,(LPARAM)&rectThumb);
            xPos=(int)((rectThumb.left+rectThumb.right)/2);
            xPos+=(int)(rectCtrl.left);
            yPos=(int)((rectThumb.bottom+rectThumb.top)/2);
            yPos+=(int)(rectCtrl.top);

            // ...according its orientation
            if(bVertOrientation)
                yPos-=4;
            break;
        }
        case 3:
        {
            // Move to the next position
            // -> Always add track gap (gap is negative when needed to decrease)
            if(bVertOrientation)
                yPos+=iTrackGap;
            else
                xPos+=iTrackGap;
            break;
        }
    }
    return TRUE;
}

BOOL CmHpWin32Slider::MoveEvent(HWND hDlg)
{
    assert(hControl!=NULL);

    if(iActStep!=1) // == 3
    {
        assert(iActStep==3);

        // Set next position
        int iNextPos=(int)SendMessage(hControl,TBM_GETPOS,0,0);
        if(iTrackGap<0) // Check direction using track gap sign (see 'MoveTo' step 3)
            iNextPos--;
        else
            iNextPos++;
        SendMessage(hControl,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)iNextPos);

        // Simulate user action messages
        ForceWin32ScrollMsg(hDlg,iNextPos,TB_THUMBPOSITION,bVertOrientation);
        ForceWin32NotifyMsg(hDlg,NM_RELEASEDCAPTURE);

        // Back to previous action
        iActStep--;
    }
    return TRUE;
}

BOOL CmHpWin32Slider::LeftPressEvent(HWND hDlg)
{
    assert(hControl!=NULL);

    // Set focus to Slider control
    SetFocus(hControl);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void* CreateNewInstance(UINT iIdx,LPCSTR lpDllPath)
{   
    return static_cast<void*>(new CmHpWin32Slider(iIdx));
}

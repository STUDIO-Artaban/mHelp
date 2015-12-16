// Copyright (c) Unknown Corporation.  All rights reserved.
//

#include "mHpWin32Button.h"
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

int GetStyleID(HWND hwndCtrl)
{   // Check if 3 states CheckBox style button
    if((IsWndStyle(hwndCtrl,BS_3STATE)) ||
       (IsWndStyle(hwndCtrl,BS_AUTO3STATE)))
        return ID_STYLE_CHK3STATE;

    // Check if CheckBox style button
    if((IsWndStyle(hwndCtrl,BS_CHECKBOX)) ||
       (IsWndStyle(hwndCtrl,BS_AUTOCHECKBOX)))
        return ID_STYLE_CHECKBOX;

    // Check RadioBox style button
    if((IsWndStyle(hwndCtrl,BS_RADIOBUTTON)) ||
       (IsWndStyle(hwndCtrl,BS_AUTORADIOBUTTON)))
        return ID_STYLE_RADIOBOX;

    // Default style: Button
    return ID_STYLE_BUTTON;
}

LRESULT CALLBACK UIDlgProc(HWND hWndUI,UINT Msg,WPARAM wParam,LPARAM lParam)
{	static HWND hWndSave=NULL;
    switch(Msg)
	{	case WM_INITDIALOG:
        {   hWndSave=(HWND)lParam;
            return TRUE;
        }
        case WM_COMMAND:
		{	switch(LOWORD(wParam))
            {   case IDC_CHECK_ACTIVE:
                {   SetWindowText(GetDlgItem(hWndUI,IDC_EDIT_STATE),"1");
                    EnableWindow(hWndSave,TRUE);
                    return TRUE;
                }
                case IDC_CHECK_INACTIVE:
                {   SetWindowText(GetDlgItem(hWndUI,IDC_EDIT_STATE),"0");
                    EnableWindow(hWndSave,TRUE);
                    return TRUE;
                }
                case IDC_CHECK_CHANGEINT:
                {   SetWindowText(GetDlgItem(hWndUI,IDC_EDIT_STATE),"2");
                    EnableWindow(hWndSave,TRUE);
                    return TRUE;
                }
                default:
                {   break;}
            }
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
void CmHpWin32Button::GetCtrlVers(char* lpVers,UINT iMaxSize) const
{   LoadString(hInstCtrlMgmt,IDS_BUTTON_VERSION,lpVers,iMaxSize);
}

void CmHpWin32Button::GetCtrlDesc(char* lpDesc,UINT iMaxSize) const
{   LoadString(hInstCtrlMgmt,IDS_BUTTON_DESCRIPTION,lpDesc,iMaxSize);
}

const char* CmHpWin32Button::DisplayState()
{
    // Check display state string allocation
    if(!szDisplayState)
        szDisplayState=new char[128];

    // Return state representation according control style
    switch(iCtrlStyleID)
    {   case ID_STYLE_BUTTON:
        case ID_STYLE_RADIOBOX:
        {   char lpNoState[64]={0};
            LoadString(hInstCtrlMgmt,IDS_NO_STATE,lpNoState,64);
            strncpy(szDisplayState,lpNoState,64);
            strncat(szDisplayState," (",2);
            const char* lpLabel=(const char*)pCfg[0]->GetValue();
            strncat(szDisplayState,lpLabel,strlen(lpLabel));
            strncat(szDisplayState,")",1);
            return szDisplayState;
        }
        case ID_STYLE_CHECKBOX:
        case ID_STYLE_CHK3STATE:
        {   const int* iState=(const int*)pState[0]->GetValue();
            switch(*iState)
            {   case 0: // BST_UNCHECKED
                {   LoadString(hInstCtrlMgmt,IDS_STATE_UNCHECKED,szDisplayState,128);
                    return szDisplayState;
                }
                case 1: // BST_CHECKED
                {   LoadString(hInstCtrlMgmt,IDS_STATE_CHECKED,szDisplayState,128);
                    return szDisplayState;
                }
                case 2: // BST_INDETERMINATE | Change state
                {   if(iCtrlStyleID==ID_STYLE_CHK3STATE)
                        LoadString(hInstCtrlMgmt,IDS_STATE_INDETERMINATE,szDisplayState,128);
                    else
                        LoadString(hInstCtrlMgmt,IDS_STATE_CHANGE,szDisplayState,128);
                    return szDisplayState;
                }
            }
            break;
        }
        case NO_VALUE: // Should be defined
        {   assert(NULL);
            break;
        }
    }
    return NULL;
}

HICON CmHpWin32Button::GetIconList(UINT iIconIdx) const
{
    // Return icon according a specific index
    switch(iIconIdx)
    {   case 0: { return LoadIcon(hInstCtrlMgmt,MAKEINTRESOURCE(IDI_ICON_BUTTON)); }     // Button icon
        case 1: { return LoadIcon(hInstCtrlMgmt,MAKEINTRESOURCE(IDI_ICON_CHECKBOX)); }   // CheckBox icon
        case 2: { return LoadIcon(hInstCtrlMgmt,MAKEINTRESOURCE(IDI_ICON_RADIOBOX)); }   // RadioBox icon
        default:
        {   break; // No more icon
        }
    }
    return NULL;
}

int CmHpWin32Button::GetIconIdx() const
{
    // Return icon index according current button style (see 'GetIconList' method)
    switch(iCtrlStyleID)
    {   case ID_STYLE_BUTTON: { return 0; }
        case ID_STYLE_CHECKBOX: { return 1; }
        case ID_STYLE_CHK3STATE:
        case ID_STYLE_RADIOBOX: { return 2; }
        case NO_VALUE: // Should be defined
        {   assert(NULL);
            break;
        }
    }
    return NO_VALUE; // Not defined
}

HWND CmHpWin32Button::CreateUI(HWND hUIParent,HWND hSaveBtn) const
{
    HWND hwndUI=NULL;

    // Create dialog handle to set button state (according control style)
    switch(iCtrlStyleID)
    {   case ID_STYLE_CHECKBOX:
        case ID_STYLE_CHK3STATE:
        {   hwndUI=CreateDialogParam(hInstCtrlMgmt,
                                     MAKEINTRESOURCE(IDD_CHECKBOX_UI),
                                     hUIParent,
                                     (DLGPROC)UIDlgProc,
                                     (LPARAM)hSaveBtn);
            // Check valid UI handle
            if(hwndUI!=NULL)
            {
                // Change RadioBox label text for a 3 states CheckBox
                if(iCtrlStyleID==ID_STYLE_CHK3STATE)
                {   char lpStrRS[64]={0};
                    LoadString(hInstCtrlMgmt,IDS_VALUE_INDETERMINATE,lpStrRS,64);
                    SetWindowText(GetDlgItem(hwndUI,IDC_CHECK_CHANGEINT),lpStrRS);
                }

                // Select current CheckBox state
                const int* iCurState=(const int*)pState[0]->GetValue();
                switch(*iCurState)
                {   case 0: // BST_UNCHECKED
                    {   SendDlgItemMessage(hwndUI,IDC_CHECK_INACTIVE,BM_SETCHECK,BST_CHECKED,0);
                        SetWindowText(GetDlgItem(hwndUI,IDC_EDIT_STATE),"0");
                        break;
                    }
                    case 1: // BST_CHECKED
                    {   SendDlgItemMessage(hwndUI,IDC_CHECK_ACTIVE,BM_SETCHECK,BST_CHECKED,0);
                        SetWindowText(GetDlgItem(hwndUI,IDC_EDIT_STATE),"1");
                        break;
                    }
                    case 2: // BST_INDETERMINATE | Change state
                    {   SendDlgItemMessage(hwndUI,IDC_CHECK_CHANGEINT,BM_SETCHECK,BST_CHECKED,0);
                        SetWindowText(GetDlgItem(hwndUI,IDC_EDIT_STATE),"2");
                        break;
                    }
                }
            }
            break;
        }
        case NO_VALUE: // Should be defined
        {   assert(NULL);
            break;
        }
    }
    return hwndUI;
}

void CmHpWin32Button::SaveUI(HWND hUI) const
{
    int iNewState;
    if(SendDlgItemMessage(hUI,IDC_CHECK_INACTIVE,BM_GETCHECK,0,0)==BST_CHECKED)
        iNewState=0; // BST_UNCHECKED
    else if(SendDlgItemMessage(hUI,IDC_CHECK_ACTIVE,BM_GETCHECK,0,0)==BST_CHECKED)
        iNewState=1; // BST_CHECKED
    else
        iNewState=2; // BST_INDETERMINATE | Change state

    // Assign new state
    pState[0]->SetValue(iNewState);
}

//////
#define HTML_CHECKBOX_TAGS  "\t<input type=\"radio\" name=\"%d-%s\" value=\"1\"%s/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font><br>\r\n" \
                            "\t<input type=\"radio\" name=\"%d-%s\" value=\"0\"%s/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font><br>\r\n" \
                            "\t<input type=\"radio\" name=\"%d-%s\" value=\"2\"%s/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font><br>\r\n"

void CmHpWin32Button::GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const
{
    *lpTags=0;

    switch(iCtrlStyleID)
    {   case ID_STYLE_BUTTON: // Button style
        case ID_STYLE_RADIOBOX: // RadioBox style
        {   // No state available
            break;
        }
        case ID_STYLE_CHECKBOX:
        case ID_STYLE_CHK3STATE:
        {
            assert(pState);

            // Get state
            const int* iState=(const int*)pState[0]->GetValue();

            // Get state label
            char lpChecked[64];
            LoadString(hInstCtrlMgmt,IDS_STATE_CHECKED,lpChecked,64);
            char lpUnchecked[64];
            LoadString(hInstCtrlMgmt,IDS_STATE_UNCHECKED,lpUnchecked,64);
            char lp3State[64];
            if(iCtrlStyleID==ID_STYLE_CHK3STATE)
                LoadString(hInstCtrlMgmt,IDS_STATE_INDETERMINATE,lp3State,64);
            else
                LoadString(hInstCtrlMgmt,IDS_STATE_CHANGE,lp3State,64);

            // Return HTML tags
            _snprintf(lpTags,
                      iMaxSize,
                      HTML_CHECKBOX_TAGS,
                      iTaskIdx,
                      BUTTON_STATE_NAME,
                      (*iState==1)? " checked":"", // BST_CHECKED
                      lpChecked,
                      iTaskIdx,
                      BUTTON_STATE_NAME,
                      (*iState==0)? " checked":"", // BST_UNCHECKED
                      lpUnchecked,
                      iTaskIdx,
                      BUTTON_STATE_NAME,
                      (*iState==2)? " checked":"", // BST_INDETERMINATE (or change state)
                      lp3State);

            // Will return following for a 3 state CheckBox button style with an unchecked current state:

            //  <input type="radio" name="0-CheckState" value="1"/>Activé<br>
            //  <input type="radio" name="0-CheckState" value="0" checked/>Désactivé<br>
            //  <input type="radio" name="0-CheckState" value="2"/>Indéterminé<br>

            break;
        }
        default:
        {   assert(NULL); // Not defined (NO_VALUE)
            break;
        }
    }
}

// --------------------------------
// Capture processus
//
BOOL CmHpWin32Button::CaptureWin32(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
    switch(Message)
    {   case WM_COMMAND: // WM_COMMAND message
        {
            // Check BN_CLICKED notification message
            if(HIWORD(wParam) == BN_CLICKED)
            {
                // Get button control handle from Win32 message
                hControl=(HWND)lParam;

                // Check button class name
                char lpBtnClass[CTRL_CLASS_SIZE]={0};
                GetClassName(hControl,lpBtnClass,CTRL_CLASS_SIZE);
                if(IsCtrlClass(lpBtnClass)) // Always check class name in CaptureMessage() method!
                {
                    // Set unique control ID
                    SetWin32CtrlID(hwnd);

                    // Set control class
                    strncpy(szClass,lpBtnClass,CTRL_CLASS_SIZE);

                    // Set button style
                    SetStyle(GetStyleID(hControl));

                    // Set captured message ID
                    iCaptMsgID=ID_MESSAGE_CLICKED;

                    // Set state & Config according control style
                    switch(iCtrlStyleID)
                    {   case ID_STYLE_BUTTON:
                        case ID_STYLE_RADIOBOX:
                        {   char lpLabel[64]={0};
                            int iLabSize=GetWindowTextLength(hControl);
                            // Check too long label
                            if(iLabSize>59)
                            {   if(GetWindowText(hControl,lpLabel,59))
                                    strncat(lpLabel,"...",3);
                            }
                            else
                                GetWindowText(hControl,lpLabel,64);
                            pCfg[0]->SetValue(lpLabel); // RadioBox or Button label
                            break;
                        }
                        case ID_STYLE_CHECKBOX:
                        case ID_STYLE_CHK3STATE:
                        {   switch((int)SendMessage(hControl,BM_GETCHECK,0,0))
                            {   case BST_UNCHECKED:
                                {   pState[0]->SetValue(0); // BST_UNCHECKED
                                    break;
                                }
                                case BST_CHECKED:
                                {   pState[0]->SetValue(1); // BST_CHECKED
                                    break;
                                }
                                case BST_INDETERMINATE:
                                {   pState[0]->SetValue(2); // BST_INDETERMINATE
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    return TRUE; // Button message
                }
            }
            break;
        }
        default: { break; }
    }
    return FALSE; // Not a button message
}

// --------------------------------
// Reproduce processus
//
BOOL CmHpWin32Button::GetCtrlPos(HWND hDlg,LONG &x,LONG &y,LONG &cx,LONG &cy) const
{
    assert(hControl!=NULL); // Start method has been called & succeeded!

    // Get screen upper-left corner position of the control
    RECT BtnRect;
    if(GetWindowRect(hControl,&BtnRect))
    {
        x=BtnRect.left;
        y=BtnRect.top;
        cx=BtnRect.right-BtnRect.left;
        cy=BtnRect.bottom-BtnRect.top;
        return TRUE;
    }
    return FALSE;
}

ImHpCtrlMgmt::ActType CmHpWin32Button::NextAction(HWND hDlg)
{
    assert(hControl!=NULL);

    // Next action step
    iActStep++;

    // Switch on message to simulate
    switch(iCaptMsgID)
    {   case ID_MESSAGE_CLICKED: // WM_COMMAND with BN_CLICKED notification (Left click)
        {
            // Switch on current action step
            switch(iActStep)
            {   case 1: { return ActMove; } // Move mHelp cursor over the control
                case 2:
                {
                    // Check control state according control style
                    switch(iCtrlStyleID)
                    {   case ID_STYLE_CHECKBOX:
                        {
                            assert(pState!=NULL); // State should be declared!
                            if(!pState)
                                return ActError;

                            // Check state to set
                            const int* iState=(const int*)pState[0]->GetValue();
                            switch(*iState)
                            {   case 0: // BST_UNCHECKED
                                {   if(BST_UNCHECKED == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                        return ActEnd; // Finished
                                    break;
                                }
                                case 1: // BST_CHECKED
                                {   if(BST_CHECKED == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                        return ActEnd; // Finished
                                    break;
                                }
                                case 2: { break; } // Change state wathever its current state
                            }
                            break;
                        }
                        case ID_STYLE_CHK3STATE:
                        {   
                            assert(pState!=NULL); // State should be declared!
                            if(!pState)
                                return ActError;

                            // Check state to set
                            const int* iState=(const int*)pState[0]->GetValue();
                            switch(*iState)
                            {   case 0: // BST_UNCHECKED
                                {   if(BST_UNCHECKED == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                        return ActEnd; // Finished
                                    break;
                                }
                                case 1: // BST_CHECKED
                                {   if(BST_CHECKED == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                        return ActEnd; // Finished
                                    break;
                                }
                                case 2: // BST_INDETERMINATE
                                {   if(BST_INDETERMINATE == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                        return ActEnd; // Finished
                                    break;
                                }
                            }
                            break;
                        }
                        case ID_STYLE_RADIOBOX:
                        {   
                            // Check if already checked
                            if(BST_CHECKED == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                return ActEnd; // Finished
                            break;
                        }
                    }

                    // Press left mHelp cursor button
                    return ActLeftPress;
                }
                case 3: { return ActLeftUnpress; } // Unpress left mHelp cursor button
                case 4:
                {
                    assert(iCtrlStyleID==ID_STYLE_CHK3STATE);   // Only for CheckBox 3 states button style!
                    assert(pState!=NULL);                       // State should be declared!
                    if(!pState)
                        return ActError;

                    // Check state to set
                    const int* iState=(const int*)pState[0]->GetValue();
                    switch(*iState)
                    {   case 0: // BST_UNCHECKED
                        {   if(BST_UNCHECKED == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                return ActEnd; // Finished
                            break;
                        }
                        case 1: // BST_CHECKED
                        {   if(BST_CHECKED == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                return ActEnd; // Finished
                            break;
                        }
                        case 2: // BST_INDETERMINATE
                        {   if(BST_INDETERMINATE == (int)SendMessage(hControl,BM_GETCHECK,0,0))
                                return ActEnd; // Finished
                            break;
                        }
                    }

                    // Wait a little (avoid double click)
                    return ActWait;
                }
                case 5: { return ActEnd; } // Finished
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

BOOL CmHpWin32Button::Start(HWND hDlg)
{
    // Get button control handle using control ID
    GetWin32CtrlHandle(hDlg);
    if(!hControl)
        return FALSE; // Failed to get Win32 control handle

    if((!IsWindow(hControl))||          // Check if 'hControl' is a valid window
       (!IsWindowVisible(hControl))||   // Check control visible
       (!IsWindowEnabled(hControl)))    // Check control enabled
    {   hControl=NULL;
        return FALSE;
    }

    // Check if class name match with current control class name
    char lpCurClass[CTRL_CLASS_SIZE];
    GetClassName(hControl,lpCurClass,CTRL_CLASS_SIZE);
    if(!IsCtrlClass(lpCurClass))
    {   hControl=NULL;
        return FALSE;
    }

    // Check if window style match with current style
    if(iCtrlStyleID!=GetStyleID(hControl))
    {   hControl=NULL;
        return FALSE;
    }

    // Check valid message to simulate
    switch(iCaptMsgID)
    {   case ID_MESSAGE_CLICKED: // WM_COMMAND with BN_CLICKED notification (Left click)
        {   break;
        }
        default: // Error: Invalid message ID!
        {   return FALSE;
        }
    }

    // Reset action step
    iActStep=0;

    return TRUE;
}

BOOL CmHpWin32Button::MoveTo(HWND hDlg,int &xPos,int &yPos)
{   
    assert(iActStep==1);    // First action!
    assert(hControl!=NULL);

	RECT rectCtrl;
    if(!GetWindowRect(hControl,&rectCtrl))
        return FALSE;

    // Move to position according to button style
    switch(iCtrlStyleID)
    {   case ID_STYLE_BUTTON:
        {   xPos=(int)(rectCtrl.right-9);
            yPos=(int)(rectCtrl.bottom-10);
            break;
        }
        case ID_STYLE_CHECKBOX:
        case ID_STYLE_CHK3STATE:
        {   xPos=(int)(rectCtrl.left+8);
            yPos=(int)(((rectCtrl.bottom-rectCtrl.top)/2)+rectCtrl.top-4);
            break;
        }
        case ID_STYLE_RADIOBOX:
        {   xPos=(int)(rectCtrl.left+9);
            yPos=(int)(((rectCtrl.bottom-rectCtrl.top)/2)+rectCtrl.top-4);
            break;
        }
    }

    return TRUE;
}

BOOL CmHpWin32Button::WaitUntil(HWND hDlg)
{
    assert(iCtrlStyleID==ID_STYLE_CHK3STATE); // Only for CheckBox 3 states button style!

    // Wait a little to avoid displaying mHelp cursor double click
    Sleep(500);

    // New click action to set the right state of the CheckBox 3 states
    iActStep=1;

    return TRUE;
}

BOOL CmHpWin32Button::LeftPressEvent(HWND hDlg)
{
    assert(hControl!=NULL);

    // Set focus to button control
    SetFocus(hControl);

    return TRUE;
}

BOOL CmHpWin32Button::LeftUnpressEvent(HWND hDlg)
{   
    assert(iActStep==3);
    assert(hControl!=NULL);

    // Simulate event according to button style
    switch(iCtrlStyleID)
    {   case ID_STYLE_BUTTON:
        {   
            // At next NextAction() call should return DCM_ACT_END
            iActStep++;

            // Send a WM_COMMAND to the parent Button control
            SendMessage(hDlg,WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(hControl),BN_CLICKED),(LPARAM)hControl);
            break;
        }
        case ID_STYLE_CHECKBOX:
        {   
            // At next NextAction() call should return DCM_ACT_END
            iActStep++;

            // Simulate a click event on CheckBox control
            SendMessage(hControl,BM_CLICK,0,0);
            break;
        }
        case ID_STYLE_CHK3STATE:
        {   
            // Simulate a click event on CheckBox control
            SendMessage(hControl,BM_CLICK,0,0);
            break;
        }
        case ID_STYLE_RADIOBOX:
        {   
            // At next NextAction() call should return DCM_ACT_END
            iActStep++;

            // Simulate a click event on RadioBox control
            SendMessage(hControl,BM_CLICK,0,0);
            break;
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void* CreateNewInstance(UINT iIdx,LPCSTR lpDllPath)
{   
    return static_cast<void*>(new CmHpWin32Button(iIdx));
}

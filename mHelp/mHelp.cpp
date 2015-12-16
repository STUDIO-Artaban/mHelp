// mHelp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#ifndef  __MHELP_H
 #include  "mHelp.h"
#endif

#ifndef  __STDIO_H
 #include <stdio.h>
#endif

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif
 
#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif
 
#ifndef  __COMMCTRL_H
 #include <commctrl.h>
#endif

#include <Objbase.h>
#include <mHpCtrl.h>
#include <mHpPak.h>
#include "mHpDlg.h"
#include "ServDlg.h"
#include "HelpCom.h"
#include "resource.h"

//-----------------------------------------------------//
// Définitions
#define		ID_CANCEL					500
#define		ID_STOCK					501
#define		IDC_TOOLBAR					502

//-----------------------------------------------------//
#define		ID_TIMER_WRITE_SHARED		100
#define		ID_TIMER_READ_SHARED		101
#define		ID_TIMER_CHANGE_WND			102
#define     ID_TIMER_VISIBLE            103
#define     ID_TIMER_TOOLSERV           104

//-----------------------------------------------------//
#define     MHPSERV_CLASS               "#mHelpServerClass"

//-----------------------------------------------------//
// Objets
class CWndCell //////////////////////////////////////////
{
public:
	HWND hWndItm;
	LONG_PTR lWndProc;
	CWndCell* NextWnd;
	CWndCell(HWND,LONG_PTR);
};

class CWndListe /////////////////////////////////////////
{
private:
	CWndCell* MainCell;
public:
	CWndListe();
	int AddWnd(HWND,LONG_PTR);
	void RemoveWnd(HWND);
	LONG_PTR GetWndProc(HWND);
	void EmptyWndList();
};

//-----------------------------------------------------------------------------------------------------------------------//
// Variables globales
char AppName[MAX_RS_SIZE]={0};		// Application name
HINSTANCE hInstDll;					// Instance de la dll
HWND hWndToolServ;					// HWND de la boite de dialogue ToolServ
HINSTANCE hInstTool=NULL;			// HINSTANCE de la boite de dialogue ToolServ
static HWND hToolbar;				// HWND de la toolbar
TBBUTTON ToolAry[2]={0};			// Tableau de toolbar boutons
BOOL bStockTask;					// TRUE: stock la tâche ainsi créé FALSE: annule
BOOL bStockFromDlg;					// TRUE: stock la tâche ainsi créé, à partir de ToolServ FALSE: sinon
CmHpCtrl* mHpCtrl;					// mHpCtrl object: Objects accesseurs
CWndListe* WndLst;					// WindowProc liste
CWndListe* FrameLst;				// FrameProc liste
CWindowListe* RealWndLst;			// RealProc liste
int iCurProcID=0;					// Identifiant du processus de création de macros en cours
static BOOL bInitPass=FALSE;		// TRUE: already called "mHelpInit", FALSE: orelse
static int iCurCustom=0;			// Last CustomID
HWND hWndMain;						// HWND de la fenêtre principale de l'application

//----------------------------------------------------------------------------------------------------------------GENERAL//
// Déclarations
void SetFrameCtrlProc(HWND);
void CreateMainDlg(HWND,BOOL,BOOL);
void MaximizeWindow(HWND);
//-----------------------------------------------------------------------------------------------------------MHP_TOOLSERV//
BOOL CreateToolServ(HWND);
HRESULT CALLBACK ToolServProc(HWND,UINT,WPARAM,LPARAM);
//------------------------------------------------------------------------------------------------------------MHP_WNDPROC//
LRESULT CALLBACK mHpWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK mHpFrameProc(HWND,UINT,WPARAM,LPARAM);
void CALLBACK mHpServTimerProc(HWND,UINT,UINT_PTR,DWORD);
//-------------------------------------------------------------------------------------------------------------MHP_EXPORT//

//****************************************************************************************************************MAINDLL//

BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{	switch (ul_reason_for_call)
	{	case DLL_PROCESS_ATTACH:
		{	hInstDll=(HINSTANCE)hModule;
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		{	break;}
	}
	return TRUE;
}

//*****************************************************************************************************************OBJETS//

// class CWndCell /////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndCell::CWndCell(HWND phWnd,LONG_PTR plProc)
{	hWndItm=phWnd;
	lWndProc=plProc;
	NextWnd=NULL;
}

// class CWndListe ////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndListe::CWndListe()
{	MainCell=NULL;}

int CWndListe::AddWnd(HWND hNewWnd,LONG_PTR NewProc)
{	CWndCell* tmp=MainCell;
	BOOL bExist=FALSE;
	char lpWndTitle[MAX_RS_SIZE]={0};
	GetWindowText(hNewWnd,lpWndTitle,MAX_RS_SIZE);
	if((strncmp(lpWndTitle,MHPPAK_SERVTITLE,MAX_RS_SIZE)!=0)&&
	   (strncmp(lpWndTitle,MHPPAK_DLGTITLE,MAX_RS_SIZE)!=0)&&
	   (strncmp(lpWndTitle,MHPPAK_DLGMSGTITLE,MAX_RS_SIZE)!=0))
	{	if(tmp==NULL)
		{	//if(GetWindowLongPtr(hNewWnd,DWLP_DLGPROC))
			//	MainCell=new CWndCell(hNewWnd,SetWindowLongPtr(hNewWnd,DWLP_DLGPROC,NewProc));
			//else
			MainCell=new CWndCell(hNewWnd,SetWindowLongPtr(hNewWnd,GWLP_WNDPROC,NewProc));
		}
		else
		{	while(tmp!=NULL)
			{	if(tmp->hWndItm==hNewWnd) bExist=TRUE;
				tmp=tmp->NextWnd;
			}
			if(!bExist)
			{	tmp=MainCell;
				while(tmp->NextWnd!=NULL) tmp=tmp->NextWnd;
				//if(GetWindowLongPtr(hNewWnd,DWLP_DLGPROC))
				//	tmp->NextWnd=new CWndCell(hNewWnd,SetWindowLongPtr(hNewWnd,DWLP_DLGPROC,NewProc));
				//else
				tmp->NextWnd=new CWndCell(hNewWnd,SetWindowLongPtr(hNewWnd,GWLP_WNDPROC,NewProc));
			}
            else
		        return 2; // TRUE: Already exists
		}
		return 1; // TRUE: Not already exists
	}
	return 0; // FALSE
}

void CWndListe::RemoveWnd(HWND hDestroyWnd)
{	CWndCell *find=MainCell;
	CWndCell *move=MainCell;
	while(find!=NULL)
	{	if(find->hWndItm==hDestroyWnd)
		{	if(move==find)
			{	MainCell=move->NextWnd;
				delete move;
			}
			else
			{	while(move->NextWnd!=find) move=move->NextWnd;
				move->NextWnd=find->NextWnd;
				delete find;
			}
			return;
		}
		find=find->NextWnd;
	}
}

LONG_PTR CWndListe::GetWndProc(HWND hCurWnd)
{	CWndCell* tmp=MainCell;
	LONG_PTR lRes=0;
	while(tmp!=NULL)
	{	if(tmp->hWndItm==hCurWnd) lRes=tmp->lWndProc;
		tmp=tmp->NextWnd;
	}
	return lRes;
}

void CWndListe::EmptyWndList()
{	CWndCell* tmp=MainCell;
	while(MainCell!=NULL)
	{	MainCell=MainCell->NextWnd;
		delete(tmp);
		tmp=MainCell;
	}
}

//****************************************************************************************************************GENERAL//

//-----------------------------------------------------//
// SetFrameCtrlProc
//-----------------------------------------------------//
void SetFrameCtrlProc(HWND hWndActive)
{	HWND hWndCtrl=GetWindow(hWndActive,GW_CHILD);
    while(hWndCtrl!=NULL)
	{	int iAddRes=FrameLst->AddWnd(hWndCtrl,(LONG_PTR)mHpFrameProc);

        ////// Implement DotNET controls
        if(iAddRes==1)
        {   // Add event(s) notification on current DotNET control
            // -> If DotNET control handle
            mHpCtrl_SetDotNetCtrlNotify(hWndCtrl);

        } //////

		SetFrameCtrlProc(hWndCtrl);
		hWndCtrl=GetWindow(hWndCtrl,GW_HWNDNEXT);
	}
}

//-----------------------------------------------------//
// CreateMainDlg
//-----------------------------------------------------//
void CreateMainDlg(HWND hWndActive,BOOL bAuto,BOOL bShared)
{	if(!mHpCtrl_bLockmHelp)
	{	if(bAuto) 
		{	MaximizeWindow(hWndActive);
			mHpCtrl_iCurrentWnd=0;
			mHpCtrl_iCountWnd=0;
			if(bShared)
			{	mHpCtrl_GetConfig(mHpCtrl->mHpTaskList,0,0,0,0,NULL,NULL,AppName,TRUE);
				mHpCtrl_SetTimer(hWndActive,1,8); // SHARED
			}
			else
			{	mHpCtrl_WriteOperationKey(1);
				mHpCtrl_GetConfig(mHpCtrl->mHpTaskList,
								  GetHelpComStruct()->iMode,
								  GetHelpComStruct()->MouseSpeed,
								  GetHelpComStruct()->iNbBoucle,
								  ((GetHelpComStruct()->iHourDelay*60)+GetHelpComStruct()->iMinuteDelay),
								  GetCurrentmHpFile(),
								  GetHelpComStruct()->IndexTab,
								  AppName,FALSE);
				mHpCtrl_SetTimer(hWndActive,1000,4); // AUTOASS
			}
		}
		else if(bShared)
		{	mHpCtrl_bUserAction=TRUE;
			mHpCtrl_bWritemHelp=TRUE;
			mHpCtrl_bLockmHelp=TRUE;
			bStockTask=TRUE;
			bStockFromDlg=FALSE;
			mHpCtrl_iCurrentWnd=0;
			mHpCtrl_iCountWnd=0;
			mHpCtrl_iLastID=NO;
			mHpCtrl_WriteAppName(mHpCtrl->mHpEventList,AppName,FALSE);
			//CreateToolServ(hWndActive);
            SetTimer(hWndActive,ID_TIMER_TOOLSERV,100,(TIMERPROC)mHpServTimerProc);
		}
		else switch(CreatemHpDlg(hWndActive))
		{	case NO:
			{	mHpPak_mHpMessage(hWndActive,hInstDll,IDS_CREATE_MHPDLG,NULL,0,0,MB_ICONSTOP);
				break;
			}
			case IDOK:
			{	MaximizeWindow(hWndActive);
				mHpCtrl_iCurrentWnd=0;
				mHpCtrl_iCountWnd=0;
				mHpCtrl_GetConfig(mHpCtrl->mHpTaskList,
								  GetHelpComStruct()->iMode,
								  GetHelpComStruct()->MouseSpeed,
								  GetHelpComStruct()->iNbBoucle,
								  ((GetHelpComStruct()->iHourDelay*60)+GetHelpComStruct()->iMinuteDelay),
								  GetCurrentmHpFile(),
								  GetHelpComStruct()->IndexTab,
								  AppName,FALSE);
				mHpCtrl_SetTimer(hWndActive,200,0); // START
				break;
			}
			case IDC_CREATE_ADD:
			{	mHpCtrl_bUserAction=TRUE;
				mHpCtrl_bWritemHelp=TRUE;
				mHpCtrl_bLockmHelp=TRUE;
				mHpCtrl_EmptyEvents(mHpCtrl->mHpEventList);
				bStockTask=TRUE;
				bStockFromDlg=FALSE;
				mHpCtrl_iCurrentWnd=0;
				mHpCtrl_iCountWnd=0;
				mHpCtrl_iLastID=NO;
                mHpCtrl_InitLastCtrlID();
                iCurProcID=mHpCtrl_WriteAppName(mHpCtrl->mHpEventList,AppName,TRUE);
				mHpPak_ProcWndLst->WriteAppWndKey();
				//CreateToolServ(hWndActive);
                SetTimer(hWndActive,ID_TIMER_TOOLSERV,100,(TIMERPROC)mHpServTimerProc);
				break;
			}
		}
	}
}

//-----------------------------------------------------//
// MaximizeWindow
//-----------------------------------------------------//
void MaximizeWindow(HWND hWndActive)
{	int xResol=GetDeviceCaps(GetDC(hWndActive),HORZRES);
	int yResol=GetDeviceCaps(GetDC(hWndActive),VERTRES);	
	if(mHpPak_IsMenuItemReady(GetSystemMenu(hWndActive,FALSE),NO,SC_MAXIMIZE))
	{	WINDOWPLACEMENT WndPlacement;
		WndPlacement.length=sizeof(WINDOWPLACEMENT);
		if(GetWindowPlacement(hWndActive,&WndPlacement))
		{	WndPlacement.flags=WPF_RESTORETOMAXIMIZED;
			WndPlacement.showCmd=SW_SHOWMAXIMIZED;
			WndPlacement.length=sizeof(WINDOWPLACEMENT);
			CONST WINDOWPLACEMENT CntlpWndPl=WndPlacement;
			SetWindowPlacement(hWndActive,&CntlpWndPl);
		}
	}
	else if(!IsZoomed(hWndActive))
	{	RECT Rect;
		GetWindowRect(hWndActive,&Rect);
		if((((int)(Rect.right))>xResol)||(((int)(Rect.left))<0)||
		   (((int)(Rect.bottom))>yResol)||(((int)(Rect.top))<0))
		{	MoveWindow(hWndActive,(int)((xResol/2)-((Rect.right-Rect.left)/2)),
				(int)((yResol/2)-((Rect.bottom-Rect.top)/2)),
				(int)(Rect.right-Rect.left),(int)(Rect.bottom-Rect.top),TRUE);
		}
	}
}

//***********************************************************************************************************MHP_TOOLSERV//
// CreateToolServ
//-----------------------------------------------------//
BOOL CreateToolServ(HWND hWndActive)
{   WNDCLASS ToolClass;
    memset(&ToolClass, 0x00, sizeof(WNDCLASS));
	ToolClass.style = CS_HREDRAW | CS_VREDRAW; 
    ToolClass.lpfnWndProc = (WNDPROC)ToolServProc; 
    ToolClass.cbClsExtra = 0; 
    ToolClass.cbWndExtra = 0; 
    ToolClass.hInstance = hInstTool; 
    ToolClass.hIcon = LoadIcon(hInstTool,IDI_APPLICATION); 
    ToolClass.hCursor = LoadCursor(NULL,IDC_ARROW); 
    ToolClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    ToolClass.lpszMenuName = MHPPAK_SERVTITLE; 
    ToolClass.lpszClassName = MHPSERV_CLASS;
	if(RegisterClass(&ToolClass)==0)
	{	mHpPak_mHpMessage(hWndActive,hInstDll,IDS_CREATE_MHPDLG,NULL,0,0,MB_ICONSTOP);
		return FALSE;
	}
    hWndToolServ=CreateWindow(MHPSERV_CLASS,MHPPAK_SERVTITLE,WS_CAPTION,500,500,
                              100,52,hWndActive,NULL,hInstTool,0);
	if(hWndToolServ==NULL)
	{	mHpPak_mHpMessage(hWndActive,hInstDll,IDS_CREATE_MHPDLG,NULL,0,0,MB_ICONSTOP);
		return FALSE;
	}
	ShowWindow(hWndToolServ,SW_SHOWNORMAL);
	UpdateWindow(hWndToolServ);
	//SetActiveWindow(hWndActive);
	return TRUE;
}

//-----------------------------------------------------//
// ToolServProc
//-----------------------------------------------------//
HRESULT CALLBACK ToolServProc(HWND hWndTool,UINT MessageID,WPARAM wParamTool,LPARAM lParamTool)
{	switch(MessageID)
	{	case WM_CREATE:
		{	RECT ToolRect;
			RECT WndRect;
			ToolAry[0].iBitmap   = 0;
			ToolAry[0].idCommand = ID_CANCEL;
			ToolAry[0].fsState   = TBSTATE_ENABLED;
			ToolAry[0].fsStyle   = TBSTYLE_BUTTON;
			ToolAry[0].dwData    = 0L;
			ToolAry[0].iString   = 0;
			ToolAry[1].iBitmap   = 1;
			ToolAry[1].idCommand = ID_STOCK;
			ToolAry[1].fsState   = TBSTATE_ENABLED;
			ToolAry[1].fsStyle   = TBSTYLE_BUTTON;
			ToolAry[1].dwData    = 0L;
			ToolAry[1].iString   = 0;
			hToolbar=CreateToolbarEx(hWndTool,
									 WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | WS_BORDER,
									 IDC_TOOLBAR, 2,
									 hInstDll, IDB_TOOLSERV, ToolAry, 2,
									 0, 0, 46, 16,
									 sizeof(TBBUTTON));
			// BUG: les fenêtres XP sont trop courte //
			GetWindowRect(hToolbar,&ToolRect);
			GetWindowRect(hWndTool,&WndRect);
			if(WndRect.bottom<ToolRect.bottom) MoveWindow(hWndTool,ToolRect.left,ToolRect.top,100,59,TRUE);
			//
			return 0;
		}
		case WM_COMMAND: 
		{	switch(LOWORD(wParamTool))
			{	case ID_CANCEL:
				{	bStockTask=FALSE;
					mHelpOpen(hWndTool);
					break;
				}
				case ID_STOCK:
				{	if((!mHpCtrl_IsEmptyListe(NULL))||(mHpPak_ReadOperationKey()!=2))
					{   bStockFromDlg=TRUE;
						bStockTask=TRUE;
                        mHelpOpen(hWndTool);
					}
					break;
				}
			}
			return 0;
		}
		case WM_NOTIFY:
		{	if((((LPNMHDR)lParamTool)->code)==TTN_NEEDTEXT)
			{	char lpmHpRC[255]={0};
				LPTOOLTIPTEXT ToolTip = LPTOOLTIPTEXT((LPNMHDR)lParamTool);
				switch(wParamTool)
				{	case ID_CANCEL:
					{	LoadString(hInstDll,IDS_CANCEL,lpmHpRC,255);
						ToolTip->lpszText=lpmHpRC;
						break;
					}
					case ID_STOCK:
					{	LoadString(hInstDll,IDS_SAVE,lpmHpRC,255);
						ToolTip->lpszText=lpmHpRC;
						break;
					}
				}
			}
			break;
		}
		case WM_CLOSE:
		{	DestroyWindow(hWndTool);
			UnregisterClass(MHPSERV_CLASS,hInstTool);
			return 0;
		}
		case WM_DESTROY:
		{   return 0;}
	}
	return (HRESULT)DefWindowProc(hWndTool,MessageID,wParamTool,lParamTool);
}

//************************************************************************************************************MHP_WNDPROC//
// mHpWndProc
//-----------------------------------------------------//
LRESULT CALLBACK mHpWndProc(HWND hWndActive,UINT Message,WPARAM wParam,LPARAM lParam)
{	LONG_PTR lWindowProc=WndLst->GetWndProc(hWndActive);
    switch(mHpCtrl_WndProc(hWndActive,Message,wParam,lParam,hWndToolServ,mHpCtrl->mHpEventList,AppName,TRUE))
	{	case 0:
		{	return 0;}
		case 1:
		{	GetHelpComStruct()->bNoApp=TRUE;
			mHelpOpen(hWndActive);
			break;
		}
		case 2: // WA_INACTIVE
		{	KillTimer(hWndActive,ID_TIMER_CHANGE_WND);
			SetTimer(hWndActive,ID_TIMER_CHANGE_WND,100,(TIMERPROC)mHpServTimerProc);
			break;
		}
		case 3: // WM_NCDESTROY
		{	char lpTempClass[MAX_RS_SIZE]={0};
			char lpTempTitle[MAX_RS_SIZE]={0};
			KillTimer(hWndActive,ID_TIMER_CHANGE_WND);
			GetClassName(hWndActive,lpTempClass,MAX_RS_SIZE);
			GetWindowText(hWndActive,lpTempTitle,MAX_RS_SIZE);
			mHpPak_ProcWndLst->InactiveWnd(AppName,lpTempClass,lpTempTitle);
			if((mHpCtrl_bWritemHelp)||(mHpCtrl_bReadmHelp)) mHpPak_ProcWndLst->WriteAppWndKey();
			WndLst->RemoveWnd(hWndActive);
			if((!RealWndLst->InactiveWnd(AppName,lpTempClass,lpTempTitle))|| // Always == NO -> lpTempTitle == ""
			   (hWndMain==hWndActive))
			{	mHpPak_SetAppInfoKeys(AppName,0,0);
				mHpCtrl->DeleteTaskList();
				mHpCtrl->DeleteEventList();
				WndLst->EmptyWndList();
				FrameLst->EmptyWndList();
				mHpPak_ProcWndLst->EmptyWndLst();
				RealWndLst->EmptyWndLst();
				mHpPak_LnchFavLst->EmptyFavList();
				mHpPak_CustomFavLst->EmptyFavList();
				mHpPak_UnEventList->EmptyUnEvent();
				if(GetHelpComStruct()->mHpDlgCreatedObj)
				{	mHpPak_ItemsList->EmptyList();
					mHpPak_FavsList->EmptyFavList();
					GetHelpComStruct()->ModifList->EmptyModifList();
					GetHelpComStruct()->listBtn->EmptyButtonList();
					GetHelpComStruct()->mHpFileList->EmptymHpFileList();
					delete(mHpPak_ItemsList);
					delete(mHpPak_FavsList);
					delete(GetHelpComStruct()->ModifList);
					delete(GetHelpComStruct()->listBtn);
					delete(GetHelpComStruct()->mHpFileList);
				}
				if(GetHelpComStruct()->ServDlgCreatedObj)
				{	GetHelpComStruct()->AddList->EmptyItmList();
					delete(GetHelpComStruct()->AddList);
				}
				mHpCtrl_FreeCtrlMgmtList();
				mHpCtrl_FreemHpStruct();
				mHpPak_DeleteSecAttStructs();
				FreeHelpComStruct();
                CoUninitialize(); // Uninitialize COM (mHpNET)
				delete(WndLst);
				delete(FrameLst);
				delete(mHpPak_ProcWndLst);
                delete(mHpPak_ProcCtrlList);
				delete(RealWndLst);
				delete(mHpPak_LnchFavLst);
				delete(mHpPak_CustomFavLst);
				delete(mHpPak_UnEventList);
				delete(mHpCtrl);
			}
			break;
		}
		case 4: // WA_ACTIVE
		{	KillTimer(hWndActive,ID_TIMER_CHANGE_WND);
			SetTimer(hWndActive,ID_TIMER_CHANGE_WND,100,(TIMERPROC)mHpServTimerProc);
			break;
		}
		case 5: // WM_PARENTNOTIFY - WM_CREATE
        {   SetFrameCtrlProc(hWndActive);

            ////// Implement Qt, WPF & Java controls
            mHpCtrl_SetPlatformCtrlNotify(hWndActive); // TODO: Check if needed?
                                                       // -> May this never occured for Qt, WPF & Java applications!
			break;
		}
        /*case 7: // WM_COMMAND | WM_NOTIFY (No notify DotNet control message)
        {   return 0;
        }*/
		default:
		{	break;}
	}
	// BUG : hWndActive not in WndLst //
	if(!lWindowProc) return 0;
	/////////////////////////////////////
	return CallWindowProc((WNDPROC)lWindowProc,hWndActive,Message,wParam,lParam);
}

//-----------------------------------------------------//
// mHpFrameProc
//-----------------------------------------------------//
LRESULT CALLBACK mHpFrameProc(HWND hWndFrame,UINT Message,WPARAM wParam,LPARAM lParam)
{	LONG_PTR lFrameProc=FrameLst->GetWndProc(hWndFrame);
	switch(mHpCtrl_WndProc(hWndFrame,Message,wParam,lParam,hWndToolServ,mHpCtrl->mHpEventList,AppName,FALSE))
	{	case 3: // WM_NCDESTROY
		{	FrameLst->RemoveWnd(hWndFrame);
			break;
		}
		case 5: // WM_PARENTNOTIFY - WM_CREATE
		{   SetFrameCtrlProc(hWndFrame);

            ////// Implement Qt, WPF & Java controls
            mHpCtrl_SetPlatformCtrlNotify(hWndFrame); // TODO: Check if needed?
                                                      // -> May this never occured for Qt, WPF & Java applications!
			break;
		}
        /*case 7: // WM_COMMAND | WM_NOTIFY (No notify DotNet control message)
        {   return 0;
        }*/
		default:
		{	break;}
	}
	// BUG : hWndFrame not in FrameLst //
	if(!lFrameProc) return 0;
	/////////////////////////////////////
	return CallWindowProc((WNDPROC)lFrameProc,hWndFrame,Message,wParam,lParam);
}

//-----------------------------------------------------//
// mHpServTimerProc
//-----------------------------------------------------//
void CALLBACK mHpServTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{	if(uMsg==WM_TIMER)
	{	switch(idEvent)
		{	case ID_TIMER_TOOLSERV:
            {   KillTimer(hwnd,ID_TIMER_TOOLSERV);
                CreateToolServ(hwnd);
                break;
            }
            case ID_TIMER_WRITE_SHARED:
			{	KillTimer(hwnd,ID_TIMER_WRITE_SHARED);
				CreateMainDlg(hwnd,FALSE,TRUE);
				break;
			}
			case ID_TIMER_READ_SHARED:
			{	KillTimer(hwnd,ID_TIMER_READ_SHARED);
				CreateMainDlg(hwnd,TRUE,TRUE);
				break;
			}
			case ID_TIMER_CHANGE_WND:
			{	HWND hActiveWnd;
				int iAddRes=0;
				int iCurTmp=0,iCntTmp=0,iLastCnt;
				KillTimer(hwnd,ID_TIMER_CHANGE_WND);
				hActiveWnd=GetActiveWindow();
				if(hActiveWnd!=NULL)
				{	iAddRes=WndLst->AddWnd(hActiveWnd,(LONG_PTR)mHpWndProc);
					if((iAddRes!=0)&&(!mHpPak_bLockChangeWnd))
					{	RealWndLst->UpdateWndLst(AppName,hActiveWnd,iCurTmp,iCntTmp);
						if(mHpCtrl_bWritemHelp)
						{	mHpCtrl_iLastID=NO;
							mHpPak_ProcWndLst->ReadAppWndKey();
							mHpPak_ProcWndLst->UpdateWndLst(AppName,hActiveWnd,mHpCtrl_iCurrentWnd,mHpCtrl_iCountWnd);
							mHpPak_ProcWndLst->WriteAppWndKey();
							SetFrameCtrlProc(hActiveWnd);

                            ////// Implement Qt, WPF & Java controls
                            mHpCtrl_SetPlatformCtrlNotify(hActiveWnd); // TODO: Check if needed?
                                                                       // -> May this never occured for Qt, WPF & Java applications!

							EnableWindow(hWndToolServ,TRUE);
							mHpCtrl_ChangeEventWnd(mHpCtrl->mHpEventList,AppName);
							mHpCtrl_SetTimer(hActiveWnd,1,6); // NEXT
						}
						else if(mHpCtrl_bReadmHelp)
						{	if(mHpCtrl_bChangeWnd)
							{	mHpCtrl_KillTimer(hActiveWnd,3); // WAIT
                                bool bNewWnd=false;
                                iLastCnt=mHpCtrl_iCountWnd;
								mHpPak_ProcWndLst->UpdateWndLst(AppName,hActiveWnd,mHpCtrl_iCurrentWnd,mHpCtrl_iCountWnd);
								if((iLastCnt==mHpCtrl_iCountWnd)&&(iCntTmp!=0))
                                {   mHpCtrl_iCountWnd++;
                                    bNewWnd=true;
                                }
								SetFrameCtrlProc(hActiveWnd);

                                ////// Implement Qt, WPF & Java controls
                                mHpCtrl_SetPlatformCtrlNotify(hActiveWnd); // TODO: Check if needed?
                                                                           // -> May this never occured for Qt, WPF & Java applications!

								mHpPak_ToolTipsActivate(FALSE);
                                // BUG: Wrong window opacity when using Direct2D with alpha mode
                                if(bNewWnd)
                                {   SetWindowLongPtr(hActiveWnd,GWL_EXSTYLE,GetWindowLongPtr(hwnd,GWL_EXSTYLE)&~WS_EX_LAYERED);
                                    SetLayeredWindowAttributes(hActiveWnd,0,255,LWA_ALPHA);
                                    RedrawWindow(hActiveWnd,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
                                    SetTimer(hwnd,ID_TIMER_VISIBLE,400,(TIMERPROC)mHpServTimerProc);
                                    ////////////////////////////////////////////////////////////
                                }
                                else
                                    mHpCtrl_NextStepProc(hActiveWnd,FALSE,AppName);
							}
							else mHpCtrl_ViewDiffWndMsg(hActiveWnd);
						}
					}
					else mHpPak_bLockChangeWnd=FALSE;
				}
				break;
			}
            case ID_TIMER_VISIBLE:
            {   KillTimer(hwnd,ID_TIMER_VISIBLE);
                mHpCtrl_NextStepProc(GetActiveWindow(),FALSE,AppName);
                break;
            }
		}
	}
}

//*************************************************************************************************************MHP_EXPORT//
// mHelpInit
//-----------------------------------------------------//
extern "C" void MHELP_API mHelpInit(HWND hWndActive,const char* lpExeName,bool bSharedApp)
{	int iAppStat=0,imHpStat=0;
    BOOL bRes;
    ////// TODO: Remove following code
    char lpNameExe[128];
    strncpy(lpNameExe,lpExeName,128);
    //////
	if(!bInitPass)
	{	char lpclass[MAX_RS_SIZE]={0};
	    char lptitle[MAX_RS_SIZE]={0};
	    CoInitialize(NULL); // Initialize COM (mHpNET)
		mHpCtrl=new CmHpCtrl();
		WndLst=new CWndListe();
		FrameLst=new CWndListe();
        mHpPak_ProcWndLst=new CWindowListe();
        mHpPak_ProcCtrlList=new CControlList();
		mHpPak_LnchFavLst=new CFavListe();
		mHpPak_CustomFavLst=new CFavListe();
		RealWndLst=new CWindowListe();
		mHpPak_UnEventList=new CUnEventListe();
		GetClassName(hWndActive,lpclass,MAX_RS_SIZE);
		GetWindowText(hWndActive,lptitle,MAX_RS_SIZE);
		// FreeHelpComStruct -> OK
		GetHelpComStruct()->hInstServ=hInstDll;
		strncpy(AppName,lpNameExe,MAX_RS_SIZE);
		GetExeName(lpNameExe);
		bStockTask=FALSE;
		bStockFromDlg=FALSE;
		mHpPak_bLockChangeWnd=FALSE;
		GetHelpComStruct()->bNoApp=FALSE;
		int iOperation=mHpPak_ReadOperationKey();
		// mHpCtrl_FreemHpStruct -> OK
		mHpCtrl_GethWndActive(hWndActive);
        mHpCtrl_AddCommonUnEvnt();
        mHpCtrl_CreateCtrlMgmtList(hWndActive);
		mHpCtrl->NewTaskList();
		mHpCtrl->NewEventList();
		GetHelpComStruct()->mHpCtrlObj=mHpCtrl;
		RealWndLst->AddWndInfo(AppName,lpclass,lptitle,TRUE);
		hWndMain=hWndActive;
		mHpPak_SetAppInfoKeys(lpNameExe,1,0);
		// if(!bPass) => CreateMainDlg
		WndLst->AddWnd(hWndActive,(LONG_PTR)mHpWndProc);
		SetFrameCtrlProc(hWndActive);

        ////// Implement Qt, WPF & Java controls
        mHpCtrl_SetPlatformCtrlNotify(hWndActive);

		/*if(mHpPak_IsQtHwnd(hWndActive))
			mHpPak_AddQtCtrlsEvent(hWndActive);*/
		switch(iOperation)
		{	case 0: // No operation
			{   mHpPak_ProcWndLst->AddWndInfo(AppName,lpclass,lptitle,TRUE);
				mHpPak_WriteExePathKey(AppName);
				if(IsAutoAssCfg()) CreateMainDlg(hWndActive,TRUE,FALSE);
				break;
			}
			case 1: // Process operation
			{   mHpPak_SetAppInfoKeys(lpNameExe,1,3);
				mHpPak_ProcWndLst->ReadAppWndKey();
				mHpCtrl_ReadTaskKeys(hWndActive,mHpCtrl->mHpTaskList,AppName,TRUE);
				CreateManageObj();
				mHpCtrl_HideLockCursor(hWndActive);
				SetTimer(hWndActive,ID_TIMER_READ_SHARED,1000,(TIMERPROC)mHpServTimerProc);
				break;
			}
			case 2: // Create operation
			{	if(!bSharedApp)
				{	mHpPak_ProcWndLst->AddWndInfo(AppName,lpclass,lptitle,TRUE);
					mHpPak_WriteExePathKey(AppName);
				}
				else
				{	mHpPak_SetAppInfoKeys(lpNameExe,1,2);
					mHpPak_ProcWndLst->ReadAppWndKey();
					mHpPak_ProcWndLst->AddWndInfo(AppName,lpclass,lptitle,TRUE);
					mHpPak_ProcWndLst->WriteAppWndKey();
					mHpPak_WriteExePathKey(AppName);
					iCurProcID=mHpCtrl_ReadEventKeys(mHpCtrl->mHpEventList,TRUE);
					CreateManageObj();
					SetTimer(hWndActive,ID_TIMER_WRITE_SHARED,1000,(TIMERPROC)mHpServTimerProc);
				}
				break;
			}
		}
		bInitPass=TRUE;
	}
	else
	{	mHpPak_GetNextAppInfoKeys(NO,lpNameExe,0,iAppStat,imHpStat,bRes);
		mHpPak_SetAppInfoKeys(lpNameExe,1,imHpStat);
	}
}

//-----------------------------------------------------//
// mHelpOpen
//-----------------------------------------------------//
extern "C" void MHELP_API mHelpOpen(HWND hWndActive)
{	BOOL bContinue=FALSE;
	if(bInitPass)
	{	if(!mHpCtrl_bWritemHelp) CreateMainDlg(hWndActive,FALSE,FALSE);
		else
		{	mHpCtrl_KillTimer(hWndActive,6); // NEXT
			mHpCtrl_bReadmHelp=FALSE;
			mHpCtrl_bWritemHelp=FALSE;
			if(bStockTask)
			{	switch(mHpPak_ReadOperationKey())
				{	case 2:
					{	mHpCtrl_AddIfChangeExe(mHpCtrl->mHpEventList,AppName);
						if((!bStockFromDlg)&&(!GetHelpComStruct()->bNoApp)&&(!mHpCtrl_IsEmptyListe(mHpCtrl->mHpEventList)))
						{	mHpCtrl_RemoveLastTask(mHpCtrl->mHpEventList);
						}
						if(!mHpCtrl_IsEmptyListe(mHpCtrl->mHpEventList))
						{	ShowWindow(hWndToolServ,SW_HIDE);
							if(iCurProcID==mHpPak_ReadmHpProcIDKey())
							{	mHpCtrl_WriteOperationKey(3);
								switch(CreateServerDlg(hWndActive))
								{	case NO:
									{	mHpPak_mHpMessage(hWndActive,hInstDll,IDS_CREATE_MHPDLG,NULL,0,0,MB_ICONSTOP);
										break;
									}
									case IDC_LAST:
									{	mHpCtrl_bUserAction=TRUE;
										mHpCtrl_bWritemHelp=TRUE;
										bContinue=TRUE;
										break;
									}
									case IDCANCEL:
									{	mHpCtrl_EmptyEvents(mHpCtrl->mHpEventList);
										break;
									}
								}
							}
							else
							{	mHpPak_mHpMessage(hWndActive,hInstDll,IDS_STOCK_FINISHED,NULL,0,0,MB_ICONEXCLAMATION);
								mHpCtrl_EmptyEvents(mHpCtrl->mHpEventList);
							}
						}
						else
						{	mHpCtrl_bUserAction=TRUE;
							mHpCtrl_bWritemHelp=TRUE;
							bContinue=TRUE;
						}
						break;
					}
					case 3:
					{	mHpPak_mHpMessage(hWndActive,hInstDll,IDS_EVENT_CUSTOMING,NULL,0,0,MB_ICONEXCLAMATION);
						mHpCtrl_bWritemHelp=TRUE;
						bStockFromDlg=FALSE;
						return;
					}
					default:
					{	mHpPak_mHpMessage(hWndActive,hInstDll,IDS_STOCK_FINISHED,NULL,0,0,MB_ICONEXCLAMATION);
						mHpCtrl_EmptyEvents(mHpCtrl->mHpEventList);
						mHpPak_ClearmHpSharedKey();
						break;
					}
				}
			}
			else
			{	switch(mHpPak_ReadOperationKey())
				{	case 3:
					{	mHpPak_mHpMessage(hWndActive,hInstDll,IDS_EVENT_CUSTOMING,NULL,0,0,MB_ICONEXCLAMATION);
						mHpCtrl_bWritemHelp=TRUE;
						bStockFromDlg=FALSE;
						return;
					}
					default:
					{	mHpCtrl_EmptyEvents(mHpCtrl->mHpEventList);
						if(iCurProcID==mHpPak_ReadmHpProcIDKey()) mHpPak_ClearmHpSharedKey();
						break;
					}
				}
			}
			if(!bContinue)
			{	SetActiveWindow(GetParent(hWndToolServ));
				SendMessage(hWndToolServ,WM_CLOSE,0,0);
				mHpPak_SetAppInfoKeys(AppName,1,0);
				mHpCtrl_bLockmHelp=FALSE;
			}
			else
			{	SetActiveWindow(GetParent(hWndToolServ));
				ShowWindow(hWndToolServ,SW_SHOW);
				SetActiveWindow(GetParent(hWndToolServ));
			}
			bStockFromDlg=FALSE;
		}
	}
	else mHpPak_mHpMessage(hWndActive,hInstDll,IDS_MUST_CALL_MAINACTIVE,NULL,0,0,MB_ICONEXCLAMATION);
}

//-----------------------------------------------------//
// mHelp_AddUnSupport
//-----------------------------------------------------//
/*extern "C" int MHELP_API mHelp_AddUnSupport(char* lpAddClass)
{	if(bInitPass)
	{	mHpPak_UnEventList->AddUnEvent(lpAddClass);
		return 0;
	}
	return 3;
}*/

//-----------------------------------------------------//
// mHelpErrorMsg
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpErrorMsg(int imHpError,char* lpBuffer,int iMaxBuffer)
{	switch(imHpError)
	{	case 0: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_NOPROBLEM,lpBuffer,iMaxBuffer);
		case 1: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_READPROCESS,lpBuffer,iMaxBuffer);
		case 2: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_WRITEPROCESS,lpBuffer,iMaxBuffer);
		case 3: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_NOINITPROC,lpBuffer,iMaxBuffer);
		case 4: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_MHPFILEPATH,lpBuffer,iMaxBuffer);
		case 5: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_FAVINDEX,lpBuffer,iMaxBuffer);
		case 6: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_PROCESSKEY,lpBuffer,iMaxBuffer);
		case 7: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_FILECORRUPT,lpBuffer,iMaxBuffer);
		case 8: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_CUSTOMINDEX,lpBuffer,iMaxBuffer);
		case 9: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_PARAMINDEX,lpBuffer,iMaxBuffer);
		case 10: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_REPEATOPT,lpBuffer,iMaxBuffer);
		case 11: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_INTTYPE,lpBuffer,iMaxBuffer);
		case 12: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_REPEATINDEX,lpBuffer,iMaxBuffer);
		case 13: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_SELINDEX,lpBuffer,iMaxBuffer);
		case 14: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_INTSELECT,lpBuffer,iMaxBuffer);
		case 15: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_NEWINTVAL,lpBuffer,iMaxBuffer);
		case 16: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_NEWBOOLVAL,lpBuffer,iMaxBuffer);
		case 17: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_INTLISTTYPE,lpBuffer,iMaxBuffer);
		case 18: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_NOREMOVESEL,lpBuffer,iMaxBuffer);
		case 19: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_STRTYPE,lpBuffer,iMaxBuffer);
		case 20: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_STRSELECT,lpBuffer,iMaxBuffer);
		case 21: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_STRLISTTYPE,lpBuffer,iMaxBuffer);
		case 22: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_NEWSTRVAL,lpBuffer,iMaxBuffer);
		case 23: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_SELECTINT,lpBuffer,iMaxBuffer);
		case 24: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_SELECTSTR,lpBuffer,iMaxBuffer);
		case 25: return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_NOSELECTION,lpBuffer,iMaxBuffer);
		default:
		{	return LoadString(GetHelpComStruct()->hInstServ,IDS_ERROR_NOMHPERROR,lpBuffer,iMaxBuffer);
		}
	}
}

//-----------------------------------------------------//
// mHelpLaunchFavTask
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpLaunchFavTask(HWND hWndActive,char* lpmHpFile,int iFavIndex)
{	int index=0,imHpError=0;
	if(bInitPass)
	{	switch(mHpPak_ReadOperationKey())
		{	case 0:
			{	mHpPak_ViewMsg(FALSE);
				if((lpmHpFile!=NULL)&&(mHpPak_IsFilePathExist(hWndActive,lpmHpFile)))
				{	if(mHpPak_GetFavoritesFile(hWndActive,lpmHpFile,mHpPak_LnchFavLst))
					{	if((iFavIndex>=0)&&((mHpPak_LnchFavLst->GetFavCount()-1)>=iFavIndex))
						{	mHpPak_iFavIndex=iFavIndex;
							mHpPak_bLnchProc=TRUE;
							GetHelpComStruct()->iMode=1;
							GetHelpComStruct()->iNbBoucle=mHpPak_LnchFavLst->GetLoopOption(iFavIndex);
							mHpPak_EmptyTab(GetHelpComStruct()->IndexTab,MAX_PART_TREE);
							while(mHpPak_LnchFavLst->GetItemFromRang(iFavIndex)->FavItem->iRefTab[index]!=NO)
							{	GetHelpComStruct()->IndexTab[index]=mHpPak_LnchFavLst->GetItemFromRang(iFavIndex)->FavItem->iRefTab[index];
								index++;
							}
							if(mHpPak_LnchFavLst->GetItemFromRang(iFavIndex)->FavItem->ParamList!=NULL)
							{	GetHelpComStruct()->IndexTab[index]=0;
								index++;
							}
							GetHelpComStruct()->IndexTab[index]=NO;
							CreateMainDlg(hWndActive,TRUE,FALSE);
						}
						else imHpError=5;
					}
					else imHpError=7;
				}
				else imHpError=4;
				break;
			}
			case 1:
			{	imHpError=1;
				break;
			}
			case 2:
			{	imHpError=2;
				break;
			}
			default:
			{	imHpError=6;
				break;
			}
		}
	}
	else imHpError=3;
	mHpPak_ViewMsg(TRUE);
	return imHpError;
}

//-----------------------------------------------------//
// mHelpCreateCustomTask
//-----------------------------------------------------//
extern "C" void MHELP_API mHelpCreateCustomTask(HWND hWndActive,char* lpmHpFile,int iFavIndex,int &iCustomID,int& imHpError)
{	CFavListe* TmpFavLst;
	CEvoluList* CustomEvol=NULL;
	imHpError=0;
	iCustomID=NO;
	if(bInitPass)
	{	mHpPak_ViewMsg(FALSE);
		if((lpmHpFile!=NULL)&&(mHpPak_IsFilePathExist(hWndActive,lpmHpFile)))
		{	TmpFavLst=new CFavListe();
			if(mHpPak_GetFavoritesFile(hWndActive,lpmHpFile,TmpFavLst))
			{	if((iFavIndex>=0)&&((TmpFavLst->GetFavCount()-1)>=iFavIndex))
				{	if(TmpFavLst->GetItemFromRang(iFavIndex)->FavItem->ParamList!=NULL)
					{	CustomEvol=new CEvoluList();
						CustomEvol->CopyEvoluLst(TmpFavLst->GetItemFromRang(iFavIndex)->FavItem->ParamList);
					}
					mHpPak_CustomFavLst->AddFavItem(TmpFavLst->GetItemFromRang(iFavIndex)->FavItem->iRefTab,
												 TmpFavLst->GetLoopOption(iFavIndex),
												 TmpFavLst->GetmHpPath(iFavIndex),
												 TmpFavLst->GetEventStr(iFavIndex),
												 CustomEvol,FALSE);
					iCustomID=iCurCustom;
					iCurCustom++;
				}
				else imHpError=5;
			}
			else imHpError=7;
		}
		else imHpError=4;
	}
	else imHpError=3;
	mHpPak_ViewMsg(TRUE);
}

//-----------------------------------------------------//
// mHelpLnchCustomTask
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpLnchCustomTask(HWND hWndActive,int iCustomID)
{	int index=0,imHpError=0;
	if(bInitPass)
	{	switch(mHpPak_ReadOperationKey())
		{	case 0:
			{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
				{	mHpPak_iFavIndex=iCustomID;
					mHpPak_bLnchCustom=TRUE;
					GetHelpComStruct()->iMode=1;
					GetHelpComStruct()->iNbBoucle=mHpPak_CustomFavLst->GetLoopOption(iCustomID);
					mHpPak_EmptyTab(GetHelpComStruct()->IndexTab,MAX_PART_TREE);
					while(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->iRefTab[index]!=NO)
					{	GetHelpComStruct()->IndexTab[index]=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->iRefTab[index];
						index++;
					}
					if(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList!=NULL)
					{	GetHelpComStruct()->IndexTab[index]=0;
						index++;
					}
					GetHelpComStruct()->IndexTab[index]=NO;
					CreateMainDlg(hWndActive,TRUE,FALSE);
				}
				else imHpError=8;
				break;
			}
			case 1:
			{	imHpError=1;
				break;
			}
			case 2:
			{	imHpError=2;
				break;
			}
			default:
			{	imHpError=6;
				break;
			}
		}
	}
	else imHpError=3;
	return imHpError;
}

//-----------------------------------------------------//
// mHelpGetParamCnt
//-----------------------------------------------------//
extern "C" void MHELP_API mHelpGetParamCnt(int iCustomID,int &iParamCnt,int &imHpError)
{	imHpError=0;
	iParamCnt=NO;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList!=NULL)
			{	iParamCnt=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelCount();}
			else iParamCnt=0;
		}
		else imHpError=8;
	}
	else imHpError=3;	
}

//-----------------------------------------------------//
// mHelpGetParamType
//-----------------------------------------------------//
extern "C" void MHELP_API mHelpGetParamType(int iCustomID,int iParamIndex,int &iParamType,int &imHpError)
{	imHpError=0;
	iParamType=NO;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	iParamType=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex);
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
}

//-----------------------------------------------------//
// mHelpGetRepeatOpt
//-----------------------------------------------------//
extern "C" void MHELP_API mHelpGetRepeatOpt(int iCustomID,int &iRepeatOpt,int &imHpError)
{	imHpError=0;
	iRepeatOpt=NO;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	iRepeatOpt=mHpPak_CustomFavLst->GetLoopOption(iCustomID);
		}
		else imHpError=8;
	}
	else imHpError=3;	
}

//-----------------------------------------------------//
// mHelpSetRepeatOpt
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpSetRepeatOpt(int iCustomID,int iNewRepeat)
{	int imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iNewRepeat>=1)&&(iNewRepeat<100))
			{	if(iNewRepeat!=mHpPak_CustomFavLst->GetLoopOption(iCustomID))
				{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->RefreshLoopOpt(iNewRepeat);}
			}
			else imHpError=10;
		}
		else imHpError=8;
	}
	else imHpError=3;	
	return imHpError;
}

//-----------------------------------------------------//
// mHelpGetIntParamVal
//-----------------------------------------------------//
extern "C" void MHELP_API mHelpGetIntParamVal(int iCustomID,int iParamIndex,int iRepeatIndex,int iSelIndex,int &iParamVal,int &imHpError)
{	int iSel;
	imHpError=0;
	iParamVal=NO;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	if((iRepeatIndex>=0)&&(iRepeatIndex<=(mHpPak_CustomFavLst->GetLoopOption(iCustomID)-1)))
				{	iSel=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelItem(iParamIndex);
					switch(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex))
					{	case 0: // I
						case 3: // B
						{	iParamVal=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetIntParam(iRepeatIndex,0);
							break;
						}
						case 5: // LI
						case 8: // LB
						{	if(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)!=0)
							{	if((iSelIndex>=0)&&(iSelIndex<=(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)-1)))
								{	iParamVal=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetIntParam(iRepeatIndex,iSelIndex);
								}
								else imHpError=13;
							}
							else
							{	iParamVal=NO;
								if(iSelIndex>0) imHpError=25;
							}
							break;
						}
						case 1: // R
						case 2: // C
						case 4: // S
						case 6: // LR
						case 7: // LC
						case 9: // LS
						{	imHpError=11;
							break;
						}
					}
				}
				else imHpError=12;
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
}

//-----------------------------------------------------//
// mHelpSetIntParamVal
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpSetIntParamVal(int iCustomID,int iParamIndex,int iRepeatIndex,int iNewVal)
{	int iSel,imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	if((iRepeatIndex>=0)&&(iRepeatIndex<=(mHpPak_CustomFavLst->GetLoopOption(iCustomID)-1)))
				{	iSel=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelItem(iParamIndex);
					switch(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex))
					{	case 0: // I
						{	if(iNewVal>=0)
							{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->SetIntParam(iRepeatIndex,NO,0,iNewVal);
							}
							else imHpError=15;
							break;
						}
						case 3: // B
						{	if((iNewVal>=0)&&(iNewVal<=3))
							{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->SetIntParam(iRepeatIndex,NO,3,iNewVal);
							}
							else imHpError=16;
							break;
						}
						case 5: // LI
						case 8: // LB
						{	imHpError=14;
							break;
						}
						case 1: // R
						case 2: // C
						case 4: // S
						case 6: // LR
						case 7: // LC
						case 9: // LS
						{	imHpError=11;
							break;
						}
					}
				}
				else imHpError=12;
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
	return imHpError;
}

//-----------------------------------------------------//
// mHelpAddIntParamVal
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpAddIntParamVal(int iCustomID,int iParamIndex,int iRepeatIndex,int iAddVal)
{	//CType* NewIntSel;
	int iSel,imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	if((iRepeatIndex>=0)&&(iRepeatIndex<=(mHpPak_CustomFavLst->GetLoopOption(iCustomID)-1)))
				{	iSel=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelItem(iParamIndex);
					switch(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex))
					{	case 0: // I
						case 3: // B
						{	imHpError=23;
							break;
						}
						case 5: // LI
						{	if(iAddVal>=0)
							{	// SetRepeatOpt < Last -> BUG "EmptyTypeList()"
								//NewIntSel=new CIntType(iAddVal);
								//mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->AddType(NewIntSel);
								mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->AddIntTypeNoEmptyBug(iAddVal);
							}
							else imHpError=15;
							break;
						}
						case 8: // LB
						{	if((iAddVal>=0)&&(iAddVal<=2))
							{	// SetRepeatOpt < Last -> BUG "EmptyTypeList()"
								//NewIntSel=new CIntType(iAddVal);
								//mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->AddType(NewIntSel);
								mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->AddIntTypeNoEmptyBug(iAddVal);
							}
							else imHpError=16;
							break;
						}
						case 1: // R
						case 2: // C
						case 4: // S
						case 6: // LR
						case 7: // LC
						case 9: // LS
						{	imHpError=17;
							break;
						}
					}					
				}
				else imHpError=12;
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
	return imHpError;
}

//-----------------------------------------------------//
// mHelpRemoveIntParamVal
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpRemoveIntParamVal(int iCustomID,int iParamIndex,int iRepeatIndex,int iSelIndex)
{	int iSel,imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	if((iRepeatIndex>=0)&&(iRepeatIndex<=(mHpPak_CustomFavLst->GetLoopOption(iCustomID)-1)))
				{	iSel=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelItem(iParamIndex);
					if(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)!=0)
					{	switch(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex))
						{	case 0: // I
							case 3: // B
							{	imHpError=23;
								break;
							}
							case 5: // LI
							{	if((iSelIndex>=0)&&(iSelIndex<=(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)-1)))
								{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->RemoveType(iSelIndex);
								}
								else imHpError=13;
								break;
							}
							case 8: // LB
							{	if((iSelIndex>=0)&&(iSelIndex<=(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)-1)))
								{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->RemoveType(iSelIndex);
								}
								else imHpError=13;
								break;
							}
							case 1: // R
							case 2: // C
							case 4: // S
							case 6: // LR
							case 7: // LC
							case 9: // LS
							{	imHpError=17;
								break;
							}
						}
					}
					else imHpError=18;
				}
				else imHpError=12;
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
	return imHpError;
}

//-----------------------------------------------------//
// mHelpGetStrParamVal
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpGetStrParamVal(int iCustomID,int iParamIndex,int iRepeatIndex,int iSelIndex,char* lpParamVal,int iMaxStrVal)
{	char lpVal[MAX_RS_SIZE]={0};
	int indice=0,iSel,imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	if((iRepeatIndex>=0)&&(iRepeatIndex<=(mHpPak_CustomFavLst->GetLoopOption(iCustomID)-1)))
				{	iSel=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelItem(iParamIndex);
					switch(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex))
					{	case 0: // I
						case 3: // B
						case 5: // LI
						case 8: // LB
						{	imHpError=19;
							break;
						}
						case 1: // R
						case 2: // C
						case 4: // S
						{	mHpPak_InitStr(lpParamVal,iMaxStrVal);
							strncpy(lpVal,mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetStrParam(iRepeatIndex,0),MAX_RS_SIZE);
							while((lpVal[indice]!=END_LIST)&&(indice!=iMaxStrVal))
							{	lpParamVal[indice]=lpVal[indice];
								indice++;
							}
							break;
						}
						case 6: // LR
						case 7: // LC
						case 9: // LS
						{	if(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)!=0)
							{	if((iSelIndex>=0)&&(iSelIndex<=(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)-1)))
								{	mHpPak_InitStr(lpParamVal,iMaxStrVal);
									strncpy(lpVal,mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetStrParam(iRepeatIndex,iSelIndex),MAX_RS_SIZE);
									while((lpVal[indice]!=END_LIST)&&(indice!=iMaxStrVal))
									{	lpParamVal[indice]=lpVal[indice];
										indice++;
									}
								}
								else imHpError=13;
							}
							else
							{	mHpPak_InitStr(lpParamVal,iMaxStrVal);
								if(iSelIndex>0) imHpError=25;
							}
							break;
						}
					}
				}
				else imHpError=12;
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
	return imHpError;
}

//-----------------------------------------------------//
// mHelpSetStrParamVal
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpSetStrParamVal(int iCustomID,int iParamIndex,int iRepeatIndex,char* lpNewVal)
{	int iSel,imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	if((iRepeatIndex>=0)&&(iRepeatIndex<=(mHpPak_CustomFavLst->GetLoopOption(iCustomID)-1)))
				{	iSel=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelItem(iParamIndex);
					switch(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex))
					{	case 0: // I
						case 3: // B
						case 5: // LI
						case 8: // LB
						{	imHpError=19;
							break;
						}
						case 1: // R
						{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->SetStrParam(iRepeatIndex,NO,1,lpNewVal);
							break;
						}
						case 2: // C
						{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->SetStrParam(iRepeatIndex,NO,2,lpNewVal);
							break;
						}
						case 4: // S
						{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->SetStrParam(iRepeatIndex,NO,4,lpNewVal);
							break;
						}
						case 6: // LR
						case 7: // LC
						case 9: // LS
						{	imHpError=20;
							break;
						}
					}
				}
				else imHpError=12;
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
	return imHpError;
}

//-----------------------------------------------------//
// mHelpAddStrParamVal
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpAddStrParamVal(int iCustomID,int iParamIndex,int iRepeatIndex,char* lpAddVal,int iMaxAddVal)
{	//CType* NewStrSel;
	int iSel,imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	if((iRepeatIndex>=0)&&(iRepeatIndex<=(mHpPak_CustomFavLst->GetLoopOption(iCustomID)-1)))
				{	iSel=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelItem(iParamIndex);
					switch(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex))
					{	case 0: // I
						case 3: // B
						case 5: // LI
						case 8: // LB
						{	imHpError=21;
							break;
						}
						case 1: // R
						case 2: // C
						case 4: // S
						{	imHpError=24;
							break;
						}
						case 6: // LR
						case 7: // LC
						case 9: // LS
						{	if((lpAddVal!=NULL)&&(iMaxAddVal<=MAX_RS_SIZE))
							{	// SetRepeatOpt < Last -> BUG "EmptyTypeList()"
								//NewStrSel=new CStrType(lpAddVal);
								//mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->AddType(NewStrSel);
								mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->AddStrTypeNoEmptyBug(lpAddVal);
							}
							else imHpError=22;
							break;
						}
					}					
				}
				else imHpError=12;
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
	return imHpError;
}

//-----------------------------------------------------//
// mHelpRemoveStrParamVal
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpRemoveStrParamVal(int iCustomID,int iParamIndex,int iRepeatIndex,int iSelIndex)
{	int iSel,imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	if((iParamIndex>=0)&&(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex)!=NO))
			{	if((iRepeatIndex>=0)&&(iRepeatIndex<=(mHpPak_CustomFavLst->GetLoopOption(iCustomID)-1)))
				{	iSel=mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetSelItem(iParamIndex);
					if(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)!=0)
					{	switch(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetItemType(iParamIndex))
						{	case 0: // I
							case 3: // B
							case 5: // LI
							case 8: // LB
							{	imHpError=21;
								break;
							}
							case 1: // R
							case 2: // C
							case 4: // S
							{	imHpError=24;
								break;
							}
							case 6: // LR
							case 7: // LC
							case 9: // LS
							{	if((iSelIndex>=0)&&(iSelIndex<=(mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetListCount(iRepeatIndex)-1)))
								{	mHpPak_CustomFavLst->GetItemFromRang(iCustomID)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetRepeatCell(iRepeatIndex)->TypeList->RemoveType(iSelIndex);
								}
								else imHpError=13;
								break;
							}
						}					
					}
					else imHpError=18;
				}
				else imHpError=12;
			}
			else imHpError=9;
		}
		else imHpError=8;
	}
	else imHpError=3;	
	return imHpError;
}

//-----------------------------------------------------//
// mHelpDestroyCustomTask
//-----------------------------------------------------//
extern "C" int MHELP_API mHelpDestroyCustomTask(int iCustomID)
{	int imHpError=0;
	if(bInitPass)
	{	if((iCustomID>=0)&&((mHpPak_CustomFavLst->GetFavCount()-1)>=iCustomID))
		{	mHpPak_CustomFavLst->RemoveFavItem(iCustomID);
			iCurCustom--;
		}
		else imHpError=8;
	}
	else imHpError=3;
	return imHpError;
}

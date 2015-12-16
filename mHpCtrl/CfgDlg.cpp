// CfgDlg.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include "CfgDlg.h"

//-----------------------------------------------------//
#include <ComCtrl.h>

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif 

//-----------------------------------------------------//
// Définitions
#define		RADIO_SEP_STRING			"{...}"

//-----------------------------------------------------//
// Variables
HWND hWndCtrlTab;
HWND hWndStaticMsg;
HWND hWndMsg;
HWND hWndCtrlIcon;
HWND hWndFrameCtrl;
HWND hWndStaticProduct;
HWND hWndProduct;
HWND hWndStaticVers;
HWND hWndVersion;
HWND hWndFrameAbout;
HWND hWndStaticDecrit;
HWND hWndDecrit;
HWND hWndViewStop;
HWND hWndViewContinue;
HWND hWndNoMsg;
HWND hWndFrameState;
HWND hWndStaticStateDecrit;
HWND hWndStaticStateLimite;
HWND hWndStaticStateType;
HWND hWndStateDecrit;
HWND hWndStateLimite;
HWND hWndStateType;
HWND hWndApply;
HWND hWndStateFrame;
HWND hWndWaitCtrl;
HWND hWndNoStateMsg;
HWND hWndNoStateChk;
HWND hWndClose;
HWND hWndSldInt;
HWND hWndCheck;
HWND hWndUnCheck;
HWND hWndChangeInt;
HWND hWndEditState;
HWND hWndMltEdit;
HWND hWndRadFix;
HWND hWndRadUsr;
HWND hWndRadLst;
HWND hWndLink;
HWND hWndUnLink;
HWND hWndMltSel;
HWND hWndAddMlt;
HWND hWndRmvMlt;
HWND hWndHelp;
CWndDocList* CfgDocLnchList;

//-----------------------------------------------------//
// Déclarations
void LaunchmHelpDoc(HWND);
void SetCfgDocLnch();
BOOL ApplyModif(HWND);
void ReadSettings(HWND);
void ShowCtrlsState(HWND);
void HideCtrlsTab(HWND);
void HideShowCtrlsTab(HWND);
LRESULT CALLBACK CfgDlgDocProc(HWND,UINT,WPARAM,LPARAM);

//-----------------------------------------------------------------------------------------------------------------------//
// LaunchmHelpDoc
//---------------------------------------------------------//
void LaunchmHelpDoc(HWND hWndActive)
{	int iSelTab=TabCtrl_GetCurSel(hWndCtrlTab);
	if(GetmHpVarStruct()->EventList->IsCtrlState(GetmHpVarStruct()->iCtrlSel,FALSE))
	{	switch(iSelTab)
		{	case 0: // Common
			{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_CFGCOM);
				break;
			}
			case 1: // State
			{	switch(GetmHpVarStruct()->EventList->GetParamType(GetmHpVarStruct()->iCtrlSel))
				{	case 3: // Boolean
					{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_CFGBOOL);
						break;
					}
					case 0: // Integer
					{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_CFGINT);
						break;
					}
					case 4: // String
					{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_CFGSTR);
						break;
					}
					case 9: // List of string
					{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_CFGMLTSTR);
						break;
					}
				}
				break;
			}
			case 2: // About
			{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_DEFCFG);
				break;
			}
		}
	}
	else
	{	switch(iSelTab)
		{	case 0: // Common
			{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_CFGCOM);
				break;
			}
			case 1: // About
			{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_DEFCFG);
				break;
			}
		}
	}
}

//---------------------------------------------------------//
// SetCfgDocLnch
//---------------------------------------------------------//
void SetCfgDocLnch()
{	CfgDocLnchList->AddDocWnd(hWndMsg,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndCtrlTab,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndDecrit,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndViewStop,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndViewContinue,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndNoMsg,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndStateDecrit,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndStateLimite,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndStateType,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndApply,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndWaitCtrl,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndNoStateMsg,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndNoStateChk,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndSldInt,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndCheck,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndUnCheck,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndChangeInt,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndClose,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndEditState,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndMltEdit,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndRadFix,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndRadUsr,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndRadLst,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndLink,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndUnLink,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndMltSel,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndAddMlt,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndRmvMlt,(LONG_PTR)CfgDlgDocProc);
	CfgDocLnchList->AddDocWnd(hWndHelp,(LONG_PTR)CfgDlgDocProc);
}

//---------------------------------------------------------//
// ApplyModif
//-----------------------------------------------------//
BOOL ApplyModif(HWND hWndActive)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	BOOL bError=FALSE;
	int iOpt;
	// State ////////////////////////////////////////////
	if(GetmHpVarStruct()->EventList->IsCtrlState(GetmHpVarStruct()->iCtrlSel,FALSE))
	{	// SaveCfg ----------------------------------------------------------------------------------------------------**//
		ComCtrl_SaveCfg(hWndActive,bError);
		if(bError) return FALSE;
		//-------------------------------------------------------------------------------------------------------------**//
		// State info ////////////////////////////////////
		if(GetmHpVarStruct()->bCustomFolder)
		{	// Description
			GetWindowText(hWndStateDecrit,lpTmp,MAX_RS_SIZE);
			if(strncmp(lpTmp,GetmHpVarStruct()->EventList->GetStrDescription(GetmHpVarStruct()->iCtrlSel,FALSE),MAX_RS_SIZE)!=0)
			{	GetmHpVarStruct()->EventList->SetStrDescription(GetmHpVarStruct()->iCtrlSel,lpTmp);}
			// Limite
			mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
			GetWindowText(hWndStateLimite,lpTmp,MAX_RS_SIZE);
			if(strncmp(lpTmp,GetmHpVarStruct()->EventList->GetStrLimite(GetmHpVarStruct()->iCtrlSel,FALSE,FALSE),MAX_RS_SIZE)!=0)
			{	GetmHpVarStruct()->EventList->SetStrLimite(GetmHpVarStruct()->iCtrlSel,lpTmp);}
		}
	}
	// Common ///////////////////////////////////////////
	// Option
	if(SendMessage(hWndViewStop,BM_GETCHECK,0,0)==BST_CHECKED) iOpt=0;
	else if(SendMessage(hWndViewContinue,BM_GETCHECK,0,0)==BST_CHECKED) iOpt=1;
	else if(SendMessage(hWndNoMsg,BM_GETCHECK,0,0)==BST_CHECKED)
	{	iOpt=2;
		GetmHpVarStruct()->EventList->SetMsg(GetmHpVarStruct()->iCtrlSel,NULL_STRING);
	}
	else iOpt=3;
	if((GetmHpVarStruct()->EventList->IsNoStateMsg(GetmHpVarStruct()->iCtrlSel))&&
	   (SendMessage(hWndNoStateChk,BM_GETCHECK,0,0)==BST_CHECKED))
	{	iOpt+=10;}
	else GetmHpVarStruct()->EventList->SetStateMsg(GetmHpVarStruct()->iCtrlSel,NULL_STRING);
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetmHpOption(iOpt);
	// Message
	if((iOpt!=2)&&(iOpt!=12))
	{	if(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID()!=0)
		{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_INACTIF,lpRC,MAX_RS_SIZE);
		}
		else LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_EXE_NOTFOUND,lpRC,MAX_RS_SIZE);
		mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
		GetWindowText(hWndMsg,lpTmp,MAX_RS_SIZE);
		if(strncmp(lpTmp,lpRC,MAX_RS_SIZE)!=0) GetmHpVarStruct()->EventList->SetMsg(GetmHpVarStruct()->iCtrlSel,lpTmp);
	}
	// No State Message
	if(iOpt>=10)
	{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_NOSTATE,lpRC,MAX_RS_SIZE);
		mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
		GetWindowText(hWndNoStateMsg,lpTmp,MAX_RS_SIZE);
		if(strncmp(lpTmp,lpRC,MAX_RS_SIZE)!=0) GetmHpVarStruct()->EventList->SetStateMsg(GetmHpVarStruct()->iCtrlSel,lpTmp);
	}
	return TRUE;
}

//-----------------------------------------------------//
// ReadSettings
//-----------------------------------------------------//
void ReadSettings(HWND hWndActive)
{	char szbuff[MAX_RS_SIZE]={0};
	char szTabItm[MAX_RS_SIZE]={0};
	TCITEM TabItm;
	TabItm.mask=TCIF_TEXT;
	TabItm.cchTextMax=MAX_RS_SIZE;
	TabItm.pszText=szTabItm;
	int iOpt=GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpOption();
	// Common /////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Option
	if(iOpt>=10)
	{	SendMessage(hWndNoStateChk,BM_SETCHECK,BST_CHECKED,0);
		EnableWindow(hWndNoStateMsg,TRUE);
		iOpt-=10;
	}
	switch(iOpt)
	{	case 0:
		{	SendMessage(hWndViewStop,BM_SETCHECK,BST_CHECKED,0);
			break;
		}
		case 1:
		{	SendMessage(hWndViewContinue,BM_SETCHECK,BST_CHECKED,0);
			break;
		}
		case 2:
		{	SendMessage(hWndNoMsg,BM_SETCHECK,BST_CHECKED,0);
			EnableWindow(hWndMsg,FALSE);
			break;
		}
		case 3:
		{	SendMessage(hWndWaitCtrl,BM_SETCHECK,BST_CHECKED,0);
			break;
		}
	}
	/* WARNING: Cette condition est plus complexe, lorsque la tâche qui précède est un changement d'application
				il faut revenir à l'application courante et comparer les CurrentWnds de l'évènement en cours et
				du dernier évènement déclencher sur cette application*/
	if(((!GetmHpVarStruct()->iCtrlSel)&&
	    (GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCurrentWnd()!=0))||
	   ((GetmHpVarStruct()->iCtrlSel!=0)&&
	  (((GetmHpVarStruct()->EventList->GetmHpTask((GetmHpVarStruct()->iCtrlSel-1),FALSE)->GetmHpID()==0)&&
		(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCurrentWnd()!=0))||
	   ((GetmHpVarStruct()->EventList->GetmHpTask((GetmHpVarStruct()->iCtrlSel-1),FALSE)->GetmHpID()!=0)&&
		(GetmHpVarStruct()->EventList->GetmHpTask((GetmHpVarStruct()->iCtrlSel-1),FALSE)->GetCurrentWnd()!=GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCurrentWnd())))))
	{	EnableWindow(hWndViewContinue,FALSE);
		EnableWindow(hWndNoMsg,FALSE);
	}
	// Message
	if(!strncmp(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpMsg(),NULL_STRING,MAX_RS_SIZE))
	{	if(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID()!=0)
		{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_INACTIF,szbuff,MAX_RS_SIZE);
		}
		else LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_EXE_NOTFOUND,szbuff,MAX_RS_SIZE);
		SetWindowText(hWndMsg,szbuff);
	}
	else
	{	SetWindowText(hWndMsg,GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpMsg());}
	// No State Message
	if(!strncmp(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpStateMsg(),NULL_STRING,MAX_RS_SIZE))
	{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_NOSTATE,szbuff,MAX_RS_SIZE);
		SetWindowText(hWndNoStateMsg,szbuff);
	}
	else
	{	SetWindowText(hWndNoStateMsg,GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpStateMsg());}
	// State //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	mHpPak_InitStr(szbuff,MAX_RS_SIZE);
	if(GetmHpVarStruct()->EventList->IsCtrlState(GetmHpVarStruct()->iCtrlSel,FALSE))
	{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_TAB_STATE,szbuff,MAX_RS_SIZE);
		strncpy(TabItm.pszText,szbuff,MAX_RS_SIZE);
		TabCtrl_InsertItem(hWndCtrlTab,1,&TabItm);
		mHpPak_InitStr(szbuff,MAX_RS_SIZE);
		LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_TAB_ABOUT,szbuff,MAX_RS_SIZE);
		strncpy(TabItm.pszText,szbuff,MAX_RS_SIZE);
		TabCtrl_InsertItem(hWndCtrlTab,2,&TabItm);
		// Type
		SetWindowText(hWndStateType,
					  mHpPak_GetStrStateType(GetmHpVarStruct()->EventList->GetParamType(GetmHpVarStruct()->iCtrlSel),
					  szbuff,MAX_RS_SIZE));
		// Description
		SetWindowText(hWndStateDecrit,GetmHpVarStruct()->EventList->GetStrDescription(GetmHpVarStruct()->iCtrlSel,FALSE));
		// Limite
		SetWindowText(hWndStateLimite,GetmHpVarStruct()->EventList->GetStrLimite(GetmHpVarStruct()->iCtrlSel,FALSE,FALSE));
		// LoadCfg ----------------------------------------------------------------------------------------------------**//
		ComCtrl_LoadCfg(hWndActive,hWndCtrlTab,hWndStateDecrit,hWndStateLimite);
		//-------------------------------------------------------------------------------------------------------------**//
	}
	else
	{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_TAB_ABOUT,szbuff,MAX_RS_SIZE);
		strncpy(TabItm.pszText,szbuff,MAX_RS_SIZE);
		TabCtrl_InsertItem(hWndCtrlTab,2,&TabItm);
	}
}

//-----------------------------------------------------//
// ShowCtrlsState
//-----------------------------------------------------//
void ShowCtrlsState(HWND hWndActive)
{	ShowWindow(hWndFrameState,SW_SHOW);
	ShowWindow(hWndStaticStateDecrit,SW_SHOW);
	ShowWindow(hWndStaticStateLimite,SW_SHOW);
	ShowWindow(hWndStaticStateType,SW_SHOW);
	ShowWindow(hWndStateDecrit,SW_SHOW);
	ShowWindow(hWndStateLimite,SW_SHOW);
	ShowWindow(hWndStateType,SW_SHOW);
	ShowWindow(hWndStateFrame,SW_SHOW);
	// ShowCfgTools -----------------------------------------------------------------------------------------------**//
	ComCtrl_ShowCfgTools(hWndActive);
	//-------------------------------------------------------------------------------------------------------------**//
}

//-----------------------------------------------------//
// HideCtrlsTab
//-----------------------------------------------------//
void HideCtrlsTab(HWND hWndActive)
{	ShowWindow(hWndMsg,SW_HIDE);
	ShowWindow(hWndStaticMsg,SW_HIDE);
	ShowWindow(hWndCtrlIcon,SW_HIDE);
	ShowWindow(hWndFrameCtrl,SW_HIDE);
	ShowWindow(hWndStaticProduct,SW_HIDE);
	ShowWindow(hWndProduct,SW_HIDE);
	ShowWindow(hWndStaticVers,SW_HIDE);
	ShowWindow(hWndVersion,SW_HIDE);
	ShowWindow(hWndFrameAbout,SW_HIDE);
	ShowWindow(hWndStaticDecrit,SW_HIDE);
	ShowWindow(hWndDecrit,SW_HIDE);
	ShowWindow(hWndViewStop,SW_HIDE);
	ShowWindow(hWndViewContinue,SW_HIDE);
	ShowWindow(hWndNoMsg,SW_HIDE);
	ShowWindow(hWndFrameState,SW_HIDE);
	ShowWindow(hWndStaticStateDecrit,SW_HIDE);
	ShowWindow(hWndStaticStateLimite,SW_HIDE);
	ShowWindow(hWndStaticStateType,SW_HIDE);
	ShowWindow(hWndStateDecrit,SW_HIDE);
	ShowWindow(hWndStateLimite,SW_HIDE);
	ShowWindow(hWndStateType,SW_HIDE);
	ShowWindow(hWndStateFrame,SW_HIDE);
	ShowWindow(hWndWaitCtrl,SW_HIDE);
	ShowWindow(hWndNoStateMsg,SW_HIDE);
	ShowWindow(hWndNoStateChk,SW_HIDE);
	// HideCfgTools ---------------------------------------------------------------------------------------------------**//
	ComCtrl_HideCfgTools(hWndActive);
	//-----------------------------------------------------------------------------------------------------------------**//
}

//-----------------------------------------------------//
// HideShowCtrlsTab
//-----------------------------------------------------//
void HideShowCtrlsTab(HWND hWndActive)
{	int iSel=TabCtrl_GetCurSel(hWndCtrlTab);
	// Hide
	HideCtrlsTab(hWndActive);
	// Show
	switch(iSel)
	{	case 0: // Common
		{	ShowWindow(hWndMsg,SW_SHOW);
			ShowWindow(hWndStaticMsg,SW_SHOW);
			ShowWindow(hWndViewStop,SW_SHOW);
			ShowWindow(hWndViewContinue,SW_SHOW);
			ShowWindow(hWndNoMsg,SW_SHOW);
			ShowWindow(hWndWaitCtrl,SW_SHOW);
			if(GetmHpVarStruct()->EventList->IsNoStateMsg(GetmHpVarStruct()->iCtrlSel))
			{	ShowWindow(hWndNoStateMsg,SW_SHOW);
				ShowWindow(hWndNoStateChk,SW_SHOW);
			}
			break;
		}
		case 1: // About
		{	if(!GetmHpVarStruct()->EventList->IsCtrlState(GetmHpVarStruct()->iCtrlSel,FALSE))
			{	ShowWindow(hWndCtrlIcon,SW_SHOW);
				ShowWindow(hWndFrameCtrl,SW_SHOW);
				ShowWindow(hWndStaticProduct,SW_SHOW);
				ShowWindow(hWndProduct,SW_SHOW);
				ShowWindow(hWndStaticVers,SW_SHOW);
				ShowWindow(hWndVersion,SW_SHOW);
				ShowWindow(hWndFrameAbout,SW_SHOW);
				ShowWindow(hWndStaticDecrit,SW_SHOW);
				ShowWindow(hWndDecrit,SW_SHOW);
			}
			else ShowCtrlsState(hWndActive);
			break;
		}
		case 2: // State
		{	if(GetmHpVarStruct()->EventList->IsCtrlState(GetmHpVarStruct()->iCtrlSel,FALSE))
			{	ShowWindow(hWndCtrlIcon,SW_SHOW);
				ShowWindow(hWndFrameCtrl,SW_SHOW);
				ShowWindow(hWndStaticProduct,SW_SHOW);
				ShowWindow(hWndProduct,SW_SHOW);
				ShowWindow(hWndStaticVers,SW_SHOW);
				ShowWindow(hWndVersion,SW_SHOW);
				ShowWindow(hWndFrameAbout,SW_SHOW);
				ShowWindow(hWndStaticDecrit,SW_SHOW);
				ShowWindow(hWndDecrit,SW_SHOW);
			}
			break;
		}
	}
}

//-----------------------------------------------------//
// CfgDlgDocProc
//-----------------------------------------------------//
LRESULT CALLBACK CfgDlgDocProc(HWND hWndDocLnch,UINT DocMessage,WPARAM DocWParam,LPARAM DocLParam)
{	LONG_PTR lPrevProc=CfgDocLnchList->GetDocWndProc(hWndDocLnch);
	switch(DocMessage)
	{	case WM_KEYUP:
		{	switch(DocWParam)
			{	case VK_F1:
				{	LaunchmHelpDoc(hWndDocLnch);
					break;
				}
				default:
				{	break;}
			}
			break;
		}
		case WM_NCDESTROY:
		{	CfgDocLnchList->RemoveDocWnd(hWndDocLnch);
			break;
		}
	}
	return CallWindowProc((WNDPROC)lPrevProc,hWndDocLnch,DocMessage,DocWParam,DocLParam);
}

//-----------------------------------------------------------------------------------------------------------------------//
// ConfigDlgProc()
//
//-----------------------------------------------------------------------------------------------------------------------//
LRESULT CALLBACK ConfigDlgProc(HWND hWndCfg,UINT CfgMsg,WPARAM wCfgParam,LPARAM lCfgParam)
{	static BOOL bChangedCfg=FALSE;
	static BOOL bPassed=FALSE;
	// ProcCfg --------------------------------------------------------------------------------------------------------**//
	if(ComCtrl_ProcCfg(hWndCfg,CfgMsg,wCfgParam,lCfgParam,hWndStateDecrit,hWndStateLimite,hWndApply)) return FALSE;
	//-----------------------------------------------------------------------------------------------------------------**//
	switch(CfgMsg)
	{	case WM_INITDIALOG:
		{	RECT Rect;
			RECT SetRect;
			char lpRC[MAX_RS_SIZE]={0};
			char lpTabItem[MAX_RS_SIZE]={0};
			TCITEM TabItem;
			if(!bPassed)
			{	CfgDocLnchList=new CWndDocList();
				bPassed=TRUE;
			}
			InitCommonControls();
			hWndMsg=GetDlgItem(hWndCfg,IDC_EDIT_MSG);
			hWndStaticMsg=GetDlgItem(hWndCfg,IDC_STATIC_MSG);
			hWndCtrlTab=GetDlgItem(hWndCfg,IDC_CTRL_TAB);
			hWndCtrlIcon=GetDlgItem(hWndCfg,IDC_CTRL_ICON);
			hWndFrameCtrl=GetDlgItem(hWndCfg,IDC_STATIC_CTRL);
			hWndStaticProduct=GetDlgItem(hWndCfg,IDC_STATIC_PRODUCT);
			hWndProduct=GetDlgItem(hWndCfg,IDC_VIEW_CLASS);
			hWndStaticVers=GetDlgItem(hWndCfg,IDC_STATIC_VERSION);
			hWndVersion=GetDlgItem(hWndCfg,IDC_VERSION_CTRL);
			hWndFrameAbout=GetDlgItem(hWndCfg,IDC_FRAME_ABOUTCFG);
			hWndStaticDecrit=GetDlgItem(hWndCfg,IDC_STATIC_DESCRIPTION);
			hWndDecrit=GetDlgItem(hWndCfg,IDC_DESCRIPTION_CTRL);
			hWndViewStop=GetDlgItem(hWndCfg,IDC_RADIO_VIEWMSG_STOP);
			hWndViewContinue=GetDlgItem(hWndCfg,IDC_RADIO_VIEWMSG_CONTINUE);
			hWndNoMsg=GetDlgItem(hWndCfg,IDC_RADIO_NOMSG);
			hWndFrameState=GetDlgItem(hWndCfg,IDC_STATIC_STATEINFO);
			hWndStaticStateDecrit=GetDlgItem(hWndCfg,IDC_STATIC_STATEDESCRIPTION);
			hWndStaticStateLimite=GetDlgItem(hWndCfg,IDC_STATIC_LIMITE);
			hWndStaticStateType=GetDlgItem(hWndCfg,IDC_STATIC_STATETYPE);
			hWndStateDecrit=GetDlgItem(hWndCfg,IDC_EDIT_DESCRIPTION);
			hWndStateLimite=GetDlgItem(hWndCfg,IDC_EDIT_BORNE);
			hWndStateType=GetDlgItem(hWndCfg,IDC_EDIT_TYPEFLAG);
			hWndApply=GetDlgItem(hWndCfg,IDC_APPLY);
			hWndStateFrame=GetDlgItem(hWndCfg,IDC_FRAME_STATE);
			hWndWaitCtrl=GetDlgItem(hWndCfg,IDC_RADIO_DELAYMSG);
			hWndNoStateMsg=GetDlgItem(hWndCfg,IDC_EDIT_NOSTATE_MSG);
			hWndNoStateChk=GetDlgItem(hWndCfg,IDC_CHECK_NOSTATE);
			hWndClose=GetDlgItem(hWndCfg,IDCANCEL);
			hWndSldInt=GetDlgItem(hWndCfg,IDC_SLIDER_CFG);
			hWndCheck=GetDlgItem(hWndCfg,IDC_CHECK_ACTIVE);
			hWndUnCheck=GetDlgItem(hWndCfg,IDC_CHECK_INACTIVE);
			hWndChangeInt=GetDlgItem(hWndCfg,IDC_CHECK_CHANGEINT);
			hWndEditState=GetDlgItem(hWndCfg,IDC_EDIT_STATE);
			hWndMltEdit=GetDlgItem(hWndCfg,IDC_MULTI_EDIT);
			hWndRadFix=GetDlgItem(hWndCfg,IDC_RADIO_FIXED);
			hWndRadUsr=GetDlgItem(hWndCfg,IDC_RADIO_USER);
			hWndRadLst=GetDlgItem(hWndCfg,IDC_LIST_RADIO);
			hWndLink=GetDlgItem(hWndCfg,IDC_LINK_ITEM);
			hWndUnLink=GetDlgItem(hWndCfg,IDC_UNLINK_ITEM);
			hWndMltSel=GetDlgItem(hWndCfg,IDC_LISTBOX_MULTILIST);
			hWndAddMlt=GetDlgItem(hWndCfg,IDC_ADD_MULTILIST);
			hWndRmvMlt=GetDlgItem(hWndCfg,IDC_REMOVE_MULTILIST);
			hWndHelp=GetDlgItem(hWndCfg,IDC_LAUNCH_DOC);
			//Tab---------------------------------//
			TabItem.mask=TCIF_TEXT;
			TabItem.cchTextMax=MAX_RS_SIZE;
			TabItem.pszText=lpTabItem;
			LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_TAB_COMMON,lpRC,MAX_RS_SIZE);
			strncpy(TabItem.pszText,lpRC,MAX_RS_SIZE);
			TabCtrl_InsertItem(hWndCtrlTab,0,&TabItem);
			// State info-------------------------//
			if(!GetmHpVarStruct()->bCustomFolder)
			{	EnableWindow(hWndStateDecrit,FALSE);
				EnableWindow(hWndStateLimite,FALSE);
			}
			// InfosCfg -----------------------------------------------------------------------------------------------**//
			ComCtrl_GetInfosCfg(hWndCfg,
                                hWndApply,
                                hWndCtrlIcon,
                                hWndProduct,
                                hWndVersion,
                                hWndDecrit,
                                hWndFrameCtrl,
                                hWndViewContinue,
                                hWndNoMsg,
                                hWndWaitCtrl);
			//---------------------------------------------------------------------------------------------------------**//
			ReadSettings(hWndCfg);
			GetWindowRect(hWndCfg,&Rect);
			MoveWindow(hWndCfg,Rect.left,Rect.top,320,362,TRUE);
			MoveWindow(hWndCtrlTab,3,3,308,300,TRUE);
			MoveWindow(hWndHelp,4,308,87,25,TRUE);
			MoveWindow(hWndApply,131,308,87,25,TRUE);
			MoveWindow(hWndClose,224,308,87,25,TRUE);
			MoveWindow(hWndStaticMsg,7,27,280,16,TRUE);
			MoveWindow(hWndMsg,7,44,299,58,TRUE);
			MoveWindow(hWndCtrlIcon,16,44,36,36,TRUE);
			MoveWindow(hWndFrameCtrl,9,26,295,150,TRUE);
			MoveWindow(hWndStaticProduct,59,43,79,16,TRUE);
			MoveWindow(hWndProduct,141,43,137,16,TRUE);
			MoveWindow(hWndStaticVers,59,58,49,16,TRUE);
			MoveWindow(hWndVersion,110,58,157,16,TRUE);
			MoveWindow(hWndFrameAbout,9,177,295,118,TRUE);
			MoveWindow(hWndStaticDecrit,59,73,60,16,TRUE);
			MoveWindow(hWndDecrit,127,73,150,76,TRUE);
			MoveWindow(hWndViewStop,7,107,279,16,TRUE);
			MoveWindow(hWndViewContinue,7,127,279,16,TRUE);
			MoveWindow(hWndNoMsg,7,147,279,16,TRUE);
			MoveWindow(hWndStateFrame,9,26,295,159,TRUE);
			MoveWindow(hWndStaticStateDecrit,18,201,100,16,TRUE);
			MoveWindow(hWndStateDecrit,18,217,276,20,TRUE);
			MoveWindow(hWndStaticStateType,18,242,104,16,TRUE);
			MoveWindow(hWndStateType,123,241,171,20,TRUE);
			MoveWindow(hWndStaticStateLimite,18,266,104,16,TRUE);
			MoveWindow(hWndStateLimite,123,265,171,20,TRUE);
			MoveWindow(hWndFrameState,9,187,295,109,TRUE);
			MoveWindow(hWndWaitCtrl,7,167,279,16,TRUE);
			MoveWindow(hWndNoStateMsg,7,188,299,58,TRUE);
			MoveWindow(hWndNoStateChk,7,251,286,16,TRUE);
			// BUG: les fenêtres XP sont trop courte //
			GetWindowRect(hWndApply,&SetRect);
			GetWindowRect(hWndCfg,&Rect);
			if(Rect.bottom<=SetRect.bottom)
                MoveWindow(hWndCfg,Rect.left,Rect.top,320,369,TRUE);
            else
            {   DWORD winVersion=GetVersion();
                DWORD VersMajor=(DWORD)(LOBYTE(LOWORD(winVersion)));
                DWORD VersMinor=(DWORD)(HIBYTE(LOWORD(winVersion)));
                // Check Windows 7 or later
                if((VersMajor>6)||((VersMajor==6)&&(VersMinor>=1)))
                    MoveWindow(hWndCfg,Rect.left,Rect.top,320,367,TRUE);
            }
			//
			HideShowCtrlsTab(hWndCfg);
			bChangedCfg=FALSE;
			EnableWindow(hWndApply,FALSE);
			SetCfgDocLnch();
			return TRUE;
		}
        case WM_DRAWITEM:
        {   LPDRAWITEMSTRUCT lpDrawItmStruct=(LPDRAWITEMSTRUCT)lCfgParam;
            if(lpDrawItmStruct->CtlID==IDC_CTRL_TAB)
            {   char szTabLabel[MAX_KEY_SIZE]={0};
                TCITEM tci;
                tci.mask=TCIF_TEXT;
                tci.pszText=szTabLabel;
                tci.cchTextMax=MAX_KEY_SIZE;
                TabCtrl_GetItem(hWndCtrlTab,lpDrawItmStruct->itemID,&tci);
                if(lpDrawItmStruct->itemAction&ODA_DRAWENTIRE)
                {   SetBkMode(lpDrawItmStruct->hDC,TRANSPARENT);
                    RECT ItmTabRect=lpDrawItmStruct->rcItem;
                    FillRect(lpDrawItmStruct->hDC,&ItmTabRect,(HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)));
                    if(!(lpDrawItmStruct->itemState&ODS_SELECTED))
                        ItmTabRect.top+=GetSystemMetrics(SM_CYBORDER);
                    DrawText(lpDrawItmStruct->hDC,tci.pszText,(int)strlen(tci.pszText),&ItmTabRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
                }
            }
            break;
        }
		case WM_NOTIFY:
		{	switch(((LPNMHDR)lCfgParam)->idFrom)
			{	case IDC_CTRL_TAB:
				{	switch(((LPNMHDR)lCfgParam)->code)
					{	case TCN_SELCHANGE:
						{	HideShowCtrlsTab(hWndCfg);
							break;
						}
					}
					break;
				}
				default:
				{	return DefWindowProc(hWndCfg,CfgMsg,wCfgParam,lCfgParam);}
			}
			break;
		}
		case WM_COMMAND:
		{	switch(LOWORD(wCfgParam))
			{	case IDC_APPLY:
				{	if(ApplyModif(hWndCfg))
					{	bChangedCfg=TRUE;
						EnableWindow(hWndApply,FALSE);
						SetFocus(hWndClose);
					}
					break;
				}
				case IDCANCEL:
				{	if(IsWindowEnabled(hWndApply))
					{	if(mHpPak_mHpMessage(hWndCfg,GetmHpVarStruct()->hmHpCtrlInst,IDS_WARNING_NOSAVE,NULL,0,0,MB_ICONEXCLAMATION|MB_YESNO)==IDYES)
						{	if(bChangedCfg) return EndDialog(hWndCfg,IDOK);
							else return EndDialog(hWndCfg,IDCANCEL);
						}
					}
					else
					{	if(bChangedCfg) return EndDialog(hWndCfg,IDOK);
						else return EndDialog(hWndCfg,IDCANCEL);
					}
					break;
				}
				case IDC_LAUNCH_DOC:
				{	LaunchmHelpDoc(hWndCfg);
					break;
				}
				case IDC_RADIO_VIEWMSG_STOP:
				{	SendMessage(hWndWaitCtrl,BM_SETCHECK,0,0);
					EnableWindow(hWndMsg,TRUE);
					EnableWindow(hWndApply,TRUE);
					break;
				}
				case IDC_RADIO_VIEWMSG_CONTINUE:
				{	SendMessage(hWndWaitCtrl,BM_SETCHECK,0,0);
					EnableWindow(hWndMsg,TRUE);
					EnableWindow(hWndApply,TRUE);
					break;
				}
				case IDC_RADIO_NOMSG:
				{	SendMessage(hWndWaitCtrl,BM_SETCHECK,0,0);
					EnableWindow(hWndMsg,FALSE);
					EnableWindow(hWndApply,TRUE);
					break;
				}
				case IDC_RADIO_DELAYMSG:
				{	SendMessage(hWndViewStop,BM_SETCHECK,0,0);
					SendMessage(hWndViewContinue,BM_SETCHECK,0,0);
					SendMessage(hWndNoMsg,BM_SETCHECK,0,0);
					EnableWindow(hWndMsg,TRUE);
					EnableWindow(hWndApply,TRUE);
					break;
				}
				case IDC_CHECK_NOSTATE:
				{	if(SendMessage(hWndNoStateChk,BM_GETCHECK,0,0)==BST_CHECKED) EnableWindow(hWndNoStateMsg,TRUE);
					else EnableWindow(hWndNoStateMsg,FALSE);
					EnableWindow(hWndApply,TRUE);
					break;
				}
				case IDC_EDIT_MSG:
				case IDC_EDIT_BORNE:
				case IDC_EDIT_DESCRIPTION:
				case IDC_EDIT_NOSTATE_MSG:
				{	if(HIWORD(wCfgParam)==EN_CHANGE) EnableWindow(hWndApply,TRUE);
					break;
				}
				default:{ break;}
			}
		}
	}
	return FALSE;
}
// CboCtrl.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <ComboBox\CboCtrl.h>
#include <EditBox\EditCtrl.h>

//-------------------------------//
#include "mHp2D.h"
#include "resource.h"

#ifndef  __MHPSOUND_H
 #include "mHpSnd.h"
#endif

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

//-----------------------------------------------------//
// Class name
#define		CPP_TASK_COMBOBOX			"ComboBox"                      // C/C++ control
#define		VB_TASK_COMBOBOX			"ThunderRT6ComboBox"            // VB control
#define		DOTNET_TASK_COMBOBOX        "WindowsForms10.COMBOBOX.app"   // DotNet control

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles
char ComboText[MAX_RS_SIZE]={0};		// Chaine de caracteres intermediaire pour l'objet "CComboTask"

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration
BOOL FillComboText(HWND);

//----------------------------------------------------------------------------------------------------------------GENERAL//
// FillComboText
//-----------------------------------------------------//
BOOL FillComboText(HWND hWndActive)
{	char lpText[MAX_RS_SIZE]={0};
	char chartemp[1]={0};
	GetWindowText(GetmHpVarStruct()->hWndCurCtrl,lpText,MAX_RS_SIZE);
	_snprintf(chartemp,1,"%c",GetmHpVarStruct()->CourantTask->GetMultiIndex(NO)[GetmHpVarStruct()->CboEditIndice]);
	strncat(lpText,chartemp,1);
	if(GetmHpVarStruct()->imHpMode!=1)
	{	SetWindowText(GetmHpVarStruct()->hWndCurCtrl,lpText);
		GetmHpVarStruct()->CboEditIndice++;
		if((GetmHpVarStruct()->CboEditIndice-1)!=mHpPak_FindStrLenght(GetmHpVarStruct()->CourantTask->GetMultiIndex(NO),MAX_RS_SIZE)) return FALSE;
		GetmHpVarStruct()->CboEditIndice=0;
		return TRUE;
	}
	SetWindowText(GetmHpVarStruct()->hWndCurCtrl,GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
	return TRUE;
}

//****************************************************************************************************************OBJECTS//
// Class CComboTask
CComboTask::CComboTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,char* lpCtrlName)
		   :CListTask(4,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,lpCtrlName)
{}

CComboTask::CComboTask()
		   :CListTask()
{}

// class CComboWrite //////////////////////////////////////////////////////////////////////////////////////////////////////
CComboWrite::CComboWrite(int iStyle)
			:CStyleWrite(iStyle)
{}

CComboWrite::CComboWrite()
			:CStyleWrite()
{}

//****************************************************************************************************************MHPFILE//
// Combo_ReadEvent
//---------------------------------------------------//
BOOL Combo_ReadEvent(HWND hWndActive,const std::string& strPostData,UINT &iCustIdx)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpSel[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	int iCur,iCnt,iTmp,iOpt;
	char chNext=END_RS_TASK;
	BOOL bNoSel=FALSE,bError,bContinue;
	CComboTask *CboTask;
	// iCurrentWnd,iCountWnd,iOption,lpmHpMsg
	GetCommonEvent(hWndActive,iCur,iCnt,iOpt,lpMsg,MAX_RS_SIZE,lpStateMsg,MAX_RS_SIZE,chNext,iTmp);
	if(iTmp==NO) return TRUE; // Error
    // mHpCtrlName
    char lpCtrlName[MAX_CTRL_NAME]={0};
    mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpCtrlName,MAX_CTRL_NAME,chNext,TRUE);
	if(iTmp==NO) return TRUE; // Error
	// mHpCtrlID
	ReadmHpCtrlID(hWndActive,GetmHpVarStruct()->mHpFileName,lpctrl,MAX_PART_TREE,chNext);
	if((lpctrl[0]==NO)&&(*lpCtrlName==0)) return TRUE; // Error
    if(*lpCtrlName!=0) CboTask=new CComboTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName);
    else CboTask=new CComboTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL);
#ifdef MHELP_OLD
	GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpSel,MAX_RS_SIZE,bContinue,bError);
	if((!bError)&&(!bContinue))
	{	if(lpSel[0]==RS_PARAM)
		{	iTmp=mHpPak_GetIntFromStr(lpSel,MAX_RS_SIZE,1);
			if(!DidItSelected(iTmp)) bNoSel=TRUE;
			else
			{	GetStrEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0,lpSel,MAX_RS_SIZE);
				if(iTmp!=NO) CboTask->AddMultiIndex(lpSel);
				else return TRUE; // Error
			}
		}
		else CboTask->AddMultiIndex(lpSel);
	}
	else return TRUE; // Error
#else
    ////// TODO: Be able to remove lines below (state always in HTML)
	GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpSel,MAX_RS_SIZE,bContinue,bError);
	if((bError)||(bContinue))
	    return TRUE; // Error
    //CboTask->AddMultiIndex(lpSel);
    //////

    // Check if selected
    if(!IsHtmlCustSelected(strPostData,iCustIdx))
        bNoSel=TRUE;
    else
    {   // Get HTML user setting
        if(!GetHtmlCustValue(lpSel,
                             MAX_RS_SIZE,
                             strPostData,
                             LISTSEL_KEY,
                             iCustIdx))
        {   return TRUE; // Error: No state parameter
        }
        // Assign state
        CboTask->AddMultiIndex(lpSel);
    }
    // Increase number of custom task
    iCustIdx++;
#endif
	if(!bNoSel) GetmHpVarStruct()->TaskList->AddCellule(CboTask);
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// Combo_WriteEvent
//---------------------------------------------------//
void Combo_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
                      BOOL bFolderParam,char* lpCurExe,int &IndexParam,BOOL &bRes)
{	if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))
	{	bRes=FALSE;
		return;
	}
	if((bFolderParam)&&(EvntObj->GetCustomVisible(Index)))
	{	if((!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
		{	bRes=FALSE;}
		IndexParam++;
	}
	else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrState(Index,TRUE),TRUE))
	{	bRes=FALSE;}
}

//-----------------------------------------------------------------------------------------------------------------------//
// Combo_WriteParam
//---------------------------------------------------//
BOOL Combo_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}			

//*******************************************************************************************************************KEYS//
// Combo_ReadEventKeys
//---------------------------------------------------//
void Combo_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpWrite* ReadWrite;
	CmHpParam* ReadParam;
	ReadTask=new CComboTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL)
	{	ReadWrite=new CComboWrite();
		ReadParam=new CmHpParam();
		ReadWrite->ReadEventKey(Index);
		if(!ReadParam->ReadParamKey(Index))
		{	delete(ReadParam);
			ReadParam=NULL;
		}
		EvntObj->AddEvent(ReadTask,ReadWrite,ReadParam);
	}
	else TaskList->AddCellule(ReadTask);
}

//********************************************************************************************************************ADD//
// Combo_AddEvent
//---------------------------------------------------//
void Combo_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndCbo,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
	CComboWrite* wItem;
    if(*lpCtrlName!=0)
	    Item=new CComboTask(NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName);
    else
    {	int lpctrl[MAX_PART_TREE]={0};
	    AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndCbo));
	    Item=new CComboTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL);
    }
	int iSel=(int)SendMessage(hWndCbo,CB_GETCURSEL,0,0);
	char lpComboText[MAX_RS_SIZE]={0};
	if(iSel==CB_ERR) GetWindowText(hWndCbo,lpComboText,MAX_RS_SIZE);
	else SendMessage(hWndCbo,CB_GETLBTEXT,(WPARAM)iSel,(LPARAM)lpComboText);
	Item->AddMultiIndex(lpComboText);
	if(IsWndStyle(hWndCbo,CBS_DROPDOWNLIST)==CBS_DROPDOWNLIST)
	{	wItem=new CComboWrite(1);}
	else wItem=new CComboWrite(0);
	if(bWrite)
	{	Item->WriteTaskKey();
		wItem->WriteEventKey();
		GetmHpVarStruct()->EventList->AddEvent(Item,wItem,NULL);
	}
	else
	{	RemoveLastKeyPos();
		Item->WriteTaskKey();
		wItem->WriteEventKey();
		GetmHpVarStruct()->EventList->ReplaceEvent(Item,wItem);
	}
}

//*************************************************************************************************************PROPERTIES//
// Combo_IsClassName
//---------------------------------------------------//
BOOL Combo_IsClassName(HWND hWndCbo,char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_COMBOBOX,ClassSize))||(!strncmp(CtrlClass,VB_TASK_COMBOBOX,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_COMBOBOX,sizeof(DOTNET_TASK_COMBOBOX)-1))) return TRUE;
	// Combobox as an Editbox
	if(Edit_IsClassName(CtrlClass,ClassSize))
	{	char lpParentClass[MAX_RS_SIZE]={0};
	    GetClassName(GetParent(hWndCbo),lpParentClass,MAX_RS_SIZE);
        if((!strncmp(lpParentClass,CPP_TASK_COMBOBOX,ClassSize))||(!strncmp(lpParentClass,VB_TASK_COMBOBOX,ClassSize))||
           (!strncmp(lpParentClass,DOTNET_TASK_COMBOBOX,sizeof(DOTNET_TASK_COMBOBOX)-1)))
		{	return TRUE;
		}
	}
	return FALSE;
}

//---------------------------------------------------//
// Combo_IsCustom
//---------------------------------------------------//
void Combo_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//---------------------------------------------------//
// Combo_StrState
//---------------------------------------------------//
void Combo_StrState(CEventCell* pCurCell,char* lpState,int StateSize)
{	strncpy(lpState,pCurCell->mHpTask->GetMultiIndex(NO),StateSize);}

//---------------------------------------------------//
// Combo_ParamType
//---------------------------------------------------//
int Combo_ParamType()
{	return 4; // S
}

//-----------------------------------------------------//
// Combo_DefaultBound
//-----------------------------------------------------//
void Combo_DefaultBound(char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	strncpy(pStrBound,STRING_LIMITE,BoundSize);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
	strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
}

//-----------------------------------------------------//
// Combo_IsCustomized
//-----------------------------------------------------//
BOOL Combo_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------//
// Combo_IsNoStateMsg
//-----------------------------------------------------//
BOOL Combo_IsNoStateMsg(CEventCell* pCurEvnt)
{	if(pCurEvnt->mHpWrite->GetCtrlStyle()==1) return TRUE;
	return FALSE;
}

//-----------------------------------------------------//
// Combo_GetHtmlTag
//-----------------------------------------------------//
#define COMBOBOX_HTML_TAG   "\t<input type=\"text\" name=\"%d-%s\" maxlength=\"100\" value=\"%s\"/>\r\n"

void Combo_GetHtmlTag(CEventCell* pCurEvnt,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{
    _snprintf(lpHtmlTag,
              iMaxSize,
              COMBOBOX_HTML_TAG,
              iCustIndex,
              LISTSEL_KEY,
              pCurEvnt->mHpTask->GetMultiIndex(NO));
}

//***************************************************************************************************************CFGTOOLS//
// Combo_ShowCfgTools
//---------------------------------------------------//
void Combo_ShowCfgTools(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndState=GetDlgItem(hWndActive,IDC_STATIC_STATE);
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	MoveWindow(hWndState,18,43,256,16,TRUE);
	MoveWindow(hWndEditState,18,59,276,20,TRUE);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_STRSEL,lpTmp,MAX_RS_SIZE);
	SetWindowText(hWndState,lpTmp);
	ShowWindow(hWndState,SW_SHOW);
	ShowWindow(hWndEditState,SW_SHOW);
}

//---------------------------------------------------//
// Combo_HideCfgTools
//---------------------------------------------------//
void Combo_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
}

//---------------------------------------------------//
// Combo_SaveCfg
//---------------------------------------------------//
void Combo_SaveCfg(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	GetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lpTmp,MAX_RS_SIZE);
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->InitMultiIndex();
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->AddMultiIndex(lpTmp);
}

//---------------------------------------------------//
// Combo_LoadCfg
//---------------------------------------------------//
void Combo_LoadCfg(HWND hWndActive)
{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMultiIndex(NO));
}

//---------------------------------------------------//
// Combo_GetInfosCfg
//---------------------------------------------------//
void Combo_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_COMBO,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_COMBO,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_COMBO,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_COMBO,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_COMBO)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// Combo_ProcCfg
//---------------------------------------------------//
BOOL Combo_ProcCfg(UINT Message,WPARAM wParam,HWND hWndChng)
{	if(Message==WM_COMMAND)
	{	if(LOWORD(wParam)==IDC_EDIT_STATE)
		{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
			return TRUE;
		}
	}
	return FALSE;
}

//****************************************************************************************************************PROCESS//
// Combo_CaptureMsg
//---------------------------------------------------//
void Combo_CaptureMsg(HWND hWndActive,char* lpCtrlName,HWND hWndCtl,WPARAM wpParam,HWND hToolWnd,int iCurOp,CEventListe* EventObj,char* lpNameApp,
					  UINT &pIdLast,int pCurwnd,int pCntwnd,BOOL &pActionUser,int &pResCapt)
{	int iWriteRes=0;
	if((HIWORD(wpParam)==CBN_SELCHANGE)||(HIWORD(wpParam)==CBN_EDITCHANGE))
	{	WriteImmediatTask(hWndActive,lpCtrlName,hWndCtl,hToolWnd,NULL,iCurOp,4,(UINT)((int)LOWORD(wpParam)),
                          EventObj,NULL,NULL,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
		switch(iWriteRes)
		{	case 0:
			{	pActionUser=FALSE;
				SetTimer(hWndActive,ID_TIMER_NEXT,1,(TIMERPROC)mHpTimerProc);
				break;
			}
			case 1:
			{	pResCapt=0;
				break;
			}
			default:
			{	break;}
		}
	}
}

//---------------------------------------------------//
// Combo_mHpProcess
//---------------------------------------------------//
void Combo_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2: 
		{	int iSelIndex=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_FINDSTRING,(WPARAM)NO,
										   (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
			if((GetmHpVarStruct()->imHpMode==1)||(SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETCOUNT,0,0)<=iSelIndex))
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(15);
				GetmHpVarStruct()->iSkipSnd=1;
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
				break;
			}
			if(GetmHpVarStruct()->EcartDepl!=1) GetmHpVarStruct()->EcartDepl/=2;
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			if(SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETCURSEL,0,0)==iSelIndex)
			{	GetmHpVarStruct()->CursorType=0;
				GetmHpVarStruct()->CourantTask->SetmHpStep(15);
				GetmHpVarStruct()->iSkipSnd=1;
			}
			SetTimer(hWndActive,ID_TIMER_DDRAW,200,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_DELAI,200,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			EndObjects(hWndActive);
			SetFocus(GetmHpVarStruct()->hWndCurCtrl);
			SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_SHOWDROPDOWN,1,0);
			if(!DoInit(hWndActive))
			{	StopmHelp(hWndActive,TRUE,TRUE);
				return;
			}
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetDDrawObj(hWndActive,0);
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 5:
		{	BOOL bUse;
			GetmHpVarStruct()->bBlockUpdateFrame=FALSE;
			GetmHpVarStruct()->bNewDepl=TRUE;
			ComboListBoxStatusPos(hWndActive,bUse,TRUE,
								  (int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_FINDSTRING,(WPARAM)NO,
												   (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO)));
			if((!GetmHpVarStruct()->AscensSens)||((GetmHpVarStruct()->AscensSens)&&(!bUse)))
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(8);}
			else GetAscensNextPos(hWndActive,TRUE,TRUE);
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6:
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_ASCENS,20,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 8:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			GetAscensNextPos(hWndActive,FALSE,TRUE);
			GetmHpVarStruct()->bNewDepl=TRUE;
			GetmHpVarStruct()->CursorType=0;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 9:
		{	EndObjects(hWndActive);
			SetRealCursor(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur,FALSE);
			if(!DoInit(hWndActive))
			{	StopmHelp(hWndActive,TRUE,TRUE);
				return;
			}
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			GetmHpVarStruct()->CourantTask->SetmHpStep(14);
			break;
		}
		case 10: // Edit
		{	if(GetmHpVarStruct()->EcartDepl!=1) GetmHpVarStruct()->EcartDepl/=2;
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			SetTimer(hWndActive,ID_TIMER_DDRAW,200,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 11:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 12:
		{	SetFocus(GetmHpVarStruct()->hWndCurCtrl);
			PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=0;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 13:
		{	GetmHpVarStruct()->XnextPos+=8;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 14:
		{	if(GetmHpVarStruct()->imHpMode==2) GetWindowText(GetmHpVarStruct()->hWndCurCtrl,ComboText,MAX_RS_SIZE);
			SetWindowText(GetmHpVarStruct()->hWndCurCtrl,NULL_STRING);
			if(!FillComboText(hWndActive)) SetTimer(hWndActive,ID_TIMER_COMBO,1,(TIMERPROC)mHpTimerProc);
			else
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(15); // Fin
				GetmHpVarStruct()->iSkipSnd=1;
				ForceMessage(hWndActive,CBN_EDITCHANGE);
				SetTimer(hWndActive,ID_TIMER_DELAI,100,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 15:
		{	if((((int)(SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_FINDSTRINGEXACT,(WPARAM)NO,(LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO))))==CB_ERR)&&
			   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWNLIST)==CBS_DROPDOWNLIST)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10))
			{	ViewCtrlNotReadyMsg(hWndActive,4);
			}
			else
			{	if(PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS))) 
				{	SetTimer(hWndActive,ID_TIMER_DELAI,200,(TIMERPROC)mHpTimerProc);}
				else SetTimer(hWndActive,ID_TIMER_DELAI,500,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 16:
		{	int iSel=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_FINDSTRING,(WPARAM)NO,(LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
			PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			if(iSel!=CB_ERR)
			{	EndObjects(hWndActive);
				SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_SHOWDROPDOWN,0,0);
				if((GetmHpVarStruct()->imHpMode!=2)&&(SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETCOUNT,0,0)>iSel))
					SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_SETCURSEL,iSel,0);
				ForceMessage(hWndActive,CBN_SELCHANGE);
                //if(!IsWndStyle(hWndcombo,WS_EX_NOPARENTNOTIFY))
                    ForceMessage(hWndActive,CBN_SELENDOK);  // RUSTINE: CommonDlg
                if(!DoInit(hWndActive))
				{	StopmHelp(hWndActive,TRUE,TRUE);
					return;
				}
			}
			GetmHpVarStruct()->EcartDepl=GetmHpVarStruct()->imHpMouseSpeed;
			SetTimer(hWndActive,ID_TIMER_DELAI,100,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 17:
		{	if(GetmHpVarStruct()->TaskList->NextCellule())
			{	GetmHpVarStruct()->bNewDepl=TRUE;
				SetDDrawObj(hWndActive,0);
				GetmHpVarStruct()->CourantTask=GetmHpVarStruct()->TaskList->GetCurrentTask();
				GetNextPos(hWndActive);
			}
			else NextRepeatAssistProc(hWndActive);
			break;
		}
		default: NULL;
	}
}

//---------------------------------------------------//
// Combo_StartProc
//---------------------------------------------------//
BOOL Combo_StartProc(HWND hWndActive)
{	GetmHpVarStruct()->hWndCurCtrl=NULL;
	/*if(*GetmHpVarStruct()->CourantTask->GetmHpCtrlName()!=0)
	{	if(mHpPak_IsDotNetHwnd(hWndActive))
		{   int iState=0,iStyle=0;
			GetmHpVarStruct()->hWndCurCtrl=mHpPak_GetDotNetCtrlHandle(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlName(),iStyle,iState);
		}
	}*/
    if(!GetmHpVarStruct()->hWndCurCtrl)
    {   if(GetmHpVarStruct()->CourantTask->GetmHpID()>10) // ToolBar - ComboBox ///////////////////////////////////////////////
		    GetmHpVarStruct()->hWndCurCtrl=GetDlgItem(GetToolCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID()),GetmHpVarStruct()->CourantTask->GetCommandID());
	    else // ComboBox //////////////////////////////////////////////////////////////////////////////////////////////////////
		    GetmHpVarStruct()->hWndCurCtrl=GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
    }
	if(!IsCtrlReady(hWndActive,GetmHpVarStruct()->hWndCurCtrl)) return FALSE;
    if(((int)(SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_FINDSTRINGEXACT,(WPARAM)NO,(LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO))))==CB_ERR)
	{	RECT rect;
        GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&rect);
		GetmHpVarStruct()->XnextPos=(int)(rect.right-8);
		GetmHpVarStruct()->YnextPos=(int)(((rect.bottom-rect.top)/2)+rect.top);
		GetmHpVarStruct()->YnextPos-=2;
		if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWNLIST)==CBS_DROPDOWNLIST)
		{	GetmHpVarStruct()->CourantTask->SetmHpStep(14);
			GetmHpVarStruct()->iSkipSnd=2;
		}
		else
		{	char lpTemp[MAX_RS_SIZE]={0};
            GetWindowText(GetmHpVarStruct()->hWndCurCtrl,lpTemp,MAX_RS_SIZE);
			if(!strncmp(lpTemp,GetmHpVarStruct()->CourantTask->GetMultiIndex(NO),MAX_RS_SIZE))
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(14);
				GetmHpVarStruct()->iSkipSnd=2;
			}
			else
			{	GetmHpVarStruct()->XnextPos-=15;
				if((IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_SIMPLE)==CBS_SIMPLE)&&
				   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWNLIST)!=CBS_DROPDOWNLIST)&&
				   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWN)!=CBS_DROPDOWN))
				{	GetmHpVarStruct()->YnextPos=(int)(rect.top+8);
					GetmHpVarStruct()->XnextPos+=10;
				}
				GetmHpVarStruct()->CourantTask->SetmHpStep(9);
			}
		}
	}
	else if((IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_SIMPLE)==CBS_SIMPLE)&&
			(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWNLIST)!=CBS_DROPDOWNLIST)&&
			(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWN)!=CBS_DROPDOWN))
	{	GetmHpVarStruct()->CourantTask->SetmHpStep(4);
		GetmHpVarStruct()->bBlockUpdateFrame=TRUE;
	}
	else
	{	RECT rect;
        GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&rect);
		GetmHpVarStruct()->XnextPos=(int)(rect.right-8);
		GetmHpVarStruct()->YnextPos=(int)(((rect.bottom-rect.top)/2)+rect.top);
		GetmHpVarStruct()->YnextPos-=2;
	}
	return TRUE;
}

//**************************************************************************************************************TIMERPROC//
// Combo_TimerProc
//---------------------------------------------------//
void Combo_TimerProc(HWND hWndActive)
{	if(!FillComboText(hWndActive)) SetTimer(hWndActive,ID_TIMER_COMBO,1,(TIMERPROC)mHpTimerProc);
	else
	{	if(GetmHpVarStruct()->imHpMode==2)
		{	SetWindowText(GetmHpVarStruct()->hWndCurCtrl,ComboText);
			mHpPak_InitStr(ComboText,MAX_RS_SIZE);
		}
		else
		{	// Messages "CBN_SETFOCUS" "CBN_KILLFOCUS"
			SetFocus(GetmHpVarStruct()->hWndCurCtrl);
			ForceMessage(hWndActive,CBN_EDITCHANGE);
			SetFocus(NULL);
		}
		GetmHpVarStruct()->CourantTask->SetmHpStep(15);
		GetmHpVarStruct()->iSkipSnd=1;
		NextStepTaskProc(hWndActive,FALSE);
	}
}

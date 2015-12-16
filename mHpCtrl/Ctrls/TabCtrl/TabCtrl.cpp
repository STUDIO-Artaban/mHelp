// TabCtrl.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <TabCtrl\TabCtrl.h>

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
#define		CPP_TASK_TABCTRL1			"SysTabControl"                         // C/C++ control
#define		CPP_TASK_TABCTRL2			"SysTabControl32"                       // C/C++ control
#define		VB_TASK_TABCTRL1			"TabStripWndClass"                      // VB control
#define		VB_TASK_TABCTRL2			"TabStrip20WndClass"                    // VB control
#define     DOTNET_TASK_TABCTRL         "WindowsForms10.SysTabControl32.app"    // DotNet control

//-----------------------------------------------------//
// KeyName
#define		TABCTRL_KEY					"TabCtrl"

//-----------------------------------------------------//
// ValueKeys
#define		TABSEL_KEY					"TabSel"

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration
int GetTabItemToSel(HWND);

//-----------------------------------------------------------------------------------------------------------------------//
// GetTabItemToSel
//---------------------------------------------------//
int GetTabItemToSel(HWND hWndTab)
{	char lptcitm[MAX_RS_SIZE]={0};
	TCITEM tcitem;
	int iSel=0;
	tcitem.mask=TCIF_TEXT;
	tcitem.cchTextMax=MAX_RS_SIZE;
	tcitem.pszText=lptcitm;
	while(TabCtrl_GetItem(hWndTab,iSel,&tcitem))
	{	if(!strncmp(tcitem.pszText,GetmHpVarStruct()->CourantTask->GetTabSel(),MAX_RS_SIZE)) return iSel;
		tcitem.mask=TCIF_TEXT;
		tcitem.cchTextMax=MAX_RS_SIZE;
		iSel++;
	}
	return NO;
}

//*****************************************************************************************************************OBJECT//
// Class CTabCtrlTask
CTabCtrlTask::CTabCtrlTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,char* lpCtrlName,char* lpSel)
 			 :CmHpTask(10,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,lpCtrlName)
{	mHpPak_InitStr(lpTabSel,MAX_RS_SIZE);
	strncpy(lpTabSel,lpSel,MAX_RS_SIZE);
}

CTabCtrlTask::CTabCtrlTask()
 			 :CmHpTask()
{}

BOOL CTabCtrlTask::ReadTaskKey(int index)
{	HKEY hSubTabTaskKey=NULL,hTabTaskKey=NULL;
	CmHpTask::ReadCommonKey(index,TRUE,hSubTabTaskKey,hTabTaskKey);
	if((hTabTaskKey!=NULL)&&(RegOpenKeyEx(hTabTaskKey,TABCTRL_KEY,0,KEY_READ,&hTabTaskKey)==ERROR_SUCCESS))
	{	// TabSel
		if(!mHpPak_ReadLongStrRegVal(hTabTaskKey,TABSEL_KEY,lpTabSel,MAX_RS_SIZE))
		{	mHpPak_InitStr(lpTabSel,MAX_RS_SIZE);}
		return TRUE;
	}
	return FALSE;
}

BOOL CTabCtrlTask::WriteTaskKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubTabKey=NULL,hTabKey=NULL;
	BOOL bAclRes;
	CmHpTask::WriteCommonKey(TRUE,hSubTabKey,hTabKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hTabKey!=NULL)
	{	// TabCtrl
		if(RegCreateKeyEx(hTabKey,TABCTRL_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// TabSel
		if((strncmp(lpTabSel,NULL_STRING,MAX_RS_SIZE)!=0)&&(!mHpPak_WriteLongStrRegVal(hTabKey,TABCTRL_KEY,TABSEL_KEY,lpTabSel)))
		{	return FALSE;}
		return TRUE;
	}
	return FALSE;
}

char* CTabCtrlTask::GetTabSel()
{	return lpTabSel;}

void CTabCtrlTask::SetTabSel(char* lpNewSel)
{	mHpPak_InitStr(lpTabSel,MAX_RS_SIZE);
	strncpy(lpTabSel,lpNewSel,MAX_RS_SIZE);
}

//****************************************************************************************************************MHPFILE//
// TabCtrl_ReadEvent
//---------------------------------------------------//
BOOL TabCtrl_ReadEvent(HWND hWndActive,const std::string& strPostData,UINT &iCustIdx)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpSel[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	int iCur,iCnt,iTmp,iOpt;
	char chNext=END_RS_TASK;
	BOOL bNoSel=FALSE,bError,bContinue;
	CTabCtrlTask *TabTask;
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
#ifdef MHELP_OLD
	// lpTabSel
	GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpSel,MAX_RS_SIZE,bContinue,bError);
	if((!bError)&&(!bContinue))
	{	if(lpSel[0]==RS_PARAM)
		{	iTmp=mHpPak_GetIntFromStr(lpSel,MAX_RS_SIZE,1);
			if(!DidItSelected(iTmp)) bNoSel=TRUE;
			else
			{	GetStrEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0,lpSel,MAX_RS_SIZE);
				if(iTmp==NO) return TRUE;
			}
		}
	}
	else return TRUE;
#else
    ////// TODO: Be able to remove lines below (state always in HTML)
	GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpSel,MAX_RS_SIZE,bContinue,bError);
	if((bError)||(bContinue))
	    return TRUE; // Error
    //////

    // Check if selected
    if(!IsHtmlCustSelected(strPostData,iCustIdx))
        bNoSel=TRUE;
    else
    {   // Get HTML user setting
        if(!GetHtmlCustValue(lpSel,
                             MAX_RS_SIZE,
                             strPostData,
                             TABSEL_KEY,
                             iCustIdx))
        {   return TRUE; // Error: No state parameter
        }
    }
    // Increase number of custom task
    iCustIdx++;
#endif
	if(!bNoSel)
	{	if(*lpCtrlName!=0) TabTask=new CTabCtrlTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName,lpSel);
        else TabTask=new CTabCtrlTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL,lpSel);
		GetmHpVarStruct()->TaskList->AddCellule(TabTask);
	}
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// TabCtrl_WriteEvent
//---------------------------------------------------//
void TabCtrl_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
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
// TabCtrl_WriteParam
//---------------------------------------------------//
BOOL TabCtrl_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}			

//*******************************************************************************************************************KEYS//
// TabCtrl_ReadEventKeys
//---------------------------------------------------//
void TabCtrl_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpParam* ReadParam;
	ReadTask=new CTabCtrlTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL)
	{	ReadParam=new CmHpParam();
		if(!ReadParam->ReadParamKey(Index))
		{	delete(ReadParam);
			ReadParam=NULL;
		}
		EvntObj->AddEvent(ReadTask,NULL,ReadParam);
	}
	else TaskList->AddCellule(ReadTask);
}

//********************************************************************************************************************ADD//
// TabCtrl_AddEvent
//---------------------------------------------------//
void TabCtrl_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndTab,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
    int lpctrl[MAX_PART_TREE]={0};
    if(*lpCtrlName==0)
    	AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndTab));
    char lpSelTab[MAX_RS_SIZE]={0};
	int iSel=(int)SendMessage(hWndTab,TCM_GETCURSEL,0,0);
	if(iSel!=NO)
	{	char lptcitm[MAX_RS_SIZE]={0};
	    TCITEM pitem;
	    pitem.mask=TCIF_TEXT;
	    pitem.cchTextMax=MAX_RS_SIZE;
	    pitem.pszText=lptcitm;
        if(TabCtrl_GetItem(hWndTab,iSel,&pitem))
		{	strncpy(lpSelTab,pitem.pszText,MAX_RS_SIZE);
		}
		else mHpPak_InitStr(lpSelTab,MAX_RS_SIZE);
	}
	else mHpPak_InitStr(lpSelTab,MAX_RS_SIZE);	
    if(*lpCtrlName!=0) Item=new CTabCtrlTask(NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName,lpSelTab);
    else Item=new CTabCtrlTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,lpSelTab);
	if(bWrite)
	{	Item->WriteTaskKey();
		GetmHpVarStruct()->EventList->AddEvent(Item,NULL,NULL);
	}
	else
	{	RemoveLastKeyPos();
		Item->WriteTaskKey();
		GetmHpVarStruct()->EventList->ReplaceEvent(Item,NULL);
	}
}

//*************************************************************************************************************PROPERTIES//
// TabCtrl_IsClassName
//---------------------------------------------------//
BOOL TabCtrl_IsClassName(char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_TABCTRL2,ClassSize))||
	   (!strncmp(CtrlClass,CPP_TASK_TABCTRL1,ClassSize))||
	   (!strncmp(CtrlClass,VB_TASK_TABCTRL1,ClassSize))||
	   (!strncmp(CtrlClass,VB_TASK_TABCTRL2,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_TABCTRL,sizeof(DOTNET_TASK_TABCTRL)-1)))
	{	return TRUE;}
	return FALSE;
}

//---------------------------------------------------//
// TabCtrl_IsCustom
//---------------------------------------------------//
void TabCtrl_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//---------------------------------------------------//
// TabCtrl_StrState
//---------------------------------------------------//
void TabCtrl_StrState(CEventCell* pCurCell,char* lpState,int StateSize)
{	strncpy(lpState,pCurCell->mHpTask->GetTabSel(),StateSize);}

//---------------------------------------------------//
// TabCtrl_ParamType
//---------------------------------------------------//
int TabCtrl_ParamType()
{	return 4; // S
}

//---------------------------------------------------//
// TabCtrl_DefaultBound
//---------------------------------------------------//
void TabCtrl_DefaultBound(char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	strncpy(pStrBound,STRING_LIMITE,BoundSize);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
	strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
}

//-----------------------------------------------------//
// TabCtrl_IsCustomized
//-----------------------------------------------------//
BOOL TabCtrl_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------//
// TabCtrl_IsNoStateMsg
//-----------------------------------------------------//
BOOL TabCtrl_IsNoStateMsg()
{	return TRUE;}

//-----------------------------------------------------//
// TabCtrl_GetHtmlTag
//-----------------------------------------------------//
#define TABCTRL_HTML_TAG    "\t<input type=\"text\" name=\"%d-%s\" maxlength=\"100\" value=\"%s\"/>\r\n"

void TabCtrl_GetHtmlTag(CEventCell* pCurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{
    _snprintf(lpHtmlTag,
              iMaxSize,
              TABCTRL_HTML_TAG,
              iCustIndex,
              TABSEL_KEY,
              pCurCell->mHpTask->GetTabSel());
}

//***************************************************************************************************************CFGTOOLS//
// TabCtrl_ShowCfgTools
//---------------------------------------------------//
void TabCtrl_ShowCfgTools(HWND hWndActive)
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
// TabCtrl_HideCfgTools
//---------------------------------------------------//
void TabCtrl_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
}				

//---------------------------------------------------//
// TabCtrl_SaveCfg
//---------------------------------------------------//
void TabCtrl_SaveCfg(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	GetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lpTmp,MAX_RS_SIZE);
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetTabSel(lpTmp);
}

//---------------------------------------------------//
// TabCtrl_LoadCfg
//---------------------------------------------------//
void TabCtrl_LoadCfg(HWND hWndActive)
{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetTabSel());
}

//---------------------------------------------------//
// TabCtrl_GetInfosCfg
//---------------------------------------------------//
void TabCtrl_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_TABCTRL,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_TABCTRL,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_TABCTRL,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_TABCTRL,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TABCTRL)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// TabCtrl_ProcCfg
//---------------------------------------------------//
BOOL TabCtrl_ProcCfg(UINT Message,WPARAM wParam,HWND hWndChng)
{	if(Message==WM_COMMAND)
	{	if(LOWORD(wParam)==IDC_EDIT_STATE)
		{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
			return TRUE;
		}
	}
	return FALSE;
}

//****************************************************************************************************************PROCESS//
// TabCtrl_CaptureMsg
//---------------------------------------------------//
void TabCtrl_CaptureMsg(HWND hWndActive,char* lpCtrlName,HWND hWndCtl,WPARAM wpParam,LPARAM lpParam,HWND hToolWnd,int iCurOp,CEventListe* EventObj,
						char* lpNameApp,UINT &pIdLast,int pCurwnd,int pCntwnd,BOOL &pActionUser,int &pResCapt)
{	int iWriteRes=0;
	if(((LPNMHDR)lpParam)->code==TCN_SELCHANGE)
	{	WriteImmediatTask(hWndActive,lpCtrlName,hWndCtl,hToolWnd,NULL,iCurOp,10,(UINT)((int)wpParam),EventObj,
                          NULL,NULL,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
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
// TabCtrl_mHpProcess
//---------------------------------------------------//
void TabCtrl_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2:
		{	if(TabCtrl_GetCurSel(GetmHpVarStruct()->hWndCurCtrl)!=GetTabItemToSel(GetmHpVarStruct()->hWndCurCtrl))
			{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
				GetmHpVarStruct()->CursorType=1;
				SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			}
			else
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(5);
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 3:
		{	if(!PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS)))
			{	SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);}
			else SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			EndObjects(hWndActive);
			DoInit(hWndActive);
			if((GetmHpVarStruct()->imHpMode==2)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()==NO))
			{	SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);}
			else	
			{	GetmHpVarStruct()->CourantIDCommand=GetmHpVarStruct()->CourantTask->GetCtrlID();
				if((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=NO)&&
				   ((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=iCurrentWnd)||(GetmHpVarStruct()->CourantTask->GetCountWnd()!=iCountWnd)))
				{	bChangeWnd=TRUE;}
				if(GetmHpVarStruct()->CourantTask->GetCurrentWnd()==NO)
				{	GetmHpVarStruct()->bNoInactiveMsg=TRUE;
					if(GetmHpVarStruct()->TaskList->GetNextmHpID()!=NO)
					{	GetmHpVarStruct()->TaskList->NextCellule();
						if(GetmHpVarStruct()->imHpMode!=1)
						{	WriteCursorPoskey(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur);}
					}
					else StopmHelp(hWndActive,TRUE,TRUE);
				}
				TabCtrl_SetCurFocus(GetmHpVarStruct()->hWndCurCtrl,GetTabItemToSel(GetmHpVarStruct()->hWndCurCtrl));
				if(bChangeWnd)
				{	mHpPak_ToolTipsActivate(TRUE);
					ForceNotifyMsg(hWndActive,NM_CLICK);
				}
				else
				{	ForceNotifyMsg(hWndActive,NM_CLICK);
					SetTimer(hWndActive,ID_TIMER_DELAI,10,(TIMERPROC)mHpTimerProc);
				}
			}
			break;
		}
		case 5:
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetDDrawObj(hWndActive,0);
			GetmHpVarStruct()->CourantTask->SetmHpStep(6);
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6:
		{	if((GetTabItemToSel(GetmHpVarStruct()->hWndCurCtrl)==NO)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10))
			{	ViewCtrlNotReadyMsg(hWndActive,4);}
			else SetTimer(hWndActive,ID_TIMER_DELAI,300,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7:
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
// TabCtrl_StartProc
//---------------------------------------------------//
BOOL TabCtrl_StartProc(HWND hWndActive)
{   GetmHpVarStruct()->hWndCurCtrl=NULL;
	/*if(*GetmHpVarStruct()->CourantTask->GetmHpCtrlName()!=0)
	{	if(mHpPak_IsDotNetHwnd(hWndActive))
		{   int iState=0,iStyle=0;
			GetmHpVarStruct()->hWndCurCtrl=mHpPak_GetDotNetCtrlHandle(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlName(),iStyle,iState);
		}
	}*/
    if(!GetmHpVarStruct()->hWndCurCtrl)
        GetmHpVarStruct()->hWndCurCtrl=GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
	if(!IsCtrlReady(hWndActive,GetmHpVarStruct()->hWndCurCtrl)) return FALSE;
	int iNextSel=GetTabItemToSel(GetmHpVarStruct()->hWndCurCtrl);
	if(iNextSel!=NO)
	{	RECT itemRct;
	    RECT CtrlRct;
        TabCtrl_GetItemRect(GetmHpVarStruct()->hWndCurCtrl,iNextSel,&itemRct);
		GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&CtrlRct);
		GetmHpVarStruct()->YnextPos=(int)((itemRct.bottom-6)+CtrlRct.top);
		GetmHpVarStruct()->XnextPos=(int)((itemRct.right-7)+CtrlRct.left);
	}
	else
	{	if(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10) GetmHpVarStruct()->CourantTask->SetmHpStep(5);
		else GetmHpVarStruct()->CourantTask->SetmHpStep(6);
	}
	return TRUE;
}

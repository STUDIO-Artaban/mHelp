// mHpCtrl.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#ifndef  __MHELPCTRL_H
 #include "mHpCtrl.h"
#endif

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

//-----------------------------------------------------//
#include <ComCtrl.h>

#include <stdio.h>
#include "CfgDlg.h"
#include "mHp2D.h"
#include "resource.h"

//-----------------------------------------------------//
// Définitions
#define			MAX_MHPPROCID			99999

//-----------------------------------------------------//
// Variables

//-----------------------------------------------------//
// Déclarations
int GetEventIndex(CEventListe*,int);
int GetCtrlManIconIdx(CmHpTask*);

//****************************************************************************************************************MAINDLL//

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{   switch (ul_reason_for_call)
	{	case DLL_PROCESS_ATTACH:
		{	GetmHpVarStruct()->hmHpCtrlInst=(HINSTANCE)hModule;
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		{	break;}
    }
    return TRUE;
}

//----------------------------------------------------------------------------------------------------------------GENERAL//
// GetEventIndex
//-----------------------------------------------------//
int GetEventIndex(CEventListe* EvntObject,int iCustom)
{	int iRang=0,iCtrlCnt=0,iSel=0;
	while(EvntObject->GetNextEvent(iRang,FALSE))
	{	if(EvntObject->IsCtrlState(iRang,TRUE))
		{	if(iCustom==iCtrlCnt) iSel=iRang;
			iCtrlCnt++;
		}			
		iRang++;
	}
	return iSel;
}

//-----------------------------------------------------//
// GetCtrlManIconIdx
//-----------------------------------------------------//
int GetCtrlManIconIdx(CmHpTask* pCtrlMan)
{   if(pCtrlMan->GetCtrlMgmt()!=NULL)
    {
        int iPrevIconIdx=0;
        // Get current control management index
        UINT iCurCtrlIdx=pCtrlMan->GetCtrlMgmt()->GetIndex();

        // Get icon index according control list and current control index
        UINT iCtrlMgmtIdx=0;
        CCtrlMgmtCell* CurCtrlCell=GetmHpVarStruct()->CtrlMgmtList->GetCtrlMgmtCell(iCtrlMgmtIdx);
        while((iCtrlMgmtIdx!=iCurCtrlIdx)&&(CurCtrlCell!=NULL))
        {
            iPrevIconIdx+=CurCtrlCell->iIconCount;

            // Next control management
            CurCtrlCell=GetmHpVarStruct()->CtrlMgmtList->GetCtrlMgmtCell(++iCtrlMgmtIdx);
        }

        // Check control management cell found
        if((iCtrlMgmtIdx==iCurCtrlIdx)&&(CurCtrlCell!=NULL))
        {   
            // Check valid control task icon index with icon loaded
            if((pCtrlMan->GetCtrlMgmt()->GetIconIdx()!=NO)&&((UINT)pCtrlMan->GetCtrlMgmt()->GetIconIdx()<CurCtrlCell->iIconCount))
                return (iPrevIconIdx + pCtrlMan->GetCtrlMgmt()->GetIconIdx() + 20); // 19 common control icon + 1 default icon
        }
    }
    return NO;
}

//**************************************************************************************************************VARIABLES//

MHPCTRL_API UINT mHpCtrl_iLastID=NO;
MHPCTRL_API BOOL mHpCtrl_bChangeWnd=FALSE;
MHPCTRL_API int  mHpCtrl_iCurrentWnd=0;
MHPCTRL_API int  mHpCtrl_iCountWnd=0;
MHPCTRL_API BOOL mHpCtrl_bUserAction=TRUE;
MHPCTRL_API BOOL mHpCtrl_bReadmHelp=FALSE;
MHPCTRL_API BOOL mHpCtrl_bWritemHelp=FALSE;
MHPCTRL_API BOOL mHpCtrl_bLockmHelp=FALSE;
MHPCTRL_API int  mHpCtrl_IndexTask=NO;

//*********************************************************************************************************OBJECTS-ACCESS//
CmHpCtrl::CmHpCtrl()
{}

void CmHpCtrl::NewTaskList()
{	mHpTaskList=new CTaskListe();}

void CmHpCtrl::NewEventList()
{	mHpEventList=new CEventListe();}

void CmHpCtrl::DeleteTaskList()
{	mHpTaskList->EmptyListe();
	delete(mHpTaskList);
}

void CmHpCtrl::DeleteEventList()
{	mHpEventList->EmptyEvents();
	delete(mHpEventList);
}

//********************************************************************************************************METHODES-ACCESS//
// mHpCtrl_IsCustomEvent
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_IsCustomEvent(CEventListe* EventObject)
{	return (BOOL)EventObject->GetCustEventCount();}

//-----------------------------------------------------//
// mHpCtrl_EmptyChange
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_EmptyChange(CEventListe* EventObject)
{	EventObject->EmptyChange();}

//-----------------------------------------------------//
// mHpCtrl_EmptyEvents
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_EmptyEvents(CEventListe* EventObject)
{	EventObject->EmptyEvents();}

//-----------------------------------------------------//
// mHpCtrl_IsEmptyListe
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_IsEmptyListe(CEventListe* EventObject)
{	CEventListe* EvntObj=NULL;
	if(EventObject!=NULL) return EventObject->IsEmptyListe();
	int iPos=1; // 0 -> ChangeExe
    HKEY hSubPosKey=NULL,hPosKey=NULL;
	GetmHpKeyFromPos(iPos,hSubPosKey,hPosKey,FALSE);
    // Check existing one task at least
	if(hPosKey!=NULL)
    {   RegCloseKey(hPosKey);
        return FALSE;
    }
    return TRUE;
}

//-----------------------------------------------------//
// mHpCtrl_ChangeEventWnd
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_ChangeEventWnd(CEventListe* EventObject,char* lpNameAppli)
{	CEventListe* EvntObj=new CEventListe();
	mHpCtrl_ReadEventKeys(EvntObj,FALSE);
	if(!strncmp(EvntObj->GetLastExeName(NO),lpNameAppli,MAX_RS_LISTE))
	{	EventObject->ChangeEventWnd(mHpCtrl_iCurrentWnd,mHpCtrl_iCountWnd);}
	delete(EvntObj);
}

//-----------------------------------------------------//
// mHpCtrl_RemoveLastTask
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_RemoveLastTask(CEventListe* EventObject)
{	RemoveLastKeyPos();
	EventObject->RemoveLastEvent();
}

//**********************************************************************************************************CONFIGURATION//
// mHpCtrl_GetConfig
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_GetConfig(CTaskListe* TaskListObj,int iMode,int iMouseSpeed,int iRepeat,int iDelayOpt,
								   char* mHelpFileName,int* iIndexTab,char* lpAppName,BOOL bShared)
{	static BOOL bCreateObjs=FALSE;
	int ipMode,ipSpeed,ipRepeat,ipCurLoop,ipDelayOpt;
	CGroupListe* GrpLst;
	if(!bCreateObjs)
	{	GrpLst=new CGroupListe();
		GetmHpVarStruct()->SelItemListe=GrpLst;
		bCreateObjs=TRUE;
	}
	GetmHpVarStruct()->TaskList=TaskListObj;
	mHpPak_InitStr(GetmHpVarStruct()->lpCurApp,MAX_RS_SIZE);
	strncpy(GetmHpVarStruct()->lpCurApp,lpAppName,MAX_RS_SIZE);
	if(!bShared)
	{	GetmHpVarStruct()->imHpMode=iMode;
		GetmHpVarStruct()->imHpMouseSpeed=iMouseSpeed;
		GetmHpVarStruct()->imHpRepeat=iRepeat;
		GetmHpVarStruct()->imHpDelay=iDelayOpt;
		mHpPak_InitStr(GetmHpVarStruct()->mHpFileName,MAX_RS_SIZE);
		strncpy(GetmHpVarStruct()->mHpFileName,mHelpFileName,MAX_RS_SIZE);
		mHpPak_CopyTab(GetmHpVarStruct()->IndexTab,MAX_PART_TREE,iIndexTab);
	}
	else
	{	mHpPak_ReadSettingsKey(ipMode,ipRepeat,ipCurLoop,ipSpeed,ipDelayOpt);
		GetmHpVarStruct()->imHpMode=ipMode;
		GetmHpVarStruct()->imHpRepeat=ipRepeat;
		GetmHpVarStruct()->RepeatLoop=ipCurLoop;
		GetmHpVarStruct()->imHpDelay=ipDelayOpt;
		if((!GetmHpVarStruct()->imHpMode)||(GetmHpVarStruct()->imHpMode==2)) GetmHpVarStruct()->imHpMouseSpeed=ipSpeed;
	}
}

//****************************************************************************************************************GENERAL//
// mHpCtrl_SetCtrlListHeader
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_SetCtrlListHeader(HWND hWndCtrlList,HIMAGELIST hCtrlImgLst)
{	LVCOLUMN lvcolumn;
	int iStyle;
	char lpLvcolumn[MAX_RS_SIZE]={0};
	char lpRC[MAX_RS_SIZE]={0};
	if(hCtrlImgLst!=NULL) ImageList_Destroy(hCtrlImgLst);
	hCtrlImgLst=ImageList_Create(16,16,ILC_MASK|ILC_COLOR16,6,1);
	// mHpCtrl's Icon List --------------------------------------------------------------------------------------------**//
	ComCtrl_AddIcon(hCtrlImgLst);
	//-----------------------------------------------------------------------------------------------------------------**//
	ListView_SetImageList(hWndCtrlList,hCtrlImgLst,LVSIL_SMALL);
	iStyle=(int)GetWindowLongPtr(hWndCtrlList,GWL_STYLE);
	iStyle&=~(LVS_TYPEMASK);
	iStyle|=LVS_REPORT;
	SetWindowLongPtr(hWndCtrlList,GWL_STYLE,(LONG_PTR)iStyle);
	lvcolumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvcolumn.fmt=LVCFMT_LEFT;
	lvcolumn.cx=100;
	lvcolumn.iSubItem=0;
	lvcolumn.pszText=lpLvcolumn;
	lvcolumn.cchTextMax=MAX_RS_SIZE;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_TYPE,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndCtrlList,0,&lvcolumn);
	lvcolumn.cx=160;
	lvcolumn.iSubItem=1;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_STATE,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndCtrlList,1,&lvcolumn);
	lvcolumn.cx=67;
	lvcolumn.iSubItem=2;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_APPLI,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndCtrlList,2,&lvcolumn);
	lvcolumn.cx=39;
	lvcolumn.iSubItem=3;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_ROW,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndCtrlList,3,&lvcolumn);
}		

//-----------------------------------------------------//
// mHpCtrl_SetCustomListHeader
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_SetCustomListHeader(HWND hWndList,HIMAGELIST hCustomImgLst)
{	LVCOLUMN lvcolumn;
	int iStyle;
	char lpLvcolumn[MAX_RS_SIZE]={0};
	char lpRC[MAX_RS_SIZE]={0};
	if(hCustomImgLst!=NULL) ImageList_Destroy(hCustomImgLst);
	hCustomImgLst=ImageList_Create(16,16,ILC_MASK|ILC_COLOR16,6,1);
	// mHpCtrl's Icon List --------------------------------------------------------------------------------------------**//
	ComCtrl_AddIcon(hCustomImgLst);
	//-----------------------------------------------------------------------------------------------------------------**//
	ListView_SetImageList(hWndList,hCustomImgLst,LVSIL_SMALL);
	iStyle=(int)GetWindowLongPtr(hWndList,GWL_STYLE);
	iStyle&=~(LVS_TYPEMASK);
	iStyle|=LVS_REPORT;
	SetWindowLongPtr(hWndList,GWL_STYLE,(LONG_PTR)iStyle);
	lvcolumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvcolumn.fmt=LVCFMT_LEFT;
	lvcolumn.cx=98;
	lvcolumn.iSubItem=0;
	lvcolumn.cchTextMax=MAX_RS_SIZE;
	lvcolumn.pszText=lpLvcolumn;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_TYPE,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndList,0,&lvcolumn);
	lvcolumn.cx=120;
	lvcolumn.iSubItem=1;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_STATE,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndList,1,&lvcolumn);
	lvcolumn.cx=67;
	lvcolumn.iSubItem=2;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_APPLI,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndList,2,&lvcolumn);
	lvcolumn.cx=42;
	lvcolumn.iSubItem=3;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_VISIBLE,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndList,3,&lvcolumn);	
	lvcolumn.cx=39;
	lvcolumn.iSubItem=4;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_ROW,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndList,4,&lvcolumn);
}

//-----------------------------------------------------//
// mHpCtrl_FillCtrlList
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_FillCtrlList(HWND hWndCtrlList,CEventListe* EventObject)
{	char lpRC[MAX_RS_SIZE]={0};
	int iRang=0;
	ListView_DeleteAllItems(hWndCtrlList);
	while(EventObject->GetNextEvent(iRang,FALSE))
	{	if(EventObject->GetmHpTask(iRang,FALSE)->GetmHpID()!=MHPID_CTRL_MAN)
        {   // Control name
            LoadString(GetmHpVarStruct()->hmHpCtrlInst,CtrlTabName[EventObject->GetmHpTask(iRang,FALSE)->GetmHpID()],lpRC,MAX_RS_SIZE);
		    // Type
		    AddItemList(hWndCtrlList,lpRC,EventObject->GetmHpTask(iRang,FALSE)->GetmHpID()+1,iRang);
        }
        else
        {   ////// mHelp Control Management /////////////////////////////////

            CmHpTask* pCtrlTask=EventObject->GetmHpTask(iRang,FALSE);
            // Control name
            if(pCtrlTask->GetCtrlMgmt()!=NULL)
                strncpy(lpRC,pCtrlTask->GetCtrlMgmt()->GetCtrlName(),MAX_RS_SIZE);
            // Type
            int iIconIdx=GetCtrlManIconIdx(EventObject->GetmHpTask(iRang,FALSE));
            if(iIconIdx==NO)
                iIconIdx=0; // Default icon index

            AddItemList(hWndCtrlList,lpRC,iIconIdx,iRang);

        } /////////////////////////////////////////////////////////////////////

		// State
		ListView_SetItemText(hWndCtrlList,iRang,1,EventObject->GetStrState(iRang,FALSE));
		// Appli
		if(!EventObject->GetmHpTask(iRang,FALSE)->GetmHpID())
		{	ListView_SetItemText(hWndCtrlList,iRang,2,EventObject->GetmHpTask(iRang,FALSE)->GetExeName());
		}
		else ListView_SetItemText(hWndCtrlList,iRang,2,EventObject->GetLastExeName(iRang));
		// Row
		mHpPak_InitStr(lpRC,MAX_RS_SIZE);
		_snprintf(lpRC,MAX_RS_SIZE,"%d",(iRang+1));
		ListView_SetItemText(hWndCtrlList,iRang,3,lpRC);
		iRang++;
	}
}

//-----------------------------------------------------//
// mHpCtrl_FillCustomList
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_FillCustomList(HWND hWndList,CEventListe* EventObject)
{	char lpRC[MAX_RS_SIZE]={0};
	int iRang=0,iRemove=0;
	GetmHpVarStruct()->EventList=EventObject;
	ListView_DeleteAllItems(hWndList);
	while(EventObject->GetNextEvent(iRang,FALSE))
	{	if(EventObject->GetmHpTask(iRang,FALSE)->GetmHpID()!=MHPID_CTRL_MAN)
        {   // Control name
            LoadString(GetmHpVarStruct()->hmHpCtrlInst,CtrlTabName[EventObject->GetmHpTask(iRang,FALSE)->GetmHpID()],lpRC,MAX_RS_SIZE);
            // Type
            AddItemList(hWndList,lpRC,EventObject->GetmHpTask(iRang,FALSE)->GetmHpID()+1,iRang);
        }
        else
        {   ////// mHelp Control Management /////////////////////////////////

            CmHpTask* pCtrlTask=EventObject->GetmHpTask(iRang,FALSE);
            // Control name
            if(pCtrlTask->GetCtrlMgmt()!=NULL)
                strncpy(lpRC,pCtrlTask->GetCtrlMgmt()->GetCtrlName(),MAX_RS_SIZE);
            // Type
            int iIconIdx=GetCtrlManIconIdx(EventObject->GetmHpTask(iRang,FALSE));
            if(iIconIdx==NO)
                iIconIdx=0; // Default icon index

            AddItemList(hWndList,lpRC,iIconIdx,iRang);

        } /////////////////////////////////////////////////////////////////////

		// State
		ListView_SetItemText(hWndList,iRang,1,EventObject->GetStrState(iRang,FALSE));
		// Appli
		ListView_SetItemText(hWndList,iRang,2,EventObject->GetLastExeName(iRang));
		// Visible
		if(EventObject->GetCustomVisible(iRang))
		{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_TRUE,lpRC,MAX_RS_SIZE);}
		else LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_FALSE,lpRC,MAX_RS_SIZE);
		ListView_SetItemText(hWndList,iRang,3,lpRC);
		// Row
		mHpPak_InitStr(lpRC,MAX_RS_SIZE);
		_snprintf(lpRC,MAX_RS_SIZE,"%d",(iRang+1));
		ListView_SetItemText(hWndList,iRang,4,lpRC);
		iRang++;
	}
	iRang=0;
	while(EventObject->GetNextEvent(iRang,FALSE))
	{	if(!EventObject->IsCtrlState(iRang,TRUE))
		{	ListView_DeleteItem(hWndList,(iRang-iRemove));
			iRemove++;
		}
		iRang++;
	}
}

//-----------------------------------------------------//
// mHpCtrl_EnableCfgTools
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_EnableCfgTools(CEventListe* EventObject,int iSelCtrl,HWND hWndUp,HWND hWndDown)
{	EnableWindow(hWndUp,FALSE);
	EnableWindow(hWndDown,FALSE);
	if(EventObject->GetmHpTask(iSelCtrl,FALSE)->GetmHpID()!=0)
	{	if(iSelCtrl!=0)
		{	if(iSelCtrl==1)
			{	if(!EventObject->GetmHpTask(0,FALSE)->GetCurrentWnd()) EnableWindow(hWndUp,TRUE);
			}
			else
			{	if((!EventObject->GetmHpTask((iSelCtrl-2),FALSE)->GetmHpID())&&
				   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCurrentWnd()==EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetCurrentWnd())&&
				   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCountWnd()==EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetCountWnd())&&
				   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetmHpID()!=0)&&
				   (EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetmHpID()!=0))
				{	EnableWindow(hWndUp,TRUE);}
				else if((EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCurrentWnd()==EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetCurrentWnd())&&
				   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCountWnd()==EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetCountWnd())&&
				   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCurrentWnd()==EventObject->GetmHpTask((iSelCtrl-2),FALSE)->GetCurrentWnd())&&
				   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCountWnd()==EventObject->GetmHpTask((iSelCtrl-2),FALSE)->GetCountWnd())&&
				   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetmHpID()!=0)&&
				   (EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetmHpID()!=0))
				{	EnableWindow(hWndUp,TRUE);}
			}
		}
		if((EventObject->GetmHpTask((iSelCtrl+1),FALSE)!=NULL)&&
		   (EventObject->GetmHpTask((iSelCtrl+1),FALSE)->GetmHpID()!=0)&&
		   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetmHpID()!=0))
		{	if((EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCurrentWnd()==EventObject->GetmHpTask((iSelCtrl+1),FALSE)->GetCurrentWnd())&&
			   (EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCountWnd()==EventObject->GetmHpTask((iSelCtrl+1),FALSE)->GetCountWnd()))
			{	if(iSelCtrl!=0)
				{	if(!EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetmHpID())
					{	EnableWindow(hWndDown,TRUE);}
					else if((EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCurrentWnd()==EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetCurrentWnd())&&
							(EventObject->GetmHpTask(iSelCtrl,FALSE)->GetCountWnd()==EventObject->GetmHpTask((iSelCtrl-1),FALSE)->GetCountWnd()))
					{	EnableWindow(hWndDown,TRUE);}
				}
				else if(!EventObject->GetmHpTask(0,FALSE)->GetCurrentWnd()) EnableWindow(hWndDown,TRUE);
			}
		}
	}
}

//-----------------------------------------------------//
// mHpCtrl_MoveUp
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_MoveUp(CEventListe* EventObject,int iSelCtrl)
{	EventObject->MovemHpCtrl(iSelCtrl,(iSelCtrl-1));}

//-----------------------------------------------------//
// mHpCtrl_MoveDown
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_MoveDown(CEventListe* EventObject,int iSelCtrl)
{	EventObject->MovemHpCtrl(iSelCtrl,(iSelCtrl+1));}

//-----------------------------------------------------//
// mHpCtrl_CreateCtrlCfgBox
//-----------------------------------------------------//
MHPCTRL_API int mHpCtrl_CreateCtrlCfgBox(HWND hWndActive,CEventListe* EventObj,int CtrlSel,BOOL bParamFolder)
{	GetmHpVarStruct()->EventList=EventObj;
	if(bParamFolder) GetmHpVarStruct()->iCtrlSel=GetEventIndex(EventObj,CtrlSel);
	else GetmHpVarStruct()->iCtrlSel=CtrlSel;
	GetmHpVarStruct()->bCustomFolder=bParamFolder;
	return (int)DialogBox(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDD_DLG_CFGCTRL),hWndActive,(DLGPROC)ConfigDlgProc);
}

//-----------------------------------------------------//
// mHpCtrl_HideLockCursor
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_HideLockCursor(HWND hWndActive)
{	HideLockCursor(hWndActive);}

//-----------------------------------------------------//
// mHpCtrl_GethWndActive
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_GethWndActive(HWND hWndActive)
{	GetmHpVarStruct()->hHandleExeWnd=hWndActive;}

//-----------------------------------------------------//
// mHpCtrl_IsCustomVisible
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_IsCustomVisible(CEventListe* EvntObj,int CustomIndex)
{	GetmHpVarStruct()->EventList=EvntObj;
	return EvntObj->GetCustomVisible(GetEventIndex(EvntObj,CustomIndex));
}

//-----------------------------------------------------//
// mHpCtrl_ChangeCustomVisible
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_ChangeCustomVisible(CEventListe* EvntObj,int CustomIndex)
{	GetmHpVarStruct()->EventList=EvntObj;
	if(EvntObj->GetCustomVisible(GetEventIndex(EvntObj,CustomIndex)))
	{	EvntObj->SetCustomVisible(GetEventIndex(EvntObj,CustomIndex),FALSE);}
	else EvntObj->SetCustomVisible(GetEventIndex(EvntObj,CustomIndex),TRUE);
}

//-----------------------------------------------------//
// mHpCtrl_ViewDiffWndMsg
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_ViewDiffWndMsg(HWND hWndActive)
{	ViewCtrlNotReadyMsg(hWndActive,2);}

//-----------------------------------------------------//
// mHpCtrl_FreemHpStruct
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_FreemHpStruct()
{	if(GetmHpVarStruct()->bCreateTaskObj)
	{	GetmHpVarStruct()->CtrlLink->EmptyLinkList();
		delete(GetmHpVarStruct()->CtrlLink);
	}
	FreemHpVarStruct();
}

//-----------------------------------------------------//
// mHpCtrl_AddCommonUnEvnt
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_AddCommonUnEvnt()
{	ComCtrl_AddCommonUnEvnt();
}

//-----------------------------------------------------//
// mHpCtrl_CreateCtrlMgmtList
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_CreateCtrlMgmtList(HWND hWndActive)
{   GetmHpVarStruct()->CtrlMgmtList=new CCtrlMgmtList();
    GetmHpVarStruct()->CtrlMgmtList->FillList(hWndActive);
    GetmHpVarStruct()->pLastID=new CCtrlID();
}

//-----------------------------------------------------//
// mHpCtrl_FillCtrlMgmtImgList
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_FillCtrlMgmtImgList(HIMAGELIST hImgList)
{   if(GetmHpVarStruct()->CtrlMgmtList)
    {   GetmHpVarStruct()->CtrlMgmtList->FillImgList(hImgList);
        return TRUE;
    }
    return FALSE;
}

//-----------------------------------------------------//
// mHpCtrl_FillCtrlMgmtListView
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_FillCtrlMgmtListView(HWND hWndLV)
{   if(GetmHpVarStruct()->CtrlMgmtList)
    {   GetmHpVarStruct()->CtrlMgmtList->FillListView(hWndLV);
        return TRUE;
    }
    return FALSE;
}

//-----------------------------------------------------//
// mHpCtrl_FreeCtrlMgmtList
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_FreeCtrlMgmtList()
{   delete GetmHpVarStruct()->CtrlMgmtList;
    delete GetmHpVarStruct()->pLastID;
}

//*******************************************************************************************************************KEYS//
// mHpCtrl_RefreshEventKeys
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_RefreshEventKeys(CEventListe* EventObject)
{	int index=0;
	mHpPak_ClearmHpSharedKey();
	SetOperationModeKey(2);
	while(EventObject->GetNextEvent(index,TRUE))
	{	// **** WriteTaskKey FIRST!!!
		EventObject->GetmHpTask(index,TRUE)->WriteTaskKey();
		// **** And WriteParamKey!!!
		if(EventObject->GetmHpParam(index,TRUE)!=NULL) EventObject->GetmHpParam(index,TRUE)->WriteParamKey();
		// **** And then WriteEventKey!!!
		if(EventObject->GetmHpWrite(index,TRUE)!=NULL) EventObject->GetmHpWrite(index,TRUE)->WriteEventKey();
		index++;
	}
	mHpPak_ProcWndLst->WriteAppWndKey();
}

//-----------------------------------------------------//
// mHpCtrl_WriteHwndExeKey
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_WriteHwndExeKey(HWND hWndActive,char* lpNameApp)
{	return WriteHwndExeKey(hWndActive,lpNameApp);}

//-----------------------------------------------------//
// mHpCtrl_ReadEventKeys
//-----------------------------------------------------//
MHPCTRL_API int mHpCtrl_ReadEventKeys(CEventListe* EventObject,BOOL bNewApp)
{	ReadEventKeys(EventObject);
	if(bNewApp) GetmHpVarStruct()->imHpProcID=mHpPak_ReadmHpProcIDKey();
	return GetmHpVarStruct()->imHpProcID;
}

//-----------------------------------------------------//
// mHpCtrl_ReadTaskKeys
//-----------------------------------------------------//
MHPCTRL_API int mHpCtrl_ReadTaskKeys(HWND hWndActive,CTaskListe* TaskLst,char* AppliName,BOOL bMainActive)
{	int index=0,imHpId,iRes=0;
	imHpId=GetmHpIDKey(index);
	TaskLst->EmptyListe();
	while(imHpId!=NO)
	{	// ReadTaskKeys -----------------------------------------------------------------------------------------------**//
		ComCtrl_ReadEventKeys(imHpId,
							  index,
							  NULL,
							  TaskLst);
		//-------------------------------------------------------------------------------------------------------------**//
		index++;
		imHpId=GetmHpIDKey(index);
	}
	if(index!=0)
	{	mHpCtrl_WriteHwndExeKey(hWndActive,AppliName);
		if(bMainActive) TaskLst->NextCellule();
		if(TaskLst->SetCurrentTask())
		{	if((!TaskLst->GetCurrentTask()->GetmHpID())&&(!TaskLst->GetCurrentTask()->GetNewExe())&&
			   (!strncmp(TaskLst->GetCurrentTask()->GetExeName(),AppliName,MAX_RS_SIZE)))
			{	if(!TaskLst->NextCellule())
				{	// The last task is a "ChangeExeTask" -> NextRepeatAssistProc
					iRes=2;
				}
				else iRes=1;
			}
			else iRes=1;
		}
		else
		{	// The last task is a "ChangeExeTask" and the last app has been closed -> NextRepeatAssistProc
			iRes=2;
		}
	}
	return iRes;
}

//-----------------------------------------------------//
// mHpCtrl_WriteOperationKey
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_WriteOperationKey(int iNewOp)
{	SetOperationModeKey(iNewOp);}

//****************************************************************************************************************PROCESS//
// mHpCtrl_NextStepProc
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_NextStepProc(HWND hWndActive,BOOL bDoInit,char* lpAppExe)
{	if(bDoInit) DoInit(hWndActive);
	if((GetmHpVarStruct()->CourantTask->GetCurrentWnd()==mHpCtrl_iCurrentWnd)&&
	   (GetmHpVarStruct()->CourantTask->GetCountWnd()==mHpCtrl_iCountWnd)) 
	{	mHpCtrl_bChangeWnd=FALSE;}
	mHpCtrl_WriteHwndExeKey(hWndActive,lpAppExe);
	if((mHpCtrl_bChangeWnd)&&(mHpCtrl_iCountWnd>GetmHpVarStruct()->CourantTask->GetCountWnd()))
	{	ViewCtrlNotReadyMsg(hWndActive,2);}
	else NextStepTaskProc(hWndActive,TRUE);
}

//-----------------------------------------------------//
// mHpCtrl_WndProc
//-----------------------------------------------------//
MHPCTRL_API int mHpCtrl_WndProc(HWND hWndActive,UINT Msg,WPARAM wParam,LPARAM lParam,HWND hServTool,
								 CEventListe* EventObj,char* NameApp,BOOL bWndMsg)
{	int iResCapt;
	if(mHpCtrl_bWritemHelp) //////////////////////////////////////////////////////////// mHpWrite
	{	switch(Msg)
		{	case WM_INITMENU:
			{	GetmHpVarStruct()->hCurMenu=(HMENU)wParam;
				break;
			}
			case WM_DESTROY:
			{	if(bWndMsg)
				{	if(!EventObj->IsEmptyListe())
					{	if(!mHpCtrl_iCurrentWnd)
						{	EventObj->ChangeEventWnd(NO,0);
							if(!EventObj->IsAnAppActive())
							{	return 1;
							}
						}
					}
				}
				return 6; // No Process
			}
			case WM_NCDESTROY:
			{	return 3;
			}
			case WM_ACTIVATE:
			{	if(bWndMsg)
				{	if(LOWORD(wParam)==WA_INACTIVE) return 2;
					if(LOWORD(wParam)==WA_ACTIVE) return 4;
				}
				break;
			}
			case WM_PARENTNOTIFY:
			{	if(LOWORD(wParam)==WM_CREATE) return 5;
				break;
			}
			default:
			{	break;}
		}
		if((!mHpCtrl_bUserAction)&&(wParam!=ID_TIMER_NEXT)&&(KillTimer(hWndActive,ID_TIMER_NEXT)))
		{	SetTimer(hWndActive,ID_TIMER_NEXT,1,(TIMERPROC)mHpTimerProc);}
		else if(!mHpPak_IsWaitingCursor())
		{	// Capture Msg --------------------------------------------------------------------**//
			ComCtrl_CaptureMsg(hWndActive,
							   Msg,
							   wParam,
							   lParam,
							   mHpCtrl_bUserAction,
							   mHpPak_ReadOperationKey(),
							   hServTool,
							   NameApp,
							   EventObj,
							   iResCapt,
							   mHpCtrl_iLastID,
							   mHpCtrl_iCurrentWnd,
							   mHpCtrl_iCountWnd);
			//---------------------------------------------------------------------------------**//
			return iResCapt;
		}
		return 6; // No Process
	}
	else if(mHpCtrl_bReadmHelp) ///////////////////////////////////////////////////////// mHpRead
	{	/*if(ComCtrl_LockNoNotifyDotNetCtrlMsg(Msg,lParam))
        {   // Check if this no notify DotNet message locked
            if(GetmHpVarStruct()->bLockDotNetMsg)
                GetmHpVarStruct()->bLockDotNetMsg=FALSE;
            else return 7;
        }*/
        switch(Msg)
		{	case WM_INITMENU:
			{	GetmHpVarStruct()->hCurMenu=(HMENU)wParam;
				break;
			}
			case WM_NCDESTROY:
			{	if(bWndMsg)
				{	if((!mHpCtrl_iCurrentWnd)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()==NO)&&
					   (GetmHpVarStruct()->TaskList->GetCurrentTask()->GetmHpID()!=0))
					{	StopmHelp(hWndActive,TRUE,TRUE);}
					return 3;
				}
				else return 3;
				break;
			}
			case WM_ACTIVATE:
			{	if(bWndMsg)
				{	if(LOWORD(wParam)==WA_INACTIVE)
					{	if(!GetmHpVarStruct()->bLockUnWaMsg)
                        {   if(!RestoreBackBuffer(hWndActive))
                            {   StopmHelp(hWndActive,TRUE,TRUE);
                                return 0;
                            }
                            UpdateBackBuffer(TRUE);
                            return 2;
                        }
						else GetmHpVarStruct()->bLockUnWaMsg=FALSE;
					}
					else if(LOWORD(wParam)==WA_ACTIVE)
					{	if(!GetmHpVarStruct()->bLockWaMsg) return 4;
						else GetmHpVarStruct()->bLockWaMsg=FALSE;
					}
				}
				break;
			}
			case WM_ACTIVATEAPP:
			{	int iReadOp;
				if(bWndMsg)
				{	if(((BOOL)wParam)==TRUE)
					{	iReadOp=mHpPak_ReadOperationKey();
						switch(iReadOp)
						{	case 1: // Read
							{	switch(mHpCtrl_ReadTaskKeys(hWndActive,GetmHpVarStruct()->TaskList,NameApp,FALSE))
								{	case 0:
									{	StopmHelp(hWndActive,TRUE,TRUE);
										break;
									}
									case 1:
									{	if(ReadHwndExeKey(NameApp)==GetActiveWindow())
										{	GetmHpVarStruct()->bLockWaMsg=TRUE;
											mHpCtrl_SetTimer(hWndActive,100,8); // SHARED
										}
										break;
									}
									case 2:
									{	GetmHpVarStruct()->bLockWaMsg=TRUE;
										SetTimer(hWndActive,ID_TIMER_ENDTASK,100,(TIMERPROC)mHpTimerProc);
										break;
									}
								}
								break;
							}
							default:
							{	StopmHelp(hWndActive,FALSE,TRUE);
								break;
							}
						}
					}
					else
					{	if(!GetmHpVarStruct()->bNoInactiveMsg)
						{	if(ReadHwndExeKey(NameApp)==hWndActive)
							{	EndObjects(hWndActive);
								GetmHpVarStruct()->TaskList->NextCellule();
							}
						}
						else GetmHpVarStruct()->bNoInactiveMsg=FALSE;
					}
				}
				break;
			}
			/*case WM_QUERYNEWPALETTE:
			{	OnDDrawQueryNewPal();	
				break;
			}
			case WM_LBUTTONDOWN:
			{	// Stop mHelp message
				StopmHelp(hWndActive,TRUE);
				break;
			}*/
			case WM_COMMAND:
			{	// Si == au default bouton?
				if(LOWORD(wParam)==IDOK) 
				{	if(GetmHpVarStruct()->EditFlag)
					{	NextStepTaskProc(hWndActive,FALSE);
						return 0;
					}
				}

                //
                // TODO: Compare last control ID for managed control
                //

				if(LOWORD(wParam)==GetmHpVarStruct()->CourantIDCommand)
				{	if(mHpCtrl_bChangeWnd)
                        EndObjects(hWndActive);
				}
				break;
			}
			default:
			{	break; }
		}
	}
	else //////////////////////////////////////////////////////////////////////////// No mHpProcess
	{	//if(ComCtrl_LockNoNotifyDotNetCtrlMsg(Msg,lParam))
        //    return 7;
        switch(Msg)
		{	case WM_NCDESTROY:
			{	return 3;
			}
			case WM_ACTIVATE:
			{	if(bWndMsg)
				{	if(LOWORD(wParam)==WA_INACTIVE) return 2;
					if(LOWORD(wParam)==WA_ACTIVE) return 4;
				}
				break;
			}
			default:
			{	break;}
		}
	}
	return 6; // No Process
}

//-----------------------------------------------------//
// mHpCtrl_WriteAppName
//-----------------------------------------------------//
MHPCTRL_API int mHpCtrl_WriteAppName(CEventListe* EventObject,char* lpAppName,BOOL bNewProc)
{	int iNewProcID=0;
	CmHpTask* AppTask=CreateChangeExeObj(TRUE,NULL,lpAppName,NULL);
	AppTask->WriteTaskKey();
	EventObject->AddEvent(AppTask,NULL,NULL);
	if(bNewProc)
	{	iNewProcID=mHpPak_ReadmHpProcIDKey();
		iNewProcID++;
		if(iNewProcID>=MAX_MHPPROCID) iNewProcID=1;
		mHpPak_WritemHpProcIDKey(iNewProcID);
		GetmHpVarStruct()->imHpProcID=iNewProcID;
	}
	return iNewProcID;
}

//-----------------------------------------------------//
// mHpCtrl_AddIfChangeExe
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_AddIfChangeExe(CEventListe* EventObject,char *lpAppName)
{	AddIfChangeExe(EventObject,lpAppName);}

//-----------------------------------------------------//
// mHpCtrl_SetDotNetCtrlNotify
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_SetDotNetCtrlNotify(HWND hWndCtrl)
{   return ComCtrl_AddDotNetCtrlNotify(hWndCtrl);
}

//-----------------------------------------------------//
// mHpCtrl_SetPlatformCtrlNotify
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_SetPlatformCtrlNotify(HWND hWndActive)
{   return ComCtrl_AddPlatformCtrlNotify(hWndActive);
}

//******************************************************************************************************************WRITE//
// mHpCtrl_WriteCustomEvent
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_WriteCustomEvent(HWND hWndActive,HWND hWndCtrlList,CEventListe* EventObject,char* mHpFileName,int iRefTab,int Line)
{	char lpTmp[MAX_RS_SIZE]={0};
	int indice=0;
	if(!mHpPak_InsertInmHpFile(hWndActive,mHpFileName,(Line+1),FALSE,FALSE)) return FALSE;
	while(indice!=iRefTab)
	{	// '|'
		if(!mHpPak_WriteParty(hWndActive,NEXT_RS,NO,NULL,FALSE)) return FALSE;
		indice++;
	}
	for(int i=0;i<ListView_GetItemCount(hWndCtrlList);i++)
	{	if(EventObject->GetCustomVisible(i))
		{	// WriteParam ---------------------------------------------------------------------------------------------**//
			if(!ComCtrl_WriteParam(hWndActive,i,EventObject))
			{	return FALSE;}
			//---------------------------------------------------------------------------------------------------------**//
		}
	}
	// '\n'
	if((!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))||(!mHpPak_ConcatInmHpFile(hWndActive,mHpFileName,(Line+1),FALSE)))
	{	return FALSE;}
	return TRUE;
}

//-----------------------------------------------------//
// mHpCtrl_WriteEventTask
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_WriteEventTask(HWND hWndActive,char* lpmHpFileName,CEventListe* EventObject,
                                        CWindowListe* CurWndLst,BOOL bParamFolder)
{	int iParamIndex=0;
	char lpCurExeName[MAX_RS_SIZE]={0};
	int iLine=mHpPak_GetTaskLine(hWndActive,lpmHpFileName,FALSE);
	int index=0;
	// bRes=FALSE -> ComCtrl_WriteTask (Linked controls)
	BOOL bRes=FALSE;
	if(EventObject->GetNextEvent(index,TRUE)) strncpy(lpCurExeName,EventObject->GetmHpTask(index,TRUE)->GetExeName(),MAX_RS_SIZE);
	else return FALSE;			
	if(iLine!=NO) iLine--;
	else return FALSE;
	if((!mHpPak_InsertInmHpFile(hWndActive,lpmHpFileName,iLine,TRUE,FALSE))||
	   (!mHpPak_WriteParty(hWndActive,NEXT_TASK,NO,lpCurExeName,TRUE))) // '_'
	{	return FALSE;}
	while(EventObject->GetNextEvent(index,FALSE))
	{	// WriteTask --------------------------------------------------------------------------------------------------**//
		ComCtrl_WriteTask(hWndActive,index,lpCurExeName,MAX_RS_SIZE,EventObject,CurWndLst,bParamFolder,iParamIndex,bRes);
		if(!bRes)
		{	return FALSE;}
		//-------------------------------------------------------------------------------------------------------------**//
		index++;
        //Sleep(1);
	}
    // '\n'
	if((!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))||
       (!mHpPak_ConcatInmHpFile(hWndActive,lpmHpFileName,(iLine+1),FALSE)))
	{	return FALSE;}
	return TRUE;
}

//-----------------------------------------------------//
// mHpCtrl_WriteWindowLst
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_WriteWindowLst(HWND hWndActive,char* lpmHpFileName,CWindowListe* WriteWndLst,BOOL bEmpty)
{	char lpTmpApp[MAX_RS_SIZE]={0};
	int indice=0,index=0,iCntApp=0,iLine=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFileName);
	if(iLine==NO) return FALSE;
	iLine--;
	if(!bEmpty)
	{	if(!mHpPak_GetWindowList(hWndActive,lpmHpFileName,WriteWndLst)) return FALSE;
		//WriteWndLst->View();
		mHpPak_ProcWndLst->FusionWndLst(WriteWndLst);
		iCntApp=mHpPak_GetAppWindowCnt(hWndActive,lpmHpFileName);
		if(iCntApp==NO) return FALSE;
	}
	else
	{	mHpPak_ProcWndLst->InitCurWnd();
		WriteWndLst->CopyWindowList(mHpPak_ProcWndLst);
	}
	while(WriteWndLst->GetWndCellFrom(indice,0)!=NULL)
	{	if(!mHpPak_InsertInmHpFile(hWndActive,lpmHpFileName,iLine,TRUE,FALSE)) return FALSE;
		if((!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))|| // '\n'
		   (!mHpPak_WriteParty(hWndActive,START_TASK,NO,NULL,FALSE))) // '#'
		{	return FALSE;}
        //Sleep(1);
        if(!mHpPak_WriteParty(hWndActive,NEXT_TASK,NO,WriteWndLst->GetWndAppName(indice),TRUE)) // '_'
		{	return FALSE;}
        index=0;
        while(WriteWndLst->GetWndCellFrom(indice,index)!=NULL)
		{	if((!mHpPak_WriteParty(hWndActive,NEXT_TASK,NO,WriteWndLst->GetWndClass(indice,index),TRUE))|| // '_'
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteWndLst->GetWndTitle(indice,index),TRUE))) // '¤'
			{	return FALSE;}
			index++;
            //Sleep(1);
		}
		if((!bEmpty)&&(iCntApp>=indice)) iLine++;
		// '\n'
		if((!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))||(!mHpPak_ConcatInmHpFile(hWndActive,lpmHpFileName,(iLine+1),FALSE)))
		{	return FALSE;}
		indice++;
        //Sleep(1);
	}
	return TRUE;
}

//-----------------------------------------------------//
// mHpCtrl_WriteCtrlList
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_WriteCtrlList(HWND hWndActive,char* lpmHpFileName,BOOL bEmptyList)
{   int iLine=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFileName);
	if(iLine==NO)
        return FALSE;
    iLine-=(bEmptyList)? 1:2; // Due to control list flag in file: '@' (not '#' which is managed differently)
                              // Same remark at line parameter of mHpPak_ConcatInmHpFile() call below
    if(!mHpPak_InsertInmHpFile(hWndActive,lpmHpFileName,iLine,TRUE,FALSE))
        return FALSE;
    if((!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))|| // '\n'
       (!mHpPak_WriteParty(hWndActive,START_CTRL,NO,NULL,FALSE))) // '@'
        return FALSE;
    UINT index=0;
    const CControlCell* CurCtrlCell=mHpPak_ProcCtrlList->GetCtrlCell(index);
    while(CurCtrlCell!=NULL)
    {   if((!mHpPak_WriteParty(hWndActive,NEXT_TASK,CurCtrlCell->iPlatformID,NULL,TRUE))|| // '_'
           (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,CurCtrlCell->lpClassName,TRUE))|| // '¤'
           (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,CurCtrlCell->lpVersionDll,TRUE))) // '¤'
        {   return FALSE;
        }
        CurCtrlCell=mHpPak_ProcCtrlList->GetCtrlCell(++index);
    }
    // '\n'
    if((!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))||
       (!mHpPak_ConcatInmHpFile(hWndActive,lpmHpFileName,iLine+((bEmptyList)? 1:2),FALSE)))
    {   return FALSE;
    }
    return TRUE;
}

//-----------------------------------------------------//
// mHpCtrl_WriteFavsFile
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_WriteFavsFile(HWND hWndActive,char* lpmHpFileName,CFavListe* WriteFavsFile,int iLastCnt)
{	BOOL bNoFavs=FALSE,bRepeat=TRUE;
	int iSel,iType,iRepeat,index,iSelect,indice=0,iEndLine=mHpPak_GetFavsLineCnt(hWndActive,lpmHpFileName);
	if(iEndLine==NO) return FALSE;
	mHpPak_AddNewEndFile(hWndActive,lpmHpFileName,iEndLine,bNoFavs);
	if(iEndLine==NO) return FALSE;
	if((!bNoFavs)&&(!WriteFavsFile->GetFavCount())) iEndLine--;
	if(!mHpPak_InsertInmHpFile(hWndActive,lpmHpFileName,iEndLine,FALSE,TRUE)) return FALSE;
	while(indice!=WriteFavsFile->GetFavCount())
	{	if((bNoFavs)&&(!indice))
		{	if((!mHpPak_WriteParty(hWndActive,END_FILE,NO,NULL,FALSE))|| // '¨'
			   (!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))) // '\n'
			{	return FALSE;}
		}
		if((!mHpPak_WriteParty(hWndActive,START_TASK,WriteFavsFile->GetLoopOption(indice),NULL,TRUE))|| // '#'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteFavsFile->GetEventStr(indice),TRUE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,NEXT_TASK,NO,WriteFavsFile->GetEventRef(indice),FALSE))) // '_'
		{	return FALSE;}
		if(WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList!=NULL)
		{	index=0;
			while(index!=WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetSelCount())
			{	iSel=WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetSelItem(index);
				iType=WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetItemType(index);
				if((!mHpPak_WriteParty(hWndActive,NEXT_TASK,iSel,NULL,TRUE))|| // '_'
				   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,iType,NULL,TRUE))) // '¤'
				{	return FALSE;}
				iRepeat=0;
				bRepeat=TRUE;
				while((iRepeat!=WriteFavsFile->GetLoopOption(indice))&&(bRepeat))
				{	if(iType>=5) // ListSel
					{	iSelect=0;
						switch(iType)
						{	case 5: // LI
							case 8: // LB
							{	while(WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetIntParam(iRepeat,iSelect)!=NO)
								{	if(!iSelect)
									{	if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetIntParam(iRepeat,iSelect),NULL,TRUE)) // '¤'
										{	return FALSE;}
									}
									else
									{	if(!mHpPak_WriteParty(hWndActive,ITEM_GROUP,WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetIntParam(iRepeat,iSelect),NULL,TRUE)) // '^'
										{	return FALSE;}
									}
									iSelect++;
								}
								break;
							}
							case 6:
							case 7:
							case 9:
							{	while(WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetStrParam(iRepeat,iSelect)!=NULL)
								{	if(!iSelect)
									{	if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetStrParam(iRepeat,iSelect),TRUE)) // '¤'
										{	return FALSE;}
									}
									else
									{	if(!mHpPak_WriteParty(hWndActive,ITEM_GROUP,NO,WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetStrParam(iRepeat,iSelect),TRUE)) // '^'
										{	return FALSE;}
									}
									iSelect++;
								}
								break;
							}
						}
					}
					else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetStringParam(iRepeat,iType),TRUE)) // '¤'
					{	return FALSE;}
					bRepeat=WriteFavsFile->GetItemFromRang(indice)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetEvoluFlag();
					iRepeat++;
				}
				index++;
			}
		}
		// '\n'
		if(!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))
		{	return FALSE;}
		indice++;
	}
	if(!bNoFavs)
	{	iEndLine+=iLastCnt;
		if(!indice) iEndLine++;
	}
	if(!mHpPak_ConcatInmHpFile(hWndActive,lpmHpFileName,iEndLine,TRUE)) return FALSE;
	return TRUE;
}

//-----------------------------------------------------//
// mHpCtrl_InitLastCtrlID
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_InitLastCtrlID()
{   // Initialize last control ID for CtrlMan
    *GetmHpVarStruct()->szLastClass=0;
    GetmHpVarStruct()->iLastStyle=NO;
    GetmHpVarStruct()->pLastID->ClearValue();
}

//*******************************************************************************************HTML//
#define HTML_SINGLE_TAG     "<form action=\"mHpStart\" method=\"post\">\r\n" \
                            "<table style=\"border-style: solid; border-color: #E0E0E0\" border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=\"100%%\">\r\n" \
                            "<tr>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\">\r\n" \
                            "\t<font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font>\r\n" \
                            "</td>\r\n" \
                            "</tr>\r\n" \
                            "<tr>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" align=\"right\">\r\n" \
                            "\t<input type=\"hidden\" name=\"mHpID\" value=\"%s\"/>\r\n" \
                            "\t<input type=\"hidden\" name=\"mHpIdx\" value=\"%d\"/>\r\n" \
                            "\t<input type=\"submit\" value=\"%s\"/>\r\n" \
                            "</tr>\r\n" \
                            "</td>\r\n" \
                            "</table>\r\n" \
                            "</form>\r\n"

//////
#define HTML_ONEPARAM_TAG   "<form action=\"mHpStart\" method=\"post\">\r\n" \
                            "<table style=\"border-style: solid; border-color: #E0E0E0\" border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=\"100%%\">\r\n" \
                            "<tr>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" colspan=\"2\">\r\n" \
                            "\t<font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font>\r\n" \
                            "</td>\r\n" \
                            "</tr>\r\n" \
                            "<tr>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\">\r\n%s" \
                            "</td>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" valign=\"top\">\r\n" \
                            "\t<font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font>\r\n" \
                            "</td>\r\n" \
                            "</tr>\r\n" \
                            "<tr>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" align=\"right\" colspan=\"2\">\r\n" \
                            "\t<input type=\"hidden\" name=\"0-ParamSel\" value=\"on\"/>\r\n" \
                            "\t<input type=\"hidden\" name=\"mHpID\" value=\"%s\"/>\r\n" \
                            "\t<input type=\"hidden\" name=\"mHpIdx\" value=\"%d\"/>\r\n" \
                            "\t<input type=\"submit\" value=\"%s\"/>\r\n" \
                            "</td>\r\n" \
                            "</tr>\r\n" \
                            "</table>\r\n" \
                            "</form>\r\n"

//////
#define HTML_SEVPARAM_TAG1  "<form action=\"mHpStart\" method=\"post\">\r\n" \
                            "<table style=\"border-style: solid; border-color: #E0E0E0\" border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=\"100%%\">\r\n" \
                            "<tr>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" colspan=\"3\">\r\n" \
                            "\t<font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font>\r\n" \
                            "</td>\r\n" \
                            "</tr>\r\n"

#define HTML_SEVPARAM_TAG2  "<tr>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" valign=\"top\" width=\"20\">\r\n" \
                            "\t<input type=\"checkbox\" name=\"%d-ParamSel\" checked/>\r\n" \
                            "</td>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" valign=\"top\">\r\n%s" \
                            "</td>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" valign=\"top\">\r\n" \
                            "\t<font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font>\r\n" \
                            "</td>\r\n" \
                            "</tr>\r\n"

#define HTML_SEVPARAM_TAG3  "<tr>\r\n" \
                            "<td style=\"border-style: solid; border-color: #E0E0E0\" align=\"right\" colspan=\"3\">\r\n" \
                            "\t<input type=\"hidden\" name=\"mHpID\" value=\"%s\"/>\r\n" \
                            "\t<input type=\"hidden\" name=\"mHpIdx\" value=\"%d\"/>\r\n" \
                            "\t<input type=\"submit\" value=\"%s\"/>\r\n" \
                            "</td>\r\n" \
                            "</tr>\r\n" \
                            "</table>\r\n" \
                            "</form>\r\n"

//-----------------------------------------------------//
// mHpCtrl_InsertHtmlTag
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_InsertHtmlTag(HWND hWndActive,CEventListe* EventObject,const char* lpHtmlFile,
                                       char* lpmHpFile,int* lpIndexTab,const char* lpmHpID)
{   // Get next mHelp task index
    UINT imHpIdx=0;
    int iTaskLine=NO;
    mHpPak_FindEventLine(hWndActive,lpmHpFile,lpIndexTab,iTaskLine);
    while(iTaskLine!=NO)
    {   iTaskLine++;
        mHpPak_SetViewMsgFlag(FALSE); // Do not display error message
        mHpPak_FindEventLine(hWndActive,lpmHpFile,lpIndexTab,iTaskLine);
        if(iTaskLine!=NO) // Next task found
            imHpIdx++;
    }
    mHpPak_SetViewMsgFlag(TRUE);
    mHpPak_ClosemHpFile();

    // Load HTML string from resource
    char lpSubmit[MAX_SHT_SIZE]={0};
    LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_HTML_SUBMIT,lpSubmit,MAX_SHT_SIZE);
    char lpTaskDesc[MAX_KEY_SIZE]={0};
    LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_HTML_TASKDESC,lpTaskDesc,MAX_SHT_SIZE);
    char lpStateDesc[MAX_KEY_SIZE]={0};
    LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_HTML_STATEDESC,lpStateDesc,MAX_SHT_SIZE);

    // Check existing custom task contained in event list
    UINT iCustCount=EventObject->GetCustEventCount();
    char lpHtmlTag[MAX_RS_LISTE]={0};
    switch(iCustCount)
    {   case 0: // No custom task found
        {
            // Add single HTML form
            _snprintf(lpHtmlTag,
                      MAX_RS_LISTE,
                      HTML_SINGLE_TAG,
                      lpTaskDesc,   // Add task description here...
                      lpmHpID,      // mHpID
                      imHpIdx,      // mHpIdx
                      lpSubmit);    // Submit label
            break;
        }
        case 1: // One custom task into event list
        {
            char lpCtrlTag[MAX_LONG_SIZE];
            if(!EventObject->GetNextHtmlTag(0,lpCtrlTag,MAX_LONG_SIZE))
            {
                //
                // TODO: Add error message
                // -> Display managed control error message
                //

                assert(NULL);
                return FALSE;
            }

            // Add one custom task HTML form
            _snprintf(lpHtmlTag,
                      MAX_RS_LISTE,
                      HTML_ONEPARAM_TAG,
                      lpTaskDesc,   // Add task description here...
                      lpCtrlTag,    // Add control HTML tag
                      lpStateDesc,  // Add control state description here...
                      lpmHpID,      // mHpID
                      imHpIdx,      // mHpIdx
                      lpSubmit);    // Submit label
            break;
        }
        default: // Several custom task into event list
        {
            // Add HTML custom task header form
            _snprintf(lpHtmlTag,
                      MAX_RS_LISTE,
                      HTML_SEVPARAM_TAG1,
                      lpTaskDesc);  // Add task description here...

            // Loop to add custom task HTML form
            for(UINT i=0; i<iCustCount; i++)
            {
                char lpCtrlTag[MAX_LONG_SIZE];
                if(!EventObject->GetNextHtmlTag(i,lpCtrlTag,MAX_LONG_SIZE))
                {
                    //
                    // TODO: Add error message
                    // -> Display managed control error message
                    //

                    assert(NULL);
                    return FALSE;
                }

                char lpCtrlForm[MAX_LONG_SIZE];
                _snprintf(lpCtrlForm,
                          MAX_LONG_SIZE,
                          HTML_SEVPARAM_TAG2,
                          i,            // Custom task index
                          lpCtrlTag,    // Add control HTML tag
                          lpStateDesc); // Add control state description here...

                strncat(lpHtmlTag,lpCtrlForm,strlen(lpCtrlForm));
            }

            // Add submit input
            char lpSubForm[MAX_LONG_SIZE];
            _snprintf(lpSubForm,
                      MAX_LONG_SIZE,
                      HTML_SEVPARAM_TAG3,
                      lpmHpID,      // mHpID
                      imHpIdx,      // mHpIdx
                      lpSubmit);    // Submit label

            strncat(lpHtmlTag,lpSubForm,strlen(lpSubForm));
            break;
        }
    }

    // Insert HTML tag into the file
    return mHpPak_InsertHtmlTag(hWndActive,lpHtmlFile,lpHtmlTag);
}

//-----------------------------------------------------//
// mHpCtrl_SetStartPostData
//-----------------------------------------------------//
MHPCTRL_API void mHpCtrl_SetStartPostData(const std::string& strPostData)
{   ComCtrl_SetStartPostData(strPostData);
}

//******************************************************************************************************************TIMER//
// mHpCtrl_SetTimer
//-----------------------------------------------------//
MHPCTRL_API int mHpCtrl_SetTimer(HWND hWndActive,int iDelay,int iTimerIndex)
{	UINT TimerID=GetmHpTimerID(iTimerIndex);
	return (int)SetTimer(hWndActive,(UINT_PTR)TimerID,iDelay,(TIMERPROC)mHpTimerProc);
}

//-----------------------------------------------------//
// mHpCtrl_KillTimer
//-----------------------------------------------------//
MHPCTRL_API BOOL mHpCtrl_KillTimer(HWND hWndActive,int iTimerIndex)
{	UINT TimerID=GetmHpTimerID(iTimerIndex);
	return KillTimer(hWndActive,TimerID);
}

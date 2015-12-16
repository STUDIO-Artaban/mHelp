// ListView.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <ListView\ListView.h>

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
#define		CPP_TASK_LISTVIEW			"SysListView32"                     // C/C++ control
#define		VB_TASK_LISTVIEW			"ListViewWndClass"                  // VB control
#define     DOTNET_TASK_LISTVIEW        "WindowsForms10.SysListView32.app"  // DotNet control

//-----------------------------------------------------//
// KeyName
#define		LISTVIEW_KEY				"ListView"

//-----------------------------------------------------//
// ValueKeys
#define		ITEMSEL_KEY					"ItemSel"

//-----------------------------------------------------//
// Définitions
#define		ECART_SCROLL_SMALL			16
#define		ECART_SCROLL_NORMAL			32
#define		ID_TIMER_WAIT_LVMSG			303

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles
int LVcur=0;
int LVcnt=0;
int LVtype=0;
char LVname[MAX_CTRL_NAME]={0};
HWND LVhwnd=NULL;
BOOL bUnSelAll=FALSE;
int iNextSel=NO;
int iShiftOp=2;			// 0: 1°step of shift operation, 1: 2°step of shift operation, 2: NextSel
int iAltGrOp=0;			// 0: No AltGr, 1: AltGr anim, 2: AltGr image
BOOL bFindAll=FALSE;	// TRUE: All item to select have been found, FALSE: orelse

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration
int GetNextShiftSel(HWND);
void DoShiftSelection(HWND);
void GetLvItemRect(HWND,int,RECT*);
void GetNextItemPosCur(HWND,HWND,int,RECT);
BOOL IsShiftOperation();
BOOL IsAlreadySelected(int*);
BOOL IsMoreSelThanNoSel(int*);
void ListView_GetSelItems(HWND,int*,int);
int GetSLVItemToSel(HWND);
void CALLBACK LVTimerProc(HWND,UINT,UINT,DWORD);

//-----------------------------------------------------------------------------------------------------------------------//
// GetNextShiftSel
//---------------------------------------------------//
int GetNextShiftSel(HWND hWndMLV)
{	int iCurAct,iLastSel;
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	iCurAct=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	while(iCurAct!=NO)
	{	iLastSel=iCurAct;
		iCurAct=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	}
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	return iLastSel;
}

//---------------------------------------------------//
// DoShiftSelection
//---------------------------------------------------//
void DoShiftSelection(HWND hWndMLV)
{	int iFirstSel=0;
	while(ListView_GetItemState(hWndMLV,iFirstSel,LVIS_SELECTED)!=LVIS_SELECTED) iFirstSel++;
	iFirstSel++;
	while(iNextSel!=iFirstSel)
	{	ListView_SetItemState(hWndMLV,iFirstSel,LVIS_SELECTED,LVIS_SELECTED);
		iFirstSel++;
	}
	ListView_SetItemState(hWndMLV,iNextSel,LVIS_SELECTED,LVIS_SELECTED);
}

//---------------------------------------------------//
// GetLvItemRect
//---------------------------------------------------//
void GetLvItemRect(HWND hWndlv,int iSel,RECT* rectItem)
{   ListView_GetItemRect(hWndlv,iSel,rectItem,LVIR_SELECTBOUNDS);
    // BUG: Listview control in Windows Vista/7 common dialog
    int FisrtColWidth=ListView_GetColumnWidth(hWndlv,0);
    if(FisrtColWidth) // LVS_REPORT
        rectItem->right=FisrtColWidth;
    //
}

//---------------------------------------------------//
// GetNextItemPosCur
//---------------------------------------------------//
void GetNextItemPosCur(HWND hWndActive,HWND hWndlv,int iSel,RECT lvRect)
{	RECT itemRct;
    GetLvItemRect(hWndlv,iSel,&itemRct);
	ListTreeStatusPos(hWndActive,itemRct);
	switch(GetmHpVarStruct()->AscensSens)
	{	case 0:
		{	GetLvItemRect(hWndlv,iSel,&itemRct);
            GetmHpVarStruct()->YnextPos=(int)((itemRct.bottom-((itemRct.bottom-itemRct.top)/2))+lvRect.top);
			GetmHpVarStruct()->XnextPos=(int)((itemRct.right-((itemRct.right-itemRct.left)/2))+lvRect.left);
			break;
		}
		case 1: // Monte
		{	GetmHpVarStruct()->YnextPos=(int)(lvRect.top+6);
			GetmHpVarStruct()->XnextPos=(int)(lvRect.right-9);
			break;
		}
		case 2: // Descend
		{	if(IsWndStyle(hWndlv,WS_HSCROLL)==WS_HSCROLL) GetmHpVarStruct()->YnextPos=(int)(lvRect.bottom-27);
			else GetmHpVarStruct()->YnextPos=(int)(lvRect.bottom-12);
			GetmHpVarStruct()->XnextPos=(int)(lvRect.right-11);
			break;
		}
		case 3: // Droite
		{	GetmHpVarStruct()->YnextPos=(int)(lvRect.bottom-12);
			if(IsWndStyle(hWndlv,WS_VSCROLL)==WS_VSCROLL) GetmHpVarStruct()->XnextPos=(int)(lvRect.right-24);
			else GetmHpVarStruct()->XnextPos=(int)(lvRect.right-10);
			break;
		}
		case 4: // Gauche
		{	GetmHpVarStruct()->YnextPos=(int)(lvRect.bottom-12);
			GetmHpVarStruct()->XnextPos=(int)(lvRect.left+9);
			break;
		}
	}
}

//---------------------------------------------------//
// IsShiftOperation
//---------------------------------------------------//
BOOL IsShiftOperation()
{	int iLagTab[MAX_RS_SIZE]={0};
	int iLag=0,iLenSel=0,iLastSel=NO,iCurSel;
	iLagTab[0]=NO;
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	iCurSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	while(iCurSel!=NO)
	{	if(iLastSel!=NO)
		{	while(iCurSel!=(iLastSel+1))
			{	iLastSel++;
				iLagTab[iLag]=iLastSel;
				iLag++;
				iLagTab[iLag]=NO;
			}
		}
		iLastSel=iCurSel;
		iLenSel++;
		iCurSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	}
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	if(iLenSel>=(iLag+2)) return TRUE;
	return FALSE;
}

//---------------------------------------------------//
// IsAlreadySelected
//---------------------------------------------------//
BOOL IsAlreadySelected(int* iTabSel)
{	BOOL bSame=TRUE;
	int indice=0,iCurSel,iCntSame=0;
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	iCurSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	while((iCurSel!=NO)&&(bSame))
	{	if(iTabSel[indice]!=iCurSel) bSame=FALSE;
		else iCntSame++;
		indice++;
		iCurSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	}
	if((bSame)&&(iTabSel[indice]!=NO)) bSame=FALSE;
	if((!bSame)&&(iCntSame>=2)) iAltGrOp=1;
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	return bSame;
}

//---------------------------------------------------//
// IsMoreSelThanNoSel
//---------------------------------------------------//
BOOL IsMoreSelThanNoSel(int* iSelectedTab)
{	int iActCnt=0,iSel=0,iCurIndex,iCntToSel=0;
	int iActTab[MAX_RS_SIZE]={0};
	BOOL bExist;
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	iCurIndex=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	iActTab[0]=NO;
	while(iCurIndex!=NO)
	{	bExist=FALSE;
		while((iSelectedTab[iSel]!=NO)&&(iSelectedTab[iSel]<=iCurIndex))
		{	if(iSelectedTab[iSel]==iCurIndex) bExist=TRUE;
			else
			{	iActTab[iActCnt]=iSelectedTab[iSel];
				iActCnt++;
				iActTab[iActCnt]=NO;
			}
			iSel++;
		}
		if(!bExist)
		{	iActTab[iActCnt]=iCurIndex;
			iActCnt++;
			iActTab[iActCnt]=NO;
		}
		iCntToSel++;
		iCurIndex=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	}
	while(iSelectedTab[iSel]!=NO)
	{	iActTab[iActCnt]=iSelectedTab[iSel];
		iActCnt++;
		iActTab[iActCnt]=NO;
		iSel++;
	}
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	if(iCntToSel>=iActCnt)
	{	GetmHpVarStruct()->SelItemListe->EmptyGroupListe();
		iActCnt=0;
		while(iActTab[iActCnt]!=NO)
		{	GetmHpVarStruct()->SelItemListe->AddItemIndex(iActTab[iActCnt]);
			iActCnt++;
		}
		GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
		return TRUE;
	}
	return FALSE;
}

//---------------------------------------------------//
// ListView_GetSelItems
//---------------------------------------------------//
void ListView_GetSelItems(HWND hWndLV,int* iSelTab,int iMaxSel)
{	int iCnt=ListView_GetItemCount(hWndLV);
	mHpPak_EmptyTab(iSelTab,iMaxSel);
	for(int i=0;i<iCnt;i++)
	{	if(ListView_GetItemState(hWndLV,i,LVIS_SELECTED)==LVIS_SELECTED) mHpPak_AddIndex(iSelTab,i);
	}
}

//---------------------------------------------------//
// GetSLVItemToSel
//---------------------------------------------------//
int GetSLVItemToSel(HWND hWndLv)
{	char lpText[MAX_RS_SIZE]={0};
	int iCnt=ListView_GetItemCount(hWndLv);
	int iSel=0;
	bFindAll=TRUE;
	while(iCnt!=iSel)
	{	mHpPak_InitStr(lpText,MAX_RS_SIZE);
		ListView_GetItemText(hWndLv,iSel,0,lpText,MAX_RS_SIZE);
		if(!strncmp(lpText,GetmHpVarStruct()->CourantTask->GetMultiIndex(NO),MAX_RS_SIZE))
		{	if(ListView_GetItemState(hWndLv,iSel,LVIS_SELECTED)==LVIS_SELECTED) return NO;
			return iSel;
		}
		iSel++;
	}
	bFindAll=FALSE;
	return NO;
}

//-----------------------------------------------------//
// LVTimerProc
//-----------------------------------------------------//
void CALLBACK LVTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{	if(uMsg==WM_TIMER)
	{	switch(idEvent)
		{	case ID_TIMER_WAIT_LVMSG:
			{	KillTimer(hwnd,ID_TIMER_WAIT_LVMSG);
				if(LVtype==11) SListView_AddEvent(hwnd,LVname,LVhwnd,LVcur,LVcnt,FALSE);
				else MListView_AddEvent(hwnd,LVname,LVhwnd,LVcur,LVcnt,FALSE);
				break;
			}
			default:
			{	break;
			}
		}
	}
}

//*****************************************************************************************************************OBJECT//
// Class CListViewTask
CListViewTask::CListViewTask(int mHpId,char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,char* lpCtrlName)
 			  :CListTask(mHpId,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,lpCtrlName)
{}

CListViewTask::CListViewTask()
 			  :CListTask()
{}

//****************************************************************************************************************GENERAL//
// MListView_ItemIndexToSel
//---------------------------------------------------//
int MListView_ItemIndexToSel(HWND hWndActive)
{	LVFINDINFO LvFdInfo;
	LvFdInfo.flags=LVFI_STRING;
	LvFdInfo.psz=GetmHpVarStruct()->CourantTask->GetMultiIndex(NO);
	return ListView_FindItem(GetmHpVarStruct()->hWndCurCtrl,NO,&LvFdInfo);
}

//****************************************************************************************************************MHPFILE//
// ListView_ReadEvent
//---------------------------------------------------//
BOOL ListView_ReadEvent(HWND hWndActive,int pmHpID,const std::string& strPostData,UINT &iCustIdx)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpSel[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	char chNext=END_RS_TASK;
	int iCur,iCnt,iTmp,iOpt,iCntSel;
	BOOL bNoSel=FALSE,bContinue=TRUE,bError;
	CListViewTask *LViewTask;
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
	if(*lpCtrlName!=0) LViewTask=new CListViewTask(pmHpID,lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName);
    else LViewTask=new CListViewTask(pmHpID,lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL);
#ifdef MHELP_OLD
	// ** Param Type: String ** //
	while((bContinue)&&(!bNoSel))
	{	GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpSel,MAX_RS_SIZE,bContinue,bError);
		if(!bError)
		{	if(lpSel[0]==RS_PARAM)
			{	iTmp=mHpPak_GetIntFromStr(lpSel,MAX_RS_SIZE,1);
				if(!DidItSelected(iTmp)) bNoSel=TRUE;
				else
				{	iCntSel=0;
					while(iTmp!=NO)
					{	GetStrEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,iCntSel,lpSel,MAX_RS_SIZE);
						if(iTmp!=NO) LViewTask->AddMultiIndex(lpSel);
						iCntSel++;
					}
					if(iCntSel==1) return TRUE; // WARNING: There is something wrong here!!!!
				}
				bContinue=FALSE;
			}
			else LViewTask->AddMultiIndex(lpSel);
		}
		else return TRUE; // Error
	}
#else
    ////// TODO: Be able to remove lines below (state always in HTML)
	while(bContinue)
	{	GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpSel,MAX_RS_SIZE,bContinue,bError);
		if(bError)
		    return TRUE; // Error
        //LViewTask->AddMultiIndex(lpSel);
	}
    //////

    // Check if selected
    if(!IsHtmlCustSelected(strPostData,iCustIdx))
        bNoSel=TRUE;
    else
    {   // Get HTML user setting
        char lpStrState[MAX_LONG_SIZE]={0};
        if(!GetHtmlCustValue(lpStrState,
                             MAX_LONG_SIZE,
                             strPostData,
                             LISTSEL_KEY,
                             iCustIdx))
        {   return TRUE; // Error: No state parameter
        }
        // Assign state
        if(*lpStrState)
        {   int iStrIdx=0;
            while(iStrIdx!=NO)
            {   char lpState[MAX_RS_SIZE]={0};
                mHpPak_DecodeString(lpState,MAX_RS_SIZE,lpStrState,iStrIdx);
                LViewTask->AddMultiIndex(lpState);
            }
        }
        else
            return TRUE; // WARNING: There is something wrong here!!!!
    }
    // Increase number of custom task
    iCustIdx++;
#endif
	if(!bNoSel) GetmHpVarStruct()->TaskList->AddCellule(LViewTask);
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_WriteEvent
//---------------------------------------------------//
void SListView_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
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

//---------------------------------------------------//
// MListView_WriteEvent
//---------------------------------------------------//
void MListView_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
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
	else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrState(Index,TRUE),FALSE))
	{	bRes=FALSE;}
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_WriteParam
//---------------------------------------------------//
BOOL SListView_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}			

//---------------------------------------------------//
// MListView_WriteParam
//---------------------------------------------------//
BOOL MListView_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),FALSE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}			

//*******************************************************************************************************************KEYS//
// ListView_ReadEventKeys
//---------------------------------------------------//
void ListView_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpParam* ReadParam;
	ReadTask=new CListViewTask();
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
// SListView_AddEvent
//---------------------------------------------------//
void SListView_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndListView,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
    if(*lpCtrlName!=0)
	    Item=new CListViewTask(11,NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName);
    else
    {   int lpctrl[MAX_PART_TREE]={0};
	    AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndListView));
	    Item=new CListViewTask(11,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL);
    }
	int iSel=ListView_GetNextItem(hWndListView,NO,LVNI_SELECTED);
	if(iSel!=NO)
	{	char lptcitm[MAX_RS_SIZE]={0};
	    LVITEM pItem;
	    pItem.mask=LVIF_TEXT; 
	    pItem.iSubItem=0; 
	    pItem.cchTextMax=MAX_RS_SIZE; 
	    pItem.pszText=lptcitm;
        pItem.iItem=iSel; 
		char lpSel[MAX_RS_SIZE]={0};
	    if(ListView_GetItem(hWndListView,&pItem)) strncpy(lpSel,pItem.pszText,MAX_RS_SIZE);
		else mHpPak_InitStr(lpSel,MAX_RS_SIZE);	
		Item->AddMultiIndex(lpSel);
	}
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

//---------------------------------------------------//
// MListView_AddEvent
//---------------------------------------------------//
void MListView_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndListView,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
    if(*lpCtrlName!=0)
        Item=new CListViewTask(12,NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName);
    else
    {   int lpctrl[MAX_PART_TREE]={0};
        AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndListView));
        Item=new CListViewTask(12,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL);
    }
	int iSelCount=ListView_GetSelectedCount(hWndListView);
	if(iSelCount>=MAX_RS_SIZE)
	{	mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_CREATE_MULTILISTE,NULL,0,0,MB_ICONEXCLAMATION);
		return;
	}
    int iSel[MAX_RS_SIZE]={0};
	ListView_GetSelItems(hWndListView,iSel,MAX_RS_SIZE);
	for(int i=0;i<iSelCount;i++)
	{   char lpTemp[MAX_RS_SIZE]={0};
	    ListView_GetItemText(hWndListView,iSel[i],0,lpTemp,MAX_RS_SIZE);
		Item->AddMultiIndex(lpTemp);
	}
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
// SListView_IsClassName
//---------------------------------------------------//
BOOL SListView_IsClassName(HWND hWndCtrl,char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_LISTVIEW,ClassSize))||
       (!strncmp(CtrlClass,VB_TASK_LISTVIEW,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_LISTVIEW,sizeof(DOTNET_TASK_LISTVIEW)-1)))
	{	if(IsWndStyle(hWndCtrl,LVS_SINGLESEL)==LVS_SINGLESEL) return TRUE;
	}
	return FALSE;
}

//---------------------------------------------------//
// MListView_IsClassName
//---------------------------------------------------//
BOOL MListView_IsClassName(HWND hWndCtrl,char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_LISTVIEW,ClassSize))||
       (!strncmp(CtrlClass,VB_TASK_LISTVIEW,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_LISTVIEW,sizeof(DOTNET_TASK_LISTVIEW)-1)))
	{	if(IsWndStyle(hWndCtrl,LVS_SINGLESEL)!=LVS_SINGLESEL) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_IsCustom
//---------------------------------------------------//
void SListView_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//---------------------------------------------------//
// MListView_IsCustom
//---------------------------------------------------//
void MListView_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_StrState
//---------------------------------------------------//
void SListView_StrState(CEventCell* pCurCell,char* lpState,int StateSize)
{	strncpy(lpState,pCurCell->mHpTask->GetMultiIndex(NO),StateSize);}

//---------------------------------------------------//
// MListView_StrState
//---------------------------------------------------//
void MListView_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
{	char lpTemp[MAX_RS_SIZE]={0};
	int indice=0;
	while(pCurCell->mHpTask->GetNextIndex())
	{	if(indice!=0)
		{	strncat(lpState,"^",1);
			if(pWriteOp) mHpPak_WriteString(lpTemp,MAX_RS_SIZE,pCurCell->mHpTask->GetMultiIndex(NO));
			else
			{	mHpPak_InitStr(lpTemp,MAX_RS_SIZE);
				strncpy(lpTemp,pCurCell->mHpTask->GetMultiIndex(NO),MAX_RS_SIZE);
			}
			strncat(lpState,lpTemp,mHpPak_FindStrLenght(lpTemp,MAX_RS_SIZE));
		}
		else
		{	if(pWriteOp) mHpPak_WriteString(lpTemp,MAX_RS_SIZE,pCurCell->mHpTask->GetMultiIndex(NO));
			else strncpy(lpTemp,pCurCell->mHpTask->GetMultiIndex(NO),MAX_RS_SIZE);
			strncpy(lpState,lpTemp,StateSize);
		}
		indice++;
	}
	if(indice==0) LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_NO_SELECTION,lpState,StateSize);
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_ParamType
//---------------------------------------------------//
int SListView_ParamType()
{	return 4; // S
}

//---------------------------------------------------//
// MListView_ParamType
//---------------------------------------------------//
int MListView_ParamType()
{	return 9; // LS
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_DefaultBound
//---------------------------------------------------//
void SListView_DefaultBound(char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	strncpy(pStrBound,STRING_LIMITE,BoundSize);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
	strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
}

//-----------------------------------------------------//
// MListView_DefaultBound
//-----------------------------------------------------//
void MListView_DefaultBound(char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	strncpy(pStrBound,LISTSTR_LIMITE,BoundSize);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
	strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_IsCustomized
//-----------------------------------------------------//
BOOL SListView_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------//
// MListView_IsCustomized
//-----------------------------------------------------//
BOOL MListView_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_IsNoStateMsg
//-----------------------------------------------------//
BOOL SListView_IsNoStateMsg()
{	return TRUE;}

//-----------------------------------------------------//
// MListView_IsNoStateMsg
//-----------------------------------------------------//
BOOL MListView_IsNoStateMsg()
{	return TRUE;}

//-----------------------------------------------------//
// SListView_GetHtmlTag
//-----------------------------------------------------//
#define LISTVIEW_HTML_TAG   "\t<input type=\"text\" name=\"%d-%s\" maxlength=\"100\" value=\"%s\"/>\r\n"

void SListView_GetHtmlTag(CEventCell* pCurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{
    _snprintf(lpHtmlTag,
              iMaxSize,
              LISTVIEW_HTML_TAG,
              iCustIndex,
              LISTSEL_KEY,
              pCurCell->mHpTask->GetMultiIndex(NO));
}

//-----------------------------------------------------//
// MListView_GetHtmlTag
//-----------------------------------------------------//
void MListView_GetHtmlTag(CEventCell* pCurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{
    char lpState[MAX_LONG_SIZE]={0};
    int indice=0;
	while(pCurCell->mHpTask->GetNextIndex())
	{	if(indice!=0)
		{	strncat(lpState,"^",1);
			strncat(lpState,pCurCell->mHpTask->GetMultiIndex(NO),mHpPak_FindStrLenght(pCurCell->mHpTask->GetMultiIndex(NO),MAX_RS_SIZE));
		}
		else
		    strncpy(lpState,pCurCell->mHpTask->GetMultiIndex(NO),MAX_LONG_SIZE);
		indice++;
	}
    _snprintf(lpHtmlTag,
              iMaxSize,
              LISTVIEW_HTML_TAG,
              iCustIndex,
              LISTSEL_KEY,
              lpState);
}

//***************************************************************************************************************CFGTOOLS//
// SListView_ShowCfgTools
//---------------------------------------------------//
void SListView_ShowCfgTools(HWND hWndActive)
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
// MListView_ShowCfgTools
//---------------------------------------------------//
void MListView_ShowCfgTools(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndState=GetDlgItem(hWndActive,IDC_STATIC_STATE);
	HWND hWndMultiItm=GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST);
	HWND hWndEditItm=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	HWND hWndAddItm=GetDlgItem(hWndActive,IDC_ADD_MULTILIST);
	HWND hWndRemoveItm=GetDlgItem(hWndActive,IDC_REMOVE_MULTILIST);
	MoveWindow(hWndState,18,42,256,16,TRUE);
	MoveWindow(hWndEditItm,18,59,276,20,TRUE);
	MoveWindow(hWndMultiItm,18,79,276,70,TRUE);	
	MoveWindow(hWndAddItm,18,154,87,23,TRUE);
	MoveWindow(hWndRemoveItm,111,154,87,23,TRUE);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_STRSELS,lpTmp,MAX_RS_SIZE);
	SetWindowText(hWndState,lpTmp);
	if(SendMessage(hWndMultiItm,LB_GETCURSEL,0,0)!=LB_ERR) EnableWindow(hWndRemoveItm,TRUE);
	else EnableWindow(hWndRemoveItm,FALSE);
	ShowWindow(hWndState,SW_SHOW);
	ShowWindow(hWndMultiItm,SW_SHOW);
	ShowWindow(hWndEditItm,SW_SHOW);
	ShowWindow(hWndAddItm,SW_SHOW);
	ShowWindow(hWndRemoveItm,SW_SHOW);
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_HideCfgTools
//---------------------------------------------------//
void SListView_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
}				

//---------------------------------------------------//
// MListView_HideCfgTools
//---------------------------------------------------//
void MListView_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_REMOVE_MULTILIST),SW_HIDE);
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_SaveCfg
//---------------------------------------------------//
void SListView_SaveCfg(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	GetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lpTmp,MAX_RS_SIZE);
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->InitMultiIndex();
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->AddMultiIndex(lpTmp);
}

//---------------------------------------------------//
// MListView_SaveCfg
//---------------------------------------------------//
void MListView_SaveCfg(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndMultiItm=GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST);
	int iCount=(int)SendMessage(hWndMultiItm,LB_GETCOUNT,0,0);
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->InitMultiIndex();
	for(int i=0;i<iCount;i++)
	{	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
		SendMessage(hWndMultiItm,LB_GETTEXT,i,(LPARAM)lpTmp);
		GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->AddMultiIndex(lpTmp);
	}
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_LoadCfg
//---------------------------------------------------//
void SListView_LoadCfg(HWND hWndActive)
{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMultiIndex(NO));
}

//---------------------------------------------------//
// MListView_LoadCfg
//---------------------------------------------------//
void MListView_LoadCfg(HWND hWndActive)
{	HWND hWndMultiItm=GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST);
	EnableWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),FALSE);
	while(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetNextIndex())
	{	SendMessage(hWndMultiItm,LB_ADDSTRING,0,(LPARAM)GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMultiIndex(NO));
	}
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_GetInfosCfg
//---------------------------------------------------//
void SListView_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_LISTVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_LISTVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_LISTVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_LISTVIEW,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_LISTVIEW)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// MListView_GetInfosCfg
//---------------------------------------------------//
void MListView_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_LISTVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_LISTVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_LISTVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_LISTVIEW,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_LISTVIEW)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_ProcCfg
//---------------------------------------------------//
BOOL SListView_ProcCfg(UINT Message,WPARAM wParam,HWND hWndChng)
{	if(Message==WM_COMMAND)
	{	if(LOWORD(wParam)==IDC_EDIT_STATE)
		{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
			return TRUE;
		}
	}
	return FALSE;
}

//---------------------------------------------------//
// MListView_ProcCfg
//---------------------------------------------------//
BOOL MListView_ProcCfg(HWND hWndActive,UINT Message,WPARAM wParam,HWND hWndChng)
{	if(Message==WM_COMMAND)
	{	switch(LOWORD(wParam))
		{	case IDC_LISTBOX_MULTILIST:
			{	switch(HIWORD(wParam))
				{	case LBN_SELCHANGE:
					{	if(SendMessage(GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST),LB_GETCURSEL,0,0)!=LB_ERR)
						{	EnableWindow(GetDlgItem(hWndActive,IDC_REMOVE_MULTILIST),TRUE);}
						else EnableWindow(GetDlgItem(hWndActive,IDC_REMOVE_MULTILIST),FALSE);
						return TRUE;
					}
				}
				break;
			}
			case IDC_EDIT_STATE:
			{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),TRUE);
				return TRUE;
			}
			case IDC_ADD_MULTILIST:
			{	char lpTmp[MAX_RS_SIZE]={0};
				HWND hWndMultiItm=GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST);
				HWND hWndEditItm=GetDlgItem(hWndActive,IDC_EDIT_STATE);
				int iSel;
				GetWindowText(hWndEditItm,lpTmp,MAX_RS_SIZE);
				iSel=(int)SendMessage(hWndMultiItm,LB_FINDSTRING,NO,(LPARAM)lpTmp);
				if(iSel==LB_ERR)
				{	SendMessage(hWndMultiItm,LB_ADDSTRING,0,(LPARAM)lpTmp);
					EnableWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),FALSE);
					SetFocus(GetDlgItem(hWndActive,IDCANCEL));
					EnableWindow(hWndChng,TRUE);
				}
				else
				{	SendMessage(hWndMultiItm,LB_SETCURSEL,iSel,0);
					EnableWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),FALSE);
					SetFocus(GetDlgItem(hWndActive,IDCANCEL));
					EnableWindow(GetDlgItem(hWndActive,IDC_REMOVE_MULTILIST),TRUE);
				}
				return TRUE;
			}
			case IDC_REMOVE_MULTILIST:
			{	HWND hWndMultiItm=GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST);
				int iSel=(int)SendMessage(hWndMultiItm,LB_GETCURSEL,0,0);
				SendMessage(hWndMultiItm,LB_DELETESTRING,iSel,0);
				EnableWindow(GetDlgItem(hWndActive,IDC_REMOVE_MULTILIST),FALSE);
				SetFocus(GetDlgItem(hWndActive,IDCANCEL));
				EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			default:
			{	break;}
		}
	}
	return FALSE;
}

//****************************************************************************************************************PROCESS//
// ListView_CaptureMsg
//---------------------------------------------------//
void ListView_CaptureMsg(HWND hWndActive,char* lpCtrlName,WPARAM wpParam,LPARAM lpParam,HWND hToolWnd,int iCurOp,
                         CEventListe* EventObj,char* lpNameApp,UINT &pIdLast,int pCurwnd,int pCntwnd,int imHpCtrlID,
                         BOOL &pActionUser,int &pResCapt)
{	int iWriteRes=0;
	if(((LPNMLISTVIEW)lpParam)->hdr.code==LVN_ITEMCHANGED)
	{	if((((LPNMLISTVIEW)lpParam)->uChanged&LVIF_STATE)==LVIF_STATE)
		{	WriteImmediatTask(hWndActive,lpCtrlName,((LPNMHDR)lpParam)->hwndFrom,hToolWnd,NULL,iCurOp,imHpCtrlID,
                              (UINT)((int)wpParam),EventObj,NULL,NULL,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
			switch(iWriteRes)
			{	case 0:
				{	//** Wait the next listview message if exist **//
					LVcur=pCurwnd;
					LVcnt=pCntwnd;
					LVtype=imHpCtrlID;
                    strncpy(LVname,lpCtrlName,MAX_CTRL_NAME);
                    LVhwnd=((LPNMHDR)lpParam)->hwndFrom;
                    SetTimer(hWndActive,ID_TIMER_WAIT_LVMSG,100,(TIMERPROC)LVTimerProc);
					//*********************************************//
					pActionUser=FALSE;
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
}

//-----------------------------------------------------------------------------------------------------------------------//
// SListView_mHpProcess
//---------------------------------------------------//
void SListView_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2: // CLICK
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	BOOL bSnd=PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			if((!GetmHpVarStruct()->AscensSens)||(GetmHpVarStruct()->imHpMode==1))
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(6);
			}
			if(!bSnd) SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			else SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4: // SCROLL
		{	RECT itmRct;
			int iToSel=GetSLVItemToSel(GetmHpVarStruct()->hWndCurCtrl);
			switch(GetmHpVarStruct()->AscensSens)
			{	case 1: // Monte
				{	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    else
                    {   UpdateBackBuffer(TRUE);
					    if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_ICON)==LVS_ICON) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,(ECART_SCROLL_NORMAL*NO));
					    else if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_REPORT)==LVS_REPORT)	ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,NO);
					    else ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,(ECART_SCROLL_SMALL*NO));
                        if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    }
					break;
				}
				case 2: // Descend
				{	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    else
                    {   UpdateBackBuffer(TRUE);
					    if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_ICON)==LVS_ICON) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,ECART_SCROLL_NORMAL);
					    else if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_REPORT)==LVS_REPORT)	ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,1);
					    else ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,ECART_SCROLL_SMALL);
                        if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    }
					break;
				}
				case 3: // Droite
				{	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    else
                    {   UpdateBackBuffer(TRUE);
					    if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_LIST)==LVS_LIST) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,1,0);
					    else ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,ECART_SCROLL_NORMAL,0);
                        if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    }
					break;
				}
				case 4: // Gauche
				{	EndObjects(hWndActive);
					if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
					else
					{	if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_LIST)==LVS_LIST) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,NO,0);
						else ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,(ECART_SCROLL_NORMAL*NO),0);
						GetmHpVarStruct()->bRefreshDDraw=TRUE;
						GetmHpVarStruct()->CursorType=1;
						SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
						return;
					}
					break;
				}
			}
			GetLvItemRect(GetmHpVarStruct()->hWndCurCtrl,iToSel,&itmRct);
            ListTreeStatusPos(hWndActive,itmRct);
			GetmHpVarStruct()->CourantTask->SetmHpStep(5);
			if(GetmHpVarStruct()->AscensSens!=0) GetmHpVarStruct()->CourantTask->SetmHpStep(3);
			SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 5: // SCROLL GAUCHE
		{	RECT itemRct;
			GetLvItemRect(GetmHpVarStruct()->hWndCurCtrl,GetSLVItemToSel(GetmHpVarStruct()->hWndCurCtrl),&itemRct);
            ListTreeStatusPos(hWndActive,itemRct);
			if(GetmHpVarStruct()->AscensSens!=0) GetmHpVarStruct()->CourantTask->SetmHpStep(3);
			SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6: // MOVE ON ITEM
		{	RECT itmRect,ctrlRect;
			int iSel=GetSLVItemToSel(GetmHpVarStruct()->hWndCurCtrl);
			PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			GetmHpVarStruct()->CourantTask->SetmHpStep(1);
			GetLvItemRect(GetmHpVarStruct()->hWndCurCtrl,iSel,&itmRect);
            GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&ctrlRect);
			GetmHpVarStruct()->YnextPos=(int)((itmRect.bottom-((itmRect.bottom-itmRect.top)/2))+ctrlRect.top);
			GetmHpVarStruct()->XnextPos=(int)((itmRect.right-((itmRect.right-itmRect.left)/2))+ctrlRect.left);
			EndObjects(hWndActive);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->bNewDepl=TRUE;
			SetDDrawObj(hWndActive,0);
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,3,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7: // MSG
		{	EndObjects(hWndActive);
			if(!DoInit(hWndActive))
                StopmHelp(hWndActive,TRUE,TRUE);
            else if((GetmHpVarStruct()->imHpMode==2)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()==NO))
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
					else
                    {   StopmHelp(hWndActive,TRUE,TRUE);
                        return;
                    }
				}
				int iItmSel=GetSLVItemToSel(GetmHpVarStruct()->hWndCurCtrl);
                if(GetFocus()!=GetmHpVarStruct()->hWndCurCtrl) SetFocus(GetmHpVarStruct()->hWndCurCtrl);
				if(bChangeWnd)
				{	mHpPak_ToolTipsActivate(TRUE);
                    if(iItmSel==NO)
					{	ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,iItmSel,0,LVIS_SELECTED);}
					else ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,iItmSel,LVIS_SELECTED,LVIS_SELECTED);
				}
				else
				{	if(iItmSel==NO)
					{	ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,iItmSel,0,LVIS_SELECTED);}
					else ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,iItmSel,LVIS_SELECTED,LVIS_SELECTED);
					SetTimer(hWndActive,ID_TIMER_DELAI,10,(TIMERPROC)mHpTimerProc);
				}
			}
			break;
		}
		case 8:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			GetmHpVarStruct()->CourantTask->SetmHpStep(9);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetDDrawObj(hWndActive,0);
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 9:
		{	if((!bFindAll)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10)) ViewCtrlNotReadyMsg(hWndActive,4);
			else SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 10:
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
// SListView_StartProc
//---------------------------------------------------//
BOOL SListView_StartProc(HWND hWndActive)
{	GetmHpVarStruct()->hWndCurCtrl=NULL;
	/*if(*GetmHpVarStruct()->CourantTask->GetmHpCtrlName()!=0)
	{	if(mHpPak_IsDotNetHwnd(hWndActive))
		{   int iState=0,iStyle=0;
			GetmHpVarStruct()->hWndCurCtrl=mHpPak_GetDotNetCtrlHandle(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlName(),iStyle,iState);
		}
	}*/
    if(!GetmHpVarStruct()->hWndCurCtrl)
        GetmHpVarStruct()->hWndCurCtrl=GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
	if(!IsCtrlReady(hWndActive,GetmHpVarStruct()->hWndCurCtrl)) return FALSE;
	RECT CtrlRct;
    GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&CtrlRct);
	if((!strncmp(GetmHpVarStruct()->CourantTask->GetMultiIndex(NO),NULL_STRING,MAX_RS_SIZE))&&(ListView_GetSelectedCount(GetmHpVarStruct()->hWndCurCtrl)!=0))
	{	// No selection
		GetmHpVarStruct()->YnextPos=(int)(CtrlRct.top+15);
		if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,WS_VSCROLL)!=WS_VSCROLL) GetmHpVarStruct()->XnextPos=(int)(CtrlRct.right-25);
		else GetmHpVarStruct()->XnextPos=(int)(CtrlRct.right-41);
	}
	else
	{	int iToSel=GetSLVItemToSel(GetmHpVarStruct()->hWndCurCtrl);
		if(iToSel!=NO) GetNextItemPosCur(hWndActive,GetmHpVarStruct()->hWndCurCtrl,iToSel,CtrlRct);
		else
		{	GetmHpVarStruct()->CourantTask->SetmHpStep(8);
			GetmHpVarStruct()->YnextPos=(int)(CtrlRct.top+((CtrlRct.bottom-CtrlRct.top)/2));
			GetmHpVarStruct()->XnextPos=(int)(CtrlRct.left+((CtrlRct.right-CtrlRct.left)/2));
		}
	}
	return TRUE;
}

//-----------------------------------------------------------------------------------------------------------------------//
// MListView_mHpProcess
//---------------------------------------------------//
void MListView_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2: // CLICK
		{	EndObjects(hWndActive);
            SetDDrawObj(hWndActive,0);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	BOOL bSound=PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			if((!GetmHpVarStruct()->AscensSens)||(GetmHpVarStruct()->imHpMode==1))
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(7); // NextSel
			}
			if(!bSound) SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			else SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4: // SCROLL
		{	RECT itmRct;
			switch(GetmHpVarStruct()->AscensSens)
			{	case 1: // Monte
				{	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    else
                    {   UpdateBackBuffer(TRUE);
                        if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_ICON)==LVS_ICON) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,(ECART_SCROLL_NORMAL*NO));
					    else if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_REPORT)==LVS_REPORT) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,NO);
					    else ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,(ECART_SCROLL_SMALL*NO));
                        if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    }
					break;
				}
				case 2: // Descend
				{	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    else
                    {   UpdateBackBuffer(TRUE);
                        if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_ICON)==LVS_ICON) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,ECART_SCROLL_NORMAL);
					    else if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_REPORT)==LVS_REPORT) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,1);
					    else ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,0,ECART_SCROLL_SMALL);
                        if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    }
					break;
				}
				case 3: // Droite
				{	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    else
                    {   UpdateBackBuffer(TRUE);
                        if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_LIST)==LVS_LIST) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,1,0);
					    else ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,ECART_SCROLL_NORMAL,0);
                        if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                    }
					break;
				}
				case 4: // Gauche
				{	EndObjects(hWndActive);
					if(!DoInit(hWndActive))
                    {   StopmHelp(hWndActive,TRUE,TRUE);
                        return;
                    }
					else
					{	if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,LVS_LIST)==LVS_LIST) ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,NO,0);
						else ListView_Scroll(GetmHpVarStruct()->hWndCurCtrl,(ECART_SCROLL_NORMAL*NO),0);
                        GetmHpVarStruct()->bRefreshDDraw=TRUE;
						GetmHpVarStruct()->CursorType=1;
						SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
						return;
					}
					break;
				}
			}
			GetLvItemRect(GetmHpVarStruct()->hWndCurCtrl,iNextSel,&itmRct);
            ListTreeStatusPos(hWndActive,itmRct);
			GetmHpVarStruct()->CourantTask->SetmHpStep(6); // Move on item to select
			if(iAltGrOp==2) GetmHpVarStruct()->CourantTask->SetmHpStep(5); // Refresh image
			if(GetmHpVarStruct()->AscensSens!=0) GetmHpVarStruct()->CourantTask->SetmHpStep(3); // Boucle
			SetTimer(hWndActive,ID_TIMER_DELAI,100,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 5: // SCROLL GAUCHE
		{	RECT itemRct;
			GetLvItemRect(GetmHpVarStruct()->hWndCurCtrl,iNextSel,&itemRct);
            ListTreeStatusPos(hWndActive,itemRct);
			if(GetmHpVarStruct()->AscensSens!=0) GetmHpVarStruct()->CourantTask->SetmHpStep(3); // Boucle
			else if(iAltGrOp!=2) GetmHpVarStruct()->CourantTask->SetmHpStep(6); // Move on item to select
			SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6: // REFRESH ALTGR IMAGE
		{	EndObjects(hWndActive);
            GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->iCurAnim=2;
			GetmHpVarStruct()->iCntAnim=3;
			GetmHpVarStruct()->iMaxAnim=3;
			SetDDrawObj(hWndActive,5);
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7: // MOVE ON ITEM
		{	RECT itmRect,ctrlRect;
			PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			GetmHpVarStruct()->CourantTask->SetmHpStep(1); // Click
			if((iShiftOp==1)||(iAltGrOp==1))
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(10); // SHITH - ALTGR
			}
			GetLvItemRect(GetmHpVarStruct()->hWndCurCtrl,iNextSel,&itmRect);
            GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&ctrlRect);
			GetmHpVarStruct()->YnextPos=(int)((itmRect.bottom-((itmRect.bottom-itmRect.top)/2))+ctrlRect.top);
			GetmHpVarStruct()->XnextPos=(int)((itmRect.right-((itmRect.right-itmRect.left)/2))+ctrlRect.left);
			if(iAltGrOp!=2) EndObjects(hWndActive);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->bNewDepl=TRUE;
			SetDDrawObj(hWndActive,0);
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,3,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 8: // MSG
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			EndObjects(hWndActive);
			if(!DoInit(hWndActive))
                StopmHelp(hWndActive,TRUE,TRUE);
			else if((GetmHpVarStruct()->imHpMode==2)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()==NO))
			{	SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
            }
            else
			{	GetmHpVarStruct()->CourantIDCommand=GetmHpVarStruct()->CourantTask->GetCtrlID();
				if((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=NO)&&
				   ((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=iCurrentWnd)||(GetmHpVarStruct()->CourantTask->GetCountWnd()!=iCountWnd)))
				{	/* WARNING: Dans le cas d'un changement de fenêtre Modal!! */
					//bChangeWnd=TRUE;
				}
				if(GetmHpVarStruct()->CourantTask->GetCurrentWnd()==NO)
				{	GetmHpVarStruct()->bNoInactiveMsg=TRUE;
					if(GetmHpVarStruct()->TaskList->GetNextmHpID()!=NO)
					{	GetmHpVarStruct()->TaskList->NextCellule();
						if(GetmHpVarStruct()->imHpMode!=1)
						{	WriteCursorPoskey(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur);}
					}
					else
                    {   StopmHelp(hWndActive,TRUE,TRUE);
                        return;
                    }
				}
                if(GetFocus()!=GetmHpVarStruct()->hWndCurCtrl) SetFocus(GetmHpVarStruct()->hWndCurCtrl);
				if((iNextSel==NO)||(bUnSelAll))
				{	// Unselect All
					ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,NO,0,LVIS_SELECTED);
					if(bUnSelAll)
					{	bUnSelAll=FALSE;
						if(IsShiftOperation()) iShiftOp=0;
						iNextSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
					}
				}
				else
				{	if(GetmHpVarStruct()->imHpMode==1)
					{	ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,NO,0,LVIS_SELECTED);
						while(iNextSel!=NO)
						{	ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,iNextSel,LVIS_SELECTED,LVIS_SELECTED);
							iNextSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
						}
					}
					else switch(iShiftOp)
					{	case 0:
						{	ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,NO,0,LVIS_SELECTED);
							ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,iNextSel,LVIS_SELECTED,LVIS_SELECTED);
							iNextSel=GetNextShiftSel(GetmHpVarStruct()->hWndCurCtrl);
							iShiftOp++;
							break;
						}
						case 1:
						{	int iItmTab[MAX_RS_SIZE]={0};
							DoShiftSelection(GetmHpVarStruct()->hWndCurCtrl);
							ListView_GetSelItems(GetmHpVarStruct()->hWndCurCtrl,iItmTab,MAX_RS_SIZE);
							IsMoreSelThanNoSel(iItmTab); // Return FALSE
							iNextSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
							iShiftOp++;
							iAltGrOp++;
							break;
						}
						case 2:
						{	if(ListView_GetItemState(GetmHpVarStruct()->hWndCurCtrl,iNextSel,LVIS_SELECTED)!=LVIS_SELECTED)
							{	// Select
								ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,iNextSel,LVIS_SELECTED,LVIS_SELECTED);
							}
							else
							{	// UnSelect
								ListView_SetItemState(GetmHpVarStruct()->hWndCurCtrl,iNextSel,0,LVIS_SELECTED);
							}
							iNextSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
							if(iAltGrOp<2) iAltGrOp++;
							break;
						}
					}
				}
				/* WARNING: Dans le cas d'un changement de fenêtre Modal!! */
				//if(!bChangeWnd)
				//{	SetTimer(hWndActive,ID_TIMER_DELAI,10,(TIMERPROC)mHpTimerProc);} // Relache Click
				SetTimer(hWndActive,ID_TIMER_DELAI,10,(TIMERPROC)mHpTimerProc); // Relache Click
			}
			break;
		}
		case 9:
		{	if((iNextSel!=NO)&&(iAltGrOp==2)) // REFRESH ALTGR IMAGE
			{	// Nothing to do with Direct2D!
                /*EndObjects(hWndActive);
				GetmHpVarStruct()->bRefreshDDraw=TRUE;
				GetmHpVarStruct()->iCurAnim=2;
				GetmHpVarStruct()->iCntAnim=3;
				GetmHpVarStruct()->iMaxAnim=3;
				SetDDrawObj(hWndActive,5);
				if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
				else SetTimer(hWndActive,ID_TIMER_DDRAW,3,(TIMERPROC)mHpTimerProc);*/
                SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
			}
			else
			{	if(iNextSel==NO) GetmHpVarStruct()->CourantTask->SetmHpStep(12); // FIN
				GetmHpVarStruct()->bRefreshDDraw=TRUE;
				SetDDrawObj(hWndActive,0);
		    	if(!RestoreKeyBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 10:
		{	if(iNextSel!=NO)
			{	// NewDepl, StartProc
				RECT ctrlRct;
				GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&ctrlRct);
				GetmHpVarStruct()->bNewDepl=TRUE;
				SetDDrawObj(hWndActive,0);
				GetNextItemPosCur(hWndActive,GetmHpVarStruct()->hWndCurCtrl,iNextSel,ctrlRct);
				GetmHpVarStruct()->CourantTask->SetmHpStep(1);
				if(!GetmHpVarStruct()->AscensSens)
				{	if((iShiftOp==1)||(iAltGrOp==1))
					{	GetmHpVarStruct()->CourantTask->SetmHpStep(10); // SHIFT - ALTGR
					}
				}
                else if(!RestoreKeyBackBuffer(hWndActive))
                {   StopmHelp(hWndActive,TRUE,TRUE);
                    break;
                }
				if((iAltGrOp==2)&&(!DoInit(hWndActive))) StopmHelp(hWndActive,TRUE,TRUE);
				else SetTimer(hWndActive,ID_TIMER_DDRAW,100,(TIMERPROC)mHpTimerProc);
			}
			else 
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(12); // FIN
				SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 11: // SHIFT - ALTGR
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->iCurAnim=0;
			GetmHpVarStruct()->iCntAnim=0;
			GetmHpVarStruct()->iMaxAnim=3;
			if(iShiftOp==1)
            {   SetDDrawObj(hWndActive,6); // SHIFT
            }
			else
			{	iAltGrOp=2;
				SetDDrawObj(hWndActive,5); // ALTGR
			}
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else
            {   GetmHpVarStruct()->bConserveBGD=TRUE;
                SetTimer(hWndActive,ID_TIMER_DDRAW,200,(TIMERPROC)mHpTimerProc);
            }
			break;
		}
		case 12:
		{	if((!bFindAll)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10)) ViewCtrlNotReadyMsg(hWndActive,4);
			else
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(1);
				SetTimer(hWndActive,ID_TIMER_DELAI,800,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 13:
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
// MListView_StartProc
//---------------------------------------------------//
BOOL MListView_StartProc(HWND hWndActive)
{	GetmHpVarStruct()->hWndCurCtrl=NULL;
	/*if(*GetmHpVarStruct()->CourantTask->GetmHpCtrlName()!=0)
	{	if(mHpPak_IsDotNetHwnd(hWndActive))
		{   int iState=0,iStyle=0;
			GetmHpVarStruct()->hWndCurCtrl=mHpPak_GetDotNetCtrlHandle(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlName(),iStyle,iState);
		}
	}*/
    if(!GetmHpVarStruct()->hWndCurCtrl)
        GetmHpVarStruct()->hWndCurCtrl=GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
	if(!IsCtrlReady(hWndActive,GetmHpVarStruct()->hWndCurCtrl)) return FALSE;
	RECT CtrlRct;
    GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&CtrlRct);
	CreateItemListe(hWndActive,bFindAll);
	if((!bFindAll)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10))
	{	GetmHpVarStruct()->CourantTask->SetmHpStep(11);
		GetmHpVarStruct()->YnextPos=(int)(CtrlRct.top+((CtrlRct.bottom-CtrlRct.top)/2));
		GetmHpVarStruct()->XnextPos=(int)(CtrlRct.left+((CtrlRct.right-CtrlRct.left)/2));
	}
	else
	{	bUnSelAll=FALSE;
		iShiftOp=2;iAltGrOp=0;
		GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
		if(GetmHpVarStruct()->SelItemListe->IsListeEmpty())
		{	// No Selection
			iNextSel=NO;
			GetmHpVarStruct()->YnextPos=(int)(CtrlRct.top+15);
			if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,WS_VSCROLL)!=WS_VSCROLL) GetmHpVarStruct()->XnextPos=(int)(CtrlRct.right-25);
			else GetmHpVarStruct()->XnextPos=(int)(CtrlRct.right-41);
			if(ListView_GetSelectedCount(GetmHpVarStruct()->hWndCurCtrl)==0)
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(9); // No Click, Délai, Fin
			}
		}
		else
		{   int iSelTab[MAX_RS_SIZE]={0};
	        ListView_GetSelItems(GetmHpVarStruct()->hWndCurCtrl,iSelTab,MAX_RS_SIZE);
			if(IsAlreadySelected(iSelTab)) // May Change "iAltGrOp"
			{	// Already Selected
				iNextSel=NO;
				GetmHpVarStruct()->YnextPos=(int)(CtrlRct.top+15);
				GetmHpVarStruct()->XnextPos=(int)(CtrlRct.right-25);
				GetmHpVarStruct()->CourantTask->SetmHpStep(9); // No Click, Délai, Fin
			}
			else
			{	if((iAltGrOp!=1)&&((IsShiftOperation())||(IsMoreSelThanNoSel(iSelTab))))
				{	if(IsShiftOperation()) iShiftOp=0;
					iNextSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
					GetNextItemPosCur(hWndActive,GetmHpVarStruct()->hWndCurCtrl,iNextSel,CtrlRct);
				}
				else
				{	// Désélectionne tous
					iAltGrOp=0;
					bUnSelAll=TRUE;
					iNextSel=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
					GetmHpVarStruct()->YnextPos=(int)(CtrlRct.top+((CtrlRct.bottom-CtrlRct.top)/2));
					GetmHpVarStruct()->XnextPos=(int)(CtrlRct.left+((CtrlRct.right-CtrlRct.left)/2));
				}
			}
		}
	}
	return TRUE;
}

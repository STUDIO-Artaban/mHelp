// TreeView.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <TreeView\TreeView.h>

//-------------------------------//
#include "mHp2D.h"
#include "resource.h"

#ifndef  __MHPSOUND_H
 #include "mHpSnd.h"
#endif

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif

//-----------------------------------------------------//
// Class name
#define		CPP_TASK_TREEVIEW			"SysTreeView32"                     // C/C++ control
#define		VB_TASK_TREEVIEW			"TreeViewWndClass"                  // VB control

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles
HTREEITEM hCurSelItm=NULL;
BOOL bFindItm;

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration
void GetTreeItemSels(HWND,WPARAM,CmHpTask*);
BOOL GetTVitemSelList(HWND,HTREEITEM,int);
HTREEITEM GetNexthTreeItemToSel(HWND,HTREEITEM);
BOOL FindCurItemBetween(HWND,HTREEITEM,HTREEITEM);
void MoveOnScrollBar(HWND,RECT);

//-----------------------------------------------------------------------------------------------------------------------//
// GetTreeItemSels
//---------------------------------------------------//
void GetTreeItemSels(HWND hWndTV,WPARAM wParam,CmHpTask* ObjItem)
{	char lptcitm[MAX_RS_SIZE]={0};
	TVITEM pItem;
	pItem.pszText=lptcitm;
    pItem.mask=TVIF_TEXT;
    pItem.cchTextMax=MAX_RS_SIZE;
    /*if(wParam!=NULL) // DotNet TreeView message
    {   int* iSelTab=(int*)wParam; // See mHelpTreeViewEvent() method
        HTREEITEM hTreeItm=TreeView_GetRoot(hWndTV);
        for(int i=0; (hTreeItm!=NULL)&&(iSelTab[i]!=NO); i++)
        {   // Get indexed item
            for(int j=iSelTab[i]; j!=0; j--)
                hTreeItm=TreeView_GetNextItem(hWndTV,hTreeItm,TVGN_NEXT);
            if(hTreeItm!=NULL)
            {   pItem.hItem=hTreeItm;
		        if(TreeView_GetItem(hWndTV,&pItem))
		        {	char lpSel[MAX_RS_SIZE]={0};
                    strncpy(lpSel,pItem.pszText,MAX_RS_SIZE);
			        ObjItem->AddMultiIndex(lpSel);
                    // Get child item
                    hTreeItm=TreeView_GetNextItem(hWndTV,hTreeItm,TVGN_CHILD);
		        }
                else break;
            }
            else break;
        }
    }
    else*/
    {   HTREEITEM hTreeItm=TreeView_GetSelection(hWndTV);
	    while(hTreeItm!=NULL)
	    {	pItem.hItem=hTreeItm;
		    if(TreeView_GetItem(hWndTV,&pItem))
		    {	char lpSel[MAX_RS_SIZE]={0};
                strncpy(lpSel,pItem.pszText,MAX_RS_SIZE);
			    ObjItem->AddMultiIndex(lpSel);
		    }
		    hTreeItm=TreeView_GetNextItem(hWndTV,hTreeItm,TVGN_PARENT);
	    }
	    ObjItem->InverseMultiIndex();
    }
}

//---------------------------------------------------//
// GetTVitemSelList
//---------------------------------------------------//
BOOL GetTVitemSelList(HWND hWndTV,HTREEITEM hItmStart,int StrIndex)
{	HTREEITEM hItm=hItmStart;
	TVITEM pitem;
	int index=0;
	char lpItmTxt[MAX_RS_SIZE]={0};
	pitem.pszText=lpItmTxt;
    pitem.mask=TVIF_TEXT;
	if(GetmHpVarStruct()->CourantTask->GetMultiIndex(StrIndex)==NULL) return TRUE;
	while(hItm!=NULL)
	{	GetmHpVarStruct()->SelItemListe->AddItemIndex(index);
		pitem.cchTextMax=MAX_RS_SIZE;
		pitem.hItem=hItm;
		TreeView_GetItem(hWndTV,&pitem);
		if(!strncmp(pitem.pszText,GetmHpVarStruct()->CourantTask->GetMultiIndex(StrIndex),MAX_RS_SIZE))
		{	if(GetTVitemSelList(hWndTV,TreeView_GetNextItem(hWndTV,hItm,TVGN_CHILD),(StrIndex+1))) return TRUE;
		}
		hItm=TreeView_GetNextItem(hWndTV,hItm,TVGN_NEXT);
		index++;
		GetmHpVarStruct()->SelItemListe->RemoveLastIndexItem();
	}
	return FALSE;
}

//---------------------------------------------------//
// GetNexthTreeItemToSel
//---------------------------------------------------//
HTREEITEM GetNexthTreeItemToSel(HWND hWndTV,HTREEITEM hCurItem)
{	HTREEITEM hItm=hCurItem;
	TVITEM pitm;
	BOOL bExpanded=TRUE;
	int index=GetmHpVarStruct()->SelItemListe->GetItemIndex();
	if(index!=NO)
	{	while((bExpanded)&&(index!=NO))
		{	while(index!=0)
			{	hItm=TreeView_GetNextItem(hWndTV,hItm,TVGN_NEXT);
				index--;
			}
			pitm.mask=TVIF_STATE;
			pitm.stateMask=TVIS_STATEIMAGEMASK;
			pitm.hItem=hItm;
			TreeView_GetItem(hWndTV,&pitm);
			if((pitm.state&TVIS_EXPANDED)==TVIS_EXPANDED)
			{	GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
				index=GetmHpVarStruct()->SelItemListe->GetItemIndex();
				hItm=TreeView_GetNextItem(hWndTV,hItm,TVGN_CHILD);
				bExpanded=TRUE;
			}
			else bExpanded=FALSE;
		}
	}
	else hItm=NULL;
	return hItm;
}

//---------------------------------------------------//
// FindCurItemBetween
//---------------------------------------------------//
BOOL FindCurItemBetween(HWND hWndTV,HTREEITEM itemStart,HTREEITEM itemEnd)
{	HTREEITEM hItm=itemStart;
	while((hItm!=NULL)&&(hItm!=itemEnd)&&(hItm!=hCurSelItm))
	{	if(TreeView_GetNextItem(hWndTV,hItm,TVGN_CHILD)!=NULL)
		{	if(FindCurItemBetween(hWndTV,TreeView_GetNextItem(hWndTV,hItm,TVGN_CHILD),itemEnd)) return TRUE;
		}
		hItm=TreeView_GetNextItem(hWndTV,hItm,TVGN_NEXT);
	}
	if((hItm!=NULL)&&(hItm!=itemEnd)) return TRUE;
	return FALSE;
}

//---------------------------------------------------//
// MoveOnScrollBar
//---------------------------------------------------//
void MoveOnScrollBar(HWND hWndTV,RECT RectTV)
{	switch(GetmHpVarStruct()->AscensSens)
	{	case 1: // Up
		{	GetmHpVarStruct()->YnextPos=(int)(RectTV.top+6);
			GetmHpVarStruct()->XnextPos=(int)(RectTV.right-9);
			break;
		}
		case 2: // Down
		{	if(IsWndStyle(hWndTV,WS_HSCROLL)==WS_HSCROLL) GetmHpVarStruct()->YnextPos=(int)(RectTV.bottom-27);
			else GetmHpVarStruct()->YnextPos=(int)(RectTV.bottom-12);
			GetmHpVarStruct()->XnextPos=(int)(RectTV.right-11);
			break;
		}
		case 3: // Right
		{	GetmHpVarStruct()->YnextPos=(int)(RectTV.bottom-12);
			if(IsWndStyle(hWndTV,WS_VSCROLL)==WS_VSCROLL) GetmHpVarStruct()->XnextPos=(int)(RectTV.right-24);
			else GetmHpVarStruct()->XnextPos=(int)(RectTV.right-10);
			break;
		}
		case 4: // Left
		{	GetmHpVarStruct()->YnextPos=(int)(RectTV.bottom-12);
			GetmHpVarStruct()->XnextPos=(int)(RectTV.left+9);
			break;
		}
	}
}

//*****************************************************************************************************************OBJECT//
// Class CTreeViewTask
CTreeViewTask::CTreeViewTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,char* lpCtrlName)
 			  :CListTask(13,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,lpCtrlName)
{}

CTreeViewTask::CTreeViewTask()
 			  :CListTask()
{}

//****************************************************************************************************************MHPFILE//
// TreeView_ReadEvent
//---------------------------------------------------//
BOOL TreeView_ReadEvent(HWND hWndActive,const std::string& strPostData,UINT &iCustIdx)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpSel[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	char chNext=END_RS_TASK;
	int iCur,iCnt,iTmp,iOpt,iCntSel;
	BOOL bNoSel=FALSE,bContinue=TRUE,bError;
	CTreeViewTask *TViewTask;
	// iCurrentWnd,iCountWnd,iOption,lpmHpMsg
	GetCommonEvent(hWndActive,iCur,iCnt,iOpt,lpMsg,MAX_RS_SIZE,lpStateMsg,MAX_RS_SIZE,chNext,iTmp);
	if(iTmp==NO) return TRUE;
    // mHpCtrlName
    char lpCtrlName[MAX_CTRL_NAME]={0};
    mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpCtrlName,MAX_CTRL_NAME,chNext,TRUE);
	if(iTmp==NO) return TRUE;
	// mHpCtrlID
	ReadmHpCtrlID(hWndActive,GetmHpVarStruct()->mHpFileName,lpctrl,MAX_PART_TREE,chNext);
	if((lpctrl[0]==NO)&&(*lpCtrlName==0)) return TRUE;
	// lpItemSel
	if(*lpCtrlName!=0) TViewTask=new CTreeViewTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName);
    else TViewTask=new CTreeViewTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL);
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
						if(iTmp!=NO) TViewTask->AddMultiIndex(lpSel);
						iCntSel++;
					}
					if(iCntSel==1) return TRUE; // WARNING: There is something wrong here!!!!
				}
				bContinue=FALSE;
			}
			else TViewTask->AddMultiIndex(lpSel);
		}
		else return TRUE;
	}
#else
    ////// TODO: Be able to remove lines below (state always in HTML)
	while(bContinue)
	{	GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpSel,MAX_RS_SIZE,bContinue,bError);
		if(bError)
		    return TRUE; // Error
        //TViewTask->AddMultiIndex(lpSel);
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
                TViewTask->AddMultiIndex(lpState);
            }
        }
        else
            return TRUE; // WARNING: There is something wrong here!!!!
    }
    // Increase number of custom task
    iCustIdx++;
#endif
	if(!bNoSel) GetmHpVarStruct()->TaskList->AddCellule(TViewTask);
	return FALSE; // OK
}

//---------------------------------------------------//
// TreeView_WriteEvent
//---------------------------------------------------//
void TreeView_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
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

//---------------------------------------------------//
// TreeView_WriteParam
//---------------------------------------------------//
BOOL TreeView_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),FALSE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}			

//*******************************************************************************************************************KEYS//
// TreeView_ReadEventKeys
//---------------------------------------------------//
void TreeView_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpParam* ReadParam;
	ReadTask=new CTreeViewTask();
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
// TreeView_AddEvent
//---------------------------------------------------//
void TreeView_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndTreeView,WPARAM wParam,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
    if(*lpCtrlName!=0)
	    Item=new CTreeViewTask(NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName);
    else
    {	int lpctrl[MAX_PART_TREE]={0};
    	AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndTreeView));
	    Item=new CTreeViewTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL);
    }
	GetTreeItemSels(hWndTreeView,wParam,Item);
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
// TreeView_IsClassName
//---------------------------------------------------//
BOOL TreeView_IsClassName(char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_TREEVIEW,ClassSize))||
       (!strncmp(CtrlClass,VB_TASK_TREEVIEW,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_TREEVIEW,sizeof(DOTNET_TASK_TREEVIEW)-1)))
	{	return TRUE;}
	return FALSE;
}

//---------------------------------------------------//
// TreeView_IsCustom
//---------------------------------------------------//
void TreeView_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//---------------------------------------------------//
// TreeView_StrState
//---------------------------------------------------//
void TreeView_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
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

//---------------------------------------------------//
// TreeView_ParamType
//---------------------------------------------------//
int TreeView_ParamType()
{	return 9; // LS
}

//-----------------------------------------------------//
// TreeView_DefaultBound
//-----------------------------------------------------//
void TreeView_DefaultBound(char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	strncpy(pStrBound,LISTSTR_LIMITE,BoundSize);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
	strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
}

//-----------------------------------------------------//
// TreeView_IsCustomized
//-----------------------------------------------------//
BOOL TreeView_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------//
// TreeView_IsNoStateMsg
//-----------------------------------------------------//
BOOL TreeView_IsNoStateMsg()
{	return TRUE;}

//-----------------------------------------------------//
// TreeView_GetHtmlTag
//-----------------------------------------------------//
#define TREEVIEW_HTML_TAG   "\t<input type=\"text\" name=\"%d-%s\" maxlength=\"100\" value=\"%s\"/>\r\n"

void TreeView_GetHtmlTag(CEventCell* pCurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
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
              TREEVIEW_HTML_TAG,
              iCustIndex,
              LISTSEL_KEY,
              lpState);
}

//***************************************************************************************************************CFGTOOLS//
// TreeView_ShowCfgTools
//---------------------------------------------------//
void TreeView_ShowCfgTools(HWND hWndActive)
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

//---------------------------------------------------//
// TreeView_HideCfgTools
//---------------------------------------------------//
void TreeView_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_REMOVE_MULTILIST),SW_HIDE);
}

//---------------------------------------------------//
// TreeView_SaveCfg
//---------------------------------------------------//
void TreeView_SaveCfg(HWND hWndActive)
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

//---------------------------------------------------//
// TreeView_LoadCfg
//---------------------------------------------------//
void TreeView_LoadCfg(HWND hWndActive)
{	HWND hWndMultiItm=GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST);
	EnableWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),FALSE);
	while(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetNextIndex())
	{	SendMessage(hWndMultiItm,LB_ADDSTRING,0,(LPARAM)GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMultiIndex(NO));
	}
}

//---------------------------------------------------//
// TreeView_GetInfosCfg
//---------------------------------------------------//
void TreeView_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_TREEVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_TREEVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_TREEVIEW,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_TREEVIEW,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TREEVIEW)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// TreeView_ProcCfg
//---------------------------------------------------//
BOOL TreeView_ProcCfg(HWND hWndActive,UINT Message,WPARAM wParam,HWND hWndChng)
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
				if(iSel==CB_ERR)
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
// TreeView_CaptureMsg
//---------------------------------------------------//
void TreeView_CaptureMsg(HWND hWndActive,char* lpCtrlName,WPARAM wpParam,LPARAM lpParam,HWND hToolWnd,int iCurOp,CEventListe* EventObj,
						 char* lpNameApp,UINT &pIdLast,int pCurwnd,int pCntwnd,BOOL &pActionUser,int &pResCapt)
{	int iWriteRes=0;
	if(((LPNMHDR)lpParam)->code==TVN_SELCHANGED)
	{	if(*lpCtrlName!=0)
            WriteImmediatTask(hWndActive,lpCtrlName,((LPNMHDR)lpParam)->hwndFrom,hToolWnd,wpParam,iCurOp,13,
                             (UINT)((LPNMHDR)lpParam)->idFrom,EventObj,NULL,0,lpNameApp,pIdLast,pCurwnd,pCntwnd,
                             iWriteRes);
        else
            WriteImmediatTask(hWndActive,lpCtrlName,((LPNMHDR)lpParam)->hwndFrom,hToolWnd,NULL,iCurOp,13,
                              (UINT)((LPNMHDR)lpParam)->idFrom,EventObj,NULL,0,lpNameApp,pIdLast,pCurwnd,pCntwnd,
                              iWriteRes);
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
        // Stop this message if no notify DotNet control message
        /*if(*lpCtrlName!=0)
            pResCapt=7;*/
	}
}

//---------------------------------------------------//
// TreeView_mHpProcess
//---------------------------------------------------//
void TreeView_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2: // 1° CLICK
		{	EndObjects(hWndActive);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetDDrawObj(hWndActive,7);
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3: // DELAY
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_DELAI,50,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4: // UNCLICK + Selection
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			EndObjects(hWndActive);
			if(GetFocus()!=GetmHpVarStruct()->hWndCurCtrl) SetFocus(GetmHpVarStruct()->hWndCurCtrl);
			TreeView_SelectItem(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetDDrawObj(hWndActive,0);
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 5: // DELAY
		{	SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6: // 2° CLICK
		{	EndObjects(hWndActive);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetDDrawObj(hWndActive,8);
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7: // DELAY
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_DELAI,50,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 8: // UNCLICK + Expand
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			TreeView_Expand(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm,TVE_EXPAND);
			EndObjects(hWndActive);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetDDrawObj(hWndActive,0);
			if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 9: // DELAY
		{	SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 10: // NEXT SELECTION
		{	GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
			hCurSelItm=GetNexthTreeItemToSel(GetmHpVarStruct()->hWndCurCtrl,TreeView_GetChild(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm));
			if(hCurSelItm!=NULL)
			{	RECT CtlRct;
			    GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&CtlRct);
				GetmHpVarStruct()->bRefreshDDraw=TRUE;
				GetmHpVarStruct()->bNewDepl=TRUE;
				RECT ItmRct;
			    TreeView_GetItemRect(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm,&ItmRct,TRUE);
				ListTreeStatusPos(hWndActive,ItmRct);
				if(!GetmHpVarStruct()->AscensSens)
				{	// When the last visible item is selected, that launch an auto-scroll
					TreeView_EnsureVisible(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm);
					TreeView_GetItemRect(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm,&ItmRct,TRUE);
					//
					GetmHpVarStruct()->YnextPos=(int)(CtlRct.top+((ItmRct.bottom-ItmRct.top)/2)+ItmRct.top);
					GetmHpVarStruct()->XnextPos=(int)(CtlRct.left+((ItmRct.right-ItmRct.left)/2)+ItmRct.left);
					// DoubleClick | Click
					if(GetmHpVarStruct()->SelItemListe->IsNextIndexExist()) GetmHpVarStruct()->CourantTask->SetmHpStep(1);
				}
				else MoveOnScrollBar(GetmHpVarStruct()->hWndCurCtrl,CtlRct); // Not visible
				SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			}
			else // END
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(14);
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 11: // CLICK
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetDDrawObj(hWndActive,0);
			GetmHpVarStruct()->CursorType=1;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 12: // DELAY
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 13: // LAST SELECTION
		{	RECT ctlRct;
			RECT itmRct;
			int iLastAscens;
			if(!GetmHpVarStruct()->AscensSens)
			{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
				EndObjects(hWndActive);
				if(GetFocus()!=GetmHpVarStruct()->hWndCurCtrl) SetFocus(GetmHpVarStruct()->hWndCurCtrl);
				TreeView_SelectItem(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm);
				GetmHpVarStruct()->bRefreshDDraw=TRUE;
				SetDDrawObj(hWndActive,0);
				GetmHpVarStruct()->CourantTask->SetmHpStep(14);
				if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
				else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			}
			else
			{	switch(GetmHpVarStruct()->AscensSens)
				{	case 1: // UP
				    {	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                        else
                        {   UpdateBackBuffer(TRUE);
						    SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_VSCROLL,
									    MAKEWPARAM(SB_THUMBPOSITION,(GetScrollPos(GetmHpVarStruct()->hWndCurCtrl,SB_VERT)-1)),NULL);
                            if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                        }
						break;
					}
					case 2: // DOWN
				    {	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                        else
                        {   UpdateBackBuffer(TRUE);
						    SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_VSCROLL,
									    MAKEWPARAM(SB_THUMBPOSITION,(GetScrollPos(GetmHpVarStruct()->hWndCurCtrl,SB_VERT)+1)),NULL);
                            if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                        }
						break;
					}
					case 3: // RIGHT
				    {	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                        else
                        {   UpdateBackBuffer(TRUE);
						    SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_HSCROLL,
									    MAKEWPARAM(SB_THUMBPOSITION,(GetScrollPos(GetmHpVarStruct()->hWndCurCtrl,SB_HORZ)+1)),NULL);
                            if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                        }
						break;
					}
					case 4: // LEFT
				    {	if(!RestoreBackBuffer(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                        else
                        {   UpdateBackBuffer(TRUE);
						    SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_HSCROLL,
									    MAKEWPARAM(SB_THUMBPOSITION,(GetScrollPos(GetmHpVarStruct()->hWndCurCtrl,SB_HORZ)-1)),NULL);
                            if(!UpdateFrame(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                        }
						break;
					}
				}
				iLastAscens=GetmHpVarStruct()->AscensSens;
				GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&ctlRct);
				TreeView_GetItemRect(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm,&itmRct,TRUE);
				ListTreeStatusPos(hWndActive,itmRct);
				if(!GetmHpVarStruct()->AscensSens)
				{	// When the last visible item is selected, that launch an auto-scroll
					TreeView_EnsureVisible(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm);
					TreeView_GetItemRect(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm,&itmRct,TRUE);
					//
					GetmHpVarStruct()->YnextPos=(int)(ctlRct.top+((itmRct.bottom-itmRct.top)/2)+itmRct.top);
					GetmHpVarStruct()->XnextPos=(int)(ctlRct.left+((itmRct.right-itmRct.left)/2)+itmRct.left);
					// DoubleClick | Click
					if(GetmHpVarStruct()->SelItemListe->IsNextIndexExist()) GetmHpVarStruct()->CourantTask->SetmHpStep(1);
					else GetmHpVarStruct()->CourantTask->SetmHpStep(10);
					PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
					EndObjects(hWndActive);
					GetmHpVarStruct()->bRefreshDDraw=TRUE;
					GetmHpVarStruct()->bNewDepl=TRUE;
					SetDDrawObj(hWndActive,0);
					if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
					else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
				}
				else // Not visible
				{	if(iLastAscens!=GetmHpVarStruct()->AscensSens)
					{	MoveOnScrollBar(GetmHpVarStruct()->hWndCurCtrl,ctlRct);
						GetmHpVarStruct()->CourantTask->SetmHpStep(10);
						PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
						EndObjects(hWndActive);
						GetmHpVarStruct()->bRefreshDDraw=TRUE;
						GetmHpVarStruct()->bNewDepl=TRUE;
						SetDDrawObj(hWndActive,0);
						if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
						else SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
					}
					else
					{	GetmHpVarStruct()->CourantTask->SetmHpStep(12);
						SetTimer(hWndActive,ID_TIMER_DELAI,200,(TIMERPROC)mHpTimerProc);
					}
				}
			}
			break;
		}
		case 14:
		{	if((!bFindItm)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10)) ViewCtrlNotReadyMsg(hWndActive,4);
			else SetTimer(hWndActive,ID_TIMER_DELAI,300,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 15:
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
// TreeView_StartProc
//---------------------------------------------------//
BOOL TreeView_StartProc(HWND hWndActive)
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
	GetmHpVarStruct()->SelItemListe->EmptyGroupListe();
	GetTVitemSelList(GetmHpVarStruct()->hWndCurCtrl,TreeView_GetRoot(GetmHpVarStruct()->hWndCurCtrl),0);
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	hCurSelItm=GetNexthTreeItemToSel(GetmHpVarStruct()->hWndCurCtrl,TreeView_GetRoot(GetmHpVarStruct()->hWndCurCtrl));
	if(hCurSelItm==NULL)
	{	bFindItm=FALSE;
		GetmHpVarStruct()->CourantTask->SetmHpStep(13);
		GetmHpVarStruct()->YnextPos=(int)(((CtrlRct.bottom-CtrlRct.top)/2)+CtrlRct.top);
		GetmHpVarStruct()->XnextPos=(int)(((CtrlRct.right-CtrlRct.left)/2)+CtrlRct.left);
	}
	else
	{	bFindItm=TRUE;
		if(GetmHpVarStruct()->imHpMode==1) TreeView_EnsureVisible(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm);
	    RECT itmRct;
		TreeView_GetItemRect(GetmHpVarStruct()->hWndCurCtrl,hCurSelItm,&itmRct,TRUE);
		ListTreeStatusPos(hWndActive,itmRct);
		if(!GetmHpVarStruct()->AscensSens)
		{	GetmHpVarStruct()->YnextPos=(int)(CtrlRct.top+((itmRct.bottom-itmRct.top)/2)+itmRct.top);
			GetmHpVarStruct()->XnextPos=(int)(CtrlRct.left+((itmRct.right-itmRct.left)/2)+itmRct.left);
			// Click | DoubleClick
			if(!GetmHpVarStruct()->SelItemListe->IsNextIndexExist()) GetmHpVarStruct()->CourantTask->SetmHpStep(10);
		}
		else // Not visible
		{	GetmHpVarStruct()->CourantTask->SetmHpStep(10);
			MoveOnScrollBar(GetmHpVarStruct()->hWndCurCtrl,CtrlRct);
		}
	}
	return TRUE;
}

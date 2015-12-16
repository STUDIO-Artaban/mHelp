//-----------------------------------------------------//
// Fichier ServDlg.cpp
//
// 
//-----------------------------------------------------//

#include "stdafx.h"
#include "servdlg.h"
#include "helpcom.h"
#include "resource.h"
#include <commdlg.h>

//-----------------------------------------------------//
// Définitions
#define				MHP_EXTENSION				".mhf"

//-----------------------------------------------------//
// Variables globales
HIMAGELIST hServImgList=NULL;
HIMAGELIST hCtrlImgList=NULL;
HIMAGELIST hParamImgList=NULL;
HWND hWndServRS;
HWND hWndbtnAdd;
HWND hWndbtnNext;
HWND hWndbtnLast;
HWND hWndConfigTask;
HWND hWndListTask;
HWND hWndInfo;
HWND hWndMsg;
HWND hWndmHpSave;
HWND hWndNewmHp;
HWND hWndOpenmHp;
HWND hWndRemovemHp;
HWND hWndStaticState;
HWND hWndServStatus;
HWND hWndServProgress;
HWND hWndFolder;
HWND hWndDoc;
HWND hWndParam;
HWND hWndFrameRS;
HWND hWndMoveUp;
HWND hWndMoveDown;
HWND hWndListParam;
HWND hWndSaveNew;
HWND hWndHideShow;
HWND hWndTerminate;
HWND hWndHelpDoc;
HWND hWndHideServTree;
int ServIndexTab[MAX_PART_TREE]={0};
BOOL bRepParam;
int iSelCtrl;
int iSelParam;
char lpServRC[255]={0};
LONG_PTR lmHpPrevProc;
CWndDocList* ServDlgDocLst;
int iStep;
BOOL bBugClose;
BOOL bTaskSaveFlag;

//------------------------------------------------------//
// Déclarations globales
LRESULT CALLBACK ServDlgDocProc(HWND,UINT,WPARAM,LPARAM);
void SetServDlgDocLnch();
void SetmHpSaveCtrls();
void WritemHelpTask(HWND,char*,BOOL);
BOOL IsCtrlVisible();
void GetNewTopic(char*,int,int);
void CreateSaveNewBtn(int);
void SetEditListViewStyle(BOOL);
int InsertNewmHpFile();
void EnableRadiosRS(int);
BOOL WriteTreeRS(HWND,char*);
BOOL IsParamFolder(HTREEITEM);
void DecalTaskRefProc(HWND,char*,int&);
void ShowServCtrl(int);
void EnableEditTree(HWND,BOOL);
BOOL AddParamDetail(HWND,HTREEITEM,int);
BOOL CreateServTree(HWND,int);
LRESULT CALLBACK mHpFileProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK mHpServerProc(HWND,UINT,WPARAM,LPARAM);

//-----------------------------------------------------------------------------------------------------------------------//
// ServDlgDocProc
//-----------------------------------------------------//
LRESULT CALLBACK ServDlgDocProc(HWND hWndDocLnch,UINT DocMessage,WPARAM DocWParam,LPARAM DocLParam)
{	LONG_PTR lPrevProc=ServDlgDocLst->GetDocWndProc(hWndDocLnch);
	switch(DocMessage)
	{	case WM_KEYUP:
		{	switch(DocWParam)
			{	case VK_F1:
				{	mHpPak_OpenmHelpDoc(hWndDocLnch,MHPPAK_DOC_SERV+iStep);
					break;
				}
				default:
				{	break;}
			}
			break;
		}
		case WM_NCDESTROY:
		{	ServDlgDocLst->RemoveDocWnd(hWndDocLnch);
			break;
		}
	}
	return CallWindowProc((WNDPROC)lPrevProc,hWndDocLnch,DocMessage,DocWParam,DocLParam);
}

//---------------------------------------------------------//
// SetServDlgDocLnch
//---------------------------------------------------------//
void SetServDlgDocLnch()
{	ServDlgDocLst->AddDocWnd(hWndServRS,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndbtnAdd,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndbtnNext,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndbtnLast,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndConfigTask,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndListTask,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndNewmHp,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndOpenmHp,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndRemovemHp,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndFolder,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndDoc,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndParam,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndMoveUp,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndMoveDown,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndListParam,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndHideShow,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndTerminate,(LONG_PTR)ServDlgDocProc);
	ServDlgDocLst->AddDocWnd(hWndHelpDoc,(LONG_PTR)ServDlgDocProc);
}

//---------------------------------------------------------//
// SetmHpSaveCtrls
//---------------------------------------------------------//
void SetmHpSaveCtrls()
{	int imHpSel=ListView_GetNextItem(hWndmHpSave,NO,LVNI_SELECTED);
	DestroyWindow(hWndSaveNew);
    hWndSaveNew=NULL;
	if(imHpSel!=NO)
	{	EnableWindow(hWndRemovemHp,TRUE);
		if(!GetHelpComStruct()->mHpFileList->GetNewFile(imHpSel))
		{	SetEditListViewStyle(FALSE);
			EnableWindow(hWndbtnNext,TRUE);
		}
		else
		{	SetEditListViewStyle(TRUE);
			EnableWindow(hWndbtnNext,FALSE);
			SetFocus(hWndTerminate);
			CreateSaveNewBtn(imHpSel);
		}
	}
	else
	{	EnableWindow(hWndbtnNext,FALSE);
		SetFocus(hWndTerminate);
		EnableWindow(hWndRemovemHp,FALSE);
	}
}

//---------------------------------------------------------//
// WritemHelpTask
//---------------------------------------------------------//
void WritemHelpTask(HWND hWndActive,char* lpmHpSaveFile,BOOL bFileEmpty)
{
#ifdef MHELP_OLD
    CWindowListe* TmpWndLst;
	SetmHpCursor(hWndActive,1);
	if(WriteTreeRS(hWndActive,lpmHpSaveFile))
	{	Sleep(1000);
        GetHelpComStruct()->AddList->EmptyItmList();
		TmpWndLst=new CWindowListe();
        if(mHpCtrl_WriteWindowLst(hWndActive,lpmHpSaveFile,TmpWndLst,bFileEmpty))
        {   Sleep(1000);
            if((bFileEmpty)||(mHpPak_GetControlList(hWndActive,lpmHpSaveFile)))
            {   BOOL bEmptyCtrlList=mHpPak_ProcCtrlList->IsEmpty();
                mHpPak_ProcCtrlList->SetUpdated(FALSE);
                if(mHpCtrl_WriteEventTask(hWndActive,lpmHpSaveFile,GetHelpComStruct()->mHpCtrlObj->mHpEventList,TmpWndLst,bRepParam))
                {   Sleep(1000);
                    if((!mHpPak_ProcCtrlList->IsUpdated())||(mHpCtrl_WriteCtrlList(hWndActive,lpmHpSaveFile,bEmptyCtrlList)))
                    {   Sleep(1000);
                        delete(TmpWndLst);
			            SetmHpCursor(hWndActive,0);
			            //
			            LoadString(GetHelpComStruct()->hInstServ,IDS_SAVE_SUCCESSFUL,lpServRC,255);
			            MessageBox(hWndActive,lpServRC,MHPPAK_DLGMSGTITLE,MB_ICONINFORMATION);
                        bTaskSaveFlag=TRUE;
                        return;
                    }
                }
            }
        }
		delete(TmpWndLst);
		SetmHpCursor(hWndActive,0);
	}
	else
	{	GetHelpComStruct()->AddList->EmptyItmList();
		SetmHpCursor(hWndActive,0);
	}
#else
    SetmHpCursor(hWndActive,1);
    GetHelpComStruct()->AddList->EmptyItmList();
	int iLine=mHpPak_GetTaskLine(hWndActive,lpmHpSaveFile,FALSE);
	if((iLine==NO)||(!mHpPak_InsertInmHpFile(hWndActive,lpmHpSaveFile,iLine,FALSE,FALSE)))
    {   SetmHpCursor(hWndActive,0);
		return;
    }
    mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
    mHpPak_AddIndex(ServIndexTab,0);
    GetIndexFromTree(TreeView_GetSelection(hWndServRS),hWndServRS,ServIndexTab);
	int indice=0;
    char lpIndexTab[MAX_RS_SIZE]={0};
	while(ServIndexTab[indice]!=NO)
	{	char lpIndex[16];
        _snprintf(lpIndex,16,"%d",ServIndexTab[indice]);
        if(!indice)
		{	if(!mHpPak_WriteParty(hWndActive,START_TASK,ServIndexTab[indice],NULL,FALSE)) // '#'
            {   SetmHpCursor(hWndActive,0);
			    return;
            }
            //
            strncpy(lpIndexTab,lpIndex,MAX_RS_SIZE);
		}
		else
        {   if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,ServIndexTab[indice],NULL,FALSE)) // '¤'
            {   SetmHpCursor(hWndActive,0);
			    return;
            }
            //
            strncat(lpIndexTab,"-",sizeof("-"));
            strncat(lpIndexTab,lpIndex,strlen(lpIndex));
        }
		indice++;
	}
	if((!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))||(!mHpPak_ConcatInmHpFile(hWndActive,lpmHpSaveFile,iLine,FALSE))) // '\n'
    {   SetmHpCursor(hWndActive,0);
		return;
    }
	Sleep(1000);
	CWindowListe* TmpWndLst=new CWindowListe();
    if(mHpCtrl_WriteWindowLst(hWndActive,lpmHpSaveFile,TmpWndLst,bFileEmpty))
    {   Sleep(1000);
        if((bFileEmpty)||(mHpPak_GetControlList(hWndActive,lpmHpSaveFile)))
        {   BOOL bEmptyCtrlList=mHpPak_ProcCtrlList->IsEmpty();
            mHpPak_ProcCtrlList->SetUpdated(FALSE);
            if(mHpCtrl_WriteEventTask(hWndActive,lpmHpSaveFile,GetHelpComStruct()->mHpCtrlObj->mHpEventList,TmpWndLst,bRepParam))
            {   Sleep(1000);
                if((!mHpPak_ProcCtrlList->IsUpdated())||(mHpCtrl_WriteCtrlList(hWndActive,lpmHpSaveFile,bEmptyCtrlList)))
                {   Sleep(1000);
                    char lpFile[MAX_RS_SIZE]={0};
                    SelectHtmlURL(lpFile,MAX_RS_SIZE,hWndServRS,hWndHideServTree,NULL,NULL);
                    if(mHpCtrl_InsertHtmlTag(hWndActive,GetHelpComStruct()->mHpCtrlObj->mHpEventList,lpFile,lpmHpSaveFile,ServIndexTab,lpIndexTab))
                    {   delete(TmpWndLst);
			            SetmHpCursor(hWndActive,0);
			            //
			            LoadString(GetHelpComStruct()->hInstServ,IDS_SAVE_SUCCESSFUL,lpServRC,255);
			            MessageBox(hWndActive,lpServRC,MHPPAK_DLGMSGTITLE,MB_ICONINFORMATION);
                        bTaskSaveFlag=TRUE;
                        return;
                    }
                }
            }
        }
    }
	delete(TmpWndLst);
	SetmHpCursor(hWndActive,0);
#endif
}

//---------------------------------------------------------//
// IsCtrlVisible
//---------------------------------------------------------//
BOOL IsCtrlVisible()
{	int iVisibleCnt=0;
	for(int i=0;i<ListView_GetItemCount(hWndListParam);i++)
	{	if(mHpCtrl_IsCustomVisible(GetHelpComStruct()->mHpCtrlObj->mHpEventList,i)) iVisibleCnt++;
	}
	if(iVisibleCnt>1) return TRUE;
	return FALSE;
}

//---------------------------------------------------------//
// GetNewTopic
//---------------------------------------------------------//
void GetNewTopic(char* lpNewTopic,int TopicSize,int iIndex)
{	char lplvitm[MAX_RS_SIZE]={0};
	LVITEM pItem;
	pItem.mask=LVIF_TEXT; 
	pItem.iItem=iIndex; 
	pItem.iSubItem=0; 
	pItem.cchTextMax=MAX_RS_SIZE;
	pItem.pszText=lplvitm;
	if(ListView_GetItem(hWndmHpSave,&pItem)) strncpy(lpNewTopic,pItem.pszText,TopicSize);
	else strncpy(lpNewTopic,NULL_STRING,TopicSize);
}

//-----------------------------------------------------//
// CreateSaveNewBtn
//-----------------------------------------------------//
void CreateSaveNewBtn(int iIndex)
{	RECT Rect,RectBnd;
	int iDecal;
	if((ListView_GetTopIndex(hWndmHpSave)<=iIndex)&&
	  ((ListView_GetCountPerPage(hWndmHpSave)+ListView_GetTopIndex(hWndmHpSave))>iIndex))
	{	GetWindowRect(hWndmHpSave,&Rect);
		ListView_GetItemRect(hWndmHpSave,iIndex,&RectBnd,LVIR_BOUNDS);
		if(ListView_GetCountPerPage(hWndmHpSave)<ListView_GetItemCount(hWndmHpSave)) iDecal=37;
		else iDecal=21;
		hWndSaveNew=CreateWindowEx(WS_EX_TOPMOST|WS_EX_TRANSPARENT,
								"Button","...",
								WS_VISIBLE|WS_CHILD,
								(int)((Rect.right-Rect.left)-iDecal),(int)(RectBnd.top),17,17,
								hWndmHpSave,(HMENU)1,GetHelpComStruct()->hInstServ,0);
	}
}

//-----------------------------------------------------//
// SetEditListViewStyle
//-----------------------------------------------------//
void SetEditListViewStyle(BOOL bEnable)
{	long lStyle=(long)GetWindowLongPtr(hWndmHpSave,GWL_STYLE);
	if(bEnable)	lStyle|=LVS_EDITLABELS;
	else lStyle&=~(LVS_EDITLABELS);
	SetWindowLongPtr(hWndmHpSave,GWL_STYLE,(LONG_PTR)lStyle);
}

//-----------------------------------------------------//
// InsertNewmHpFile
//-----------------------------------------------------//
int InsertNewmHpFile()
{	LVITEM lItm;
	int iResSel=NO,indice=0,iNewIndex=ListView_GetItemCount(hWndmHpSave);
	BOOL bNewExist=FALSE;
	lItm.mask=LVIF_TEXT|LVIF_IMAGE;
	lItm.iItem=iNewIndex;
	lItm.iSubItem=0;
	lItm.iImage=7;
	lItm.pszText=NULL_STRING;
	while((!bNewExist)&&(GetHelpComStruct()->mHpFileList->IsmHpFileExist(indice)))
	{	if(GetHelpComStruct()->mHpFileList->GetNewFile(indice))
		{	iResSel=indice;
			bNewExist=TRUE;
		}
		indice++;
	}
	if(!bNewExist) 
	{	iResSel=ListView_InsertItem(hWndmHpSave,&lItm);
		if(iResSel!=NO)
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_UNDEFINED,lpServRC,255);
			ListView_SetItemText(hWndmHpSave,iNewIndex,1,lpServRC);
			GetHelpComStruct()->mHpFileList->AddmHpFile(NULL_STRING,TRUE);
		}
	}
	return iResSel;
}

//-----------------------------------------------------//
// EnableRadiosRS
//-----------------------------------------------------//
void EnableRadiosRS(int iView)
{	EnableWindow(hWndFolder,FALSE);
	EnableWindow(hWndDoc,FALSE);
	EnableWindow(hWndParam,FALSE);
	EnableWindow(hWndbtnAdd,TRUE);
	switch(iView)
	{	case 0: // Folder
		{	EnableWindow(hWndFolder,TRUE);
			SendMessage(hWndFolder,BM_SETCHECK,BST_CHECKED,0);
			SendMessage(hWndDoc,BM_SETCHECK,BST_UNCHECKED,0);
			SendMessage(hWndParam,BM_SETCHECK,BST_UNCHECKED,0);
			break;
		}
		case 1: // Folder, ParamRS?, Document
		{	EnableWindow(hWndFolder,TRUE);
			EnableWindow(hWndDoc,TRUE);
			if(mHpCtrl_IsCustomEvent(GetHelpComStruct()->mHpCtrlObj->mHpEventList)) EnableWindow(hWndParam,TRUE);
			break;
		}
		default:
		{	EnableWindow(hWndbtnAdd,FALSE);
			break;
		}
	}
}

//-----------------------------------------------------//
// WriteTreeRS
//-----------------------------------------------------//
BOOL WriteTreeRS(HWND hWndActive,char* mHpFileName)
{	char lpRS[MAX_RS_SIZE]={0};
	int iTab[MAX_PART_TREE]={0};
	int iMaxDecalRef,indice=0,iType,iLine=0,iIndex;
	BOOL bError=FALSE,bExist=FALSE;
	DecalTaskRefProc(hWndActive,mHpFileName,iMaxDecalRef);
	if(iMaxDecalRef==NO) return FALSE;
	while(!bExist)
	{	iTab[indice]=ServIndexTab[indice];
		indice++;
		iTab[indice]=NO;
		GetHelpComStruct()->AddList->bExistItm(iTab,iType,bExist);
	}
	while(bExist)
	{	if(!iLine) 
		{	iIndex=mHpPak_GetCountIndex(iTab)-1;
			iLine=mHpPak_GetLineFromTab(hWndActive,mHpFileName,iTab,iMaxDecalRef);
			if(iLine==NO) return FALSE;
		}
		else iLine++;
		if(!mHpPak_InsertInmHpFile(hWndActive,mHpFileName,iLine,FALSE,FALSE)) return FALSE;
		iType=mHpPak_GetCountIndex(iTab)-1;
		if(iType!=NO)
		{	while(iType!=0)
			{	// '|'
				if(!mHpPak_WriteParty(hWndActive,NEXT_RS,NO,NULL,FALSE)) return FALSE;
				iType--;
			}
		}
		else iTab[0]=0;
		GetHelpComStruct()->AddList->GetSetStrRS(iTab,lpRS,MAX_RS_SIZE,TRUE);
		if(lpRS==NULL) return FALSE;
		// RS,ParamRS,Doc || '\n'
		if((!mHpPak_WriteParty(hWndActive,END_FILE,NO,lpRS,TRUE))||(!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE)))
		{	return FALSE;}
		iTab[indice]=ServIndexTab[indice];
		if(iTab[indice]!=NO)
		{	indice++;
			iTab[indice]=NO;
			GetHelpComStruct()->AddList->bExistItm(iTab,iType,bExist);
		}
		else bExist=FALSE;
		if(!mHpPak_ConcatInmHpFile(hWndActive,mHpFileName,iLine,FALSE)) return FALSE;
	}
	if((bRepParam)&&(!mHpCtrl_WriteCustomEvent(hWndActive,hWndListTask,GetHelpComStruct()->mHpCtrlObj->mHpEventList,mHpFileName,mHpPak_GetCountIndex(iTab),iLine))) return FALSE;
	iLine=mHpPak_GetTaskLine(hWndActive,mHpFileName,FALSE);
	if((iLine==NO)||(!mHpPak_InsertInmHpFile(hWndActive,mHpFileName,iLine,FALSE,FALSE))) return FALSE;
	if(iMaxDecalRef) ServIndexTab[iIndex]-=iMaxDecalRef;
	indice=0;
	while(ServIndexTab[indice]!=NO)
	{	if(!indice)
		{	// '#'
			if(!mHpPak_WriteParty(hWndActive,START_TASK,ServIndexTab[indice],NULL,FALSE))
			{	return FALSE;}
		}
		else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,ServIndexTab[indice],NULL,FALSE)) // '¤'
		{	return FALSE;}
		indice++;
	}
	if((bRepParam)&&(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,0,NULL,FALSE))) return FALSE;
	if((!mHpPak_WriteParty(hWndActive,END_RS_TASK,NO,NULL,FALSE))||(!mHpPak_ConcatInmHpFile(hWndActive,mHpFileName,iLine,FALSE))) // '\n'
	{	return FALSE;}
	return TRUE;
}

//-----------------------------------------------------//
// IsParamFolder
//-----------------------------------------------------//
BOOL IsParamFolder(HTREEITEM hTreeItem)
{	TVITEM TvItem;
	if(hTreeItem!=NULL)
	{	TvItem.hItem=hTreeItem;
		TvItem.mask=TVIF_IMAGE;
		TreeView_GetItem(hWndServRS,&TvItem);
		if((TvItem.iImage==2)||(TvItem.iImage==5)) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------//
// DecalTaskRefProc
//-----------------------------------------------------//
void DecalTaskRefProc(HWND hWndActive,char* mHpFileName,int &pMaxDecalRef)
{	HTREEITEM hTreeTmp;
	HTREEITEM hTreeLock;
	int iWhat;
	BOOL bExist,bParam=FALSE;
	mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
	pMaxDecalRef=0;
	if(bRepParam)
	{	hTreeTmp=TreeView_GetPrevSibling(hWndServRS,TreeView_GetSelection(hWndServRS));
		bParam=IsParamFolder(hTreeTmp);
		while((hTreeTmp!=NULL)&&(TreeView_GetChild(hWndServRS,hTreeTmp)==NULL)&&(!bParam))
		{	mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
			mHpPak_AddIndex(ServIndexTab,0);
			GetIndexFromTree(hTreeTmp,hWndServRS,ServIndexTab);
			if(!mHpPak_DecalTaskRef(hWndActive,mHpFileName,ServIndexTab))
			{	pMaxDecalRef=NO;
				return;
			}
			mHpPak_FavsList->ChangeRefFromFile(mHpFileName,ServIndexTab);
			RemoveExistFavRegKey(mHpFileName,ServIndexTab,NULL);
			RemoveExistFavsFile(hWndActive,mHpFileName,ServIndexTab);
			hTreeTmp=TreeView_GetPrevSibling(hWndServRS,hTreeTmp);
			bParam=IsParamFolder(hTreeTmp);
			pMaxDecalRef++;
            //Sleep(1);
		}
	}
	if(!pMaxDecalRef)
	{	hTreeTmp=TreeView_GetParent(hWndServRS,TreeView_GetSelection(hWndServRS));
		mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
		mHpPak_AddIndex(ServIndexTab,0);
		GetIndexFromTree(hTreeTmp,hWndServRS,ServIndexTab);
		GetHelpComStruct()->AddList->bExistItm(ServIndexTab,iWhat,bExist);
		bParam=IsParamFolder(hTreeTmp);
		while((hTreeTmp!=NULL)&&(bExist)&&(!pMaxDecalRef)) // &&(TreeView_GetRoot(hWndServRS)!=hTreeTmp)
		{	hTreeLock=hTreeTmp;
			hTreeTmp=TreeView_GetPrevSibling(hWndServRS,hTreeTmp);
			bParam=IsParamFolder(hTreeTmp);
			while((hTreeTmp!=NULL)&&(TreeView_GetChild(hWndServRS,hTreeTmp)==NULL)&&(!bParam))
			{	mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
				mHpPak_AddIndex(ServIndexTab,0);
				GetIndexFromTree(hTreeTmp,hWndServRS,ServIndexTab);
				if(!mHpPak_DecalTaskRef(hWndActive,mHpFileName,ServIndexTab))
				{	pMaxDecalRef=NO;
					return;
				}
				mHpPak_FavsList->ChangeRefFromFile(mHpFileName,ServIndexTab);
				RemoveExistFavRegKey(mHpFileName,ServIndexTab,NULL);
				RemoveExistFavsFile(hWndActive,mHpFileName,ServIndexTab);
				hTreeTmp=TreeView_GetPrevSibling(hWndServRS,hTreeTmp);
				bParam=IsParamFolder(hTreeTmp);
				pMaxDecalRef++;
			}
			hTreeTmp=TreeView_GetParent(hWndServRS,hTreeLock);
			if((hTreeTmp!=NULL)&&(!pMaxDecalRef))
			{	mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
				mHpPak_AddIndex(ServIndexTab,0);
				GetIndexFromTree(hTreeTmp,hWndServRS,ServIndexTab);
				GetHelpComStruct()->AddList->bExistItm(ServIndexTab,iWhat,bExist);
			}
            //Sleep(1);
		}
	}
	mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
	mHpPak_AddIndex(ServIndexTab,0);
	GetIndexFromTree(TreeView_GetSelection(hWndServRS),hWndServRS,ServIndexTab);
}

//-----------------------------------------------------//
// ShowServCtrl
//-----------------------------------------------------//
void ShowServCtrl(int pWhere)
{	switch(pWhere)
	{	case 0:
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_LIST_OF_CTRL,lpServRC,255);
			SetWindowText(hWndInfo,lpServRC);
			LoadString(GetHelpComStruct()->hInstServ,IDS_NEXT,lpServRC,255);
			SetWindowText(hWndbtnNext,lpServRC);
			// hide
			ShowWindow(hWndServRS,SW_HIDE);
			ShowWindow(hWndFolder,SW_HIDE);
			ShowWindow(hWndDoc,SW_HIDE);
			ShowWindow(hWndParam,SW_HIDE);
			ShowWindow(hWndFrameRS,SW_HIDE);
			ShowWindow(hWndbtnAdd,SW_HIDE);
			ShowWindow(hWndmHpSave,SW_HIDE);
			ShowWindow(hWndNewmHp,SW_HIDE);
			ShowWindow(hWndOpenmHp,SW_HIDE);
			ShowWindow(hWndRemovemHp,SW_HIDE);
			ShowWindow(hWndStaticState,SW_HIDE);
			ShowWindow(hWndServStatus,SW_HIDE);
			ShowWindow(hWndServProgress,SW_HIDE);
			ShowWindow(hWndListParam,SW_HIDE);
			ShowWindow(hWndHideShow,SW_HIDE);
			// show
			ShowWindow(hWndConfigTask,SW_SHOW);
			ShowWindow(hWndListTask,SW_SHOW);
			ShowWindow(hWndMoveUp,SW_SHOW);
			ShowWindow(hWndMoveDown,SW_SHOW);
			break;
		}
		case 1:
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_SELECT_MHPFILE,lpServRC,255);
			SetWindowText(hWndInfo,lpServRC);
			LoadString(GetHelpComStruct()->hInstServ,IDS_NEXT,lpServRC,255);
			SetWindowText(hWndbtnNext,lpServRC);
			// hide
			ShowWindow(hWndConfigTask,SW_HIDE);
			ShowWindow(hWndListTask,SW_HIDE);
			ShowWindow(hWndServRS,SW_HIDE);
			ShowWindow(hWndbtnAdd,SW_HIDE);
			ShowWindow(hWndFolder,SW_HIDE);
			ShowWindow(hWndDoc,SW_HIDE);
			ShowWindow(hWndParam,SW_HIDE);
			ShowWindow(hWndFrameRS,SW_HIDE);
			ShowWindow(hWndMoveUp,SW_HIDE);
			ShowWindow(hWndMoveDown,SW_HIDE);
			ShowWindow(hWndListParam,SW_HIDE);
			ShowWindow(hWndHideShow,SW_HIDE);
			// show
			ShowWindow(hWndmHpSave,SW_SHOW);
			ShowWindow(hWndNewmHp,SW_SHOW);
			ShowWindow(hWndOpenmHp,SW_SHOW);
			ShowWindow(hWndRemovemHp,SW_SHOW);
			ShowWindow(hWndStaticState,SW_SHOW);
			ShowWindow(hWndServStatus,SW_SHOW);
			ShowWindow(hWndServProgress,SW_SHOW);
			break;
		}
		case 2:
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_ADD_CONTENTS,lpServRC,255);
			SetWindowText(hWndInfo,lpServRC);
			// hide
			ShowWindow(hWndConfigTask,SW_HIDE);
			ShowWindow(hWndListTask,SW_HIDE);
			ShowWindow(hWndmHpSave,SW_HIDE);
			ShowWindow(hWndNewmHp,SW_HIDE);
			ShowWindow(hWndOpenmHp,SW_HIDE);
			ShowWindow(hWndRemovemHp,SW_HIDE);
			ShowWindow(hWndStaticState,SW_HIDE);
			ShowWindow(hWndServStatus,SW_HIDE);
			ShowWindow(hWndServProgress,SW_HIDE);
			ShowWindow(hWndMoveUp,SW_HIDE);
			ShowWindow(hWndMoveDown,SW_HIDE);
			ShowWindow(hWndListParam,SW_HIDE);
			ShowWindow(hWndHideShow,SW_HIDE);
			// show
			ShowWindow(hWndServRS,SW_SHOW);
			ShowWindow(hWndFolder,SW_SHOW);
			ShowWindow(hWndDoc,SW_SHOW);
			ShowWindow(hWndParam,SW_SHOW);
			ShowWindow(hWndFrameRS,SW_SHOW);
			ShowWindow(hWndbtnAdd,SW_SHOW);
			break;
		}
		case 3:
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_CFGCUSTOM_INFO,lpServRC,255);
			SetWindowText(hWndInfo,lpServRC);
			// hide
			ShowWindow(hWndListTask,SW_HIDE);
			ShowWindow(hWndmHpSave,SW_HIDE);
			ShowWindow(hWndNewmHp,SW_HIDE);
			ShowWindow(hWndOpenmHp,SW_HIDE);
			ShowWindow(hWndRemovemHp,SW_HIDE);
			ShowWindow(hWndStaticState,SW_HIDE);
			ShowWindow(hWndServStatus,SW_HIDE);
			ShowWindow(hWndServProgress,SW_HIDE);
			ShowWindow(hWndMoveUp,SW_HIDE);
			ShowWindow(hWndMoveDown,SW_HIDE);
			ShowWindow(hWndServRS,SW_HIDE);
			ShowWindow(hWndFolder,SW_HIDE);
			ShowWindow(hWndDoc,SW_HIDE);
			ShowWindow(hWndParam,SW_HIDE);
			ShowWindow(hWndFrameRS,SW_HIDE);
			ShowWindow(hWndbtnAdd,SW_HIDE);
			// show
			ShowWindow(hWndListParam,SW_SHOW);
			ShowWindow(hWndConfigTask,SW_SHOW);
			ShowWindow(hWndHideShow,SW_SHOW);
			break;
		}
		default:
		{	break;}
	}
}

//-----------------------------------------------------//
// CreateServerDlg
//-----------------------------------------------------//
int CreateServerDlg(HWND hWndParam)
{	FARPROC lpServmHpDlg;
	// creation de la boite de dialogue Serveur MHP Help
    lpServmHpDlg=MakeProcInstance((FARPROC)mHpServerProc,GetHelpComStruct()->hInstServ);
	int iResult=(int)DialogBox(GetHelpComStruct()->hInstServ,
                               MAKEINTRESOURCE(IDD_DLG_MHPSERV),
                               hWndParam,
                               (DLGPROC)mHpServerProc);
    FreeProcInstance(lpServmHpDlg);
	// Test la création de la boite de dialogue
	return iResult;
}

//-----------------------------------------------------//
// EnableEditTree
//-----------------------------------------------------//
void EnableEditTree(HWND hWndTree,BOOL bType)
{	int dwStyle;
	dwStyle=(int)GetWindowLongPtr(hWndTree,GWL_STYLE);
	if(bType) dwStyle|=TVS_EDITLABELS;
	else dwStyle&=~(TVS_EDITLABELS);
	SetWindowLongPtr(hWndTree,GWL_STYLE,(LONG_PTR)dwStyle);
}

//-----------------------------------------------------//
// AddParamDetail
//-----------------------------------------------------//
BOOL AddParamDetail(HWND hWndActive,HTREEITEM hParentItm,int iTopic)
{	char StrDescription[MAX_RS_SIZE]={0};
	char Strlimit[MAX_RS_SIZE]={0};
	char ParamRS[MAX_RS_SIZE]={0};
	char lpmHpFilePath[MAX_RS_SIZE]={0};
	char ParamDetail[MAX_RS_LISTE]={0};
	int type;
	BOOL bContinue=TRUE;		
	while(bContinue)
	{	mHpPak_GetDetailParamRS(hWndActive,GetmHpFileName(hWndmHpSave,iTopic,lpmHpFilePath,MAX_RS_SIZE),ParamRS,MAX_RS_SIZE,
								type,Strlimit,MAX_RS_SIZE,StrDescription,MAX_RS_SIZE,bContinue);
		if(type!=NO)
		{	mHpPak_InitStr(ParamDetail,MAX_RS_LISTE);
			strncpy(ParamDetail,ParamRS,MAX_RS_LISTE);
			strncat(ParamDetail," {",2);
			LoadString(GetHelpComStruct()->hInstServ,RSParamType[type],lpServRC,255);
			strncat(ParamDetail,lpServRC,mHpPak_FindStrLenght(lpServRC,255));
			strncat(ParamDetail,"}{",2);
			strncat(ParamDetail,StrDescription,mHpPak_FindStrLenght(StrDescription,MAX_RS_SIZE));
			strncat(ParamDetail,"}{",2);
			strncat(ParamDetail,Strlimit,mHpPak_FindStrLenght(Strlimit,MAX_RS_SIZE));
			strncat(ParamDetail,"}",1);
			AddItem(hWndServRS,hParentItm,3,3,ParamDetail,0);
		}
		else return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------//
// CreateServTree
//-----------------------------------------------------//
BOOL CreateServTree(HWND hWndActive,int iTopic)
{	HTREEITEM hParent;
	HTREEITEM hHideParent;
	char lpTopic[MAX_RS_SIZE]={0};
	char lpStrRS[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpHidden[MAX_RS_SIZE]={0};
	char lpmHpFilePath[MAX_RS_SIZE]={0};
	int iParam,iParamLast,iTmpPos,iCurrentPos,iLineCnt;
	GetmHpFileName(hWndmHpSave,iTopic,lpmHpFilePath,MAX_RS_SIZE);
	iLineCnt=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFilePath);
	if(iLineCnt==NO) return FALSE;
	mHpPak_GetTopicmHpFile(hWndActive,lpmHpFilePath,lpTopic,MAX_RS_SIZE);
	TreeView_DeleteAllItems(hWndServRS);
	LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_TOPIC,lpTmp,MAX_RS_SIZE);
	strncat(lpTmp," '",2);
	strncat(lpTmp,lpTopic,mHpPak_FindStrLenght(lpTopic,MAX_RS_SIZE));
	strncat(lpTmp,"'",1);
	SetWindowText(hWndServStatus,lpTmp);
	hParent=AddItem(hWndServRS,TVGN_ROOT,7,8,lpTopic,0);
    hHideParent=AddItem(hWndHideServTree,TVGN_ROOT,0,0,lpTopic,0);
	if(iLineCnt!=1)
	{	for(int i=0;i<iLineCnt;i++)
		{	ProceedMsgQueue(hWndActive);//,1);
			if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+1),FALSE)) return FALSE;
			mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpStrRS,MAX_RS_SIZE,iCurrentPos,iParamLast);
			if(iCurrentPos==NO) return FALSE;
			if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+2),FALSE)) return FALSE;
			mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpTmp,MAX_RS_SIZE,iTmpPos,iParam);
			if(iTmpPos==NO) return FALSE;
			if(!iParam) // Single
			{	if(iTmpPos>iCurrentPos)
                {
#ifdef MHELP_OLD
                    hParent=AddItem(hWndServRS,hParent,0,4,lpStrRS,0);
#else
                    SplitHtmlLinkRS(lpStrRS,MAX_RS_SIZE,lpmHpFilePath,lpHidden,MAX_RS_SIZE);
                    hParent=AddItem(hWndServRS,hParent,0,4,lpStrRS,0);
                    hHideParent=AddItem(hWndHideServTree,hHideParent,0,0,lpHidden,0);
#endif
                }
				else
				{	if(!iParamLast)
                    {
#ifdef MHELP_OLD
                        AddItem(hWndServRS,hParent,1,1,lpStrRS,0);
#else
                        SplitHtmlLinkRS(lpStrRS,MAX_RS_SIZE,lpmHpFilePath,lpHidden,MAX_RS_SIZE);
                        AddItem(hWndServRS,hParent,1,1,lpStrRS,0);
                        AddItem(hWndHideServTree,hHideParent,0,0,lpHidden,0);
#endif
                    }
					else if(iParamLast==1)
					{	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+1),FALSE)) return FALSE;
						mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpStrRS,MAX_RS_SIZE,iCurrentPos,iParamLast);
						if(iCurrentPos==NO) return FALSE;
						if(!AddParamDetail(hWndActive,hParent,iTopic)) return FALSE;
					}
					while(iTmpPos!=iCurrentPos)
					{	hParent=TreeView_GetParent(hWndServRS,hParent);
#ifndef MHELP_OLD
                        hHideParent=TreeView_GetParent(hWndHideServTree,hHideParent);
#endif
						iTmpPos++;
					}
				}
			}
			else if(iParam==1) hParent=AddItem(hWndServRS,hParent,2,5,lpStrRS,0); // Param
			else
			{	if(!iParamLast)
                {
#ifdef MHELP_OLD
                    AddItem(hWndServRS,hParent,1,1,lpStrRS,0);
#else
                    SplitHtmlLinkRS(lpStrRS,MAX_RS_SIZE,lpmHpFilePath,lpHidden,MAX_RS_SIZE);
                    AddItem(hWndServRS,hParent,1,1,lpStrRS,0);
                    AddItem(hWndHideServTree,hHideParent,0,0,lpHidden,0);
#endif
                }
				else if(iParamLast==1)
				{	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+1),FALSE)) return FALSE;
					mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpStrRS,MAX_RS_SIZE,iCurrentPos,iParamLast);
					if(iCurrentPos==NO) return FALSE;
					if(!AddParamDetail(hWndActive,hParent,iTopic)) return FALSE;
				}
			}
			mHpPak_IncreaseProgressBar(hWndServProgress,iLineCnt);
		}
	}
	mHpPak_ClosemHpFile();
	TreeView_Expand(hWndServRS,TreeView_GetRoot(hWndServRS),TVE_EXPAND);
	TreeView_SelectItem(hWndServRS,TreeView_GetRoot(hWndServRS));
	return TRUE;
}

//-----------------------------------------------------//
// mHpFileProc
//-----------------------------------------------------//
LRESULT CALLBACK mHpFileProc(HWND hWndmHp,UINT mHpMessage,WPARAM mHpWParam,LPARAM mHpLParam)
{	switch(mHpMessage)
	{	case WM_COMMAND:
		{	switch(HIWORD(mHpWParam))
			{	case BN_CLICKED:
				{	char lpNewmHpFile[MAX_RS_SIZE]={0};
					char lpTopicFile[MAX_RS_SIZE]={0};
					int iItmSel=ListView_GetNextItem(hWndmHpSave,NO,LVNI_SELECTED);
					if(((HWND)mHpLParam)==hWndSaveNew)
					{	if(mHpPak_GetNewmHpFile(hWndmHp,GetHelpComStruct()->hInstServ,IDS_NEW_FILTER,lpNewmHpFile,MAX_RS_SIZE))
						{	mHpPak_AddmHpExtension(lpNewmHpFile,MAX_RS_SIZE);
							GetNewTopic(lpTopicFile,MAX_RS_SIZE,iItmSel);
							switch(mHpPak_CreatemHpFile(hWndmHp,lpNewmHpFile,lpTopicFile))
							{	case 0: // Error
								{	break;
								}
								case 1: // Created
								{	int iSelect=IsItAlreadyOpened(hWndmHpSave,lpNewmHpFile,MAX_RS_SIZE);
									GetHelpComStruct()->mHpFileList->RemovemHpFile(iItmSel);
									if(iSelect==NO)
									{	GetHelpComStruct()->mHpFileList->AddmHpFile(lpNewmHpFile,FALSE);
										ListView_DeleteAllItems(hWndmHpSave);
										ReplacemHpLst(hWndmHp,hWndmHpSave);
										iSelect=ListView_GetItemCount(hWndmHpSave)-1;
									}
									else
									{	ListView_DeleteItem(hWndmHpSave,iItmSel);
										if(iSelect>iItmSel) iSelect--;
										ListView_SetItemText(hWndmHpSave,iSelect,0,lpTopicFile);
									}
									ListView_SetItemState(hWndmHpSave,iSelect,LVIS_SELECTED,LVIS_SELECTED);
									mHpPak_FavsList->RemoveFromFile(lpNewmHpFile);
									RemoveExistFavRegKey(lpNewmHpFile,NULL,NULL);
									break;
								}
								case 2: // Already exist - Open it
								{	int iSelected=IsItAlreadyOpened(hWndmHpSave,lpNewmHpFile,MAX_RS_SIZE);
									if(iSelected==NO)
									{	AddItemmHp(hWndmHp,hWndmHpSave,lpNewmHpFile,ListView_GetItemCount(hWndmHpSave));
										GetHelpComStruct()->mHpFileList->AddmHpFile(lpNewmHpFile,FALSE);
									}
									else ListView_SetItemState(hWndmHpSave,iSelected,LVIS_SELECTED,LVIS_SELECTED);
									mHpPak_FavsList->RemoveFromFile(lpNewmHpFile);
									RemoveExistFavRegKey(lpNewmHpFile,NULL,NULL);
									break;
								}
							}
						}
					}
					break;
				}
			}
			break;
		}
		case WM_HSCROLL:
		case WM_VSCROLL:
		{	if(hWndSaveNew!=NULL)
			{	DestroyWindow(hWndSaveNew);
                hWndSaveNew=NULL;
				CreateSaveNewBtn(ListView_GetItemSel(hWndmHpSave));
			}
			break;
		}
		case WM_KEYUP:
		{	switch(mHpWParam)
			{	case VK_F1:
				{	mHpPak_OpenmHelpDoc(hWndmHp,MHPPAK_DOC_SERV+iStep);
					break;
				}
				default:
				{	break;}
			}
			break;
		}
	}
	return CallWindowProc((WNDPROC)lmHpPrevProc,hWndmHp,mHpMessage,mHpWParam,mHpLParam);
}

//-----------------------------------------------------//
// mHpServerProc()
//
//-----------------------------------------------------//
LRESULT CALLBACK mHpServerProc(HWND hWndServ,UINT DlgMessage,WPARAM wParam,LPARAM lParam)
{	static int iWhere=0;
	static BOOL bPass=FALSE;
	switch(DlgMessage)
	{	case WM_INITDIALOG:
		{	HTREEITEM hItemA;
			LV_COLUMN lvcolumn;
			RECT NextRect;
			RECT ServRect;
			if(!bPass)
			{	ServDlgDocLst=new CWndDocList();
				bPass=TRUE;
			}
			iWhere=0;
			bRepParam=FALSE;
			iSelCtrl=NO;
			iSelParam=NO;
			iStep=NO;
			bBugClose=FALSE;
            bTaskSaveFlag=FALSE;
			mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
			if(!GetHelpComStruct()->ServDlgCreatedObj)
			{	GetHelpComStruct()->AddList=new CItmList();
				GetHelpComStruct()->ServDlgCreatedObj=TRUE;
			}
			hWndServRS=GetDlgItem(hWndServ,IDC_VIEW_RSTREE);
			hWndbtnAdd=GetDlgItem(hWndServ,IDC_BTN_ADD);
			hWndbtnNext=GetDlgItem(hWndServ,IDC_NEXT);
			hWndbtnLast=GetDlgItem(hWndServ,IDC_LAST);
			hWndConfigTask=GetDlgItem(hWndServ,IDC_BTN_CONFIG);
			hWndListTask=GetDlgItem(hWndServ,IDC_SERV_LIST_TASK);
			hWndInfo=GetDlgItem(hWndServ,IDC_FRAME_WHAT);
			hWndmHpSave=GetDlgItem(hWndServ,IDC_LIST_MHPSAVE);
			hWndNewmHp=GetDlgItem(hWndServ,IDC_NEW_MHPSAVE);
			hWndOpenmHp=GetDlgItem(hWndServ,IDC_OPEN_MHPSAVE);
			hWndRemovemHp=GetDlgItem(hWndServ,IDC_REMOVE_MHPSAVE);
			hWndStaticState=GetDlgItem(hWndServ,IDC_STATIC_STATE);
			hWndServStatus=GetDlgItem(hWndServ,IDC_CURRENT_STATUS);
			hWndServProgress=GetDlgItem(hWndServ,IDC_PROGRESS_MHPSAVE);
			hWndFolder=GetDlgItem(hWndServ,IDC_RADIO_FOLDER);
			hWndDoc=GetDlgItem(hWndServ,IDC_RADIO_DOC);
			hWndParam=GetDlgItem(hWndServ,IDC_RADIO_PARAM);
			hWndFrameRS=GetDlgItem(hWndServ,IDC_FRAME_TREEITEM);
			hWndMoveUp=GetDlgItem(hWndServ,IDC_BTN_MOVEUP);
			hWndMoveDown=GetDlgItem(hWndServ,IDC_BTN_MOVEDOWN);
			hWndListParam=GetDlgItem(hWndServ,IDC_PARAM_LIST_TASK);
			hWndHideShow=GetDlgItem(hWndServ,IDC_BTN_HIDESHOW);
			hWndHelpDoc=GetDlgItem(hWndServ,IDC_DOCUMENTATION);
			hWndTerminate=GetDlgItem(hWndServ,IDCANCEL);
            hWndHideServTree=GetDlgItem(hWndServ,IDC_HIDDEN_SERVTREE);
			hWndSaveNew=NULL;
			ShowWindow(hWndbtnLast,SW_SHOWNORMAL);
			if(GetHelpComStruct()->bNoApp) EnableWindow(hWndbtnLast,FALSE);
			EnableRadiosRS(1);
			if(hServImgList!=NULL) ImageList_Destroy(hServImgList);
			hServImgList=ImageList_Create(16,16,ILC_MASK|ILC_COLOR16,6,1);
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_RSREP)));
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_TASK)));
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_PARAM_REP)));
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_PARAM)));
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_REP)));
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_PARAM)));
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_RADIO_PARAM)));
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_BOOK)));
			ImageList_AddIcon(hServImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_BOOK)));
			TreeView_SetImageList(hWndServRS,hServImgList,TVSIL_NORMAL);
			ListView_SetImageList(hWndmHpSave,hServImgList,LVSIL_SMALL);
			mHpCtrl_SetCtrlListHeader(hWndListTask,hCtrlImgList);
			mHpCtrl_SetCustomListHeader(hWndListParam,hParamImgList);
			lvcolumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
			lvcolumn.fmt=LVCFMT_LEFT;
			lvcolumn.cchTextMax=255;
			lvcolumn.iSubItem=0;
			lvcolumn.cx=110;
			LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_TOPIC,lpServRC,255);
			lvcolumn.pszText=lpServRC;
			ListView_InsertColumn(hWndmHpSave,0,&lvcolumn);
			LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_FILEPATH,lpServRC,255);
			lvcolumn.pszText=lpServRC;
			lvcolumn.iSubItem=1;
			lvcolumn.cx=256;
			ListView_InsertColumn(hWndmHpSave,1,&lvcolumn);
			hItemA=TreeView_GetNextItem(hWndServRS,NULL,TVGN_ROOT);
			TreeView_Expand(hWndServRS,hItemA,TVE_EXPAND);
			lmHpPrevProc=SetWindowLongPtr(hWndmHpSave,GWLP_WNDPROC,(LONG_PTR)mHpFileProc);
			ReplacemHpLst(hWndServ,hWndmHpSave);
			SetWindowText(hWndServStatus,NULL_STRING);
			GetWindowRect(hWndServ,&ServRect);
			SendMessage(hWndFolder,BM_SETCHECK,BST_CHECKED,0);
			MoveWindow(hWndServ,ServRect.left,ServRect.top,451,366,TRUE);
			MoveWindow(hWndbtnNext,271,314,75,23,TRUE);
			MoveWindow(hWndHelpDoc,6,314,75,23,TRUE);
			MoveWindow(hWndbtnLast,186,314,75,23,TRUE);
			MoveWindow(hWndTerminate,365,314,75,23,TRUE);
			MoveWindow(hWndListTask,60,23,370,233,TRUE);
			MoveWindow(hWndConfigTask,60,261,87,24,TRUE);
			MoveWindow(hWndInfo,51,5,389,288,TRUE);
			MoveWindow(GetDlgItem(hWndServ,IDC_STATIC_MHPICON),5,11,38,38,TRUE);
			MoveWindow(GetDlgItem(hWndServ,IDC_STATIC_SEPHOR),5,293,436,15,TRUE);
			MoveWindow(hWndmHpSave,60,23,370,190,TRUE);
			MoveWindow(hWndNewmHp,60,218,75,23,TRUE);
			MoveWindow(hWndOpenmHp,143,218,75,23,TRUE);
			MoveWindow(hWndRemovemHp,226,218,75,23,TRUE);
			MoveWindow(hWndStaticState,60,244,40,15,TRUE);
			MoveWindow(hWndServStatus,100,244,280,15,TRUE);
			MoveWindow(hWndServProgress,60,262,370,22,TRUE);
			MoveWindow(hWndServRS,60,23,261,262,TRUE);
			MoveWindow(hWndbtnAdd,328,130,103,21,TRUE);
			MoveWindow(hWndFrameRS,328,17,103,106,TRUE);
			MoveWindow(hWndFolder,336,35,80,16,TRUE);
			MoveWindow(hWndDoc,336,55,80,16,TRUE);
			MoveWindow(hWndParam,336,75,80,16,TRUE);
			MoveWindow(hWndMoveUp,160,261,87,24,TRUE);
			MoveWindow(hWndMoveDown,260,261,87,24,TRUE);
			MoveWindow(hWndListParam,60,23,370,233,TRUE);
			MoveWindow(hWndHideShow,160,261,87,24,TRUE);
			SetWindowText(hWndServ,MHPPAK_DLGTITLE);
			// BUG: les fenêtres XP et 7 sont trop courte //
			GetWindowRect(hWndbtnNext,&NextRect);
			GetWindowRect(hWndServ,&ServRect);
			if(ServRect.bottom<=NextRect.bottom)
                MoveWindow(hWndServ,ServRect.left,ServRect.top,451,373,TRUE);
            else
            {   DWORD winVersion=GetVersion();
                DWORD VersMajor=(DWORD)(LOBYTE(LOWORD(winVersion)));
                DWORD VersMinor=(DWORD)(HIBYTE(LOWORD(winVersion)));
                // Check Windows 7 or later
                if((VersMajor>6)||((VersMajor==6)&&(VersMinor>=1)))
                    MoveWindow(hWndServ,ServRect.left,ServRect.top,451,371,TRUE);
            }
			//
			mHpCtrl_FillCtrlList(hWndListTask,GetHelpComStruct()->mHpCtrlObj->mHpEventList);
			SetServDlgDocLnch();
			return TRUE;
		}
		case WM_MOUSEMOVE:
		{	SetmHpCursor(hWndServ,NO);
			/*{	SetClassLongPtr(hWndServ,GCL_HCURSOR,(LONG_PTR)hWaitCursor);
				SetCursor(hWaitCursor);
			}*/
			break;
		}
		case WM_NOTIFY:
		{	if(((LPNMHDR)lParam)->idFrom==IDC_VIEW_RSTREE)
			{	switch(((LPNMHDR)lParam)->code)
				{	case TVN_SELCHANGED:
					{	HTREEITEM hTree=TreeView_GetSelection(hWndServRS);
#ifdef MHELP_OLD
						BOOL bExisT;
						int iWhat;
						if(hTree!=NULL)
						{	if(TreeView_GetParent(hWndServRS,hTree)==NULL)
							{	EnableEditTree(hWndServRS,FALSE);}
							else EnableEditTree(hWndServRS,TRUE);
							EnableRadiosRS(NO);
							mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
							mHpPak_AddIndex(ServIndexTab,0);
							GetIndexFromTree(hTree,hWndServRS,ServIndexTab);
							GetHelpComStruct()->AddList->bExistItm(ServIndexTab,iWhat,bExisT);
							if(bExisT)
							{	EnableRadiosRS(1);
								if(iWhat)
								{	if(iWhat==1)
									{	LoadString(GetHelpComStruct()->hInstServ,IDS_SAVE,lpServRC,255);}
									else LoadString(GetHelpComStruct()->hInstServ,IDS_NEXT,lpServRC,255);
									SetWindowText(hWndbtnNext,lpServRC);
									EnableWindow(hWndbtnNext,TRUE);
									EnableRadiosRS(NO);
								}
								else
								{	EnableWindow(hWndbtnNext,FALSE);
									SetFocus(hWndTerminate);
								}
							}
							else
							{	EnableWindow(hWndbtnNext,FALSE);
								SetFocus(hWndTerminate);
								if((TreeView_GetParent(hWndServRS,hTree)==NULL)||
								   (TreeView_GetChild(hWndServRS,hTree)!=NULL))
								{	if(TreeView_GetParent(hWndServRS,hTree)==NULL) EnableRadiosRS(0);
									else
									{	if(!IsParamFolder(hTree)) EnableRadiosRS(1);
									}
								}
							}
						}
#else
                        EnableRadiosRS(NO);
						if((hTree!=NULL)&&(hTree!=TreeView_GetRoot(hWndServRS)))
                        {   EnableEditTree(hWndServRS,FALSE);
                            LoadString(GetHelpComStruct()->hInstServ,IDS_SAVE,lpServRC,255);
                            SetWindowText(hWndbtnNext,lpServRC);
                            EnableWindow(hWndbtnNext,TRUE);
                        }
                        else
                            EnableWindow(hWndbtnNext,FALSE);
#endif
						break;
					}
					case TVN_ITEMEXPANDED:
					{	TV_ITEM TvItem=((NM_TREEVIEW FAR *)lParam)->itemNew;
						TvItem.mask=TVIF_IMAGE;
						if(TvItem.hItem==TreeView_GetRoot(hWndServRS))
						{	if(((NM_TREEVIEW FAR *)lParam)->action==1) TvItem.iImage=7;
							else TvItem.iImage=8;
						}
						else if(!IsParamFolder(TvItem.hItem))
						{	if(((NM_TREEVIEW FAR *)lParam)->action==1) TvItem.iImage=0;
							else TvItem.iImage=4;
						}
						else
						{	if(((NM_TREEVIEW FAR *)lParam)->action==1) TvItem.iImage=2;
							else TvItem.iImage=5;
						}
						TreeView_SetItem(hWndServRS,&TvItem);
						break;
					}
					case TVN_BEGINLABELEDIT:
					{	HWND hWndEditTree=TreeView_GetEditControl(hWndServRS);
						ServDlgDocLst->AddDocWnd(hWndEditTree,(LONG_PTR)ServDlgDocProc);
						break;
					}
					case TVN_ENDLABELEDIT:
					{	if(((TV_DISPINFO FAR *)lParam)->item.mask==TVIF_TEXT)
						{	TV_ITEM TvItem=((TV_DISPINFO FAR *)lParam)->item;
							GetHelpComStruct()->AddList->GetSetStrRS(ServIndexTab,TvItem.pszText,TvItem.cchTextMax,FALSE);
							TreeView_SetItem(hWndServRS,&TvItem);
						}
						break;
					}
				}
			}
			else if(((LPNMHDR)lParam)->idFrom==IDC_SERV_LIST_TASK)
			{	switch(((LPNMHDR)lParam)->code)
				{	case LVN_ITEMCHANGED:
					{	iSelCtrl=ListView_GetNextItem(hWndListTask,NO,LVNI_SELECTED);
						if(iSelCtrl!=NO)
						{	EnableWindow(hWndConfigTask,TRUE);
							mHpCtrl_EnableCfgTools(GetHelpComStruct()->mHpCtrlObj->mHpEventList,iSelCtrl,hWndMoveUp,hWndMoveDown);
						}
						else
						{	EnableWindow(hWndConfigTask,FALSE);
							EnableWindow(hWndMoveUp,FALSE);
							EnableWindow(hWndMoveDown,FALSE);
						}
						break;
					}
					case NM_DBLCLK:
					{	if(iSelCtrl!=NO)
						{	SendMessage(hWndConfigTask,BM_CLICK,0,0);}
						break;
					}
				}
			}
			else if(((LPNMHDR)lParam)->idFrom==IDC_LIST_MHPSAVE)
			{	switch(((LPNMHDR)lParam)->code)
				{	case LVN_ITEMCHANGED:
					{	SetmHpSaveCtrls();
						break;
					}
					case LVN_ENDLABELEDIT:
					{	if(((LV_DISPINFO FAR*)lParam)->item.mask==LVIF_TEXT)
						{	LV_ITEM LvItem=((LV_DISPINFO FAR*)lParam)->item;
							/** BUG: It launch the IDCANCEL WM_COMMAND? ***********/
							bBugClose=TRUE;
							ListView_SetItem(hWndmHpSave,&LvItem);
							bBugClose=FALSE;
							/******************************************************/
						}
						break;
					}
					case LVN_BEGINLABELEDIT:
					{	HWND hWndEditTopic=ListView_GetEditControl(hWndmHpSave);
						ServDlgDocLst->AddDocWnd(hWndEditTopic,(LONG_PTR)ServDlgDocProc);
						break;
					}
					case NM_DBLCLK:
					{	int iSel=ListView_GetNextItem(hWndmHpSave,NO,LVNI_SELECTED);
						if(iSel!=NO)
						{	if(!GetHelpComStruct()->mHpFileList->GetNewFile(iSel)) SendMessage(hWndbtnNext,BM_CLICK,0,0);
							else ListView_EditLabel(hWndmHpSave,iSel);
						}
						break;
					}
				}
			}
			else if(((LPNMHDR)lParam)->idFrom==IDC_PARAM_LIST_TASK)
			{	switch(((LPNMHDR)lParam)->code)
				{	case LVN_ITEMCHANGED:
					{	iSelParam=ListView_GetNextItem(hWndListParam,NO,LVNI_SELECTED);
						if(iSelParam!=NO)
						{	EnableWindow(hWndConfigTask,TRUE);
							EnableWindow(hWndHideShow,TRUE);
							if(mHpCtrl_IsCustomVisible(GetHelpComStruct()->mHpCtrlObj->mHpEventList,iSelParam))
							{	LoadString(GetHelpComStruct()->hInstServ,IDS_HIDE,lpServRC,255);
								SetWindowText(hWndHideShow,lpServRC);
								EnableWindow(hWndHideShow,IsCtrlVisible());
							}
							else
							{	LoadString(GetHelpComStruct()->hInstServ,IDS_SHOW,lpServRC,255);
								SetWindowText(hWndHideShow,lpServRC);
							}
						}
						else
						{	EnableWindow(hWndConfigTask,FALSE);
							EnableWindow(hWndHideShow,FALSE);
						}
						break;
					}
					case NM_DBLCLK:
					{	if(iSelParam!=NO)
						{	SendMessage(hWndConfigTask,BM_CLICK,0,0);}
						break;
					}
				}
			}
			else return DefWindowProc(hWndServ,DlgMessage,wParam,lParam);
			break;
		}
		case WM_COMMAND:
		{	switch(LOWORD(wParam))
			{	case IDC_BTN_ADD:
				{	int what,EraseWhat;
					BOOL temp=FALSE;
					HTREEITEM hTree1=TreeView_GetSelection(hWndServRS);
 					if(hTree1!=NULL)
					{	HTREEITEM hTree2;
						TV_ITEM tvitem;
						int iTemp=0;
						if(SendMessage(hWndDoc,BM_GETCHECK,0,0)==BST_CHECKED) iTemp=1;
						else if(SendMessage(hWndParam,BM_GETCHECK,0,0)==BST_CHECKED) iTemp=2;
						EnableRadiosRS(1);
						mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
						mHpPak_AddIndex(ServIndexTab,0);
						hTree2=TreeView_GetChild(hWndServRS,hTree1);
						if(hTree2!=NULL)
						{	while(TreeView_GetNextSibling(hWndServRS,hTree2)!=NULL)
							{	hTree2=TreeView_GetNextSibling(hWndServRS,hTree2);}
							GetIndexFromTree(hTree2,hWndServRS,ServIndexTab);
							GetHelpComStruct()->AddList->bExistItm(ServIndexTab,EraseWhat,temp);
						}
						if(temp)
						{	mHpPak_AddIndex(ServIndexTab,0);
							GetHelpComStruct()->AddList->bExistItm(ServIndexTab,what,temp);
							if((!temp)||(mHpPak_mHpMessage(hWndServ,GetHelpComStruct()->hInstServ,IDS_QUERY_SERVTREE,NULL,0,0,MB_ICONEXCLAMATION|MB_YESNO)==IDYES))
							{	if(temp)
								{	TreeView_DeleteItem(hWndServRS,TreeView_GetChild(hWndServRS,hTree2));
									while(GetHelpComStruct()->AddList->RemoveItm(ServIndexTab))
									{	mHpPak_AddIndex(ServIndexTab,0);}
								}
								if(EraseWhat!=iTemp)
								{	mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
									mHpPak_AddIndex(ServIndexTab,0);
									GetIndexFromTree(hTree2,hWndServRS,ServIndexTab);
									GetHelpComStruct()->AddList->ChangeType(ServIndexTab,iTemp);
									tvitem.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE;
									tvitem.hItem=hTree2;
									switch(iTemp)
									{	case 0:
										{	tvitem.iImage=0;
											tvitem.iSelectedImage=4;
											break;
										}
										case 1:
										{	tvitem.iImage=1;
											tvitem.iSelectedImage=1;
											break;
										}
										case 2:
										{	tvitem.iImage=2;
											tvitem.iSelectedImage=5;
											break;
										}
									}
									TreeView_SetItem(hWndServRS,&tvitem);		
								}
							}
							else break;
						}
						else
						{	mHpPak_EmptyTab(ServIndexTab,MAX_PART_TREE);
							mHpPak_AddIndex(ServIndexTab,0);
							switch(iTemp)
							{	case 0: 
									hTree2=AddItem(hWndServRS,hTree1,0,4," ",0);
									GetIndexFromTree(hTree2,hWndServRS,ServIndexTab);
									GetHelpComStruct()->AddList->AddItm(iTemp,ServIndexTab);
									break;
								case 1: 
									hTree2=AddItem(hWndServRS,hTree1,1,1," ",0);
									GetIndexFromTree(hTree2,hWndServRS,ServIndexTab);
									GetHelpComStruct()->AddList->AddItm(iTemp,ServIndexTab);
									break;
								case 2: 
									hTree2=AddItem(hWndServRS,hTree1,2,5," ",0);
									GetIndexFromTree(hTree2,hWndServRS,ServIndexTab);
									GetHelpComStruct()->AddList->AddItm(iTemp,ServIndexTab);
									break;
							}
						}
						TreeView_SelectItem(hWndServRS,hTree2);
						EnableEditTree(hWndServRS,TRUE);
						TreeView_EditLabel(hWndServRS,hTree2);
					}
					break;
				}
				case IDCANCEL: 
				{	/** BUG: It launch the IDCANCEL WM_COMMAND? ***********/
					if(!bBugClose)
					{	int nMsgRes=IDYES;
                        if(!bTaskSaveFlag)
                        {   LoadString(GetHelpComStruct()->hInstServ,IDS_SRVMSG_TERMINATE,lpServRC,255);
                            nMsgRes=MessageBox(hWndServ,lpServRC,MHPPAK_DLGMSGTITLE,MB_ICONQUESTION|MB_YESNO);
                        }
						if(nMsgRes==IDYES)
						{	GetHelpComStruct()->AddList->EmptyItmList();
							SetmHpListFromCtrl(hWndmHpSave,FALSE);
							mHpPak_ClearmHpSharedKey();
							return EndDialog(hWndServ,IDCANCEL);
						}
					}
					break;
				}
				case IDC_NEXT: 
				{	BOOL bEmptyFile;
					iStep++;
					switch(iWhere)
					{	case 0: // mHpFiles
						{	SetmHpSaveCtrls();
							EnableWindow(hWndbtnLast,TRUE);
							break;
						}
						case 1: // Add content
						{	SetmHpCursor(hWndServ,1);
							CreateServTree(hWndServ,ListView_GetItemSel(hWndmHpSave));
							SetmHpCursor(hWndServ,0);
							SendMessage(hWndServProgress,PBM_SETPOS,0,0);	
							SetWindowText(hWndServStatus,NULL_STRING);
							EnableWindow(hWndbtnNext,FALSE);
							SetFocus(hWndTerminate);
							EnableWindow(hWndbtnLast,TRUE);
							break;
						}
						case 2: // ParamList
						{
#ifdef MHELP_OLD
                            int iwhat;
                            BOOL btemp;
                            GetHelpComStruct()->AddList->bExistItm(ServIndexTab,iwhat,btemp);
							if(iwhat==1)
							{	bRepParam=FALSE;
                                char lpmHpFilePath[MAX_RS_SIZE]={0};
								GetmHpFileName(hWndmHpSave,ListView_GetItemSel(hWndmHpSave),lpmHpFilePath,MAX_RS_SIZE);
								if(mHpPak_GetTreeLineCnt(hWndServ,lpmHpFilePath)==1)
								{	bEmptyFile=TRUE;}
								else bEmptyFile=FALSE;
								//
								WritemHelpTask(hWndServ,lpmHpFilePath,bEmptyFile);
								//								
								iWhere=NO;
								iStep=NO;
								if(GetHelpComStruct()->bNoApp)
								{	EnableWindow(hWndbtnLast,FALSE);
									SetFocus(hWndbtnNext);
								}
							}
							else
							{	bRepParam=TRUE;
								LoadString(GetHelpComStruct()->hInstServ,IDS_SAVE,lpServRC,255);
								SetWindowText(hWndbtnNext,lpServRC);
								mHpCtrl_FillCustomList(hWndListParam,GetHelpComStruct()->mHpCtrlObj->mHpEventList);
							}
							if(GetHelpComStruct()->bNoApp)
							{	EnableWindow(hWndbtnLast,FALSE);
								SetFocus(hWndbtnNext);
							}
							else EnableWindow(hWndbtnLast,TRUE);
#else
                            char lpmHpFilePath[MAX_RS_SIZE]={0};
                            GetmHpFileName(hWndmHpSave,ListView_GetItemSel(hWndmHpSave),lpmHpFilePath,MAX_RS_SIZE);
							bEmptyFile=FALSE;
							//
							WritemHelpTask(hWndServ,lpmHpFilePath,bEmptyFile);
							//
							iWhere=NO;
							iStep=NO;
							if(GetHelpComStruct()->bNoApp)
							{	EnableWindow(hWndbtnLast,FALSE);
								SetFocus(hWndbtnNext);
							}
#endif
							break;
						}
						case 3:
						{   char lpmHpFilePath[MAX_RS_SIZE]={0};
                            GetmHpFileName(hWndmHpSave,ListView_GetItemSel(hWndmHpSave),lpmHpFilePath,MAX_RS_SIZE);
							if(mHpPak_GetTreeLineCnt(hWndServ,lpmHpFilePath)==1)
							{	bEmptyFile=TRUE;}
							else bEmptyFile=FALSE;
							//
							WritemHelpTask(hWndServ,lpmHpFilePath,bEmptyFile);
							//
							iWhere=NO;
							iStep=NO;
							if(GetHelpComStruct()->bNoApp)
							{	EnableWindow(hWndbtnLast,FALSE);
								SetFocus(hWndbtnNext);
							}
							break;
						}
					}
					iWhere++;
					ShowServCtrl(iWhere);
					break;
				}
				case IDC_LAST: 
				{	iWhere--;
					iStep--;
					LoadString(GetHelpComStruct()->hInstServ,IDS_NEXT,lpServRC,255);
					SetWindowText(hWndbtnNext,lpServRC);
					if(iWhere==1)
					{	if(!GetHelpComStruct()->AddList->IsEmpty())
						{	if(mHpPak_mHpMessage(hWndServ,GetHelpComStruct()->hInstServ,IDS_WARNING_TREE,NULL,0,0,MB_ICONWARNING|MB_YESNO)!=IDNO)
							{	EnableWindow(hWndbtnNext,TRUE);
								GetHelpComStruct()->AddList->EmptyItmList();
							}
							else
							{	iWhere=2;
								iStep=2;
								return FALSE;
							}
						}
						mHpCtrl_FillCtrlList(hWndListTask,GetHelpComStruct()->mHpCtrlObj->mHpEventList);
						EnableWindow(hWndbtnNext,TRUE);						
					}
					else if(!iWhere)
					{	EnableWindow(hWndbtnNext,TRUE);
						if(GetHelpComStruct()->bNoApp)
						{	EnableWindow(hWndbtnLast,FALSE);
							SetFocus(hWndbtnNext);
						}	
					}
					if(iWhere<0) 
					{	mHpCtrl_RefreshEventKeys(GetHelpComStruct()->mHpCtrlObj->mHpEventList);
						SetmHpListFromCtrl(hWndmHpSave,FALSE);
						return EndDialog(hWndServ,IDC_LAST);
					}
					ShowServCtrl(iWhere);
					break;
				}
				case IDC_BTN_CONFIG:
				{	if(iWhere==3)
					{	if(mHpCtrl_CreateCtrlCfgBox(hWndServ,GetHelpComStruct()->mHpCtrlObj->mHpEventList,iSelParam,TRUE)==IDOK)
						{	mHpCtrl_FillCustomList(hWndListParam,GetHelpComStruct()->mHpCtrlObj->mHpEventList);
							EnableWindow(hWndConfigTask,FALSE);
							SetFocus(hWndTerminate);
						}
					}
					else
					{	if(mHpCtrl_CreateCtrlCfgBox(hWndServ,GetHelpComStruct()->mHpCtrlObj->mHpEventList,iSelCtrl,FALSE)==IDOK)
						{	mHpCtrl_FillCtrlList(hWndListTask,GetHelpComStruct()->mHpCtrlObj->mHpEventList);
							EnableWindow(hWndConfigTask,FALSE);
							EnableWindow(hWndMoveUp,FALSE);
							EnableWindow(hWndMoveDown,FALSE);
							SetFocus(hWndTerminate);
						}
					}
					break;
				}
				case IDC_BTN_MOVEUP:
				{	int iCons=iSelCtrl-1;
					mHpCtrl_MoveUp(GetHelpComStruct()->mHpCtrlObj->mHpEventList,iSelCtrl);
					mHpCtrl_FillCtrlList(hWndListTask,GetHelpComStruct()->mHpCtrlObj->mHpEventList);
					ListView_SetItemState(hWndListTask,iCons,LVIS_SELECTED,LVIS_SELECTED);
					if(!IsWindowEnabled(hWndMoveUp)) SetFocus(hWndTerminate);
					break;
				}
				case IDC_BTN_MOVEDOWN:
				{	int iServe=iSelCtrl+1;
					mHpCtrl_MoveDown(GetHelpComStruct()->mHpCtrlObj->mHpEventList,iSelCtrl);
					mHpCtrl_FillCtrlList(hWndListTask,GetHelpComStruct()->mHpCtrlObj->mHpEventList);
					ListView_SetItemState(hWndListTask,iServe,LVIS_SELECTED,LVIS_SELECTED);
					if(!IsWindowEnabled(hWndMoveDown)) SetFocus(hWndTerminate);
					break;
				}
				case IDC_NEW_MHPSAVE:
				{	int iNewIndex=InsertNewmHpFile();
					if(iNewIndex!=NO)
					{	ListView_SetItemState(hWndmHpSave,iNewIndex,LVIS_SELECTED,LVIS_SELECTED);
						SetFocus(hWndmHpSave);
						ListView_EditLabel(hWndmHpSave,iNewIndex);
					}
					break;
				}
				case IDC_OPEN_MHPSAVE:
				{	if(GetOpenmHpFiles(hWndServ,hWndmHpSave)!=NO) GetHelpComStruct()->bmHpFileOpened=TRUE;
					SetmHpListFromCtrl(hWndmHpSave,TRUE);
					break;
				}
				case IDC_REMOVE_MHPSAVE:
				{	int iMove=ListView_GetItemSel(hWndmHpSave);
					ListView_DeleteItem(hWndmHpSave,iMove);
					GetHelpComStruct()->mHpFileList->RemovemHpFile(iMove);
					EnableWindow(hWndRemovemHp,FALSE);
					EnableWindow(hWndbtnNext,FALSE);
					SetFocus(hWndTerminate);
					if((!ListView_GetItemCount(hWndmHpSave))||(GetHelpComStruct()->mHpFileList->GetNewFile(0))) GetHelpComStruct()->bmHpFileOpened=FALSE;
					break;
				}
				case IDC_BTN_HIDESHOW:
				{	mHpCtrl_ChangeCustomVisible(GetHelpComStruct()->mHpCtrlObj->mHpEventList,iSelParam);
					if(mHpCtrl_IsCustomVisible(GetHelpComStruct()->mHpCtrlObj->mHpEventList,iSelParam))
					{	LoadString(GetHelpComStruct()->hInstServ,IDS_HIDE,lpServRC,255);
						SetWindowText(hWndHideShow,lpServRC);
						LoadString(GetHelpComStruct()->hInstServ,IDS_TRUE,lpServRC,255);
						ListView_SetItemText(hWndListParam,iSelParam,3,lpServRC);
					}
					else
					{	LoadString(GetHelpComStruct()->hInstServ,IDS_SHOW,lpServRC,255);
						SetWindowText(hWndHideShow,lpServRC);
						LoadString(GetHelpComStruct()->hInstServ,IDS_FALSE,lpServRC,255);
						ListView_SetItemText(hWndListParam,iSelParam,3,lpServRC);
					}
					break;
				}
				case IDC_DOCUMENTATION:
				{   mHpPak_OpenmHelpDoc(hWndServ,MHPPAK_DOC_SERV+iStep);
					break;
				}
			}
			break;
		}
	}
	return FALSE;
}

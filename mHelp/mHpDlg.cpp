// mHpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mHpDlg.h"

#include <mHpWebClient.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "HelpCom.h"
#include "FavsDlg.h"
#include "EvaluateDlg.h"

//-----------------------------------------------------//
// Macros
#define		IsItAChar(c)			((BOOL)(((c>=0x41)&(c<=0x5A))|(c==' ')))

//-----------------------------------------------------//
// Définitions
#define		ID_TIMER_UPDATEEDITRS		10003
#define     ID_TIMER_UPDATEWEBCLIENT    10004
//
#define		INDEXLIST_ITEMINVIEW		12
//
//#define		RS_TREE_X					9
//#define		RS_TREE_W					194
//#define		RS_LIST_X					208
//#define		RS_LIST_W					285
//#define		RS_CTRL_Y					31
//#define		RS_CTRL_H					292

#define		COLUMN_WIDTH_PARAM			100
#define		COLUMN_WIDTH_TYPE			42
#define		COLUMN_WIDTH_DESC			225
#define		COLUMN_WIDTH_LIMIT			122

#define     PREFIX_ANCHOR_LINK          "file:///"

//-----------------------------------------------------//
// Variables globales
int TabReplace[MAX_PART_TREE]={0};
HIMAGELIST hImageList;
HIMAGELIST hMgmtImgList;
HWND hWndStart=NULL;			// Bouton "Demarrer"
HWND hWndSpeed=NULL;			// TrackBar de la mouse speed...
HWND hWndTreeRS=NULL;
HWND hWndListRS=NULL;
HWND hWndmHelp=NULL;
HWND hWndAssist=NULL;
HWND hWndProgress=NULL;
HWND hWndBoucle=NULL;
HWND hWndAdd=NULL;
HWND hWndIcon=NULL;
HWND hWndEditRS=NULL;
HWND hWndStatus=NULL;
HWND hWndStaticStatus=NULL;
HWND hWndSetSearch=NULL;
HWND hWndStaticSearch=NULL;
HWND hWndStaticRepeat=NULL;
HWND hWndStaticNumRepeat=NULL;
HWND hWndStaticIndex=NULL;
HWND hWndSearch=NULL;
HWND hWndRemoveFav=NULL;
HWND hWndRemovemHpFiles=NULL;
HWND hWndFrameMode=NULL;
HWND hWndSearchList=NULL;
HWND hWndIndexList=NULL;
HWND hWndFavList=NULL;
HWND hWndmHpFilesList=NULL;
HWND hWndSetIndex=NULL;
HWND hWndEditSearch=NULL;
HWND hWndEditIndex=NULL;
HWND hWndAddFav=NULL;
HWND hWndAddmHpFiles=NULL;
HWND hWndStaticSlow=NULL;
HWND hWndStaticFast=NULL;
HWND hWndFramemHelp=NULL;
HWND hWndmHelpIcon=NULL;
HWND hWndFrameSpeed=NULL;
HWND hWndFrameSepHory=NULL;
HWND hWndSetFav=NULL;
HWND hWndFrameSepVert=NULL;
HWND hWndAbout=NULL;
HWND hWndmHpTab=NULL;
HWND hWndClose=NULL;
HWND hWndStaticEvent=NULL;
HWND hWndEditEvent=NULL;
HWND hWndRepeatCfg=NULL;
HWND hWndAutoAssistCfg=NULL;
HWND hWndRadioFav=NULL;
HWND hWndRadioAuto=NULL;
HWND hWndFrameFav=NULL;
HWND hWndStaticAuto=NULL;
HWND hWndEditAuto=NULL;
HWND hWndResetAuto=NULL;
HWND hWndCustomAuto=NULL;
HWND hWndSaveFav=NULL;
HWND hWndOpenFavs=NULL;
HWND hWndStaticDelay=NULL;
HWND hWndDelayStatic=NULL;
HWND hWndHour=NULL;
HWND hWndMinute=NULL;
HWND hWndPoints=NULL;
HWND hWndHelp=NULL;
HWND hWndDestroy=NULL;
HWND hWndHelpParam=NULL;
HWND hWndHiddenTree=NULL;
HWND hWndCompList=NULL;
HWND hWndCompLabel=NULL;
HWND hWndWebClient=NULL;
HWND hWndTopicTree=NULL;
BOOL bStyle=FALSE;
//-----------------------------------------------------//
HBRUSH hTabBrush=NULL;
/*BOOL bChangeSize=FALSE;
int XclkPos;
int Xg,Xd;
int cxTree;
int Xlist;
int cxList;*/
int iColumnParam;
int iColumnType;
int iColumnDesc;
int iColumnLimit;
int iDlgWidth;
int iDlgHeight;
//-----------------------------------------------------//
int ItemIndex;
BOOL bHeaderDid;
CTypeList *TamponList;
BOOL bTampon;
char lpRC[MAX_KEY_SIZE]={0};
char lpExeName[MAX_KEY_SIZE]={0};
int imHpFileIndex=0;
LONG_PTR lListPrevProc;
BOOL bLockOp=FALSE;
CWndDocList* MainDlgDocLst;
std::string strURL;
mHpWebClient* pWebClientCtrl;
//-----------------------------------------------------//
HIMAGELIST hImageParamList;
CWndDocList* ParamDlgDocLst;
HWND hWndType=NULL;
HWND hWndDecrit=NULL;
HWND hWndLimite=NULL;
HWND hWndEvolu=NULL;
HWND hWndAssoc=NULL;
HWND hWndExt=NULL;
HWND hWndParamList=NULL;
HWND hWndApply=NULL;
HWND hWndCopier=NULL;
HWND hWndColler=NULL;
HWND hWndValAssoc=NULL;
HWND hWndRemoveParam=NULL;
HWND hWndValidParam=NULL;
HWND hWndCancelParam=NULL;
HWND hWndStaticSel=NULL;

//-----------------------------------------------------//
// Déclaration
BOOL IsOthermHelpOpenned(int);
void RestoreFavsList();
void GetDeletedTabs(int*,int,int*,int);
LRESULT CALLBACK MainDlgDocProc(HWND,UINT,WPARAM,LPARAM);
void SetMainDlgDocLnch();
void LaunchmHelpDoc(HWND);
void LockCursorArea(HWND,long,long,long,long);
void OnMouseMove(HWND,BOOL,int,int);
void ReadRegisterKeys(BOOL);
void ReplaceAutoFavList(BOOL);
void RestoreAutoRowLag(int);
void UpdateAutoAssSel();
void ReplaceAutoAssSel();
BOOL RemoveFromAutoAss(HWND,int);
void ReplaceEvoluValues(HWND,int);
void RefreshViewContent(HWND,int);
void AddRemoveFavList(HWND,BOOL);
void WriteRegisterKeys(int);
void SetRepeatView();
void SetParamListView();
BOOL RefreshButton(HWND);
BOOL UpdateSearchList(HWND,char*);
void SetSelectedEvent(HWND,int);
void ModifyParamRef(char*,int,int);
void GetKeywordFromString(char*,char*,int,int&,BOOL&);
void UpdateEditEvent(int);
BOOL IsItAValidString(char*,char*,int);
void SetEventLag(char*,int,int,int);
BOOL UpdateIndexList(HWND,int,BOOL);
void ExpandNewTopicItem(int);
void SetmHpTabItems(BOOL);
void AddFavoritesList(char*,char*,int,int);
void AddSearchList(int,char*,int);
void AddIndexList(int,char*,char*,int,int);
int GetTopicFromTree();
void HideFavCtrls();
void HideShowFavCtrls();
void HidemHpCtrls();
void HideShowmHpCtrls(HWND);
void UpdateEditRS(void);
//-----------------------------------------------------//
void ShowCtrl(int);
void MoveDlgCtrl(HWND);
BOOL IsLoopOption();
void ReplaceDelayCfg();
//-----------------------------------------------------//
void SetLastIndex();
void CopyIndexTab(int*,int);
void CopyTabReplace(void);
void ChangeSelectedParam(void);
//-----------------------------------------------------//
void ChangeStyle(BOOL);
void GetDispInfo(LV_DISPINFO*);
void SetHeader(BOOL);
void SetCompListHeader();
void AddItemList(HWND,LPSTR,int,int);
void ReplaceTree(HWND,int*);
BOOL GetAllParamDetail(HWND,int);
BOOL CreateTree(HWND,int,HTREEITEM,HTREEITEM);
BOOL SelectUrlHTML(HTREEITEM,HTREEITEM);
BOOL CreateTopicTree(HWND,int);
BOOL SetChangeListView(HWND);
void DestroyAllButton(void);
int GetLastIndiceFromSymbolRang(char*,char);
float GetFloatFromStr(char*);
void IncrementeNumber(BOOL,BOOL);
void IncrementeString(BOOL);
void CopieFirstSel(void);
void ChangeBoolState(void);
void CopierEvolu(void);
void CollerEvolu(void);
LRESULT CALLBACK ParamDlgDocProc(HWND,UINT,WPARAM,LPARAM);
void SetParamDlgDocLnch();
//-----------------------------------------------------//
LRESULT CALLBACK ListRSProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK ParamDlgProc(HWND,UINT,WPARAM,LPARAM);

//***********************************************************************************************************************//
// CreateManageObj
//-----------------------------------------------------//
void CreateManageObj()
{	if(!GetHelpComStruct()->mHpDlgCreatedObj)
	{	mHpPak_ItemsList=new CItemListe();
		mHpPak_FavsList=new CFavListe();
		GetHelpComStruct()->ModifList=new CModifyList();
		GetHelpComStruct()->listBtn=new CButtonList();
		GetHelpComStruct()->mHpFileList=new CmHpFileList();
		GetHelpComStruct()->mHpDlgCreatedObj=TRUE;
	}
}			

//-----------------------------------------------------//
// IsAutoAssCfg
//-----------------------------------------------------//
BOOL IsAutoAssCfg()
{	char lpAuto[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	BYTE lpBt[MAX_KEY_SIZE]={0};
	HKEY ApplKey,TmpKey;
	BOOL bAss=FALSE;
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_SZ;
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&ApplKey)==ERROR_SUCCESS)
	{	// mHelp	
		if(RegOpenKeyEx(ApplKey,MHELP_KEY,0,KEY_READ,&ApplKey)==ERROR_SUCCESS)
		{	// mHpApp
			if(RegOpenKeyEx(ApplKey,MHPAPP_KEY,0,KEY_READ,&ApplKey)==ERROR_SUCCESS)
			{	// AppExe
				if(RegOpenKeyEx(ApplKey,lpExeName,0,KEY_READ,&ApplKey)==ERROR_SUCCESS)
				{	// Favorites
					if(RegOpenKeyEx(ApplKey,FAVORITES_KEY,0,KEY_READ,&TmpKey)==ERROR_SUCCESS)
					{	// AutoAss
						if(RegQueryValueEx(TmpKey,AUTO_ASSIST_KEY,NULL,&dwType,lpBt,&iSize)==ERROR_SUCCESS)
						{	mHpPak_FillStringFromData(lpAuto,MAX_RS_SIZE,lpBt,(int)iSize);
							if(strncmp(lpAuto,NULL_STRING,MAX_RS_SIZE)!=0) bAss=TRUE;
						}
					}	
					if(bAss)
					{	CreateManageObj();
						ReadFavRegKey(ApplKey,mHpPak_FavsList);
						mHpPak_ChangeAutoAss(lpTmp,MAX_RS_SIZE,0,
											 GetHelpComStruct()->iMode,
											 GetHelpComStruct()->IndexTab,MAX_PART_TREE,
											 GetHelpComStruct()->iNbBoucle);
					}
				}
			}
		}
	}
	return mHpPak_bAutoAss;
}

//***********************************************************************************************************************//
// IsOthermHelpOpenned
//-----------------------------------------------------//
BOOL IsOthermHelpOpenned(int iNoStatus)
{	char lpNameApp[MAX_KEY_SIZE]={0};
	BOOL bReturn=FALSE,bRes=FALSE;
	int iAppStat,imHpStat,iNext=0;
	mHpPak_GetNextAppInfoKeys(iNext,lpNameApp,MAX_KEY_SIZE,iAppStat,imHpStat,bRes);
	while(bRes)
	{	if((strncmp(lpNameApp,lpExeName,MAX_KEY_SIZE)!=0)&&(imHpStat==iNoStatus)) bReturn=TRUE;
		iNext++;
		mHpPak_GetNextAppInfoKeys(iNext,lpNameApp,MAX_KEY_SIZE,iAppStat,imHpStat,bRes);
	}
	return bReturn;
}

//-----------------------------------------------------//
// RestoreFavsList
//-----------------------------------------------------//
void RestoreFavsList()
{	long lStyle;
	// mHpPak_FavsList
	ReadRegisterKeys(FALSE);
	ReplaceAutoFavList(FALSE);
	SendMessage(hWndRadioFav,BM_SETCHECK,BST_CHECKED,0);
	SendMessage(hWndRadioAuto,BM_SETCHECK,BST_UNCHECKED,0);
	ListView_SetItemState(hWndFavList,NO,0,LVIS_SELECTED);
	lStyle=(long)GetWindowLongPtr(hWndFavList,GWL_STYLE);
	lStyle|=LVS_SINGLESEL;
	SetWindowLongPtr(hWndFavList,GWL_STYLE,(LONG_PTR)lStyle);
	EnableWindow(hWndRemoveFav,FALSE);
	EnableWindow(hWndSaveFav,FALSE);
	EnableWindow(hWndSetFav,FALSE);
	EnableWindow(hWndRadioAuto,TRUE);
	EnableWindow(hWndResetAuto,TRUE);
	if(strncmp(mHpPak_FavsList->GetAutoAssist(),NULL_STRING,MAX_RS_SIZE)!=0)
	{	LoadString(GetHelpComStruct()->hInstServ,IDS_AUTOLNCH_YES,lpRC,MAX_KEY_SIZE);
		SetWindowText(hWndAutoAssistCfg,lpRC);
	}
	else
	{	if(ListView_GetItemCount(hWndFavList)==0) EnableWindow(hWndRadioAuto,FALSE);
		EnableWindow(hWndResetAuto,FALSE);
		LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
		SetWindowText(hWndAutoAssistCfg,lpRC);
	}
}

//-----------------------------------------------------//
// GetDeletedTabs
//-----------------------------------------------------//
void GetDeletedTabs(int* pTabLine,int iMaxLine,int* pTabDecal,int iMaxDecal)
{	char lpItm[MAX_RS_SIZE]={0};
	TVITEM Tvitem;
	int iLineRef;
	HTREEITEM hHideItem=TreeView_GetSelection(hWndHiddenTree);
	BOOL bStop=FALSE;
	Tvitem.mask=TVIF_TEXT;
	Tvitem.cchTextMax=MAX_RS_SIZE;
	Tvitem.pszText=lpItm;
	Tvitem.hItem=hHideItem;
	mHpPak_EmptyTab(pTabLine,iMaxLine);
	TreeView_GetItem(hWndHiddenTree,&Tvitem);
	iLineRef=mHpPak_GetTopicFromRef(Tvitem.pszText,0);
	if(mHpPak_GetTopicFromRef(Tvitem.pszText,1)==2) // Param
	{	mHpPak_AddIndex(pTabLine,(iLineRef+1));}
	mHpPak_AddIndex(pTabLine,iLineRef);
	if((TreeView_GetNextItem(hWndHiddenTree,hHideItem,TVGN_NEXT)==NULL)&&
	   (TreeView_GetNextItem(hWndHiddenTree,hHideItem,TVGN_PREVIOUS)==NULL))
	{	hHideItem=TreeView_GetParent(hWndHiddenTree,hHideItem);
		while((hHideItem!=NULL)&&(!bStop))
		{	Tvitem.mask=TVIF_TEXT;
			Tvitem.cchTextMax=MAX_RS_SIZE;
			Tvitem.hItem=hHideItem;
			TreeView_GetItem(hWndHiddenTree,&Tvitem);
			mHpPak_AddIndex(pTabLine,mHpPak_GetTopicFromRef(Tvitem.pszText,0));
			if((TreeView_GetNextItem(hWndHiddenTree,hHideItem,TVGN_NEXT)!=NULL)||
			   (TreeView_GetNextItem(hWndHiddenTree,hHideItem,TVGN_PREVIOUS)!=NULL))
			{	bStop=TRUE;}
			else
			{	if(TreeView_GetParent(hWndHiddenTree,hHideItem)!=NULL)	
				{	hHideItem=TreeView_GetParent(hWndHiddenTree,hHideItem);}
				else bStop=TRUE;
			}
		}
	}
	// Inverse tab 'pTabLine'
	InverseTab(pTabLine,TRUE);
	mHpPak_EmptyTab(pTabDecal,iMaxDecal);
	while(hHideItem!=NULL)
	{	iLineRef=0;
		while(TreeView_GetNextItem(hWndHiddenTree,hHideItem,TVGN_PREVIOUS)!=NULL)
		{	hHideItem=TreeView_GetNextItem(hWndHiddenTree,hHideItem,TVGN_PREVIOUS);
			iLineRef++;
		}
		mHpPak_AddIndex(pTabDecal,iLineRef);
		hHideItem=TreeView_GetParent(hWndHiddenTree,hHideItem);
	}
	// Inverse tab 'pTabDecal'
	InverseTab(pTabDecal,FALSE);
}

//-----------------------------------------------------//
// MainDlgDocProc
//-----------------------------------------------------//
LRESULT CALLBACK MainDlgDocProc(HWND hWndDoc,UINT DocMessage,WPARAM DocWParam,LPARAM DocLParam)
{	LONG_PTR lPrevProc=MainDlgDocLst->GetDocWndProc(hWndDoc);
	switch(DocMessage)
	{	case WM_KEYUP:
		{	switch(DocWParam)
			{	case VK_F1:
				{	LaunchmHelpDoc(hWndDoc);
					break;
				}
				default:
				{	break;}
			}
			break;
		}
		/*case WM_MOUSEMOVE:
		{	SetClassLongPtr(hWndDoc,GCL_HCURSOR,(LONG_PTR)GetHelpComStruct()->hmHpCursor);
			SetCursor(GetHelpComStruct()->hmHpCursor);
			break;
		}*/
		case WM_NCDESTROY:
		{	MainDlgDocLst->RemoveDocWnd(hWndDoc);
			break;
		}
	}
	return CallWindowProc((WNDPROC)lPrevProc,hWndDoc,DocMessage,DocWParam,DocLParam);
}

//-----------------------------------------------------//
// SetMainDlgDocLnch
//-----------------------------------------------------//
void SetMainDlgDocLnch()
{	MainDlgDocLst->AddDocWnd(hWndStart,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndSpeed,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndTreeRS,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndmHelp,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndAssist,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndBoucle,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndAdd,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndEditRS,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndSetSearch,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndSearch,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndRemoveFav,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndRemovemHpFiles,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndSearchList,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndIndexList,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndFavList,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndmHpFilesList,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndSetIndex,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndEditSearch,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndEditIndex,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndAddFav,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndAddmHpFiles,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndSetFav,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndmHpTab,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndClose,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndEditEvent,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndRadioFav,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndRadioAuto,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndEditAuto,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndResetAuto,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndCustomAuto,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndSaveFav,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndOpenFavs,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndHour,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndMinute,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndHelp,(LONG_PTR)MainDlgDocProc);
	MainDlgDocLst->AddDocWnd(hWndDestroy,(LONG_PTR)MainDlgDocProc);
}

//-----------------------------------------------------//
// LaunchmHelpDoc
//-----------------------------------------------------//
void LaunchmHelpDoc(HWND hWndActive)
{	int iSelTab=TabCtrl_GetCurSel(hWndmHpTab);
	if(GetHelpComStruct()->bmHpFileOpened)
	{	switch(iSelTab)
		{	case 0: // mHelp File
			case 1: // Contents
			case 2: // Configuration
			case 3: // Search
			case 4: // Index
			{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_MAIN+iSelTab);
				break;
			}
			case 5: // Favorites
			{	if(SendMessage(hWndRadioFav,BM_GETCHECK,0,0)==BST_CHECKED)
				{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_MAINFAV);}
				else
				{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_MAINAUTO);}
				break;
			}
			case 6: // UI Component
			{	
                //
                // TODO: Add Help document
                //
                break;
			}
			case 7: // About
			{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_WELCOME);
				break;
			}
		}
	}
	else
	{	switch(iSelTab)
		{	case 0: // mHelp File
			{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_MAIN);
				break;
			}
			case 1:
			{	if(ListView_GetItemCount(hWndFavList)!=0) // Favorites
				{	if(SendMessage(hWndRadioFav,BM_GETCHECK,0,0)==BST_CHECKED)
					{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_MAINFAV);}
					else
					{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_MAINAUTO);}
				}
				else mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_WELCOME); // About
				break;
			}
			case 2: // UI Component
			{
                //
                // TODO: Add Help document
                //
				break;
			}
			case 3: // About
			{	mHpPak_OpenmHelpDoc(hWndActive,MHPPAK_DOC_WELCOME);
				break;
			}
		}
	}
}

//-----------------------------------------------------//
// LockCursorArea
//-----------------------------------------------------//
/*void LockCursorArea(HWND hWndActive,long lLeft,long lTop,long lRight,long lBottom)
{	RECT hWndRect;
	GetWindowRect(hWndActive,&hWndRect);
	CONST RECT LockArea={(lLeft+hWndRect.left),(lTop+hWndRect.top),(lRight+hWndRect.left),(lBottom+hWndRect.top)};
	ClipCursor(&LockArea);
}

//-----------------------------------------------------//
// OnMouseMove
//-----------------------------------------------------//
void OnMouseMove(HWND hWndActive,BOOL bWndXP,int iXpos,int iYpos)
{	if(bChangeSize)
	{	SetmHpCursor(hWndActive,2);
		if((XclkPos!=iXpos)&&(iXpos<=(RS_LIST_X+RS_LIST_W-60))&&(iXpos>=(RS_TREE_X+60)))
		{	if(XclkPos>iXpos)
			{	cxTree--;
				Xlist--;
				cxList++;
				Xg--;
				Xd--;
			}
			else
			{	cxTree++;
				Xlist++;
				cxList--;
				Xg++;
				Xd++;
			}
			MoveWindow(hWndTreeRS,RS_TREE_X,RS_CTRL_Y,cxTree,RS_CTRL_H,TRUE);
			MoveWindow(hWndListRS,Xlist,RS_CTRL_Y,cxList,RS_CTRL_H,TRUE);
			if(bWndXP) LockCursorArea(hWndActive,(long)(Xg+3),(RS_CTRL_Y+33),(long)(Xd+3),(RS_CTRL_Y+RS_CTRL_H+26));
			else LockCursorArea(hWndActive,(long)(Xg+3),(RS_CTRL_Y+26),(long)(Xd+3),(RS_CTRL_Y+RS_CTRL_H+18));
			XclkPos=iXpos;
		}
	}
	else if((iYpos>=RS_CTRL_Y)&&(iYpos<=(RS_CTRL_Y+RS_CTRL_H))&&(iXpos>=Xg)&&(iXpos<=Xd))
	{	SetmHpCursor(hWndActive,2);}
	else SetmHpCursor(hWndActive,0);
}*/

//-----------------------------------------------------//
// ReadRegisterKeys
//-----------------------------------------------------//
void ReadRegisterKeys(BOOL bAll)
{	char lpmHpPath[MAX_RS_SIZE]={0};
	char lpPathKey[MAX_KEY_SIZE]={0};
	BYTE szBTab[MAX_KEY_SIZE]={0};
	HKEY AppKey,TmpKey;
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_SZ;
	int imHpPath=0,iAss,iHelp;
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&AppKey)!=ERROR_SUCCESS) return;
	// mHelp	
	if(RegOpenKeyEx(AppKey,MHELP_KEY,0,KEY_READ,&AppKey)!=ERROR_SUCCESS) return;
	// mHpApp
	if(RegOpenKeyEx(AppKey,MHPAPP_KEY,0,KEY_READ,&AppKey)!=ERROR_SUCCESS) return;
	// AppExe
	if(RegOpenKeyEx(AppKey,lpExeName,0,KEY_READ,&AppKey)!=ERROR_SUCCESS) return;
	// Favorites
	ReadFavRegKey(AppKey,mHpPak_FavsList);
	if(!bAll) return;
	// CurrentFiles
	GetHelpComStruct()->mHpFileList->EmptymHpFileList();
	if(RegOpenKeyEx(AppKey,CURRENT_FILES_KEY,0,KEY_READ,&TmpKey)!=ERROR_SUCCESS) return;
	_snprintf(lpPathKey,MAX_KEY_SIZE,"%d",imHpPath);
	while(RegQueryValueEx(TmpKey,lpPathKey,NULL,&dwType,szBTab,&iSize)==ERROR_SUCCESS)
	{	mHpPak_FillStringFromData(lpmHpPath,MAX_RS_SIZE,szBTab,(int)iSize);
		GetHelpComStruct()->mHpFileList->AddmHpFile(lpmHpPath,FALSE);
		GetHelpComStruct()->bmHpFileOpened=TRUE;
		imHpPath++;
		mHpPak_InitStr(lpPathKey,MAX_KEY_SIZE);
		_snprintf(lpPathKey,MAX_KEY_SIZE,"%d",imHpPath);
		iSize=MAX_KEY_SIZE;
	}
	// Settings
	if(RegOpenKeyEx(AppKey,SETTINGS_KEY,0,KEY_READ,&TmpKey)!=ERROR_SUCCESS) return;
	// AssistCheck
	dwType=REG_BINARY;iSize=MAX_KEY_SIZE;
	if(RegQueryValueEx(TmpKey,ASSIST_CHECK_KEY,NULL,&dwType,szBTab,&iSize)!=ERROR_SUCCESS) return;
	mHpPak_FillIntFromData(szBTab,iAss,TRUE);
	// HelpCheck
	iSize=MAX_KEY_SIZE;
	if(RegQueryValueEx(TmpKey,HELP_CHECK_KEY,NULL,&dwType,szBTab,&iSize)!=ERROR_SUCCESS) return;
	mHpPak_FillIntFromData(szBTab,iHelp,TRUE);
	if(iAss!=0)
	{	if(iHelp!=0) GetHelpComStruct()->iMode=0;
		else GetHelpComStruct()->iMode=1;
	}
	else if(iHelp!=0) GetHelpComStruct()->iMode=2;
	// LoopOption
	dwType=REG_DWORD;iSize=MAX_KEY_SIZE;
	if(RegQueryValueEx(TmpKey,LOOPOPTION_KEY,NULL,&dwType,szBTab,&iSize)!=ERROR_SUCCESS) return;
	mHpPak_FillIntFromData(szBTab,GetHelpComStruct()->iNbBoucle,FALSE);
	if((GetHelpComStruct()->iNbBoucle<=0)||(GetHelpComStruct()->iNbBoucle>=100)) GetHelpComStruct()->iNbBoucle=1;
	// MouseSpeed
	iSize=MAX_KEY_SIZE;
	if(RegQueryValueEx(TmpKey,MOUSESPEED_KEY,NULL,&dwType,szBTab,&iSize)!=ERROR_SUCCESS) return;
	mHpPak_FillIntFromData(szBTab,GetHelpComStruct()->MouseSpeed,FALSE);
	if((GetHelpComStruct()->MouseSpeed<6)||(GetHelpComStruct()->MouseSpeed>20))
	    GetHelpComStruct()->MouseSpeed=14;
	// HourDelay
	iSize=MAX_KEY_SIZE;
	if(RegQueryValueEx(TmpKey,HOUR_DELAY_KEY,NULL,&dwType,szBTab,&iSize)!=ERROR_SUCCESS) return;
	mHpPak_FillIntFromData(szBTab,GetHelpComStruct()->iHourDelay,FALSE);
	if((GetHelpComStruct()->iHourDelay<0)||(GetHelpComStruct()->iHourDelay>999)) GetHelpComStruct()->iHourDelay=0;
	// MinuteDelay
	iSize=MAX_KEY_SIZE;
	if(RegQueryValueEx(TmpKey,MINUTE_DELAY_KEY,NULL,&dwType,szBTab,&iSize)!=ERROR_SUCCESS) return;
	mHpPak_FillIntFromData(szBTab,GetHelpComStruct()->iMinuteDelay,FALSE);
	if((GetHelpComStruct()->iMinuteDelay<0)||(GetHelpComStruct()->iMinuteDelay>59))
	{	if(GetHelpComStruct()->iHourDelay!=0) GetHelpComStruct()->iMinuteDelay=0;
		else GetHelpComStruct()->iMinuteDelay=30;
	}
}

//-----------------------------------------------------//
// ReplaceAutoFavList
//-----------------------------------------------------//
void ReplaceAutoFavList(BOOL bReSelect)
{	int iSelTab[MAX_PART_TREE]={0};
	int iNext=0,iSelCnt=0,iSel,iImg;
	BOOL bEvol;
	if(bReSelect)
	{	mHpPak_EmptyTab(iSelTab,MAX_PART_TREE);
		for(int i=0;i<ListView_GetItemCount(hWndFavList);i++)
		{	if(ListView_GetItemState(hWndFavList,i,LVIS_SELECTED)==LVIS_SELECTED)
			{	iSelTab[iSelCnt]=i;
				iSelCnt++;
			}
		}
		iSelTab[iSelCnt]=NO;
	}
	ListView_DeleteAllItems(hWndFavList);
	while(iNext!=mHpPak_FavsList->GetFavCount())
	{	if(mHpPak_FavsList->GetItemFromRang(iNext)->FavItem->ParamList!=NULL)
		{	iSelCnt=0;bEvol=FALSE;
			while(iSelCnt!=mHpPak_FavsList->GetItemFromRang(iNext)->FavItem->ParamList->GetSelCount())
			{	iSel=mHpPak_FavsList->GetItemFromRang(iNext)->FavItem->ParamList->GetSelItem(iSelCnt);
				if(mHpPak_FavsList->GetItemFromRang(iNext)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetEvoluFlag())
				{	bEvol=TRUE;}
				iSelCnt++;
			}
			if(bEvol) iImg=6;
			else  iImg=3;
		}
		else iImg=1;
		if(mHpPak_FavsList->GetLoopOption(iNext)!=1)
		{	AddFavoritesList(mHpPak_FavsList->GetmHpPath(iNext),
							 mHpPak_FavsList->GetEventStr(iNext),
							 mHpPak_FavsList->GetLoopOption(iNext),iImg);
		}
		else AddFavoritesList(mHpPak_FavsList->GetmHpPath(iNext),mHpPak_FavsList->GetEventStr(iNext),NO,iImg);
		iNext++;
	}
	if(iNext!=0)
	{	if(bReSelect)
		{	iSelCnt=0;
			while(iSelTab[iSelCnt]!=NO)
			{	ListView_SetItemState(hWndFavList,iSelTab[iSelCnt],LVIS_SELECTED,LVIS_SELECTED);
				iSelCnt++;
			}
		}
		EnableWindow(hWndRadioAuto,TRUE);
		if(strncmp(mHpPak_FavsList->GetAutoAssist(),NULL_STRING,MAX_RS_SIZE)!=0)
		{	EnableWindow(hWndResetAuto,TRUE);
			LoadString(GetHelpComStruct()->hInstServ,IDS_AUTOLNCH_YES,lpRC,MAX_KEY_SIZE);
			SetWindowText(hWndAutoAssistCfg,lpRC);
		}
		else
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
			SetWindowText(hWndAutoAssistCfg,lpRC);
		}
	}
	else EnableWindow(hWndRadioAuto,FALSE);
}

//-----------------------------------------------------//
// RestoreAutoRowLag
//-----------------------------------------------------//
void RestoreAutoRowLag(int iMoveIndex)
{	char lpNewAss[MAX_RS_SIZE]={0};
	char lpRow[MAX_RS_SIZE]={0};
	int index=0,iCurRow;
	for(int i=0;i<ListView_GetItemCount(hWndFavList);i++)
	{	if(i>=iMoveIndex)
		{	mHpPak_InitStr(lpRow,MAX_RS_SIZE);
			_snprintf(lpRow,MAX_RS_SIZE,"%d",(i+1));
			ListView_SetItemText(hWndFavList,i,3,lpRow);
		}
	}
	if(strncmp(mHpPak_FavsList->GetAutoAssist(),NULL_STRING,MAX_RS_SIZE)!=0)
	{	iCurRow=mHpPak_GetAutoAssRow(mHpPak_FavsList->GetAutoAssist(),index);
		while(iCurRow!=NO)
		{	if(iCurRow>(iMoveIndex+1)) iCurRow--;
			if(!index) _snprintf(lpNewAss,MAX_RS_SIZE,"%d",iCurRow);
			else
			{	mHpPak_InitStr(lpRow,MAX_RS_SIZE);
				_snprintf(lpRow,MAX_RS_SIZE,"%d",iCurRow);
				strncat(lpNewAss,STR_SEPARATED_AUTOASS,1);
				strncat(lpNewAss,lpRow,mHpPak_FindStrLenght(lpRow,MAX_RS_SIZE));
			}
			index++;
			iCurRow=mHpPak_GetAutoAssRow(mHpPak_FavsList->GetAutoAssist(),index);
		}
		mHpPak_FavsList->SetAutoAssist(lpNewAss);
		LoadString(GetHelpComStruct()->hInstServ,IDS_AUTOLNCH_YES,lpRC,MAX_KEY_SIZE);
		SetWindowText(hWndAutoAssistCfg,lpRC);
	}
}

//-----------------------------------------------------//
// UpdateAutoAssSel
//-----------------------------------------------------//
void UpdateAutoAssSel()
{	char lpLastAuto[MAX_RS_SIZE]={0};
	char lpNewAuto[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	BOOL bFind;
	int index,irow,iRemAdd=NO;
	GetWindowText(hWndEditAuto,lpLastAuto,MAX_RS_SIZE);
	for(int i=0;i<ListView_GetItemCount(hWndFavList);i++)
	{	if(ListView_GetItemState(hWndFavList,i,LVIS_SELECTED)==LVIS_SELECTED)
		{	index=0;bFind=FALSE;
			irow=mHpPak_GetAutoAssRow(lpLastAuto,index);
			while(irow!=NO)
			{	if(irow==(i+1)) bFind=TRUE;
				index++;
				irow=mHpPak_GetAutoAssRow(lpLastAuto,index);
			}
			if(!bFind) iRemAdd=(i+1);
		}
	}
	if(iRemAdd!=NO) // Add
	{	if(strncmp(lpLastAuto,NULL_STRING,MAX_RS_SIZE)!=0)
		{	strncpy(lpNewAuto,lpLastAuto,MAX_RS_SIZE);
			strncat(lpNewAuto,STR_SEPARATED_AUTOASS,1);
			_snprintf(lpTmp,MAX_RS_SIZE,"%d",iRemAdd);
			strncat(lpNewAuto,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
		}
		else _snprintf(lpNewAuto,MAX_RS_SIZE,"%d",iRemAdd);
	}
	else // Remove
	{	index=0;bFind=FALSE;
		irow=mHpPak_GetAutoAssRow(lpLastAuto,index);
		while(irow!=NO)
		{	if(ListView_GetItemState(hWndFavList,(irow-1),LVIS_SELECTED)==LVIS_SELECTED)
			{	if(!bFind)
				{	_snprintf(lpNewAuto,MAX_RS_SIZE,"%d",irow);
					bFind=TRUE;
				}
				else
				{	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
					_snprintf(lpTmp,MAX_RS_SIZE,"%d",irow);
					strncat(lpNewAuto,STR_SEPARATED_AUTOASS,1);
					strncat(lpNewAuto,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
				}
			}
			index++;
			irow=mHpPak_GetAutoAssRow(lpLastAuto,index);
		}
	}
	SetWindowText(hWndEditAuto,lpNewAuto);
}

//-----------------------------------------------------//
// ReplaceAutoAssSel
//-----------------------------------------------------//
void ReplaceAutoAssSel()
{	long lStyle=(long)GetWindowLongPtr(hWndFavList,GWL_STYLE);
	int index=0,iRow;
	lStyle&=~(LVS_SINGLESEL);
	SetWindowLongPtr(hWndFavList,GWL_STYLE,(LONG_PTR)lStyle);
	if(strncmp(mHpPak_FavsList->GetAutoAssist(),NULL_STRING,MAX_RS_SIZE)!=0)
	{	iRow=mHpPak_GetAutoAssRow(mHpPak_FavsList->GetAutoAssist(),index);
		while(iRow!=NO)
		{	ListView_SetItemState(hWndFavList,(iRow-1),LVIS_SELECTED,LVIS_SELECTED);
			index++;
			iRow=mHpPak_GetAutoAssRow(mHpPak_FavsList->GetAutoAssist(),index);
		}
		SetWindowText(hWndEditAuto,mHpPak_FavsList->GetAutoAssist());
	}
	else SetWindowText(hWndEditAuto,NULL_STRING);
}

//-----------------------------------------------------//
// RemoveFromAutoAss
//-----------------------------------------------------//
BOOL RemoveFromAutoAss(HWND hWndActive,int iSelIndex)
{	char lpAuto[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	int indice=0,iRow;
	BOOL bFirst=TRUE;
	iRow=mHpPak_GetAutoAssRow(mHpPak_FavsList->GetAutoAssist(),indice);
	while(iRow!=NO)
	{	if((iSelIndex+1)!=iRow)
		{	if(!bFirst) strncat(lpAuto,STR_SEPARATED_AUTOASS,1);
			mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
			_snprintf(lpTmp,MAX_RS_SIZE,"%d",iRow);
			if(bFirst)
			{	strncpy(lpAuto,lpTmp,MAX_RS_SIZE);
				bFirst=FALSE;
			}
			else strncat(lpAuto,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
		}
		else if(mHpPak_mHpMessage(hWndActive,GetHelpComStruct()->hInstServ,IDS_REMOVEFAV_AUTOASSIST,NULL,0,0,MB_YESNO|MB_ICONQUESTION)!=IDYES)
		{	return FALSE;}
		indice++;
		iRow=mHpPak_GetAutoAssRow(mHpPak_FavsList->GetAutoAssist(),indice);
	}
	mHpPak_FavsList->SetAutoAssist(lpAuto);
	SetWindowText(hWndEditAuto,lpAuto);
	if(!strncmp(lpAuto,NULL_STRING,MAX_RS_SIZE))
	{	EnableWindow(hWndResetAuto,FALSE);
		LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
		SetWindowText(hWndAutoAssistCfg,lpRC);
	}
	else
	{	EnableWindow(hWndResetAuto,TRUE);
		LoadString(GetHelpComStruct()->hInstServ,IDS_AUTOLNCH_YES,lpRC,MAX_KEY_SIZE);
		SetWindowText(hWndAutoAssistCfg,lpRC);
	}
	return TRUE;
}

//-----------------------------------------------------//
// ReplaceEvoluValues
//-----------------------------------------------------//
void ReplaceEvoluValues(HWND hWndActive,int iSelect)
{	char lplvitm[MAX_RS_SIZE]={0};
	int index=0,ItmSel,PType,iRepeat;
	LVITEM lvNewItm,lvLastItm;
	lvNewItm.mask=LVIF_TEXT;
	lvNewItm.iSubItem=0; 
	lvNewItm.cchTextMax=MAX_RS_SIZE;
	lvLastItm.mask=LVIF_IMAGE;
	lvLastItm.iSubItem=0;
	lvNewItm.pszText=lplvitm;
	index=0;
	if(mHpPak_FavsList->GetItemFromRang(iSelect)->FavItem->ParamList!=NULL)
	{	while(index!=mHpPak_FavsList->GetItemFromRang(iSelect)->FavItem->ParamList->GetSelCount())
		{	ItmSel=mHpPak_FavsList->GetItemFromRang(iSelect)->FavItem->ParamList->GetSelItem(index);
			PType=mHpPak_FavsList->GetItemFromRang(iSelect)->FavItem->ParamList->GetCellFromItem(ItmSel)->iItemType;
			lvLastItm.iItem=ItmSel;
			if(mHpPak_FavsList->GetItemFromRang(iSelect)->FavItem->ParamList->GetCellFromItem(ItmSel)->EvoluItem->GetEvoluFlag())
			{	ListView_DeleteAllItems(GetDlgItem(hWndActive,IDC_HIDE_FAVLIST));
				iRepeat=0;
				while(iRepeat!=GetHelpComStruct()->iNbBoucle)
				{	lvNewItm.iItem=iRepeat; 
					strncpy(lvNewItm.pszText,mHpPak_FavsList->GetItemFromRang(iSelect)->FavItem->ParamList->GetCellFromItem(ItmSel)->EvoluItem->GetStringParam(iRepeat,PType),MAX_RS_SIZE);
					ListView_InsertItem(GetDlgItem(hWndActive,IDC_HIDE_FAVLIST),&lvNewItm);
					iRepeat++;
				}
				mHpPak_ItemsList->AttachRepeatParam(ItmSel,GetDlgItem(hWndActive,IDC_HIDE_FAVLIST));
				lvLastItm.iImage=6;
				ListView_SetItem(hWndListRS,&lvLastItm);
			}
			else
			{	// Le 2° paramètre est une string de MAX_RS_LISTE et non de MAX_RS_SIZE !!!
				mHpPak_ItemsList->ModifyStrParam(ItmSel,mHpPak_FavsList->GetItemFromRang(iSelect)->FavItem->ParamList->GetCellFromItem(ItmSel)->EvoluItem->GetStringParam(0,PType));
				lvLastItm.iImage=3;
				ListView_SetItem(hWndListRS,&lvLastItm);
			}
			ListView_SetItemText(hWndListRS,ItmSel,0,
								 mHpPak_FavsList->GetItemFromRang(iSelect)->FavItem->ParamList->GetCellFromItem(ItmSel)->EvoluItem->GetStringParam(0,PType));
			index++;
		}
	}
}

//-----------------------------------------------------//
// RefreshViewContent
//-----------------------------------------------------//
void RefreshViewContent(HWND hWndActive,int iLastCnt)
{	int indice;
	BOOL bOpened=GetHelpComStruct()->bmHpFileOpened;
	if(!GetHelpComStruct()->bmHpFileOpened) GetHelpComStruct()->bmHpFileOpened=TRUE;
	for(indice=iLastCnt;indice<ListView_GetItemCount(hWndmHpFilesList);indice++)
	{	SetmHpCursor(hWndActive,1);
#ifdef MHELP_OLD
		CreateTree(hWndActive,indice,NULL,NULL);
		SendMessage(hWndProgress,PBM_SETPOS,0,0);
		UpdateIndexList(hWndActive,indice,TRUE);
#else
        CreateTopicTree(hWndActive,0);
        char lpFile[MAX_RS_SIZE]={0};
        SelectHtmlURL(lpFile,MAX_RS_SIZE,hWndTopicTree,hWndHiddenTree,NULL,NULL);
        strURL=lpFile;
        if(!pWebClientCtrl)
        {   pWebClientCtrl=new mHpWebClient(strURL,
                                            GetHelpComStruct()->hInstServ,
                                            true,
                                            208,31,285,292,
                                            hWndActive);
            hWndWebClient=pWebClientCtrl->hwnd_;
        }
        else
        {   assert(!hWndWebClient);
            pWebClientCtrl->Create(208,31,285,292,hWndActive);
            pWebClientCtrl->bUserTrigURL_=false;
            pWebClientCtrl->URL(strURL);
            hWndWebClient=pWebClientCtrl->hwnd_;
        }
        MoveWindow(hWndWebClient,218,31,iDlgWidth-(506-276),iDlgHeight-(407-334),TRUE);
        EnableWindow(hWndAddmHpFiles,FALSE);
#endif
		SetmHpCursor(hWndActive,0);
		SendMessage(hWndProgress,PBM_SETPOS,0,0);
		ExpandNewTopicItem(indice);
	}
	LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_READY,lpRC,MAX_KEY_SIZE);
	SetWindowText(hWndStatus,lpRC);
	if(!bOpened)
	{	SetmHpTabItems(TRUE);
		HideShowmHpCtrls(hWndActive);
		TreeView_SelectItem(hWndTreeRS,TreeView_GetRoot(hWndTreeRS));
	}
}

//-----------------------------------------------------//
// AddRemoveFavList
//-----------------------------------------------------//
void AddRemoveFavList(HWND hWndActive,BOOL bAddRemove)
{	char lpPath[MAX_RS_SIZE]={0};
	char lpValue[MAX_RS_SIZE]={0};
	char lpEvent[MAX_RS_SIZE]={0};
	int indice,iRep,iType;
	BOOL bEvolu=FALSE;
	CEvoluList* NewEvo=NULL;
	CRepeatList* NewRep=NULL;
	if(bAddRemove)
	{	ListView_GetItemText(hWndmHpFilesList,GetTopicFromTree(),1,lpPath,MAX_RS_SIZE);
		GetWindowText(hWndEditRS,lpEvent,MAX_RS_SIZE);
		if((GetWindowLongPtr(hWndListRS,GWL_STYLE)&LVS_REPORT)==LVS_REPORT) // Param
		{	NewEvo=new CEvoluList();
			for(indice=0;indice<ListView_GetItemCount(hWndListRS);indice++)
			{	if(ListView_GetItemState(hWndListRS,indice,LVIS_SELECTED)==LVIS_SELECTED)
				{	NewRep=new CRepeatList();
					iRep=0;iType=mHpPak_ItemsList->GetParameterType(indice);				
					while(iRep!=GetHelpComStruct()->iNbBoucle)
					{	NewRep->AddTypeList(iType,mHpPak_ItemsList->GetDetail(indice,0,iRep,lpRC,MAX_KEY_SIZE),MAX_KEY_SIZE);
						iRep++;
						if(!mHpPak_ItemsList->IsRepeating(indice)) iRep=GetHelpComStruct()->iNbBoucle;
						else bEvolu=TRUE;
					}
					if(mHpPak_ItemsList->IsRepeating(indice)) NewRep->SetEvoluFlag(TRUE);
					NewEvo->AddEvoluItem(indice,iType,NewRep);
				}
			}
			if(GetHelpComStruct()->iMode==1) iType=mHpPak_FavsList->AddFavItem(GetHelpComStruct()->IndexTab,GetHelpComStruct()->iNbBoucle,lpPath,lpEvent,NewEvo,TRUE);
			else iType=mHpPak_FavsList->AddFavItem(GetHelpComStruct()->IndexTab,1,lpPath,lpEvent,NewEvo,TRUE);
			if(iType!=NO)
			{	ListView_SetItemState(hWndFavList,iType,LVIS_SELECTED,LVIS_SELECTED);}
			else if(!bEvolu)
			{	if((GetHelpComStruct()->iMode==1)&&(GetHelpComStruct()->iNbBoucle>1)) AddFavoritesList(lpPath,lpEvent,GetHelpComStruct()->iNbBoucle,3);
				else AddFavoritesList(lpPath,lpEvent,NO,3);
			}
			else AddFavoritesList(lpPath,lpEvent,GetHelpComStruct()->iNbBoucle,6);
		}
		else // Doc
		{	mHpPak_AddIndex(GetHelpComStruct()->IndexTab,ListView_GetItemSel(hWndListRS));
			if(GetHelpComStruct()->iMode==1) iType=mHpPak_FavsList->AddFavItem(GetHelpComStruct()->IndexTab,GetHelpComStruct()->iNbBoucle,lpPath,lpEvent,NewEvo,TRUE);
			else iType=mHpPak_FavsList->AddFavItem(GetHelpComStruct()->IndexTab,1,lpPath,lpEvent,NewEvo,TRUE);
			if(iType!=NO)
			{	ListView_SetItemState(hWndFavList,iType,LVIS_SELECTED,LVIS_SELECTED);}
			else
			{	if((GetHelpComStruct()->iMode==1)&&(GetHelpComStruct()->iNbBoucle>1)) AddFavoritesList(lpPath,lpEvent,GetHelpComStruct()->iNbBoucle,1);
				else AddFavoritesList(lpPath,lpEvent,NO,1);
			}
			SetLastIndex();
		}
	}
	else
	{	indice=ListView_GetItemSel(hWndFavList);
		if((strncmp(mHpPak_FavsList->GetAutoAssist(),NULL_STRING,MAX_RS_SIZE)!=0)&&(!RemoveFromAutoAss(hWndActive,indice)))
		{	return;}
		mHpPak_FavsList->RemoveFavItem(indice);
		ListView_DeleteItem(hWndFavList,indice);
		RestoreAutoRowLag(indice);
		EnableWindow(hWndSetFav,FALSE);
		EnableWindow(hWndRemoveFav,FALSE);
		EnableWindow(hWndSaveFav,FALSE);
		if((!ListView_GetItemCount(hWndFavList))&&(!GetHelpComStruct()->bmHpFileOpened))
		{	SetmHpTabItems(FALSE);
			TabCtrl_SetCurSel(hWndmHpTab,0);
			HideShowmHpCtrls(hWndActive);
		}
	}
}

//-----------------------------------------------------//
// WriteRegisterKeys
//-----------------------------------------------------//
void WriteRegisterKeys(int iOperation) 
{	SECURITY_ATTRIBUTES lpSecu;
	HKEY lpNewKey,lpAppKey,lpTmpKey,lpmHpKey;
	DWORD dWord=REG_OPENED_EXISTING_KEY;
	char lpIndex[MAX_KEY_SIZE]={0};
	char lpmHpPath[MAX_KEY_SIZE]={0};
	BYTE ByteTab[MAX_KEY_SIZE]={0};
	int i,iSize;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecu,bAclRes);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpmHpKey,&dWord)!=ERROR_SUCCESS) return;
	// mHelp
	if(RegCreateKeyEx(lpmHpKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpmHpKey,&dWord)!=ERROR_SUCCESS) return;
	// mHpShared
	if(RegCreateKeyEx(lpmHpKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpTmpKey,&dWord)!=ERROR_SUCCESS) return;
	if(!bLockOp)
	{	if(RegCreateKeyEx(lpmHpKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpNewKey,&dWord)!=ERROR_SUCCESS) return;
		iSize=mHpPak_FillDataFromNumber(ByteTab,iOperation,FALSE);
		RegSetValueEx(lpNewKey,READWRITE_KEY,0,REG_DWORD,ByteTab,iSize);	
		RegCloseKey(lpNewKey);
	}
	// mHpApp
	if(RegCreateKeyEx(lpmHpKey,MHPAPP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpmHpKey,&dWord)!=ERROR_SUCCESS) return;
	// AppName
	if(RegCreateKeyEx(lpmHpKey,lpExeName,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpAppKey,&dWord)!=ERROR_SUCCESS) return;
	/*if(RegCreateKeyEx(lpmHpKey,lpExeName,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpNewKey,&dWord)!=ERROR_SUCCESS) return;
	GetCurrentDirectory(MAX_KEY_SIZE,lpmHpPath);
	iSize=mHpPak_FillDataFromString(ByteTab,lpmHpPath);
	RegSetValueEx(lpNewKey,EXEPATH_KEY,0,REG_SZ,ByteTab,iSize);
	RegCloseKey(lpNewKey);*/
	if(ListView_GetItemCount(hWndmHpFilesList)==0)
	{	if(RegCreateKeyEx(lpAppKey,CURRENT_FILES_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpTmpKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		// Remove CurrentFile Key
		mHpPak_DeleteSubKey(lpAppKey,CURRENT_FILES_KEY);
		if(RegCreateKeyEx(lpAppKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpTmpKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		// Remove Settings Key
		mHpPak_DeleteSubKey(lpAppKey,SETTINGS_KEY);
	}
	else
	{	// CurrentFile
		if(RegCreateKeyEx(lpAppKey,CURRENT_FILES_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpTmpKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		mHpPak_DeleteSubKey(lpAppKey,CURRENT_FILES_KEY);
		if(RegCreateKeyEx(lpAppKey,CURRENT_FILES_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpTmpKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		for(i=0;i<ListView_GetItemCount(hWndmHpFilesList);i++)
		{	if(RegCreateKeyEx(lpAppKey,CURRENT_FILES_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
							&lpNewKey,&dWord)!=ERROR_SUCCESS)
			{	RemoveRegAppKey(lpExeName,lpmHpKey);
				return;
			}
			mHpPak_InitStr(lpIndex,MAX_KEY_SIZE);
			mHpPak_InitStr(lpmHpPath,MAX_KEY_SIZE);
			_snprintf(lpIndex,MAX_KEY_SIZE,"%d",i);
			ListView_GetItemText(hWndmHpFilesList,i,1,lpmHpPath,MAX_KEY_SIZE);
			iSize=mHpPak_FillDataFromString(ByteTab,lpmHpPath);
			RegSetValueEx(lpNewKey,lpIndex,0,REG_SZ,ByteTab,iSize);
			RegCloseKey(lpNewKey);
		}
		RegCloseKey(lpTmpKey);
		// Settings
		if(RegCreateKeyEx(lpAppKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpTmpKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		if(RegCreateKeyEx(lpAppKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpNewKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		if((!GetHelpComStruct()->iMode)||(GetHelpComStruct()->iMode==2)) iSize=mHpPak_FillDataFromNumber(ByteTab,NO,TRUE);
		else iSize=mHpPak_FillDataFromNumber(ByteTab,NO,FALSE);
		RegSetValueEx(lpNewKey,HELP_CHECK_KEY,0,REG_BINARY,ByteTab,iSize);
		RegCloseKey(lpNewKey);
		if(RegCreateKeyEx(lpAppKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpNewKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		if((!GetHelpComStruct()->iMode)||(GetHelpComStruct()->iMode==1)) iSize=mHpPak_FillDataFromNumber(ByteTab,NO,TRUE);
		else iSize=mHpPak_FillDataFromNumber(ByteTab,NO,FALSE);
		RegSetValueEx(lpNewKey,ASSIST_CHECK_KEY,0,REG_BINARY,ByteTab,iSize);
		RegCloseKey(lpNewKey);
		if(RegCreateKeyEx(lpAppKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpNewKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		iSize=mHpPak_FillDataFromNumber(ByteTab,GetHelpComStruct()->iNbBoucle,FALSE);
		RegSetValueEx(lpNewKey,LOOPOPTION_KEY,0,REG_DWORD,ByteTab,iSize);
		RegCloseKey(lpNewKey);
		if(RegCreateKeyEx(lpAppKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpNewKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		iSize=mHpPak_FillDataFromNumber(ByteTab,GetHelpComStruct()->MouseSpeed,FALSE);
		RegSetValueEx(lpNewKey,MOUSESPEED_KEY,0,REG_DWORD,ByteTab,iSize);
		RegCloseKey(lpNewKey);
		if(RegCreateKeyEx(lpAppKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpNewKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		iSize=mHpPak_FillDataFromNumber(ByteTab,GetHelpComStruct()->iHourDelay,FALSE);
		RegSetValueEx(lpNewKey,HOUR_DELAY_KEY,0,REG_DWORD,ByteTab,iSize);
		if(RegCreateKeyEx(lpAppKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpNewKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpExeName,lpmHpKey);
			return;
		}
		iSize=mHpPak_FillDataFromNumber(ByteTab,GetHelpComStruct()->iMinuteDelay,FALSE);
		RegSetValueEx(lpNewKey,MINUTE_DELAY_KEY,0,REG_DWORD,ByteTab,iSize);
		RegCloseKey(lpTmpKey);
	}
	WriteFavsRegKey(lpExeName,mHpPak_FavsList);
	RegCloseKey(lpAppKey);
}

//-----------------------------------------------------//
// SetRepeatView
//-----------------------------------------------------//
void SetRepeatView()
{	char lpLoop[100]={0};
	if(IsLoopOption())
	{	_snprintf(lpLoop,100,"%d",GetHelpComStruct()->iNbBoucle);
		SetWindowText(hWndRepeatCfg,lpLoop);
	}
	else
	{	LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
		SetWindowText(hWndRepeatCfg,lpRC);
	}
}

//-----------------------------------------------------//
// SetParamListView
//-----------------------------------------------------//
void SetParamListView()
{	char lpParamVal[MAX_RS_SIZE]={0};
	LV_ITEM lvitem;
	if((!IsLoopOption())&&(mHpPak_ItemsList->WhatIsIt(0,FALSE)!=0))
	{	for(int i=0;i<ListView_GetItemCount(hWndListRS);i++)
		{	lvitem.mask=LVIF_IMAGE;
			lvitem.iItem=i;
			lvitem.iSubItem=0;
			lvitem.iImage=3;
			mHpPak_InitStr(lpParamVal,MAX_RS_SIZE);
			ListView_GetItemText(hWndListRS,i,0,lpParamVal,MAX_RS_SIZE);
			mHpPak_ItemsList->ModifyStrParam(i,lpParamVal);
			ListView_SetItem(hWndListRS,&lvitem);
		}
	}
}

//-----------------------------------------------------//
// RefreshButton
//-----------------------------------------------------//
BOOL RefreshButton()
{	RECT Rect;
	GetHelpComStruct()->listBtn->EmptyButtonList();
	for(int i=ListView_GetTopIndex(hWndListRS);i<(ListView_GetTopIndex(hWndListRS)+ListView_GetCountPerPage(hWndListRS));i++)
	{	if((ListView_GetItemState(hWndListRS,i,LVIS_SELECTED)==LVIS_SELECTED)&&(!GetHelpComStruct()->listBtn->bExistButton(i)))
		{	ListView_GetItemRect(hWndListRS,i,&Rect,LVIR_BOUNDS);
			if(!GetHelpComStruct()->listBtn->AddButton(i,Rect.top,hWndListRS)) return FALSE;
		}
		else if((ListView_GetItemState(hWndListRS,i,LVIS_SELECTED)!=LVIS_SELECTED)&&(GetHelpComStruct()->listBtn->bExistButton(i)))
		{	GetHelpComStruct()->listBtn->RemoveButton(i);}
	}
	return TRUE;
}

//-----------------------------------------------------//
// UpdateSearchList
//-----------------------------------------------------//
BOOL UpdateSearchList(HWND hWndActive,char* lpSearch)
{	char IndexRS[MAX_RS_SIZE]={0};
	char NextIndexRS[MAX_RS_SIZE]={0};
	char lpEvent[MAX_RS_SIZE]={0};
	char lpRef[MAX_RS_SIZE]={0};
	char lpTmpRef[MAX_RS_SIZE]={0};
	char lpDescrit[MAX_RS_SIZE]={0};
	char lpTopic[MAX_RS_SIZE]={0};
	int RefTab[MAX_PART_TREE]={0};
	int j,iLineCnt,iPos,iParam,iNextPos,iNextParam,iRang,iTopic,indice=0;
	BOOL bContinue;
	ListBox_ResetContent(GetDlgItem(hWndActive,IDC_HIDE_SEARCHLIST));
	ListView_DeleteAllItems(hWndSearchList);
	lpSearch=_strupr(_strdup(lpSearch));
	for(iTopic=0;iTopic<ListView_GetItemCount(hWndmHpFilesList);iTopic++)
	{	LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_SEARCHTOPIC,lpRC,MAX_KEY_SIZE);
	    char lpmHpFilePath[MAX_RS_SIZE]={0};
		GetmHpFileName(hWndmHpFilesList,iTopic,lpmHpFilePath,MAX_RS_SIZE);
		mHpPak_GetTopicmHpFile(hWndActive,lpmHpFilePath,lpTopic,MAX_RS_SIZE);
		strncat(lpRC," \"",2);
		strncat(lpRC,lpTopic,mHpPak_FindStrLenght(lpTopic,MAX_RS_SIZE));
		strncat(lpRC,"\"...",4);
		SetWindowText(hWndStatus,lpRC);
		iLineCnt=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFilePath);
		mHpPak_EmptyTab(RefTab,MAX_PART_TREE);
		mHpPak_InitStr(lpEvent,MAX_RS_SIZE);
		strncpy(lpEvent,SEPARATED_EVENT,MAX_RS_SIZE);
		iPos=mHpPak_GetTaskLine(hWndActive,lpmHpFilePath,FALSE);
		if((iPos!=NO)&&(iPos!=1))
		{	for(j=0;j<iLineCnt;j++)
			{	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(j+1),FALSE)) return FALSE;
				mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,IndexRS,MAX_RS_SIZE,iPos,iParam);
				if(iPos==NO) return FALSE;
				if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(j+2),FALSE)) return FALSE;
				mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,NextIndexRS,MAX_RS_SIZE,iNextPos,iNextParam);
				if(iNextPos==NO) return FALSE;
				RefTab[iPos]++;
				RefTab[(iPos+1)]=NO;
				strncat(lpEvent,IndexRS,mHpPak_FindStrLenght(IndexRS,MAX_RS_SIZE));
				mHpPak_InitStr(lpRef,MAX_RS_SIZE);
				_snprintf(lpRef,MAX_RS_SIZE,"%d",iTopic);
				strncat(lpRef,SEPARATED_STRING,1);
				indice=0;
				while(RefTab[indice]!=NO)
				{	mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
					_snprintf(lpTmpRef,MAX_RS_SIZE,"%d",RefTab[indice]);
					strncat(lpRef,lpTmpRef,mHpPak_FindStrLenght(lpTmpRef,MAX_RS_SIZE));
					if(RefTab[(indice+1)]!=NO) strncat(lpRef,SEPARATED_STRING,1);
					indice++;
				}
				if(!iParam) // Single
				{	strncpy(IndexRS,_strupr(_strdup(IndexRS)),MAX_RS_SIZE);
					if(strstr(IndexRS,lpSearch)!=NULL)
					{	mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
						strncpy(lpTmpRef,lpSearch,MAX_RS_SIZE);
						strncat(lpTmpRef,SEPARATED_STRING,1);
						strncat(lpTmpRef,lpRef,mHpPak_FindStrLenght(lpRef,MAX_RS_SIZE));
						if(iNextPos>iPos)
						{	if(!iNextParam)
							{	ListBox_AddString(GetDlgItem(hWndActive,IDC_HIDE_SEARCHLIST),lpTmpRef);
								AddSearchList(iTopic,lpEvent,0);
							}
							else if(iNextParam==1)
							{	ListBox_AddString(GetDlgItem(hWndActive,IDC_HIDE_SEARCHLIST),lpTmpRef);
								AddSearchList(iTopic,lpEvent,2);
							}
						}
						else if((iNextPos<=iPos)||(iNextParam==1))
						{	ListBox_AddString(GetDlgItem(hWndActive,IDC_HIDE_SEARCHLIST),lpTmpRef);
							AddSearchList(iTopic,lpEvent,1);
						}
					}
				}
				else if(iParam==1) // Param
				{	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(j+1),FALSE)) return FALSE;
					mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,IndexRS,MAX_RS_SIZE,iPos,iParam);
					if(iPos==NO) return FALSE;
					bContinue=TRUE;
					iRang=0;
					while(bContinue)
					{	mHpPak_GetDetailParamRS(hWndActive,lpmHpFilePath,IndexRS,MAX_RS_SIZE,iNextParam,
												NextIndexRS,MAX_RS_SIZE,lpDescrit,MAX_RS_SIZE,bContinue);
						if(iNextParam==NO) return FALSE;
						ModifyParamRef(lpRef,MAX_RS_SIZE,iRang);
						strncpy(lpDescrit,_strupr(_strdup(lpDescrit)),MAX_RS_SIZE);
						if(strstr(lpDescrit,lpSearch)!=NULL)
						{	mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
							strncpy(lpTmpRef,lpSearch,MAX_RS_SIZE);
							strncat(lpTmpRef,SEPARATED_STRING,1);
							strncat(lpTmpRef,lpRef,mHpPak_FindStrLenght(lpRef,MAX_RS_SIZE));
							strncat(lpEvent,"{",1);
							strncat(lpEvent,lpDescrit,mHpPak_FindStrLenght(lpDescrit,MAX_RS_SIZE));
							strncat(lpEvent,"}",1);
							ListBox_AddString(GetDlgItem(hWndActive,IDC_HIDE_SEARCHLIST),lpTmpRef);
							AddSearchList(iTopic,lpEvent,3);
							if(bContinue) SetEventLag(lpEvent,MAX_RS_SIZE,iPos,iPos);
						}
						iRang++;
					}
				}
				SetEventLag(lpEvent,MAX_RS_SIZE,iPos,iNextPos);
				mHpPak_IncreaseProgressBar(hWndProgress,iLineCnt);
			}
		}
		SendMessage(hWndProgress,PBM_SETPOS,0,0);
		LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_READY,lpRC,MAX_KEY_SIZE);
		SetWindowText(hWndStatus,lpRC);
		mHpPak_ClosemHpFile();
	}
	return TRUE;
}

//-----------------------------------------------------//
// SetSelectedEvent
//-----------------------------------------------------//
void SetSelectedEvent(HWND hWndActive,int iFrom)
{	char lpRef[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LVITEM lvi;
	int Index=0,iSel,iTopic;
	switch(iFrom)
	{	case 3: // Search
		{	iSel=ListView_GetItemSel(hWndSearchList);
			ListBox_GetText(GetDlgItem(hWndActive,IDC_HIDE_SEARCHLIST),iSel,lpRef);
			break;
		}
		case 4: // Index
		{	iSel=ListView_GetItemSel(hWndIndexList);
			ListBox_GetText(GetDlgItem(hWndActive,IDC_HIDE_INDEXLIST),iSel,lpRef);
			break;
		}
		case 1:
		case 5: // Favorites
		{	iSel=ListView_GetItemSel(hWndFavList);
			iTopic=IsItAlreadyOpened(hWndmHpFilesList,mHpPak_FavsList->GetmHpPath(iSel),MAX_RS_SIZE);
			if(iTopic==NO)
			{	iTopic=ListView_GetItemCount(hWndmHpFilesList);
				if(mHpPak_IsFilePathExist(hWndActive,mHpPak_FavsList->GetmHpPath(iSel)))
				{	AddItemmHp(hWndActive,hWndmHpFilesList,mHpPak_FavsList->GetmHpPath(iSel),ListView_GetItemCount(hWndmHpFilesList));
					RefreshViewContent(hWndActive,iTopic);
				}
				else return;
			}
			strncpy(lpRef,SEPARATED_STRING,MAX_RS_SIZE);
			_snprintf(lpTmp,MAX_RS_SIZE,"%d",iTopic);
			strncat(lpRef,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
			strncat(lpRef,SEPARATED_STRING,1);
			strncat(lpRef,mHpPak_FavsList->GetEventRef(iSel),mHpPak_FindStrLenght(mHpPak_FavsList->GetEventRef(iSel),MAX_RS_SIZE));
			break;
		}
	}
	SetmHpCursor(hWndActive,1);
	imHpFileIndex=mHpPak_GetTopicFromRef(lpRef,0);
	mHpPak_EmptyTab(TabReplace,MAX_PART_TREE);
	while(mHpPak_GetTopicFromRef(lpRef,Index)!=NO)
	{	if(!Index) TabReplace[Index]=0;
		else TabReplace[Index]=mHpPak_GetTopicFromRef(lpRef,Index);
		Index++;	
	}
	TabReplace[Index]=NO;
	lvi.mask=LVIF_IMAGE;
	lvi.iItem=iSel;
	lvi.iSubItem=0;
	switch(iFrom)
	{	case 3: // Search
		{	ListView_GetItem(hWndSearchList,&lvi);
			break;
		}
		case 4: // Index
		{	ListView_GetItem(hWndIndexList,&lvi);
			break;
		}
		case 1:
		case 5: // Favorites
		{	ListView_GetItem(hWndFavList,&lvi);
			break;
		}
	}
	if(((iFrom==1)||(iFrom==5))&&((lvi.iImage==3)||(lvi.iImage==6)))
	{	TabReplace[Index]=0;
		TabReplace[(Index+1)]=NO;
	}
	ReplaceTree(hWndTreeRS,TabReplace);
	CopyTabReplace();
	mHpPak_ItemsList->EmptyList();
	SetChangeListView(hWndActive);
	if((iFrom==1)||(iFrom==5))
	{	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
		_snprintf(lpTmp,MAX_RS_SIZE,"%d",mHpPak_FavsList->GetLoopOption(iSel));
		SetWindowText(hWndBoucle,lpTmp);
		if(mHpPak_FavsList->GetLoopOption(iSel)>1)
		{	SetWindowText(hWndRepeatCfg,lpTmp);
			if(GetHelpComStruct()->iMode!=1) SendMessage(hWndmHelp,BM_CLICK,0,0);
		}
		else
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
			SetWindowText(hWndRepeatCfg,lpRC);
		}
	}
	if(((iFrom==1)||(iFrom==5))&&((lvi.iImage==3)||(lvi.iImage==6)))
	{	ReplaceEvoluValues(hWndActive,iSel);
		Index=0;
		if(mHpPak_FavsList->GetItemFromRang(iSel)->FavItem->ParamList!=NULL)
		{	while(Index!=mHpPak_FavsList->GetItemFromRang(iSel)->FavItem->ParamList->GetSelCount())
			{	ListView_SetItemState(hWndListRS,mHpPak_FavsList->GetItemFromRang(iSel)->FavItem->ParamList->GetSelItem(Index),
									  LVIS_SELECTED,LVIS_SELECTED);
				Index++;
			}
			EnableWindow(hWndStart,TRUE);
			EnableWindow(hWndDestroy,TRUE);
			EnableWindow(hWndAddFav,TRUE);
		}
	}
	else
	{	ListView_SetItemState(hWndListRS,mHpPak_GetIndex(TabReplace),LVIS_SELECTED,LVIS_SELECTED);
		UpdateEditRS();
		if((lvi.iImage==3)||(lvi.iImage==1))
		{	EnableWindow(hWndStart,TRUE);
			EnableWindow(hWndDestroy,TRUE);
			EnableWindow(hWndAddFav,TRUE);
		}
		else
		{	EnableWindow(hWndStart,FALSE);
			EnableWindow(hWndDestroy,FALSE);
			EnableWindow(hWndAddFav,FALSE);
		}
	}
	SetmHpCursor(hWndActive,0);
	TabCtrl_SetCurSel(hWndmHpTab,1);
	HideShowmHpCtrls(hWndActive);
}

//-----------------------------------------------------//
// ModifyParamRef
//-----------------------------------------------------//
void ModifyParamRef(char* StrRef,int RefSize,int Rang)
{	char lpRefTmp[MAX_RS_SIZE]={0};
	char lpRang[MAX_RS_SIZE]={0};
	int index=0;
	if(!Rang)
	{	while(StrRef[index]!=END_LIST) index++;
		index--;
		while(StrRef[index]!=TREE_PARAM_GROUP) index--;
		StrRef[index]=NEXT_RS;
	}
	else
	{	while(StrRef[index]!=NEXT_RS)
		{	lpRefTmp[index]=StrRef[index];
			index++;
		}
		lpRefTmp[index]=NEXT_RS;
		_snprintf(lpRang,MAX_RS_SIZE,"%d",Rang);
		strncat(lpRefTmp,lpRang,mHpPak_FindStrLenght(lpRang,MAX_RS_SIZE));
		strncpy(StrRef,lpRefTmp,RefSize);
	}
}

//-----------------------------------------------------//
// GetKeywordFromString
//-----------------------------------------------------//
void GetKeywordFromString(char* String,char* Keyword,int KeySize,int &StrIndice,BOOL &Next)
{	int indice1=0;
	Next=FALSE;
	mHpPak_InitStr(Keyword,KeySize);
	while(String[StrIndice]!=END_LIST)
	{	if(String[StrIndice]!=' ')
		{	Keyword[indice1]=String[StrIndice];
			indice1++;
		}
		if((String[(StrIndice+1)]==END_LIST)||(String[(StrIndice+1)]==' '))
		{	if((indice1!=0)&&((IsItAChar(Keyword[0]))||(indice1>1)))
			{	if(String[(StrIndice+1)]==' ') Next=TRUE;
				StrIndice++;
				return;
			}
		}
		StrIndice++;
	}
}

//-----------------------------------------------------//
// UpdateEditEvent
//-----------------------------------------------------//
void UpdateEditEvent(int TabIndex)
{	char lpEvent[MAX_RS_SIZE]={0};
	switch(TabIndex)
	{	case 3: // Search
		{	if(ListView_GetSelectedCount(hWndSearchList)!=0)
			{	ListView_GetItemText(hWndSearchList,ListView_GetItemSel(hWndSearchList),1,lpEvent,MAX_RS_SIZE);
				SetWindowText(hWndEditEvent,lpEvent);
			}
			else SetWindowText(hWndEditEvent,SEPARATED_EVENT);
			break;
		}
		case 4: // Index
		{	if(ListView_GetSelectedCount(hWndIndexList)!=0)
			{	ListView_GetItemText(hWndIndexList,ListView_GetItemSel(hWndIndexList),2,lpEvent,MAX_RS_SIZE);
				SetWindowText(hWndEditEvent,lpEvent);
			}
			else SetWindowText(hWndEditEvent,SEPARATED_EVENT);
			break;
		}
		case 5: // Favorites
		{	if(ListView_GetSelectedCount(hWndFavList)!=0)
			{	ListView_GetItemText(hWndFavList,ListView_GetItemSel(hWndFavList),1,lpEvent,MAX_RS_SIZE);
				SetWindowText(hWndEditEvent,lpEvent);
			}
			else SetWindowText(hWndEditEvent,SEPARATED_EVENT);
			break;
		}
	}
}
			
//-----------------------------------------------------//
// IsItAValidString
//-----------------------------------------------------//
BOOL IsItAValidString(char* lpString,char* lpValid,int ValidSize)
{	int indice=0,index=0;
	BOOL bChar=FALSE;
	mHpPak_InitStr(lpValid,ValidSize);
	while(lpString[indice]!=END_LIST)
	{	if(lpString[indice]!=' ') bChar=TRUE;
		if(bChar) 
		{	lpValid[index]=lpString[indice];
			index++;
		}
		indice++;
	}
	if((indice==0)||(!bChar)) return FALSE;
	return TRUE;
}

//-----------------------------------------------------//
// SetEventLag
//-----------------------------------------------------//
void SetEventLag(char* Event,int EventSize,int LastPos,int Pos)
{	char lpTmp[MAX_RS_SIZE]={0};
	int indice=0,iCount=0,iPos=Pos;
	if(LastPos>=Pos)
	{	while(Event[indice]!=END_LIST) indice++;
		iCount=indice-1;
		while(LastPos>=iPos)
		{	if(Event[iCount]==TREE_PARAM_GROUP) iPos++;
			iCount--;
		}
		mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
		for(indice=0;indice<(iCount+3);indice++) lpTmp[indice]=Event[indice];
		strncpy(Event,lpTmp,EventSize);
	}
	else strncat(Event,SEPARATED_EVENT,2);
}

//-----------------------------------------------------//
// UpdateIndexList
//-----------------------------------------------------//
BOOL UpdateIndexList(HWND hWndActive,int iTopic,BOOL bAddRemove)
{	//LVCOLUMN lvcol;
	char IndexRS[MAX_RS_SIZE]={0};
	char NextIndexRS[MAX_RS_SIZE]={0};
	char lpKeyword[MAX_RS_SIZE]={0};
	char lpEvent[MAX_RS_SIZE]={0};
	char lpRef[MAX_RS_SIZE]={0};
	char lpTmpRef[MAX_RS_SIZE]={0};
	char lpDescrit[MAX_RS_SIZE]={0};
	int RefTab[MAX_PART_TREE]={0};
	int j,iLineCnt,iPos,iParam,iNextPos,iNextParam,iRang,indice=0;
	BOOL bContinue,bNext;
	HWND hWndHideList=GetDlgItem(hWndActive,IDC_HIDE_INDEXLIST);
	LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_UPDATEINDEX,lpRC,MAX_KEY_SIZE);
	SetWindowText(hWndStatus,lpRC);
	if(bAddRemove)	
	{	char lpmHpFilePath[MAX_RS_SIZE]={0};
        GetmHpFileName(hWndmHpFilesList,iTopic,lpmHpFilePath,MAX_RS_SIZE);
		iLineCnt=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFilePath);
		mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
		mHpPak_EmptyTab(RefTab,MAX_PART_TREE);
		mHpPak_InitStr(lpEvent,MAX_RS_SIZE);
		strncpy(lpEvent,SEPARATED_EVENT,MAX_RS_SIZE);
		iPos=mHpPak_GetTaskLine(hWndActive,lpmHpFilePath,FALSE);
		mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
		if((iPos!=NO)&&(iPos!=1))
		{	for(j=0;j<iLineCnt;j++)
			{	ProceedMsgQueue(hWndActive);//,0);
				if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(j+1),FALSE)) return FALSE;
				mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
				mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,IndexRS,MAX_RS_SIZE,iPos,iParam);
				mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
				if(iPos==NO) return FALSE;
				if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(j+2),FALSE)) return FALSE;
				mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
				mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,NextIndexRS,MAX_RS_SIZE,iNextPos,iNextParam);
				mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
				if(iNextPos==NO) return FALSE;
				RefTab[iPos]++;
				RefTab[(iPos+1)]=NO;
				strncat(lpEvent,IndexRS,mHpPak_FindStrLenght(IndexRS,MAX_RS_SIZE));
				mHpPak_InitStr(lpRef,MAX_RS_SIZE);
				_snprintf(lpRef,MAX_RS_SIZE,"%d",iTopic);
				strncat(lpRef,SEPARATED_STRING,1);
				indice=0;
				while(RefTab[indice]!=NO)
				{	mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
					_snprintf(lpTmpRef,MAX_RS_SIZE,"%d",RefTab[indice]);
					strncat(lpRef,lpTmpRef,mHpPak_FindStrLenght(lpTmpRef,MAX_RS_SIZE));
					if(RefTab[(indice+1)]!=NO) strncat(lpRef,SEPARATED_STRING,1);
					indice++;
				}
				if(!iParam) // Single
				{	indice=0;
					bNext=TRUE;
					while(bNext)
					{	GetKeywordFromString(IndexRS,lpKeyword,MAX_RS_SIZE,indice,bNext);
						if(strncmp(lpKeyword,NULL_STRING,MAX_RS_SIZE)!=0)
						{	mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
							strncpy(lpTmpRef,lpKeyword,MAX_RS_SIZE);
							strncat(lpTmpRef,SEPARATED_STRING,1);
							strncat(lpTmpRef,lpRef,mHpPak_FindStrLenght(lpRef,MAX_RS_SIZE));
							if(iNextPos>iPos)
							{	if(!iNextParam)
								{	ListBox_AddString(hWndHideList,lpTmpRef);
									AddIndexList(iTopic,lpKeyword,lpEvent,0,ListBox_FindString(hWndHideList,0,lpTmpRef));
								}
								else if(iNextParam==1)
								{	ListBox_AddString(hWndHideList,lpTmpRef);
									AddIndexList(iTopic,lpKeyword,lpEvent,2,ListBox_FindString(hWndHideList,0,lpTmpRef));
								}
							}
							else if((iNextPos<=iPos)||(iNextParam==1))
							{	ListBox_AddString(hWndHideList,lpTmpRef);
								AddIndexList(iTopic,lpKeyword,lpEvent,1,ListBox_FindString(hWndHideList,0,lpTmpRef));
							}
						}
					}
				}
				else if(iParam==1) // Param
				{	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(j+1),FALSE)) return FALSE;
					mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
					mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,IndexRS,MAX_RS_SIZE,iPos,iParam);
					mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
					if(iPos==NO) return FALSE;
					bContinue=TRUE;
					iRang=0;
					while(bContinue)
					{	mHpPak_GetDetailParamRS(hWndActive,lpmHpFilePath,IndexRS,MAX_RS_SIZE,iNextParam,
												NextIndexRS,MAX_RS_SIZE,lpDescrit,MAX_RS_SIZE,bContinue);
						mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
						if(iNextParam==NO) return FALSE;
						ModifyParamRef(lpRef,MAX_RS_SIZE,iRang);
						indice=0;
						bNext=TRUE;
						while(bNext)
						{	GetKeywordFromString(lpDescrit,lpKeyword,MAX_RS_SIZE,indice,bNext);
							if(strncmp(lpKeyword,NULL_STRING,MAX_RS_SIZE)!=0)
							{	mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
								strncpy(lpTmpRef,lpKeyword,MAX_RS_SIZE);
								strncat(lpTmpRef,SEPARATED_STRING,1);
								strncat(lpTmpRef,lpRef,mHpPak_FindStrLenght(lpRef,MAX_RS_SIZE));
								strncat(lpEvent,"{",1);
								strncat(lpEvent,lpDescrit,mHpPak_FindStrLenght(lpDescrit,MAX_RS_SIZE));
								strncat(lpEvent,"}",1);
								ListBox_AddString(hWndHideList,lpTmpRef);
								AddIndexList(iTopic,lpKeyword,lpEvent,3,ListBox_FindString(hWndHideList,0,lpTmpRef));
								if(bNext) SetEventLag(lpEvent,MAX_RS_SIZE,iPos,iPos);
							}
						}
						iRang++;
					}
				}
				SetEventLag(lpEvent,MAX_RS_SIZE,iPos,iNextPos);
				mHpPak_IncreaseProgressBar(hWndProgress,iLineCnt);
			}
		}
		/*if(ListView_GetItemCount(hWndIndexList)>=INDEXLIST_ITEMINVIEW)
		{	lvcol.mask=LVCF_WIDTH;
			lvcol.cx=284;
			ListView_SetColumn(hWndIndexList,2,&lvcol);
		}*/
		mHpPak_ClosemHpFile();
	}
	else
	{	for(j=0;j<ListBox_GetCount(hWndHideList);j++)
		{	mHpPak_InitStr(lpTmpRef,MAX_RS_SIZE);
			ListBox_GetText(hWndHideList,j,lpTmpRef);
			if(mHpPak_GetTopicFromRef(lpTmpRef,0)==iTopic)
			{	ListBox_DeleteString(hWndHideList,j);
				ListView_DeleteItem(hWndIndexList,j);
				j--;
			}
		}
	}
	return TRUE;
}

//---------------------------------------------------------//
// ExpandNewTopicItem
//---------------------------------------------------------//
void ExpandNewTopicItem(int iTopic)
{	HTREEITEM hTmpItem=TreeView_GetRoot(hWndTreeRS);
	int indice=0;
	while(indice!=iTopic)
	{	hTmpItem=TreeView_GetNextItem(hWndTreeRS,hTmpItem,TVGN_NEXT);
		indice++;
	}
	TreeView_Expand(hWndTreeRS,hTmpItem,TVE_EXPAND);
}

//---------------------------------------------------------//
// SetmHpTabItems
//---------------------------------------------------------//
void SetmHpTabItems(BOOL bShowHide)
{	char lpTcItem[MAX_KEY_SIZE]={0};
	TCITEM TcItem;
	TcItem.mask=TCIF_TEXT;
	TcItem.cchTextMax=MAX_KEY_SIZE;
	TcItem.pszText=lpTcItem;
	if(bShowHide)
	{   if(ListView_GetItemCount(hWndFavList)!=0)
            TabCtrl_DeleteItem(hWndmHpTab,1); // Favorites
        TabCtrl_DeleteItem(hWndmHpTab,1); // Component
		TabCtrl_DeleteItem(hWndmHpTab,1); // About
		LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_CONTENTS,lpRC,MAX_KEY_SIZE);
		strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
		TabCtrl_InsertItem(hWndmHpTab,1,&TcItem);
		LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_CONFIG,lpRC,MAX_KEY_SIZE);
		strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
		TabCtrl_InsertItem(hWndmHpTab,2,&TcItem);
		LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_SEARCH,lpRC,MAX_KEY_SIZE);
		strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
		TabCtrl_InsertItem(hWndmHpTab,3,&TcItem);
		LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_INDEX,lpRC,MAX_KEY_SIZE);
		strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
		TabCtrl_InsertItem(hWndmHpTab,4,&TcItem);
		LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_FAVORITES,lpRC,MAX_KEY_SIZE);
		strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
		TabCtrl_InsertItem(hWndmHpTab,5,&TcItem);
        LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_COMPONENT,lpRC,MAX_KEY_SIZE);
		strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
        TabCtrl_InsertItem(hWndmHpTab,6,&TcItem);
        LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_ABOUT,lpRC,MAX_KEY_SIZE);
		strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
		TabCtrl_InsertItem(hWndmHpTab,7,&TcItem);
	}
	else
	{	TabCtrl_DeleteItem(hWndmHpTab,1); // Content
		TabCtrl_DeleteItem(hWndmHpTab,1); // Config
		TabCtrl_DeleteItem(hWndmHpTab,1); // Search
		TabCtrl_DeleteItem(hWndmHpTab,1); // Index
		TabCtrl_DeleteItem(hWndmHpTab,1); // Favorites
		TabCtrl_DeleteItem(hWndmHpTab,1); // Components
		TabCtrl_DeleteItem(hWndmHpTab,1); // About
		if(ListView_GetItemCount(hWndFavList)!=0)
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_FAVORITES,lpRC,MAX_KEY_SIZE);
			strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
			TabCtrl_InsertItem(hWndmHpTab,1,&TcItem);
			LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_COMPONENT,lpRC,MAX_KEY_SIZE);
			strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
			TabCtrl_InsertItem(hWndmHpTab,2,&TcItem);
			LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_ABOUT,lpRC,MAX_KEY_SIZE);
			strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
			TabCtrl_InsertItem(hWndmHpTab,3,&TcItem);
		}
		else
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_COMPONENT,lpRC,MAX_KEY_SIZE);
			strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
			TabCtrl_InsertItem(hWndmHpTab,1,&TcItem);
			LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_ABOUT,lpRC,MAX_KEY_SIZE);
			strncpy(TcItem.pszText,lpRC,MAX_KEY_SIZE);
			TabCtrl_InsertItem(hWndmHpTab,2,&TcItem);
		}
	}
}

//-----------------------------------------------------//
// AddFavoritesList
//-----------------------------------------------------//
void AddFavoritesList(char* lpmHpFilePath,char* lpEvent,int iLoop,int iImage)
{	char lpTmp[MAX_RS_SIZE]={0};
	char lpLvi[MAX_RS_SIZE]={0};
	int nbSep=0;
	LV_ITEM lvi;
	lvi.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvi.state=lvi.stateMask=0;
	lvi.iItem=ListView_GetItemCount(hWndFavList);
	lvi.iSubItem=0;
	lvi.cchTextMax=MAX_RS_SIZE;
	lvi.pszText=lpLvi;
	strncpy(lvi.pszText,lpmHpFilePath,MAX_RS_SIZE);
	lvi.iImage=iImage;
	ListView_InsertItem(hWndFavList,&lvi);
	lvi.iSubItem=1;
	strncpy(lvi.pszText,lpEvent,MAX_RS_SIZE);
	ListView_SetItem(hWndFavList,&lvi);
	lvi.iSubItem=2;
	if(iLoop!=NO)
	{	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
		_snprintf(lpTmp,MAX_RS_SIZE,"%d",iLoop);
		strncpy(lvi.pszText,lpTmp,MAX_RS_SIZE);
	}
	else
	{	LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
		strncpy(lvi.pszText,lpRC,MAX_RS_SIZE);
	}
	ListView_SetItem(hWndFavList,&lvi);
	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
	_snprintf(lpTmp,MAX_RS_SIZE,"%d",ListView_GetItemCount(hWndFavList));
	strncpy(lvi.pszText,lpTmp,MAX_RS_SIZE);
	lvi.iSubItem=3;
	ListView_SetItem(hWndFavList,&lvi);
}

//-----------------------------------------------------//
// AddSearchList
//-----------------------------------------------------//
void AddSearchList(int Topic,char* Event,int iImage)
{	char lpTopic[MAX_RS_SIZE]={0};
	char lpLvi[MAX_RS_SIZE]={0};
	LV_ITEM lvi;
	lvi.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvi.state=lvi.stateMask=0;
	lvi.iItem=ListView_GetItemCount(hWndSearchList);
	lvi.iSubItem=0;
	lvi.cchTextMax=MAX_RS_SIZE;
	lvi.pszText=lpLvi;
	ListView_GetItemText(hWndmHpFilesList,Topic,0,lpTopic,MAX_RS_SIZE);
	strncpy(lvi.pszText,lpTopic,MAX_RS_SIZE);
	lvi.iImage=iImage;
	ListView_InsertItem(hWndSearchList,&lvi);
	lvi.iSubItem=1;
	strncpy(lvi.pszText,Event,MAX_RS_SIZE);
	ListView_SetItem(hWndSearchList,&lvi);
}

//---------------------------------------------------------//
// AddIndexList
//---------------------------------------------------------//
void AddIndexList(int Topic,char* Keyword,char* Event,int iImage,int iRang)
{	char lpTopic[MAX_RS_SIZE]={0};
	char lpLvi[MAX_RS_SIZE]={0};
	LV_ITEM lvi;
	lvi.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvi.state=lvi.stateMask=0;
	lvi.iItem=iRang;
	lvi.iSubItem=0;
	lvi.cchTextMax=MAX_RS_SIZE;
	lvi.pszText=lpLvi;
	ListView_GetItemText(hWndmHpFilesList,Topic,0,lpTopic,MAX_RS_SIZE);
	strncpy(lvi.pszText,lpTopic,MAX_RS_SIZE);
	lvi.iImage=iImage;
	ListView_InsertItem(hWndIndexList,&lvi);
	lvi.iSubItem=1;
	strncpy(lvi.pszText,Keyword,MAX_RS_SIZE);
	ListView_SetItem(hWndIndexList,&lvi);
	lvi.iSubItem=2;
	strncpy(lvi.pszText,Event,MAX_RS_SIZE);
	ListView_SetItem(hWndIndexList,&lvi);
}

//---------------------------------------------------------//
// GetTopicFromTree
//---------------------------------------------------------//
int GetTopicFromTree()
{	HTREEITEM hSelTree=TreeView_GetSelection(hWndTreeRS);
	int iTopic=0;
	while(TreeView_GetParent(hWndTreeRS,hSelTree)!=NULL)
	{	hSelTree=TreeView_GetParent(hWndTreeRS,hSelTree);}
	while((hSelTree!=NULL)&&(hSelTree!=TreeView_GetRoot(hWndTreeRS)))
	{	hSelTree=TreeView_GetNextItem(hWndTreeRS,hSelTree,TVGN_PREVIOUS);
		iTopic++;
	}
	return iTopic;
}

//---------------------------------------------------------//
// HideFavCtrls
//---------------------------------------------------------//
void HideFavCtrls()
{	ShowWindow(hWndRemoveFav,SW_HIDE);
	ShowWindow(hWndAddFav,SW_HIDE);
	ShowWindow(hWndSetFav,SW_HIDE);
	ShowWindow(hWndStaticEvent,SW_HIDE);
	ShowWindow(hWndEditEvent,SW_HIDE);
	ShowWindow(hWndStaticAuto,SW_HIDE);
	ShowWindow(hWndEditAuto,SW_HIDE);
	ShowWindow(hWndResetAuto,SW_HIDE);
	ShowWindow(hWndCustomAuto,SW_HIDE);
	ShowWindow(hWndSaveFav,SW_HIDE);
}

//---------------------------------------------------------//
// HideShowFavCtrls
//---------------------------------------------------------//
void HideShowFavCtrls()
{	HideFavCtrls();
	if(SendMessage(hWndRadioFav,BM_GETCHECK,0,0)==BST_CHECKED)
	{	ShowWindow(hWndAddFav,SW_SHOW);
		ShowWindow(hWndRemoveFav,SW_SHOW);
		ShowWindow(hWndSetFav,SW_SHOW);
		MoveWindow(hWndStaticEvent,117,iDlgHeight-(407-265),63,16,TRUE);
        MoveWindow(hWndEditEvent,184,iDlgHeight-(407-263),iDlgWidth-(506-309),20,TRUE);
		ShowWindow(hWndStaticEvent,SW_SHOW);
		ShowWindow(hWndEditEvent,SW_SHOW);
		ShowWindow(hWndSaveFav,SW_SHOW);
	}
	else
	{	ShowWindow(hWndStaticAuto,SW_SHOW);
		ShowWindow(hWndEditAuto,SW_SHOW);
		ShowWindow(hWndResetAuto,SW_SHOW);
		ShowWindow(hWndCustomAuto,SW_SHOW);
	}
}

//---------------------------------------------------------//
// HidemHpCtrls
//---------------------------------------------------------//
void HidemHpCtrls()
{	ShowWindow(hWndStatus,SW_HIDE);
	ShowWindow(hWndStaticStatus,SW_HIDE);
	ShowWindow(hWndProgress,SW_HIDE);
	ShowWindow(hWndSetSearch,SW_HIDE);
	ShowWindow(hWndStaticSearch,SW_HIDE);
	ShowWindow(hWndStaticRepeat,SW_HIDE);
	ShowWindow(hWndStaticNumRepeat,SW_HIDE);
	ShowWindow(hWndStaticIndex,SW_HIDE);
	ShowWindow(hWndSearch,SW_HIDE);
	ShowWindow(hWndRemovemHpFiles,SW_HIDE);
	ShowWindow(hWndFrameMode,SW_HIDE);
	ShowWindow(hWndSearchList,SW_HIDE);
	ShowWindow(hWndIndexList,SW_HIDE);
	ShowWindow(hWndFavList,SW_HIDE);
	ShowWindow(hWndmHpFilesList,SW_HIDE);
	ShowWindow(hWndSetIndex,SW_HIDE);
	ShowWindow(hWndEditSearch,SW_HIDE);
	ShowWindow(hWndEditIndex,SW_HIDE);
	ShowWindow(hWndAddmHpFiles,SW_HIDE);
	ShowWindow(hWndStaticSlow,SW_HIDE);
	ShowWindow(hWndStaticFast,SW_HIDE);
	ShowWindow(hWndFramemHelp,SW_HIDE);
	ShowWindow(hWndmHelpIcon,SW_HIDE);
	ShowWindow(hWndFrameSpeed,SW_HIDE);
	ShowWindow(hWndFrameSepHory,SW_HIDE);
	ShowWindow(hWndFrameSepVert,SW_HIDE);
	ShowWindow(hWndAbout,SW_HIDE);
	ShowWindow(hWndmHelp,SW_HIDE);
	ShowWindow(hWndAssist,SW_HIDE);
	ShowWindow(hWndSpeed,SW_HIDE);
	ShowWindow(hWndIcon,SW_HIDE);
	ShowWindow(hWndTreeRS,SW_HIDE);
    ShowWindow(hWndTopicTree,SW_HIDE);
	ShowWindow(hWndListRS,SW_HIDE);
    ShowWindow(hWndWebClient,SW_HIDE);
	ShowWindow(hWndBoucle,SW_HIDE);		
	ShowWindow(hWndFrameFav,SW_HIDE);
	ShowWindow(hWndRadioFav,SW_HIDE);
	ShowWindow(hWndRadioAuto,SW_HIDE);
	ShowWindow(hWndOpenFavs,SW_HIDE);
	ShowWindow(hWndStaticDelay,SW_HIDE);
	ShowWindow(hWndDelayStatic,SW_HIDE);
	ShowWindow(hWndHour,SW_HIDE);
	ShowWindow(hWndMinute,SW_HIDE);
	ShowWindow(hWndPoints,SW_HIDE);
    ShowWindow(hWndCompList,SW_HIDE);
    ShowWindow(hWndCompLabel,SW_HIDE);
	HideFavCtrls();
}

//---------------------------------------------------------//
// HideShowmHpCtrls
//---------------------------------------------------------//
void HideShowmHpCtrls(HWND hWndActive)
{	int iSel=TabCtrl_GetCurSel(hWndmHpTab);
	// Hide
	HidemHpCtrls();
	// Show
	switch(iSel)
	{	case 0: // mHpFiles
		{	ShowWindow(hWndmHpFilesList,SW_SHOW);
			ShowWindow(hWndAddmHpFiles,SW_SHOW);
			ShowWindow(hWndRemovemHpFiles,SW_SHOW);
			ShowWindow(hWndStaticStatus,SW_SHOW);
			ShowWindow(hWndStatus,SW_SHOW);
			ShowWindow(hWndProgress,SW_SHOW);
			ShowWindow(hWndFrameSepHory,SW_SHOW);
			ShowWindow(hWndOpenFavs,SW_SHOW);
			MoveWindow(hWndFrameSepHory,9,iDlgHeight-(407-312),iDlgWidth-(506-484),10,TRUE);
			if(GetHelpComStruct()->bmHpFileOpened)
			{	LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_READY,lpRC,MAX_KEY_SIZE);}
			else LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_NOOPENEDFILE,lpRC,MAX_KEY_SIZE);
			SetWindowText(hWndStatus,lpRC);
			break;
		}
		case 1: 
		{	if(GetHelpComStruct()->bmHpFileOpened) // Contents
            {
#ifdef MHELP_OLD
                ShowWindow(hWndTreeRS,SW_SHOW);
                ShowWindow(hWndListRS,SW_SHOW);
#else
                ShowWindow(hWndTopicTree,SW_SHOW);
				ShowWindow(hWndWebClient,SW_SHOW);
#endif
				if((IsLoopOption())&&(mHpPak_ItemsList->WhatIsIt(0,FALSE)!=0)) RefreshButton();
				else GetHelpComStruct()->listBtn->EmptyButtonList();
			}
			else
			{	if(ListView_GetItemCount(hWndFavList)!=0) // Favorites
				{	ShowWindow(hWndFavList,SW_SHOW);
					ShowWindow(hWndFrameFav,SW_SHOW);
					ShowWindow(hWndRadioFav,SW_SHOW);
					ShowWindow(hWndRadioAuto,SW_SHOW);
					UpdateEditEvent(5);
					HideShowFavCtrls();
				}
				else // Component
				{   ShowWindow(hWndCompList,SW_SHOW);
                    ShowWindow(hWndCompLabel,SW_SHOW);
                }
			}
			break;
		}
		case 2:
		{	if(!GetHelpComStruct()->bmHpFileOpened)
			{   if(ListView_GetItemCount(hWndFavList)!=0) // Component
                {   ShowWindow(hWndCompList,SW_SHOW);
                    ShowWindow(hWndCompLabel,SW_SHOW);
                }
                else // About
			    {	ShowWindow(hWndFramemHelp,SW_SHOW);
				    ShowWindow(hWndmHelpIcon,SW_SHOW);
				    ShowWindow(hWndFrameSepVert,SW_SHOW);
				    ShowWindow(hWndAbout,SW_SHOW);
			    }
            }
			else // Configuration
			{	ShowWindow(hWndFrameMode,SW_SHOW);
				ShowWindow(hWndmHelp,SW_SHOW);
				ShowWindow(hWndAssist,SW_SHOW);
				ShowWindow(hWndFrameSpeed,SW_SHOW);
				ShowWindow(hWndSpeed,SW_SHOW);
				ShowWindow(hWndStaticSlow,SW_SHOW);
				ShowWindow(hWndStaticFast,SW_SHOW);
				ShowWindow(hWndIcon,SW_SHOW);
				ShowWindow(hWndStaticRepeat,SW_SHOW);
				ShowWindow(hWndStaticRepeat,SW_SHOW);
				ShowWindow(hWndBoucle,SW_SHOW);
				ShowWindow(hWndStaticNumRepeat,SW_SHOW);
				ShowWindow(hWndStaticDelay,SW_SHOW);
				ShowWindow(hWndDelayStatic,SW_SHOW);
				ShowWindow(hWndHour,SW_SHOW);
				ShowWindow(hWndMinute,SW_SHOW);
				ShowWindow(hWndPoints,SW_SHOW);
			}
			break;
		}
		case 3: // Search
		{   if(!GetHelpComStruct()->bmHpFileOpened) // About
			{	ShowWindow(hWndFramemHelp,SW_SHOW);
				ShowWindow(hWndmHelpIcon,SW_SHOW);
				ShowWindow(hWndFrameSepVert,SW_SHOW);
				ShowWindow(hWndAbout,SW_SHOW);
			}
            else
            {   ShowWindow(hWndFrameSepHory,SW_SHOW);
			    ShowWindow(hWndStaticSearch,SW_SHOW);
			    ShowWindow(hWndEditSearch,SW_SHOW);
			    ShowWindow(hWndSearch,SW_SHOW);
			    ShowWindow(hWndSearchList,SW_SHOW);
			    ShowWindow(hWndSetSearch,SW_SHOW);
			    ShowWindow(hWndStaticStatus,SW_SHOW);
			    ShowWindow(hWndStatus,SW_SHOW);
			    ShowWindow(hWndProgress,SW_SHOW);
			    UpdateEditEvent(3);
			    MoveWindow(hWndStaticEvent,9,iDlgHeight-(407-292),63,16,TRUE);
                MoveWindow(hWndEditEvent,75,iDlgHeight-(407-290),iDlgWidth-(506-320),20,TRUE);
			    ShowWindow(hWndStaticEvent,SW_SHOW);
			    ShowWindow(hWndEditEvent,SW_SHOW);
			    LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_READY,lpRC,MAX_KEY_SIZE);
			    SetWindowText(hWndStatus,lpRC);
            }
			break;
		}
		case 4: // Index
		{	ShowWindow(hWndStaticIndex,SW_SHOW);
			ShowWindow(hWndEditIndex,SW_SHOW);
			ShowWindow(hWndIndexList,SW_SHOW);
			ShowWindow(hWndSetIndex,SW_SHOW);
			UpdateEditEvent(4);
			MoveWindow(hWndStaticEvent,9,iDlgHeight-(407-345),63,16,TRUE);
            MoveWindow(hWndEditEvent,75,iDlgHeight-(407-342),iDlgWidth-(506-320),20,TRUE);
			ShowWindow(hWndStaticEvent,SW_SHOW);
			ShowWindow(hWndEditEvent,SW_SHOW);
			break;
		}
		case 5: // Favorites
		{   ShowWindow(hWndFavList,SW_SHOW);
			ShowWindow(hWndFrameFav,SW_SHOW);
			ShowWindow(hWndRadioFav,SW_SHOW);
			ShowWindow(hWndRadioAuto,SW_SHOW);
			UpdateEditEvent(5);
			HideShowFavCtrls();
			break;
		}
		case 6: // Component
		{   ShowWindow(hWndCompList,SW_SHOW);
            ShowWindow(hWndCompLabel,SW_SHOW);
            break;
		}
		case 7: // About
		{	ShowWindow(hWndFramemHelp,SW_SHOW);
			ShowWindow(hWndmHelpIcon,SW_SHOW);
			ShowWindow(hWndFrameSepVert,SW_SHOW);
			ShowWindow(hWndAbout,SW_SHOW);
			break;
		}
	}
}

//---------------------------------------------------------//
// UpdateEditRS
//---------------------------------------------------------//
void UpdateEditRS()
{	TVITEM Tvitem;
	HTREEITEM hItemTmp=TreeView_GetSelection(hWndTreeRS);
	char lpTemp1[MAX_RS_SIZE]={0};
	char lpTemp2[MAX_RS_SIZE]={0};
	char lpStock[MAX_RS_SIZE]={0};
	char lptvitm[MAX_RS_SIZE]={0};
	Tvitem.mask=TVIF_TEXT;
	Tvitem.cchTextMax=MAX_RS_SIZE;
	Tvitem.pszText=lptvitm;
	strncpy(lpStock,SEPARATED_EVENT,MAX_RS_SIZE);
	if(hItemTmp!=NULL)
	{	while(TreeView_GetParent(hWndTreeRS,hItemTmp)!=NULL)
		{	Tvitem.hItem=hItemTmp;
			Tvitem.mask=TVIF_TEXT;
			Tvitem.cchTextMax=MAX_RS_SIZE;
			if(TreeView_GetItem(hWndTreeRS,&Tvitem))
			{	strncpy(lpTemp1,lpStock,MAX_RS_SIZE);
				strncpy(lpTemp2,SEPARATED_EVENT,MAX_RS_SIZE);
				strncat(lpTemp2,Tvitem.pszText,mHpPak_FindStrLenght(Tvitem.pszText,MAX_RS_SIZE));
				strncpy(lpStock,lpTemp2,MAX_RS_SIZE);
				strncat(lpStock,lpTemp1,mHpPak_FindStrLenght(lpTemp1,MAX_RS_SIZE));
			}
			hItemTmp=TreeView_GetParent(hWndTreeRS,hItemTmp);
		}
		Tvitem.hItem=hItemTmp;
		Tvitem.mask=TVIF_TEXT;
		Tvitem.cchTextMax=MAX_RS_SIZE;
		if(TreeView_GetItem(hWndTreeRS,&Tvitem))
		{	strncpy(lpTemp1,lpStock,MAX_RS_SIZE);
			strncpy(lpTemp2,SEPARATED_EVENT,MAX_RS_SIZE);
			strncat(lpTemp2,Tvitem.pszText,mHpPak_FindStrLenght(Tvitem.pszText,MAX_RS_SIZE));
			strncpy(lpStock,lpTemp2,MAX_RS_SIZE);
			strncat(lpStock,lpTemp1,mHpPak_FindStrLenght(lpTemp1,MAX_RS_SIZE));
		}
	}
	mHpPak_InitStr(lpTemp1,MAX_RS_SIZE);
	if((hItemTmp!=NULL)&&(ListView_GetNextItem(hWndListRS,NO,LVNI_SELECTED)!=NO))
	{	if((GetWindowLongPtr(hWndListRS,GWL_STYLE)&LVS_REPORT)==LVS_REPORT)
		{	// Params
			if(ListView_GetNextItem(hWndListRS,NO,LVNI_SELECTED)!=NO) strncat(lpStock,"...",3);
		}
		else
		{	// Dossiers
			ListView_GetItemText(hWndListRS,ListView_GetNextItem(hWndListRS,NO,LVNI_SELECTED),
								 0,lpTemp1,MAX_RS_SIZE);
			strncat(lpStock,lpTemp1,mHpPak_FindStrLenght(lpTemp1,MAX_RS_SIZE));
		}
	}
	SetWindowText(hWndEditRS,lpStock);
}

//-----------------------------------------------------//
// ShowCtrl
//-----------------------------------------------------//
void ShowCtrl(int iType)
{	ShowWindow(hWndStaticSel,SW_HIDE);
	switch(iType)
	{	case 0:
		{	ShowWindow(hWndCopier,SW_HIDE);
			ShowWindow(hWndColler,SW_HIDE);
			ShowWindow(hWndAssoc,SW_SHOW);
			ShowWindow(hWndValAssoc,SW_SHOW);
			ShowWindow(hWndApply,SW_SHOW);
			ShowWindow(hWndExt,SW_HIDE);
			MoveWindow(hWndApply,159,107,119,23,TRUE);
			break;
		}
		case 1:
		{	ShowWindow(hWndCopier,SW_SHOW);
			ShowWindow(hWndColler,SW_SHOW);
			ShowWindow(hWndAssoc,SW_HIDE);
			ShowWindow(hWndValAssoc,SW_HIDE);
			ShowWindow(hWndApply,SW_HIDE);
			ShowWindow(hWndExt,SW_HIDE);
			break;
		}
		case 2:
		{	ShowWindow(hWndCopier,SW_HIDE);
			ShowWindow(hWndColler,SW_HIDE);
			ShowWindow(hWndAssoc,SW_SHOW);
			ShowWindow(hWndValAssoc,SW_SHOW);
			ShowWindow(hWndApply,SW_SHOW);
			ShowWindow(hWndExt,SW_SHOW);
			MoveWindow(hWndApply,206,107,72,23,TRUE);
			break;
		}
	}
}

//-----------------------------------------------------//
// MoveDlgCtrl
//-----------------------------------------------------//
void MoveDlgCtrl(HWND hWndActive)
{
    HDWP hMultiWndPos;
    if(hWndWebClient)
        hMultiWndPos=BeginDeferWindowPos(43);
    else
        hMultiWndPos=BeginDeferWindowPos(42);

    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndClose,NULL,iDlgWidth-(506-423),iDlgHeight-(407-376),78,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndStart,NULL,iDlgWidth-(506-339),iDlgHeight-(407-376),78,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndAdd,NULL,iDlgWidth-(506-255),iDlgHeight-(407-376),78,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndDestroy,NULL,iDlgWidth-(506-170),iDlgHeight-(407-376),78,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndHelp,NULL,4,iDlgHeight-(407-376),70,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,GetDlgItem(hWndActive,IDC_STATIC_REPEATITION),NULL,79,iDlgHeight-(407-375),60,16,SWP_NOZORDER|SWP_NOACTIVATE);
	hMultiWndPos=DeferWindowPos(hMultiWndPos,GetDlgItem(hWndActive,IDC_STATIC_AUTOASSIST),NULL,79,iDlgHeight-(407-389),60,16,SWP_NOZORDER|SWP_NOACTIVATE);
	hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndRepeatCfg,NULL,135,iDlgHeight-(407-375),30,16,SWP_NOZORDER|SWP_NOACTIVATE);
	hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndAutoAssistCfg,NULL,135,iDlgHeight-(407-389),30,16,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndmHpTab,NULL,3,3,iDlgWidth-(506-498),iDlgHeight-(407-369),SWP_NOZORDER|SWP_NOACTIVATE);
    //hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndEditRS,NULL,9,iDlgHeight-(407-340),iDlgWidth-(506-484),21,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndFrameSepHory,NULL,9,iDlgHeight-(407-312),iDlgWidth-(506-484),10,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndStaticStatus,NULL,9,iDlgHeight-(407-326),41,13,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndStatus,NULL,50,iDlgHeight-(407-326),iDlgWidth-(506-444),13,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndAddmHpFiles,NULL,9,iDlgHeight-(407-287),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndRemovemHpFiles,NULL,193,iDlgHeight-(407-287),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndOpenFavs,NULL,285,iDlgHeight-(407-287),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndmHpFilesList,NULL,9,32,iDlgWidth-(506-485),iDlgHeight-(407-251),SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndProgress,NULL,9,iDlgHeight-(407-342),iDlgWidth-(506-484),22,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndCompList,NULL,10,44,iDlgWidth-(506-483),iDlgHeight-(407-320),SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndFramemHelp,NULL,9,26,iDlgWidth-(506-484),iDlgHeight-(407-338),SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndFrameSepVert,NULL,65,40,5,iDlgHeight-(407-316),SWP_NOZORDER|SWP_NOACTIVATE);
#ifdef MHELP_OLD
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndTreeRS,NULL,9,31,194,iDlgHeight-(407-333),SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndListRS,NULL,208,31,iDlgWidth-(506-285),iDlgHeight-(407-333),SWP_NOZORDER|SWP_NOACTIVATE);
#else
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndTopicTree,NULL,9,31,204,iDlgHeight-(407-334),SWP_NOZORDER|SWP_NOACTIVATE);
    if(hWndWebClient)
        hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndWebClient,NULL,218,31,iDlgWidth-(506-276),iDlgHeight-(407-334),SWP_NOZORDER|SWP_NOACTIVATE);
#endif
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndEditSearch,NULL,9,43,iDlgWidth-(506-484),20,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndSearch,NULL,iDlgWidth-(506-407),67,87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndSearchList,NULL,9,99,iDlgWidth-(506-484),iDlgHeight-(407-184),SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndSetSearch,NULL,iDlgWidth-(506-407),iDlgHeight-(407-287),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    switch(TabCtrl_GetCurSel(hWndmHpTab))
    {   case 3: // Search
	    {	hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndStaticEvent,NULL,9,iDlgHeight-(407-345),63,16,SWP_NOZORDER|SWP_NOACTIVATE);
            hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndEditEvent,NULL,75,iDlgHeight-(407-342),iDlgWidth-(506-320),20,SWP_NOZORDER|SWP_NOACTIVATE);
            break;
        }
        case 4: // Index
		{	hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndStaticEvent,NULL,9,iDlgHeight-(407-345),63,16,SWP_NOZORDER|SWP_NOACTIVATE);
            hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndEditEvent,NULL,75,iDlgHeight-(407-342),iDlgWidth-(506-320),20,SWP_NOZORDER|SWP_NOACTIVATE);
			break;
		}
		case 5: // Favorites
        default: // Always move even if not needed (according 'BeginDeferWindowPos' window count)
	    {	hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndStaticEvent,NULL,117,iDlgHeight-(407-265),63,16,SWP_NOZORDER|SWP_NOACTIVATE);
            hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndEditEvent,NULL,184,iDlgHeight-(407-263),iDlgWidth-(506-309),20,SWP_NOZORDER|SWP_NOACTIVATE);
            break;
        }
    }
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndEditIndex,NULL,9,43,iDlgWidth-(506-484),20,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndIndexList,NULL,9,68,iDlgWidth-(506-484),iDlgHeight-(407-264),SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndSetIndex,NULL,iDlgWidth-(506-407),iDlgHeight-(407-338),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndAddFav,NULL,118,iDlgHeight-(407-288),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndRemoveFav,NULL,210,iDlgHeight-(407-288),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndSaveFav,NULL,302,iDlgHeight-(407-288),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndSetFav,NULL,iDlgWidth-(506-407),iDlgHeight-(407-288),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndFrameFav,NULL,108,iDlgHeight-(407-255),5,62,SWP_NOZORDER|SWP_NOACTIVATE);
	hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndRadioFav,NULL,9,iDlgHeight-(407-262),64,16,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndRadioAuto,NULL,9,iDlgHeight-(407-280),96,16,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndFavList,NULL,9,31,iDlgWidth-(506-484),iDlgHeight-(407-227),SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndStaticAuto,NULL,117,iDlgHeight-(407-265),107,16,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndEditAuto,NULL,226,iDlgHeight-(407-263),iDlgWidth-(506-267),20,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndResetAuto,NULL,iDlgWidth-(506-314),iDlgHeight-(407-288),87,26,SWP_NOZORDER|SWP_NOACTIVATE);
    hMultiWndPos=DeferWindowPos(hMultiWndPos,hWndCustomAuto,NULL,iDlgWidth-(506-406),iDlgHeight-(407-288),87,26,SWP_NOZORDER|SWP_NOACTIVATE);

    EndDeferWindowPos(hMultiWndPos);
}

//-----------------------------------------------------//
// IsLoopOption
//-----------------------------------------------------//
BOOL IsLoopOption()
{	if(IsWindowEnabled(hWndBoucle))
	{	char lpTemp[10]={0};
		GetHelpComStruct()->iNbBoucle=0;
		GetWindowText(hWndBoucle,lpTemp,10);
		// Test paramètre de répétition
		GetHelpComStruct()->iNbBoucle=mHpPak_GetIntFromStr(lpTemp,10,0);
		if(GetHelpComStruct()->iNbBoucle>1) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------//
// ReplaceDelayCfg
//-----------------------------------------------------//
void ReplaceDelayCfg()
{	char lpTmp[100]={0};
	int iMinute,iHour;
	GetWindowText(hWndMinute,lpTmp,100);
	if(strncmp(lpTmp,NULL_STRING,100)!=0) iMinute=mHpPak_GetIntFromStr(lpTmp,100,0);
	else iMinute=0;
	mHpPak_InitStr(lpTmp,100);
	GetWindowText(hWndHour,lpTmp,100);
	if(strncmp(lpTmp,NULL_STRING,100)!=0) iHour=mHpPak_GetIntFromStr(lpTmp,100,0);
	else iHour=0;
	if((!iHour)&&(!iMinute))
	{	SetWindowText(hWndMinute,"1");
		GetHelpComStruct()->iMinuteDelay=1;
	}
	else if(iMinute>59)
	{	mHpPak_InitStr(lpTmp,100);
		if(iHour<999)
		{	_snprintf(lpTmp,100,"%d",(iMinute-59));
			GetHelpComStruct()->iMinuteDelay=(iMinute-59);
			iHour++;
		}
		else
		{	_snprintf(lpTmp,100,"%d",59);
			GetHelpComStruct()->iMinuteDelay=59;
		}
		SetWindowText(hWndMinute,lpTmp);
	}
	else GetHelpComStruct()->iMinuteDelay=iMinute;
	GetHelpComStruct()->iHourDelay=iHour;
	mHpPak_InitStr(lpTmp,100);
	_snprintf(lpTmp,100,"%d",iHour);
	SetWindowText(hWndHour,lpTmp);
}

//-----------------------------------------------------//
// GetExeName
//-----------------------------------------------------//
void GetExeName(char* ExeName)
{	mHpPak_InitStr(lpExeName,MAX_KEY_SIZE);
	strncpy(lpExeName,ExeName,MAX_KEY_SIZE);
}

//-----------------------------------------------------//
// GetCurrentmHpFile
//-----------------------------------------------------//
char* GetCurrentmHpFile()
{	if(mHpPak_bAutoAss) return mHpPak_FavsList->GetmHpPath(mHpPak_iFavIndex);
	else if(mHpPak_bLnchProc) return mHpPak_LnchFavLst->GetmHpPath(mHpPak_iFavIndex);
	else if(mHpPak_bLnchCustom) return mHpPak_CustomFavLst->GetmHpPath(mHpPak_iFavIndex);
	else
	{	if(GetHelpComStruct()->mHpFileList->IsmHpFileExist(imHpFileIndex))
		{	return GetHelpComStruct()->mHpFileList->GetmHpFilePath(imHpFileIndex);}
	}
	return NULL;	
}

//-----------------------------------------------------//
// SetLastIndex
//-----------------------------------------------------//
void SetLastIndex()
{	int indice=0;
	while(GetHelpComStruct()->IndexTab[indice]!=NO) indice++;
	if(indice!=0)
	{	GetHelpComStruct()->IndexTab[indice]=NULL;
		GetHelpComStruct()->IndexTab[(indice-1)]=NO;
	}
}

//-----------------------------------------------------//
// CopyIndexTab
//-----------------------------------------------------//
void CopyIndexTab(int* lpDestTab,int iMaxDest)
{	int indice=0;
	mHpPak_EmptyTab(lpDestTab,iMaxDest);
	while(GetHelpComStruct()->IndexTab[indice]!=NO)
	{	lpDestTab[(indice+1)]=GetHelpComStruct()->IndexTab[indice];
		indice++;
	}
	lpDestTab[(indice+1)]=NO;
	lpDestTab[0]=0;
}

//-----------------------------------------------------//
// CopyTabReplace
//-----------------------------------------------------//
void CopyTabReplace()
{	int indice=0;
	mHpPak_EmptyTab(GetHelpComStruct()->IndexTab,MAX_PART_TREE);
	while(TabReplace[(indice+1)]!=NO)
	{	GetHelpComStruct()->IndexTab[indice]=TabReplace[(indice+1)];
		indice++;
	}
	GetHelpComStruct()->IndexTab[(indice-1)]=NO;
}

//-----------------------------------------------------//
// ChangeSelectedParam
//-----------------------------------------------------//
void ChangeSelectedParam()
{	int indice=0;
	int itemp=ListView_GetNextItem(hWndListRS,NO,LVNI_SELECTED);
	if((itemp==NO)&&(mHpPak_ItemsList->WhatIsIt(0,FALSE))) mHpPak_ItemsList->SelectItem(0);
	else while(itemp!=NO)
	{	mHpPak_ItemsList->SelectItem(itemp);
		itemp=ListView_GetNextItem(hWndListRS,indice,LVNI_SELECTED);
		indice++;
	}
}

//-----------------------------------------------------//
// ChangeStyle
//-----------------------------------------------------//
void ChangeStyle(BOOL bType)
{	int dwStyle;
	dwStyle=(int)GetWindowLongPtr(hWndListRS,GWL_STYLE);
	dwStyle&=~(LVS_TYPEMASK);
	if(bType)
	{	dwStyle|=LVS_REPORT;
		dwStyle|=LVS_EDITLABELS;
		dwStyle&=~(LVS_SINGLESEL);
	}
	else
	{	dwStyle|=LVS_SMALLICON;
		dwStyle|=LVS_SINGLESEL;
		dwStyle&=~(LVS_EDITLABELS);
	}
	SetWindowLongPtr(hWndListRS,GWL_STYLE,(LONG_PTR)dwStyle);
	if(bType) SetHeader(FALSE);
}

//-----------------------------------------------------//
// GetDispInfo
//-----------------------------------------------------//
void GetDispInfo(LV_DISPINFO * DisplayInfo)
{	char *S;
	int i = DisplayInfo->item.iItem;
	int j = DisplayInfo->item.iSubItem;
	if(j == 0)
	{	S=LPSTR_TEXTCALLBACK;
		DisplayInfo->item.pszText = S;
	}
	else
	{	if(!mHpPak_ItemsList->WhatIsIt(i,FALSE)) S=NULL_STRING;
		else S=mHpPak_ItemsList->GetDetail(i,j,0,lpRC,MAX_KEY_SIZE);
	}
	DisplayInfo->item.pszText = S;
}

//-----------------------------------------------------//
// SetHeader
//-----------------------------------------------------//
void SetHeader(BOOL bAllListRS)
{	LV_COLUMN lvc;
	lvc.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvc.fmt=LVCFMT_LEFT;
	if(!bAllListRS)
	{	if(!bHeaderDid)
		{	bHeaderDid=TRUE;
			// Add the columns.
			for(int i=0;i<NUMHEADERS;i++)
			{	lvc.iSubItem=i;
				LoadString(GetHelpComStruct()->hInstServ,RSHeaders[i],lpRC,MAX_KEY_SIZE);
				lvc.pszText=lpRC;
				switch(i)
				{	case 0: lvc.cx=COLUMN_WIDTH_PARAM;break;
					case 1: lvc.cx=COLUMN_WIDTH_TYPE;break;
					case 2: lvc.cx=COLUMN_WIDTH_DESC;break;
					case 3: lvc.cx=COLUMN_WIDTH_LIMIT;break;
				}
				ListView_InsertColumn(hWndListRS,i,&lvc);
			}
		}
	}
	else
	{	lvc.iSubItem=0;
		LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_TOPIC,lpRC,MAX_KEY_SIZE);
		lvc.pszText=lpRC;
		lvc.cx=150;
		ListView_InsertColumn(hWndmHpFilesList,0,&lvc);
		lvc.cx=90;
		ListView_InsertColumn(hWndSearchList,0,&lvc);
		ListView_InsertColumn(hWndIndexList,0,&lvc);
		LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_FILEPATH,lpRC,MAX_KEY_SIZE);
		lvc.pszText=lpRC;
		lvc.cx=120;
		ListView_InsertColumn(hWndFavList,0,&lvc);
		lvc.iSubItem=1;
		lvc.cx=331;
		ListView_InsertColumn(hWndmHpFilesList,1,&lvc);
		lvc.cx=390;
		LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_EVENT,lpRC,MAX_KEY_SIZE);
		lvc.pszText=lpRC;
		ListView_InsertColumn(hWndSearchList,1,&lvc);
		lvc.cx=252;
		ListView_InsertColumn(hWndFavList,1,&lvc);
		LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_REPEATITION,lpRC,MAX_KEY_SIZE);
		lvc.pszText=lpRC;
		lvc.cx=70;
		ListView_InsertColumn(hWndFavList,2,&lvc);
		LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_ROW,lpRC,MAX_KEY_SIZE);
		lvc.pszText=lpRC;
		lvc.cx=38;
		ListView_InsertColumn(hWndFavList,3,&lvc);
		LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_KEYWORD,lpRC,MAX_KEY_SIZE);
		lvc.pszText=lpRC;
		lvc.cx=90;
		ListView_InsertColumn(hWndIndexList,1,&lvc);
		lvc.iSubItem=2;
		LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_EVENT,lpRC,MAX_KEY_SIZE);
		lvc.pszText=lpRC;
		lvc.cx=300;
		ListView_InsertColumn(hWndIndexList,2,&lvc);
	}
}

//-----------------------------------------------------//
// SetCompListHeader
//-----------------------------------------------------//
void SetCompListHeader()
{	LV_COLUMN lvc;
	lvc.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvc.fmt=LVCFMT_LEFT;
    lvc.cx=142;
	lvc.iSubItem=0;
	LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_CTRLTYPE,lpRC,MAX_KEY_SIZE);
	lvc.pszText=lpRC;
	ListView_InsertColumn(hWndCompList,0,&lvc);
	lvc.iSubItem=1;
	lvc.cx=116;
	LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_PLATFORM,lpRC,MAX_KEY_SIZE);
	lvc.pszText=lpRC;
	ListView_InsertColumn(hWndCompList,1,&lvc);
	lvc.iSubItem=2;
    lvc.cx=259;
	LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_CTRLCLASS,lpRC,MAX_KEY_SIZE);
	lvc.pszText=lpRC;
	ListView_InsertColumn(hWndCompList,2,&lvc);
	lvc.iSubItem=3;
    lvc.cx=143;
	LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_DLLNAME,lpRC,MAX_KEY_SIZE);
	lvc.pszText=lpRC;
	ListView_InsertColumn(hWndCompList,3,&lvc);
	lvc.iSubItem=4;
    lvc.cx=52;
	LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_DLLVERS,lpRC,MAX_KEY_SIZE);
	lvc.pszText=lpRC;
	ListView_InsertColumn(hWndCompList,4,&lvc);
	lvc.iSubItem=5;
	lvc.cx=219;
	LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_DLLSTATUS,lpRC,MAX_KEY_SIZE);
	lvc.pszText=lpRC;
	ListView_InsertColumn(hWndCompList,5,&lvc);
}

//-----------------------------------------------------//
// AddItemList
//-----------------------------------------------------//
void AddItemList(HWND hWndCtrl,LPSTR lpstr,int IconIndex,int Rang)
{	LV_ITEM lvi;
	lvi.mask  = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.state = lvi.stateMask = 0;
	lvi.iItem       = Rang;
	lvi.iSubItem    = 0;
	lvi.pszText     = lpstr;
	lvi.cchTextMax  = 0;
	lvi.iImage      = IconIndex;
	ListView_InsertItem(hWndCtrl,&lvi);
}

//-----------------------------------------------------//
// ReplaceTree
//-----------------------------------------------------//
void ReplaceTree(HWND hWndTreeView,int* lpIndexTab)
{	int indice=0;
	int iTemp=0;
	HTREEITEM hItemB,hItemA;
	hItemA=TreeView_GetNextItem(hWndTreeView,NULL,TVGN_ROOT);
	while(iTemp!=imHpFileIndex)
	{	hItemA=TreeView_GetNextItem(hWndTreeView,hItemA,TVGN_NEXT);
		iTemp++;
	}
	hItemB=hItemA;
	if(lpIndexTab[indice]==NO)
	{	TreeView_Expand(hWndTreeView,hItemA,TVE_EXPAND);
		TreeView_SelectItem(hWndTreeView,hItemA);
	}
	else while(lpIndexTab[(indice+1)]!=NO)
	{	iTemp=lpIndexTab[indice];
		while((iTemp!=0)&&(hItemA!=NULL))
		{	hItemB=TreeView_GetNextItem(hWndTreeView,hItemA,TVGN_NEXT);
			hItemA=hItemB;
			iTemp--;
		}
		TreeView_Expand(hWndTreeView,hItemB,TVE_EXPAND);
		if(lpIndexTab[(indice+2)]==NO) TreeView_SelectItem(hWndTreeView,hItemB);
		hItemA=TreeView_GetNextItem(hWndTreeView,hItemB,TVGN_CHILD);
		hItemB=hItemA;
		indice++;
	}
}

//-----------------------------------------------------//
// GetAllParamDetail
//-----------------------------------------------------//
BOOL GetAllParamDetail(HWND hWndActive,int iTopic)
{	CItem *Newitem;
	char StrDescription[MAX_RS_SIZE]={0};
	char Strlimit[MAX_RS_SIZE]={0};
	char ParamRS[MAX_RS_SIZE]={0};
	int type,iRang=0;
	BOOL bContinue=TRUE;		
	while(bContinue)
	{	char lpmHpFile[MAX_RS_SIZE]={0};
        mHpPak_GetDetailParamRS(hWndActive,GetmHpFileName(hWndmHpFilesList,iTopic,lpmHpFile,MAX_RS_SIZE),ParamRS,MAX_RS_SIZE,
								type,Strlimit,MAX_RS_SIZE,StrDescription,MAX_RS_SIZE,bContinue);
		if(type!=NO)
		{	if((!GetHelpComStruct()->ModifList->IsItEmpty(GetHelpComStruct()->IndexTab,iRang))&&
			   (!GetHelpComStruct()->ModifList->AddParam(TRUE,GetHelpComStruct()->IndexTab,ParamRS,iRang)))
			{	/*if(EvoluTmp->GetEvoluTampon(Rang)!=NULL)
				{	AddItemList(hWndListRS,GetHelpComStruct()->ModifList->GetModifParam(GetHelpComStruct()->IndexTab,Rang),6,Rang);}
				else AddItemList(hWndListRS,GetHelpComStruct()->ModifList->GetModifParam(GetHelpComStruct()->IndexTab,Rang),3,Rang);*/
				AddItemList(hWndListRS,GetHelpComStruct()->ModifList->GetModifParam(GetHelpComStruct()->IndexTab,iRang),3,iRang);
				Newitem=new CParam(2,GetHelpComStruct()->ModifList->GetModifParam(GetHelpComStruct()->IndexTab,iRang),type,Strlimit,StrDescription);
			}
			else
			{	/*if(EvoluTmp->GetEvoluTampon(Rang)!=NULL)
				{	AddItemList(hWndListRS,ParamRS,6,iRang);}
				else AddItemList(hWndListRS,ParamRS,3,iRang);*/
				AddItemList(hWndListRS,ParamRS,3,iRang);
				Newitem=new CParam(2,ParamRS,type,Strlimit,StrDescription);
			}
			mHpPak_ItemsList->InsertItemList(Newitem);
		}
		else return FALSE;
		iRang++;
	}
	mHpPak_ClosemHpFile();
	return TRUE;
}

//-----------------------------------------------------//
// CreateTree
//-----------------------------------------------------//
BOOL CreateTree(HWND hWndActive,int iTopic,HTREEITEM hParentRS,HTREEITEM hParentHide)
{	HTREEITEM hParent;
	HTREEITEM hHideParent;
	char lpTopic[MAX_RS_SIZE]={0};
	char lpStrRS[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpHidden[MAX_RS_SIZE]={0};
	char lpmHpFilePath[MAX_RS_SIZE]={0};
	int iTmpPos,iCurrentPos,iLineCnt,iHideParam,iParam=0;
	GetmHpFileName(hWndmHpFilesList,iTopic,lpmHpFilePath,MAX_RS_SIZE);
	iLineCnt=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFilePath);
	if(iLineCnt==NO) return FALSE;
	LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_OPENINGFILE,lpRC,MAX_KEY_SIZE);
	strncat(lpRC," '",2);
	strncat(lpRC,lpmHpFilePath,mHpPak_FindStrLenght(lpmHpFilePath,MAX_RS_SIZE));
	strncat(lpRC,"'",1);
	SetWindowText(hWndStatus,lpRC);
	mHpPak_GetTopicmHpFile(hWndActive,lpmHpFilePath,lpTopic,MAX_RS_SIZE);
	if(hParentRS!=NULL) hParent=hParentRS;
	else hParent=AddItem(hWndTreeRS,TVGN_ROOT,7,8,lpTopic,0);
	if(hParentHide!=NULL) hHideParent=hParentHide;
	else hHideParent=AddItem(hWndHiddenTree,TVGN_ROOT,0,0,lpTopic,0);
	iTmpPos=mHpPak_GetTaskLine(hWndActive,lpmHpFilePath,FALSE);
	if((iTmpPos!=NO)&&(iTmpPos!=1))
	{	for(int i=0;i<iLineCnt;i++)
		{	ProceedMsgQueue(hWndActive);//,0);
			if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+1),FALSE)) return FALSE;
			mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpStrRS,MAX_RS_SIZE,iCurrentPos,iParam);
			iHideParam=iParam;
			if(iCurrentPos==NO) return FALSE;
			if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+2),FALSE)) return FALSE;
			mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpTmp,MAX_RS_SIZE,iTmpPos,iParam);
			if(iTmpPos==NO) return FALSE;
			if(!iParam) // Single
			{	if(iTmpPos>iCurrentPos)
				{	hParent=AddItem(hWndTreeRS,hParent,0,4,lpStrRS,0);
					_snprintf(lpHidden,MAX_RS_SIZE,"¤%d¤%d",(i+1),0); // Folder
					hHideParent=AddItem(hWndHiddenTree,hHideParent,0,0,lpHidden,0);
				}
				else
				{	if(!iHideParam)
					{	_snprintf(lpHidden,MAX_RS_SIZE,"¤%d¤%d",(i+1),1); // Doc
						AddItem(hWndHiddenTree,hHideParent,0,0,lpHidden,0);
					}
					while(iTmpPos!=iCurrentPos)
					{	hParent=TreeView_GetParent(hWndTreeRS,hParent);
						hHideParent=TreeView_GetParent(hWndHiddenTree,hHideParent);
						iTmpPos++;
					}
				}
			}
			else if(iParam==1)
			{	hParent=AddItem(hWndTreeRS,hParent,2,5,lpStrRS,0);
				_snprintf(lpHidden,MAX_RS_SIZE,"¤%d¤%d",(i+1),2); // Param
				hHideParent=AddItem(hWndHiddenTree,hHideParent,0,0,lpHidden,0);
			}
			else if((i+1)<iLineCnt)
			{	_snprintf(lpHidden,MAX_RS_SIZE,"¤%d¤%d",(i+1),1); // Doc
				AddItem(hWndHiddenTree,hHideParent,0,0,lpHidden,0);
			}
			mHpPak_IncreaseProgressBar(hWndProgress,iLineCnt);
		}
	}
	mHpPak_ClosemHpFile();
	return TRUE;
}

//-----------------------------------------------------//
// SelectUrlHTML
//-----------------------------------------------------//
BOOL SelectUrlHTML(HTREEITEM hItemSel,HTREEITEM hHideItem)
{   if(!hItemSel)
    {   hItemSel=TreeView_GetRoot(hWndTopicTree);
        hHideItem=TreeView_GetRoot(hWndHiddenTree);
        hItemSel=TreeView_GetChild(hWndTopicTree,hItemSel);
        if(!hItemSel)
            return FALSE; // Select root
        hHideItem=TreeView_GetChild(hWndHiddenTree,hHideItem);
    }
    char lpHtmlFile[MAX_KEY_SIZE]={0};
    TVITEM TvItem;
    TvItem.mask=TVIF_TEXT;
    TvItem.cchTextMax=MAX_RS_SIZE;
    TvItem.pszText=lpHtmlFile;
    TvItem.hItem=hHideItem;
    TreeView_GetItem(hWndHiddenTree,&TvItem);
    if(!strncmp(TvItem.pszText,strURL.c_str(),MAX_RS_SIZE))
    {   TreeView_SelectItem(hWndTopicTree,hItemSel);
        return TRUE; // Topic URL tree view selected
    }
    while(TreeView_GetNextSibling(hWndTopicTree,hItemSel)!=NULL)
    {   hItemSel=TreeView_GetNextSibling(hWndTopicTree,hItemSel);
        hHideItem=TreeView_GetNextSibling(hWndTopicTree,hHideItem);
        //
        TvItem.mask=TVIF_TEXT;
        TvItem.cchTextMax=MAX_RS_SIZE;
        TvItem.pszText=lpHtmlFile;
        TvItem.hItem=hHideItem;
        TreeView_GetItem(hWndHiddenTree,&TvItem);
        if(!strncmp(TvItem.pszText,strURL.c_str(),MAX_RS_SIZE))
        {   TreeView_SelectItem(hWndTopicTree,hItemSel);
            return TRUE; // Topic URL tree view selected
        }
    }
    if(TreeView_GetChild(hWndTopicTree,hItemSel)!=NULL)
    {   if(SelectUrlHTML(TreeView_GetChild(hWndTopicTree,hItemSel),TreeView_GetChild(hWndHiddenTree,hHideItem)))
            return TRUE; // Topic URL tree view selected
    }
    while(TreeView_GetPrevSibling(hWndTopicTree,hItemSel)!=NULL)
    {   hItemSel=TreeView_GetPrevSibling(hWndTopicTree,hItemSel);
        hHideItem=TreeView_GetPrevSibling(hWndTopicTree,hHideItem);
        if(TreeView_GetChild(hWndTopicTree,hItemSel)!=NULL)
        {   if(SelectUrlHTML(TreeView_GetChild(hWndTopicTree,hItemSel),TreeView_GetChild(hWndHiddenTree,hHideItem)))
                return TRUE; // Topic URL tree view selected
        }
    }
    return FALSE;
}

//-----------------------------------------------------//
// CreateTopicTree
//-----------------------------------------------------//
BOOL CreateTopicTree(HWND hWndActive,int iTopic)
{	HTREEITEM hParent;
	HTREEITEM hHideParent;
	char lpTopic[MAX_RS_SIZE]={0};
	char lpStrRS[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpHidden[MAX_RS_SIZE]={0};
	char lpmHpFilePath[MAX_RS_SIZE]={0};
	int iParam,iParamLast,iHideParam,iTmpPos,iCurrentPos,iLineCnt;
	GetmHpFileName(hWndmHpFilesList,iTopic,lpmHpFilePath,MAX_RS_SIZE);
	iLineCnt=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFilePath);
	if(iLineCnt==NO) return FALSE;
	mHpPak_GetTopicmHpFile(hWndActive,lpmHpFilePath,lpTopic,MAX_RS_SIZE);
	TreeView_DeleteAllItems(hWndTopicTree);
	hParent=AddItem(hWndTopicTree,TVGN_ROOT,7,8,lpTopic,0);
    hHideParent=AddItem(hWndHiddenTree,TVGN_ROOT,0,0,lpTopic,0);
	if(iLineCnt!=1)
	{	for(int i=0;i<iLineCnt;i++)
		{	ProceedMsgQueue(hWndActive);//,1);
			if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+1),FALSE)) return FALSE;
			mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpStrRS,MAX_RS_SIZE,iCurrentPos,iParamLast);
			if(iCurrentPos==NO) return FALSE;
            iHideParam=iParamLast;
			if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+2),FALSE)) return FALSE;
			mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpTmp,MAX_RS_SIZE,iTmpPos,iParam);
			if(iTmpPos==NO) return FALSE;
			if(!iParam) // Single
			{	if(iTmpPos>iCurrentPos)
                {   SplitHtmlLinkRS(lpStrRS,MAX_RS_SIZE,lpmHpFilePath,lpHidden,MAX_RS_SIZE);
                    hParent=AddItem(hWndTopicTree,hParent,0,4,lpStrRS,0);
                    //_snprintf(lpHidden,MAX_RS_SIZE,"¤%d¤%d",(i+1),0); // Folder
                    hHideParent=AddItem(hWndHiddenTree,hHideParent,0,0,lpHidden,0);
                }
				else
				{   SplitHtmlLinkRS(lpStrRS,MAX_RS_SIZE,lpmHpFilePath,lpHidden,MAX_RS_SIZE);
                    AddItem(hWndTopicTree,hParent,1,1,lpStrRS,0);
                    //_snprintf(lpHidden,MAX_RS_SIZE,"¤%d¤%d",(i+1),1); // Doc
                    AddItem(hWndHiddenTree,hHideParent,0,0,lpHidden,0);
					while(iTmpPos!=iCurrentPos)
					{	hParent=TreeView_GetParent(hWndTopicTree,hParent);
                        hHideParent=TreeView_GetParent(hWndHiddenTree,hHideParent);
						iTmpPos++;
					}
				}
			}
			else if((iParamLast!=2)||(iParam!=2))
			{   SplitHtmlLinkRS(lpStrRS,MAX_RS_SIZE,lpmHpFilePath,lpHidden,MAX_RS_SIZE);
                AddItem(hWndTopicTree,hParent,1,1,lpStrRS,0);
                //_snprintf(lpHidden,MAX_RS_SIZE,"¤%d¤%d",(i+1),1); // Doc
                AddItem(hWndHiddenTree,hHideParent,0,0,lpHidden,0);
			}
		}
	}
	mHpPak_ClosemHpFile();
	TreeView_Expand(hWndTopicTree,TreeView_GetRoot(hWndTopicTree),TVE_EXPAND);
    hParent=TreeView_GetChild(hWndTopicTree,TreeView_GetRoot(hWndTopicTree));
    if(hParent!=NULL)
        TreeView_SelectItem(hWndTopicTree,hParent);
    else
    {   assert(NULL);
	    TreeView_SelectItem(hWndTopicTree,TreeView_GetRoot(hWndTopicTree));
    }
	return TRUE;
}

//-----------------------------------------------------//
// SetChangeListView
//-----------------------------------------------------//
BOOL SetChangeListView(HWND hWndActive)
{	char lpStrRS[MAX_RS_SIZE]={0};
	char lpTmpRS[MAX_RS_SIZE]={0};
	char lpmHpFilePath[MAX_RS_SIZE]={0};
	int RefTab[MAX_PART_TREE]={0};
	BOOL bContinue=TRUE;
	int iCurrentPos,iTmpPos,iPos=NO,indice=0,iLine=1,iTopic=GetTopicFromTree();
	int iTmpParam,iEndParam;
	int iLineCnt;
	CItem *NewItem;
	GetmHpFileName(hWndmHpFilesList,iTopic,lpmHpFilePath,MAX_RS_SIZE);
	iLineCnt=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFilePath);
	mHpPak_EmptyTab(RefTab,MAX_PART_TREE);
	mHpPak_AddIndex(GetHelpComStruct()->IndexTab,0);
	ListView_DeleteAllItems(hWndListRS);
	for(int i=0;i<iLineCnt;i++)
	{	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+1),FALSE)) return FALSE;
		mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpStrRS,MAX_RS_SIZE,iCurrentPos,iEndParam);
		if(iCurrentPos==NO) return FALSE;
		RefTab[iCurrentPos]++;
		RefTab[(iCurrentPos+1)]=NO;
		if(mHpPak_CompareTab(GetHelpComStruct()->IndexTab,RefTab))
		{	if(iPos==NO) iPos=iCurrentPos;
			if(!iEndParam)
			{	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFilePath,(i+2),FALSE)) return FALSE;
				mHpPak_GetNextStrRS(hWndActive,lpmHpFilePath,lpTmpRS,MAX_RS_SIZE,iTmpPos,iTmpParam);
				if(iTmpPos==NO) return FALSE;
				if(bStyle)
				{	ChangeStyle(FALSE);
					bStyle=FALSE;
				}
				if(iTmpPos>iCurrentPos)
				{	if(!iTmpParam) AddItemList(hWndListRS,lpStrRS,0,GetHelpComStruct()->IndexTab[iPos]);
					else if(iTmpParam==1) AddItemList(hWndListRS,lpStrRS,2,GetHelpComStruct()->IndexTab[iPos]);
					NewItem=new CItem(0);
				}
				else
				{	AddItemList(hWndListRS,lpStrRS,1,GetHelpComStruct()->IndexTab[iPos]);
					NewItem=new CItem(1);
				}
			}
			else if(iEndParam==1)
			{	if(!bStyle)
				{	ChangeStyle(TRUE);
					bStyle=TRUE;
				}
				SetLastIndex();
				return GetAllParamDetail(hWndActive,iTopic);
			}
			GetHelpComStruct()->IndexTab[iPos]++;
			if(!iEndParam) mHpPak_ItemsList->InsertItemList(NewItem);
		}
	}
	SetLastIndex();
	mHpPak_ClosemHpFile();
	return TRUE;
}

//----------------------------------------------------------------//
// GetLastIndiceFromSymbolRang
//----------------------------------------------------------------//
int GetLastIndiceFromSymbolRang(char* LpStr,char cSymbol)
{	int i=0,iRes=NO;
	while((iRes==NO)&&(LpStr[i]!=END_LIST))
	{	if(LpStr[i]==cSymbol) iRes++;
		i++;
	}
	if(iRes!=NO)
	{	i=0,iRes=NO;
		while(mHpPak_GetIndexFromSymbolPos(i,LpStr,cSymbol)!=NO)
		{	iRes=mHpPak_GetIndexFromSymbolPos(i,LpStr,cSymbol);
			i++;
		}
		return iRes;
	}
	return NO;
}

//-------------------------------------------------------------//
// GetFloatFromStr
//-------------------------------------------------------------//
float GetFloatFromStr(char* lpfVal)
{	int index=0;
	float fRes=0.0,fDiv=1.0;
	BOOL bVirgule=FALSE;
	while(lpfVal[index]!=END_LIST)
	{	if(!mHpPak_IsItANumber(lpfVal[index]))
		{	if((lpfVal[index]==',')||(lpfVal[index]=='.')) bVirgule=TRUE;
			else return NO;
		}
		else
		{	if(bVirgule) fDiv*=10.0;
			fRes*=10.0;
			fRes+=(float)mHpPak_CharToInt(lpfVal[index]);
		}		
		index++;
	}
	return (fRes/fDiv);
}

//-------------------------------------------------------------//
// IncrementeNumber
//-------------------------------------------------------------//
void IncrementeNumber(BOOL bIncDec,BOOL bIntFloat)
{	int iCnt=ListView_GetItemCount(hWndParamList);
	int indice;
	float fRef,fInc;
	BOOL bFirst=TRUE;
	char lpTmp[MAX_RS_SIZE]={0};
	char lpVal[MAX_RS_SIZE]={0};
	GetWindowText(hWndAssoc,lpVal,MAX_RS_SIZE);
	fInc=GetFloatFromStr(lpVal);
	if(fInc==NO) return;
	if(!bIncDec) fInc*=NO;
	for(int i=0;i<iCnt;i++)
	{	if((ListView_GetItemState(hWndParamList,i,LVIS_SELECTED)&LVIS_SELECTED)==LVIS_SELECTED)
		{	if(bFirst)
			{	mHpPak_InitStr(lpVal,MAX_RS_SIZE);
				ListView_GetItemText(hWndParamList,i,0,lpVal,MAX_RS_SIZE);			
				fRef=GetFloatFromStr(lpVal);
				if(fRef==NO) return;
				bFirst=FALSE;
				mHpPak_InitStr(lpVal,MAX_RS_SIZE);
				if(!bIntFloat)
				{	_snprintf(lpVal,MAX_RS_SIZE,"%f",fRef);
					ListView_SetItemText(hWndParamList,i,0,lpVal);
				}
			}
			else
			{	fRef+=fInc;
				mHpPak_InitStr(lpVal,MAX_RS_SIZE);
				mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
				_snprintf(lpVal,MAX_RS_SIZE,"%f",fRef);
				if(bIntFloat)
				{	// Retire la virgule
					if(fRef<0)
					{	fRef=0;
						mHpPak_InitStr(lpVal,MAX_RS_SIZE);
						_snprintf(lpVal,MAX_RS_SIZE,"%f",fRef);
					}
					indice=0;
					while((lpVal[indice]!=END_LIST)&&(mHpPak_IsItANumber(lpVal[indice])))
					{	lpTmp[indice]=lpVal[indice];
						indice++;
					}
					strncpy(lpVal,lpTmp,MAX_RS_SIZE);
				}
				ListView_SetItemText(hWndParamList,i,0,lpVal);
			}
		}
	}
}

//-------------------------------------------------------------//
// IncrementeString
//-------------------------------------------------------------//
void IncrementeString(BOOL bExt)
{	int iCnt=ListView_GetItemCount(hWndParamList);
	char lpVal[MAX_RS_SIZE]={0};
	char lpTmp1[MAX_RS_SIZE]={0};
	char lpTmp2[MAX_RS_SIZE]={0};
	int ExtIndice=NO,iRef=1,iLen,Indice;
	for(int i=0;i<iCnt;i++)
	{	if((ListView_GetItemState(hWndParamList,i,LVIS_SELECTED)&LVIS_SELECTED)==LVIS_SELECTED)
		{	mHpPak_InitStr(lpVal,MAX_RS_SIZE);
			mHpPak_InitStr(lpTmp2,MAX_RS_SIZE);				
			ListView_GetItemText(hWndParamList,i,0,lpVal,MAX_RS_SIZE);			
			if(bExt) ExtIndice=GetLastIndiceFromSymbolRang(lpVal,'.');
			if(ExtIndice!=NO)
			{	Indice=0;
				mHpPak_InitStr(lpTmp1,MAX_RS_SIZE);
				while(Indice!=(ExtIndice-1))
				{	lpTmp1[Indice]=lpVal[Indice];
					Indice++;
				}
				_snprintf(lpTmp2,MAX_RS_SIZE,"%d",iRef);
				iLen=mHpPak_FindStrLenght(lpTmp2,MAX_RS_SIZE);
				strncat(lpTmp1,lpTmp2,iLen);
				while(lpVal[Indice]!=END_LIST)
				{	lpTmp1[Indice+iLen]=lpVal[Indice];
					Indice++;
				}
				strncpy(lpVal,lpTmp1,MAX_RS_SIZE);
			}
			else
			{	_snprintf(lpTmp2,MAX_RS_SIZE,"%d",iRef);
				strncat(lpVal,lpTmp2,mHpPak_FindStrLenght(lpTmp2,MAX_RS_SIZE));
			}
			ListView_SetItemText(hWndParamList,i,0,lpVal);			
			iRef++;
		}
	}
}

//-----------------------------------------------------//
// CopieFirstSel
//-----------------------------------------------------//
void CopieFirstSel()
{	int iCnt=ListView_GetItemCount(hWndParamList);
	char lpVal[MAX_RS_SIZE]={0};
	BOOL bFirst=TRUE;
	for(int i=0;i<iCnt;i++)
	{	if((ListView_GetItemState(hWndParamList,i,LVIS_SELECTED)&LVIS_SELECTED)==LVIS_SELECTED)
		{	if(bFirst)
			{	ListView_GetItemText(hWndParamList,i,0,lpVal,MAX_RS_SIZE);
				bFirst=FALSE;
			}
			else ListView_SetItemText(hWndParamList,i,0,lpVal);
		}
	}
}

//-------------------------------------------------------------//
// ChangeBoolState
//-------------------------------------------------------------//
void ChangeBoolState()
{	int iDecal,iChange,iCnt=ListView_GetItemCount(hWndParamList);
	char lpChange[MAX_RS_SIZE]={0};
	BOOL bLast,bFirst=TRUE;
	GetWindowText(hWndAssoc,lpChange,MAX_RS_SIZE);	
	iChange=mHpPak_GetIntFromStr(lpChange,MAX_RS_SIZE,0);
	if(iChange==0) return;
	iDecal=iChange;
	for(int i=0;i<iCnt;i++)
	{	if((ListView_GetItemState(hWndParamList,i,LVIS_SELECTED)&LVIS_SELECTED)==LVIS_SELECTED)
		{	if(iDecal==0)
			{	mHpPak_InitStr(lpChange,MAX_RS_SIZE);
				if(bLast) bLast=FALSE;
				else bLast=TRUE;
				_snprintf(lpChange,MAX_RS_SIZE,"%d",(int)bLast);
				ListView_SetItemText(hWndParamList,i,0,lpChange);
				iDecal=(iChange-1);
			}
			else
			{	mHpPak_InitStr(lpChange,MAX_RS_SIZE);
				ListView_GetItemText(hWndParamList,i,0,lpChange,MAX_RS_SIZE);
				if(bFirst)
				{	bLast=(BOOL)mHpPak_GetIntFromStr(lpChange,MAX_RS_SIZE,0);
					bFirst=FALSE;
				}
				else
				{	mHpPak_InitStr(lpChange,MAX_RS_SIZE);
					_snprintf(lpChange,MAX_RS_SIZE,"%d",(int)bLast);
					ListView_SetItemText(hWndParamList,i,0,lpChange);
				}
				iDecal--;
			}
		}
	}
}

//-------------------------------------------------------------//
// CopierEvolu
//-------------------------------------------------------------//
void CopierEvolu()
{	int iCnt=ListView_GetItemCount(hWndParamList);
	char lpVal[MAX_RS_SIZE]={0};
	CType *NewType;
	TamponList->EmptyTypeList();
	for(int i=0;i<iCnt;i++)
	{	if((ListView_GetItemState(hWndParamList,i,LVIS_SELECTED)&LVIS_SELECTED)==LVIS_SELECTED)
		{	ListView_GetItemText(hWndParamList,i,0,lpVal,MAX_RS_SIZE);
			NewType=new CStrType(lpVal);
			TamponList->AddType(NewType);
		}
	}
}

//-------------------------------------------------------------//
// CollerEvolu
//-------------------------------------------------------------//
void CollerEvolu()
{	int iCnt=ListView_GetItemCount(hWndParamList);
	int index=0;
	for(int i=0;i<iCnt;i++)
	{	if((ListView_GetItemState(hWndParamList,i,LVIS_SELECTED)&LVIS_SELECTED)==LVIS_SELECTED)
		{	if((index!=0)&&((TamponList->GetStrType(index)==NULL)||(TamponList->GetStrType(index)[0]==END_LIST))) index=0;
			ListView_SetItemText(hWndParamList,i,0,TamponList->GetStrType(index));
			index++;
		}
	}
}

//-----------------------------------------------------//
// ParamDlgDocProc
//-----------------------------------------------------//
LRESULT CALLBACK ParamDlgDocProc(HWND hWndDoc,UINT DocMessage,WPARAM DocWParam,LPARAM DocLParam)
{	LONG_PTR lPrevProc=ParamDlgDocLst->GetDocWndProc(hWndDoc);
	switch(DocMessage)
	{	case WM_KEYUP:
		{	switch(DocWParam)
			{	case VK_F1:
				{	mHpPak_OpenmHelpDoc(hWndDoc,MHPPAK_DOC_EVOL);
					break;
				}
				default:
				{	break;}
			}
			break;
		}
		case WM_NCDESTROY:
		{	ParamDlgDocLst->RemoveDocWnd(hWndDoc);
			break;
		}
	}
	return CallWindowProc((WNDPROC)lPrevProc,hWndDoc,DocMessage,DocWParam,DocLParam);
}

//-----------------------------------------------------//
// SetParamDlgDocLnch
//-----------------------------------------------------//
void SetParamDlgDocLnch()
{	ParamDlgDocLst->AddDocWnd(hWndType,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndDecrit,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndLimite,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndEvolu,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndAssoc,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndExt,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndParamList,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndApply,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndCopier,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndColler,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndRemoveParam,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndValidParam,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndCancelParam,(LONG_PTR)ParamDlgDocProc);
	ParamDlgDocLst->AddDocWnd(hWndHelpParam,(LONG_PTR)ParamDlgDocProc);
}

//-----------------------------------------------------------------------------------------------------------------------//
// CreatemHpDlg
//-----------------------------------------------------//
int CreatemHpDlg(HWND hWndParam)
{	FARPROC lpfnmHpDlg;
	// creation de la boite de dialogue MHP Help
	lpfnmHpDlg=MakeProcInstance((FARPROC)DlgProc,GetHelpComStruct()->hInstServ);
	int iResult=(int)DialogBox(GetHelpComStruct()->hInstServ,
                               MAKEINTRESOURCE(IDD_DLG_MHELP),
                               hWndParam,
                               (DLGPROC)lpfnmHpDlg);
	FreeProcInstance(lpfnmHpDlg);
	// Test la création de la boite de dialogue
	return iResult;
}

//-----------------------------------------------------//
// ListRSProc
//-----------------------------------------------------//
LRESULT CALLBACK ListRSProc(HWND hWndLst,UINT LstMessage,WPARAM LstWParam,LPARAM LstLParam)
{	switch(LstMessage)
	{	case WM_COMMAND:
		{	switch(HIWORD(LstWParam))
			{	case BN_CLICKED:
				{	int iRes,iIndex=GetHelpComStruct()->listBtn->GetIndexFromWnd((HWND)LstLParam);
					if(iIndex!=NO)
					{	LV_ITEM lvitem;
						ItemIndex=iIndex;
						iRes=(int)DialogBox(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDD_DLG_PARAM),hWndLst,(DLGPROC)ParamDlgProc);
						if(iRes==IDOK)
						{	lvitem.mask=LVIF_IMAGE;
							lvitem.iItem=ItemIndex;
							lvitem.iSubItem=0;
							lvitem.iImage=6;
							ListView_SetItemText(hWndListRS,ItemIndex,0,mHpPak_ItemsList->GetDetail(ItemIndex,0,0,lpRC,MAX_KEY_SIZE));
							ListView_SetItem(hWndListRS,&lvitem);
						}
						else if(iRes==IDC_REMOVE_PARAM)
						{	lvitem.mask=LVIF_IMAGE;
							lvitem.iItem=ItemIndex;
							lvitem.iSubItem=0;
							lvitem.iImage=3;
							ListView_SetItemText(hWndListRS,ItemIndex,0,mHpPak_ItemsList->GetDetail(ItemIndex,0,0,lpRC,MAX_KEY_SIZE));
							ListView_SetItem(hWndListRS,&lvitem);
						}
						for(int i=0;i<ListView_GetItemCount(hWndListRS);i++)
						{	if(GetHelpComStruct()->listBtn->bExistButton(i))
							{	ListView_SetItemState(hWndListRS,i,LVIS_SELECTED,LVIS_SELECTED);}
						}
						RefreshButton();
					}
					break;
				}
			}
			break;
		}
		/*case WM_MOVE:
		{	if((bChangeSize)&&
			   (((GetWindowLongPtr(hWndLst,GWL_STYLE)&WS_HSCROLL)==WS_HSCROLL)||((GetWindowLongPtr(hWndLst,GWL_STYLE)&WS_VSCROLL)==WS_VSCROLL))&&
			   (IsLoopOption())&&(mHpPak_ItemsList->WhatIsIt(0,FALSE)!=0))
			{	RefreshButton();
			}
			break;
		}*/
		case WM_PAINT:
		{	int iNextParam,iNextType,iNextDesc,iNextLimit;
			iNextParam=ListView_GetColumnWidth(hWndLst,0);
			iNextType=ListView_GetColumnWidth(hWndLst,1);
			iNextDesc=ListView_GetColumnWidth(hWndLst,2);
			iNextLimit=ListView_GetColumnWidth(hWndLst,3);
			if((iNextParam!=iColumnParam)||(iNextType!=iColumnType)||(iNextDesc!=iColumnDesc)||(iNextLimit!=iColumnLimit))
			{	if((IsLoopOption())&&(mHpPak_ItemsList->WhatIsIt(0,FALSE)!=0)&&(ListView_GetSelectedCount(hWndLst)!=0))
				{	RefreshButton();
					if((iNextParam<iColumnParam)||(iNextType<iColumnType)||(iNextDesc<iColumnDesc)||(iNextLimit<iColumnLimit))
					{	ShowWindow(hWndLst,SW_HIDE);
						ShowWindow(hWndLst,SW_SHOW);
					}
				}
				iColumnParam=ListView_GetColumnWidth(hWndLst,0);
				iColumnType=ListView_GetColumnWidth(hWndLst,1);
				iColumnDesc=ListView_GetColumnWidth(hWndLst,2);
				iColumnLimit=ListView_GetColumnWidth(hWndLst,3);
			}
			break;
		}
		case WM_HSCROLL:
		case WM_VSCROLL:
		{	if((IsLoopOption())&&(mHpPak_ItemsList->WhatIsIt(0,FALSE)!=0)) RefreshButton();
			else GetHelpComStruct()->listBtn->EmptyButtonList();
			break;
		}
		case WM_KEYUP:
		{	switch(LstWParam)
			{	case VK_F1:
				{	mHpPak_OpenmHelpDoc(hWndLst,MHPPAK_DOC_MAINCONT);
					break;
				}
				default:
				{	break;}
			}
			break;
		}
	}
	return CallWindowProc((WNDPROC)lListPrevProc,hWndLst,LstMessage,LstWParam,LstLParam);
}

//-----------------------------------------------------------------------------------------------------------------------//
// DlgProc()
//
//-----------------------------------------------------//
LRESULT CALLBACK DlgProc(HWND hWndDlg,UINT DlgMessage,WPARAM wParam,LPARAM lParam)
{	static BOOL bBlock=FALSE;
	static BOOL bPass=FALSE;
	static BOOL bLoading=TRUE;
    static BOOL bMaximized=FALSE;
	static BOOL bWindowsXP=FALSE;
	static BOOL bWindows7=FALSE;
    static BOOL bAddOperation=FALSE; // BUG: Java application send WM_ACTIVATEAPP after IDC_CREATE_ADD release
	// Evaluated Version //////////////////////////////////////////////////////////////////////////////////////////////////
	static BOOL bLicence=FALSE;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch(DlgMessage)
	{	case WM_INITDIALOG:
		{	bAddOperation=FALSE;
            if(mHpPak_ReadOperationKey()!=0)
			{	mHpPak_mHpMessage(hWndDlg,GetHelpComStruct()->hInstServ,IDS_LOCK_DHPOPEN,NULL,0,0,MB_ICONEXCLAMATION);
				return EndDialog(hWndDlg,IDCANCEL);
			}
			if(!bLicence)
			{	DWORD winVersion=GetVersion();
                DWORD VersMajor=(DWORD)(LOBYTE(LOWORD(winVersion)));
                DWORD VersMinor=(DWORD)(HIBYTE(LOWORD(winVersion)));
                // Check Windows 7 or later
                if((VersMajor>6)||((VersMajor==6)&&(VersMinor>=1)))
                {   bWindows7=TRUE;
                    iDlgWidth=506;
                    iDlgHeight=407;
                }
                else
                {   // TODO: Define 'iDlgWidth' & 'iDlgHeight' according Windows version
                    // -> Below for Windows 7 only
                    iDlgWidth=506;
                    iDlgHeight=407;
                }
                // Evaluated Version //////////////////////////////////////////////////////////////////////////////////////
				if(CreateEvaluateDlg(hWndDlg)==IDOK) bLicence=TRUE;
				else return EndDialog(hWndDlg,IDCANCEL);
				///////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			char lpStrrs[MAX_RS_SIZE]={0};
			char lptabItem[MAX_KEY_SIZE]={0};
            int iFilesCount=0;
			RECT AddRect;
			RECT DlgRect;
			TCITEM TabItem;
            //
            HICON hIcon=(HICON)LoadImage(GetHelpComStruct()->hInstServ,
                                         MAKEINTRESOURCE(IDI_ICON_ENG_HELP),
                                         IMAGE_ICON,
                                         GetSystemMetrics(SM_CXSMICON),
                                         GetSystemMetrics(SM_CYSMICON),
                                         0);
            if(hIcon)
                SendMessage(hWndDlg,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
			bLoading=TRUE;
			bLockOp=FALSE;
			if(!bPass)
			{	CreateManageObj();
				ReadRegisterKeys(TRUE);
				/*Xg=(RS_TREE_X+RS_TREE_W);
				Xd=(RS_LIST_X);
				cxTree=RS_TREE_W;
				Xlist=RS_LIST_X;
				cxList=RS_LIST_W;*/
				MainDlgDocLst=new CWndDocList();
                iDlgWidth=958;
                iDlgHeight=777;
                pWebClientCtrl=NULL;
                hWndWebClient=NULL;
				bPass=TRUE;
			}
			if(bBlock) CopyIndexTab(TabReplace,MAX_PART_TREE);
			else mHpPak_EmptyTab(TabReplace,MAX_PART_TREE);
			// Initialise les options d'assistance et de répétition //
			//GetHelpComStruct()->iMode=0;
			//GetHelpComStruct()->iNbBoucle=1;
			//////////////////////////////////////////////////////////
			hTabBrush=(HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
            //bChangeSize=FALSE;
            bStyle=FALSE;
			ItemIndex=NO;
			bHeaderDid=FALSE;
			iColumnParam=COLUMN_WIDTH_PARAM;
			iColumnType=COLUMN_WIDTH_TYPE;
			iColumnDesc=COLUMN_WIDTH_DESC;
			iColumnLimit=COLUMN_WIDTH_LIMIT;
			mHpPak_iFavIndex=NO;
			mHpPak_EmptyTab(GetHelpComStruct()->IndexTab,MAX_PART_TREE);
			hWndIcon=GetDlgItem(hWndDlg,IDC_STATIC_MOUSEICON);
			hWndStart=GetDlgItem(hWndDlg,IDC_START);
			hWndmHelp=GetDlgItem(hWndDlg,IDC_CHECK_MHELP);
			hWndAssist=GetDlgItem(hWndDlg,IDC_CHECK_ASSIST);
			hWndEditRS=GetDlgItem(hWndDlg,IDC_EDIT_RS);
			// Creation et initialisation du "Slider vitesse"
			InitCommonControls();
			hWndListRS=GetDlgItem(hWndDlg,IDC_TASK_LIST);
			hWndSpeed=GetDlgItem(hWndDlg,IDC_SLIDER_SPEED);
			hWndBoucle=GetDlgItem(hWndDlg,IDC_EDIT_NUMREPEAT);
			hWndAdd=GetDlgItem(hWndDlg,IDC_CREATE_ADD);
			hWndStatus=GetDlgItem(hWndDlg,IDC_STATUS);
			hWndStaticStatus=GetDlgItem(hWndDlg,IDC_STATIC_STATUS);
			hWndProgress=GetDlgItem(hWndDlg,IDC_LOAD_PROGRESS);
			hWndSetSearch=GetDlgItem(hWndDlg,IDC_SET_SEARCH);
			hWndStaticSearch=GetDlgItem(hWndDlg,IDC_STATIC_SEARCH);
			hWndStaticRepeat=GetDlgItem(hWndDlg,IDC_FRAME_REPEAT);
			hWndStaticNumRepeat=GetDlgItem(hWndDlg,IDC_STATIC_NUMREPEAT);
			hWndStaticIndex=GetDlgItem(hWndDlg,IDC_STATIC_INDEX);
			hWndSearch=GetDlgItem(hWndDlg,IDC_SEARCH);
			hWndRemoveFav=GetDlgItem(hWndDlg,IDC_REMOVE_FAVORITES);
			hWndRemovemHpFiles=GetDlgItem(hWndDlg,IDC_REMOVE_MHPFILES);
			hWndFrameMode=GetDlgItem(hWndDlg,IDC_FRAME_MODE);
			hWndSearchList=GetDlgItem(hWndDlg,IDC_LIST_SEARCH);
			hWndIndexList=GetDlgItem(hWndDlg,IDC_LIST_INDEX);
			hWndFavList=GetDlgItem(hWndDlg,IDC_LIST_FAVORITES);
			hWndmHpFilesList=GetDlgItem(hWndDlg,IDC_LIST_MHPFILES);
			hWndSetIndex=GetDlgItem(hWndDlg,IDC_SET_INDEX);
			hWndEditSearch=GetDlgItem(hWndDlg,IDC_EDIT_SEARCH);
			hWndEditIndex=GetDlgItem(hWndDlg,IDC_EDIT_INDEX);
			hWndAddFav=GetDlgItem(hWndDlg,IDC_ADD_FAVORITES);
			hWndAddmHpFiles=GetDlgItem(hWndDlg,IDC_ADD_MHPFILES);
			hWndStaticSlow=GetDlgItem(hWndDlg,IDC_STATIC_SLOW);
			hWndStaticFast=GetDlgItem(hWndDlg,IDC_STATIC_FAST);
			hWndFramemHelp=GetDlgItem(hWndDlg,IDC_FRAME_MHELPVERS);
			hWndmHelpIcon=GetDlgItem(hWndDlg,IDC_MHELP_ICON);
			hWndFrameSpeed=GetDlgItem(hWndDlg,IDC_FRAME_CURSORSPEED);
			hWndFrameSepHory=GetDlgItem(hWndDlg,IDC_FRAME_SEPHORY);
			hWndSetFav=GetDlgItem(hWndDlg,IDC_SET_FAVORITES);
			hWndFrameSepVert=GetDlgItem(hWndDlg,IDC_FRAME_SEPVERT);
			hWndAbout=GetDlgItem(hWndDlg,IDC_STATIC_ABOUT);
			hWndmHpTab=GetDlgItem(hWndDlg,IDC_TAB_MHELP);
			hWndClose=GetDlgItem(hWndDlg,IDCANCEL);
			hWndStaticEvent=GetDlgItem(hWndDlg,IDC_STATIC_EVENT);
			hWndEditEvent=GetDlgItem(hWndDlg,IDC_EDIT_EVENT);
			hWndRepeatCfg=GetDlgItem(hWndDlg,IDC_STATIC_REPEATCFG);
			hWndAutoAssistCfg=GetDlgItem(hWndDlg,IDC_STATIC_AUTOASSISTCFG);
			hWndRadioFav=GetDlgItem(hWndDlg,IDC_RADIO_FAV);
			hWndRadioAuto=GetDlgItem(hWndDlg,IDC_RADIO_AUTOASSIST);
			hWndFrameFav=GetDlgItem(hWndDlg,IDC_FRAME_AUTOASSIST);
			hWndStaticAuto=GetDlgItem(hWndDlg,IDC_STATIC_FAVROW);
			hWndEditAuto=GetDlgItem(hWndDlg,IDC_EDIT_AUTOASSIST);
			hWndResetAuto=GetDlgItem(hWndDlg,IDC_RESET_AUTOASSIST);
			hWndCustomAuto=GetDlgItem(hWndDlg,IDC_SAVE_AUTOASSIST);
			hWndSaveFav=GetDlgItem(hWndDlg,IDC_SAVE_FAVORITES);
			hWndOpenFavs=GetDlgItem(hWndDlg,IDC_OPEN_FAVORITES);
			hWndStaticDelay=GetDlgItem(hWndDlg,IDC_STATIC_MAXDELAY);
			hWndDelayStatic=GetDlgItem(hWndDlg,IDC_STATIC_EDITDELAY);
			hWndHour=GetDlgItem(hWndDlg,IDC_EDIT_HOUR);
			hWndMinute=GetDlgItem(hWndDlg,IDC_EDIT_MINUTE);
			hWndPoints=GetDlgItem(hWndDlg,IDC_STATIC_POINTS);
			hWndHelp=GetDlgItem(hWndDlg,IDC_BUTTON_HELP);
			hWndDestroy=GetDlgItem(hWndDlg,IDC_DESTROY);
            hWndCompList=GetDlgItem(hWndDlg,IDC_LISTVIEW_COMPONENT);
            hWndCompLabel=GetDlgItem(hWndDlg,IDC_STATIC_CTRLMGMT);
            hWndTopicTree=GetDlgItem(hWndDlg,IDC_TOPIC_TREE);
			//
			SetmHpCursor(hWndDlg,1);
			//
			SendMessage(hWndHour,EM_LIMITTEXT,3,0);
			SendMessage(hWndBoucle,EM_LIMITTEXT,2,0);
			SendMessage(hWndMinute,EM_LIMITTEXT,2,0);
			_snprintf(lpStrrs,MAX_RS_SIZE,"%d",GetHelpComStruct()->iHourDelay);
			SetWindowText(hWndHour,lpStrrs);
			mHpPak_InitStr(lpStrrs,MAX_RS_SIZE);
			_snprintf(lpStrrs,MAX_RS_SIZE,"%d",GetHelpComStruct()->iMinuteDelay);
			SetWindowText(hWndMinute,lpStrrs);
            // TODO: Check why SetParent below?
			//SetParent(hWndProgress,hWndmHpTab);
            //SetParent(hWndCompList,hWndmHpTab);
			//SetParent(hWndIndexList,hWndmHpTab);
			//SetParent(hWndFavList,hWndmHpTab);
			mHpPak_InitStr(lpStrrs,MAX_RS_SIZE);
			_snprintf(lpStrrs,MAX_RS_SIZE,"%d",GetHelpComStruct()->iNbBoucle);
			SetWindowText(hWndBoucle,lpStrrs);
			LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
			SetWindowText(hWndRepeatCfg,lpRC);
			SendMessage(hWndSpeed,TBM_SETRANGE,FALSE,MAKELONG((WORD)6,(WORD)20)); 
			SendMessage(hWndSpeed,TBM_SETPAGESIZE,0,1L);
			SendMessage(hWndSpeed,TBM_SETPOS,TRUE,(long)(GetHelpComStruct()->MouseSpeed));				
			switch(GetHelpComStruct()->iMode)
			{	case 0:
					SendDlgItemMessage(hWndDlg,IDC_CHECK_MHELP,BM_SETCHECK,1,0);
					SendDlgItemMessage(hWndDlg,IDC_CHECK_ASSIST,BM_SETCHECK,1,0);
					break;
				case 1:
					SendDlgItemMessage(hWndDlg,IDC_CHECK_ASSIST,BM_SETCHECK,1,0);
					SendMessage(hWndIcon,STM_SETICON,(WPARAM)LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_STOPMOUSE)),0);
					EnableWindow(hWndSpeed,FALSE);
					EnableWindow(hWndBoucle,TRUE);
					break;
				case 2:
					SendDlgItemMessage(hWndDlg,IDC_CHECK_MHELP,BM_SETCHECK,1,0);
					break;
			}
            hWndTopicTree=GetDlgItem(hWndDlg,IDC_TOPIC_TREE);
			hWndTreeRS=GetDlgItem(hWndDlg,IDC_TREEVIEW_RS);
			hWndHiddenTree=GetDlgItem(hWndDlg,IDC_HIDDEN_TREEVIEW);
			hImageList=ImageList_Create(16,16,ILC_MASK|ILC_COLOR16,6,1);
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_RSREP)));
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_TASK)));
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_PARAM_REP)));
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_PARAM)));
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_REP)));
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_PARAM)));
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_LOCKED)));
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_BOOK)));
			ImageList_AddIcon(hImageList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_BOOK)));
			TreeView_SetImageList(hWndTreeRS,hImageList,TVSIL_NORMAL);
            TreeView_SetImageList(hWndTopicTree,hImageList,TVSIL_NORMAL);
            ListView_SetImageList(hWndListRS,hImageList,LVSIL_SMALL);
			ListView_SetImageList(hWndmHpFilesList,hImageList,LVSIL_SMALL);
			ListView_SetImageList(hWndSearchList,hImageList,LVSIL_SMALL);
			ListView_SetImageList(hWndIndexList,hImageList,LVSIL_SMALL);
			ListView_SetImageList(hWndFavList,hImageList,LVSIL_SMALL);
			hMgmtImgList=ImageList_Create(16,16,ILC_MASK|ILC_COLOR16,6,1);
            mHpCtrl_FillCtrlMgmtImgList(hMgmtImgList);
            ListView_SetImageList(hWndCompList,hMgmtImgList,LVSIL_SMALL);
			SetHeader(TRUE);
            SetCompListHeader();
			SetRepeatView();
			EnableWindow(hWndRemovemHpFiles,FALSE);
			EnableWindow(hWndSetIndex,FALSE);
			EnableWindow(hWndSetSearch,FALSE);
			EnableWindow(hWndSetFav,FALSE);
			EnableWindow(hWndRemoveFav,FALSE);
			EnableWindow(hWndStart,FALSE);
			EnableWindow(hWndDestroy,FALSE);
			EnableWindow(hWndAddFav,FALSE);
			EnableWindow(hWndSaveFav,FALSE);
			EnableWindow(hWndResetAuto,FALSE);
			EnableWindow(hWndOpenFavs,FALSE);
			SendMessage(hWndRadioFav,BM_SETCHECK,BST_CHECKED,0);
			EnableWindow(hWndCustomAuto,FALSE);
			lListPrevProc=SetWindowLongPtr(hWndListRS,GWLP_WNDPROC,(LONG_PTR)ListRSProc);
			TabItem.mask=TCIF_TEXT;
			TabItem.cchTextMax=MAX_KEY_SIZE;
			TabItem.pszText=lptabItem;
			LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_MHPFILES,lpRC,MAX_KEY_SIZE);
			strncpy(TabItem.pszText,lpRC,MAX_KEY_SIZE);
			TabCtrl_InsertItem(hWndmHpTab,0,&TabItem);
			ReplaceAutoFavList(TRUE);
			ReplacemHpLst(hWndDlg,hWndmHpFilesList);
			if(GetHelpComStruct()->bmHpFileOpened)
			{
#ifdef MHELP_OLD
                while(iFilesCount!=ListView_GetItemCount(hWndmHpFilesList))
				{	mHpPak_ClosemHpFile();
					CreateTree(hWndDlg,iFilesCount,NULL,NULL);
					SendMessage(hWndProgress,PBM_SETPOS,0,0);
					UpdateIndexList(hWndDlg,iFilesCount,TRUE);
					SendMessage(hWndProgress,PBM_SETPOS,0,0);
					ExpandNewTopicItem(iFilesCount);
					iFilesCount++;
				}
				if(!bBlock) imHpFileIndex=0;
				if((!bBlock)||(!mHpPak_ItemsList->WhatIsIt(0,FALSE))) bLoading=FALSE;
				ReplaceTree(hWndTreeRS,TabReplace);
				if(bBlock)
				{	CopyTabReplace();
					if(!mHpPak_ItemsList->WhatIsIt(0,FALSE)) // DocRS
					{	mHpPak_ItemsList->EmptyList();
						SetChangeListView(hWndDlg);
						ListView_SetItemState(hWndListRS,mHpPak_GetIndex(TabReplace),LVIS_SELECTED,LVIS_SELECTED);
					}
					else // ParamRS
					{	mHpPak_EmptyTab(TabReplace,MAX_PART_TREE);
						iFilesCount=0;
						for(int i=0;i<mHpPak_ItemsList->GetItemCount();i++)
						{	if(mHpPak_ItemsList->GetSelection(i))
							{	TabReplace[iFilesCount]=i;
								iFilesCount++;
							}
						}
						TabReplace[iFilesCount]=NO;
						mHpPak_ItemsList->EmptyList();
						SetChangeListView(hWndDlg);
						iFilesCount=0;
						while(TabReplace[iFilesCount]!=NO)
						{	ListView_SetItemState(hWndListRS,TabReplace[iFilesCount],LVIS_SELECTED,LVIS_SELECTED);
							iFilesCount++;
						}
					}
					UpdateEditRS();
					EnableWindow(hWndStart,TRUE);
					EnableWindow(hWndDestroy,TRUE);
					EnableWindow(hWndAddFav,TRUE);
				}
				bBlock=FALSE;
#else
                char lpFile[MAX_RS_SIZE]={0};
                CreateTopicTree(hWndDlg,0);
                if(!pWebClientCtrl)
                {   SelectHtmlURL(lpFile,MAX_RS_SIZE,hWndTopicTree,hWndHiddenTree,NULL,NULL);
                    strURL=lpFile;
                    pWebClientCtrl=new mHpWebClient(strURL,
                                                    GetHelpComStruct()->hInstServ,
                                                    true,
                                                    208,31,285,292,
                                                    hWndDlg);
                    hWndWebClient=pWebClientCtrl->hwnd_;
                }
                else
                {   pWebClientCtrl->Create(208,31,285,292,hWndDlg);
                    SelectHtmlURL(lpFile,MAX_RS_SIZE,hWndTopicTree,hWndHiddenTree,NULL,NULL);
                    strURL=lpFile;
                    pWebClientCtrl->bUserTrigURL_=false;
                    pWebClientCtrl->URL(strURL);
                    hWndWebClient=pWebClientCtrl->hwnd_;
                }
                EnableWindow(hWndAddmHpFiles,FALSE);
                mHpPak_ItemsList->EmptyList();
#endif
				LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_CONTENTS,lpRC,MAX_KEY_SIZE);
				strncpy(TabItem.pszText,lpRC,MAX_KEY_SIZE);
				TabCtrl_InsertItem(hWndmHpTab,1,&TabItem);
				LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_CONFIG,lpRC,MAX_KEY_SIZE);
				strncpy(TabItem.pszText,lpRC,MAX_KEY_SIZE);
				TabCtrl_InsertItem(hWndmHpTab,2,&TabItem);
				LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_SEARCH,lpRC,MAX_KEY_SIZE);
				strncpy(TabItem.pszText,lpRC,MAX_KEY_SIZE);
				TabCtrl_InsertItem(hWndmHpTab,3,&TabItem);
				LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_INDEX,lpRC,MAX_KEY_SIZE);
				strncpy(TabItem.pszText,lpRC,MAX_KEY_SIZE);
				TabCtrl_InsertItem(hWndmHpTab,4,&TabItem);
				LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_FAVORITES,lpRC,MAX_KEY_SIZE);
				strncpy(TabItem.pszText,lpRC,MAX_KEY_SIZE);
				TabCtrl_InsertItem(hWndmHpTab,5,&TabItem);
				LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_COMPONENT,lpRC,MAX_KEY_SIZE);
				strncpy(TabItem.pszText,lpRC,MAX_KEY_SIZE);
				TabCtrl_InsertItem(hWndmHpTab,6,&TabItem);
				LoadString(GetHelpComStruct()->hInstServ,IDS_TAB_ABOUT,lpRC,MAX_KEY_SIZE);
				strncpy(TabItem.pszText,lpRC,MAX_KEY_SIZE);
				TabCtrl_InsertItem(hWndmHpTab,7,&TabItem);
				LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_READY,lpRC,MAX_KEY_SIZE);
				SetWindowText(hWndStatus,lpRC);
				TabCtrl_SetCurSel(hWndmHpTab,1);
			}
			else SetmHpTabItems(FALSE);
			MoveWindow(hWndmHpTab,3,3,498,369,TRUE);

            // TODO: Remove hWndEditRS
			//MoveWindow(hWndEditRS,9,340,484,21,TRUE);
            ShowWindow(hWndEditRS,SW_HIDE);
            MoveWindow(hWndmHpFilesList,9,32,485,251,TRUE);
			MoveWindow(hWndAddmHpFiles,9,287,87,26,TRUE);
			MoveWindow(hWndRemovemHpFiles,193,287,87,26,TRUE);
            MoveWindow(hWndOpenFavs,285,287,87,26,TRUE);
			MoveWindow(hWndStaticStatus,9,326,41,13,TRUE);
			MoveWindow(hWndStatus,50,326,444,13,TRUE);
			MoveWindow(hWndProgress,9,342,484,22,TRUE);
			//MoveWindow(hWndTreeRS,RS_TREE_X,RS_CTRL_Y,cxTree,RS_CTRL_H,TRUE);
            //MoveWindow(hWndListRS,Xlist,RS_CTRL_Y,cxList,RS_CTRL_H,TRUE);
#ifdef MHELP_OLD
			MoveWindow(hWndTreeRS,9,31,194,333,TRUE);
            MoveWindow(hWndListRS,208,31,285,333,TRUE);
#else
            MoveWindow(hWndTopicTree,9,31,204,334,TRUE);
            MoveWindow(hWndWebClient,218,31,276,334,TRUE);
#endif
			MoveWindow(hWndFrameMode,9,26,124,65,TRUE);
			MoveWindow(hWndmHelp,18,45,47,16,TRUE);
			MoveWindow(hWndAssist,18,66,72,16,TRUE);
			MoveWindow(hWndFrameSpeed,140,26,353,65,TRUE);
			MoveWindow(hWndSpeed,149,42,287,29,TRUE);
			MoveWindow(hWndStaticSlow,150,72,30,13,TRUE);
			MoveWindow(hWndStaticFast,411,72,35,13,TRUE);
			MoveWindow(hWndIcon,447,47,32,32,TRUE);
			MoveWindow(hWndStaticRepeat,9,92,195,50,TRUE);
			MoveWindow(hWndFrameSepHory,9,312,484,10,TRUE);
			MoveWindow(hWndStaticNumRepeat,18,113,114,16,TRUE);
			MoveWindow(hWndBoucle,135,110,57,20,TRUE);
			MoveWindow(hWndStaticSearch,9,27,430,16,TRUE);
			MoveWindow(hWndEditSearch,9,43,484,20,TRUE);
			MoveWindow(hWndSearch,407,67,87,26,TRUE);
			MoveWindow(hWndSearchList,9,99,484,184,TRUE);
			MoveWindow(hWndSetSearch,407,287,87,26,TRUE);
			MoveWindow(hWndStaticIndex,9,27,430,16,TRUE);
			MoveWindow(hWndEditIndex,9,43,484,20,TRUE);
			MoveWindow(hWndIndexList,9,68,484,264,TRUE);
			MoveWindow(hWndSetIndex,407,338,87,26,TRUE);
			MoveWindow(hWndFavList,9,31,484,227,TRUE);
			MoveWindow(hWndSetFav,407,288,87,26,TRUE);
			MoveWindow(hWndAddFav,118,288,87,26,TRUE);
			MoveWindow(hWndRemoveFav,210,288,87,26,TRUE);
			MoveWindow(hWndFramemHelp,9,26,484,338,TRUE);
			MoveWindow(hWndmHelpIcon,20,47,36,36,TRUE);
			MoveWindow(hWndFrameSepVert,65,40,5,275,TRUE);
			MoveWindow(hWndAbout,75,55,293,260,TRUE);			
			MoveWindow(hWndStaticEvent,9,293,63,16,TRUE);
			MoveWindow(hWndEditEvent,75,291,320,20,TRUE);
			MoveWindow(GetDlgItem(hWndDlg,IDC_STATIC_AUTOASSIST),79,389,60,16,TRUE);
			MoveWindow(hWndRepeatCfg,135,375,30,16,TRUE);
			MoveWindow(hWndAutoAssistCfg,135,389,30,16,TRUE);
			MoveWindow(hWndRadioFav,9,262,64,16,TRUE);
			MoveWindow(hWndRadioAuto,9,280,96,16,TRUE);
			MoveWindow(hWndFrameFav,108,255,5,62,TRUE);
			MoveWindow(hWndStaticAuto,117,265,107,16,TRUE);
			MoveWindow(hWndEditAuto,226,263,267,20,TRUE);
			MoveWindow(hWndResetAuto,314,288,87,26,TRUE);
			MoveWindow(hWndCustomAuto,406,288,87,26,TRUE);
			MoveWindow(hWndSaveFav,302,288,87,26,TRUE);
			MoveWindow(hWndStaticDelay,211,92,200,50,TRUE);
			MoveWindow(hWndDelayStatic,220,113,105,16,TRUE);
			MoveWindow(hWndHour,330,111,30,20,TRUE);
			MoveWindow(hWndMinute,370,111,30,20,TRUE);
			MoveWindow(hWndPoints,361,113,9,16,TRUE);
			MoveWindow(hWndHelp,4,376,70,26,TRUE);
			MoveWindow(hWndDestroy,170,376,78,26,TRUE);
			MoveWindow(hWndAdd,255,376,78,26,TRUE);
			MoveWindow(hWndStart,339,376,78,26,TRUE);
			MoveWindow(hWndClose,423,376,78,26,TRUE);
            MoveWindow(hWndCompList,10,44,483,320,TRUE);
            MoveWindow(hWndCompLabel,10,28,484,13,TRUE);
			HideShowmHpCtrls(hWndDlg);
			SetWindowText(hWndDlg,MHPPAK_DLGTITLE);
			SetMainDlgDocLnch();
			mHpPak_SetAppInfoKeys(lpExeName,1,1);
            mHpCtrl_FillCtrlMgmtListView(hWndCompList);
            if(!bMaximized)
            {   // BUG: les fenêtres XP et 7 sont trop courte //
                GetWindowRect(hWndDlg,&DlgRect);
                if(bWindows7)
                    MoveWindow(hWndDlg,DlgRect.left,DlgRect.top,iDlgWidth+16,iDlgHeight+38,TRUE);
                else
                {   GetWindowRect(hWndAdd,&AddRect);
	                GetWindowRect(hWndDlg,&DlgRect);
                    // Check Windows XP
	                if(DlgRect.bottom<AddRect.bottom)
                    {   bWindowsXP=TRUE;
		                MoveWindow(hWndDlg,DlgRect.left,DlgRect.top,510,437,TRUE);
	                }
                    else // TODO: Set Dialog size according other Windows version (default size below)
		                MoveWindow(hWndDlg,DlgRect.left,DlgRect.top,510,430,TRUE);
                }
                //
            }
            else
            {   ShowWindow(hWndDlg,SW_SHOWMAXIMIZED);
                // BUG: Refresh web client control to avoid full gray background displaying!?!
                if((TabCtrl_GetCurSel(hWndmHpTab)==1)&&(GetHelpComStruct()->bmHpFileOpened))
                {   KillTimer(hWndDlg,ID_TIMER_UPDATEWEBCLIENT);
                    SetTimer(hWndDlg,ID_TIMER_UPDATEWEBCLIENT,100,NULL);
                }
                //
            }
            SetmHpCursor(hWndDlg,0);
			bLoading=FALSE;
			return TRUE;
		}
        case WM_SYSCOMMAND:
        {   switch(wParam)
            {   case SC_MAXIMIZE:
                {   bMaximized=TRUE;
                    // BUG: Refresh web client control to avoid full gray background displaying!?!
                    if((TabCtrl_GetCurSel(hWndmHpTab)==1)&&(GetHelpComStruct()->bmHpFileOpened))
                        SetTimer(hWndDlg,ID_TIMER_UPDATEWEBCLIENT,100,NULL);
                    //
                    break;
                }
                case SC_RESTORE: bMaximized=FALSE; break;
            }
            break;
        }
        case WM_SIZE:
        {   iDlgWidth=(int)LOWORD(lParam);
            iDlgHeight=(int)HIWORD(lParam);
            MoveDlgCtrl(hWndDlg);
            // BUG: Refresh web client control to avoid full gray background displaying!?!
            if((wParam==SIZE_MAXIMIZED)&&(TabCtrl_GetCurSel(hWndmHpTab)==1)&&(GetHelpComStruct()->bmHpFileOpened))
            {   KillTimer(hWndDlg,ID_TIMER_UPDATEWEBCLIENT);
                SetTimer(hWndDlg,ID_TIMER_UPDATEWEBCLIENT,100,NULL);
            }
            //
            return TRUE;
        }
        case WM_GETMINMAXINFO:
        {   LPMINMAXINFO pMinMaxInfo=(LPMINMAXINFO)lParam;
			// BUG: les fenêtres XP et 7 sont trop courte //
            if(bWindowsXP)
            {   pMinMaxInfo->ptMinTrackSize.x=510;
                pMinMaxInfo->ptMinTrackSize.y=437;
            }
            else if(bWindows7)
            {   pMinMaxInfo->ptMinTrackSize.x=522;
                pMinMaxInfo->ptMinTrackSize.y=445;
            }
            else
            {   pMinMaxInfo->ptMinTrackSize.x=510;
                pMinMaxInfo->ptMinTrackSize.y=430;
            }
            //
            break;
        }
        case WM_DRAWITEM:
        {   LPDRAWITEMSTRUCT lpDrawItmStruct=(LPDRAWITEMSTRUCT)lParam;
            if(lpDrawItmStruct->CtlID==IDC_TAB_MHELP)
            {   if(lpDrawItmStruct->itemAction&ODA_DRAWENTIRE)
                {   char szTabLabel[MAX_KEY_SIZE]={0};
                    TCITEM tci;
                    tci.mask=TCIF_TEXT;
                    tci.pszText=szTabLabel;
                    tci.cchTextMax=MAX_KEY_SIZE;
                    TabCtrl_GetItem(hWndmHpTab,lpDrawItmStruct->itemID,&tci);
                    RECT ItmTabRect(lpDrawItmStruct->rcItem);
                    SetBkMode(lpDrawItmStruct->hDC,TRANSPARENT);
                    FillRect(lpDrawItmStruct->hDC,&ItmTabRect,hTabBrush);
                    DrawText(lpDrawItmStruct->hDC,szTabLabel,(int)strlen(szTabLabel),&ItmTabRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
                    return TRUE;
                }
            }
            break;
        }
		case WM_TIMER:
		{	switch(LOWORD(wParam))
			{	case ID_TIMER_UPDATEEDITRS:
				{	KillTimer(hWndDlg,ID_TIMER_UPDATEEDITRS);
					UpdateEditRS();
					break;
				}
                case ID_TIMER_UPDATEWEBCLIENT:
                {
                    KillTimer(hWndDlg,ID_TIMER_UPDATEWEBCLIENT);
                    RedrawWindow(hWndWebClient,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
                    break;
                }
                case ID_TIMER_URLCHANGED:
                {
                    KillTimer(hWndDlg,ID_TIMER_URLCHANGED);
                    std::list<std::string>::reverse_iterator iterLast=pWebClientCtrl->lstUrlHisto.rbegin();
                    std::string strNewURL=(*iterLast);
                    // Remove "file:///" if any
                    if(!strNewURL.find(PREFIX_ANCHOR_LINK))
                        strNewURL.erase(0,sizeof(PREFIX_ANCHOR_LINK)-1);
                    // Replace all "/" with "\\"
                    size_t iPos=strNewURL.find('/');
                    while(iPos!=std::string::npos)
                    {   strNewURL.replace(iPos,1,"\\");
                        iPos=strNewURL.find('/');
                    }
                    // Check if not a '#' anchor link (same URL)
                    if(strNewURL.rfind(strURL))
                    {   strURL=strNewURL;
                        if(!SelectUrlHTML(NULL,NULL))
                            TreeView_SelectItem(hWndTopicTree,TreeView_GetRoot(hWndTopicTree));
                    }
                    //else // == 0 -> Same URL found with perhaps existing anchor
                    RedrawWindow(hWndTopicTree,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
                    break;
                }
                case ID_TIMER_USERSTART:
                {
                    KillTimer(hWndDlg,ID_TIMER_USERSTART);
                    // Check existing post data
                    if(pWebClientCtrl->strmHpPostData.empty())
                    {
                        //
                        // TODO: Display an error message
                        // -> 'mHpStart' action without existing post data
                        //

                        break;
                    }
                    // Check existing 'mHpID' & 'mHpIdx' post data
                    size_t iPosID=pWebClientCtrl->strmHpPostData.find("mHpID=");
                    size_t iPosIdx=pWebClientCtrl->strmHpPostData.find("mHpIdx=");
                    if((iPosID==std::string::npos)||(iPosIdx==std::string::npos))
                    {
                        //
                        // TODO: Display an error message
                        // -> No 'mHpID' and/or 'mHpIdx' post data in the 'mHpStart' action
                        //

                        break;
                    }
                    // Get 'mHpID' & 'mHpIdx' post data
                    UINT iIndex=0;
                    mHpPak_EmptyTab(GetHelpComStruct()->IndexTab,MAX_PART_TREE);
                    iPosID+=sizeof("mHpID=")-1;
                    while((iPosID<pWebClientCtrl->strmHpPostData.length())&&
                          (pWebClientCtrl->strmHpPostData.at(iPosID)!='&'))
                    {   if((pWebClientCtrl->strmHpPostData.at(iPosID)>47)&&
                           (pWebClientCtrl->strmHpPostData.at(iPosID)<58)) // Digit (!= '-')
                        {   if(iIndex)
                            {   iIndex*=10;
                                iIndex+=(UINT)pWebClientCtrl->strmHpPostData.at(iPosID)&0x0F;
                            }
                            else
                                iIndex=(UINT)pWebClientCtrl->strmHpPostData.at(iPosID)&0x0F;
                        }
                        else // Should be '-'
                        {   mHpPak_AddIndex(GetHelpComStruct()->IndexTab,iIndex);
                            iIndex=0;
                        }
                        iPosID++;
                    }
                    mHpPak_AddIndex(GetHelpComStruct()->IndexTab,iIndex);
                    iIndex=0;
                    iPosIdx+=sizeof("mHpIdx=")-1;
                    while((iPosIdx<pWebClientCtrl->strmHpPostData.length())&&
                          (pWebClientCtrl->strmHpPostData.at(iPosIdx)!='&'))
                    {   if((pWebClientCtrl->strmHpPostData.at(iPosIdx)>47)&&
                           (pWebClientCtrl->strmHpPostData.at(iPosIdx)<58)) // Digit
                        {   if(iIndex)
                            {   iIndex*=10;
                                iIndex+=(UINT)pWebClientCtrl->strmHpPostData.at(iPosIdx)&0x0F;
                            }
                            else
                                iIndex=(UINT)pWebClientCtrl->strmHpPostData.at(iPosIdx)&0x0F;
                        }
                        iPosIdx++;
                    }
                    mHpCtrl_IndexTask=iIndex;

                    // Replace "%5E" -> "^"
                    iPosIdx=pWebClientCtrl->strmHpPostData.find("%5E");
                    while(iPosIdx!=std::string::npos)
                    {   pWebClientCtrl->strmHpPostData.replace(iPosIdx,3,"^");
                        iPosIdx=pWebClientCtrl->strmHpPostData.find("%5E");
                    }
                    // Replace "+" -> " "
                    iPosIdx=pWebClientCtrl->strmHpPostData.find("+");
                    while(iPosIdx!=std::string::npos)
                    {   pWebClientCtrl->strmHpPostData.replace(iPosIdx,1," ");
                        iPosIdx=pWebClientCtrl->strmHpPostData.find("+");
                    }

                    mHpCtrl_SetStartPostData(pWebClientCtrl->strmHpPostData);
                    ////// TODO: See decodeURIComponent in c++ from google (uriparser library)

                    //
                    bBlock=TRUE;
                    SetmHpListFromCtrl(hWndmHpFilesList,FALSE);
                    imHpFileIndex=GetTopicFromTree();
                    WriteRegisterKeys(1);
                    mHpPak_SetAppInfoKeys(lpExeName,1,3);
                    EndDialog(hWndDlg,IDOK);
                    break;
                }
			}
			break;
		}
		case WM_NOTIFY:
		{	switch(((LPNMHDR)lParam)->idFrom)
			{	case IDC_TREEVIEW_RS:
				{	switch(((LPNMHDR)lParam)->code)
					{	case TVN_SELCHANGED:
						{	HTREEITEM hTree=TreeView_GetSelection(hWndTreeRS);
							if((hTree!=NULL)&&(!bLoading))
							{	mHpPak_EmptyTab(GetHelpComStruct()->IndexTab,MAX_PART_TREE);
								mHpPak_AddIndex(GetHelpComStruct()->IndexTab,0);
								GetIndexFromTree(hTree,hWndTreeRS,GetHelpComStruct()->IndexTab);
								EnableWindow(hWndStart,FALSE);
								EnableWindow(hWndDestroy,FALSE);
								EnableWindow(hWndAddFav,FALSE);
								mHpPak_ItemsList->EmptyList();
								mHpPak_ClosemHpFile();
								SetChangeListView(hWndDlg);
								if(!SendMessage(hWndmHelp,BM_GETCHECK,0,0))
								{	EnableWindow(hWndBoucle,TRUE);}
								else
								{	SetWindowText(hWndBoucle,ONE_STRING);
									GetHelpComStruct()->iNbBoucle=1;
									EnableWindow(hWndBoucle,FALSE);
								}
								GetHelpComStruct()->listBtn->EmptyButtonList();
							}
							UpdateEditRS();
							break;
						}
						case TVN_ITEMEXPANDED:
						{	TV_ITEM TvItem=((NM_TREEVIEW FAR *)lParam)->itemNew;
							TvItem.mask=TVIF_IMAGE;
							if(TreeView_GetParent(hWndTreeRS,TvItem.hItem)==NULL)
							{	if(((NM_TREEVIEW FAR *)lParam)->action==1) TvItem.iImage=7;
								else TvItem.iImage=8;
							}
							else
							{	if(((NM_TREEVIEW FAR *)lParam)->action==1) TvItem.iImage=0;
								else TvItem.iImage=4;
							}
							TreeView_SetItem(hWndTreeRS,&TvItem);
							break;
						}
					}
					break;
				}
                case IDC_TOPIC_TREE:
                {   if((((LPNMHDR)lParam)->code==TVN_SELCHANGED)&&(!bLoading))
                    {   char lpFile[MAX_RS_SIZE]={0};
                        if((SelectHtmlURL(lpFile,MAX_RS_SIZE,hWndTopicTree,hWndHiddenTree,NULL,NULL))&&
                           (pWebClientCtrl!=NULL))
                        {   strURL=lpFile;
                            pWebClientCtrl->bUserTrigURL_=false;
                            pWebClientCtrl->URL(strURL);
                        }
                    }
                    break;
                }
				case IDC_TASK_LIST:
				{	switch(((LPNMHDR)lParam)->code)
					{	case LVN_GETDISPINFO:
						{	GetDispInfo((LV_DISPINFO FAR *)lParam);
							break;
						}
						case LVN_ITEMCHANGED:
						{	char lptemp1[MAX_RS_SIZE]={0};
							char lptemp2[MAX_RS_SIZE]={0};
							BOOL bWhatIsIt;
							ItemIndex=ListView_GetNextItem(hWndListRS,NO,LVNI_SELECTED);
							if(!mHpPak_ItemsList->WhatIsIt(0,FALSE)) // Doc
							{	if(ItemIndex!=NO)
								{	bWhatIsIt=mHpPak_ItemsList->WhatIsIt(ItemIndex,TRUE);
									EnableWindow(hWndStart,bWhatIsIt);
									EnableWindow(hWndDestroy,bWhatIsIt);
									EnableWindow(hWndAddFav,bWhatIsIt);
									mHpPak_ItemsList->NoSelection();
									mHpPak_ItemsList->SelectItem(ItemIndex);
								}
								else
								{	EnableWindow(hWndStart,FALSE);
									EnableWindow(hWndDestroy,FALSE);
									EnableWindow(hWndAddFav,FALSE);
									mHpPak_ItemsList->NoSelection();
								}
							}
							else // Param
							{	if(IsLoopOption()) RefreshButton();
								else GetHelpComStruct()->listBtn->EmptyButtonList();
								if(ItemIndex!=NO)
								{	EnableWindow(hWndStart,TRUE);
									EnableWindow(hWndDestroy,TRUE);
									EnableWindow(hWndAddFav,TRUE);
								}
								else
								{	EnableWindow(hWndStart,FALSE);
									EnableWindow(hWndDestroy,FALSE);
									EnableWindow(hWndAddFav,FALSE);
								}
							}
							//UpdateEditRS(); // BUG
							SetTimer(hWndDlg,ID_TIMER_UPDATEEDITRS,1,NULL);
							break;
						}
						case LVN_ENDLABELEDIT:
						{	if(((LV_DISPINFO FAR *)lParam)->item.mask==LVIF_TEXT)
							{	LV_ITEM LvItem=((LV_DISPINFO FAR *)lParam)->item;
								if(!mHpPak_ItemsList->IsRepeating(LvItem.iItem))
								{	if((strncmp(LvItem.pszText,NULL_STRING,LvItem.cchTextMax)!=0)||
									   (mHpPak_ItemsList->GetParameterType(LvItem.iItem)==2)||
									   (mHpPak_ItemsList->GetParameterType(LvItem.iItem)==4)||
									   (mHpPak_ItemsList->GetParameterType(LvItem.iItem)==7)||
									   (mHpPak_ItemsList->GetParameterType(LvItem.iItem)==9))
									{	mHpPak_ItemsList->ModifyStrParam(LvItem.iItem,LvItem.pszText);
										GetHelpComStruct()->ModifList->AddParam(FALSE,GetHelpComStruct()->IndexTab,LvItem.pszText,LvItem.iItem);
										ListView_SetItem(hWndListRS,&LvItem);
									}
								}
							}
							break;
						}
						case LVN_BEGINLABELEDIT:
						{	HWND hWndEditListRS=ListView_GetEditControl(hWndListRS);
							MainDlgDocLst->AddDocWnd(hWndEditListRS,(LONG_PTR)MainDlgDocProc);
							break;
						}
						case NM_DBLCLK:
						{	if((ItemIndex!=NO)&&(!mHpPak_ItemsList->WhatIsIt(ItemIndex,TRUE)))
							{	HTREEITEM hTree=TreeView_GetSelection(hWndTreeRS);
								HTREEITEM hItem=TreeView_GetNextItem(hWndTreeRS,hTree,TVGN_CHILD);
								if(hItem!=NULL)
								{	while(ItemIndex!=0)
									{	hTree=TreeView_GetNextItem(hWndTreeRS,hItem,TVGN_NEXT);
										hItem=hTree;
										ItemIndex--;
									}
									TreeView_SelectItem(hWndTreeRS,hItem);
								}
							}
							else if(ItemIndex!=NO)
							{	if(mHpPak_ItemsList->WhatIsIt(ItemIndex,TRUE)==1)
								{	SendMessage(hWndDlg,WM_COMMAND,IDC_START,0);}
								else ListView_EditLabel(hWndListRS,ItemIndex);
							}
							UpdateEditRS();
							break;
						}
						break;
					}
					break;
				}
				case IDC_TAB_MHELP:
				{	switch(((LPNMHDR)lParam)->code)
					{	case TCN_SELCHANGE:
						{	HideShowmHpCtrls(hWndDlg);
							break;
						}
					}
					break;
				}
				case IDC_LIST_MHPFILES:
				{	if((((LPNMHDR)lParam)->code)==LVN_ITEMCHANGED)
					{	if(ListView_GetSelectedCount(hWndmHpFilesList)!=0)
						{	EnableWindow(hWndRemovemHpFiles,TRUE);
							EnableWindow(hWndOpenFavs,TRUE);
						}
						else
						{	EnableWindow(hWndRemovemHpFiles,FALSE);
							EnableWindow(hWndOpenFavs,FALSE);
						}
					}
					break;
				}
				case IDC_LIST_INDEX:
				{	switch(((LPNMHDR)lParam)->code)
					{	case LVN_ITEMCHANGED:
						{	if(ListView_GetSelectedCount(hWndIndexList)!=0)
							{	EnableWindow(hWndSetIndex,TRUE);}
							else EnableWindow(hWndSetIndex,FALSE);
							UpdateEditEvent(4);
							break;
						}
						case NM_DBLCLK:
						{	if(ListView_GetItemSel(hWndIndexList)!=NO)
							{	SetSelectedEvent(hWndDlg,TabCtrl_GetCurSel(hWndmHpTab));}
							break;
						}
					}
					break;
				}
				case IDC_LIST_SEARCH:
				{	switch(((LPNMHDR)lParam)->code)
					{	case LVN_ITEMCHANGED:
						{	if(ListView_GetSelectedCount(hWndSearchList)!=0)
							{	EnableWindow(hWndSetSearch,TRUE);}
							else EnableWindow(hWndSetSearch,FALSE);
							UpdateEditEvent(3);
							break;
						}
						case NM_DBLCLK:
						{	if(ListView_GetItemSel(hWndSearchList)!=NO)
							{	SetSelectedEvent(hWndDlg,TabCtrl_GetCurSel(hWndmHpTab));}
							break;
						}
					}
					break;
				}
				case IDC_LIST_FAVORITES:
				{	switch(((LPNMHDR)lParam)->code)
					{	case LVN_ITEMCHANGED:
						{	if(SendMessage(hWndRadioFav,BM_GETCHECK,0,0)==BST_CHECKED)
							{	if(ListView_GetSelectedCount(hWndFavList)!=0)
								{	EnableWindow(hWndSetFav,TRUE);
									EnableWindow(hWndRemoveFav,TRUE);
									EnableWindow(hWndSaveFav,TRUE);
								}
								else
								{	EnableWindow(hWndSetFav,FALSE);
									EnableWindow(hWndRemoveFav,FALSE);
									EnableWindow(hWndSaveFav,FALSE);
								}
								UpdateEditEvent(5);
							}
							else
							{	UpdateAutoAssSel();
								EnableWindow(hWndCustomAuto,TRUE);
							}
							break;
						}
						case NM_DBLCLK:
						{	if(SendMessage(hWndRadioFav,BM_GETCHECK,0,0)==BST_CHECKED)
							{	if(ListView_GetItemSel(hWndFavList)!=NO)
								{	SetSelectedEvent(hWndDlg,TabCtrl_GetCurSel(hWndmHpTab));}
							}
							break;
						}
					}
					break;
				}
				case IDC_SLIDER_SPEED:
				{	switch(((LPNMHDR)lParam)->code)
					{	case NM_RELEASEDCAPTURE:
						{	GetHelpComStruct()->MouseSpeed=(int)SendMessage(hWndSpeed,TBM_GETPOS,0,0);
							break;
						}
					}
					break;
				}
				default:
				{	return DefWindowProc(hWndDlg,DlgMessage,wParam,lParam);}
			}
			break;
		}
		/*case WM_LBUTTONUP:
		{	if(bChangeSize)
			{	bChangeSize=FALSE;
				ClipCursor(NULL);
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{	int iNewX=(int)LOWORD(lParam);
			int iNewY=(int)HIWORD(lParam);
			if(((TabCtrl_GetCurSel(hWndmHpTab)==1)&&
                (GetHelpComStruct()->bmHpFileOpened)&&
				(iNewY>=RS_CTRL_Y)&&(iNewY<=(RS_CTRL_Y+RS_CTRL_H)))&&
				(iNewX<(RS_LIST_X+RS_LIST_W-60))&&(iNewX>(RS_TREE_X+60))&&
				((iNewX>=Xg)&&(iNewX<=Xd)))
			{	SetmHpCursor(hWndDlg,2);
				XclkPos=iNewX;
				if(((iNewX-Xg)>(Xd-iNewX))||(iNewX==(RS_TREE_X+61))) XclkPos--;
				else XclkPos++;
				bChangeSize=TRUE;
				OnMouseMove(hWndDlg,bWindowsXP,iNewX,iNewY);
			}
			break;
		}
		case WM_MOUSEMOVE:
		{	if((TabCtrl_GetCurSel(hWndmHpTab)==1)&&(GetHelpComStruct()->bmHpFileOpened))
                OnMouseMove(hWndDlg,bWindowsXP,(int)LOWORD(lParam),(int)HIWORD(lParam));
			break;
		}*/
		case WM_ACTIVATEAPP:
		{	if(bLicence) // Evaluated Version /////////////////////////////////////////////////////////////////////////////
			{	if(((BOOL)wParam)==TRUE)
				{   /* What is it FOR ?? ***************/
					if(mHpPak_ReadOperationKey()!=0)
					{	bLockOp=TRUE;
						mHpPak_mHpMessage(hWndDlg,GetHelpComStruct()->hInstServ,IDS_FORCE_DHPCLOSE,
										  NULL,0,0,MB_ICONEXCLAMATION);
						SendMessage(hWndClose,BM_CLICK,0,0);
						break;
					}
					else bLockOp=FALSE;
					ReadRegisterKeys(TRUE);
					ReplaceAutoFavList(TRUE);
					/**********************************/
					mHpPak_SetAppInfoKeys(lpExeName,1,1);
				}
				else if(!bAddOperation)
                    WriteRegisterKeys(0);
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			break;
		}
		case WM_COMMAND:
		{	switch(LOWORD(wParam))
			{	case IDCANCEL:
				{	mHpPak_EmptyTab(GetHelpComStruct()->IndexTab,MAX_PART_TREE);
					mHpPak_ItemsList->EmptyList();
					GetHelpComStruct()->ModifList->EmptyModifList();
					SetmHpListFromCtrl(hWndmHpFilesList,FALSE);
					WriteRegisterKeys(0);
					mHpPak_SetAppInfoKeys(lpExeName,1,0);
                    EndDialog(hWndDlg,IDCANCEL);
					return TRUE;
				}
				case IDC_START:
				{	bBlock=TRUE;
					if(mHpPak_ItemsList->WhatIsIt(0,FALSE)) // Param
					{	ChangeSelectedParam();
						mHpPak_AddIndex(GetHelpComStruct()->IndexTab,0);
					}
					else // Doc
                        mHpPak_AddIndex(GetHelpComStruct()->IndexTab,mHpPak_ItemsList->GetRangSelected());
					SetmHpListFromCtrl(hWndmHpFilesList,FALSE);
					imHpFileIndex=GetTopicFromTree();
					WriteRegisterKeys(1);
					mHpPak_SetAppInfoKeys(lpExeName,1,3);
                    mHpCtrl_IndexTask=NO;
                    EndDialog(hWndDlg,IDOK);
					return TRUE;
				}
				case IDC_CREATE_ADD:
                {   if(!IsOthermHelpOpenned(1))
					{	SetmHpListFromCtrl(hWndmHpFilesList,FALSE);
						mHpPak_SetAppInfoKeys(lpExeName,1,2);
						WriteRegisterKeys(2);
                        bAddOperation=TRUE;
                        EndDialog(hWndDlg,IDC_CREATE_ADD);
						return TRUE;
					}
					else
					{	// Openned mHelp interface found
						mHpPak_mHpMessage(hWndDlg,GetHelpComStruct()->hInstServ,IDS_MHELP_ALOPENNED,
										  NULL,0,0,MB_ICONEXCLAMATION);
					}
					break;
				}
				case IDC_EDIT_NUMREPEAT:
				{	char lpRepeat[10]={0};
					int iLoopCfg;
					if(HIWORD(wParam)==EN_KILLFOCUS)
					{	GetWindowText(hWndBoucle,lpRepeat,10);
						iLoopCfg=mHpPak_GetIntFromStr(lpRepeat,10,0);
						if(iLoopCfg==0) iLoopCfg=1;
						mHpPak_InitStr(lpRepeat,10);
						_snprintf(lpRepeat,10,"%d",iLoopCfg);
						SetWindowText(hWndBoucle,lpRepeat);
						SetRepeatView();
						SetParamListView();
					}
					break;
				}
				case IDC_EDIT_INDEX:
				{	if(HIWORD(wParam)==EN_CHANGE)
					{	char lpIndex[MAX_RS_SIZE]={0};
						char lpValidStr[MAX_RS_SIZE]={0};
						int Index;
						if(ListView_GetItemCount(hWndIndexList)>0)
						{	GetWindowText(hWndEditIndex,lpIndex,MAX_RS_SIZE);
							if(IsItAValidString(lpIndex,lpValidStr,MAX_RS_SIZE))
							{	ListBox_AddString(GetDlgItem(hWndDlg,IDC_HIDE_INDEXLIST),lpValidStr);
								Index=ListBox_FindString(GetDlgItem(hWndDlg,IDC_HIDE_INDEXLIST),0,lpValidStr);
								ListBox_DeleteString(GetDlgItem(hWndDlg,IDC_HIDE_INDEXLIST),Index);
								ListView_SetItemState(hWndIndexList,Index,LVIS_SELECTED,LVIS_SELECTED);
								ListView_EnsureVisible(hWndIndexList,Index,TRUE);
							}
						}
					}
					break;
				}
				case IDC_CHECK_MHELP: // Help & Assist option
				{	char lpLoop[100]={0};
					if(!SendDlgItemMessage(hWndDlg,IDC_CHECK_MHELP,BM_GETCHECK,0,0))
					{	SendDlgItemMessage(hWndDlg,IDC_CHECK_ASSIST,BM_SETCHECK,1,0);
						SendMessage(hWndIcon,STM_SETICON,(WPARAM)LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_STOPMOUSE)),0);
						EnableWindow(hWndSpeed,FALSE);
						GetHelpComStruct()->iMode=1;
						EnableWindow(hWndBoucle,TRUE);
					}
					else
					{	EnableWindow(hWndSpeed,TRUE);
						SendMessage(hWndIcon,STM_SETICON,(WPARAM)LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MOUSE)),0);
						if(SendDlgItemMessage(hWndDlg,IDC_CHECK_ASSIST,BM_GETCHECK,0,0)) GetHelpComStruct()->iMode=0;
						else GetHelpComStruct()->iMode=2;
						SetWindowText(hWndBoucle,ONE_STRING);
						GetHelpComStruct()->iNbBoucle=1;
						EnableWindow(hWndBoucle,FALSE);
						GetHelpComStruct()->listBtn->EmptyButtonList();
					}
					SetRepeatView();
					SetParamListView();
					break;
				}
				case IDC_CHECK_ASSIST: // Assist option
				{	if(!SendDlgItemMessage(hWndDlg,IDC_CHECK_ASSIST,BM_GETCHECK,0,0))
					{	SendDlgItemMessage(hWndDlg,IDC_CHECK_MHELP,BM_SETCHECK,1,0);
						EnableWindow(hWndSpeed,TRUE);
						SendMessage(hWndIcon,STM_SETICON,(WPARAM)LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MOUSE)),0);
						GetHelpComStruct()->iMode=2;
						SetWindowText(hWndBoucle,ONE_STRING);
						GetHelpComStruct()->iNbBoucle=1;
						EnableWindow(hWndBoucle,FALSE);
						GetHelpComStruct()->listBtn->EmptyButtonList();
					}
					else
					{	if(SendDlgItemMessage(hWndDlg,IDC_CHECK_MHELP,BM_GETCHECK,0,0)) GetHelpComStruct()->iMode=0;
						else
						{	SendMessage(hWndIcon,STM_SETICON,(WPARAM)LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_STOPMOUSE)),0);
							GetHelpComStruct()->iMode=1;
						}
						SetWindowText(hWndBoucle,ONE_STRING);
						GetHelpComStruct()->iNbBoucle=1;
						EnableWindow(hWndBoucle,FALSE);
						GetHelpComStruct()->listBtn->EmptyButtonList();
					}
					SetRepeatView();
					SetParamListView();
					break;
				}
				case IDC_RESET_AUTOASSIST:
				{	ListView_SetItemState(hWndFavList,NO,0,LVIS_SELECTED);
					mHpPak_FavsList->SetAutoAssist(NULL_STRING);
					SetWindowText(hWndEditAuto,NULL_STRING);
					LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
					SetWindowText(hWndAutoAssistCfg,lpRC);
					EnableWindow(hWndResetAuto,FALSE);
					SetFocus(hWndCustomAuto);
					break;
				}
				case IDC_SAVE_AUTOASSIST:
				{	char lpNewAuto[MAX_RS_SIZE]={0};
					GetWindowText(hWndEditAuto,lpNewAuto,MAX_RS_SIZE);
					mHpPak_FavsList->SetAutoAssist(lpNewAuto);
					if(strncmp(lpNewAuto,NULL_STRING,MAX_RS_SIZE)!=0)
					{	LoadString(GetHelpComStruct()->hInstServ,IDS_AUTOLNCH_YES,lpRC,MAX_KEY_SIZE);
						SetWindowText(hWndAutoAssistCfg,lpRC);
						EnableWindow(hWndResetAuto,TRUE);
					}
					else
					{	LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,MAX_KEY_SIZE);
						SetWindowText(hWndAutoAssistCfg,lpRC);
						EnableWindow(hWndResetAuto,FALSE);
					}
					break;
				}
				case IDC_RADIO_FAV:
				{	long lStle;
					ListView_SetItemState(hWndFavList,NO,0,LVIS_SELECTED);
					lStle=(long)GetWindowLongPtr(hWndFavList,GWL_STYLE);
					lStle|=LVS_SINGLESEL;
					SetWindowLongPtr(hWndFavList,GWL_STYLE,(LONG_PTR)lStle);
					if(mHpPak_iFavIndex!=NO) ListView_SetItemState(hWndFavList,mHpPak_iFavIndex,LVIS_SELECTED,LVIS_SELECTED);
					HideShowFavCtrls();
					break;
				}
				case IDC_RADIO_AUTOASSIST:
				{	mHpPak_iFavIndex=ListView_GetItemSel(hWndFavList);
					if(mHpPak_iFavIndex!=NO) ListView_SetItemState(hWndFavList,NO,0,LVIS_SELECTED);
					EnableWindow(hWndCustomAuto,FALSE);
					ReplaceAutoAssSel();
					HideShowFavCtrls();
					break;
				}
				case IDC_ADD_MHPFILES:
				{	int iCnt=GetOpenmHpFiles(hWndDlg,hWndmHpFilesList);
					if(iCnt!=NO)
					{	RefreshViewContent(hWndDlg,iCnt);}
					break;
				}
				case IDC_REMOVE_MHPFILES:
				{
#ifdef MHELP_OLD
                    int iTmp,iLag=0;
					BOOL bLoseSel=FALSE;
					HTREEITEM hTreeItm;
					for(int i=0;i<ListView_GetItemCount(hWndmHpFilesList);i++)
					{	if(ListView_GetItemState(hWndmHpFilesList,i,LVIS_SELECTED)==LVIS_SELECTED)
						{	if((TreeView_GetSelection(hWndTreeRS)!=NULL)&&(GetTopicFromTree()==i)) bLoseSel=TRUE;
							hTreeItm=TreeView_GetRoot(hWndTreeRS);
							iTmp=0;
							while((iTmp!=i)&&(hTreeItm!=NULL))
							{	hTreeItm=TreeView_GetNextItem(hWndTreeRS,hTreeItm,TVGN_NEXT);
								iTmp++;
							}
							if(hTreeItm!=NULL)
							{	TreeView_DeleteItem(hWndTreeRS,hTreeItm);
								ListView_DeleteItem(hWndmHpFilesList,i);
								GetHelpComStruct()->mHpFileList->RemovemHpFile(i);
								UpdateIndexList(hWndDlg,(i+iLag),FALSE);
								SendMessage(hWndProgress,PBM_SETPOS,0,0);
								iLag++;
								i--;
							}
						}
					}
					if(ListView_GetItemCount(hWndmHpFilesList)==0) GetHelpComStruct()->bmHpFileOpened=FALSE;
					if((bLoseSel)&&(GetHelpComStruct()->bmHpFileOpened))
					{	TreeView_SelectItem(hWndTreeRS,TreeView_GetRoot(hWndTreeRS));
						mHpPak_EmptyTab(GetHelpComStruct()->IndexTab,MAX_PART_TREE);
						EnableWindow(hWndStart,FALSE);
						EnableWindow(hWndDestroy,FALSE);
						EnableWindow(hWndAddFav,FALSE);
						mHpPak_ItemsList->EmptyList();
						SetChangeListView(hWndDlg);
					}
					if(!GetHelpComStruct()->bmHpFileOpened) 
					{	SetmHpTabItems(FALSE);
						HideShowmHpCtrls(hWndDlg);
						UpdateEditRS();
						EnableWindow(hWndStart,FALSE);
						EnableWindow(hWndDestroy,FALSE);
						EnableWindow(hWndAddFav,FALSE);
						LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_NOOPENEDFILE,lpRC,MAX_KEY_SIZE);
						SetWindowText(hWndStatus,lpRC);					
					}
					else
					{	SendMessage(hWndDlg,WM_COMMAND,MAKELONG(IDC_EDIT_INDEX,EN_CHANGE),(LPARAM)hWndEditIndex);
						LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_READY,lpRC,MAX_KEY_SIZE);
						SetWindowText(hWndStatus,lpRC);					
					}
#else
                    ListView_DeleteAllItems(hWndmHpFilesList);
                    GetHelpComStruct()->mHpFileList->RemovemHpFile(0);
                    UpdateIndexList(hWndDlg,0,FALSE);
                    SendMessage(hWndProgress,PBM_SETPOS,0,0);
                    GetHelpComStruct()->bmHpFileOpened=FALSE;
                    SetmHpTabItems(FALSE);
                    HideShowmHpCtrls(hWndDlg);
                    mHpPak_ItemsList->EmptyList();
                    UpdateEditRS();
                    DestroyWindow(hWndWebClient);
                    hWndWebClient=NULL;
                    EnableWindow(hWndStart,FALSE);
                    EnableWindow(hWndDestroy,FALSE);
                    EnableWindow(hWndAddFav,FALSE);
                    LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_NOOPENEDFILE,lpRC,MAX_KEY_SIZE);
                    SetWindowText(hWndStatus,lpRC);
                    EnableWindow(hWndOpenFavs,FALSE);
                    EnableWindow(hWndAddmHpFiles,TRUE);
#endif
					EnableWindow(hWndRemovemHpFiles,FALSE);
					SetFocus(hWndAddmHpFiles);
					break;
				}
				case IDC_SEARCH:
				{	char lpEdit[MAX_RS_SIZE]={0};
					char lpFind[MAX_RS_SIZE]={0};
					GetWindowText(hWndEditSearch,lpEdit,MAX_RS_SIZE);
					if(IsItAValidString(lpEdit,lpFind,MAX_RS_SIZE))
					{	UpdateSearchList(hWndDlg,lpFind);
						EnableWindow(hWndSetSearch,FALSE);
						if(ListView_GetItemCount(hWndSearchList)!=0) UpdateEditEvent(3);
						else
						{	LoadString(GetHelpComStruct()->hInstServ,IDS_STATUS_NORESULT,lpRC,MAX_KEY_SIZE);
							SetWindowText(hWndEditEvent,lpRC);					
						}
					}
					break;
				}
				case IDC_ADD_FAVORITES:
				{	AddRemoveFavList(hWndDlg,TRUE);
					EnableWindow(hWndRadioAuto,TRUE);
					break;
				}
				case IDC_REMOVE_FAVORITES:
				{	AddRemoveFavList(hWndDlg,FALSE);
					if(ListView_GetItemCount(hWndFavList)!=0) EnableWindow(hWndRadioAuto,TRUE);
					else EnableWindow(hWndRadioAuto,FALSE);
					SetFocus(hWndClose);
					break;
				}
				case IDC_SET_SEARCH:
				{	if(ListView_GetItemSel(hWndSearchList)!=NO)
					{	SetSelectedEvent(hWndDlg,TabCtrl_GetCurSel(hWndmHpTab));}
					break;
				}
				case IDC_SET_INDEX:
				{	if(ListView_GetItemSel(hWndIndexList)!=NO)
					{	SetSelectedEvent(hWndDlg,TabCtrl_GetCurSel(hWndmHpTab));}
					break;
				}
				case IDC_SET_FAVORITES:
				{	if(ListView_GetItemSel(hWndFavList)!=NO)
					{	SetSelectedEvent(hWndDlg,TabCtrl_GetCurSel(hWndmHpTab));}
					break;
				}
				case IDC_SAVE_FAVORITES:
				{	if(ListView_GetItemSel(hWndFavList)!=NO)
					{	SaveFavInmHpFile(hWndDlg,ListView_GetItemSel(hWndFavList));}
					break;
				}
				case IDC_OPEN_FAVORITES:
				{	int isel,iLastFavCnt;
					if(ListView_GetSelectedCount(hWndmHpFilesList)!=0)
					{	for(isel=0;isel<((int)ListView_GetItemCount(hWndmHpFilesList));isel++)
						{	if(ListView_GetItemState(hWndmHpFilesList,isel,LVIS_SELECTED)==LVIS_SELECTED)
							{	iLastFavCnt=mHpPak_FavsList->GetFavCount();
								OpenFavsFrommHpFile(hWndDlg,hWndmHpFilesList,isel);
								if(iLastFavCnt!=mHpPak_FavsList->GetFavCount())
								{	ReplaceAutoFavList(TRUE);}
							}
						}
					}
					break;
				}
				case IDC_EDIT_HOUR:
				{	if(HIWORD(wParam)==EN_KILLFOCUS)
					{	ReplaceDelayCfg();}
					break;
				}
				case IDC_EDIT_MINUTE:
				{	if(HIWORD(wParam)==EN_KILLFOCUS)
					{	ReplaceDelayCfg();}
					break;
				}
				case IDC_BUTTON_HELP:
				{	LaunchmHelpDoc(hWndDlg);
					break;
				}
				case IDC_DESTROY:
				{	char lpmHpPathFile[MAX_RS_SIZE]={0};
					int lpTabIndex[MAX_PART_TREE]={0};
					int lpLineTab[MAX_PART_TREE]={0};
					int lpDecalTab[MAX_PART_TREE]={0};
					HTREEITEM hItmRS;
					HTREEITEM hItmHide;
					HTREEITEM hDelItm;
					HTREEITEM hTmpItm;
					if(!IsOthermHelpOpenned(1))
					{	LoadString(GetHelpComStruct()->hInstServ,IDS_CONFIRM_DELETING,lpRC,MAX_KEY_SIZE);
						if(MessageBox(hWndDlg,lpRC,MHPPAK_DLGMSGTITLE,MB_ICONWARNING|MB_YESNO)==IDYES)
						{	imHpFileIndex=GetTopicFromTree();
							strncpy(lpmHpPathFile,GetHelpComStruct()->mHpFileList->GetmHpFilePath(imHpFileIndex),MAX_RS_SIZE);
							CopyIndexTab(lpTabIndex,MAX_PART_TREE);
							if(!mHpPak_ItemsList->WhatIsIt(0,FALSE))
							{	mHpPak_AddIndex(lpTabIndex,mHpPak_ItemsList->GetRangSelected());}
							mHpPak_AddIndex(lpTabIndex,0);
							ReplaceTree(hWndHiddenTree,lpTabIndex);
							mHpPak_CopyTab(lpTabIndex,MAX_PART_TREE,GetHelpComStruct()->IndexTab);
							if(!mHpPak_ItemsList->WhatIsIt(0,FALSE))
							{	mHpPak_AddIndex(lpTabIndex,mHpPak_ItemsList->GetRangSelected());}
							else mHpPak_AddIndex(lpTabIndex,0);
							GetDeletedTabs(lpLineTab,MAX_PART_TREE,lpDecalTab,MAX_PART_TREE);
							if(mHpPak_DestroyTaskFile(hWndDlg,lpmHpPathFile,lpLineTab,lpTabIndex,lpDecalTab))
							{	SetmHpCursor(hWndDlg,1);
								WriteFavsRegKey(lpExeName,mHpPak_FavsList);
								RemoveExistFavRegKey(lpmHpPathFile,lpTabIndex,lpDecalTab);
								//
								hItmRS=TreeView_GetSelection(hWndTreeRS);
								while(TreeView_GetParent(hWndTreeRS,hItmRS)!=NULL) hItmRS=TreeView_GetParent(hWndTreeRS,hItmRS);
								hDelItm=TreeView_GetChild(hWndTreeRS,hItmRS);
								while((hTmpItm=TreeView_GetNextItem(hWndTreeRS,hDelItm,TVGN_NEXT))!=NULL)
								{	TreeView_DeleteItem(hWndTreeRS,hTmpItm);}
								TreeView_DeleteItem(hWndTreeRS,hDelItm);
								hItmHide=TreeView_GetSelection(hWndHiddenTree);
								while(TreeView_GetParent(hWndHiddenTree,hItmHide)!=NULL) hItmHide=TreeView_GetParent(hWndHiddenTree,hItmHide);
								hDelItm=TreeView_GetChild(hWndHiddenTree,hItmHide);
								while((hTmpItm=TreeView_GetNextItem(hWndHiddenTree,hDelItm,TVGN_NEXT))!=NULL)
								{	TreeView_DeleteItem(hWndHiddenTree,hTmpItm);}
								TreeView_DeleteItem(hWndHiddenTree,hDelItm);
								//
								UpdateIndexList(hWndDlg,imHpFileIndex,FALSE);
								SendMessage(hWndProgress,PBM_SETPOS,0,0);
								CreateTree(hWndDlg,imHpFileIndex,hItmRS,hItmHide);
								SendMessage(hWndProgress,PBM_SETPOS,0,0);
								UpdateIndexList(hWndDlg,imHpFileIndex,TRUE);
								SendMessage(hWndProgress,PBM_SETPOS,0,0);
								RestoreFavsList();
								TreeView_Expand(hWndTreeRS,hItmRS,TVE_COLLAPSE);
								TreeView_Expand(hWndTreeRS,hItmRS,TVE_EXPAND);
								TreeView_SelectItem(hWndTreeRS,hItmRS);
								SetmHpCursor(hWndDlg,0);
							}
							else
							{	LoadString(GetHelpComStruct()->hInstServ,IDS_DELETING_FAILED,lpRC,MAX_KEY_SIZE);
								MessageBox(hWndDlg,lpRC,MHPPAK_DLGMSGTITLE,MB_ICONINFORMATION);
							}
						}
					}
					else
					{	// Openned mHelp interface found
						mHpPak_mHpMessage(hWndDlg,GetHelpComStruct()->hInstServ,IDS_MHELP_ALOPENNED,
										  NULL,0,0,MB_ICONEXCLAMATION);
					}
					break;
				}
			}
			break;
		}

	}
	return FALSE;
}

//-----------------------------------------------------//
// ParamDlgProc()
//
//-----------------------------------------------------//
LRESULT CALLBACK ParamDlgProc(HWND hWndParam,UINT pMessage,WPARAM pwParam,LPARAM plParam)
{	static BOOL bTamponCreate=FALSE;
	switch(pMessage)
	{	case WM_INITDIALOG:
		{	int iStyle;
			int iLast;
			LV_COLUMN lvcol;
			hWndType=GetDlgItem(hWndParam,IDC_EDIT_TYPE);
			hWndDecrit=GetDlgItem(hWndParam,IDC_EDIT_DECRIT);
			hWndLimite=GetDlgItem(hWndParam,IDC_EDIT_LIMITE);
			hWndEvolu=GetDlgItem(hWndParam,IDC_COMBO_TYPE);
			hWndAssoc=GetDlgItem(hWndParam,IDC_EDIT_ASSOC);
			hWndParamList=GetDlgItem(hWndParam,IDC_LIST_VALUE);
			hWndApply=GetDlgItem(hWndParam,IDC_APPLY);
			hWndCopier=GetDlgItem(hWndParam,IDC_COPIER);
			hWndColler=GetDlgItem(hWndParam,IDC_COLLER);
			hWndExt=GetDlgItem(hWndParam,IDC_CHECK_EXT);
			hWndRemoveParam=GetDlgItem(hWndParam,IDC_REMOVE_PARAM);
			hWndValidParam=GetDlgItem(hWndParam,IDC_VALID_PARAM);
			hWndCancelParam=GetDlgItem(hWndParam,IDCANCEL);
			hWndHelpParam=GetDlgItem(hWndParam,IDC_EVODLG_HELP);
			hWndStaticSel=GetDlgItem(hWndParam,IDC_STATIC_SELECTION);
			if(!bTamponCreate)
			{	TamponList=new CTypeList();
				ParamDlgDocLst=new CWndDocList();
				bTamponCreate=TRUE;
			}
			bTampon=FALSE;
			EnableWindow(hWndColler,FALSE);
			hWndValAssoc=GetDlgItem(hWndParam,IDC_STATIC_VALASSOC);
			MoveWindow(hWndApply,159,107,119,23,TRUE);
			LoadString(GetHelpComStruct()->hInstServ,IDS_COPY_FIRST_SEL,lpRC,MAX_KEY_SIZE);
			ComboBox_AddString(hWndEvolu,lpRC);
			LoadString(GetHelpComStruct()->hInstServ,IDS_COPY_PASTE,lpRC,MAX_KEY_SIZE);
			ComboBox_AddString(hWndEvolu,lpRC);
			if(mHpPak_ItemsList->GetParameterType(ItemIndex)==3)
			{	LoadString(GetHelpComStruct()->hInstServ,IDS_CHANGE_STATE,lpRC,MAX_KEY_SIZE);
				ComboBox_AddString(hWndEvolu,lpRC);
				LoadString(GetHelpComStruct()->hInstServ,IDS_EVERY_WHAT,lpRC,MAX_KEY_SIZE);
				SetWindowText(hWndValAssoc,lpRC);
			}
			else
			{	if(mHpPak_ItemsList->GetParameterType(ItemIndex)<5)
				{	LoadString(GetHelpComStruct()->hInstServ,IDS_INCREMENTATION,lpRC,MAX_KEY_SIZE);
					ComboBox_AddString(hWndEvolu,lpRC);
					if(mHpPak_ItemsList->GetParameterType(ItemIndex)!=4)
					{	LoadString(GetHelpComStruct()->hInstServ,IDS_DECREMENTATION,lpRC,MAX_KEY_SIZE);
						ComboBox_AddString(hWndEvolu,lpRC);
					}
				}
			}
			EnableWindow(hWndAssoc,FALSE);
			LoadString(GetHelpComStruct()->hInstServ,IDS_COPY_FIRST_SEL,lpRC,MAX_KEY_SIZE);
			ComboBox_SelectString(hWndEvolu,0,lpRC);
			EnableWindow(hWndRemoveParam,mHpPak_ItemsList->IsRepeating(ItemIndex));
			SetWindowText(hWndType,mHpPak_ItemsList->GetDetail(ItemIndex,4,0,lpRC,MAX_KEY_SIZE));
			SetWindowText(hWndDecrit,mHpPak_ItemsList->GetDetail(ItemIndex,2,0,lpRC,MAX_KEY_SIZE));
			SetWindowText(hWndLimite,mHpPak_ItemsList->GetDetail(ItemIndex,3,0,lpRC,MAX_KEY_SIZE));
			SetWindowText(hWndAssoc," ");
			MoveWindow(hWndAssoc,159,79,119,20,TRUE);
			MoveWindow(hWndStaticSel,255,82,25,20,TRUE);
			EnableWindow(hWndApply,FALSE);
			hImageParamList=ImageList_Create(16,16,ILC_MASK|ILC_COLOR16,6,1);
			ImageList_AddIcon(hImageParamList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_LOCKED)));
			ImageList_AddIcon(hImageParamList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_PARAM)));
			ListView_SetImageList(hWndParamList,hImageParamList,LVSIL_SMALL);
			iStyle=(int)GetWindowLongPtr(hWndParamList,GWL_STYLE);
			iStyle&=~(LVS_TYPEMASK);
			iStyle|=LVS_REPORT;
			SetWindowLongPtr(hWndParamList,GWL_STYLE,(LONG_PTR)iStyle);
			lvcol.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
			lvcol.fmt=LVCFMT_LEFT;
			lvcol.cx=300;
			lvcol.iSubItem=0;
			LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_VALUE,lpRC,MAX_KEY_SIZE);
			lvcol.pszText=lpRC;
			ListView_InsertColumn(hWndParamList,0,&lvcol);
			lvcol.cx=43;
			lvcol.iSubItem=1;
			LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_ROW,lpRC,MAX_KEY_SIZE);
			lvcol.pszText=lpRC;
			ListView_InsertColumn(hWndParamList,1,&lvcol);
			for(int i=0;i<GetHelpComStruct()->iNbBoucle;i++)
			{	if(mHpPak_ItemsList->GetDetail(ItemIndex,0,i,lpRC,MAX_KEY_SIZE)!=NULL)
				{	AddItemList(hWndParamList,mHpPak_ItemsList->GetDetail(ItemIndex,0,i,lpRC,MAX_KEY_SIZE),0,i);
					iLast=i;
				}
				else AddItemList(hWndParamList,mHpPak_ItemsList->GetDetail(ItemIndex,0,iLast,lpRC,MAX_KEY_SIZE),0,i);
			}
			if(ListView_GetItemCount(hWndParamList)>=10)
			{	LVCOLUMN pcol;
				pcol.mask=LVCF_WIDTH;
				pcol.cx=283;
				ListView_SetColumn(hWndParamList,0,&pcol);		
			}
			SetParamDlgDocLnch();
			return TRUE;
		}
		case WM_NOTIFY:
		{	if(((LPNMHDR)plParam)->idFrom==IDC_LIST_VALUE)
			{	switch(((LPNMHDR)plParam)->code)
				{	case LVN_GETDISPINFO:
					{	char lpRang[10]={0};
						int i=((LV_DISPINFO FAR*)plParam)->item.iItem;
						int j=((LV_DISPINFO FAR*)plParam)->item.iSubItem;
						if(j==0) ((LV_DISPINFO FAR*)plParam)->item.pszText=LPSTR_TEXTCALLBACK;
						else
						{	_snprintf(lpRang,10,"%d",(i+1));
							((LV_DISPINFO FAR*)plParam)->item.pszText=lpRang;
						}
						break;
					}
					case LVN_ITEMCHANGED:
					{	if(ListView_GetSelectedCount(hWndParamList)>=2)
						{	EnableWindow(hWndApply,TRUE);}
						else
						{	EnableWindow(hWndApply,FALSE);}
						if(ListView_GetSelectedCount(hWndParamList)>=1)
						{	EnableWindow(hWndCopier,TRUE);
							EnableWindow(hWndColler,bTampon);
						}
						else
						{	EnableWindow(hWndCopier,FALSE);
							EnableWindow(hWndColler,FALSE);
						}
						break;
					}
					case LVN_BEGINLABELEDIT:
					{	HWND hWndEditListVal=ListView_GetEditControl(hWndParamList);
						ParamDlgDocLst->AddDocWnd(hWndEditListVal,(LONG_PTR)ParamDlgDocProc);
						break;
					}
					case LVN_ENDLABELEDIT:
					{	if(((LV_DISPINFO FAR*)plParam)->item.mask==LVIF_TEXT)
						{	LV_ITEM LvItem=((LV_DISPINFO FAR*)plParam)->item;
							if((strncmp(LvItem.pszText,NULL_STRING,LvItem.cchTextMax)!=0)||
							   (mHpPak_ItemsList->GetParameterType(ItemIndex)==2)||
							   (mHpPak_ItemsList->GetParameterType(ItemIndex)==4)||
							   (mHpPak_ItemsList->GetParameterType(ItemIndex)==7)||
							   (mHpPak_ItemsList->GetParameterType(ItemIndex)==9))
							{	ListView_SetItem(hWndParamList,&LvItem);}
						}
						break;
					}
					case NM_DBLCLK:
					{	ListView_EditLabel(hWndParamList,ListView_GetNextItem(hWndParamList,NO,LVNI_SELECTED));
						break;
					}
				}
			}
			else return DefWindowProc(hWndParam,pMessage,pwParam,plParam);
			break;
		}
		case WM_MOUSEMOVE:
		{	SetmHpCursor(hWndParam,0);
			break;
		}
		case WM_COMMAND:
		{	switch(LOWORD(pwParam))
			{	case IDC_VALID_PARAM:
				{	char lpComp[MAX_RS_SIZE]={0};
					char lpText1[MAX_RS_SIZE]={0};
					BOOL bDiff=FALSE;
					int iNbitem=ListView_GetItemCount(hWndParamList);
					iNbitem--;
					ListView_GetItemText(hWndParamList,iNbitem,0,lpComp,MAX_RS_SIZE);
					iNbitem--;
					while((iNbitem>=0)&&(!bDiff))
					{	ListView_GetItemText(hWndParamList,iNbitem,0,lpText1,MAX_RS_SIZE);
						if(strncmp(lpComp,lpText1,MAX_RS_SIZE)) bDiff=TRUE;
						iNbitem--;
					}
					if(bDiff)
					{	mHpPak_ItemsList->AttachRepeatParam(ItemIndex,hWndParamList);
                        mHpCtrl_IndexTask=NO;
						return EndDialog(hWndParam,IDOK);
					}
					break;
				}
				case IDCANCEL: return EndDialog(hWndParam,IDCANCEL);
				case IDC_COMBO_TYPE:
				{	switch(HIWORD(pwParam))
					{	case CBN_SELCHANGE:
						{	MoveWindow(hWndAssoc,159,79,119,20,TRUE);
							switch(ComboBox_GetCurSel(hWndEvolu))
							{	case 0:
								{	ShowCtrl(0);
									EnableWindow(hWndAssoc,FALSE);
									SetWindowText(hWndAssoc," ");
									break;
								}
								case 1:
								{	ShowCtrl(1);
									if(ListView_GetSelectedCount(hWndParamList)<=0)
									{	EnableWindow(hWndCopier,FALSE);
										EnableWindow(hWndColler,FALSE);
									}
									else
									{	EnableWindow(hWndCopier,TRUE);
										EnableWindow(hWndColler,bTampon);
									}
									break;
								}
								case 2:
								case 3:
								case 4:
								case 5:
								case 6:
								case 7:
								{	if(((ComboBox_GetCurSel(hWndEvolu)==2)||(ComboBox_GetCurSel(hWndEvolu)==3))&&
									    (mHpPak_ItemsList->GetParameterType(ItemIndex)==4)) // S
									{	ShowCtrl(2);
									}
									else
									{	ShowCtrl(0);
										EnableWindow(hWndAssoc,TRUE);
										SetWindowText(hWndAssoc,ONE_STRING);
										if(ListView_GetSelectedCount(hWndParamList)>=2)
										{	EnableWindow(hWndApply,TRUE);}
										else
										{	EnableWindow(hWndApply,FALSE);}
									}
									if(mHpPak_ItemsList->GetParameterType(ItemIndex)==3) // B
									{	MoveWindow(hWndAssoc,159,79,89,20,TRUE);
										ShowWindow(hWndStaticSel,SW_SHOW);
									}
									break;
								}
							}
							break;
						}
					}
					break;
				}
				case IDC_REMOVE_PARAM:
				{	char lpText[MAX_RS_SIZE]={0};
					ListView_GetItemText(hWndParamList,0,0,lpText,MAX_RS_SIZE);			
					mHpPak_ItemsList->ModifyStrParam(ItemIndex,lpText);
					return EndDialog(hWndParam,IDC_REMOVE_PARAM);
				}
				case IDC_APPLY:
				{	int iSelOp=(int)ComboBox_GetCurSel(hWndEvolu);
					BOOL bSens;
					switch(iSelOp)
					{	case 0: // Copie 1° sel
						{	CopieFirstSel();
							break;
						}
						case 2: // Incrémente
						case 3: // Décrémente
						{	if(iSelOp==2) bSens=TRUE;
							else bSens=FALSE;
							// Entier
							if(!mHpPak_ItemsList->GetParameterType(ItemIndex)) IncrementeNumber(bSens,TRUE);
							// Réel
							if(mHpPak_ItemsList->GetParameterType(ItemIndex)==1) IncrementeNumber(bSens,FALSE);
							// Chaine de caractères
							if(mHpPak_ItemsList->GetParameterType(ItemIndex)==4)
							{	IncrementeString((BOOL)(SendMessage(hWndExt,BM_GETCHECK,0,0)));}
							// Booléen
							if(mHpPak_ItemsList->GetParameterType(ItemIndex)==3)
							{	ChangeBoolState();}
							break;
						}
					}
					break;
				}
				case IDC_COPIER:
				{	CopierEvolu();
					EnableWindow(hWndColler,TRUE);
					bTampon=TRUE;
					break;
				}
				case IDC_COLLER:
				{	CollerEvolu();
					break;
				}
				case IDC_EVODLG_HELP:
				{	mHpPak_OpenmHelpDoc(hWndParam,MHPPAK_DOC_EVOL);
					break;
				}
			}
		}
	}
	return FALSE;
}

//-----------------------------------------------------//
// Fichier FavsDlg.cpp
//
// 
//-----------------------------------------------------//

#include "stdafx.h"
#include "favsdlg.h"
#include <stdio.h>
#include "helpcom.h"
#include "resource.h"

//-----------------------------------------------------//
// Variables
char lpFileName[MAX_RS_SIZE]={0};
HWND hWndFavsFile=NULL;
HWND hWndRemoveFavFile=NULL;
HWND hWndAddFavorite=NULL;
HWND hWndStaticName=NULL;
HWND hWndFileName=NULL;
HWND hWndFavClose=NULL;
HWND hWndFavHelp=NULL;
HIMAGELIST hFavImgList=NULL;
CFavListe* FavsFileLst=NULL;
CWndDocList* FavDlgDocLst;

//-----------------------------------------------------//
// Déclarations
LRESULT CALLBACK FavDlgDocProc(HWND,UINT,WPARAM,LPARAM);
void SetFavDlgDocLnch();
void CreateFavsObj();
int CreateFavsDlg(HWND);
void FillFavsFileList();
LRESULT CALLBACK FavsDlgProc(HWND,UINT,WPARAM,LPARAM);

//-----------------------------------------------------------------------------------------------------------------------//
// FavDlgDocProc
//-----------------------------------------------------//
LRESULT CALLBACK FavDlgDocProc(HWND hWndDoc,UINT DocMessage,WPARAM DocWParam,LPARAM DocLParam)
{	LONG_PTR lPrevProc=FavDlgDocLst->GetDocWndProc(hWndDoc);
	switch(DocMessage)
	{	case WM_KEYUP:
		{	switch(DocWParam)
			{	case VK_F1:
				{	mHpPak_OpenmHelpDoc(hWndDoc,MHPPAK_DOC_FAV);
					break;
				}
				default:
				{	break;}
			}
			break;
		}
		case WM_NCDESTROY:
		{	FavDlgDocLst->RemoveDocWnd(hWndDoc);
			break;
		}
	}
	return CallWindowProc((WNDPROC)lPrevProc,hWndDoc,DocMessage,DocWParam,DocLParam);
}

//-----------------------------------------------------//
// SetFavDlgDocLnch
//-----------------------------------------------------//
void SetFavDlgDocLnch()
{	FavDlgDocLst->AddDocWnd(hWndFavsFile,(LONG_PTR)FavDlgDocProc);
	FavDlgDocLst->AddDocWnd(hWndRemoveFavFile,(LONG_PTR)FavDlgDocProc);
	FavDlgDocLst->AddDocWnd(hWndAddFavorite,(LONG_PTR)FavDlgDocProc);
	FavDlgDocLst->AddDocWnd(hWndFileName,(LONG_PTR)FavDlgDocProc);
	FavDlgDocLst->AddDocWnd(hWndFavClose,(LONG_PTR)FavDlgDocProc);
	FavDlgDocLst->AddDocWnd(hWndFavHelp,(LONG_PTR)FavDlgDocProc);
}

//-----------------------------------------------------//
// CreateFavsObj
//-----------------------------------------------------//
void CreateFavsObj()
{	static BOOL bCreateObj=FALSE;
	if(!bCreateObj)
	{	FavsFileLst=new CFavListe();
		bCreateObj=TRUE;
	}
}	

//-----------------------------------------------------//
// CreateFavsDlg
//-----------------------------------------------------//
int CreateFavsDlg(HWND hWndActive)
{	FARPROC lpfnmHpDlg;
	int iResult;
	// creation de la boite de dialogue MHP Help
    lpfnmHpDlg=MakeProcInstance((FARPROC)FavsDlgProc,GetHelpComStruct()->hInstServ);
	iResult=(int)DialogBox(GetHelpComStruct()->hInstServ,
                           MAKEINTRESOURCE(IDD_DLG_FAVSFILE),
                           hWndActive,
                           (DLGPROC)FavsDlgProc);
    FreeProcInstance(lpfnmHpDlg);
	// Test la création de la boite de dialogue
	return iResult;
}

//-----------------------------------------------------//
// FillFavsFileList
//-----------------------------------------------------//
void FillFavsFileList()
{	char lplv_itm[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpRC[255]={0};
	int indice;
	LV_ITEM lvi;
	lvi.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvi.state=lvi.stateMask=0;
	lvi.cchTextMax=MAX_RS_SIZE;
	lvi.pszText=lplv_itm;
	ListView_DeleteAllItems(hWndFavsFile);
	for(int i=0;i<FavsFileLst->GetFavCount();i++)
	{	lvi.iItem=i;
		lvi.iSubItem=0;
		strncpy(lvi.pszText,FavsFileLst->GetEventStr(i),MAX_RS_SIZE);
		if(FavsFileLst->GetItemFromRang(i)->FavItem->ParamList!=NULL)
		{	lvi.iImage=3;
			for(indice=0;indice<FavsFileLst->GetItemFromRang(i)->FavItem->ParamList->GetSelCount();indice++)
			{	if(FavsFileLst->GetItemFromRang(i)->FavItem->ParamList->GetCellFromItem(FavsFileLst->GetItemFromRang(i)->FavItem->ParamList->GetSelItem(indice))->EvoluItem->GetEvoluFlag())
				{	lvi.iImage=6;}
			}
		}
		else lvi.iImage=1;
		ListView_InsertItem(hWndFavsFile,&lvi);
		lvi.iSubItem=1;
		if(FavsFileLst->GetLoopOption(i)>1)
		{	_snprintf(lpTmp,MAX_RS_SIZE,"%d",FavsFileLst->GetLoopOption(i));
			strncpy(lvi.pszText,lpTmp,MAX_RS_SIZE);
		}
		else
		{	LoadString(GetHelpComStruct()->hInstServ,IDS_NOREPEAT_OPTION,lpRC,255);
			strncpy(lvi.pszText,lpRC,MAX_RS_SIZE);
		}
		ListView_SetItem(hWndFavsFile,&lvi);
		mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
		_snprintf(lpTmp,MAX_RS_SIZE,"%d",(i+1));
		strncpy(lvi.pszText,lpTmp,MAX_RS_SIZE);
		lvi.iSubItem=2;
		ListView_SetItem(hWndFavsFile,&lvi);
	}
}

//-----------------------------------------------------//
// FavsDlgProc()
//
//-----------------------------------------------------//
LRESULT CALLBACK FavsDlgProc(HWND hWndFavsDlg,UINT DlgMessage,WPARAM wParam,LPARAM lParam)
{	static BOOL bPass=FALSE;
	switch(DlgMessage)
	{	case WM_INITDIALOG:
		{	char lpRc[255]={0};
			LV_COLUMN lvc;
			lvc.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
			lvc.fmt=LVCFMT_LEFT;
			lvc.cchTextMax=255;
			if(!bPass)
			{	FavDlgDocLst=new CWndDocList();
				bPass=TRUE;
			}
			hWndFavsFile=GetDlgItem(hWndFavsDlg,IDC_LIST_FAVSFILE);
			hWndRemoveFavFile=GetDlgItem(hWndFavsDlg,IDC_REMOVE_FAVFILE);
			hWndAddFavorite=GetDlgItem(hWndFavsDlg,IDC_ADD_FAVFILE);
			hWndStaticName=GetDlgItem(hWndFavsDlg,IDC_STATIC_FILENAME);
			hWndFileName=GetDlgItem(hWndFavsDlg,IDC_EDIT_FILENAME);
			hWndFavClose=GetDlgItem(hWndFavsDlg,IDCANCEL);
			hWndFavHelp=GetDlgItem(hWndFavsDlg,IDC_FAVDLG_HELP);
			if(hFavImgList!=NULL) ImageList_Destroy(hFavImgList);
			hFavImgList=ImageList_Create(16,16,ILC_MASK|ILC_COLOR16,6,1);
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_RSREP)));
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_TASK)));
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_PARAM_REP)));
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_PARAM)));
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_REP)));
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_PARAM)));
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_MHP_LOCKED)));
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_BOOK)));
			ImageList_AddIcon(hFavImgList,LoadIcon(GetHelpComStruct()->hInstServ,MAKEINTRESOURCE(IDI_ICON_OP_BOOK)));
			ListView_SetImageList(hWndFavsFile,hFavImgList,LVSIL_SMALL);
			lvc.iSubItem=0;
			lvc.cx=263;
			LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_EVENT,lpRc,255);
			lvc.pszText=lpRc;
			ListView_InsertColumn(hWndFavsFile,0,&lvc);
			lvc.iSubItem=1;
			lvc.cx=70;
			LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_REPEATITION,lpRc,255);
			lvc.pszText=lpRc;
			ListView_InsertColumn(hWndFavsFile,1,&lvc);
			lvc.iSubItem=2;
			lvc.cx=38;
			LoadString(GetHelpComStruct()->hInstServ,IDS_COLUMN_ROW,lpRc,255);
			lvc.pszText=lpRc;
			ListView_InsertColumn(hWndFavsFile,2,&lvc);
			SetWindowText(hWndFileName,lpFileName);
			//-----------------------------------------------//
			EnableWindow(hWndRemoveFavFile,FALSE);
			EnableWindow(hWndAddFavorite,FALSE);
			FillFavsFileList();
			SetFavDlgDocLnch();
			return TRUE;
		}
		case WM_NOTIFY:
		{	if(((LPNMHDR)lParam)->idFrom==IDC_LIST_FAVSFILE)
			{	switch(((LPNMHDR)lParam)->code)
				{	case LVN_ITEMCHANGED:
					{	if(ListView_GetItemSel(hWndFavsFile)!=NO)
						{	EnableWindow(hWndRemoveFavFile,TRUE);
							EnableWindow(hWndAddFavorite,TRUE);
						}
						else
						{	EnableWindow(hWndRemoveFavFile,FALSE);
							EnableWindow(hWndAddFavorite,FALSE);
						}
						break;
					}
					case NM_DBLCLK:
					{	break;
					}
				}
			}
			else return DefWindowProc(hWndFavsDlg,DlgMessage,wParam,lParam);
			break;
		}
		case WM_COMMAND:
		{	switch(LOWORD(wParam))
			{	case IDCANCEL:
				{	EndDialog(hWndFavsDlg,IDCANCEL);
					return TRUE;
				}
				case IDC_REMOVE_FAVFILE:
				{	int LastCnt=FavsFileLst->GetFavCount();
					if(ListView_GetItemSel(hWndFavsFile)!=NO)
					{	FavsFileLst->RemoveFavItem(ListView_GetItemSel(hWndFavsFile));
						if(mHpCtrl_WriteFavsFile(hWndFavsDlg,lpFileName,FavsFileLst,LastCnt))
						{	mHpPak_mHpMessage(hWndFavsDlg,GetHelpComStruct()->hInstServ,IDS_FAVFILE_DESTROYED,NULL,0,0,MB_ICONINFORMATION);
							FillFavsFileList();
							EnableWindow(hWndRemoveFavFile,FALSE);
							EnableWindow(hWndAddFavorite,FALSE);
							SetFocus(hWndFavClose);
						}
					}
					break;
				}
				case IDC_ADD_FAVFILE:
				{	char lpRow[100]={0};
					CEvoluList* AddEvolu=NULL;
					int iAdd,iFavSel=ListView_GetItemSel(hWndFavsFile);
					if(iFavSel!=NO)
					{	if(FavsFileLst->GetItemFromRang(iFavSel)->FavItem->ParamList!=NULL)
						{	AddEvolu=new CEvoluList();
							AddEvolu->CopyEvoluLst(FavsFileLst->GetItemFromRang(iFavSel)->FavItem->ParamList);
						}
						iAdd=mHpPak_FavsList->AddFavItem(FavsFileLst->GetItemFromRang(iFavSel)->FavItem->iRefTab,
														FavsFileLst->GetLoopOption(iFavSel),
														FavsFileLst->GetmHpPath(iFavSel),
														FavsFileLst->GetEventStr(iFavSel),
														AddEvolu,TRUE);
						if(iAdd!=NO)
						{	_snprintf(lpRow,100,"%d",(iAdd+1));
							mHpPak_mHpMessage(hWndFavsDlg,GetHelpComStruct()->hInstServ,IDS_FAVORITE_EXIST,lpRow,100,0,MB_ICONEXCLAMATION);
							if(AddEvolu!=NULL)
							{	AddEvolu->EmptyEvoluList();
								delete(AddEvolu);
							}
						}
						else
						{	_snprintf(lpRow,100,"%d",mHpPak_FavsList->GetFavCount());
							mHpPak_mHpMessage(hWndFavsDlg,GetHelpComStruct()->hInstServ,IDS_FAVFILE_ADDED,lpRow,100,0,MB_ICONINFORMATION);
						}
					}
					break;
				}
				case IDC_FAVDLG_HELP:
				{	mHpPak_OpenmHelpDoc(hWndFavsDlg,MHPPAK_DOC_FAV);
					break;
				}
			}
			break;
		}

	}
	return FALSE;
}

//****************************************************************************************************************GENERAL//
// SaveFavInmHpFile
//-----------------------------------------------------//
void SaveFavInmHpFile(HWND hWndActive,int iFav)
{	CEvoluList* WriteEvolu=NULL;
	int iRes,LastCount;
	CreateFavsObj();
	if(mHpPak_IsFilePathExist(hWndActive,mHpPak_FavsList->GetmHpPath(iFav)))
	{	if(mHpPak_GetFavoritesFile(hWndActive,mHpPak_FavsList->GetmHpPath(iFav),FavsFileLst))
		{	LastCount=FavsFileLst->GetFavCount();
			if(mHpPak_FavsList->GetItemFromRang(iFav)->FavItem->ParamList!=NULL)
			{	WriteEvolu=new CEvoluList();
				WriteEvolu->CopyEvoluLst(mHpPak_FavsList->GetItemFromRang(iFav)->FavItem->ParamList);
			}
			iRes=FavsFileLst->AddFavItem(mHpPak_FavsList->GetItemFromRang(iFav)->FavItem->iRefTab,
										 mHpPak_FavsList->GetLoopOption(iFav),
										 mHpPak_FavsList->GetmHpPath(iFav),
										 mHpPak_FavsList->GetEventStr(iFav),
										 WriteEvolu,TRUE);
			if(iRes!=NO)
			{	mHpPak_mHpMessage(hWndActive,GetHelpComStruct()->hInstServ,IDS_FAVFILE_EXIST,
								  mHpPak_FavsList->GetmHpPath(iFav),MAX_RS_SIZE,0,MB_ICONEXCLAMATION);
				if(WriteEvolu!=NULL)
				{	WriteEvolu->EmptyEvoluList();
					delete(WriteEvolu);
				}
			}
			else if(mHpCtrl_WriteFavsFile(hWndActive,mHpPak_FavsList->GetmHpPath(iFav),FavsFileLst,LastCount))
			{	mHpPak_mHpMessage(hWndActive,GetHelpComStruct()->hInstServ,IDS_FAVFILE_SAVED,
								  mHpPak_FavsList->GetmHpPath(iFav),MAX_RS_SIZE,0,MB_ICONINFORMATION);
			}
			FavsFileLst->EmptyFavList();
		}
	}
}

//-----------------------------------------------------//
// OpenFavsFrommHpFile
//-----------------------------------------------------//
void OpenFavsFrommHpFile(HWND hWndActive,HWND hWndFileLst,int iFileSel)
{	char lpmHpFilePath[MAX_RS_SIZE]={0};
	CreateFavsObj();
	GetmHpFileName(hWndFileLst,iFileSel,lpmHpFilePath,MAX_RS_SIZE);
	if(mHpPak_IsFilePathExist(hWndActive,lpmHpFilePath))
	{	if(mHpPak_GetFavoritesFile(hWndActive,lpmHpFilePath,FavsFileLst))
		{	if(FavsFileLst->GetFavCount()>=1)
			{	strncpy(lpFileName,lpmHpFilePath,MAX_RS_SIZE);
				CreateFavsDlg(hWndActive);
			}
			else
			{	mHpPak_mHpMessage(hWndActive,GetHelpComStruct()->hInstServ,IDS_NO_FAVSFILE,
								lpmHpFilePath,MAX_RS_SIZE,0,MB_ICONEXCLAMATION);
			}
		}
	}
}

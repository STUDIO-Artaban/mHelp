//-----------------------------------------------------//
// Fichier HelpCom.cpp
//
// 
//-----------------------------------------------------//

#include "stdafx.h"
#include "helpcom.h"

#include <stdlib.h>
#include <stdio.h>
#include <commdlg.h>

//-----------------------------------------------------//
// Variables globales
HELPCOMSTRUCT* HlpComStruct;
int imHpCursor=0;

//-----------------------------------------------------//
// Déclarations
void ModifyIndex(int*,int);

// class CmHpFileCell /////////////////////////////////////////////////////////
CmHpFileCell::CmHpFileCell(char* lpPath,BOOL bNew)
{	mHpFilePath=(char*)malloc(MAX_RS_SIZE);
	strncpy(mHpFilePath,lpPath,MAX_RS_SIZE);
	bNewFile=bNew;
	NextmHpFile=NULL;
}

CmHpFileCell::~CmHpFileCell()
{	free(mHpFilePath);}

// class CmHpFileList /////////////////////////////////////////////////////////
CmHpFileList::CmHpFileList()
{	mHpFileHead=NULL;}

void CmHpFileList::AddmHpFile(char* lpNewPath,BOOL bNewmHp)
{	CmHpFileCell* temp=mHpFileHead;
	if(mHpFileHead==NULL) mHpFileHead=new CmHpFileCell(lpNewPath,bNewmHp);
	else
	{	while(temp->NextmHpFile!=NULL) temp=temp->NextmHpFile;
		temp->NextmHpFile=new CmHpFileCell(lpNewPath,bNewmHp);
	}
}

BOOL CmHpFileList::RemovemHpFile(int iPos)
{	int iRang=0;
	CmHpFileCell* Current=mHpFileHead;
	CmHpFileCell* Previous=mHpFileHead;
	while((iRang!=iPos)&&(Current!=NULL))
	{	if(iRang!=0) Previous=Current;
		Current=Current->NextmHpFile;
		iRang++;
	}
	if(Current==NULL) return FALSE;
	if(Current==Previous) mHpFileHead=mHpFileHead->NextmHpFile;
	else Previous->NextmHpFile=Current->NextmHpFile;
	delete Current;
	return TRUE;
}

BOOL CmHpFileList::IsmHpFileExist(int iPos)
{	int iRang=0;
	CmHpFileCell* temp=mHpFileHead;
	while((iRang!=iPos)&&(temp!=NULL))
	{	temp=temp->NextmHpFile;
		iRang++;
	}
	if(temp==NULL) return FALSE;
	return TRUE;
}

char* CmHpFileList::GetmHpFilePath(int iPos)
{	int iRang=0;
	CmHpFileCell* temp=mHpFileHead;
	while((iRang!=iPos)&&(temp!=NULL))
	{	temp=temp->NextmHpFile;
		iRang++;
	}
	if(temp==NULL) return NULL;
	return (temp->mHpFilePath);
}

BOOL CmHpFileList::GetNewFile(int iPos)
{	int iRang=0;
	CmHpFileCell* temp=mHpFileHead;
	while((iRang!=iPos)&&(temp!=NULL))
	{	temp=temp->NextmHpFile;
		iRang++;
	}
	if(temp==NULL) return FALSE;
	return (temp->bNewFile);
}

void CmHpFileList::EmptymHpFileList()
{	CmHpFileCell* temp=mHpFileHead;
	while(mHpFileHead!=NULL)
	{	mHpFileHead=mHpFileHead->NextmHpFile;
		delete temp;
		temp=mHpFileHead;
	}
}

//class CModifyParam //////////////////////////////////////////////////////////
CModifyParam::CModifyParam(char *NewParam,int iRang)
{	RangPos=iRang;
	ModifParam=(char*)malloc(MAX_RS_SIZE);
	strncpy(ModifParam,NewParam,MAX_RS_SIZE);
	ParamSuivant=NULL;
}

CModifyParam::~CModifyParam()
{	free(ModifParam);}

//class CParamCell ////////////////////////////////////////////////////////////
CParamCell::CParamCell(int *NewRef)
{	mHpPak_CopyTab(IndexRef,MAX_PART_TREE,NewRef);
	ParamList=NULL;
	CellSuivant=NULL;
}

BOOL CParamCell::IsSameRef(int *CompRef)
{	int indice=0;
	while((CompRef[indice]!=NO)&&(IndexRef[indice]!=NO))
	{	if(CompRef[indice]!=IndexRef[indice]) return FALSE;
		indice++;
	}
	if((CompRef[indice]==NO)&&(IndexRef[indice]==NO)) return TRUE;
	else return FALSE;
}

//class CModifyList ///////////////////////////////////////////////////////////
CModifyList::CModifyList()
{	ModifyTete=NULL;}

BOOL CModifyList::AddParam(BOOL bWhere,int *RefTab,
						   char *NewParam,int NewPos)
{	CParamCell *NewCell;
	CModifyParam *ParamNew;
	CParamCell *temp=ModifyTete;
	if(ModifyTete==NULL)
	{	NewCell=new CParamCell(RefTab);
		ParamNew=new CModifyParam(NewParam,NewPos);
		NewCell->ParamList=ParamNew;
		ModifyTete=NewCell;
	}
	else
	{	NewCell=ModifyTete;
		while((NewCell!=NULL)&&(!NewCell->IsSameRef(RefTab)))
			NewCell=NewCell->CellSuivant;
		if(NewCell==NULL)
		{	NewCell=new CParamCell(RefTab);
			while(temp->CellSuivant!=NULL)
				temp=temp->CellSuivant;
			temp->CellSuivant=NewCell;
			ParamNew=new CModifyParam(NewParam,NewPos);
			temp->ParamList=ParamNew;
		}
		else
		{	CModifyParam *find=NewCell->ParamList;
			while((find!=NULL)&&(find->RangPos!=NewPos))
				find=find->ParamSuivant;
			if(find==NULL)
			{	find=NewCell->ParamList;
				while(find->ParamSuivant!=NULL)
					find=find->ParamSuivant;
				ParamNew=new CModifyParam(NewParam,NewPos);
				find->ParamSuivant=ParamNew;
			}
			else if(!strncmp(find->ModifParam,NewParam,MAX_RS_SIZE))
			{	CModifyParam *Preced=NewCell->ParamList;
				if(Preced==find) NewCell->ParamList=find->ParamSuivant;
				else
				{	while(Preced->ParamSuivant!=find)
						Preced=Preced->ParamSuivant;
					Preced->ParamSuivant=find->ParamSuivant;
				}
				delete find;
				if(NewCell->ParamList==NULL)
				{	if(temp==NewCell) ModifyTete=temp->CellSuivant;
					else 
					{	while(temp->CellSuivant!=NewCell)
							temp=temp->CellSuivant;
						temp->CellSuivant=NewCell->CellSuivant;
					}
					delete temp;
				}
			}
			else if(bWhere) return FALSE;
			else strncpy(find->ModifParam,NewParam,MAX_RS_SIZE);
		}
	}
	return TRUE;
}

char* CModifyList::GetModifParam(int *RefTab,int iRang)
{	CParamCell *temp=ModifyTete;
	while((temp!=NULL)&&(!temp->IsSameRef(RefTab)))
		temp=temp->CellSuivant;
	if(temp==NULL) return NULL;
	else 
	{	CModifyParam *find=temp->ParamList;
		while((find!=NULL)&&(find->RangPos!=iRang))
			find=find->ParamSuivant;
		if(find==NULL) return NULL;
		else return (find->ModifParam);
	}
}

BOOL CModifyList::IsItEmpty(int *RefTab,int iRang)
{	CParamCell *temp=ModifyTete;
	CModifyParam *find;
	if(temp==NULL) return TRUE;
	else
	{	while((temp!=NULL)&&(!temp->IsSameRef(RefTab))) temp=temp->CellSuivant;
		if(temp==NULL) return TRUE;
		else
		{	find=temp->ParamList;
			while((find!=NULL)&&(find->RangPos!=iRang)) find=find->ParamSuivant;
			if(find==NULL) return TRUE;
			else return FALSE;
		}
	}
}

void CModifyList::EmptyModifList()
{	CParamCell *temp=ModifyTete;
	CModifyParam *temp1;
	while(temp!=NULL)
	{	ModifyTete=ModifyTete->CellSuivant;
		temp1=temp->ParamList;
		while(temp1!=NULL)
		{	temp->ParamList=temp->ParamList->ParamSuivant;
			delete temp1;
			temp1=temp->ParamList;
		}
		delete temp;
		temp=ModifyTete;
	}
}

//class CButtonCell ///////////////////////////////////////////////////////////
CButtonCell::CButtonCell(int iRang,HWND hWnd)
{	IndexPos=iRang;
	hWndButton=hWnd;
	NextButton=NULL;
}

//class CButtonList ///////////////////////////////////////////////////////////
CButtonList::CButtonList()
{	ButtonList=NULL;}

void CButtonList::EmptyButtonList()
{	CButtonCell *temp=ButtonList;
	while(ButtonList!=NULL)
	{	ButtonList=ButtonList->NextButton;
		DestroyWindow(temp->hWndButton);
		delete temp;
		temp=ButtonList;
	}
}

BOOL CButtonList::AddButton(int iIndex,long lyPos,HWND hWndRSList)
{	CButtonCell *NewCell;
	RECT Rect;
	int iDecal;
	GetWindowRect(hWndRSList,&Rect);
	if(ListView_GetCountPerPage(hWndRSList)<ListView_GetItemCount(hWndRSList)) iDecal=37; //33;
	else iDecal=21; //17;
	HWND hWndtemp=CreateWindowEx(WS_EX_TOPMOST|WS_EX_TRANSPARENT,
							"Button","...",
							WS_VISIBLE|WS_CHILD,
							(int)((Rect.right-Rect.left)-iDecal),(int)(lyPos),17,17,
							hWndRSList,NULL,GetHelpComStruct()->hInstServ,0);
	if(hWndtemp!=NULL)
	{	NewCell=new CButtonCell(iIndex,hWndtemp);
		if(ButtonList==NULL) ButtonList=NewCell;
		else
		{	CButtonCell *temp=ButtonList;
			while(temp->NextButton!=NULL) temp=temp->NextButton;
			temp->NextButton=NewCell;
		}
		return TRUE;
	}
	return FALSE;
}

void CButtonList::RemoveButton(int iIndex)
{	CButtonCell *find=ButtonList;
	while(find!=NULL)
	{	if(find->IndexPos==iIndex)
		{	CButtonCell *move=ButtonList;
			if(move==find)
			{	ButtonList=move->NextButton;
				DestroyWindow(move->hWndButton);
				delete move;
			}
			else
			{	while(move->NextButton!=find) move=move->NextButton;
				move->NextButton=find->NextButton;
				DestroyWindow(find->hWndButton);
				delete find;
			}
			return;
		}
		find=find->NextButton;
	}
}

BOOL CButtonList::bExistButton(int iWhat)
{	if((iWhat==NO)&&(ButtonList!=NULL)) return TRUE;
	if(iWhat!=NO)
	{	CButtonCell *find=ButtonList;
		while(find!=NULL)
		{	if(find->IndexPos==iWhat) return TRUE;
			find=find->NextButton;
		}
	}
	return FALSE;
}

int CButtonList::GetIndexFromWnd(HWND hWndBtnCtrl)
{	CButtonCell *find=ButtonList;
	while(find!=NULL)
	{	if(find->hWndButton==hWndBtnCtrl) return (find->IndexPos);
		find=find->NextButton;
	}
	return NO;
}

// class CItmCreate ///////////////////////////////////////////////////////////
CItmCreate::CItmCreate(int *TabRef,int iType)
{	mHpPak_InitStr(lpRS,MAX_RS_SIZE);
	mHpPak_CopyTab(IndxRf,MAX_PART_TREE,TabRef);
	ItmType=iType;
	ItmSuivant=NULL;
}

//class CItmList //////////////////////////////////////////////////////////////
CItmList::CItmList()
{	ItmTete=NULL;}

void CItmList::AddItm(int iType,int* IndexServTab)
{	CItmCreate *NewItm=new CItmCreate(IndexServTab,iType);
	CItmCreate *LastItm=ItmTete;
	if(ItmTete==NULL) ItmTete=NewItm;
	else
	{	while(LastItm->ItmSuivant!=NULL)
			LastItm=LastItm->ItmSuivant;
		LastItm->ItmSuivant=NewItm;
	}
}

CItmCreate* CItmList::GetCellFromRefTab(int *Reftab)
{	int indice;
	BOOL bEnd;
	CItmCreate *ItmTmp=ItmTete;
	while(ItmTmp!=NULL)
	{	indice=0;
		bEnd=FALSE;
		while((!bEnd)&&(Reftab[indice]!=NO)&&(ItmTmp->IndxRf[indice]!=NO))
		{	if(Reftab[indice]!=ItmTmp->IndxRf[indice]) bEnd=TRUE;
			indice++;
		}
		if((!bEnd)&&(Reftab[indice]==NO)&&(ItmTmp->IndxRf[indice]==NO)) 
		{	return ItmTmp;}
		ItmTmp=ItmTmp->ItmSuivant;
	}
	return NULL;
}

void CItmList::bExistItm(int *RefTab,int &iType,BOOL &bExist)
{	CItmCreate *Tmp=this->GetCellFromRefTab(RefTab);
	if(Tmp!=NULL)
	{	bExist=TRUE;
		iType=Tmp->ItmType;
	}
	else bExist=FALSE;
}

void CItmList::GetSetStrRS(int *RefTab,char *lprs,int rsSize,BOOL bGetSet)
{	CItmCreate *Temp=this->GetCellFromRefTab(RefTab);
	if(Temp!=NULL)
	{	if(bGetSet) strncpy(lprs,Temp->lpRS,rsSize);
		else strncpy(Temp->lpRS,lprs,MAX_RS_SIZE);
	}
	else if(bGetSet) lprs=NULL;
}

void CItmList::EmptyItmList()
{	CItmCreate *ItmTmp=ItmTete;
	while(ItmTmp!=NULL)
	{	ItmTete=ItmTete->ItmSuivant;
		delete ItmTmp;
		ItmTmp=ItmTete;
	}
}

BOOL CItmList::RemoveItm(int* Tab)
{	CItmCreate *Tmp=this->GetCellFromRefTab(Tab);
	CItmCreate *PreTmp=ItmTete;
	if(Tmp!=NULL)
	{	if(Tmp==PreTmp)
		{	PreTmp=ItmTete->ItmSuivant;
			delete ItmTete;
			ItmTete=PreTmp;
		}
		else
		{	while(PreTmp->ItmSuivant!=Tmp) PreTmp=PreTmp->ItmSuivant;
			PreTmp->ItmSuivant=Tmp->ItmSuivant;
			delete Tmp;
		}
		return TRUE;
	}
	return FALSE;
}

void CItmList::ChangeType(int *refTab,int iNewType)
{	CItmCreate *Tmp=this->GetCellFromRefTab(refTab);
	if(Tmp!=NULL) Tmp->ItmType=iNewType;
}

BOOL CItmList::IsEmpty()
{	if(ItmTete!=NULL) return FALSE;
	return TRUE;
}

//-----------------------------------------------------------------------------------------------------------------------//
// ModifyIndex
//-----------------------------------------------------//
void ModifyIndex(int *Tab,int NewIndex)
{	int indice=0;
	while(Tab[indice]!=NO) indice++;
	if(indice!=0) Tab[(indice-1)]=NewIndex;
}

//****************************************************************************************************************GENERAL//
// InverseTab
//-----------------------------------------------------//
void InverseTab(int* IndexTab,BOOL bGetLast)
{	int iTemp;
	int iEnd=0;
	int iBegin=0;
	while(IndexTab[iEnd]!=NO) iEnd++;
	iEnd--;
	if(!bGetLast)
	{	IndexTab[iEnd]=NO;
		iEnd--;
	}
	if((iEnd==NO)||(!iEnd)) return;
	else while(1)
	{	iTemp=IndexTab[iEnd];
		IndexTab[iEnd]=IndexTab[iBegin];
		IndexTab[iBegin]=iTemp;
		if((iBegin+1)==iEnd) return;
		else
		{	iBegin++;
			if((iBegin+1)==iEnd) return;
			else iEnd--;
		}
	}
}

//-----------------------------------------------------//
// GetIndexFromTree
//-----------------------------------------------------//
void GetIndexFromTree(HTREEITEM hTreeParam,HWND phWndTreeRS,int* IndexTab)
{	HTREEITEM hCurrent,hTreeRS,hTemp;
	int Temp;
	hTreeRS=TreeView_GetNextItem(phWndTreeRS,hTreeParam,TVGN_PREVIOUS);
	while(hTreeRS!=NULL)
	{	Temp=mHpPak_GetIndex(IndexTab);
		Temp++;
		ModifyIndex(IndexTab,Temp);
		hTemp=hTreeRS;
		hTreeRS=TreeView_GetNextItem(phWndTreeRS,hTemp,TVGN_PREVIOUS);
	}
	hCurrent=TreeView_GetParent(phWndTreeRS,hTreeParam);
	if(hCurrent==NULL) InverseTab(IndexTab,FALSE);
	else
	{	mHpPak_AddIndex(IndexTab,0);
		GetIndexFromTree(hCurrent,phWndTreeRS,IndexTab);
	}
}

//---------------------------------------------------------//
// GetmHpFileName
//---------------------------------------------------------//
char* GetmHpFileName(HWND hWndmHpLst,int iTopic,char* lpmHpPath,int iMaxSize)
{	if(HlpComStruct->bmHpFileOpened)
	{	/*LVITEM pItem;
		pItem.mask=LVIF_TEXT; 
		pItem.iItem=iTopic; 
		pItem.iSubItem=1; 
		pItem.cchTextMax=MAX_RS_SIZE; 
		pItem.pszText=lpmHpPath;
		if(ListView_GetItem(hWndmHpLst,&pItem))
            return (lpmHpPath);*/
        ListView_GetItemText(hWndmHpLst,iTopic,1,lpmHpPath,iMaxSize);
        return lpmHpPath;
	}
	return NULL;
}

//-----------------------------------------------------//
// AddItem
//-----------------------------------------------------//
HTREEITEM AddItem(HWND hWndTV,HTREEITEM hParent,int iImage,int iSelectedImage,LPSTR szText,LPARAM lParam)
{	TV_ITEM               tvItem;
	TV_INSERTSTRUCT       tvIns;
	tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	// Set the attribytes
	tvItem.pszText        = szText;
	tvItem.iImage         = iImage;
	tvItem.iSelectedImage = iSelectedImage;
	tvItem.lParam         = lParam;
	// Fill out the TV_INSERTSTRUCT
	tvIns.hParent         = hParent;
	tvIns.hInsertAfter    = TVI_LAST; 
	tvIns.item            = tvItem;
	return TreeView_InsertItem(hWndTV, &tvIns);
}

//-----------------------------------------------------//
// ListView_GetItemSel
//-----------------------------------------------------//
int ListView_GetItemSel(HWND hWndList)
{	for(int i=0;i<ListView_GetItemCount(hWndList);i++)
	{	if(ListView_GetItemState(hWndList,i,LVIS_SELECTED)==LVIS_SELECTED) return i;
	}
	return NO;
}

//---------------------------------------------------------//
// ReplacemHpLst
//---------------------------------------------------------//
void ReplacemHpLst(HWND hWndActive,HWND hWndLst)
{	int iPos=0;
	while(HlpComStruct->mHpFileList->IsmHpFileExist(iPos))
	{	mHpPak_ViewMsg(FALSE);
		if(mHpPak_IsFilePathExist(hWndActive,HlpComStruct->mHpFileList->GetmHpFilePath(iPos)))
		{	mHpPak_ViewMsg(TRUE);
			AddItemmHp(hWndActive,hWndLst,HlpComStruct->mHpFileList->GetmHpFilePath(iPos),iPos);
			iPos++;
		}
		else
		{	mHpPak_ViewMsg(TRUE);
			HlpComStruct->mHpFileList->RemovemHpFile(iPos);
		}
	}
	HlpComStruct->bmHpFileOpened=(BOOL)iPos;
}

//---------------------------------------------------------//
// AddItemmHp
//---------------------------------------------------------//
void AddItemmHp(HWND hWndActive,HWND hWndmHpLst,char* mHpFileName,int iRang)
{	char lpTopic[MAX_RS_SIZE]={0};
	char lpLvi[MAX_RS_SIZE]={0};
	LV_ITEM lvi;
	lvi.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvi.state=lvi.stateMask=0;
	lvi.iItem=iRang;
	lvi.iSubItem=0;
	lvi.cchTextMax=MAX_RS_SIZE;
	lvi.pszText=lpLvi;
	mHpPak_GetTopicmHpFile(hWndActive,mHpFileName,lpTopic,MAX_RS_SIZE);
	strncpy(lvi.pszText,lpTopic,MAX_RS_SIZE);
	lvi.iImage=7;
	ListView_InsertItem(hWndmHpLst,&lvi);
	lvi.iSubItem=1;
	strncpy(lvi.pszText,mHpFileName,MAX_RS_SIZE);
	ListView_SetItem(hWndmHpLst,&lvi);
}

//-----------------------------------------------------//
// SetmHpListFromCtrl
//-----------------------------------------------------//
void SetmHpListFromCtrl(HWND hWndList,BOOL bServOpen)
{	int j,iRemove=NO;
	for(j=0;j<ListView_GetItemCount(hWndList);j++)
	{	if(HlpComStruct->mHpFileList->GetNewFile(j)) iRemove=j;
	}
	HlpComStruct->mHpFileList->EmptymHpFileList();
	for(j=0;j<ListView_GetItemCount(hWndList);j++)
	{	char lpmHpFilePath[MAX_RS_SIZE]={0};
        if(j!=iRemove) HlpComStruct->mHpFileList->AddmHpFile(GetmHpFileName(hWndList,j,lpmHpFilePath,MAX_RS_SIZE),FALSE);
		else if(bServOpen) HlpComStruct->mHpFileList->AddmHpFile(NULL_STRING,TRUE);
	}
}

//-----------------------------------------------------//
// ViewMsgAlreadyOpened
//-----------------------------------------------------//
void ViewMsgAlreadyOpened(HWND hWndActive,char* lpPath,int iSizePath)
{	char lpRC[255]={0};
	char szTmp[MAX_RS_SIZE]={0};
	mHpPak_InitStr(szTmp,MAX_RS_SIZE);
	LoadString(HlpComStruct->hInstServ,IDS_FILE_ALREADYOPENED,lpRC,255);
	strncpy(szTmp,lpRC,mHpPak_FindStrLenght(lpRC,255));
	strncat(szTmp,lpPath,mHpPak_FindStrLenght(lpPath,iSizePath));
	MessageBox(hWndActive,szTmp,MHPPAK_DLGMSGTITLE,MB_ICONINFORMATION);
}

//-----------------------------------------------------//
// IsItAlreadyOpened
//-----------------------------------------------------//
int IsItAlreadyOpened(HWND hWndFileCtrl,char* lpPath,int PathSize)
{	char lpOpenedFile[MAX_RS_SIZE]={0};
	int i;
	LV_ITEM lvi;
	lvi.mask=LVIF_TEXT;
	lvi.state=lvi.stateMask=0;
	lvi.iSubItem=1;
	lvi.cchTextMax=MAX_RS_SIZE;
	lvi.pszText=lpOpenedFile;
	for(i=0;i<ListView_GetItemCount(hWndFileCtrl);i++)
	{	lvi.iItem=i;
		ListView_GetItem(hWndFileCtrl,&lvi);
		if(!strncmp(lpPath,lvi.pszText,PathSize)) return i;
	}
	return NO;
}

//---------------------------------------------------------//
// GetOpenmHpFiles
//---------------------------------------------------------//
int GetOpenmHpFiles(HWND hWndActive,HWND hWndFileCtrl)
{	char szFilter[MAX_KEY_SIZE]={0};
	char szFile[MAX_RS_SIZE]={0};
	int indice1=0,indice2=0,iCount=NO;
	BOOL bExit=FALSE;
	BOOL bMultiSel=FALSE;
    LoadString(HlpComStruct->hInstServ,IDS_OPEN_FILTER,szFilter,MAX_KEY_SIZE);
    memcpy(&szFilter[strlen(szFilter)+1],MHPPAK_MHF_EXTFILTER,sizeof(MHPPAK_MHF_EXTFILTER));
	//strncpy(&szFilter[strlen(szFilter)+1],"*.mhf",MAX_KEY_SIZE); // Stack corrupted!
    OPENFILENAME OpenFile;
    memset(&OpenFile,0,sizeof(OPENFILENAME));
	OpenFile.lStructSize=sizeof(OPENFILENAME);
	OpenFile.hwndOwner=hWndActive;
	OpenFile.lpstrFilter=szFilter;
	OpenFile.nFilterIndex=1;
	OpenFile.nMaxFile=MAX_RS_SIZE;
	OpenFile.lpstrTitle=NULL;
	OpenFile.lpstrFile=szFile;
	OpenFile.Flags=OFN_ALLOWMULTISELECT|OFN_ENABLESIZING|OFN_EXPLORER|OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	if(GetOpenFileName(&OpenFile))
	{	iCount=ListView_GetItemCount(hWndFileCtrl);
	    char szTemp[MAX_RS_SIZE]={0};
	    char szPath[MAX_RS_SIZE]={0};
		while(!bExit)
		{	if((szFile[indice1]==END_LIST)&&(szFile[(indice1+1)]==END_LIST))
			{	if(bMultiSel==TRUE)
				{	char szTmp[MAX_RS_SIZE]={0};
                    strncpy(szTmp,szPath,MAX_RS_SIZE);
					strncat(szTmp,MHPPAK_STR_ANTISHLASS,1);
					strncat(szTmp,szTemp,mHpPak_FindStrLenght(szTemp,MAX_RS_SIZE));
					if(IsItAlreadyOpened(hWndFileCtrl,szTmp,MAX_RS_SIZE)==NO)
					{	AddItemmHp(hWndActive,hWndFileCtrl,szTmp,ListView_GetItemCount(hWndFileCtrl));}
					else ViewMsgAlreadyOpened(hWndActive,szTmp,MAX_RS_SIZE);
				}
				else if(IsItAlreadyOpened(hWndFileCtrl,szTemp,MAX_RS_SIZE)==NO)
				{	AddItemmHp(hWndActive,hWndFileCtrl,szTemp,ListView_GetItemCount(hWndFileCtrl));}
				else ViewMsgAlreadyOpened(hWndActive,szTemp,MAX_RS_SIZE);
				bExit=TRUE;
			}
			else if(szFile[indice1]!=END_LIST)
			{	szTemp[indice2]=szFile[indice1];
				indice2++;
			}
			else
			{	if(!bMultiSel)
				{	bMultiSel=TRUE;
					strncpy(szPath,szTemp,MAX_RS_SIZE);
				}
				else
				{	char szTmp[MAX_RS_SIZE]={0};
                    strncpy(szTmp,szPath,MAX_RS_SIZE);
					strncat(szTmp,MHPPAK_STR_ANTISHLASS,1);
					strncat(szTmp,szTemp,mHpPak_FindStrLenght(szTemp,MAX_RS_SIZE));
					if(IsItAlreadyOpened(hWndFileCtrl,szTmp,MAX_RS_SIZE)==NO)
					{	AddItemmHp(hWndActive,hWndFileCtrl,szTmp,ListView_GetItemCount(hWndFileCtrl));}
					else ViewMsgAlreadyOpened(hWndActive,szTmp,MAX_RS_SIZE);
				}
				mHpPak_InitStr(szTemp,MAX_RS_SIZE);
				indice2=0;					
			}
			indice1++;
		}
        return iCount;
	}
	return iCount;
}

//-----------------------------------------------------//
// ReadFavRegKey
//-----------------------------------------------------//
void ReadFavRegKey(HKEY AppliKey,CFavListe* CReadFavLst)
{	// Favorites ////////////////////////////////////////
	// FavoritesLst
	char lpEvent[MAX_RS_SIZE]={0};
	char lpStrValue[MAX_RS_SIZE]={0};
	char lpmHpPath[MAX_KEY_SIZE]={0};
	char lpAssist[MAX_KEY_SIZE]={0};
	char lpFav[MAX_KEY_SIZE]={0};
	char lpParam[MAX_KEY_SIZE]={0};
	char lpEvol[MAX_KEY_SIZE]={0};
	BYTE szByteTab[MAX_KEY_SIZE]={0};
	int iTabRef[MAX_PART_TREE]={0};
	int iFav=0,iParam,iEvol,indice,iloop,iParamSel,iParamType;
	HKEY FavKey,FavRowKey,ParamKey,EvolKey;
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_SZ;
	CEvoluList* NewEvolu=NULL;
	CRepeatList* NewRepeat=NULL;
	CReadFavLst->EmptyFavList();
	if(RegOpenKeyEx(AppliKey,FAVORITES_KEY,0,KEY_READ,&FavKey)!=ERROR_SUCCESS) return;
	if(RegQueryValueEx(FavKey,AUTO_ASSIST_KEY,NULL,&dwType,szByteTab,&iSize)!=ERROR_SUCCESS) return;
	mHpPak_FillStringFromData(lpAssist,MAX_KEY_SIZE,szByteTab,(int)iSize);
	_snprintf(lpFav,MAX_KEY_SIZE,"%d",iFav);
	while(RegOpenKeyEx(FavKey,lpFav,0,KEY_READ,&FavRowKey)==ERROR_SUCCESS)
	{	NewEvolu=new CEvoluList();
		// AutoAssist
		if((!iFav)&&(strncmp(lpAssist,NULL_STRING,MAX_KEY_SIZE)!=0)) CReadFavLst->SetAutoAssist(lpAssist);
		// mHpPath
		dwType=REG_SZ;iSize=MAX_KEY_SIZE;
		if(RegQueryValueEx(FavRowKey,MHP_PATH_KEY,NULL,&dwType,szByteTab,&iSize)!=ERROR_SUCCESS)
		{	CReadFavLst->EmptyFavList();
			return;
		}
		mHpPak_FillStringFromData(lpmHpPath,MAX_KEY_SIZE,szByteTab,(int)iSize);
		// EventRef
		if(!mHpPak_ReadLongStrRegVal(FavRowKey,REF_EVENT_KEY,lpEvent,MAX_RS_SIZE))
		{	CReadFavLst->EmptyFavList();
			return;
		}
		indice=0;
		mHpPak_InitStr(lpStrValue,MAX_RS_SIZE);
		strncat(lpStrValue,SEPARATED_STRING,1);
		strncat(lpStrValue,lpEvent,mHpPak_FindStrLenght(lpEvent,MAX_RS_SIZE));
		mHpPak_EmptyTab(iTabRef,MAX_PART_TREE);
		while(mHpPak_GetTopicFromRef(lpStrValue,indice)!=NO)
		{	iTabRef[indice]=mHpPak_GetTopicFromRef(lpStrValue,indice);
			indice++;	
		}
		iTabRef[indice]=NO;
		// EventStr
		if(!mHpPak_ReadLongStrRegVal(FavRowKey,STR_EVENT_KEY,lpEvent,MAX_RS_SIZE))
		{	CReadFavLst->EmptyFavList();
			return;
		}
		// LoopOption
		dwType=REG_DWORD;iSize=MAX_KEY_SIZE;
		if(RegQueryValueEx(FavRowKey,LOOPOPTION_KEY,NULL,&dwType,szByteTab,&iSize)!=ERROR_SUCCESS)
		{	CReadFavLst->EmptyFavList();
			return;
		}
		mHpPak_FillIntFromData(szByteTab,iloop,FALSE);
		iParam=0;
		mHpPak_InitStr(lpParam,MAX_KEY_SIZE);
		_snprintf(lpParam,MAX_KEY_SIZE,"%d",iParam);
		while(RegOpenKeyEx(FavRowKey,lpParam,0,KEY_READ,&ParamKey)==ERROR_SUCCESS) // Param
		{	NewRepeat=new CRepeatList();
			// ParamIndex
			dwType=REG_DWORD;iSize=MAX_KEY_SIZE;
			if(RegQueryValueEx(ParamKey,PARAM_INDEX_KEY,NULL,&dwType,szByteTab,&iSize)!=ERROR_SUCCESS)
			{	CReadFavLst->EmptyFavList();
				return;
			}
			mHpPak_FillIntFromData(szByteTab,iParamSel,FALSE);
			// ParamType
			dwType=REG_DWORD;iSize=MAX_KEY_SIZE;
			if(RegQueryValueEx(ParamKey,PARAM_TYPE_KEY,NULL,&dwType,szByteTab,&iSize)!=ERROR_SUCCESS)
			{	CReadFavLst->EmptyFavList();
				return;
			}
			mHpPak_FillIntFromData(szByteTab,iParamType,FALSE);
			// ParamValues
			if(!mHpPak_ReadLongStrRegVal(ParamKey,PARAM_VALUES_KEY,lpStrValue,MAX_RS_SIZE)) // Evolu
			{	iEvol=0;
				mHpPak_InitStr(lpEvol,MAX_KEY_SIZE);
				_snprintf(lpEvol,MAX_KEY_SIZE,"%d",iEvol);
				while(RegOpenKeyEx(ParamKey,lpEvol,0,KEY_READ,&EvolKey)==ERROR_SUCCESS)
				{	// ParamValues
					if(!mHpPak_ReadLongStrRegVal(EvolKey,PARAM_VALUES_KEY,lpStrValue,MAX_RS_SIZE))
					{	mHpPak_InitStr(lpStrValue,MAX_RS_SIZE);}
					NewRepeat->AddTypeList(iParamType,lpStrValue,MAX_RS_SIZE);
					iEvol++;
					mHpPak_InitStr(lpEvol,MAX_KEY_SIZE);
					_snprintf(lpEvol,MAX_KEY_SIZE,"%d",iEvol);
				}
				NewRepeat->SetEvoluFlag(TRUE);
			}
			else NewRepeat->AddTypeList(iParamType,lpStrValue,MAX_RS_SIZE);
			NewEvolu->AddEvoluItem(iParamSel,iParamType,NewRepeat);
			iParam++;
			mHpPak_InitStr(lpParam,MAX_KEY_SIZE);
			_snprintf(lpParam,MAX_KEY_SIZE,"%d",iParam);
		}
		if(iParam!=0) CReadFavLst->AddFavItem(iTabRef,iloop,lpmHpPath,lpEvent,NewEvolu,TRUE);
		else
		{	CReadFavLst->AddFavItem(iTabRef,iloop,lpmHpPath,lpEvent,NULL,TRUE);
			delete(NewEvolu);
		}
		iFav++;
		mHpPak_InitStr(lpFav,MAX_KEY_SIZE);
		_snprintf(lpFav,MAX_KEY_SIZE,"%d",iFav);
	}
}

//-----------------------------------------------------//
// RemoveRegAppKey
//-----------------------------------------------------//
void RemoveRegAppKey(char* lpNameExe,HKEY mHpAppKey)
{	SECURITY_ATTRIBUTES lpSecAtt;
	HKEY lpAppKey; //,lpPathKey;
	DWORD OpDw=REG_OPENED_EXISTING_KEY;
	//BYTE BtTab[MAX_KEY_SIZE]={0};
	//char lpExePath[MAX_KEY_SIZE]={0};
	//int Size;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecAtt,bAclRes);
	// mHpApp
	if(RegCreateKeyEx(mHpAppKey,lpNameExe,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecAtt,
					&lpAppKey,&OpDw)!=ERROR_SUCCESS) return;
	mHpPak_DeleteSubKey(mHpAppKey,lpNameExe);
	// AppName
	/*if(RegCreateKeyEx(mHpAppKey,lpNameExe,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecAtt,
					&lpAppKey,&OpDw)!=ERROR_SUCCESS) return;
	if(RegCreateKeyEx(lpAppKey,lpNameExe,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecAtt,
					&lpPathKey,&OpDw)!=ERROR_SUCCESS) return;
	GetCurrentDirectory(MAX_KEY_SIZE,lpExePath);
	Size=mHpPak_FillDataFromString(BtTab,lpExePath);
	RegSetValueEx(lpPathKey,EXEPATH_KEY,0,REG_SZ,BtTab,Size);
	RegCloseKey(lpPathKey);*/
}

//-----------------------------------------------------//
// WriteFavRegKey
//-----------------------------------------------------//
BOOL WriteFavRegKey(char* lpNameExe,HKEY lpFavKey,CFavListe* CWriteFavLst)
{	DWORD opWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES lpSec;
	BYTE BTab[MAX_KEY_SIZE]={0};
	int iSize,iParam,pType,iSel,iRepeat;
	char lpTmp[MAX_RS_SIZE]={0};
	char lpFav[MAX_KEY_SIZE]={0};
	char lpParam[MAX_KEY_SIZE]={0};
	HKEY lpIndexKey,lpNewKey,lpParamKey;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSec,bAclRes);
	for(int i=0;i<CWriteFavLst->GetFavCount();i++)
	{	mHpPak_InitStr(lpFav,MAX_KEY_SIZE);
		_snprintf(lpFav,MAX_KEY_SIZE,"%d",i);
		if(RegCreateKeyEx(lpFavKey,lpFav,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
						&lpIndexKey,&opWord)!=ERROR_SUCCESS) return FALSE;
		// mHpPath
		if(RegCreateKeyEx(lpFavKey,lpFav,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
						&lpNewKey,&opWord)!=ERROR_SUCCESS) return FALSE;
		iSize=mHpPak_FillDataFromString(BTab,CWriteFavLst->GetmHpPath(i));
		RegSetValueEx(lpNewKey,MHP_PATH_KEY,0,REG_SZ,BTab,iSize);
		RegCloseKey(lpNewKey);
		// EventStr
		if(!mHpPak_WriteLongStrRegVal(lpFavKey,lpFav,STR_EVENT_KEY,CWriteFavLst->GetEventStr(i))) return FALSE;
		// EventRef
		if(!mHpPak_WriteLongStrRegVal(lpFavKey,lpFav,REF_EVENT_KEY,CWriteFavLst->GetEventRef(i))) return FALSE;
		if(RegCreateKeyEx(lpFavKey,lpFav,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
						&lpNewKey,&opWord)!=ERROR_SUCCESS) return FALSE;
		iSize=mHpPak_FillDataFromNumber(BTab,CWriteFavLst->GetLoopOption(i),FALSE);
		RegSetValueEx(lpNewKey,LOOPOPTION_KEY,0,REG_DWORD,BTab,iSize);
		RegCloseKey(lpNewKey);
		// Param
		if(CWriteFavLst->GetItemFromRang(i)->FavItem->ParamList!=NULL)
		{	iParam=0;
			while(iParam!=CWriteFavLst->GetItemFromRang(i)->FavItem->ParamList->GetSelCount())
			{	pType=CWriteFavLst->GetItemFromRang(i)->FavItem->ParamList->GetItemType(iParam);
				iSel=CWriteFavLst->GetItemFromRang(i)->FavItem->ParamList->GetSelItem(iParam);
				mHpPak_InitStr(lpParam,MAX_KEY_SIZE);
				_snprintf(lpParam,MAX_KEY_SIZE,"%d",iParam);
				if(RegCreateKeyEx(lpIndexKey,lpParam,0,NULL,REG_OPTION_NON_VOLATILE,
								KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
								&lpParamKey,&opWord)!=ERROR_SUCCESS) return FALSE;
				// ParamIndex
				if(RegCreateKeyEx(lpIndexKey,lpParam,0,NULL,REG_OPTION_NON_VOLATILE,
								KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
								&lpNewKey,&opWord)!=ERROR_SUCCESS) return FALSE;
				iSize=mHpPak_FillDataFromNumber(BTab,iSel,FALSE);
				RegSetValueEx(lpNewKey,PARAM_INDEX_KEY,0,REG_DWORD,BTab,iSize);
				RegCloseKey(lpNewKey);
				// ParamType
				if(RegCreateKeyEx(lpIndexKey,lpParam,0,NULL,REG_OPTION_NON_VOLATILE,
								KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
								&lpNewKey,&opWord)!=ERROR_SUCCESS) return FALSE;
				iSize=mHpPak_FillDataFromNumber(BTab,CWriteFavLst->GetItemFromRang(i)->FavItem->ParamList->GetItemType(iParam),FALSE);
				RegSetValueEx(lpNewKey,PARAM_TYPE_KEY,0,REG_DWORD,BTab,iSize);
				RegCloseKey(lpNewKey);
				// ParamValues
				if(!CWriteFavLst->GetItemFromRang(i)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetEvoluFlag())
				{	if(!mHpPak_WriteLongStrRegVal(lpIndexKey,lpParam,PARAM_VALUES_KEY,
									CWriteFavLst->GetItemFromRang(i)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetStringParam(0,pType)))
					{	return FALSE;}
				}
				else
				{	iRepeat=0;
					while(iRepeat!=CWriteFavLst->GetLoopOption(i))
					{	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
						_snprintf(lpTmp,MAX_RS_SIZE,"%d",iRepeat);
						if(RegCreateKeyEx(lpParamKey,lpTmp,0,NULL,REG_OPTION_NON_VOLATILE,
										KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
										&lpNewKey,&opWord)!=ERROR_SUCCESS) return FALSE;
						if(!mHpPak_WriteLongStrRegVal(lpParamKey,lpTmp,PARAM_VALUES_KEY,
									CWriteFavLst->GetItemFromRang(i)->FavItem->ParamList->GetCellFromItem(iSel)->EvoluItem->GetStringParam(iRepeat,pType)))
						{	return FALSE;}
						iRepeat++;
					}
				}
				iParam++;
				RegCloseKey(lpParamKey);
			}
		}
		RegCloseKey(lpIndexKey);
	}
	return TRUE;
}

//---------------------------------------------------------//
// WriteFavsRegKey
//---------------------------------------------------------//
void WriteFavsRegKey(char* lpNameExe,CFavListe* CWriteFavsLst)
{	SECURITY_ATTRIBUTES lpSecu;
	HKEY lpNewKey,lpAppKey,lpTmpKey,lpmHpKey;
	DWORD dWord=REG_OPENED_EXISTING_KEY;
	char lpIndex[MAX_KEY_SIZE]={0};
	char lpmHpPath[MAX_KEY_SIZE]={0};
	BYTE ByteTab[MAX_KEY_SIZE]={0};
	int iSize;
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
	// mHpApp
	if(RegCreateKeyEx(lpmHpKey,MHPAPP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpmHpKey,&dWord)!=ERROR_SUCCESS) return;
	// AppName
	if(RegCreateKeyEx(lpmHpKey,lpNameExe,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpAppKey,&dWord)!=ERROR_SUCCESS) return;
	if(RegCreateKeyEx(lpAppKey,FAVORITES_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpTmpKey,&dWord)!=ERROR_SUCCESS)
	{	RemoveRegAppKey(lpNameExe,lpmHpKey);
		return;
	}
	// Remove Favorites Key
	mHpPak_DeleteSubKey(lpAppKey,FAVORITES_KEY);
	if(CWriteFavsLst->GetFavCount()!=0)
	{	// Favorites
		if(RegCreateKeyEx(lpAppKey,FAVORITES_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpTmpKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpNameExe,lpmHpKey);
			return;
		}
		if(!WriteFavRegKey(lpNameExe,lpTmpKey,CWriteFavsLst))
		{	RemoveRegAppKey(lpNameExe,lpmHpKey);
			return;
		}
		// AutoAssist
		if(RegCreateKeyEx(lpAppKey,FAVORITES_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&lpNewKey,&dWord)!=ERROR_SUCCESS)
		{	RemoveRegAppKey(lpNameExe,lpmHpKey);
			return;
		}
		iSize=mHpPak_FillDataFromString(ByteTab,CWriteFavsLst->GetAutoAssist());
		RegSetValueEx(lpNewKey,AUTO_ASSIST_KEY,0,REG_SZ,ByteTab,iSize);
		RegCloseKey(lpNewKey);
	}
	RegCloseKey(lpTmpKey);
	RegCloseKey(lpAppKey);
}

//---------------------------------------------------------//
// RemoveExistFavRegKey
//---------------------------------------------------------//
void RemoveExistFavRegKey(char* lpRemoveNewFile,int* iChangeRef,int* iLagRef)
{	char lpAppRead[MAX_KEY_SIZE]={0};
	HKEY mHpAppKey,AppKey,TmpKey;
	DWORD iSize=MAX_KEY_SIZE;
	FILETIME lpWriteTime;
	CFavListe* CFavTmp=new CFavListe();
	int iApp=0;
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&mHpAppKey)!=ERROR_SUCCESS)
	{	delete(CFavTmp);
		return;
	}
	// mHelp	
	if(RegOpenKeyEx(mHpAppKey,MHELP_KEY,0,KEY_READ,&mHpAppKey)!=ERROR_SUCCESS)
	{	delete(CFavTmp);
		return;
	}
	// mHpApp
	if(RegOpenKeyEx(mHpAppKey,MHPAPP_KEY,0,KEY_READ,&mHpAppKey)!=ERROR_SUCCESS)
	{	delete(CFavTmp);
		return;
	}
	while(RegEnumKeyEx(mHpAppKey,iApp,lpAppRead,&iSize,NULL,NULL,NULL,&lpWriteTime)!=ERROR_NO_MORE_ITEMS)
	{	// App
		if(RegOpenKeyEx(mHpAppKey,lpAppRead,0,KEY_READ,&AppKey)!=ERROR_SUCCESS)
		{	CFavTmp->EmptyFavList();
			delete(CFavTmp);
			return;
		}
		// Favorites
		if(RegOpenKeyEx(AppKey,FAVORITES_KEY,0,KEY_READ,&TmpKey)==ERROR_SUCCESS)
		{	ReadFavRegKey(AppKey,CFavTmp);
			if(iChangeRef!=NULL)
			{	if(((iLagRef==NULL)&&(CFavTmp->ChangeRefFromFile(lpRemoveNewFile,iChangeRef)))||
				   ((iLagRef!=NULL)&&(CFavTmp->ChangeRefFromTask(lpRemoveNewFile,iChangeRef,iLagRef))))
				{	WriteFavsRegKey(lpAppRead,CFavTmp);}
			}
			else
			{	if(CFavTmp->RemoveFromFile(lpRemoveNewFile))
				{	WriteFavsRegKey(lpAppRead,CFavTmp);}
			}
		}
		RegCloseKey(TmpKey);
		RegCloseKey(AppKey);
		mHpPak_InitStr(lpAppRead,MAX_KEY_SIZE);
		iSize=MAX_KEY_SIZE;
		iApp++;
	}
	RegCloseKey(mHpAppKey);
	CFavTmp->EmptyFavList();
	delete(CFavTmp);
}

//---------------------------------------------------------//
// RemoveExistFavsFile
//---------------------------------------------------------//
BOOL RemoveExistFavsFile(HWND hWndActive,char* lpmHpFileName,int* ServRefTab)
{	CFavListe* FavFileList=new CFavListe();
	if(mHpPak_GetFavoritesFile(hWndActive,lpmHpFileName,FavFileList))
	{	FavFileList->ChangeRefFromFile(lpmHpFileName,ServRefTab);
		if(mHpCtrl_WriteFavsFile(hWndActive,lpmHpFileName,FavFileList,FavFileList->GetFavCount()))
		{	return TRUE;}
	}
	return FALSE;
}

//---------------------------------------------------------//
// SetmHpCursor
//---------------------------------------------------------//
HCURSOR SetmHpCursor(HWND hWndActive,int iCurType)
{	if(iCurType!=NO) imHpCursor=iCurType;
	if((iCurType!=NO)||(imHpCursor!=0))
	{	switch(imHpCursor)
		{	case 0: // ARROW
			{	HlpComStruct->hmHpCursor=LoadCursor(NULL,IDC_ARROW);
				break;
			}
			case 1: // WAIT
			{	HlpComStruct->hmHpCursor=LoadCursor(NULL,IDC_WAIT);
				break;
			}
			case 2: // SIZEWE
			{	HlpComStruct->hmHpCursor=LoadCursor(NULL,IDC_SIZEWE);
				break;
			}
			default:
			{	HlpComStruct->hmHpCursor=LoadCursor(NULL,IDC_ARROW);
				break;
			}
		}
		SetClassLongPtr(hWndActive,GCLP_HCURSOR,(LONG_PTR)HlpComStruct->hmHpCursor);
		return SetCursor(HlpComStruct->hmHpCursor);
	}
	return HlpComStruct->hmHpCursor;
}

//-----------------------------------------------------//
// SplitHtmlLinkRS
//-----------------------------------------------------//
void SplitHtmlLinkRS(char* lpStrRS,UINT iMaxStr,const char* lpmHpFile,char* lpHtmlRS,UINT iMaxHtml)
{   // eg: "Interface principale¤usr_MainInterface.html", ""
    //  -> "Interface principale", "mHelp file path\\usr_MainInterface.html"
    *lpHtmlRS=0;
    UINT iHtmlIdx=0;
    bool bLinkFound=false;
    for(UINT i=0;i<iMaxStr;i++)
    {   if(!bLinkFound)
        {   if(lpStrRS[i]==TREE_PARAM_GROUP)
            {   lpStrRS[i]=0;
                bLinkFound=true;
            }
        }
        else
        {   if(iHtmlIdx<iMaxHtml)
                lpHtmlRS[iHtmlIdx++]=lpStrRS[i];
            else
            {   lpHtmlRS[iHtmlIdx]=0;
                break;
            }
        }
    }
    // Get mHelp file path
    char lpCurPath[MAX_RS_SIZE]={0};
    strncpy(lpCurPath,lpmHpFile,MAX_RS_SIZE);
    UINT iPathPos=0;
    for(UINT i=0; (lpCurPath[i]!=0); i++)
    {   if(lpCurPath[i]=='\\')
            iPathPos=i;
    }
    lpCurPath[iPathPos]=0;
    // Add HTML file name
    strncat(lpCurPath,MHPPAK_STR_ANTISHLASS,sizeof(MHPPAK_STR_ANTISHLASS));
    strncat(lpCurPath,lpHtmlRS,strlen(lpHtmlRS));
    strncpy(lpHtmlRS,lpCurPath,iMaxHtml);
}

//-----------------------------------------------------//
// SelectHtmlURL
//-----------------------------------------------------//
BOOL SelectHtmlURL(char* lpHtmlFile,UINT iMaxSize,HWND hWndTree,HWND hWndHideTree,
                   HTREEITEM hSelItem,HTREEITEM hHideItem)
{   if(!hSelItem)
    {   hSelItem=TreeView_GetRoot(hWndTree);
        if(TreeView_GetSelection(hWndTree)==hSelItem)
            return FALSE; // Do not change URL
        hHideItem=TreeView_GetRoot(hWndHideTree);
    }
    if(TreeView_GetSelection(hWndTree)==hSelItem)
    {   TVITEM TvItem;
        TvItem.mask=TVIF_TEXT;
        TvItem.cchTextMax=MAX_RS_SIZE;
        TvItem.pszText=lpHtmlFile;
        TvItem.hItem=hHideItem;
        TreeView_GetItem(hWndHideTree,&TvItem);
        return TRUE; // URL has changed
    }
    while(TreeView_GetNextSibling(hWndTree,hSelItem)!=NULL)
    {   hSelItem=TreeView_GetNextSibling(hWndTree,hSelItem);
        hHideItem=TreeView_GetNextSibling(hWndHideTree,hHideItem);
        if(TreeView_GetSelection(hWndTree)==hSelItem)
        {   TVITEM TvItem;
            TvItem.mask=TVIF_TEXT;
            TvItem.cchTextMax=MAX_RS_SIZE;
            TvItem.pszText=lpHtmlFile;
            TvItem.hItem=hHideItem;
            TreeView_GetItem(hWndHideTree,&TvItem);
            return TRUE; // URL has changed
        }
    }
    if(TreeView_GetChild(hWndTree,hSelItem)!=NULL)
    {   if(SelectHtmlURL(lpHtmlFile,iMaxSize,hWndTree,hWndHideTree,
                         TreeView_GetChild(hWndTree,hSelItem),TreeView_GetChild(hWndHideTree,hHideItem)))
            return TRUE;
    }
    while(TreeView_GetPrevSibling(hWndTree,hSelItem)!=NULL)
    {   hSelItem=TreeView_GetPrevSibling(hWndTree,hSelItem);
        hHideItem=TreeView_GetPrevSibling(hWndHideTree,hHideItem);
        if(TreeView_GetChild(hWndTree,hSelItem)!=NULL)
        {   if(SelectHtmlURL(lpHtmlFile,iMaxSize,hWndTree,hWndHideTree,
                             TreeView_GetChild(hWndTree,hSelItem),TreeView_GetChild(hWndHideTree,hHideItem)))
                return TRUE; // URL has changed
        }
    }
    return FALSE;
}

//---------------------------------------------------------//
// IsSystemNT
//---------------------------------------------------------//
/*BOOL IsSystemNT()
{	BOOL bSystemNT;
	DWORD dwVersion=GetVersion();
	if(dwVersion<0x80000000) bSystemNT=TRUE;
	else bSystemNT=FALSE;
	return bSystemNT;
}*/

//---------------------------------------------------------//
// ProceedMsgQueue
//---------------------------------------------------------//
void ProceedMsgQueue(HWND hWndActive)//,int iNone)
{	MSG MsgQueue;
	while(PeekMessage(&MsgQueue,hWndActive,0,0,PM_REMOVE))
	{	/*switch(iNone)
		{	case 0: // Openning mHelp File(s): Main window //////////////////////////////////////////////////////////////
			{	switch(MsgQueue.message)
				{	case WM_COMMAND:
					{	switch(LOWORD(MsgQueue.wParam))
						{	case IDC_ADD_MHPFILES:		// Open button
							case IDC_REMOVE_MHPFILES:	// Remove button
							case IDC_OPEN_FAVORITES:	// Open fav. macro file button
							case IDC_BUTTON_HELP:		// Help button
							case IDC_CREATE_ADD:		// Create macro button
							case IDC_DESTROY:			// Destroy macro button
							case IDC_START:				// Launch macro button
							case IDCANCEL:				// Close button
							{	break;}
							default:
							{	TranslateMessage(&MsgQueue);
								DispatchMessage(&MsgQueue);
								break;
							}
						}
						break;
					}
					case WM_NOTIFY:
					{	switch(((LPNMHDR)MsgQueue.lParam)->idFrom)
						{	case IDC_TAB_MHELP:		// mHelp TabCtrl
							{	break;}
							default:
							{	TranslateMessage(&MsgQueue);
								DispatchMessage(&MsgQueue);
								break;
							}
						}
						break;
					}
					default:
					{	TranslateMessage(&MsgQueue);
						DispatchMessage(&MsgQueue);
						break;
					}
				}
				break;
			}
			case 1: // Openning mHelp File(s): Server window
			{	break;
			}
		}*/
		TranslateMessage(&MsgQueue);
		DispatchMessage(&MsgQueue);
	}
}

//********************************************************************************************************************GET//
HELPCOMSTRUCT* GetHelpComStruct()
{	static BOOL bInit=FALSE;
	if(!bInit)
	{	HlpComStruct=(HELPCOMSTRUCT*)malloc(sizeof(HELPCOMSTRUCT));
		HlpComStruct->hInstServ=NULL;
		HlpComStruct->mHpFileList=NULL;
		HlpComStruct->bmHpFileOpened=FALSE;
		HlpComStruct->iMode=0;
		HlpComStruct->MouseSpeed=14;
		HlpComStruct->iNbBoucle=1;
		HlpComStruct->mHpDlgCreatedObj=FALSE;
		HlpComStruct->ServDlgCreatedObj=FALSE;
		HlpComStruct->ModifList=NULL;
		HlpComStruct->listBtn=NULL;
		HlpComStruct->AddList=NULL;
		HlpComStruct->iHourDelay=0;
		HlpComStruct->iMinuteDelay=30;
		mHpPak_EmptyTab(HlpComStruct->IndexTab,MAX_PART_TREE);
		HlpComStruct->hmHpCursor=LoadCursor(NULL,IDC_ARROW);
		bInit=TRUE;
	}
	return HlpComStruct;
}

//*******************************************************************************************************************FREE//
void FreeHelpComStruct()
{	free(HlpComStruct);}

// mHpVar.cpp
//

#include "stdafx.h"
#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif

//////
#include <ComCtrl.h>

#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

//---------------------------------------------//
// Définitions
#define		MHPSTYLE_KEY		"Style"
#define		DESCRIPTION_KEY		"Description"
#define		LIMITE_KEY			"Limite"
#define		MHPOBJS_KEY			"mHpObjs"
#define		CUROBJ_KEY			"CurObj"
#define		MHPPARAM_KEY		"mHpParam"
#define		MHPID_KEY			"mHpID"
#define		MHPCURRENT_KEY		"CurWnd"
#define		MHPCOUNT_KEY		"CntWnd"
#define		XCURPOS_KEY			"xCurPos"
#define		YCURPOS_KEY			"yCurPos"
#define		CTRLID_KEY			"CtrlID"

//---------------------------------------------------------------------------//
// Class globlales
class COptionCell
{	public:
		BOOL bRepeatOption;
		char lpNameApp[MAX_RS_SIZE];
		COptionCell* NextOption;
		COptionCell(BOOL,char*,int);
};
class COptionList
{	private:
		COptionCell* HeadOption;
	public:
		COptionList();
		void AddOption(BOOL,char*,int);
		BOOL IsRepeatOption();
		void EmptyOptions();
};

//---------------------------------------------------------------------------//
// Variables globlales
MHPVARSTRUCT* mHpStructVar;

//---------------------------------------------------------------------------//
// Déclarations
BOOL WriteCurObjkey(int);
void WritemHpSharedKey(HKEY&,HKEY&);
void ReadmHpTaskKey(int,HKEY&,HKEY&);
void ReadmHpParamKey(int,HKEY&,HKEY&);

// class CLinkCell ////////////////////////////////////////////////////////////
CLinkCell::CLinkCell(int iLastIndex)
{	bLink=FALSE;
	iIndex=iLastIndex;
	NextCell=NULL;
}

// class CLinkList ////////////////////////////////////////////////////////////
CLinkList::CLinkList()
{	LinkHead=NULL;}

CLinkCell* CLinkList::GetLinkCell(int iPos)
{	CLinkCell* Tmp=LinkHead;
	int indice=0;
	while((Tmp!=NULL)&&(indice!=iPos))
	{	Tmp=Tmp->NextCell;
		indice++;
	}
	return Tmp;
}

void CLinkList::AddItm(int iRealIndex)
{	CLinkCell* Tmp=LinkHead;
	if(LinkHead==NULL) LinkHead=new CLinkCell(iRealIndex);
	else
	{	while(Tmp->NextCell!=NULL) Tmp=Tmp->NextCell;
		Tmp->NextCell=new CLinkCell(iRealIndex);
	}
}

int CLinkList::GetItmIndex(int iPos)
{	CLinkCell* Tmp=GetLinkCell(iPos);
	if(Tmp!=NULL) return (Tmp->iIndex);
	return NO;
}

void CLinkList::LinkItm(int iPos)
{	CLinkCell* Tmp=GetLinkCell(iPos);
	if(Tmp!=NULL) Tmp->bLink=TRUE;
}

void CLinkList::UnLinkItms()
{	CLinkCell* Tmp=LinkHead;
	while(Tmp!=NULL)
	{	Tmp->bLink=FALSE;
		Tmp=Tmp->NextCell;
	}
}

BOOL CLinkList::GetItmLink(int iPos)
{	CLinkCell* Tmp=GetLinkCell(iPos);
	if(Tmp!=NULL) return (Tmp->bLink);
	return FALSE;
}

void CLinkList::EmptyLinkList()
{	CLinkCell *Temp;
	while(LinkHead!=NULL)
	{	Temp=LinkHead->NextCell;
		delete LinkHead;
		LinkHead=Temp;		
	}
}

// ItemsCellule ///////////////////////////////////////////////////////////////
ItemsCellule::ItemsCellule(int iIndex)
{	ItemIndex=iIndex;
	ItemSuivant=NULL;
}

// CGroupListe ////////////////////////////////////////////////////////////////
CGroupListe::CGroupListe()
{	ItemHead=NULL;
	ItemCurrent=NULL;
}

void CGroupListe::AddItemIndex(int itemindex)
{	ItemsCellule *NewCell=new ItemsCellule(itemindex);
	if(ItemHead==NULL)
	{	ItemHead=NewCell;}
	else
	{	ItemsCellule *Temp=ItemHead;
		while(Temp->ItemSuivant!=NULL)
		{	Temp=Temp->ItemSuivant;}
		Temp->ItemSuivant=NewCell;
	}
}

BOOL CGroupListe::IsListeEmpty()
{	if(ItemCurrent==NULL) return TRUE;
	return FALSE;
}

int CGroupListe::GetItemIndex()
{	if(ItemCurrent!=NULL) return ItemCurrent->ItemIndex;
	return NO;
}

void CGroupListe::EmptyGroupListe()
{	ItemsCellule *Temp;
	while(ItemHead!=NULL)
	{	Temp=ItemHead->ItemSuivant;
		delete ItemHead;
		ItemHead=Temp;		
	}
}

void CGroupListe::RemoveIndexItem()
{	ItemsCellule *Temp=ItemHead;
	if(ItemCurrent==ItemHead)
	{	ItemCurrent=ItemCurrent->ItemSuivant;
		delete (ItemHead);
		ItemHead=ItemCurrent;
		return;
	}
	while(Temp->ItemSuivant!=ItemCurrent)
	{	Temp=Temp->ItemSuivant;}
	ItemCurrent=ItemCurrent->ItemSuivant;
	delete (Temp->ItemSuivant);
	Temp->ItemSuivant=ItemCurrent;
	ItemCurrent=ItemHead;
}

void CGroupListe::RemoveLastIndexItem()
{	ItemsCellule *Temp=ItemHead;
	ItemsCellule *LTemp=ItemHead;
	while((Temp!=NULL)&&(Temp->ItemSuivant!=NULL))
	{	LTemp=Temp;
		Temp=Temp->ItemSuivant;
	}
	if(Temp!=NULL)
	{	if(Temp==ItemHead) 
		{	delete (ItemHead);
			ItemHead=NULL;
		}
		else
		{	delete (Temp);
			LTemp->ItemSuivant=NULL;
		}
	}
}

BOOL CGroupListe::InsertIndexItem(int index)
{	ItemsCellule *Temp,*NewCell;
	if(ItemCurrent==NULL)
	{	ItemHead=new ItemsCellule(index);}
	else
	{	if(index>(ItemCurrent->ItemIndex))
		{	while((ItemCurrent->ItemSuivant!=NULL)&&
				  (index>(ItemCurrent->ItemSuivant->ItemIndex)))
			{	ItemCurrent=ItemCurrent->ItemSuivant;}
			if((ItemCurrent->ItemSuivant!=NULL)&&
			   (index==(ItemCurrent->ItemSuivant->ItemIndex)))
			{	ItemCurrent=ItemCurrent->ItemSuivant;
				return FALSE;
			}
			NewCell=new ItemsCellule(index);
			Temp=ItemCurrent->ItemSuivant;
			ItemCurrent->ItemSuivant=NewCell;
			NewCell->ItemSuivant=Temp;
		}
		else
		{	while((ItemCurrent!=ItemHead)&&
					(index<ItemCurrent->ItemIndex))
			{	Temp=ItemHead;
				while(Temp->ItemSuivant!=ItemCurrent)
					Temp=Temp->ItemSuivant;
				ItemCurrent=Temp;
			}
			if(index==(ItemCurrent->ItemIndex)) return FALSE;
			NewCell=new ItemsCellule(index);
			if(ItemCurrent==ItemHead)
			{	ItemHead=NewCell;
				NewCell->ItemSuivant=ItemCurrent;
				ItemCurrent=ItemHead;
				return TRUE;
			}
			Temp=ItemCurrent->ItemSuivant;
			ItemCurrent->ItemSuivant=NewCell;
			NewCell->ItemSuivant=Temp;
		}
	}
	ItemCurrent=ItemHead;
	return TRUE;
}

int CGroupListe::ItemIndexSuivant()
{	if(ItemCurrent==NULL) return NO;
	int temp=ItemCurrent->ItemIndex;
	ItemCurrent=ItemCurrent->ItemSuivant;
	return temp;
}

void CGroupListe::ReplaceCurrent()
{	ItemCurrent=ItemHead;}

BOOL CGroupListe::IsNextIndexExist()
{	if((ItemCurrent!=NULL)&&(ItemCurrent->ItemSuivant!=NULL)) return TRUE;
	return FALSE;
}

// class CmHpTask /////////////////////////////////////////////////////////////
CmHpTask::CmHpTask(int TaskID,char* TaskMsg,char *StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrlID,char* lpCtrlName)
{	int indice=0;
	imHpID=TaskID;
	iCurrentWnd=CurWnd;
	iCountWnd=CntWnd;
	if(TaskMsg!=NULL) strncpy(lpmHpMsg,TaskMsg,MAX_RS_SIZE);
	else mHpPak_InitStr(lpmHpMsg,MAX_RS_SIZE);
	if(StateMsg!=NULL) strncpy(lpmHpStateMsg,StateMsg,MAX_RS_SIZE);
	else mHpPak_InitStr(lpmHpStateMsg,MAX_RS_SIZE);
	imHpStep=1;
	imHpOption=Option;
    if(imHpID!=MHPID_CTRL_MAN)
    {   if(lpCtrlName!=NULL)
        {   strncpy(lpmHpCtrlName,lpCtrlName,MAX_CTRL_NAME);
            mHpPak_EmptyTab(lpmHpCtrlID,MAX_PART_TREE);
        }
	    else
        {   mHpPak_InitStr(lpmHpCtrlName,MAX_CTRL_NAME);
            if(lpCtrlID!=NULL) mHpPak_CopyTab(lpmHpCtrlID,MAX_PART_TREE,lpCtrlID);
	        else mHpPak_EmptyTab(lpmHpCtrlID,MAX_PART_TREE);
        }
    }
}
CmHpTask::CmHpTask()
{	imHpStep=1;}
CmHpTask::~CmHpTask()
{}

//////
int CmHpTask::GetmHpID()
{	return imHpID;}

char* CmHpTask::GetmHpMsg()
{	return lpmHpMsg;}

void CmHpTask::SetmHpMsg(char *lpNewMess)
{	strncpy(lpmHpMsg,lpNewMess,MAX_RS_SIZE);}

char* CmHpTask::GetmHpStateMsg()
{	return lpmHpStateMsg;}

void CmHpTask::SetmHpStateMsg(char *lpNewMess)
{	strncpy(lpmHpStateMsg,lpNewMess,MAX_RS_SIZE);}

int CmHpTask::GetmHpOption()
{	return imHpOption;}

void CmHpTask::SetmHpOption(int iOption)
{	imHpOption=iOption;}

void CmHpTask::NextmHpStep()
{	imHpStep++;}

int CmHpTask::GetCurrentWnd()
{	return iCurrentWnd;}

void CmHpTask::SetCurrentWnd(int iNewCur)
{	iCurrentWnd=iNewCur;}

int CmHpTask::GetCountWnd()
{	return iCountWnd;}

void CmHpTask::SetCountWnd(int iNewCnt)
{	iCountWnd=iNewCnt;}

int CmHpTask::GetmHpStep()
{	return imHpStep;}

void CmHpTask::SetmHpStep(int Step)
{	imHpStep=Step;}

char* CmHpTask::GetmHpCtrlName()
{	return lpmHpCtrlName;}

int* CmHpTask::GetmHpCtrlID()
{	return lpmHpCtrlID;}

int CmHpTask::GetCtrlID()
{	int index=0;
	if(lpmHpCtrlID[index]!=NO)
	{	while(lpmHpCtrlID[(index+1)]!=NO) index++;
	}
	return lpmHpCtrlID[index];
}

BOOL CmHpTask::ReadCommonKey(int iPos,BOOL bCommon,HKEY &SubmHpKeyTask,HKEY &mHpKeyTask)
{	char lpCtrl[MAX_KEY_SIZE]={0};
	BYTE ByteTab[MAX_KEY_SIZE]={0};
	HKEY CtrlIDKey,mHpCtrlKey;
	DWORD isize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	int iIndex=0;
	if(SubmHpKeyTask!=NULL) RegCloseKey(SubmHpKeyTask);
	if(mHpKeyTask!=NULL) RegCloseKey(mHpKeyTask);
	SubmHpKeyTask=NULL;
	mHpKeyTask=NULL;
	ReadmHpTaskKey(iPos,SubmHpKeyTask,mHpKeyTask);
	if(mHpKeyTask!=NULL)
	{	// imHpID
		imHpID=GetmHpIDKey(iPos);
		if(imHpID==NO) return FALSE;
		if(bCommon)
		{	// iCurrentWnd
			if(RegQueryValueEx(mHpKeyTask,MHPCURRENT_KEY,NULL,&dwType,ByteTab,&isize)==ERROR_SUCCESS)
			{	mHpPak_FillIntFromData(ByteTab,iCurrentWnd,FALSE);
				isize=MAX_KEY_SIZE;dwType=REG_DWORD;
			}
			else iCurrentWnd=NO;
			// iCountWnd
			if(RegQueryValueEx(mHpKeyTask,MHPCOUNT_KEY,NULL,&dwType,ByteTab,&isize)!=ERROR_SUCCESS) return FALSE;
			mHpPak_FillIntFromData(ByteTab,iCountWnd,FALSE);
			isize=MAX_KEY_SIZE;dwType=REG_DWORD;
			// imHpOption
			if(RegQueryValueEx(mHpKeyTask,MHPOPTION_KEY,NULL,&dwType,ByteTab,&isize)!=ERROR_SUCCESS) return FALSE;
			mHpPak_FillIntFromData(ByteTab,imHpOption,FALSE);
			isize=MAX_KEY_SIZE;dwType=REG_DWORD;
			// lpmHpMsg
			if(!mHpPak_ReadLongStrRegVal(mHpKeyTask,MHPMSG_KEY,lpmHpMsg,MAX_RS_SIZE))
			{	mHpPak_InitStr(lpmHpMsg,MAX_RS_SIZE);}
			// lpmHpStateMsg
			if(!mHpPak_ReadLongStrRegVal(mHpKeyTask,MHPSTATEMSG_KEY,lpmHpStateMsg,MAX_RS_SIZE))
			{	mHpPak_InitStr(lpmHpStateMsg,MAX_RS_SIZE);}
            if(imHpID!=MHPID_CTRL_MAN)
            {   // lpmHpCtrlName
			    if(!mHpPak_ReadLongStrRegVal(mHpKeyTask,MHPNAME_KEY,lpmHpCtrlName,MAX_CTRL_NAME))
			    {	mHpPak_InitStr(lpmHpCtrlName,MAX_CTRL_NAME);
                    // lpmHpCtrlID
			        mHpPak_EmptyTab(lpmHpCtrlID,MAX_PART_TREE);
			        if(RegOpenKey(mHpKeyTask,MHPCTRLID_KEY,&mHpCtrlKey)==ERROR_SUCCESS)
			        {	_snprintf(lpCtrl,MAX_KEY_SIZE,"%d",iIndex);
				        while(RegOpenKey(mHpCtrlKey,lpCtrl,&CtrlIDKey)==ERROR_SUCCESS)
				        {	if(RegQueryValueEx(CtrlIDKey,CTRLID_KEY,NULL,&dwType,ByteTab,&isize)==ERROR_SUCCESS)
					        {	mHpPak_FillIntFromData(ByteTab,lpmHpCtrlID[iIndex],FALSE);}
					        else lpmHpCtrlID[iIndex]=NO;
					        isize=MAX_KEY_SIZE;dwType=REG_DWORD;
					        iIndex++;
					        mHpPak_InitStr(lpCtrl,MAX_KEY_SIZE);
					        _snprintf(lpCtrl,MAX_KEY_SIZE,"%d",iIndex);
					        RegCloseKey(CtrlIDKey);
				        }
				        lpmHpCtrlID[iIndex]=NO;
			        }
			        RegCloseKey(mHpCtrlKey);
                }
                else mHpPak_EmptyTab(lpmHpCtrlID,MAX_PART_TREE);
            }
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CmHpTask::WriteCommonKey(BOOL bCommon,HKEY &hSubmHpTaskKey,HKEY &hmHpTaskKey)
{	char lpPos[MAX_KEY_SIZE]={0};
	char lpCtrl[MAX_KEY_SIZE]={0};
	DWORD opWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES lpSec;
	BYTE BTab[MAX_KEY_SIZE]={0};
	HKEY hmHpCtrlKey,hCtrlKey,hSubPosKey=NULL,hTmpKey;
	int iIndex=0,iSize,iNextKey=GetNextKeyPos();
	BOOL bAclRes;
	if(hSubmHpTaskKey!=NULL) RegCloseKey(hSubmHpTaskKey);
	hSubmHpTaskKey=NULL;
	mHpPak_GetSecurityAtt(lpSec,bAclRes);
	GetmHpKeyFromPos(iNextKey,hSubPosKey,hSubmHpTaskKey,TRUE);
	if(hSubmHpTaskKey!=NULL)
	{	_snprintf(lpPos,MAX_KEY_SIZE,"%d",iNextKey);
		// mHpID
		if(RegCreateKeyEx(hSubPosKey,lpPos,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
						&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
		iSize=mHpPak_FillDataFromNumber(BTab,imHpID,FALSE);
		RegSetValueEx(hTmpKey,MHPID_KEY,0,REG_DWORD,BTab,iSize);
		RegCloseKey(hTmpKey);
		// mHpTask
		if(RegCreateKeyEx(hSubmHpTaskKey,MHPTASK_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
						&hmHpTaskKey,&opWord)!=ERROR_SUCCESS) return FALSE;
		if(bCommon)
		{	// iCurrentWnd
			if(iCurrentWnd!=NO)
			{	if(RegCreateKeyEx(hSubmHpTaskKey,MHPTASK_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
								KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
								&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
				iSize=mHpPak_FillDataFromNumber(BTab,iCurrentWnd,FALSE);
				RegSetValueEx(hTmpKey,MHPCURRENT_KEY,0,REG_DWORD,BTab,iSize);
				RegCloseKey(hTmpKey);
			}
			// iCountWnd
			if(RegCreateKeyEx(hSubmHpTaskKey,MHPTASK_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
							&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
			iSize=mHpPak_FillDataFromNumber(BTab,iCountWnd,FALSE);
			RegSetValueEx(hTmpKey,MHPCOUNT_KEY,0,REG_DWORD,BTab,iSize);
			RegCloseKey(hTmpKey);
			// imHpOption
			if(RegCreateKeyEx(hSubmHpTaskKey,MHPTASK_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
							&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
			iSize=mHpPak_FillDataFromNumber(BTab,imHpOption,FALSE);
			RegSetValueEx(hTmpKey,MHPOPTION_KEY,0,REG_DWORD,BTab,iSize);
			RegCloseKey(hTmpKey);
			// lpmHpMsg
			if((strncmp(lpmHpMsg,NULL_STRING,MAX_RS_SIZE)!=0)&&(!mHpPak_WriteLongStrRegVal(hSubmHpTaskKey,MHPTASK_KEY,MHPMSG_KEY,lpmHpMsg)))
			{	return FALSE;}
			// lpmHpStateMsg
			if((strncmp(lpmHpStateMsg,NULL_STRING,MAX_RS_SIZE)!=0)&&(!mHpPak_WriteLongStrRegVal(hSubmHpTaskKey,MHPTASK_KEY,MHPSTATEMSG_KEY,lpmHpStateMsg)))
			{	return FALSE;}
            if(imHpID!=MHPID_CTRL_MAN)
            {   // lpmHpCtrlName
                if(*lpmHpCtrlName)
                {   if(!mHpPak_WriteLongStrRegVal(hSubmHpTaskKey,MHPTASK_KEY,MHPNAME_KEY,lpmHpCtrlName))
			        {	return FALSE;}
                }
                else
                {   // lpmHpCtrlID
			        if(RegCreateKeyEx(hmHpTaskKey,MHPCTRLID_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							        KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
							        &hmHpCtrlKey,&opWord)!=ERROR_SUCCESS) return FALSE;
			        _snprintf(lpCtrl,MAX_KEY_SIZE,"%d",iIndex);
			        while(lpmHpCtrlID[iIndex]!=NO)
			        {	if(RegCreateKeyEx(hmHpCtrlKey,lpCtrl,0,NULL,REG_OPTION_NON_VOLATILE,
								        KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
								        &hCtrlKey,&opWord)!=ERROR_SUCCESS) return FALSE;
				        if(RegCreateKeyEx(hmHpCtrlKey,lpCtrl,0,NULL,REG_OPTION_NON_VOLATILE,
								        KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
								        &hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
				        iSize=mHpPak_FillDataFromNumber(BTab,lpmHpCtrlID[iIndex],FALSE);
				        RegSetValueEx(hTmpKey,CTRLID_KEY,0,REG_DWORD,BTab,iSize);
				        RegCloseKey(hCtrlKey);
				        RegCloseKey(hTmpKey);
				        iIndex++;
				        mHpPak_InitStr(lpCtrl,MAX_KEY_SIZE);
				        _snprintf(lpCtrl,MAX_KEY_SIZE,"%d",iIndex);
			        }
			        RegCloseKey(hmHpCtrlKey);
                }
            }
		}
		return TRUE;
	}
	return FALSE;
}

// mHpCtrl's virtuals -----------------------------------------------------**//
BOOL CmHpTask::ReadTaskKey(int i){ return NULL;}
BOOL CmHpTask::WriteTaskKey(){ return NULL;}
// Menu control -------------------------------//
int CmHpTask::GetMenuState(){ return NULL;}
// ListBox control ----------------------------//
void CmHpTask::AddMultiIndex(char* s){ NULL;}
BOOL CmHpTask::GetNextIndex(){ return NULL;}
char* CmHpTask::GetMultiIndex(int){ return NULL;}
void CmHpTask::InitMultiIndex(){ NULL;}
void CmHpTask::InverseMultiIndex(){ NULL;}
// EditBox control ----------------------------//
char* CmHpTask::GetEditValue(){	return NULL;}
void CmHpTask::SetEditValue(char* s){ NULL;}
int CmHpTask::GetUserFixed(){ return NULL;}
void CmHpTask::SetUserFixed(int i){ NULL;}
// Slider control -----------------------------//
int CmHpTask::GetSliderPos(){	return NULL;}
void CmHpTask::SetSliderPos(int i){ NULL;}
// Button control -----------------------------//
int CmHpTask::GetBtnState(){ return NULL;}
void CmHpTask::SetBtnState(int i){ NULL;}
// ChangeExe ----------------------------------//
char* CmHpTask::GetExeName(){ return NULL;}
BOOL CmHpTask::GetNewExe(){ return NULL;}
HWND CmHpTask::GetHwndExe(){ return NULL;}
void CmHpTask::SetHwndExe(HWND h){ NULL;}
char* CmHpTask::GetCommandLine(){ return NULL;}
void CmHpTask::SetCommandLine(char* s){ NULL;}
// TabCtrl ------------------------------------//
char* CmHpTask::GetTabSel(){ return NULL;}
void CmHpTask::SetTabSel(char* s){ NULL;}
// ToolBar ------------------------------------//
UINT CmHpTask::GetCommandID(){ return NULL;}
UINT CmHpTask::GetMenuSelect(){ return NULL;}
void CmHpTask::SetMenuSelect(UINT ui){ NULL;}
// CtrlMan ------------------------------------//
BOOL CmHpTask::WriteCtrlIDKey(HKEY) const { return FALSE;}
BOOL CmHpTask::ReadCtrlIDKey(HKEY) { return FALSE;}
HICON CmHpTask::GetCtrlIcon() const { return NULL;}
void CmHpTask::GetCtrlVers(char*,UINT) const { NULL;}
void CmHpTask::GetCtrlDesc(char*,UINT) const { NULL;}
BOOL CmHpTask::GetStrState(char* s,int i,BOOL b) const { return FALSE;}
int CmHpTask::GetParamType() const { return NO;}
ImHpCtrlMgmt* CmHpTask::GetCtrlMgmt() { return NULL;}
UINT CmHpTask::GetCtrlPlatformID() { return 0; }
//-------------------------------------------------------------------------**//

// class CmHpTaskCell /////////////////////////////////////////////////////////
CmHpTaskCell::CmHpTaskCell(CmHpTask *New)
{	Element=New;
	Suivant=NULL;
}
CmHpTaskCell::~CmHpTaskCell()
{   delete Element;
}

// class CTaskListe ///////////////////////////////////////////////////////////
CTaskListe::CTaskListe()
{	Tete=NULL;
	Courant=NULL;
}

CTaskListe::CTaskListe(CmHpTask *base)
{	Tete=new CmHpTaskCell(base);
	Courant=Tete;
}

BOOL CTaskListe::WriteTaskListKey()
{	CmHpTaskCell* Tmp=Tete;
	while(Tmp!=NULL)
	{	if(!Tmp->Element->WriteTaskKey()) return FALSE;
		Tmp=Tmp->Suivant;
	}
	return TRUE;
}

void CTaskListe::AddCellule(CmHpTask *base)
{	if(Tete!=NULL)
	{	CmHpTaskCell *NewCellule=new CmHpTaskCell(base);
		while(Courant->Suivant!=NULL) Courant=Courant->Suivant;
		Courant->Suivant=NewCellule;
		Courant=Tete;
	}
	else
	{	Tete=new CmHpTaskCell(base);
		Courant=Tete;
	}
}

BOOL CTaskListe::NextCellule()
{	int iReadCur=ReadCurObjKey();
	if(Courant==NULL) return FALSE;
	Courant=Courant->Suivant;
	if(this->IsChangeExeTask())
	{	if(iReadCur!=NO) WriteCurObjkey(iReadCur+1);
		else WriteCurObjkey(0);
	}
	if(Courant==NULL) return FALSE;
	return TRUE;
}

CmHpTask* CTaskListe::GetCurrentTask()
{	if(Courant!=NULL) return (Courant->Element);
	return NULL;
}

void CTaskListe::EmptyListe()
{	Courant=Tete;
	while(Tete!=NULL)
	{	Tete=Tete->Suivant;
		delete Courant;
		Courant=Tete;
	}
}

int CTaskListe::GetCountTask()
{	int iResult=0;
	Courant=Tete;
	while(Courant!=NULL)
	{	Courant=Courant->Suivant;
		iResult++;
	}
	Courant=Tete;
	return iResult;
}

int CTaskListe::GetNextmHpID()
{	int iResult=NO;
	CmHpTaskCell *Temp=Courant->Suivant;
	if(Temp!=NULL) iResult=Temp->Element->GetmHpID();
	return iResult;
}

void CTaskListe::InsertList(CmHpTaskCell *ConditionList)
{	CmHpTaskCell *TheCourant=Courant;
	Courant=Courant->Suivant;
	CmHpTaskCell *NextCellule=Courant;
	TheCourant->Suivant=ConditionList;
	Courant=ConditionList;
	while(Courant->Suivant!=NULL) Courant=Courant->Suivant;
	Courant->Suivant=NextCellule;
	Courant=TheCourant;
}

BOOL CTaskListe::IsChangeExeTask()
{	CmHpTaskCell *Tmp=Tete;
	BOOL bRes=FALSE;
	if(Tmp!=NULL)
	{	Tmp=Tmp->Suivant;
		while(Tmp!=NULL)
		{	if(!Tmp->Element->GetmHpID()) bRes=TRUE;
			Tmp=Tmp->Suivant;
		}
	}
	return bRes;
}

BOOL CTaskListe::IsRepeatListAutorized()
{	CmHpTaskCell *temp=Tete;
	char lpLastExeName[MAX_RS_SIZE]={0};
	int iLastCurWnd=0;
	int iLastCntWnd=0;
	BOOL bRes;
	COptionList* OptLst;
	if(temp!=NULL)
	{	if(!this->IsChangeExeTask())
		{	while(temp->Suivant!=NULL) temp=temp->Suivant;
			if(temp->Element->GetCurrentWnd()==0) return TRUE;
		}
		else
		{	OptLst=new COptionList();
			strncpy(lpLastExeName,temp->Element->GetExeName(),MAX_RS_SIZE);
			while(temp!=NULL)
			{	if(!temp->Element->GetmHpID())
				{	if((!iLastCurWnd)||((iLastCurWnd)&&(!iLastCntWnd))) OptLst->AddOption(TRUE,lpLastExeName,MAX_RS_SIZE);
					else OptLst->AddOption(FALSE,lpLastExeName,MAX_RS_SIZE);
					mHpPak_InitStr(lpLastExeName,MAX_RS_SIZE);
					strncpy(lpLastExeName,temp->Element->GetExeName(),MAX_RS_SIZE);
				}
				else
				{	iLastCurWnd=temp->Element->GetCurrentWnd();
					iLastCntWnd=temp->Element->GetCountWnd();
				}
				temp=temp->Suivant;
			}
			if((!iLastCurWnd)||((iLastCurWnd)&&(!iLastCntWnd))) OptLst->AddOption(TRUE,lpLastExeName,MAX_RS_SIZE);
			else OptLst->AddOption(FALSE,lpLastExeName,MAX_RS_SIZE);
			bRes=OptLst->IsRepeatOption();
			OptLst->EmptyOptions();
			delete(OptLst);
			return bRes;
		}
	}
	return FALSE;
}

BOOL CTaskListe::SetCurrentTask()
{	int iCurIndex=ReadCurObjKey();
	CmHpTaskCell *temp=Tete;
	if((iCurIndex!=NO)&&(temp!=NULL))
	{	temp=temp->Suivant;
		while((temp!=NULL)&&(iCurIndex!=0))
		{	temp=temp->Suivant;
			iCurIndex--;
		}
		Courant=temp;
		if(Courant!=NULL) return TRUE;
	}
	Courant=NULL;
	return FALSE;
}

int CTaskListe::GetCurrentObj()
{	int iRes=0;
	CmHpTaskCell *temp=Tete;
	if((temp!=NULL)&&(Courant!=NULL))
	{	temp=temp->Suivant;
		while((temp!=NULL)&&(temp!=Courant))
		{	temp=temp->Suivant;
			iRes++;
		}
	}
	return iRes;
}

HWND CTaskListe::GetLastHwndExe(char* NextExeName)
{	CmHpTaskCell *temp=Tete;
	while(temp!=NULL)
	{	if((!temp->Element->GetmHpID())&&(!strncmp(temp->Element->GetExeName(),NextExeName,MAX_RS_SIZE)))
		{	return temp->Element->GetHwndExe();}
		temp=temp->Suivant;
	}
	return NULL;
}

BOOL CTaskListe::GetContinueProc(int iCur,int iCnt)
{	BOOL bContinue=FALSE;
	if(Courant!=NULL)
	{	if((Courant->Element->GetmHpID()!=0)&&
		   (Courant->Element->GetCurrentWnd()==iCur)&&
		   (Courant->Element->GetCountWnd()==iCnt))
		{	bContinue=TRUE;}
	}
	return bContinue;
}

/*BOOL CTaskListe::IsFirstExeChange()
{	CmHpTaskCell *temp=Tete;
	BOOL bFirstChg=TRUE;
	if((Courant!=NULL)&&(temp!=NULL))
	{	temp=temp->Suivant;
		while((temp!=NULL)&&(Courant!=temp))
		{	if((!temp->Element->GetmHpID())&&(temp->Element->GetNewExe())) bFirstChg=FALSE;
			temp=temp->Suivant;
		}
	}
	return bFirstChg;
}*/

// class CmHpParam ////////////////////////////////////////////////////////////
CmHpParam::CmHpParam(char* lpDescription,char* lpLimite)
{	if(lpDescription!=NULL) strncpy(lpmHpDescription,lpDescription,MAX_RS_SIZE);
	else mHpPak_InitStr(lpmHpDescription,MAX_RS_SIZE);
	if(lpLimite!=NULL) strncpy(lpmHpLimite,lpLimite,MAX_RS_SIZE);
	else mHpPak_InitStr(lpmHpLimite,MAX_RS_SIZE);
}

CmHpParam::CmHpParam()
{}

BOOL CmHpParam::WriteParamKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int iNextPos=GetNextKeyPos();
	HKEY hmHpParamKey=NULL,hSubmHpParamKey=NULL;
	BOOL bAclRes;
	GetmHpKeyFromPos(iNextPos-1,hSubmHpParamKey,hmHpParamKey,TRUE);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hmHpParamKey!=NULL)
	{	// mHpParam
		if(RegCreateKeyEx(hmHpParamKey,MHPPARAM_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hmHpParamKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// Description
		if(!mHpPak_WriteLongStrRegVal(hmHpParamKey,MHPPARAM_KEY,DESCRIPTION_KEY,lpmHpDescription)) return FALSE;
		// Limite
		if(!mHpPak_WriteLongStrRegVal(hmHpParamKey,MHPPARAM_KEY,LIMITE_KEY,lpmHpLimite)) return FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL CmHpParam::ReadParamKey(int index)
{	HKEY hSubParamKey=NULL,hParamKey=NULL;
	ReadmHpParamKey(index,hSubParamKey,hParamKey);
	if(hParamKey!=NULL)
	{	// Description
		if(!mHpPak_ReadLongStrRegVal(hParamKey,DESCRIPTION_KEY,lpmHpDescription,MAX_RS_SIZE)) return FALSE;
		// Limite
		if(!mHpPak_ReadLongStrRegVal(hParamKey,LIMITE_KEY,lpmHpLimite,MAX_RS_SIZE)) return FALSE;
		return TRUE;
	}
	return FALSE;
}

char* CmHpParam::GetmHpLimite()
{	return lpmHpLimite;}

char* CmHpParam::GetmHpDescription()
{	return lpmHpDescription;}

void CmHpParam::SetmHpLimite(char* lpNewLimite)
{	if(lpNewLimite!=NULL) strncpy(lpmHpLimite,lpNewLimite,MAX_RS_SIZE);
	else mHpPak_InitStr(lpmHpLimite,MAX_RS_SIZE);
}

void CmHpParam::SetmHpDescription(char* lpNewDescription)
{	if(lpNewDescription!=NULL) strncpy(lpmHpDescription,lpNewDescription,MAX_RS_SIZE);
	else mHpPak_InitStr(lpmHpDescription,MAX_RS_SIZE);
}

// class CmHpWrite ////////////////////////////////////////////////////////////
CmHpWrite::CmHpWrite()
{}

void CmHpWrite::WritemHpWriteKey(HKEY &hSubmHpWriteKey,HKEY &hmHpWriteKey)
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	BYTE bTab[MAX_KEY_SIZE]={0};
	BOOL bAclRes;
	if(hSubmHpWriteKey!=NULL) RegCloseKey(hSubmHpWriteKey);
	if(hmHpWriteKey!=NULL) RegCloseKey(hmHpWriteKey);
	hSubmHpWriteKey=NULL;
	hmHpWriteKey=NULL;
	GetmHpKeyFromPos((GetNextKeyPos()-1),hmHpWriteKey,hSubmHpWriteKey,TRUE);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hSubmHpWriteKey!=NULL)
	{	// mHpWrite
		RegCreateKeyEx(hSubmHpWriteKey,MHPWRITE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hmHpWriteKey,&dWord);
	}
}

// mHpCtrl's virtuals -----------------------------------------------------**//
BOOL CmHpWrite::ReadEventKey(int i){ return NULL;}
BOOL CmHpWrite::WriteEventKey(){ return NULL;}
// Common -------------------------------------//
int CmHpWrite::GetCtrlStyle(){ return NULL;}
// Slider control -----------------------------//
int CmHpWrite::GetMinPos(){	return NULL;}
int CmHpWrite::GetMaxPos(){	return NULL;}
// Radiobox control ---------------------------//
int CmHpWrite::GetLinked(){	return NULL;}
void CmHpWrite::SetLinked(int i){ NULL;}
int CmHpWrite::GetRadioRow(){	return NULL;}
void CmHpWrite::SetRadioRow(int i){ NULL;}
char* CmHpWrite::GetRadioName(){ return NULL;}
//-------------------------------------------------------------------------**//

// class CStyleWrite //////////////////////////////////////////////////////////
CStyleWrite::CStyleWrite(int iStyle)
			:CmHpWrite()
{	iCtrlStyle=iStyle;}
		
CStyleWrite::CStyleWrite()
			:CmHpWrite()
{}

BOOL CStyleWrite::ReadEventKey(int index)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;	
	HKEY hSubStyleKey=NULL,hStyleKey=NULL;
	ReadmHpWriteKey(index,hSubStyleKey,hStyleKey);
	if(hStyleKey!=NULL)
	{	// iStyle
		if(RegQueryValueEx(hStyleKey,MHPSTYLE_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iCtrlStyle,FALSE);		
		return TRUE;
	}
	return FALSE;
}

BOOL CStyleWrite::WriteEventKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubmHpWrtKey=NULL,hmHpWrtKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size;	
	BOOL bAclRes;
	CmHpWrite::WritemHpWriteKey(hSubmHpWrtKey,hmHpWrtKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hmHpWrtKey!=NULL)
	{	// iStyle
		if(RegCreateKeyEx(hSubmHpWrtKey,MHPWRITE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iCtrlStyle,FALSE);
		RegSetValueEx(hTmpKey,MHPSTYLE_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

int CStyleWrite::GetCtrlStyle()
{	return iCtrlStyle;}

// class CEventCell ///////////////////////////////////////////////////////////
CEventCell::CEventCell(CmHpTask* pmHpTask,CmHpWrite* pmHpWrite,CmHpParam* pmHpParam)
{	mHpTask=pmHpTask;
	mHpWrite=pmHpWrite;
	mHpParam=pmHpParam;
	bViewParam=TRUE;
	NextEvent=NULL;	
}

// class CEventListe //////////////////////////////////////////////////////////
CEventListe::CEventListe()
{	mHpPak_InitStr(lpView,MAX_RS_LISTE);
	Head=NULL;
}

CEventCell* CEventListe::GetCellFromPos(int iPos,BOOL bShared)
{	CEventCell *Tmp=Head;
	int i=0,iLag;
	if(bShared)
	{	while((iPos!=i)&&(Tmp!=NULL))
		{	Tmp=Tmp->NextEvent;
			i++;
		}
	}
	else
	{	while(((iPos+1)!=i)&&(Tmp!=NULL))
		{	if(((Tmp->mHpTask->GetmHpID()==8)||(Tmp->mHpTask->GetmHpID()==16))&&(Tmp->mHpWrite->GetLinked()!=NO)) 
			{	iLag=Tmp->mHpWrite->GetLinked();
				while(iLag!=1)
				{	Tmp=Tmp->NextEvent;
					iLag--;
				}
			}
			Tmp=Tmp->NextEvent;
			i++;
		}
	}
	return Tmp;
}

CEventCell* CEventListe::GetPrevCell(CEventCell* Cell)
{	CEventCell* PrevCell=Head;
	while(PrevCell->NextEvent!=Cell) PrevCell=PrevCell->NextEvent;
	return PrevCell;
}

CEventCell* CEventListe::GetLastCell(CEventCell* Cell)
{	CEventCell* LastCell=Cell;
	int iLag;	
	if(((Cell->mHpTask->GetmHpID()==8)||(Cell->mHpTask->GetmHpID()==16))&&(Cell->mHpWrite->GetLinked()!=NO))
	{	iLag=Cell->mHpWrite->GetLinked();
		while(iLag!=1)
		{	LastCell=LastCell->NextEvent;
			iLag--;
		}
	}
	return LastCell;
}
	
void CEventListe::AddEvent(CmHpTask* AddTask,CmHpWrite* AddWrite,CmHpParam* AddParam)
{	CEventCell *NewCell=new CEventCell(AddTask,AddWrite,AddParam);
	CEventCell *Tmp=Head;
	if(Tmp==NULL) Head=NewCell;
	else
	{	while(Tmp->NextEvent!=NULL) Tmp=Tmp->NextEvent;
		Tmp=Tmp->NextEvent=NewCell;
	}
}

void CEventListe::ReplaceEvent(CmHpTask* pmHpTask,CmHpWrite* pmHpWrite)
{	CEventCell *NewCell=new CEventCell(pmHpTask,pmHpWrite,NULL);
	CEventCell *FindCell=Head;
	CEventCell *LastCell=Head;
	while(FindCell->NextEvent!=NULL)
	{	LastCell=FindCell;
		FindCell=FindCell->NextEvent;
	}
	if(FindCell!=Head)
	{	//-------------------------------------------//
		FindCell->mHpTask->InitMultiIndex();
		//-------------------------------------------//
		delete(FindCell->mHpTask);
		if(FindCell->mHpWrite!=NULL) delete(FindCell->mHpWrite);
		delete(FindCell);
		LastCell->NextEvent=NewCell;
	}
}

BOOL CEventListe::IsEmptyListe()
{	if(Head==NULL) return TRUE;
	if(Head->NextEvent==NULL) return TRUE;
	return FALSE;
}

CmHpTask* CEventListe::GetmHpTask(int iViewSel,BOOL bShared)
{	CEventCell* Tmp=this->GetCellFromPos(iViewSel,bShared);
	if(Tmp!=NULL) return Tmp->mHpTask;
	return NULL;
}

CmHpWrite* CEventListe::GetmHpWrite(int iViewSel,BOOL bShared)
{	CEventCell* Tmp=this->GetCellFromPos(iViewSel,bShared);
	if(Tmp!=NULL) return Tmp->mHpWrite;
	return NULL;
}

CmHpParam* CEventListe::GetmHpParam(int iViewSel,BOOL bShared)
{	CEventCell* Tmp=this->GetCellFromPos(iViewSel,bShared);
	if(Tmp!=NULL) return Tmp->mHpParam;
	return NULL;
}

void CEventListe::ChangeEventWnd(int iCur,int iCnt)
{	CEventCell *Tmp=Head;
	if(Head!=NULL)
	{	while(Tmp->NextEvent!=NULL) Tmp=Tmp->NextEvent;
		Tmp->mHpTask->SetCurrentWnd(iCur);
		Tmp->mHpTask->SetCountWnd(iCnt);
		RemoveLastKeyPos();
		Tmp->mHpTask->WriteTaskKey();
        if(Tmp->mHpWrite!=NULL)
            Tmp->mHpWrite->WriteEventKey();
	}
}

void CEventListe::RemoveLastEvent()
{	CEventCell *FindCell=Head;
	CEventCell *LastCell=Head;
	while(FindCell->NextEvent!=NULL)
	{	LastCell=FindCell;
		FindCell=FindCell->NextEvent;
	}
	if(FindCell!=Head)
	{	//-------------------------------------------//
		FindCell->mHpTask->InitMultiIndex();
		//-------------------------------------------//
		delete(FindCell->mHpTask);
		if(FindCell->mHpWrite!=NULL) delete(FindCell->mHpWrite);
		delete(FindCell);
		LastCell->NextEvent=NULL;
	}
}

void CEventListe::EmptyEvents()
{	CEventCell *Tmp=Head;
	while(Head!=NULL)
	{	Head=Head->NextEvent;
		//-------------------------------------//
		Tmp->mHpTask->InitMultiIndex();
		//-------------------------------------//
		delete (Tmp->mHpTask); 
		if(Tmp->mHpWrite!=NULL) delete (Tmp->mHpWrite); 
		if(Tmp->mHpParam!=NULL) delete (Tmp->mHpParam); 
		delete Tmp;
		Tmp=Head;
	}
}

UINT CEventListe::GetCustEventCount() const
{	CEventCell *Tmp=Head;
	// First control is 'ChangeExe' which can be customized
	if(Tmp!=NULL) Tmp=Tmp->NextEvent;
    int iLastID=NO;
    UINT iCustCount=0;
	while(Tmp!=NULL)
	{	BOOL bCustom=FALSE;
        // IsCustom -------------------------------------------------------**//
		ComCtrl_IsCustom(Tmp,iLastID,bCustom);
		//-----------------------------------------------------------------**//
        if(bCustom)
            iCustCount++;
		Tmp=Tmp->NextEvent;
	}
	return iCustCount;
}

BOOL CEventListe::GetNextHtmlTag(int iNextCust,char* lpHtmlTag,UINT iMaxSize) const
{	CEventCell *Tmp=Head;
	// First control is 'ChangeExe' which can be customized
	if(Tmp!=NULL) Tmp=Tmp->NextEvent;
    int iLastID=NO,iCurCust=0;
    while(Tmp!=NULL)
	{	BOOL bCustom=FALSE;
        // IsCustom -------------------------------------------------------**//
		ComCtrl_IsCustom(Tmp,iLastID,bCustom);
		//-----------------------------------------------------------------**//
        if(bCustom)
        {   if(iCurCust==iNextCust)
            {   // GetHtmlTag ---------------------------------------------**//
		        ComCtrl_GetHtmlTag(Tmp,lpHtmlTag,iMaxSize,iCurCust);
		        //---------------------------------------------------------**//
                if(*lpHtmlTag)
                    return TRUE;
                return FALSE;
            }
            iCurCust++;
        }
		Tmp=Tmp->NextEvent;
	}
    return FALSE;
}

char* CEventListe::GetStrState(int index,BOOL bWrite)
{	CEventCell *temp=this->GetCellFromPos(index,FALSE);
	mHpPak_InitStr(lpView,MAX_RS_LISTE);
	if(temp!=NULL)
	{	// StrState -------------------------------------------------------**//
		ComCtrl_StrState(temp,bWrite,lpView,MAX_RS_LISTE);
		//-----------------------------------------------------------------**//
	}
	return lpView;
}

int CEventListe::GetParamType(int index)
{	CEventCell *Temp=this->GetCellFromPos(index,FALSE);
	if(Temp!=NULL)
	{	// ParamType ------------------------------------------------------**//
		return ComCtrl_ParamType(Temp);
		//-----------------------------------------------------------------**//
	}
	return NO;
}

char* CEventListe::GetStrLimite(int index,BOOL bWrite,BOOL bShared)
{	CEventCell *tmp=this->GetCellFromPos(index,bShared);
	mHpPak_InitStr(lpView,MAX_RS_LISTE);
	if(tmp!=NULL)
	{	if(tmp->mHpParam!=NULL) return tmp->mHpParam->GetmHpLimite();
		if(!bWrite)
		{	// DefaultBound -----------------------------------------------**//
			ComCtrl_DefaultBound(tmp,lpView,MAX_RS_LISTE);
			//-------------------------------------------------------------**//
		}
	}
	return lpView;
}

char* CEventListe::GetStrDescription(int index,BOOL bShared)
{	CEventCell *tmp=this->GetCellFromPos(index,bShared);
	mHpPak_InitStr(lpView,MAX_RS_LISTE);
	if((tmp!=NULL)&&(tmp->mHpParam!=NULL)) return tmp->mHpParam->GetmHpDescription();
	return lpView;
}

void CEventListe::EmptyChange()
{	CEventCell *Tmp=Head;
	while(Tmp!=NULL)
	{	if(((Tmp->mHpTask->GetmHpID()==8)||(Tmp->mHpTask->GetmHpID()==16))&&(Tmp->mHpWrite->GetLinked()!=NO))
		{	Tmp->mHpWrite->SetLinked(NO);
			Tmp->mHpWrite->SetRadioRow(1);
		}
		if(Tmp->mHpParam!=NULL)
		{	delete Tmp->mHpParam;
			Tmp->mHpParam=NULL;
		}
		Tmp=Tmp->NextEvent;
	}
}

BOOL CEventListe::GetNextEvent(int index,BOOL bShared)
{	CEventCell *tmp=this->GetCellFromPos(index,bShared);
	if(tmp==NULL) return FALSE;
	return TRUE;
}

char* CEventListe::GetLastExeName(int index)
{	CEventCell* Tmp=Head;
	int i=0;
	mHpPak_InitStr(lpView,MAX_RS_LISTE);
	strncpy(lpView,Tmp->mHpTask->GetExeName(),MAX_RS_LISTE);
	Tmp=this->GetCellFromPos(0,FALSE);
	if(index==NO)
	{	while(Tmp!=NULL)
		{	if(!Tmp->mHpTask->GetmHpID())
			{	mHpPak_InitStr(lpView,MAX_RS_LISTE);
				strncpy(lpView,Tmp->mHpTask->GetExeName(),MAX_RS_LISTE);
			}
			i++;
			Tmp=this->GetCellFromPos(i,FALSE);
		}
	}
	else while((Tmp!=NULL)&&((index+1)!=i))
	{	if(!Tmp->mHpTask->GetmHpID())
		{	mHpPak_InitStr(lpView,MAX_RS_LISTE);
			strncpy(lpView,Tmp->mHpTask->GetExeName(),MAX_RS_LISTE);
		}
		i++;
		Tmp=this->GetCellFromPos(i,FALSE);
	}
	return lpView;
}

BOOL CEventListe::IsCtrlState(int index,BOOL bCustom)
{	CEventCell *Tmp=this->GetCellFromPos(index,FALSE);
	if(Tmp!=NULL)
	{	// IsCustomized ---------------------------------------------------**//
		return ComCtrl_IsCustomized(Tmp,index,bCustom);
		//-----------------------------------------------------------------**//
	}
	return FALSE;
}

BOOL CEventListe::IsNoStateMsg(int index)
{	CEventCell *Tmp=this->GetCellFromPos(index,FALSE);
	if(Tmp!=NULL)
	{	// IsNoStateMsg ---------------------------------------------------**//
		return ComCtrl_IsNoStateMsg(Tmp);
		//-----------------------------------------------------------------**//
	}
	return FALSE;
}

void CEventListe::SetStrLimite(int index,char* lpNewLimite)
{	CEventCell *Tmp=this->GetCellFromPos(index,FALSE);
	if(Tmp!=NULL)
	{	if(Tmp->mHpParam!=NULL) Tmp->mHpParam->SetmHpLimite(lpNewLimite);
		else Tmp->mHpParam=new CmHpParam(NULL,lpNewLimite);
	}
}

void CEventListe::SetStrDescription(int index,char* lpNewDecrit)
{	CEventCell *Tmp=this->GetCellFromPos(index,FALSE);
	if(Tmp!=NULL)
	{	if(Tmp->mHpParam!=NULL) Tmp->mHpParam->SetmHpDescription(lpNewDecrit);
		else Tmp->mHpParam=new CmHpParam(lpNewDecrit,NULL);
	}
}

void CEventListe::SetMsg(int index,char* lpmsg)
{	int i=0;
	CEventCell *temp=this->GetCellFromPos(index,FALSE);
	if(temp!=NULL) temp->mHpTask->SetmHpMsg(lpmsg);
}

void CEventListe::SetStateMsg(int index,char* lpmsg)
{	int i=0;
	CEventCell *temp=this->GetCellFromPos(index,FALSE);
	if(temp!=NULL) temp->mHpTask->SetmHpStateMsg(lpmsg);
}

void CEventListe::MovemHpCtrl(int iPos,int iNextPos)
{	CEventCell* MoveCell=this->GetCellFromPos(iPos,FALSE);
	CEventCell* PrevNext=this->GetCellFromPos(iNextPos,FALSE);
	if((MoveCell!=NULL)&&(PrevNext!=NULL)&&(iPos!=iNextPos))
	{	if(iPos>iNextPos) // Up
		{	this->GetPrevCell(MoveCell)->NextEvent=this->GetLastCell(MoveCell)->NextEvent;
			this->GetPrevCell(PrevNext)->NextEvent=MoveCell;
			this->GetLastCell(MoveCell)->NextEvent=PrevNext;
		}
		else // Down
		{	this->GetPrevCell(MoveCell)->NextEvent=this->GetLastCell(MoveCell)->NextEvent;
			this->GetLastCell(MoveCell)->NextEvent=this->GetLastCell(PrevNext)->NextEvent;
			this->GetLastCell(PrevNext)->NextEvent=MoveCell;
		}
	}
}

void CEventListe::DeletemHpParam(int iPos)
{	CEventCell* Tmp=this->GetCellFromPos(iPos,FALSE);
	if((Tmp!=NULL)&&(Tmp->mHpParam!=NULL))
	{	delete Tmp->mHpParam;
		Tmp->mHpParam=NULL;
	}
}

int CEventListe::GetEventCount()
{	int iRes=0;
	CEventCell* Tmp=Head;
	while(Tmp!=NULL)
	{	iRes++;
		Tmp=Tmp->NextEvent;
	}
	return iRes;
}

BOOL CEventListe::IsAnAppActive()
{	CEventCell* Tmp=Head;
	int iCntNew=0,iCntClose=0;
	while(Tmp!=NULL)
	{	if((!Tmp->mHpTask->GetmHpID())&&(Tmp->mHpTask->GetNewExe())) iCntNew++;
		else if(Tmp->mHpTask->GetCurrentWnd()==NO) iCntClose++;
		Tmp=Tmp->NextEvent;
	}
	if(iCntNew==iCntClose) return FALSE;
	return TRUE;
}

BOOL CEventListe::GetCustomVisible(int iPos)
{	CEventCell* Tmp=this->GetCellFromPos(iPos,FALSE);
	if(Tmp!=NULL) return Tmp->bViewParam;
	return TRUE;
}

void CEventListe::SetCustomVisible(int iPos,BOOL bVisible)
{	CEventCell* Tmp=this->GetCellFromPos(iPos,FALSE);
	if(Tmp!=NULL) Tmp->bViewParam=bVisible;
}

//---------------------------------------------------------------------------//
// class COptionCell
COptionCell::COptionCell(BOOL bOpt,char* NameApp,int NameSize)
{	bRepeatOption=bOpt;
	mHpPak_InitStr(lpNameApp,MAX_RS_SIZE);
	if(strncmp(NameApp,NULL_STRING,NameSize)!=0) strncpy(lpNameApp,NameApp,MAX_RS_SIZE);
	NextOption=NULL;
}

// class COptionList
COptionList::COptionList()
{	HeadOption=NULL;}

void COptionList::AddOption(BOOL bNewOpt,char* NewNameApp,int AppSize)
{	COptionCell* NewCell=new COptionCell(bNewOpt,NewNameApp,AppSize);
	COptionCell* TmpCell=HeadOption;
	if(HeadOption!=NULL)
	{	while(TmpCell->NextOption!=NULL) TmpCell=TmpCell->NextOption;
		TmpCell->NextOption=NewCell;
	}
	else HeadOption=NewCell;
}

BOOL COptionList::IsRepeatOption()
{	char lpTmp[MAX_RS_SIZE]={0};
	COptionCell* TmpCell=HeadOption;
	COptionCell* CellTmp;
	BOOL bTmp=FALSE;
	while(TmpCell->NextOption!=NULL) TmpCell=TmpCell->NextOption;
	if((TmpCell->bRepeatOption)&&(HeadOption->bRepeatOption)&&(!strncmp(TmpCell->lpNameApp,HeadOption->lpNameApp,MAX_RS_SIZE)))
	{	TmpCell=HeadOption;
		while(TmpCell!=NULL)
		{	if(strncmp(TmpCell->lpNameApp,NULL_STRING,MAX_RS_SIZE)!=0)
			{	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
				strncpy(lpTmp,TmpCell->lpNameApp,MAX_RS_SIZE);
				CellTmp=HeadOption;
				while(CellTmp!=NULL)
				{	if(!strncmp(CellTmp->lpNameApp,lpTmp,MAX_RS_SIZE)) bTmp=CellTmp->bRepeatOption;
					CellTmp=CellTmp->NextOption;
				}
				if(!bTmp) return FALSE;
			}
			else return FALSE;
			TmpCell=TmpCell->NextOption;
		}
		return TRUE;
	}
	return FALSE;
}

void COptionList::EmptyOptions()
{	COptionCell* TmpCell;
	while(HeadOption!=NULL)
	{	TmpCell=HeadOption->NextOption;
		delete HeadOption;
		HeadOption=TmpCell;		
	}
}

//------------------------------------------------//
// WriteCurObjkey
//------------------------------------------------//
BOOL WriteCurObjkey(int iCurObj)
{	SECURITY_ATTRIBUTES lpSecu;
	DWORD dWord=REG_OPENED_EXISTING_KEY;
	BYTE lpBt[MAX_KEY_SIZE]={0};
	DWORD Size=MAX_KEY_SIZE;
	HKEY hTmpKey,hSubObjCurKey=NULL,hObjCurKey=NULL;
	BOOL bAclRes;
	WritemHpSharedKey(hSubObjCurKey,hObjCurKey);
	mHpPak_GetSecurityAtt(lpSecu,bAclRes);
	if(hObjCurKey!=NULL)
	{	// mHpObjs
		if(RegCreateKeyEx(hObjCurKey,MHPOBJS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE|KEY_READ,&lpSecu,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// CurObj
		if(RegCreateKeyEx(hObjCurKey,MHPOBJS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		Size=mHpPak_FillDataFromNumber(lpBt,iCurObj,FALSE);
		RegSetValueEx(hTmpKey,CUROBJ_KEY,0,REG_DWORD,lpBt,Size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------//
// WriteCursorPoskey
//------------------------------------------------//
BOOL WriteCursorPoskey(int xPos,int yPos)
{	SECURITY_ATTRIBUTES lpSecu;
	DWORD dWord=REG_OPENED_EXISTING_KEY;
	BYTE lpBt[MAX_KEY_SIZE]={0};
	DWORD Size=MAX_KEY_SIZE;
	HKEY hTmpKey,hSubCurPosKey=NULL,hCurPosKey=NULL;
	BOOL bAclRes;
	WritemHpSharedKey(hSubCurPosKey,hCurPosKey);
	mHpPak_GetSecurityAtt(lpSecu,bAclRes);
	if(hCurPosKey!=NULL)
	{	// mHpObjs
		if(RegCreateKeyEx(hCurPosKey,MHPOBJS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE|KEY_READ,&lpSecu,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// xCurPos
		if(RegCreateKeyEx(hCurPosKey,MHPOBJS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		Size=mHpPak_FillDataFromNumber(lpBt,xPos,FALSE);
		RegSetValueEx(hTmpKey,XCURPOS_KEY,0,REG_DWORD,lpBt,Size);
		RegCloseKey(hTmpKey);
		// yCurPos
		if(RegCreateKeyEx(hCurPosKey,MHPOBJS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		Size=mHpPak_FillDataFromNumber(lpBt,yPos,FALSE);
		RegSetValueEx(hTmpKey,YCURPOS_KEY,0,REG_DWORD,lpBt,Size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------//
// ReadCursorPoskey
//------------------------------------------------//
void ReadCursorPoskey(int &xPos,int &yPos)
{	HKEY hCurPosKey;
	BYTE lpBt[MAX_KEY_SIZE]={0};
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	// Softawre
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&hCurPosKey)==ERROR_SUCCESS)
	{	// mHelp
		if(RegOpenKeyEx(hCurPosKey,MHELP_KEY,0,KEY_READ,&hCurPosKey)==ERROR_SUCCESS)
		{	// mHpShared
			if(RegOpenKeyEx(hCurPosKey,MHPSHARED_KEY,0,KEY_READ,&hCurPosKey)==ERROR_SUCCESS)
			{	// mHpObjs
				if(RegOpenKeyEx(hCurPosKey,MHPOBJS_KEY,0,KEY_READ,&hCurPosKey)==ERROR_SUCCESS)
				{	// xCurPos
					if(RegQueryValueEx(hCurPosKey,XCURPOS_KEY,NULL,&dwType,lpBt,&iSize)==ERROR_SUCCESS)
					{	mHpPak_FillIntFromData(lpBt,xPos,FALSE);
						iSize=MAX_KEY_SIZE;dwType=REG_DWORD;
						// yCurPos
						if(RegQueryValueEx(hCurPosKey,YCURPOS_KEY,NULL,&dwType,lpBt,&iSize)==ERROR_SUCCESS)
						{	mHpPak_FillIntFromData(lpBt,yPos,FALSE);
							return;
						}
					}
				}
			}
		}
	}
	xPos=NO;yPos=NO;
}

//------------------------------------------------//
// WritemHpSharedKey
//------------------------------------------------//
void WritemHpSharedKey(HKEY &hSubSharedKey,HKEY &hSharedKey)
{	SECURITY_ATTRIBUTES lpSecu;
	DWORD dWord=REG_OPENED_EXISTING_KEY;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecu,bAclRes);
	if(hSubSharedKey!=NULL) RegCloseKey(hSubSharedKey);
	if(hSharedKey!=NULL) RegCloseKey(hSharedKey);
	hSubSharedKey=NULL;
	hSharedKey=NULL;
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&hSubSharedKey,&dWord)!=ERROR_SUCCESS) return;
	// mHelp
	if(RegCreateKeyEx(hSubSharedKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&hSubSharedKey,&dWord)!=ERROR_SUCCESS) return;
	// mHpShared
	RegCreateKeyEx(hSubSharedKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE|KEY_READ,&lpSecu,
					&hSharedKey,&dWord);
}

//------------------------------------------------//
// GetmHpKeyFromPos
//------------------------------------------------//
void GetmHpKeyFromPos(int iRang,HKEY &hSubPosKey,HKEY &hPosKey,BOOL bWriteRead)
{	SECURITY_ATTRIBUTES lSecu;
	DWORD Word=REG_OPENED_EXISTING_KEY;
	char lpRang[MAX_KEY_SIZE]={0};
	BOOL bAclRes;
	if(hSubPosKey!=NULL) RegCloseKey(hSubPosKey);
	if(hPosKey!=NULL) RegCloseKey(hPosKey);
	hSubPosKey=NULL;
	hPosKey=NULL;
	mHpPak_GetSecurityAtt(lSecu,bAclRes);
	_snprintf(lpRang,MAX_KEY_SIZE,"%d",iRang);
	if(bWriteRead)
	{	WritemHpSharedKey(hSubPosKey,hPosKey);
		if(hSubPosKey!=NULL)
		{	// mHpObjs
			if(RegCreateKeyEx(hPosKey,MHPOBJS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE|KEY_READ,&lSecu,
							&hSubPosKey,&Word)!=ERROR_SUCCESS) return;
			// indice
			RegCloseKey(hPosKey);
			RegCreateKeyEx(hSubPosKey,lpRang,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE|KEY_READ,&lSecu,
							&hPosKey,&Word);
		}
	}
	else
	{	// Softawre
		if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&hSubPosKey)!=ERROR_SUCCESS) return;
		// mHelp
		if(RegOpenKeyEx(hSubPosKey,MHELP_KEY,0,KEY_READ,&hSubPosKey)!=ERROR_SUCCESS) return;
		// mHpShared
		if(RegOpenKeyEx(hSubPosKey,MHPSHARED_KEY,0,KEY_READ,&hSubPosKey)!=ERROR_SUCCESS) return;
		// mHpObjs
		if(RegOpenKeyEx(hSubPosKey,MHPOBJS_KEY,0,KEY_READ,&hSubPosKey)!=ERROR_SUCCESS) return;
		// indice
		RegOpenKeyEx(hSubPosKey,lpRang,0,KEY_READ,&hPosKey);
	}
}

//------------------------------------------------//
// ReadmHpTaskKey
//------------------------------------------------//
void ReadmHpTaskKey(int iRang,HKEY &hSubTaskKey,HKEY &hTaskKey)
{	GetmHpKeyFromPos(iRang,hTaskKey,hSubTaskKey,FALSE);
	if(hSubTaskKey!=NULL)
	{	RegCloseKey(hTaskKey);
		RegOpenKeyEx(hSubTaskKey,MHPTASK_KEY,0,KEY_READ,&hTaskKey);
	}
}

//------------------------------------------------//
// ReadmHpParamKey
//------------------------------------------------//
void ReadmHpParamKey(int iRang,HKEY &hSubParamKey,HKEY &hParamKey)
{	GetmHpKeyFromPos(iRang,hParamKey,hSubParamKey,FALSE);
	if(hSubParamKey!=NULL)
	{	RegCloseKey(hParamKey);
		RegOpenKeyEx(hSubParamKey,MHPPARAM_KEY,0,KEY_READ,&hParamKey);
	}
}

//***********************************************************************HTML//
// IsHtmlCustSelected
//------------------------------------------------//
BOOL IsHtmlCustSelected(const std::string& PostData,UINT iParamIdx)
{
    // Return if HTML parameter is selected
    char lpIdxState[64];
    _snprintf(lpIdxState,64,"%d-ParamSel=on",iParamIdx);
    size_t iPos=PostData.find(lpIdxState);
    if(iPos==std::string::npos)
        return FALSE; // Not selected

    // Selected
    return TRUE;
}

//------------------------------------------------//
// GetHtmlCustValue
//------------------------------------------------//
BOOL GetHtmlCustValue(char* lpValue,UINT iMaxSize,const std::string& PostData,const char* lpStateName,UINT iParamIdx)
{
    *lpValue=0;

    // Check existing HTML state parameter
    char lpIdxState[MAX_MID_SIZE];
    _snprintf(lpIdxState,MAX_MID_SIZE,"%d-%s=",iParamIdx,lpStateName);
    size_t iValPos=PostData.find(lpIdxState);
    if(iValPos==std::string::npos)
        return FALSE; // Error: No state parameter
    iValPos+=strlen(lpIdxState);

    // Get HTML state value
    UINT i=0;
    for( ; (i<iMaxSize)&&(iValPos<PostData.length())&&(PostData.at(iValPos)!='&'); i++,iValPos++)
    {   lpValue[i]=PostData.at(iValPos);
    }
    if(i<iMaxSize)
        lpValue[i]=0;
    else
        *lpValue=0;

    return TRUE;
}

//*******************************************************************KEYSPROC//
// GetNextKeyPos
//------------------------------------------------//
int GetNextKeyPos()
{	HKEY hSubPosKey=NULL,hPosKey=NULL;
	int iPos=0;
	GetmHpKeyFromPos(iPos,hSubPosKey,hPosKey,FALSE);
	while(hPosKey!=NULL)
	{	iPos++;
		GetmHpKeyFromPos(iPos,hSubPosKey,hPosKey,FALSE);
	}
	return iPos;
}

//------------------------------------------------//
// GetmHpIDKey
//------------------------------------------------//
int GetmHpIDKey(int iRang)
{	HKEY hSubKeyRang=NULL,hKeyRang=NULL;
	int imHpIDRes=NO;
	BYTE lpBt[MAX_KEY_SIZE]={0};
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;	
	GetmHpKeyFromPos(iRang,hSubKeyRang,hKeyRang,FALSE);
	if(hKeyRang!=NULL)
	{	// mHpID
		if(RegQueryValueEx(hKeyRang,MHPID_KEY,NULL,&dwType,lpBt,&iSize)==ERROR_SUCCESS)
		{	mHpPak_FillIntFromData(lpBt,imHpIDRes,FALSE);}
		RegCloseKey(hKeyRang);
	}
	return imHpIDRes;
}

//------------------------------------------------//
// RemoveLastKeyPos
//------------------------------------------------//
void RemoveLastKeyPos()
{	char lpRemove[MAX_KEY_SIZE]={0};
	HKEY hSubRemoveKey=NULL,hRemoveKey=NULL;
	int iNextKeyPos=GetNextKeyPos();
	if(iNextKeyPos!=0)
	{	GetmHpKeyFromPos((iNextKeyPos-1),hSubRemoveKey,hRemoveKey,TRUE);
		if(hRemoveKey!=NULL)
		{	_snprintf(lpRemove,MAX_KEY_SIZE,"%d",(iNextKeyPos-1));
			mHpPak_DeleteSubKey(hSubRemoveKey,lpRemove);
		}
	}
}

//------------------------------------------------//
// SetOperationModeKey
//------------------------------------------------//
void SetOperationModeKey(int iOperation)
{	DWORD oWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES lpSecu;
	BYTE bTab[MAX_KEY_SIZE]={0};
	HKEY hTmpKey,hSubShared=NULL,hShared=NULL;
	int Size;
	BOOL bAclRes;
	WritemHpSharedKey(hSubShared,hShared);
	mHpPak_GetSecurityAtt(lpSecu,bAclRes);
	if(hShared!=NULL)
	{	// NoReadWrite
		if(RegCreateKeyEx(hSubShared,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
						&hTmpKey,&oWord)==ERROR_SUCCESS)
		{	Size=mHpPak_FillDataFromNumber(bTab,iOperation,FALSE);
			RegSetValueEx(hTmpKey,READWRITE_KEY,0,REG_DWORD,bTab,Size);
			RegCloseKey(hTmpKey);
		}
	}
}

//------------------------------------------------//
// ReadCurObjKey
//------------------------------------------------//
int ReadCurObjKey()
{	int iIndex;
	HKEY hObjKey;
	BYTE lpBt[MAX_KEY_SIZE]={0};
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	// Softawre
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&hObjKey)==ERROR_SUCCESS)
	{	// mHelp
		if(RegOpenKeyEx(hObjKey,MHELP_KEY,0,KEY_READ,&hObjKey)==ERROR_SUCCESS)
		{	// mHpShared
			if(RegOpenKeyEx(hObjKey,MHPSHARED_KEY,0,KEY_READ,&hObjKey)==ERROR_SUCCESS)
			{	// mHpObjs
				if(RegOpenKeyEx(hObjKey,MHPOBJS_KEY,0,KEY_READ,&hObjKey)==ERROR_SUCCESS)
				{	// CurObj
					if(RegQueryValueEx(hObjKey,CUROBJ_KEY,NULL,&dwType,lpBt,&iSize)==ERROR_SUCCESS)
					{	mHpPak_FillIntFromData(lpBt,iIndex,FALSE);
						return iIndex;
					}
				}
			}
		}
	}
	return NO;
}

//------------------------------------------------//
// WriteHwndExeKey
//------------------------------------------------//
BOOL WriteHwndExeKey(HWND hWndActive,char* lpNameExe)
{	char lpReadExe[MAX_RS_SIZE]={0};
	DWORD opWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	BYTE BTab[MAX_KEY_SIZE]={0};
	HKEY hTmpKey,hSubNextKey=NULL,hNextKey=NULL;
	int iSize,iIndex=0;
	BOOL bRes=FALSE,bFind=FALSE;
	BOOL bAclRes;
	GetmHpKeyFromPos(0,hSubNextKey,hNextKey,FALSE);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	while(hNextKey!=NULL)
	{	// mHpTask
		if(RegOpenKeyEx(hNextKey,MHPTASK_KEY,0,KEY_READ,&hNextKey)==ERROR_SUCCESS)
		{	// ChangeExe
			if(RegOpenKeyEx(hNextKey,CHANGEEXE_KEY,0,KEY_READ,&hNextKey)==ERROR_SUCCESS)
			{	// AppName
				if(mHpPak_ReadLongStrRegVal(hNextKey,APPNAME_KEY,lpReadExe,MAX_RS_SIZE))
				{	if(!strncmp(lpReadExe,lpNameExe,MAX_RS_SIZE))
					{	bRes=TRUE;
						bFind=TRUE;
					}
				}
			}
		}
		RegCloseKey(hNextKey);
		if(bFind)
		{	GetmHpKeyFromPos(iIndex,hSubNextKey,hNextKey,FALSE);
			// mHpTask
			if(RegCreateKeyEx(hNextKey,MHPTASK_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
							&hNextKey,&opWord)!=ERROR_SUCCESS) return FALSE;
			// ChangeExe
			if(RegCreateKeyEx(hNextKey,CHANGEEXE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
							&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
			// HwndExe
			if(RegCreateKeyEx(hNextKey,CHANGEEXE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
							&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
			iSize=mHpPak_FillDataFromNumber(BTab,(int)hWndActive,FALSE);
			RegSetValueEx(hTmpKey,HWNDEXE_KEY,0,REG_DWORD,BTab,iSize);
			RegCloseKey(hTmpKey);
		}
		bFind=FALSE;
		iIndex++;
		GetmHpKeyFromPos(iIndex,hSubNextKey,hNextKey,FALSE);
	}
	return bRes;
}

//---------------------------------------------//
// ReadHwndExeKey
//---------------------------------------------//
HWND ReadHwndExeKey(char* lpNameExe)
{	BYTE ByteTab[MAX_KEY_SIZE]={0};
	char lpExe[MAX_RS_SIZE]={0};
	HKEY hSubHwndKey=NULL,hHwndKey=NULL;
	DWORD isize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	int Index=0,iTmp;
	GetmHpKeyFromPos(0,hSubHwndKey,hHwndKey,FALSE);
	while(hHwndKey!=NULL)
	{	// mHpTask
		if(RegOpenKeyEx(hHwndKey,MHPTASK_KEY,0,KEY_READ,&hHwndKey)==ERROR_SUCCESS)
		{	// ChangeExe
			if(RegOpenKeyEx(hHwndKey,CHANGEEXE_KEY,0,KEY_READ,&hHwndKey)==ERROR_SUCCESS)
			{	// AppName
				if(mHpPak_ReadLongStrRegVal(hHwndKey,APPNAME_KEY,lpExe,MAX_RS_SIZE))
				{	if(!strncmp(lpExe,lpNameExe,MAX_RS_SIZE))
					{	// HwndExe
						if(RegQueryValueEx(hHwndKey,HWNDEXE_KEY,NULL,&dwType,ByteTab,&isize)==ERROR_SUCCESS)
						{	mHpPak_FillIntFromData(ByteTab,iTmp,FALSE);
							return ((HWND)iTmp);
						}
					}
				}
			}
		}
		RegCloseKey(hHwndKey);
		Index++;
		GetmHpKeyFromPos(Index,hSubHwndKey,hHwndKey,FALSE);
	}
	return NULL;
}

//---------------------------------------------//
// ReadmHpWriteKey
//---------------------------------------------//
void ReadmHpWriteKey(int iRang,HKEY &hSubWriteKey,HKEY &hWriteKey)
{	GetmHpKeyFromPos(iRang,hWriteKey,hSubWriteKey,FALSE);
	if(hSubWriteKey!=NULL)
	{	RegCloseKey(hWriteKey);
		RegOpenKeyEx(hSubWriteKey,MHPWRITE_KEY,0,KEY_READ,&hWriteKey);
	}
}

//*****************************************************************ACCESSEURS//
// Get ////////////////////////////////////////////////////////////////////////
MHPVARSTRUCT* GetmHpVarStruct()
{	static BOOL bInitStruct=FALSE;
	if(!bInitStruct)
	{	mHpStructVar=(MHPVARSTRUCT*)malloc(sizeof(MHPVARSTRUCT));
		mHpStructVar->hmHpCtrlInst=NULL;
		mHpStructVar->bBlockUpdateFrame=FALSE;
		mHpStructVar->bRefreshDDraw=FALSE;
		mHpStructVar->XposCur=0;
		mHpStructVar->YposCur=0;
		mHpStructVar->XnextPos=0;
		mHpStructVar->YnextPos=0;
		mHpStructVar->CourantTask=NULL;
		mHpStructVar->RepeatLoop=0;
		mHpStructVar->AscensSens=0;
		mHpStructVar->ItemsVisu=0;
		mHpStructVar->iLastTopIndex=NO;
        mHpStructVar->hWndCurCtrl=NULL;
        //mHpStructVar->bLockDotNetMsg=FALSE;
        mHpStructVar->CurAct=ImHpCtrlMgmt::ActEnd;
        mHpStructVar->szLastClass[0]=0;
        mHpStructVar->iLastStyle=NO;
        mHpStructVar->pLastID=NULL;
		mHpStructVar->CursorType=0;
		mHpStructVar->bNewDepl=TRUE;
		mHpStructVar->EcartDepl=14;
		mHpStructVar->iCurAnim=0;
		mHpStructVar->iCntAnim=0;
		mHpStructVar->iMaxAnim=3;
		mHpStructVar->bLockAnim=FALSE;
        mHpStructVar->bMoveDone=FALSE;
		mHpStructVar->CboEditIndice=0;
		mHpStructVar->EditFlag=FALSE;
		mHpStructVar->xResolution=0;
		mHpStructVar->yResolution=0;
		mHpStructVar->DDrawBitmap=0;
		mHpStructVar->SelItemListe=NULL;
		mHpStructVar->CourantIDCommand=1;
		mHpStructVar->bConserveBGD=FALSE;
		mHpStructVar->iCtrlSel=0;
		mHpStructVar->bCustomFolder=FALSE;
		mHpStructVar->iLnkLag=NO;
		mHpStructVar->iLnkSel=NO;
		mHpStructVar->hHandleExeWnd=NULL;
		mHpStructVar->bNoInactiveMsg=FALSE;
		mHpStructVar->bLockWaMsg=FALSE;
		mHpStructVar->bCreateTaskObj=FALSE;
		mHpStructVar->CtrlLink=NULL;
		mHpStructVar->DelayLoop=0;
		mHpStructVar->iCurObj=0;
		mHpStructVar->iCurOption=0;
		mHpStructVar->bContinueProc=FALSE;
		mHpStructVar->bNextmHpID=FALSE;
		mHpStructVar->bWaitCtrl=FALSE;
		mHpStructVar->iSkipSnd=0;
		mHpStructVar->bLockUnWaMsg=FALSE;
		mHpStructVar->iLastCmdID=NO;
		mHpStructVar->hCurMenu=NULL;
		mHpStructVar->imHpProcID=0;
		mHpStructVar->bNoClearmHpKeys=FALSE;
		//----------------------------------//
		mHpStructVar->imHpMode=0;
		mHpStructVar->imHpMouseSpeed=0;
		mHpStructVar->imHpRepeat=0;
		mHpStructVar->imHpDelay=0;
		mHpStructVar->TaskList=NULL;
		mHpStructVar->EventList=NULL;
        mHpStructVar->CtrlMgmtList=NULL;
		mHpPak_EmptyTab(mHpStructVar->IndexTab,MAX_PART_TREE);
		mHpPak_InitStr(mHpStructVar->mHpFileName,MAX_RS_SIZE);
		mHpPak_InitStr(mHpStructVar->lpCurApp,MAX_RS_SIZE);
		bInitStruct=TRUE;
	}
	return mHpStructVar;
}

// FREE ///////////////////////////////////////////////////////////////////////
void FreemHpVarStruct()
{	free(mHpStructVar);}

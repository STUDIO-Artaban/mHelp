// ListCtrl.cpp
//

#include "stdafx.h"
#include <ListBox\ListCtrl.h>

#include <stdio.h>
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
#define		CPP_TASK_LISTBOX			"ListBox"                       // C/C++ control
#define		VB_TASK_LISTBOX				"ThunderRT6ListBox"             // VB control
#define     DOTNET_TASK_LISTBOX         "WindowsForms10.LISTBOX.app"    // DotNet control

//-----------------------------------------------------//
// KeyName
#define		LISTBOX_KEY					"ListBox"

//-----------------------------------------------------------------------------------------------------------------------//
// Définition
#define		LIST_MAXI_SELITEMS				500

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles
int CourantIndex=0;						// Index courant d'une ListBox multiple...
int AscensInc=0;						// Nb de deplacement de l'ascenseur (ListBox Multiple).
BOOL bAllFnd=FALSE;						// TRUE: All item to select have been found, FALSE: orelse

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration
void SetYListPos(HWND);
void ChangeCourantItemState(HWND);
BOOL IsIndexInVisu(HWND);
BOOL NextMultiListStep(HWND);
BOOL NoChangeMultiList(HWND);

//-----------------------------------------------------//
// SetYListPos
//-----------------------------------------------------//
void SetYListPos(HWND hWndActive)
{	int Temp=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETTOPINDEX,0,0);
	if(Temp!=CourantIndex) GetmHpVarStruct()->YnextPos+=((CourantIndex-Temp)*ITEM_ECART);
}

//-----------------------------------------------------//
// ChangeCourantItemState
//-----------------------------------------------------//
void ChangeCourantItemState(HWND hWndActive)
{	if(GetmHpVarStruct()->imHpMode!=2)
	{	if(SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETSEL,(WPARAM)CourantIndex,0))
		    SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_SETSEL,(WPARAM)FALSE,(LPARAM)(UINT)CourantIndex);
		else
			SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_SETSEL,(WPARAM)TRUE,(LPARAM)(UINT)CourantIndex);
	}
	// Force le message "LBN_SELCHANGE"
	ForceMessage(hWndActive,LBN_SELCHANGE);
}

//-----------------------------------------------------//
// IsIndexInVisu
//-----------------------------------------------------//
BOOL IsIndexInVisu(HWND hWndActive)
{	int topIndex=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETTOPINDEX,0,0);
    if(((CourantIndex>=topIndex)&&((topIndex+GetmHpVarStruct()->ItemsVisu-1)>=CourantIndex))) return TRUE;
	if(CourantIndex<topIndex)
	{	AscensInc=topIndex-CourantIndex;
		GetmHpVarStruct()->AscensSens=1;
	}
	else
	{	AscensInc=CourantIndex-(topIndex+GetmHpVarStruct()->ItemsVisu-1);
		GetmHpVarStruct()->AscensSens=2;
	}
	return FALSE;
}

//-----------------------------------------------------//
// NextMultiListStep
//-----------------------------------------------------//
BOOL NextMultiListStep(HWND hWndActive)
{	int SelItem=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
	if(SelItem!=NO) CourantIndex++;
	else return FALSE;
	if(IsIndexInVisu(hWndActive)) GetmHpVarStruct()->YnextPos+=ITEM_ECART;
	while((SelItem!=CourantIndex)&&(IsIndexInVisu(hWndActive)))
	{	GetmHpVarStruct()->YnextPos+=ITEM_ECART;
		CourantIndex++;
	}
	if(!IsIndexInVisu(hWndActive))
	{	RECT ItemRect;
		GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&ItemRect);
		GetmHpVarStruct()->AscensSens=2;
		AscensInc=(SelItem-CourantIndex)+GetmHpVarStruct()->ItemsVisu;
		CourantIndex=SelItem;
		GetmHpVarStruct()->XnextPos=(int)(ItemRect.right-8);
		GetmHpVarStruct()->YnextPos=(int)(ItemRect.bottom-ITEM_ECART);
	}
	GetmHpVarStruct()->bNewDepl=TRUE;
	GetmHpVarStruct()->CourantTask->SetmHpStep(2);
	SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
	return TRUE;
}

//-----------------------------------------------------//
// NoChangeMultiList
//-----------------------------------------------------//
BOOL NoChangeMultiList(HWND hWndActive)
{	int indice=0;
	int SelItems[LIST_MAXI_SELITEMS]={0};
	int SelCount=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETSELITEMS,(WPARAM)LIST_MAXI_SELITEMS,(LPARAM)(LPINT)SelItems);
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	while(indice!=SelCount)
	{	if(!GetmHpVarStruct()->SelItemListe->IsListeEmpty())
		{	if(GetmHpVarStruct()->SelItemListe->GetItemIndex()!=SelItems[indice])
			{	if(!GetmHpVarStruct()->SelItemListe->InsertIndexItem(SelItems[indice]))
					GetmHpVarStruct()->SelItemListe->RemoveIndexItem();
			}
			else GetmHpVarStruct()->SelItemListe->RemoveIndexItem();
		}
		else
		{	if(!GetmHpVarStruct()->SelItemListe->InsertIndexItem(SelItems[indice]))
				GetmHpVarStruct()->SelItemListe->RemoveIndexItem();
		}
		indice++;
	}
	GetmHpVarStruct()->SelItemListe->ReplaceCurrent();
	return GetmHpVarStruct()->SelItemListe->IsListeEmpty();
}

//*****************************************************************************************************************OBJECT//
// class CListTask
CListTask::CListTask(int TaskID,char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,char* lpCtrlName)
 		  :CmHpTask(TaskID,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,lpCtrlName)
{	HeadSel=NULL;
	CurrentSel=NULL;
}

CListTask::CListTask()
 		  :CmHpTask()
{	HeadSel=NULL;
	CurrentSel=NULL;
}

BOOL CListTask::ReadTaskKey(int index)
{	char lpLstSel[MAX_RS_SIZE]={0};
	char lpIndice[MAX_KEY_SIZE]={0};
	BYTE BtTab[MAX_KEY_SIZE]={0};
	HKEY hTmpKey,hSubLstKey=NULL,hLstKey=NULL;
	int indice=0;
	CmHpTask::ReadCommonKey(index,TRUE,hSubLstKey,hLstKey);
	if((hLstKey!=NULL)&&(RegOpenKeyEx(hLstKey,LISTBOX_KEY,0,KEY_READ,&hLstKey)==ERROR_SUCCESS))
	{	_snprintf(lpIndice,MAX_KEY_SIZE,"%d",indice);
		while(RegOpenKeyEx(hLstKey,lpIndice,0,KEY_READ,&hTmpKey)==ERROR_SUCCESS)
		{	// ListSel
			if(!mHpPak_ReadLongStrRegVal(hTmpKey,LISTSEL_KEY,lpLstSel,MAX_RS_SIZE)) return FALSE;
			this->AddMultiIndex(lpLstSel);
			RegCloseKey(hTmpKey);
			indice++;
			mHpPak_InitStr(lpIndice,MAX_KEY_SIZE);
			_snprintf(lpIndice,MAX_KEY_SIZE,"%d",indice);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CListTask::WriteTaskKey()
{	char lpIndice[MAX_KEY_SIZE]={0};
	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubListKey=NULL,hListKey=NULL;
	int indice=0;
	BOOL bAclRes;
	CmHpTask::WriteCommonKey(TRUE,hSubListKey,hListKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hListKey!=NULL)
	{	// ListBox
		if(RegCreateKeyEx(hListKey,LISTBOX_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hSubListKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		_snprintf(lpIndice,MAX_KEY_SIZE,"%d",indice);
		while(this->GetNextIndex())
		{	// lpIndice
			if(RegCreateKeyEx(hSubListKey,lpIndice,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
							&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
			// ListSel
			if(!mHpPak_WriteLongStrRegVal(hSubListKey,lpIndice,LISTSEL_KEY,this->GetMultiIndex(NO))) return FALSE;
			RegCloseKey(hTmpKey);
			indice++;
			mHpPak_InitStr(lpIndice,MAX_KEY_SIZE);
			_snprintf(lpIndice,MAX_KEY_SIZE,"%d",indice);
		}
		return TRUE;
	}
	return FALSE;
}

void CListTask::AddMultiIndex(char* lpNewSel)
{	CTypeCell *temp=HeadSel;
	CType *NewType=new CStrType(lpNewSel);
	if(HeadSel==NULL) HeadSel=new CTypeCell(NewType);
	else
	{	while(temp->NextType!=NULL) temp=temp->NextType;
		temp->NextType=new CTypeCell(NewType);
	}
}

BOOL CListTask::GetNextIndex()
{	if(CurrentSel==NULL) CurrentSel=HeadSel;
	else CurrentSel=CurrentSel->NextType;
	if(CurrentSel!=NULL) return TRUE;
	return FALSE;
}

char* CListTask::GetMultiIndex(int index)
{	int itemp=index;
	CTypeCell *ctemp=HeadSel;
	if(index==NO)
	{	if(CurrentSel!=NULL) return CurrentSel->Type->GetStr();
		if(HeadSel!=NULL) return HeadSel->Type->GetStr();
	}
	else
	{	while((itemp!=0)&&(ctemp!=NULL))
		{	ctemp=ctemp->NextType;
			itemp--;
		}
		if(ctemp!=NULL) return ctemp->Type->GetStr();
	}
	return NULL;
}

void CListTask::InitMultiIndex()
{	CTypeCell* tmp=HeadSel;
	while(tmp!=NULL)
	{	HeadSel=tmp->NextType;
		delete tmp->Type;
		delete tmp;
		tmp=HeadSel;
	}
	HeadSel=NULL;
	CurrentSel=NULL;
}

void CListTask::InverseMultiIndex()
{	CTypeCell* head=HeadSel;
	CTypeCell* last=HeadSel;
	CTypeCell* blast=HeadSel;
	if((head!=NULL)&&(head->NextType!=NULL))
	{	last=HeadSel->NextType;
		while(last->NextType!=NULL)
		{	blast=last;
			last=last->NextType;
		}
		HeadSel=last;
		if(blast==head) // 2 selections
		{	last->NextType=head;
			blast->NextType=NULL;
		}
		else
		{	while(blast!=last)
			{	last->NextType=blast;
				last=blast;
				blast=head;
				while((blast!=last)&&(blast->NextType!=last)) blast=blast->NextType;
			}
			last->NextType=NULL;
		}
	}
}

//****************************************************************************************************************GENERAL//
// MultiList_ItemIndexToSel
//---------------------------------------------------//
int MultiList_ItemIndexToSel(HWND hWndActive)
{	int iSelItm=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,
                                 LB_FINDSTRING,
					             (WPARAM)NO,
					             (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
	if(iSelItm!=LB_ERR) return iSelItm;
	return NO;
}

//****************************************************************************************************************MHPFILE//
// List_ReadEvent
//---------------------------------------------------//
BOOL List_ReadEvent(HWND hWndActive,int pmHpID,const std::string& strPostData,UINT &iCustIdx)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpSel[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	char chNext=END_RS_TASK;
	int iCur,iCnt,iTmp,iOpt,iCntSel;
	BOOL bNoSel=FALSE,bContinue=TRUE,bError;
	CListTask *ListTask;
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
    if(*lpCtrlName!=0) ListTask=new CListTask(pmHpID,lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName);
    else ListTask=new CListTask(pmHpID,lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL);
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
						if(iTmp!=NO) ListTask->AddMultiIndex(lpSel);
						iCntSel++;
					}
					if(iCntSel==1) return TRUE; // WARNING: There is something wrong here!!!!
				}
				bContinue=FALSE;
			}
			else ListTask->AddMultiIndex(lpSel);
		}
		else return TRUE;
	}
#else
    ////// TODO: Be able to remove lines below (state always in HTML)
	while(bContinue)
	{	GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpSel,MAX_RS_SIZE,bContinue,bError);
		if(bError)
		    return TRUE; // Error
        //ListTask->AddMultiIndex(lpSel);
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
                ListTask->AddMultiIndex(lpState);
            }
        }
    }
    // Increase number of custom task
    iCustIdx++;
#endif
	if(!bNoSel) GetmHpVarStruct()->TaskList->AddCellule(ListTask);
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_WriteEvent
//---------------------------------------------------//
void SingleList_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
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
// MultiList_WriteEvent
//---------------------------------------------------//
void MultiList_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
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
// SingleList_WriteParam
//---------------------------------------------------//
BOOL SingleList_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}			

//---------------------------------------------------//
// MultiList_WriteParam
//---------------------------------------------------//
BOOL MultiList_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),FALSE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}

//*******************************************************************************************************************KEYS//
// List_ReadEventKeys
//---------------------------------------------------//
void List_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpParam* ReadParam;
	ReadTask=new CListTask();
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
// SingleList_AddEvent
//---------------------------------------------------//
void SingleList_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndList,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
    if(*lpCtrlName!=0)
	    Item=new CListTask(3,NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName);
    else
    {   int lpctrl[MAX_PART_TREE]={0};
	    AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndList));
	    Item=new CListTask(3,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL);
    }
	int iSel=(int)ListBox_GetCurSel(hWndList);
	char lpTemp[MAX_RS_SIZE]={0};
	ListBox_GetText(hWndList,iSel,lpTemp);
	Item->AddMultiIndex(lpTemp);
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
// MultiList_AddEvent
//---------------------------------------------------//
void MultiList_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndList,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
    if(*lpCtrlName!=0)
	    Item=new CListTask(9,NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName);
    else
    {   int lpctrl[MAX_PART_TREE]={0};
	    AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndList));
	    Item=new CListTask(9,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL);
    }
	int iSelCount=ListBox_GetSelCount(hWndList);
	if(iSelCount>=MAX_RS_SIZE)
	{	mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_CREATE_MULTILISTE,NULL,0,0,MB_ICONEXCLAMATION);
		return;
	}
	int iSel[MAX_RS_SIZE]={0};
	ListBox_GetSelItems(hWndList,MAX_RS_SIZE,iSel);
	char lpTemp[MAX_RS_SIZE]={0};
	for(int i=0;i<iSelCount;i++)
	{	ListBox_GetText(hWndList,iSel[i],lpTemp);
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
// SingleList_IsClassName
//---------------------------------------------------//
BOOL SingleList_IsClassName(HWND hCtrlWnd,char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_LISTBOX,ClassSize))||(!strncmp(CtrlClass,VB_TASK_LISTBOX,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_LISTBOX,sizeof(DOTNET_TASK_LISTBOX)-1)))
	{	if(ListBox_GetSelCount(hCtrlWnd)==LB_ERR) return TRUE;
	}
	return FALSE;
}

//---------------------------------------------------//
// MultiList_IsClassName
//---------------------------------------------------//
BOOL MultiList_IsClassName(HWND hCtrlWnd,char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_LISTBOX,ClassSize))||(!strncmp(CtrlClass,VB_TASK_LISTBOX,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_LISTBOX,sizeof(DOTNET_TASK_LISTBOX)-1)))
	{	if(ListBox_GetSelCount(hCtrlWnd)!=LB_ERR) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_IsCustom
//---------------------------------------------------//
void SingleList_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//---------------------------------------------------//
// MultiList_IsCustom
//---------------------------------------------------//
void MultiList_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_StrState
//---------------------------------------------------//
void SingleList_StrState(CEventCell* pCurCell,char* lpState,int StateSize)
{	strncpy(lpState,pCurCell->mHpTask->GetMultiIndex(NO),StateSize);}

//---------------------------------------------------//
// MultiList_StrState
//---------------------------------------------------//
void MultiList_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
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
// SingleList_ParamType
//---------------------------------------------------//
int SingleList_ParamType()
{	return 4; // S
}

//---------------------------------------------------//
// MultiList_ParamType
//---------------------------------------------------//
int MultiList_ParamType()
{	return 9; // LS
}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_DefaultBound
//-----------------------------------------------------//
void SingleList_DefaultBound(char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	strncpy(pStrBound,STRING_LIMITE,BoundSize);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
	strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
}

//-----------------------------------------------------//
// MultiList_DefaultBound
//-----------------------------------------------------//
void MultiList_DefaultBound(char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	strncpy(pStrBound,LISTSTR_LIMITE,BoundSize);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
	strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_IsCustomized
//-----------------------------------------------------//
BOOL SingleList_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------//
// MultiList_IsCustomized
//-----------------------------------------------------//
BOOL MultiList_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_IsNoStateMsg
//-----------------------------------------------------//
BOOL SingleList_IsNoStateMsg()
{	return TRUE;}

//-----------------------------------------------------//
// MultiList_IsNoStateMsg
//-----------------------------------------------------//
BOOL MultiList_IsNoStateMsg()
{	return TRUE;}

//-----------------------------------------------------//
// SingleList_GetHtmlTag
//-----------------------------------------------------//
#define LISTBOX_HTML_TAG    "\t<input type=\"text\" name=\"%d-%s\" maxlength=\"100\" value=\"%s\"/>\r\n"

void SingleList_GetHtmlTag(CEventCell* pCurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{
    _snprintf(lpHtmlTag,
              iMaxSize,
              LISTBOX_HTML_TAG,
              iCustIndex,
              LISTSEL_KEY,
              pCurCell->mHpTask->GetMultiIndex(NO));
}

//-----------------------------------------------------//
// MultiList_GetHtmlTag
//-----------------------------------------------------//
void MultiList_GetHtmlTag(CEventCell* pCurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
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
              LISTBOX_HTML_TAG,
              iCustIndex,
              LISTSEL_KEY,
              lpState);
}

//***************************************************************************************************************CFGTOOLS//
// SingleList_ShowCfgTools
//---------------------------------------------------//
void SingleList_ShowCfgTools(HWND hWndActive)
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
// MultiList_ShowCfgTools
//---------------------------------------------------//
void MultiList_ShowCfgTools(HWND hWndActive)
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
	ShowWindow(hWndEditItm,SW_SHOW);
	ShowWindow(hWndMultiItm,SW_SHOW);
	ShowWindow(hWndAddItm,SW_SHOW);
	ShowWindow(hWndRemoveItm,SW_SHOW);
}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_HideCfgTools
//---------------------------------------------------//
void SingleList_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
}				

//---------------------------------------------------//
// MultiList_HideCfgTools
//---------------------------------------------------//
void MultiList_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_REMOVE_MULTILIST),SW_HIDE);
}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_SaveCfg
//---------------------------------------------------//
void SingleList_SaveCfg(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	GetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lpTmp,MAX_RS_SIZE);
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->InitMultiIndex();
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->AddMultiIndex(lpTmp);
}

//---------------------------------------------------//
// MultiList_SaveCfg
//---------------------------------------------------//
void MultiList_SaveCfg(HWND hWndActive)
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
// SingleList_LoadCfg
//---------------------------------------------------//
void SingleList_LoadCfg(HWND hWndActive)
{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMultiIndex(NO));
}

//---------------------------------------------------//
// MultiList_LoadCfg
//---------------------------------------------------//
void MultiList_LoadCfg(HWND hWndActive)
{	HWND hWndMultiItm=GetDlgItem(hWndActive,IDC_LISTBOX_MULTILIST);
	EnableWindow(GetDlgItem(hWndActive,IDC_ADD_MULTILIST),FALSE);
	while(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetNextIndex())
	{	SendMessage(hWndMultiItm,LB_ADDSTRING,0,(LPARAM)GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMultiIndex(NO));
	}
}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_GetInfosCfg
//---------------------------------------------------//
void SingleList_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_LIST,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_LIST,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_LIST,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_LIST,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_SINGLELIST)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// MultiList_GetInfosCfg
//---------------------------------------------------//
void MultiList_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_LIST,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_LIST,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_LIST,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_LIST,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_MULTILIST)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//-----------------------------------------------------------------------------------------------------------------------//
// SingleList_ProcCfg
//---------------------------------------------------//
BOOL SingleList_ProcCfg(UINT Message,WPARAM wParam,HWND hWndChng)
{	if(Message==WM_COMMAND)
	{	if(LOWORD(wParam)==IDC_EDIT_STATE)
		{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
			return TRUE;
		}
	}
	return FALSE;
}

//---------------------------------------------------//
// MultiList_ProcCfg
//---------------------------------------------------//
BOOL MultiList_ProcCfg(HWND hWndActive,UINT Message,WPARAM wParam,HWND hWndChng)
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
// List_CaptureMsg
//---------------------------------------------------//
void List_CaptureMsg(HWND hWndActive,char* lpCtrlName,HWND hWndCtl,WPARAM wpParam,HWND hToolWnd,int iCurOp,CEventListe* EventObj,
                     char* lpNameApp,UINT &pIdLast,int pCurwnd,int pCntwnd,int pmHpCtrlID,BOOL &pActionUser,
                     int &pResCapt)
{	int iWriteRes=0;
	if(HIWORD(wpParam)==LBN_SELCHANGE)
	{	WriteImmediatTask(hWndActive,lpCtrlName,hWndCtl,hToolWnd,NULL,iCurOp,pmHpCtrlID,(UINT)((int)LOWORD(wpParam)),
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
// SingleList_mHpProcess
//---------------------------------------------------//
void SingleList_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2:
		{	if(GetmHpVarStruct()->imHpMode==1) 
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(4);
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
				break;
			}
			if(GetmHpVarStruct()->EcartDepl!=1) GetmHpVarStruct()->EcartDepl/=2;
			GetmHpVarStruct()->bBlockUpdateFrame=FALSE;
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_ASCENS,20,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			if(!GetmHpVarStruct()->bBlockUpdateFrame) GetAscensNextPos(hWndActive,FALSE,FALSE);
			else GetmHpVarStruct()->bBlockUpdateFrame=FALSE;
			GetmHpVarStruct()->bNewDepl=TRUE;
			GetmHpVarStruct()->CursorType=0;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 5:
		{	if(SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETCURSEL,0,0)!=SendMessage(GetmHpVarStruct()->hWndCurCtrl,
                                                                                         LB_FINDSTRING,
                                                                                         (WPARAM)NO,
                                                                                         (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO)))
			{	GetmHpVarStruct()->CursorType=1;}
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6:
		{	if(GetmHpVarStruct()->CursorType==1)
			{	EndObjects(hWndActive);
				if(GetmHpVarStruct()->imHpMode!=2)
				{	SendMessage(GetmHpVarStruct()->hWndCurCtrl,
                                LB_SELECTSTRING,
                                (WPARAM)NO,
                                (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
                }
				ForceMessage(hWndActive,LBN_SELCHANGE);
				if(!DoInit(hWndActive))
				{	StopmHelp(hWndActive,TRUE,TRUE);
					return;
				}
			}
			else GetmHpVarStruct()->iSkipSnd=2;
			if(GetmHpVarStruct()->imHpMode==1) GetmHpVarStruct()->CourantTask->SetmHpStep(7);
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7:
		{	int isel=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,
									  LB_FINDSTRING,
									  (WPARAM)NO,
									  (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
			if((isel==LB_ERR)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10))
			{	ViewCtrlNotReadyMsg(hWndActive,4);
			}
			else
			{	if(PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS)))
				{	SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);}
				else SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 8:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			GetmHpVarStruct()->EcartDepl=GetmHpVarStruct()->imHpMouseSpeed;
			if(GetmHpVarStruct()->TaskList->NextCellule())
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

//-----------------------------------------------------//
// SingleList_StartProc
//-----------------------------------------------------//
BOOL SingleList_StartProc(HWND hWndActive)
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
	int iSel=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,
                              LB_FINDSTRING,
                              (WPARAM)NO,
                              (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
	if(iSel==LB_ERR)
	{	RECT rect;
        GetmHpVarStruct()->CourantTask->SetmHpStep(6);
		GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&rect);
		GetmHpVarStruct()->XnextPos=(int)(((rect.right-rect.left)/2)+rect.left);
		GetmHpVarStruct()->YnextPos=(int)(((rect.bottom-rect.top)/2)+rect.top);
		GetmHpVarStruct()->iSkipSnd=2;
		return TRUE;
	}
	BOOL bUse=FALSE;
	ComboListBoxStatusPos(hWndActive,bUse,FALSE,iSel);
	if((!GetmHpVarStruct()->AscensSens)||((GetmHpVarStruct()->AscensSens)&&(!bUse)))
	{	GetmHpVarStruct()->CourantTask->SetmHpStep(3); 
		GetmHpVarStruct()->bBlockUpdateFrame=TRUE;
		GetmHpVarStruct()->iSkipSnd=1;
	}
	else GetAscensNextPos(hWndActive,TRUE,FALSE);
	return TRUE;
}

//-----------------------------------------------------//
// MultiList_mHpProcess
//-----------------------------------------------------//
void MultiList_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2:
		{	if(GetmHpVarStruct()->imHpMode==1) 
			{	CourantIndex=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
				if(CourantIndex==NO) GetmHpVarStruct()->CourantTask->SetmHpStep(7);
				else
				{	GetmHpVarStruct()->CourantTask->SetmHpStep(1);
					ChangeCourantItemState(hWndActive);
				}
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
				break;
			}	
			if(GetmHpVarStruct()->EcartDepl!=1) GetmHpVarStruct()->EcartDepl/=2;
			SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_DELAI,200,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 5:
		{	if(AscensInc!=0)
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(2);
                SetTimer(hWndActive,ID_TIMER_LISTE,1,(TIMERPROC)mHpTimerProc);
			}
			else
			{	EndObjects(hWndActive);
				ChangeCourantItemState(hWndActive);
				DoInit(hWndActive);
				GetmHpVarStruct()->bRefreshDDraw=TRUE;
				SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 6:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=0;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7:
		{	if((!bAllFnd)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10)) ViewCtrlNotReadyMsg(hWndActive,4);
			else if(!NextMultiListStep(hWndActive))
			{	SetTimer(hWndActive,ID_TIMER_DELAI,100,(TIMERPROC)mHpTimerProc);}
			break;
		}
		case 8:
		{	GetmHpVarStruct()->EcartDepl=GetmHpVarStruct()->imHpMouseSpeed;
			if(GetmHpVarStruct()->TaskList->NextCellule())
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

//-----------------------------------------------------//
// MultiList_StartProc
//-----------------------------------------------------//
BOOL MultiList_StartProc(HWND hWndActive)
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
	RECT rect;
    GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&rect);
	GetmHpVarStruct()->XnextPos=(int)(rect.right-8);
	GetmHpVarStruct()->YnextPos=(int)(rect.top+6);
	CreateItemListe(hWndActive,bAllFnd);
	if((!bAllFnd)&&(GetmHpVarStruct()->CourantTask->GetmHpOption()>=10))
	{	GetmHpVarStruct()->CourantTask->SetmHpStep(6);
		GetmHpVarStruct()->XnextPos=(int)(((rect.right-rect.left)/2)+rect.left);
		GetmHpVarStruct()->YnextPos=(int)(((rect.bottom-rect.top)/2)+rect.top);
	}
	else if(!NoChangeMultiList(hWndActive))
	{	if(GetmHpVarStruct()->imHpMode==1) return TRUE;
		CourantIndex=GetmHpVarStruct()->SelItemListe->ItemIndexSuivant();
		GetItemsInVisu(hWndActive,FALSE);
		if(!GetmHpVarStruct()->AscensSens) GetmHpVarStruct()->YnextPos+=(CourantIndex*ITEM_ECART);
		else if(IsIndexInVisu(hWndActive))
		{	GetmHpVarStruct()->XnextPos-=ASCENS_ECART;
			SetYListPos(hWndActive);
		}
		else if(GetmHpVarStruct()->AscensSens==2)
		{	GetmHpVarStruct()->YnextPos=(int)(rect.bottom-ITEM_ECART);}
	}
	else
	{	GetmHpVarStruct()->CourantTask->SetmHpStep(5);
		GetmHpVarStruct()->iSkipSnd=1;
	}
	return TRUE;
}

//**************************************************************************************************************TIMERPROC//
// List_TimerProc
//---------------------------------------------------//
void List_TimerProc(HWND hWndActive)
{	int Count=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETCOUNT,0,0);
	int Topindex=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETTOPINDEX,0,0);
    if(!RestoreBackBuffer(hWndActive))
    {   StopmHelp(hWndActive,TRUE,TRUE);
        return;
    }
    UpdateBackBuffer(TRUE);
	if(GetmHpVarStruct()->AscensSens==1) // Monte
	{	Topindex--;
		SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_SETTOPINDEX,Topindex,0);
	}
	else // Descend
	{	Topindex++;
		SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_SETTOPINDEX,Topindex,0);
	}
	RedrawWindow(GetmHpVarStruct()->hWndCurCtrl,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
    AscensInc--;
	if((!AscensInc)||((Topindex+GetmHpVarStruct()->ItemsVisu)>=Count))
	{	GetmHpVarStruct()->CursorType=0;
		GetmHpVarStruct()->bNewDepl=TRUE;
		GetmHpVarStruct()->XnextPos-=ASCENS_ECART;
		if(GetmHpVarStruct()->AscensSens==2)
		{	RECT itemRect;
			GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&itemRect);
			GetmHpVarStruct()->YnextPos=(int)(itemRect.top+6);
			if(AscensInc!=0)
			{	GetmHpVarStruct()->YnextPos+=(AscensInc*ITEM_ECART);
				AscensInc=0;
			}
			else SetYListPos(hWndActive);
		}
		PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
		SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
	}
    else
    {   if(!UpdateFrame(hWndActive))
            StopmHelp(hWndActive,TRUE,TRUE);
        else SetTimer(hWndActive,ID_TIMER_LISTE,10,(TIMERPROC)mHpTimerProc);
    }
}

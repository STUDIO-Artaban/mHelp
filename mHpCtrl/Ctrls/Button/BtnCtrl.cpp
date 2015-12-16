// BtnCtrl.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Button\BtnCtrl.h>

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
#define		CPP_TASK_BUTTON				"Button"                    // C/C++ control
#define		VB_TASK_BUTTON				"ThunderRT6CommandButton"   // VB button
#define		VB_TASK_CHECKBOX			"ThunderRT6CheckBox"        // VB checkbox
#define		VB_TASK_RADIOBTN			"ThunderRT6OptionButton"    // VB radiobox
#define		PASCAL_TASK_BUTTON			"TButton"                   // Delphi control

//-----------------------------------------------------//
// KeyName
#define		BUTTON_KEY					"Button"

//-----------------------------------------------------//
// ValueKeys
#define		BTNSTATE_KEY				"BtnState"
//-----------------------------------------------------//
#define		RADIOROW_KEY				"RadioRow"
#define		RADNAME_KEY					"RadioName"

//-----------------------------------------------------//
// Définitions

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Déclarations
int GetBtnStyleID(HWND);
int GetBtnState(HWND);

//-----------------------------------------------------------------------------------------------------------------------//
// Variables
BOOL bItemLinked=FALSE;
HWND hWndRadioList=NULL;
int iBtnStyle=0;

//-----------------------------------------------------//
// GetBtnStyleID
//-----------------------------------------------------//
int GetBtnStyleID(HWND phWndCtrl)
{	int iResult=0;
    if(IsWndStyle(phWndCtrl,BS_3STATE)==BS_3STATE) iResult=3;
	else if(IsWndStyle(phWndCtrl,BS_CHECKBOX)==BS_CHECKBOX) iResult=1;
	else if(IsWndStyle(phWndCtrl,BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX) iResult=1;
	else if(IsWndStyle(phWndCtrl,BS_RADIOBUTTON)==BS_RADIOBUTTON) iResult=2;
	else if(IsWndStyle(phWndCtrl,BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON) iResult=2;
    return iResult;
}

//-----------------------------------------------------//
// GetBtnState
//-----------------------------------------------------//
int GetBtnState(HWND hWndActive)
{	int iState;
	/*if((*GetmHpVarStruct()->CourantTask->GetmHpCtrlName()!=0)&&(mHpPak_IsDotNetHwnd(hWndActive)))
	{	int iStyle;
		if(mHpPak_GetDotNetCtrlHandle(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlName(),iStyle,iState))
			return iState;
	}*/
    iState=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,BM_GETCHECK,0,0);
    switch(iState)
    {   case BST_UNCHECKED:     iState=0; break;
        case BST_CHECKED:       iState=1; break;
        case BST_INDETERMINATE: iState=2; break;
    }
    return iState;
}

//****************************************************************************************************************OBJECTS//
// class CButtonTask
CButtonTask::CButtonTask(int TaskID,char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrlID,char* lpCtrlName,int iState)
 			:CmHpTask(TaskID,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrlID,lpCtrlName)
{	iBtnState=iState;}

CButtonTask::CButtonTask()
 			:CmHpTask()
{}

BOOL CButtonTask::ReadTaskKey(int index)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	HKEY hSubBtnTaskKey=NULL,hBtnTaskKey=NULL;
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;
	CmHpTask::ReadCommonKey(index,TRUE,hSubBtnTaskKey,hBtnTaskKey);
	if((hBtnTaskKey!=NULL)&&(RegOpenKeyEx(hBtnTaskKey,BUTTON_KEY,0,KEY_READ,&hBtnTaskKey)==ERROR_SUCCESS))
	{	// iBtnState
		if(RegQueryValueEx(hBtnTaskKey,BTNSTATE_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iBtnState,FALSE);		
		return TRUE;
	}
	return FALSE;
}

BOOL CButtonTask::WriteTaskKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubBtnKey=NULL,hBtnKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size;
	BOOL bAclRes;
	CmHpTask::WriteCommonKey(TRUE,hSubBtnKey,hBtnKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hBtnKey!=NULL)
	{	// Button
		if(RegCreateKeyEx(hBtnKey,BUTTON_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// iBtnState
		if(RegCreateKeyEx(hBtnKey,BUTTON_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iBtnState,FALSE);
		RegSetValueEx(hTmpKey,BTNSTATE_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

int CButtonTask::GetBtnState()
{	return iBtnState;}

void CButtonTask::SetBtnState(int iNewState)
{	iBtnState=iNewState;}

// class CRadioWrite //////////////////////////////////////////////////////////////////////////////////////////////////////
CRadioWrite::CRadioWrite()
			:CmHpWrite()
{	iLinked=NO;
	iRadioRow=1;
	mHpPak_InitStr(lpRadName,MAX_RS_SIZE);
}

CRadioWrite::CRadioWrite(char* lpRadText)
			:CmHpWrite()
{	iLinked=NO;
	iRadioRow=1;
	mHpPak_InitStr(lpRadName,MAX_RS_SIZE);
	if(lpRadText!=NULL) strncpy(lpRadName,lpRadText,MAX_RS_SIZE);
}

BOOL CRadioWrite::ReadEventKey(int index)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;	
	HKEY hSubRadioKey=NULL,hRadioKey=NULL;
	ReadmHpWriteKey(index,hSubRadioKey,hRadioKey);
	if(hRadioKey!=NULL)
	{	// iLinked
		if(RegQueryValueEx(hRadioKey,LINKED_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iLinked,FALSE);		
		if(!iLinked) iLinked=NO;
		Size=MAX_KEY_SIZE;Type=REG_DWORD;
		// iRadioRow
		if(RegQueryValueEx(hRadioKey,RADIOROW_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iRadioRow,FALSE);		
		// lpRadName
		if(!mHpPak_ReadLongStrRegVal(hRadioKey,RADNAME_KEY,lpRadName,MAX_RS_SIZE))
		{	mHpPak_InitStr(lpRadName,MAX_RS_SIZE);}
		return TRUE;
	}
	return FALSE;
}

BOOL CRadioWrite::WriteEventKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubRadioWrtKey=NULL,hRadioWrtKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size;	
	BOOL bAclRes;
	CmHpWrite::WritemHpWriteKey(hSubRadioWrtKey,hRadioWrtKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hRadioWrtKey!=NULL)
	{	// iLinked
		if(RegCreateKeyEx(hSubRadioWrtKey,MHPWRITE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		if(iLinked!=NO) size=mHpPak_FillDataFromNumber(bTab,iLinked,FALSE);
		else size=mHpPak_FillDataFromNumber(bTab,0,FALSE);
		RegSetValueEx(hTmpKey,LINKED_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		// iRadioRow
		if(RegCreateKeyEx(hSubRadioWrtKey,MHPWRITE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iRadioRow,FALSE);
		RegSetValueEx(hTmpKey,RADIOROW_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		// lpRadName
		if((strncmp(lpRadName,NULL_STRING,MAX_RS_SIZE)!=0)&&(!mHpPak_WriteLongStrRegVal(hSubRadioWrtKey,MHPWRITE_KEY,RADNAME_KEY,lpRadName)))
		{	return FALSE;}
		return TRUE;
	}
	return FALSE;
}

int CRadioWrite::GetLinked()
{	return iLinked;}

void CRadioWrite::SetLinked(int iNewLink)
{	iLinked=iNewLink;}
		
int CRadioWrite::GetRadioRow()
{	return iRadioRow;}

void CRadioWrite::SetRadioRow(int iNewRow)
{	iRadioRow=iNewRow;}

char* CRadioWrite::GetRadioName()
{	return lpRadName;}

// class CCheckWrite //////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckWrite::CCheckWrite(int iStyle)
			:CStyleWrite(iStyle)
{}

CCheckWrite::CCheckWrite()
			:CStyleWrite()
{}

//****************************************************************************************************************MHPFILE//
// Button_ReadEvent
//---------------------------------------------------//
BOOL Button_ReadEvent(HWND hWndActive)
{	int lpctrl[MAX_PART_TREE]={0};
	int iCur,iCnt,iTmp,iOpt;
	char lpTmp[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	char chNext=END_RS_TASK;
	CButtonTask *BtnTask;
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
	if(*lpCtrlName!=0) BtnTask=new CButtonTask(2,lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName,0);
    else BtnTask=new CButtonTask(2,lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL,0);
	GetmHpVarStruct()->TaskList->AddCellule(BtnTask);
	return FALSE; // OK
}

//---------------------------------------------------//
// CheckRadio_ReadEvent
//---------------------------------------------------//
BOOL CheckRadio_ReadEvent(HWND hWndActive,int pmHpID)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	int iCur,iCnt,iState=0,iTmp,iOpt;
	char chNext=END_RS_TASK;
	BOOL bSel=TRUE;
	CButtonTask *BtnTask;
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
	if((pmHpID==7)||((pmHpID==8)&&(chNext==TREE_PARAM_GROUP)))
	{	// iBtnState // ** Param type: Booléan **
		mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
		if(iState==NO)
		{	if((lpTmp[0]==END_LIST)||(lpTmp[0]!=RS_PARAM)) return TRUE;
			iTmp=mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,1);
			if(!DidItSelected(iTmp)) bSel=FALSE;
			else
			{	iState=GetIntBoolEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0);
				if(iState==NO) return TRUE;
				if(pmHpID==8)
				{	if(GetmHpVarStruct()->iLnkSel!=iTmp)
					{	GetmHpVarStruct()->iLnkSel=iTmp;
						GetmHpVarStruct()->iLnkLag=NO;
					}
					if(GetmHpVarStruct()->iLnkLag!=NO) GetmHpVarStruct()->iLnkLag++;
					else GetmHpVarStruct()->iLnkLag=1;
					if(GetmHpVarStruct()->iLnkLag!=iState) bSel=FALSE;
				}				
			}
		}	
	}
	if(bSel)
	{	if(*lpCtrlName!=0) BtnTask=new CButtonTask(pmHpID,lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName,iState);
        else BtnTask=new CButtonTask(pmHpID,lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL,iState);
		GetmHpVarStruct()->TaskList->AddCellule(BtnTask);
	}
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_WriteEvent
//---------------------------------------------------//
void Button_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
                       char* lpCurExe,BOOL &bRes)
{	if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index)) bRes=FALSE;
}

//---------------------------------------------------//
// Check_WriteEvent
//---------------------------------------------------//
void Check_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
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
// Radio_WriteEvent
//---------------------------------------------------//
void Radio_WriteEvent(HWND hWndActive,int &Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
                      BOOL bFolderParam,char* lpCurExe,int &IndexParam,BOOL &bLnkFirst,BOOL &bRes)
{	static int iLnkLag=0;
	static int iLnkCons=0;
	static int iLnkIndex=0;
	static int iRowSel=0;
	static BOOL bNoCustom=FALSE;
	if(bLnkFirst)
	{	iLnkLag=0;
		iLnkCons=0;
		iLnkIndex=0;
		iRowSel=0;
		bNoCustom=FALSE;
		bLnkFirst=FALSE;
	}
	if(iLnkLag!=0)
	{	if(!bNoCustom)
		{	if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))|| // Common
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
			   (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
			{	bRes=FALSE;
				return;
			}
			iLnkLag--;
			if(!iLnkLag)
			{	IndexParam++;
				EvntObj->GetmHpWrite(iLnkIndex,FALSE)->SetLinked(iLnkCons);
				Index=iLnkIndex;
			}
		}
		else
		{	if(iRowSel==((Index-iLnkIndex)+1))
			{	if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))
				{	bRes=FALSE;
					return;
				}
				EvntObj->GetmHpWrite(iLnkIndex,FALSE)->SetLinked(iLnkCons);
				Index=iLnkIndex;
				iLnkLag=0;
				bNoCustom=FALSE;
			}
			if(iLnkLag!=0) iLnkLag--;
		}
	}
	else if((bFolderParam)&&(EvntObj->GetmHpWrite(Index,FALSE)->GetLinked()!=NO))
	{	iLnkCons=EvntObj->GetmHpWrite(Index,FALSE)->GetLinked();
		iLnkIndex=Index;
		if(EvntObj->GetCustomVisible(Index))
		{	iLnkLag=EvntObj->GetmHpWrite(Index,FALSE)->GetLinked()-1;
			EvntObj->GetmHpWrite(Index,FALSE)->SetLinked(NO);
			if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))|| // Common
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
			   (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
			{	bRes=FALSE;}
		}
		else
		{	iRowSel=EvntObj->GetmHpWrite(Index,FALSE)->GetRadioRow();
			if(iRowSel==1)
			{	if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index)) 
				{	bRes=FALSE;}
			}
			else
			{	bNoCustom=TRUE;
				iLnkLag=EvntObj->GetmHpWrite(Index,FALSE)->GetLinked()-1;
				EvntObj->GetmHpWrite(Index,FALSE)->SetLinked(NO);
			}
		}
	}
	else if((EvntObj->GetmHpWrite(Index,FALSE)->GetLinked()!=NO)&&(!bFolderParam))
	{	iLnkCons=EvntObj->GetmHpWrite(Index,FALSE)->GetLinked();
		iLnkIndex=Index;
		iRowSel=EvntObj->GetmHpWrite(Index,FALSE)->GetRadioRow();
		if(iRowSel==1)
		{	if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index)) 
			{	bRes=FALSE;}
		}
		else
		{	bNoCustom=TRUE;
			iLnkLag=EvntObj->GetmHpWrite(Index,FALSE)->GetLinked()-1;
			EvntObj->GetmHpWrite(Index,FALSE)->SetLinked(NO);
		}
	}
	else if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index)) bRes=FALSE;
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_WriteParam
//-----------------------------------------------------//
BOOL Button_WriteParam()
{	return TRUE;}

//---------------------------------------------------//
// Check_WriteParam
//---------------------------------------------------//
BOOL Check_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}			

//---------------------------------------------------//
// Radio_WriteParam
//---------------------------------------------------//
BOOL Radio_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if(EvntObj->GetmHpWrite(Index,FALSE)->GetLinked()!=NO)
	{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),FALSE))|| // '§'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
		{	return FALSE;}
	}
	return TRUE;
}

//*******************************************************************************************************************KEYS//
// Button_ReadEventKeys
//---------------------------------------------------//
void Button_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	ReadTask=new CButtonTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL) EvntObj->AddEvent(ReadTask,NULL,NULL);
	else TaskList->AddCellule(ReadTask);
}

//---------------------------------------------------//
// Check_ReadEventKeys
//---------------------------------------------------//
void Check_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpWrite* ReadWrite;
	CmHpParam* ReadParam;
	ReadTask=new CButtonTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL)
	{	ReadWrite=new CCheckWrite();
		ReadParam=new CmHpParam();
		ReadWrite->ReadEventKey(Index);
		if(!ReadParam->ReadParamKey(Index))
		{	delete(ReadParam);
			ReadParam=NULL;
		}
		EvntObj->AddEvent(ReadTask,ReadWrite,ReadParam);
	}
	else TaskList->AddCellule(ReadTask);
}

//---------------------------------------------------//
// Radio_ReadEventKeys
//---------------------------------------------------//
void Radio_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpWrite* ReadWrite;
	CmHpParam* ReadParam;
	ReadTask=new CButtonTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL)
	{	ReadWrite=new CRadioWrite();
		ReadParam=new CmHpParam();
		ReadWrite->ReadEventKey(Index);
		if(!ReadParam->ReadParamKey(Index))
		{	delete(ReadParam);
			ReadParam=NULL;
		}
		EvntObj->AddEvent(ReadTask,ReadWrite,ReadParam);
	}
	else TaskList->AddCellule(ReadTask);
}
			
//********************************************************************************************************************ADD//
// Button_AddEvent
//---------------------------------------------------//
void Button_AddEvent(HWND hWndActive,char* lpCtrlName,UINT iTaskID,int iCurWnd,int iCntWnd)
{	CmHpTask* Item;
    if(*lpCtrlName!=0)
	    Item=new CButtonTask(2,NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName,0);
    else
    {   int lpctrl[MAX_PART_TREE]={0};
	    AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,iTaskID);
	    Item=new CButtonTask(2,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,0);
    }
	Item->WriteTaskKey();
	GetmHpVarStruct()->EventList->AddEvent(Item,NULL,NULL);
}

//---------------------------------------------------//
// Check_AddEvent
//---------------------------------------------------//
void Check_AddEvent(HWND hWndActive,char* lpCtrlName,UINT iTaskID,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
	CmHpWrite* wItem;
    /*if(*lpCtrlName!=0)
    {   int iStyle=0,iState=0;
		if(mHpPak_IsDotNetHwnd(hWndActive))
			mHpPak_GetDotNetCtrlHandle(hWndActive,lpCtrlName,iStyle,iState);
        Item=new CButtonTask(7,NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName,iState);
        wItem=new CCheckWrite((iStyle!=3)? 0:1);
    }
    else*/
    {   int iChkStyle;
        int lpctrl[MAX_PART_TREE]={0};
	    AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,iTaskID);
	    if((IsWndStyle(GetDlgItem(hWndActive,iTaskID),BS_3STATE)==BS_3STATE)||
	       (IsWndStyle(GetDlgItem(hWndActive,iTaskID),BS_AUTO3STATE)==BS_AUTO3STATE))
	    {	iChkStyle=1;}
	    else iChkStyle=0;
	    if(!iChkStyle)
	    {	if(Button_GetCheck(GetDlgItem(hWndActive,iTaskID))!=BST_CHECKED)
		    {	Item=new CButtonTask(7,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,0);}
		    else
		    {	Item=new CButtonTask(7,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,1);}
	    }
	    else
	    {	if((Button_GetCheck(GetDlgItem(hWndActive,iTaskID))!=BST_CHECKED)&&
		       (Button_GetCheck(GetDlgItem(hWndActive,iTaskID))!=BST_UNCHECKED))
		    {	Item=new CButtonTask(7,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,2);}
		    else if(Button_GetCheck(GetDlgItem(hWndActive,iTaskID))!=BST_CHECKED)
		    {	Item=new CButtonTask(7,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,0);}
		    else Item=new CButtonTask(7,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,1);
	    }
	    wItem=new CCheckWrite(iChkStyle);
    }
	if(bWrite)
	{	Item->WriteTaskKey();
		wItem->WriteEventKey();
		GetmHpVarStruct()->EventList->AddEvent(Item,wItem,NULL);
	}
	else
	{	RemoveLastKeyPos();
		Item->WriteTaskKey();
		wItem->WriteEventKey();
		GetmHpVarStruct()->EventList->ReplaceEvent(Item,wItem);
	}
}

//---------------------------------------------------//
// Radio_AddEvent
//---------------------------------------------------//
void Radio_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndRadio,int iCurWnd,int iCntWnd,BOOL bWrite)
{	char lpCaption[MAX_RS_SIZE]={0};
	CmHpTask* Item;
	CmHpWrite* wItem;
    if(*lpCtrlName!=0)
	    Item=new CButtonTask(8,NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName,0);
    else
    {   int lpctrl[MAX_PART_TREE]={0};
	    AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndRadio));
	    Item=new CButtonTask(8,NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,0);
    }
    GetWindowText(hWndRadio,lpCaption,MAX_RS_SIZE);
	wItem=new CRadioWrite(lpCaption);
	if(bWrite)
	{	Item->WriteTaskKey();
		wItem->WriteEventKey();
		GetmHpVarStruct()->EventList->AddEvent(Item,wItem,NULL);
	}
	else
	{	RemoveLastKeyPos();
		Item->WriteTaskKey();
		wItem->WriteEventKey();
		GetmHpVarStruct()->EventList->ReplaceEvent(Item,wItem);
	}
}

//*************************************************************************************************************PROPERTIES//
// Button_IsClassName
//---------------------------------------------------//
BOOL Button_IsClassName(HWND hCtrlWnd,char* CtrlClass,int ClassSize)
{	if(!strncmp(CtrlClass,VB_TASK_BUTTON,ClassSize)) return TRUE;
	if((!strncmp(CtrlClass,CPP_TASK_BUTTON,ClassSize))||
       (!strncmp(CtrlClass,PASCAL_TASK_BUTTON,ClassSize)))
	{	if(!GetBtnStyleID(hCtrlWnd)) return TRUE;
	}
    if(!strncmp(CtrlClass,DOTNET_TASK_BUTTON,sizeof(DOTNET_TASK_BUTTON)-1))
        return TRUE;
	return FALSE;
}

//---------------------------------------------------//
// Check_IsClassName
//---------------------------------------------------//
BOOL Check_IsClassName(HWND hCtrlWnd,char* CtrlClass,int ClassSize)
{	if(!strncmp(CtrlClass,VB_TASK_CHECKBOX,ClassSize)) return TRUE;
	if((!strncmp(CtrlClass,CPP_TASK_BUTTON,ClassSize))||
       (!strncmp(CtrlClass,PASCAL_TASK_BUTTON,ClassSize)))
	{	if((GetBtnStyleID(hCtrlWnd)==1)||(GetBtnStyleID(hCtrlWnd)==3)) return TRUE;
	}
    if(!strncmp(CtrlClass,DOTNET_TASK_BUTTON,sizeof(DOTNET_TASK_BUTTON)-1))
        return TRUE;
	return FALSE;
}	

//---------------------------------------------------//
// Radio_IsClassName
//---------------------------------------------------//
BOOL Radio_IsClassName(HWND hCtrlWnd,char* CtrlClass,int ClassSize)
{	if(!strncmp(CtrlClass,VB_TASK_RADIOBTN,ClassSize)) return TRUE;
	if((!strncmp(CtrlClass,CPP_TASK_BUTTON,ClassSize))||
       (!strncmp(CtrlClass,PASCAL_TASK_BUTTON,ClassSize)))
	{	if(GetBtnStyleID(hCtrlWnd)==2) return TRUE;
	}
    if(!strncmp(CtrlClass,DOTNET_TASK_BUTTON,sizeof(DOTNET_TASK_BUTTON)-1))
        return TRUE;
	return FALSE;
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_IsCustom
//---------------------------------------------------//
void Button_IsCustom(int &LastID)
{	LastID=NO;}

//---------------------------------------------------//
// Check_IsCustom
//---------------------------------------------------//
void Check_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//---------------------------------------------------//
// Radio_IsCustom
//---------------------------------------------------//
void Radio_IsCustom(CEventCell* EvntCell,BOOL &bcustom)
{	if(EvntCell->mHpWrite->GetLinked()!=NO) bcustom=TRUE;
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_StrState
//---------------------------------------------------//
void Button_StrState(char* lpState,int StateSize)
{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_NO_STATE,lpState,StateSize);}

//---------------------------------------------------//
// Check_StrState
//---------------------------------------------------//
void Check_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
{	if(pWriteOp) _snprintf(lpState,StateSize,"%d",pCurCell->mHpTask->GetBtnState());
	else
	{	switch(pCurCell->mHpTask->GetBtnState())
		{	case 0:
			{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_INACTIVE,lpState,StateSize);
				break;
			}
			case 1:
			{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_ACTIVE,lpState,StateSize);
				break;
			}
			case 2:
			{	if(!pCurCell->mHpWrite->GetCtrlStyle())
				{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHANGED_STATE,lpState,StateSize);}
				else
				{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_INDETERMINATE_STATE,lpState,StateSize);}
				break;
			}
		}
	}
}

//---------------------------------------------------//
// Radio_StrState
//---------------------------------------------------//
void Radio_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
{	if(pWriteOp) _snprintf(lpState,StateSize,"%d",pCurCell->mHpWrite->GetRadioRow());
	else
	{	if(pCurCell->mHpWrite->GetLinked()!=NO)
		{	_snprintf(lpState,StateSize,"%d°",pCurCell->mHpWrite->GetRadioRow());
			strncat(lpState," radiobox",9);
		}
		else strncpy(lpState,pCurCell->mHpWrite->GetRadioName(),MAX_RS_SIZE);
	}
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_ParamType
//-----------------------------------------------------//
int Button_ParamType()
{	return NO;}

//---------------------------------------------------//
// Check_ParamType
//---------------------------------------------------//
int Check_ParamType()
{	return 3; // B
}

//---------------------------------------------------//
// Radio_ParamType
//---------------------------------------------------//
int Radio_ParamType()
{	return 0; // I
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_DefaultBound
//-----------------------------------------------------//
void Button_DefaultBound()
{	NULL;}

//-----------------------------------------------------//
// Check_DefaultBound
//-----------------------------------------------------//
void Check_DefaultBound(char* pStrBound,int BoundSize)
{	strncpy(pStrBound,CHECKBOX_LIMITE,BoundSize);}

//-----------------------------------------------------//
// Radio_DefaultBound
//-----------------------------------------------------//
void Radio_DefaultBound(CEventCell* pCurEvent,char* pStrBound,int BoundSize)
{	if(pCurEvent->mHpWrite->GetLinked()!=NO)
	{	_snprintf(pStrBound,BoundSize,"[1;%d]",pCurEvent->mHpWrite->GetLinked());}
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_IsCustomized
//-----------------------------------------------------//
BOOL Button_IsCustomized()
{	return FALSE;}

//-----------------------------------------------------//
// Check_IsCustomized
//-----------------------------------------------------//
BOOL Check_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------//
// Radio_IsCustomized
//-----------------------------------------------------//
BOOL Radio_IsCustomized(CEventCell* pCurEvnt,int Indice,BOOL pCustom)
{	if(pCurEvnt->mHpWrite->GetLinked()==NO)
	{	if(((GetmHpVarStruct()->EventList->GetCellFromPos((Indice+1),FALSE)!=NULL)&&
			(GetmHpVarStruct()->EventList->GetCellFromPos((Indice+1),FALSE)->mHpTask->GetmHpID()==8)&&
			(GetmHpVarStruct()->EventList->GetCellFromPos((Indice+1),FALSE)->mHpWrite->GetLinked()==NO))||
		   ((GetmHpVarStruct()->EventList->GetCellFromPos((Indice-1),FALSE)!=NULL)&&
			(GetmHpVarStruct()->EventList->GetCellFromPos((Indice-1),FALSE)->mHpTask->GetmHpID()==8)&&
			(GetmHpVarStruct()->EventList->GetCellFromPos((Indice-1),FALSE)->mHpWrite->GetLinked()==NO)))
		{	if(!pCustom) return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_IsNoStateMsg
//-----------------------------------------------------//
BOOL Button_IsNoStateMsg()
{	return FALSE;}

//-----------------------------------------------------//
// Check_IsNoStateMsg
//-----------------------------------------------------//
BOOL Check_IsNoStateMsg()
{	return FALSE;}

//-----------------------------------------------------//
// Radio_IsNoStateMsg
//-----------------------------------------------------//
BOOL Radio_IsNoStateMsg()
{	return FALSE;}

//-----------------------------------------------------//
// Button_GetHtmlTag
//-----------------------------------------------------//
void Button_GetHtmlTag(CEventCell*,char*,UINT,int)
{}

//-----------------------------------------------------//
// Check_GetHtmlTag
//-----------------------------------------------------//
void Check_GetHtmlTag(CEventCell*,char*,UINT,int)
{}

//-----------------------------------------------------//
// Radio_GetHtmlTag
//-----------------------------------------------------//
void Radio_GetHtmlTag(CEventCell*,char*,UINT,int)
{}

//***************************************************************************************************************CFGTOOLS//
// Check_ShowCfgTools
//---------------------------------------------------//
void Check_ShowCfgTools(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndState=GetDlgItem(hWndActive,IDC_STATIC_STATE);
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	HWND hWndChkActive=GetDlgItem(hWndActive,IDC_CHECK_ACTIVE);
	HWND hWndChkInactive=GetDlgItem(hWndActive,IDC_CHECK_INACTIVE);
	HWND hWndChkChgInt=GetDlgItem(hWndActive,IDC_CHECK_CHANGEINT);
	MoveWindow(hWndState,118,43,120,16,TRUE);
	MoveWindow(hWndEditState,236,41,60,20,TRUE);
	MoveWindow(hWndChkActive,18,43,100,16,TRUE);
	MoveWindow(hWndChkInactive,18,60,100,16,TRUE);
	MoveWindow(hWndChkChgInt,18,77,100,16,TRUE);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_ASSIGNED,lpTmp,MAX_RS_SIZE);
	SetWindowText(hWndState,lpTmp);
	EnableWindow(hWndEditState,FALSE);
	ShowWindow(hWndState,SW_SHOW);
	ShowWindow(hWndEditState,SW_SHOW);
	ShowWindow(hWndChkActive,SW_SHOW);
	ShowWindow(hWndChkInactive,SW_SHOW);
	ShowWindow(hWndChkChgInt,SW_SHOW);
}

//---------------------------------------------------//
// Radio_ShowCfgTools
//---------------------------------------------------//
void Radio_ShowCfgTools(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndState=GetDlgItem(hWndActive,IDC_STATIC_STATE);
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	HWND hWndLinkItm=GetDlgItem(hWndActive,IDC_LINK_ITEM);
	HWND hWndUnlinkItm=GetDlgItem(hWndActive,IDC_UNLINK_ITEM);
	MoveWindow(hWndState,188,100,105,16,TRUE);
	MoveWindow(hWndEditState,188,118,107,20,TRUE);
	MoveWindow(hWndRadioList,14,40,163,134,TRUE);
	MoveWindow(hWndLinkItm,188,42,107,25,TRUE);
	MoveWindow(hWndUnlinkItm,188,72,107,25,TRUE);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_CTRLROW,lpTmp,MAX_RS_SIZE);
	SetWindowText(hWndState,lpTmp);
	ShowWindow(hWndRadioList,SW_SHOW);
	ShowWindow(hWndLinkItm,SW_SHOW);
	ShowWindow(hWndUnlinkItm,SW_SHOW);
	ShowWindow(hWndState,SW_SHOW);
	ShowWindow(hWndEditState,SW_SHOW);
}

//-----------------------------------------------------------------------------------------------------------------------//
// Check_HideCfgTools
//---------------------------------------------------//
void Check_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_CHECK_ACTIVE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_CHECK_INACTIVE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_CHECK_CHANGEINT),SW_HIDE);
}

//---------------------------------------------------//
// Radio_HideCfgTools
//---------------------------------------------------//
void Radio_HideCfgTools(HWND hWndActive)
{	ShowWindow(hWndRadioList,SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_UNLINK_ITEM),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
}

//-----------------------------------------------------------------------------------------------------------------------//
// Check_SaveCfg
//---------------------------------------------------//
void Check_SaveCfg(HWND hWndActive)
{	if(SendMessage(GetDlgItem(hWndActive,IDC_CHECK_INACTIVE),BM_GETCHECK,0,0)==BST_CHECKED)
	{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetBtnState(0);}
	else if(SendMessage(GetDlgItem(hWndActive,IDC_CHECK_ACTIVE),BM_GETCHECK,0,0)==BST_CHECKED)
	{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetBtnState(1);}
	else
	{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetBtnState(2);}
}

//---------------------------------------------------//
// Radio_SaveCfg
//---------------------------------------------------//
void Radio_SaveCfg(HWND hWndActive,BOOL &bErreur)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	if(bItemLinked)
	{	GetWindowText(hWndEditState,lpTmp,MAX_RS_SIZE);
		if((mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,0)>((int)ListView_GetSelectedCount(hWndRadioList)))||
		   (mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,0)<1))
		{	mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,
							  IDS_CTRL_MAXRADIO1,
							  lpTmp,MAX_RS_SIZE,
							  IDS_CTRL_MAXRADIO2,MB_ICONSTOP);
			SetFocus(hWndEditState);
			bErreur=TRUE;
			return;
		}
	}
	ApplyLnkCtrlChange(hWndActive,hWndRadioList,bItemLinked);
}

//-----------------------------------------------------------------------------------------------------------------------//
// Check_LoadCfg
//---------------------------------------------------//
void Check_LoadCfg(HWND hWndActive)
{	HWND hWndChkChgInt=GetDlgItem(hWndActive,IDC_CHECK_CHANGEINT);
	char szbuff[MAX_RS_SIZE]={0};
	if(GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCtrlStyle()) // 3STATE
	{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_INDETERMINATE,szbuff,MAX_RS_SIZE);
		SetWindowText(hWndChkChgInt,szbuff);
	}
	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetBtnState())
	{	case 0: // Unchecked
		{	SendMessage(GetDlgItem(hWndActive,IDC_CHECK_INACTIVE),BM_SETCHECK,BST_CHECKED,0);
			SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),STRING_ZERO);
			break;
		}
		case 1: // Checked
		{	SendMessage(GetDlgItem(hWndActive,IDC_CHECK_ACTIVE),BM_SETCHECK,BST_CHECKED,0);
			SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),STRING_ONE);
			break;
		}
		case 2: // Change state / Indeterminate
		{	SendMessage(hWndChkChgInt,BM_SETCHECK,BST_CHECKED,0);
			SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),STRING_TWO);
			break;
		}
	}
}

//---------------------------------------------------//
// Radio_LoadCfg
//---------------------------------------------------//
void Radio_LoadCfg(HWND hWndActive,HWND pWndTabCtrl,HWND pWndDecrit,HWND pWndBound)
{	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	HWND hWndLinkItm=GetDlgItem(hWndActive,IDC_LINK_ITEM);
	HWND hWndUnlinkItm=GetDlgItem(hWndActive,IDC_UNLINK_ITEM);
	char szbuff[MAX_RS_SIZE]={0};
	hWndRadioList=GetDlgItem(hWndActive,IDC_LIST_RADIO);
	CreateTaskObjects();
	SetParent(hWndRadioList,pWndTabCtrl);
	SetEditStyleAsNumber(hWndEditState);
	FillLnkCtrlLst(hWndRadioList,8,IDI_ICON_RADIO,IDS_VIEW_RADIO);
	EnableWindow(hWndLinkItm,FALSE);
	if(GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetLinked()==NO)
	{	bItemLinked=FALSE;
		EnableWindow(hWndEditState,FALSE);
		EnableWindow(hWndUnlinkItm,FALSE);
		if(GetmHpVarStruct()->bCustomFolder)
		{	EnableWindow(pWndDecrit,FALSE);
			EnableWindow(pWndBound,FALSE);
		}
	}
	else
	{	bItemLinked=TRUE;
		_snprintf(szbuff,MAX_RS_SIZE,"%d",GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetRadioRow());
		SetWindowText(hWndEditState,szbuff);
		EnableWindow(hWndLinkItm,FALSE);
		EnableWindow(hWndRadioList,FALSE);
		if(GetmHpVarStruct()->bCustomFolder) EnableWindow(hWndUnlinkItm,FALSE);
	}
}

//-----------------------------------------------------------------------------------------------------------------------//
// Button_GetInfosCfg
//---------------------------------------------------//
void Button_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_BUTTON,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_BUTTON)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// Check_GetInfosCfg
//---------------------------------------------------//
void Check_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_BUTTON,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_CHECK)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// Radio_GetInfosCfg
//---------------------------------------------------//
void Radio_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_BUTTON,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_BUTTON,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_RADIO)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//-----------------------------------------------------------------------------------------------------------------------//
// Check_ProcCfg
//---------------------------------------------------//
BOOL Check_ProcCfg(HWND hWndActive,UINT Message,WPARAM wParam,LPARAM lParam,HWND hWndChng)
{	if(Message==WM_COMMAND)
	{	switch(LOWORD(wParam))
		{	case IDC_EDIT_STATE:
			{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			case IDC_CHECK_ACTIVE:
			{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),STRING_ONE);
				EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			case IDC_CHECK_INACTIVE:
			{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),STRING_ZERO);
				EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			case IDC_CHECK_CHANGEINT:
			{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),STRING_TWO);
				EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			default:
			{	break;}
		}
	}
	return FALSE;
}

//---------------------------------------------------//
// Radio_ProcCfg
//---------------------------------------------------//
BOOL Radio_ProcCfg(HWND hWndActive,UINT Message,WPARAM wParam,LPARAM lParam,HWND hDecrit,HWND hLimite,HWND hWndChng)
{	switch(Message)
	{	case WM_NOTIFY:
		{	if(((LPNMHDR)lParam)->idFrom==IDC_LIST_RADIO)
			{	if(((LPNMHDR)lParam)->code==LVN_ITEMCHANGED)
				{	if(ListView_GetSelectedCount(hWndRadioList)>=2) 
					{	EnableWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),TRUE);}
					else EnableWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),FALSE);
					return TRUE;
				}
			}
			break;
		}
		case WM_COMMAND:
		{	switch(LOWORD(wParam))
			{	case IDC_EDIT_STATE:
				{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
					return TRUE;
				}
				case IDC_LINK_ITEM:
				{	char lpTmp[MAX_RS_SIZE]={0};
					HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
					LinkItems(hWndRadioList);
					SetWindowText(hWndEditState,STRING_ONE);
					EnableWindow(hWndEditState,TRUE);
					EnableWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),FALSE);
					EnableWindow(GetDlgItem(hWndActive,IDC_UNLINK_ITEM),TRUE);
					SetFocus(GetDlgItem(hWndActive,IDC_UNLINK_ITEM));
					EnableWindow(hWndRadioList,FALSE);
					_snprintf(lpTmp,MAX_RS_SIZE,"[1;%d]",ListView_GetSelectedCount(hWndRadioList));
					SetWindowText(hLimite,lpTmp);
					if(GetmHpVarStruct()->bCustomFolder)
					{	EnableWindow(hLimite,TRUE);
						EnableWindow(hDecrit,TRUE);
					}
					bItemLinked=TRUE;
					EnableWindow(hWndChng,TRUE);
					return TRUE;
				}
				case IDC_UNLINK_ITEM:
				{	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
					GetmHpVarStruct()->CtrlLink->UnLinkItms();
					SetWindowText(hLimite,NULL_STRING);
					SetWindowText(hDecrit,NULL_STRING);
					SetWindowText(hWndEditState,NULL_STRING);
					EnableWindow(hWndRadioList,TRUE);
					EnableWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),TRUE);
					EnableWindow(GetDlgItem(hWndActive,IDC_UNLINK_ITEM),FALSE);
					SetFocus(GetDlgItem(hWndActive,IDC_LINK_ITEM));
					EnableWindow(hWndEditState,FALSE);
					if(GetmHpVarStruct()->bCustomFolder)
					{	EnableWindow(hLimite,FALSE);
						EnableWindow(hDecrit,FALSE);
					}
					bItemLinked=FALSE;
					EnableWindow(hWndChng,TRUE);
					return TRUE;
				}
				default:
				{	break;}
			}
			break;
		}
		default:
		{	break;}
	}
	return FALSE;
}

//****************************************************************************************************************PROCESS//
// Button_CaptureMsg
//---------------------------------------------------//
void Button_CaptureMsg(HWND hWndActive,char* lpCtrlName,HWND hWndCtl,WPARAM wpParam,HWND hToolWnd,int iCurOp,CEventListe* EventObj,
                       char* lpNameApp,UINT &pIdLast,int pCurwnd,int pCntwnd,int pmHpCtrlID,BOOL &pActionUser,
                       int &pResCapt)
{	int iWriteRes=0;
	if(HIWORD(wpParam)==BN_CLICKED)
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
        // Stop this message if no notify DotNet control message
        /*if(*lpCtrlName!=0)
            pResCapt=7;*/
	}
}

//---------------------------------------------------//
// Button_mHpProcess
//---------------------------------------------------//
void Button_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2:
		{	if(GetmHpVarStruct()->imHpMode==1) 
			{	if(GetmHpVarStruct()->CourantTask->GetmHpID()>10) // ToolBar //////////////////////////////////////////////
				{	if(GetmHpVarStruct()->CourantTask->GetmHpID()!=14) // Check+Radio
					{	if(((SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETSTATE,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),0)&TBSTATE_CHECKED)==TBSTATE_CHECKED)&&
							(GetmHpVarStruct()->CourantTask->GetBtnState()!=2))
						{	GetmHpVarStruct()->CourantTask->SetmHpStep(5); // Already checked
						}
						else GetmHpVarStruct()->CourantTask->SetmHpStep(3);
					}
					else GetmHpVarStruct()->CourantTask->SetmHpStep(3);
				}
				else // Button ////////////////////////////////////////////////////////////////////////////////////////////
				{	if(iBtnStyle!=0) // Check+Radio
					{   if(((iBtnStyle==1)&&(GetBtnState(hWndActive)==GetmHpVarStruct()->CourantTask->GetBtnState()))||
						   ((iBtnStyle==2)&&(GetBtnState(hWndActive)==1))) // BST_CHECKED
						{	GetmHpVarStruct()->CourantTask->SetmHpStep(5); // Already checked
						}
						else GetmHpVarStruct()->CourantTask->SetmHpStep(3);
					}
					else GetmHpVarStruct()->CourantTask->SetmHpStep(3);
				}
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
				break;
			}
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			if(GetmHpVarStruct()->CourantTask->GetmHpID()>10) // ToolBar
			{	if((GetmHpVarStruct()->CourantTask->GetmHpID()!=14)&&
				   ((SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETSTATE,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),0)&TBSTATE_CHECKED)==TBSTATE_CHECKED)&&
				   (GetmHpVarStruct()->CourantTask->GetBtnState()!=2))
				{	GetmHpVarStruct()->CursorType=0; // Already checked
					GetmHpVarStruct()->CourantTask->SetmHpStep(5);
				}
			}
			else
			{	if(((iBtnStyle==1)&&(GetBtnState(hWndActive)==GetmHpVarStruct()->CourantTask->GetBtnState()))||
				   ((iBtnStyle==2)&&(GetBtnState(hWndActive)==1))) // BST_CHECKED: Already checked
				{	GetmHpVarStruct()->CursorType=0;
					GetmHpVarStruct()->CourantTask->SetmHpStep(5);
				}
			}
			SetTimer(hWndActive,ID_TIMER_DDRAW,200,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4:
		{	RECT rect;
			PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			if(GetmHpVarStruct()->CourantTask->GetmHpID()>10) // ToolBar //////////////////////////////////////////////////
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(5); // END
				if(GetmHpVarStruct()->CourantTask->GetmHpID()==14) // Bouton
				{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
					SetDDrawObj(hWndActive,0);
                    if((GetmHpVarStruct()->imHpMode==2)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()==NO))
					{	SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);}
					else
					{	if(!RestoreBackBuffer(hWndActive))
                        {   StopmHelp(hWndActive,TRUE,TRUE);
                            return;
                        }
                        UpdateBackBuffer(TRUE);
                        GetmHpVarStruct()->CourantIDCommand=GetmHpVarStruct()->CourantTask->GetCommandID();
						if((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=NO)&&
						   ((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=iCurrentWnd)||
						    (GetmHpVarStruct()->CourantTask->GetCountWnd()!=iCountWnd)))
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
						if(bChangeWnd)
						{	/*SendMessage(hWndActive,
										WM_COMMAND,MAKELONG(GetmHpVarStruct()->CourantIDCommand,0),
										(LPARAM)GetmHpVarStruct()->hWndCurCtrl);*/
							mHpPak_ToolTipsActivate(TRUE);
							SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&rect);
							SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_LBUTTONDOWN,0,MAKELPARAM((rect.left+4),(rect.top+4)));
							SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_LBUTTONUP,0,MAKELPARAM((rect.left+4),(rect.top+4)));
						}
						else
						{	/*SendMessage(hWndActive,
										WM_COMMAND,MAKELONG(GetmHpVarStruct()->CourantIDCommand,0),
										(LPARAM)GetmHpVarStruct()->hWndCurCtrl);*/
							SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&rect);
							SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_LBUTTONDOWN,0,MAKELPARAM((rect.left+4),(rect.top+4)));
							SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_LBUTTONUP,0,MAKELPARAM((rect.left+4),(rect.top+4)));
							SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
						}
					}
				}
				else // Check+Radio
				{	if(GetmHpVarStruct()->imHpMode!=2)
					{	if(!RestoreBackBuffer(hWndActive))
                        {   StopmHelp(hWndActive,TRUE,TRUE);
                            return;
                        }
                        UpdateBackBuffer(TRUE);
                        SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&rect);
						SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_LBUTTONDOWN,0,MAKELPARAM((rect.left+4),(rect.top+4)));
						SendMessage(GetmHpVarStruct()->hWndCurCtrl,WM_LBUTTONUP,0,MAKELPARAM((rect.left+4),(rect.top+4)));
					}
					GetmHpVarStruct()->bRefreshDDraw=TRUE;
					SetDDrawObj(hWndActive,0);
					SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
				}
			}
			else // Button ////////////////////////////////////////////////////////////////////////////////////////////////
			{	if(iBtnStyle==0) // Bouton
				{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
					SetDDrawObj(hWndActive,0);
					GetmHpVarStruct()->CourantTask->SetmHpStep(5); // END
					if((GetmHpVarStruct()->imHpMode==2)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()==NO))
					{	SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);}
					else	
					{	GetmHpVarStruct()->CourantIDCommand=GetmHpVarStruct()->CourantTask->GetCtrlID();
						//if(GetmHpVarStruct()->TaskList->GetNextmHpID()!=NO)
						//{	
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
                        //GetmHpVarStruct()->bLockDotNetMsg=TRUE; // Lock no notify DotNet button message
						if(bChangeWnd)
						{	mHpPak_ToolTipsActivate(TRUE);
							SendMessage(GetParent(GetmHpVarStruct()->hWndCurCtrl),
										WM_COMMAND,MAKELONG(GetmHpVarStruct()->CourantIDCommand,BN_CLICKED),
										(LPARAM)GetmHpVarStruct()->hWndCurCtrl);
						}
						else
						{	SendMessage(GetParent(GetmHpVarStruct()->hWndCurCtrl),
										WM_COMMAND,MAKELONG(GetmHpVarStruct()->CourantIDCommand,BN_CLICKED),
										(LPARAM)GetmHpVarStruct()->hWndCurCtrl);
							SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
						}
						/*}
						else
						{	if(GetmHpVarStruct()->imHpMode!=1)
							{	GetmHpVarStruct()->RepeatLoop++;
								StopmHelp(hWndActive,TRUE,TRUE);
								SendMessage(GetParent(GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantIDCommand)),
											WM_COMMAND,MAKELONG(GetmHpVarStruct()->CourantIDCommand,BN_CLICKED),
											(LPARAM)GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantIDCommand));
								if((GetmHpVarStruct()->imHpMode==1)&&(GetmHpVarStruct()->imHpRepeat>GetmHpVarStruct()->RepeatLoop))
								{	mHpPak_ClearmHpSharedKey();
									SetOperationModeKey(1);
									StartmHelp(hWndActive,FALSE,FALSE);
								}
								else if(GetNextAutoAss())
								{	SetTimer(hWndActive,ID_TIMER_AUTOASS,1000,(TIMERPROC)mHpTimerProc);}
							}
							else
							{	if((GetmHpVarStruct()->CourantTask->GetCurrentWnd()<=GetmHpVarStruct()->CourantTask->GetCountWnd())&&
								  ((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=iCurrentWnd)||(GetmHpVarStruct()->CourantTask->GetCountWnd()!=iCountWnd)))
								{	bChangeWnd=TRUE;}
								SendMessage(GetParent(GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantIDCommand)),
											WM_COMMAND,MAKELONG(GetmHpVarStruct()->CourantIDCommand,BN_CLICKED),
											(LPARAM)GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantIDCommand));
								if((GetmHpVarStruct()->imHpMode==1)&&(GetmHpVarStruct()->imHpRepeat>(GetmHpVarStruct()->RepeatLoop+1))&&(!bChangeWnd))
								{	SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);}
							}
						}*/
					}
				}
				else // Check/Radio
				{	if(!RestoreBackBuffer(hWndActive))
                    {   StopmHelp(hWndActive,TRUE,TRUE);
                        break;
                    }
                    UpdateBackBuffer(TRUE);
                    if(GetmHpVarStruct()->imHpMode!=2)
                    {   //GetmHpVarStruct()->bLockDotNetMsg=TRUE; // Lock no notify DotNet button message
                        SendMessage(GetmHpVarStruct()->hWndCurCtrl,BM_CLICK,0,0);
                    }
					else GetmHpVarStruct()->CourantTask->SetmHpStep(5); // END
					//RefreshControl(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
					GetmHpVarStruct()->bRefreshDDraw=TRUE;
					SetDDrawObj(hWndActive,0);
					if(iBtnStyle==3)
					{	if(GetBtnState(hWndActive)==GetmHpVarStruct()->CourantTask->GetBtnState())
                            GetmHpVarStruct()->CourantTask->SetmHpStep(5); // END
					}
					else GetmHpVarStruct()->CourantTask->SetmHpStep(5); // END
					SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
				}
			}
			break;
		}
		case 5:
		{	GetmHpVarStruct()->CourantTask->SetmHpStep(1);
			SetTimer(hWndActive,ID_TIMER_DELAI,300,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6:
		{	//GetmHpVarStruct()->bLockDotNetMsg=FALSE; // Reset lock no notify DotNet button message
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

//---------------------------------------------------//
// Button_StartProc
//---------------------------------------------------//
BOOL Button_StartProc(HWND hWndActive)
{	GetmHpVarStruct()->hWndCurCtrl=NULL;
	/*if(*GetmHpVarStruct()->CourantTask->GetmHpCtrlName()!=0)
	{	if(mHpPak_IsDotNetHwnd(hWndActive))
		{   int iState=0;
			iBtnStyle=0;
			GetmHpVarStruct()->hWndCurCtrl=mHpPak_GetDotNetCtrlHandle(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlName(),iBtnStyle,iState);
		}
		else if(mHpPak_IsQtHwnd(hWndActive))
			GetmHpVarStruct()->hWndCurCtrl=mHpPak_GetQtCtrlHandle(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlName(),GetmHpVarStruct()->CourantTask->GetmHpID());
	}*/
    if(!GetmHpVarStruct()->hWndCurCtrl)
    {	if(GetmHpVarStruct()->CourantTask->GetmHpID()>10) // ToolBar - Button ///////////////////////////////////////////////
	        GetmHpVarStruct()->hWndCurCtrl=GetToolCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
        else // Button //////////////////////////////////////////////////////////////////////////////////////////////////////
	        GetmHpVarStruct()->hWndCurCtrl=GetCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
	    iBtnStyle=GetBtnStyleID(GetmHpVarStruct()->hWndCurCtrl);
    }
	if(!IsCtrlReady(hWndActive,GetmHpVarStruct()->hWndCurCtrl)) return FALSE;
	RECT rect;
    GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&rect);
	if(!iBtnStyle) // Button
	{	GetmHpVarStruct()->XnextPos=(int)(rect.right-9);
		GetmHpVarStruct()->YnextPos=(int)(rect.bottom-10);
	}
	else // Check + Radio
	{	GetmHpVarStruct()->XnextPos=(int)(rect.left+9);
		if(iBtnStyle==1) GetmHpVarStruct()->XnextPos++;
		GetmHpVarStruct()->YnextPos=(int)(((rect.bottom-rect.top)/2)+rect.top);
		GetmHpVarStruct()->YnextPos-=4;
	}
	return TRUE;
}

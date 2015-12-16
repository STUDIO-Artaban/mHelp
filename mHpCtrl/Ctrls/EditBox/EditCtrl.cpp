// EditCtrl.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <EditBox\EditCtrl.h>

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
#define		CPP_TASK_EDITBOX			"Edit"                      // C/C++ control
#define		VB_TASK_EDITBOX				"ThunderRT6TextBox"         // VB control
#define     DOTNET_TASK_EDITBOX         "WindowsForms10.EDIT.app"   // DotNet control

//-----------------------------------------------------//
// KeyName
#define		EDITBOX_KEY					"Edit"

//-----------------------------------------------------//
// ValueKeys
#define		USERFIXED_KEY				"UserFixed"
#define		EDITVAL_KEY					"EditVal"

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles
char EditText[MAX_RS_SIZE]={0};		// Chaine de caracteres intermediaire pour l'objet "CEditTask"

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration
BOOL SetEditValue(HWND);

//----------------------------------------------------------------------------------------------------------------GENERAL//
// SetEditValue
//-----------------------------------------------------//
BOOL SetEditValue(HWND hWndActive)
{	char temp[MAX_RS_SIZE]={0};
	char chartemp[1]={0};
	if(!GetmHpVarStruct()->CboEditIndice) SetWindowText(GetmHpVarStruct()->hWndCurCtrl,NULL_STRING);
	else
	{	strncpy(temp,GetmHpVarStruct()->CourantTask->GetEditValue(),MAX_RS_SIZE);
		if(((GetmHpVarStruct()->CboEditIndice-1)==MAX_RS_SIZE)||(temp[(GetmHpVarStruct()->CboEditIndice-1)]==END_LIST))
		{	if(GetmHpVarStruct()->CourantTask->GetUserFixed())
			{	SendMessage(GetmHpVarStruct()->hWndCurCtrl,EM_SETSEL,0,GetmHpVarStruct()->CboEditIndice);}
			GetmHpVarStruct()->CboEditIndice=0;
			mHpPak_InitStr(EditText,MAX_RS_SIZE);
			return FALSE;
		}
		if(GetmHpVarStruct()->imHpMode!=1)
		{	_snprintf(chartemp,1,"%c",temp[(GetmHpVarStruct()->CboEditIndice-1)]);
			strncat(EditText,chartemp,1);
			SetWindowText(GetmHpVarStruct()->hWndCurCtrl,EditText);
			SendMessage(GetmHpVarStruct()->hWndCurCtrl,EM_SETSEL,0,GetmHpVarStruct()->CboEditIndice);
			SetFocus(NULL);
		}
		else
		{	SetWindowText(GetmHpVarStruct()->hWndCurCtrl,temp);
			GetmHpVarStruct()->CboEditIndice=0;
			mHpPak_InitStr(EditText,MAX_RS_SIZE);
			if(GetmHpVarStruct()->CourantTask->GetUserFixed())
			{	SendMessage(GetmHpVarStruct()->hWndCurCtrl,EM_SETSEL,0,NO);}
			else SetFocus(NULL);
			return FALSE;
		}
	}
	GetmHpVarStruct()->CboEditIndice++;
	return TRUE;
}

//****************************************************************************************************************OBJECTS//
// class CEditTask
CEditTask::CEditTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,char* lpCtrlName,char* Edit)
		  :CmHpTask(6,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,lpCtrlName)
{	char lpTemp[MAX_RS_SIZE]={0};
	int indice=0;
	mHpPak_InitStr(lpEdit,MAX_RS_SIZE);
	mHpPak_InitStr(lpTemp,MAX_RS_SIZE);
	while(Edit[indice]!=END_LIST)
	{	if(indice==0)
		{	if(Edit[indice]!='1') iUserFixed=0;
			else iUserFixed=1;
		}
		else if(indice>1) lpTemp[(indice-2)]=Edit[indice];
		indice++;
	}
	strncpy(lpEdit,lpTemp,MAX_RS_SIZE);
}

CEditTask::CEditTask()
		  :CmHpTask()
{}

BOOL CEditTask::ReadTaskKey(int index)
{	char lpTmp[MAX_RS_SIZE]={0};
	BYTE BtTab[MAX_KEY_SIZE]={0};
	HKEY hSubEditTaskKey=NULL,hEditTaskKey=NULL;
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;
	CmHpTask::ReadCommonKey(index,TRUE,hSubEditTaskKey,hEditTaskKey);
	if((hEditTaskKey!=NULL)&&(RegOpenKeyEx(hEditTaskKey,EDITBOX_KEY,0,KEY_READ,&hEditTaskKey)==ERROR_SUCCESS))
	{	// iUserFixed
		if(RegQueryValueEx(hEditTaskKey,USERFIXED_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iUserFixed,FALSE);		
		// EditVal
		if(!mHpPak_ReadLongStrRegVal(hEditTaskKey,EDITVAL_KEY,lpEdit,MAX_RS_SIZE))
		{	mHpPak_InitStr(lpEdit,MAX_RS_SIZE);}
		return TRUE;
	}
	return FALSE;
}

BOOL CEditTask::WriteTaskKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubEditKey=NULL,hEditKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size;
	BOOL bAclRes;
	CmHpTask::WriteCommonKey(TRUE,hSubEditKey,hEditKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hEditKey!=NULL)
	{	// EditBox
		if(RegCreateKeyEx(hEditKey,EDITBOX_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// iUserFixed
		if(RegCreateKeyEx(hEditKey,EDITBOX_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iUserFixed,FALSE);
		RegSetValueEx(hTmpKey,USERFIXED_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		// EditVal
		if((strncmp(lpEdit,NULL_STRING,MAX_RS_SIZE)!=0)&&(!mHpPak_WriteLongStrRegVal(hEditKey,EDITBOX_KEY,EDITVAL_KEY,lpEdit)))
		{	return FALSE;}
		return TRUE;
	}
	return FALSE;
}

char* CEditTask::GetEditValue()
{	return lpEdit;}

void CEditTask::SetEditValue(char* lpNewEdit)
{	mHpPak_InitStr(lpEdit,MAX_RS_SIZE);
	strncpy(lpEdit,lpNewEdit,MAX_RS_SIZE);
}

int CEditTask::GetUserFixed()
{	return iUserFixed;}

void CEditTask::SetUserFixed(int iNewFlag)
{	iUserFixed=iNewFlag;}

// class CEditWrite ///////////////////////////////////////////////////////////////////////////////////////////////////////
CEditWrite::CEditWrite(int iStyle)
		   :CStyleWrite(iStyle)
{}

CEditWrite::CEditWrite()
		   :CStyleWrite()
{}

//****************************************************************************************************************MHPFILE//
// Edit_ReadEvent
//---------------------------------------------------//
BOOL Edit_ReadEvent(HWND hWndActive,const std::string& strPostData,UINT &iCustIdx)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpEdit[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	int iCur,iCnt,iTmp,iOpt;
	char chNext=END_RS_TASK;
	BOOL bSel=TRUE;
	CEditTask *EditTask;
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
#ifdef MHELP_OLD
	// lpEdit // ** Param type: String **
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpEdit,MAX_RS_SIZE,chNext,TRUE);
	if(lpEdit[0]==END_LIST) return TRUE;
	if(lpEdit[2]==RS_PARAM)
	{	iTmp=mHpPak_GetIntFromStr(lpEdit,MAX_RS_SIZE,3);
		if(!DidItSelected(iTmp)) bSel=FALSE;
		else
		{	mHpPak_InitStr(lpEdit,MAX_RS_SIZE);
			strncpy(lpEdit,"0^",MAX_RS_SIZE);
			GetStrEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0,lpTmp,MAX_RS_SIZE);
			if(iTmp==NO) return TRUE;
			strncat(lpEdit,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
		}
	}
#else
    ////// TODO: Be able to remove lines below (state always in HTML)
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpEdit,MAX_RS_SIZE,chNext,TRUE);
	if(lpEdit[0]==END_LIST)
        return TRUE; // Error
    //////

    // Check if selected
    if(!IsHtmlCustSelected(strPostData,iCustIdx))
        bSel=FALSE;
    else
    {   // Get HTML user setting
        if(!GetHtmlCustValue(lpTmp,
                             MAX_RS_SIZE,
                             strPostData,
                             EDITVAL_KEY,
                             iCustIdx))
        {   return TRUE; // Error: No state parameter
        }
        strncpy(lpEdit,"0^",sizeof("0^"));
        strncat(lpEdit,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
    }
    // Increase number of custom task
    iCustIdx++;
#endif
	if(bSel)
	{	if(*lpCtrlName!=0) EditTask=new CEditTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName,lpEdit);
        else EditTask=new CEditTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL,lpEdit);
		GetmHpVarStruct()->TaskList->AddCellule(EditTask);
	}
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// Edit_WriteEvent
//---------------------------------------------------//
void Edit_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
                     BOOL bFolderParam,char* lpCurExe,int &IndexParam,BOOL &bRes)
{	if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))
	{	bRes=FALSE;
		return;
	}
	if(!EvntObj->GetmHpWrite(Index,FALSE)->GetCtrlStyle())
	{	if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetmHpTask(Index,FALSE)->GetUserFixed(),NULL,FALSE)) // '¤'
		{	bRes=FALSE;
			return;
		}
	}
	else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,0,NULL,FALSE)) // '¤'
	{	bRes=FALSE;
		return;
	}
	if((bFolderParam)&&(EvntObj->GetCustomVisible(Index))&&(!EvntObj->GetmHpTask(Index,FALSE)->GetUserFixed()))
	{	if((!mHpPak_WriteParty(hWndActive,ITEM_GROUP,NO,NULL,FALSE))|| // '^'
		   (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
		{	bRes=FALSE;}
		IndexParam++;
	}
	else if(!mHpPak_WriteParty(hWndActive,ITEM_GROUP,NO,EvntObj->GetStrState(Index,TRUE),TRUE)) // '^'
	{	bRes=FALSE;}
}

//-----------------------------------------------------------------------------------------------------------------------//
// Edit_WriteParam
//---------------------------------------------------//
BOOL Edit_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if(!EvntObj->GetmHpTask(Index,FALSE)->GetUserFixed())
	{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
		{	return FALSE;}
	}
	return TRUE;
}			

//*******************************************************************************************************************KEYS//
// Edit_ReadEventKeys
//---------------------------------------------------//
void Edit_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpWrite* ReadWrite;
	CmHpParam* ReadParam;
	ReadTask=new CEditTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL)
	{	ReadWrite=new CEditWrite();
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
// Edit_AddEvent
//---------------------------------------------------//
void Edit_AddEvent(HWND hWndActive,char* lpCtrlName,UINT iTaskID,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
	CmHpWrite* wItem;
    HWND hWndEdit=NULL;
    int iStyle=0;
    int lpctrl[MAX_PART_TREE]={0};
    /*if(*lpCtrlName!=0)
    {   int iState=0;
		if(mHpPak_IsDotNetHwnd(hWndActive))
			hWndEdit=mHpPak_GetDotNetCtrlHandle(hWndActive,lpCtrlName,iStyle,iState);
    }
    else*/
    {   AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,iTaskID);
        hWndEdit=GetDlgItem(hWndActive,iTaskID);
	    if(IsWndStyle(hWndEdit,ES_MULTILINE)!=ES_MULTILINE) iStyle=0;
	    else if((IsWndStyle(hWndEdit,ES_MULTILINE)==ES_MULTILINE)&&
                (IsWndStyle(hWndEdit,ES_WANTRETURN)!=ES_WANTRETURN))
	    {	iStyle=1;}
	    else
	    {	iStyle=2;}
    }
	char lpTemp[MAX_RS_SIZE]={0};
	char lpEditVal[MAX_RS_SIZE]={0};
	GetWindowText(hWndEdit,lpEditVal,MAX_RS_SIZE);
	mHpPak_InitStr(lpTemp,MAX_RS_SIZE);
	_snprintf(lpTemp,MAX_RS_SIZE,"%d%c%s",0,NEXT_RS,lpEditVal);
	mHpPak_InitStr(lpEditVal,MAX_RS_SIZE);
	strncpy(lpEditVal,lpTemp,MAX_RS_SIZE);
    if(*lpCtrlName!=0) Item=new CEditTask(NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName,lpEditVal);
    else Item=new CEditTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,lpEditVal);
    wItem=new CEditWrite(iStyle);
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
// Edit_IsClassName
//---------------------------------------------------//
BOOL Edit_IsClassName(char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_EDITBOX,ClassSize))||(!strncmp(CtrlClass,VB_TASK_EDITBOX,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_EDITBOX,sizeof(DOTNET_TASK_EDITBOX)-1))) return TRUE;
	return FALSE;
}

//---------------------------------------------------//
// Edit_IsCustom
//---------------------------------------------------//
void Edit_IsCustom(CEventCell* pCurCell,BOOL &bcustom)
{	if(!pCurCell->mHpTask->GetUserFixed()) bcustom=TRUE;
	else bcustom=FALSE;
}

//---------------------------------------------------//
// Edit_StrState
//---------------------------------------------------//
void Edit_StrState(CEventCell* pCurCell,char* lpState,int StateSize)
{	strncpy(lpState,pCurCell->mHpTask->GetEditValue(),StateSize);}

//---------------------------------------------------//
// Edit_ParamType
//---------------------------------------------------//
int Edit_ParamType()
{	return 4; // S
}

//-----------------------------------------------------//
// Edit_DefaultBound
//-----------------------------------------------------//
void Edit_DefaultBound(char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	strncpy(pStrBound,STRING_LIMITE,BoundSize);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
	strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
}

//-----------------------------------------------------//
// Edit_IsCustomized
//-----------------------------------------------------//
BOOL Edit_IsCustomized(CEventCell* pCurCell,BOOL bCustom)
{	if((!pCurCell->mHpTask->GetUserFixed())||(!bCustom)) return TRUE;
	return FALSE;
}

//-----------------------------------------------------//
// Edit_IsNoStateMsg
//-----------------------------------------------------//
BOOL Edit_IsNoStateMsg()
{	return FALSE;}

//-----------------------------------------------------//
// Edit_GetHtmlTag
//-----------------------------------------------------//
#define EDITBOX_HTML_TAG    "\t<input type=\"text\" name=\"%d-%s\" maxlength=\"100\" value=\"%s\"/>\r\n"

void Edit_GetHtmlTag(CEventCell* pCurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{
    if(!pCurCell->mHpTask->GetUserFixed())
    {
        _snprintf(lpHtmlTag,
                  iMaxSize,
                  EDITBOX_HTML_TAG,
                  iCustIndex,
                  EDITVAL_KEY,
                  pCurCell->mHpTask->GetEditValue());
    }
}

//***************************************************************************************************************CFGTOOLS//
// Edit_ShowCfgTools
//---------------------------------------------------//
void Edit_ShowCfgTools(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndState=GetDlgItem(hWndActive,IDC_STATIC_STATE);
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	HWND hWndFixed=GetDlgItem(hWndActive,IDC_RADIO_FIXED);
	HWND hWndUser=GetDlgItem(hWndActive,IDC_RADIO_USER);
	HWND hWndMultiEdit=GetDlgItem(hWndActive,IDC_MULTI_EDIT);
	MoveWindow(hWndState,18,43,256,16,TRUE);
	MoveWindow(hWndFixed,18,84,200,16,TRUE);
	MoveWindow(hWndUser,18,102,200,16,TRUE);
	MoveWindow(hWndMultiEdit,18,59,276,117,TRUE);
	if(!GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCtrlStyle())
	{	MoveWindow(hWndEditState,18,59,276,20,TRUE);
		if(SendMessage(hWndFixed,BM_GETCHECK,0,0)==BST_CHECKED)
		{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_CONTENTS,lpTmp,MAX_RS_SIZE);}
		else
		{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_DESCRIPTION,lpTmp,MAX_RS_SIZE);}
		ShowWindow(hWndFixed,SW_SHOW);
		ShowWindow(hWndUser,SW_SHOW);
		ShowWindow(hWndEditState,SW_SHOW);
	}
	else
	{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_CONTENTS,lpTmp,MAX_RS_SIZE);
		if(GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCtrlStyle()==2)
		{	ShowWindow(hWndMultiEdit,SW_SHOW);}				
		else
		{	MoveWindow(hWndEditState,18,59,276,20,TRUE);
			ShowWindow(hWndEditState,SW_SHOW);
		}
	}
	SetWindowText(hWndState,lpTmp);
	ShowWindow(hWndState,SW_SHOW);
}

//---------------------------------------------------//
// Edit_HideCfgTools
//---------------------------------------------------//
void Edit_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_RADIO_FIXED),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_RADIO_USER),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_MULTI_EDIT),SW_HIDE);
}

//---------------------------------------------------//
// Edit_SaveCfg
//---------------------------------------------------//
void Edit_SaveCfg(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	int indice=0;
	if(GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCtrlStyle()!=2)
	{	GetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lpTmp,MAX_RS_SIZE);
		GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetEditValue(lpTmp);
	}
	else
	{	GetWindowText(GetDlgItem(hWndActive,IDC_MULTI_EDIT),lpTmp,MAX_RS_SIZE);
		GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetEditValue(lpTmp);
	}
	if(!GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCtrlStyle())
	{	if(SendMessage(GetDlgItem(hWndActive,IDC_RADIO_FIXED),BM_GETCHECK,0,0)==BST_CHECKED)
		{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetUserFixed(0);}
		else
		{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetUserFixed(1);}
	}
}

//---------------------------------------------------//
// Edit_LoadCfg
//---------------------------------------------------//
void Edit_LoadCfg(HWND hWndActive)
{	int indice=0;
	if(GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCtrlStyle()==2)
	{	SetWindowText(GetDlgItem(hWndActive,IDC_MULTI_EDIT),GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetEditValue());}
	else
	{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetEditValue());
		if(!GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCtrlStyle())
		{	if(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetUserFixed())
			{	SendMessage(GetDlgItem(hWndActive,IDC_RADIO_USER),BM_SETCHECK,BST_CHECKED,0);}
			else
			{	SendMessage(GetDlgItem(hWndActive,IDC_RADIO_FIXED),BM_SETCHECK,BST_CHECKED,0);}
			if(GetmHpVarStruct()->bCustomFolder)
			{	EnableWindow(GetDlgItem(hWndActive,IDC_RADIO_USER),FALSE);
				EnableWindow(GetDlgItem(hWndActive,IDC_RADIO_FIXED),FALSE);
			}
		}
	}
}

//---------------------------------------------------//
// Edit_GetInfosCfg
//---------------------------------------------------//
void Edit_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_EDIT,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_EDIT,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_EDIT,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_EDIT,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_EDIT)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// Edit_ProcCfg
//---------------------------------------------------//
BOOL Edit_ProcCfg(HWND hWndActive,UINT Message,WPARAM wParam,HWND hWndChng)
{	if(Message==WM_COMMAND)
	{	switch(LOWORD(wParam))
		{	case IDC_EDIT_STATE:
			{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			case IDC_RADIO_FIXED:
			{	char lpTmp[MAX_RS_SIZE]={0};
				LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_CONTENTS,lpTmp,MAX_RS_SIZE);
				SetWindowText(GetDlgItem(hWndActive,IDC_STATIC_STATE),lpTmp);
				EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			case IDC_RADIO_USER:
			{	char lpTmp1[MAX_RS_SIZE]={0};
				LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_DESCRIPTION,lpTmp1,MAX_RS_SIZE);
				SetWindowText(GetDlgItem(hWndActive,IDC_STATIC_STATE),lpTmp1);
				EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			case IDC_MULTI_EDIT:
			{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			default:
			{	break;}
		}
	}
	return FALSE;
}

//****************************************************************************************************************PROCESS//
// Edit_CaptureMsg
//---------------------------------------------------//
void Edit_CaptureMsg(HWND hWndActive,char* lpCtrlName,WPARAM wpParam,HWND hToolWnd,int iCurOp,CEventListe* EventObj,
                     char* lpNameApp,UINT &pIdLast,int pCurwnd,int pCntwnd,BOOL &pActionUser,int &pResCapt)
{	int iWriteRes=0;
	if(HIWORD(wpParam)==EN_CHANGE)
	{	WriteImmediatTask(hWndActive,lpCtrlName,NULL,hToolWnd,NULL,iCurOp,6,(UINT)((int)LOWORD(wpParam)),
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
// Edit_mHpProcess
//---------------------------------------------------//
void Edit_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2:
		{	char chartab[MAX_PATH]={0};
			char comptab[MAX_PATH]={0};
			if(!GetmHpVarStruct()->CourantTask->GetUserFixed())
			{	GetWindowText(GetmHpVarStruct()->hWndCurCtrl,chartab,MAX_PATH);
				_snprintf(comptab,MAX_PATH,GetmHpVarStruct()->CourantTask->GetEditValue());
				if(!strncmp(comptab,chartab,MAX_PATH))
				{	GetmHpVarStruct()->CourantTask->SetmHpStep(6);
					SetTimer(hWndActive,ID_TIMER_DELAI,300,(TIMERPROC)mHpTimerProc);
					break;
				}
				else if(GetmHpVarStruct()->imHpMode==2) GetmHpVarStruct()->CourantTask->SetEditValue(chartab);
			}
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			if(GetmHpVarStruct()->imHpMode==1)
			{	SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);}
			else SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=0;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 5:
		{	GetmHpVarStruct()->XnextPos+=8;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6:
		{	SetEditValue(hWndActive);
			SetTimer(hWndActive,ID_TIMER_EDIT,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7:
		{	GetmHpVarStruct()->EditFlag=FALSE;
			SetFocus(NULL);
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
// Edit_StartProc
//---------------------------------------------------//
BOOL Edit_StartProc(HWND hWndActive)
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
	return TRUE;
}

//**************************************************************************************************************TIMERPROC//
// Edit_TimerProc
//---------------------------------------------------//
void Edit_TimerProc(HWND hWndActive)
{	if(!UpdateFrame(hWndActive))
        StopmHelp(hWndActive,TRUE,TRUE);
    else if(SetEditValue(hWndActive))
    {	SetTimer(hWndActive,ID_TIMER_EDIT,50,(TIMERPROC)mHpTimerProc);}
	else if(GetmHpVarStruct()->CourantTask->GetUserFixed())
	{	GetmHpVarStruct()->EditFlag=TRUE;
		SetFocus(GetmHpVarStruct()->hWndCurCtrl);
	}
	else
	{	// Messages "EN_SETFOCUS" "EN_KILLFOCUS"
		SetFocus(GetmHpVarStruct()->hWndCurCtrl);
		ForceMessage(hWndActive,EN_CHANGE);
		SetFocus(NULL);
		NextStepTaskProc(hWndActive,FALSE);
	}
}

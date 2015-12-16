// ChExCtrl.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <ChangeExe\ChExCtrl.h>

//-------------------------------//
#include "mHp2D.h"
#include "resource.h"

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

//-----------------------------------------------------//
// KeyName

//-----------------------------------------------------//
// ValueKeys
#define		NEWEXE_KEY			"NewExe"
#define		COMMANDLINE_KEY		"CmdLn"

//-----------------------------------------------------------------------------------------------------------------------//
// Variables

//----------------------------------------------------------------------------------------------------------------GENERAL//
// Déclarations

//*****************************************************************************************************************OBJECT//
CChangeExeTask::CChangeExeTask(int Option,BOOL bNewApp,char* lpMessage,char* lpAppliName,char* lpCommand)
			   :CmHpTask(0,lpMessage,NULL,0,0,Option,NULL,NULL)
{	strncpy(lpmHpExe,lpAppliName,MAX_RS_SIZE);
	if(lpCommand!=NULL) strncpy(lpCmdLine,lpCommand,MAX_RS_SIZE);
	else mHpPak_InitStr(lpCmdLine,MAX_RS_SIZE);
	bNewExe=bNewApp;
	hWndExe=NULL;
}

CChangeExeTask::CChangeExeTask()
			   :CmHpTask()
{}

BOOL CChangeExeTask::ReadTaskKey(int index)
{	char lpTmp[MAX_RS_SIZE]={0};
	BYTE ByteTab[MAX_KEY_SIZE]={0};
	DWORD isize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	HKEY hSubChgExeTskKey=NULL,hChgExeTskKey=NULL;
	int iTmp;
	CmHpTask::ReadCommonKey(index,FALSE,hSubChgExeTskKey,hChgExeTskKey);
	mHpPak_EmptyTab(this->GetmHpCtrlID(),MAX_PART_TREE);
	this->GetmHpCtrlID()[0]=NO;
	if(hChgExeTskKey!=NULL)
	{	// imHpOption
		if(RegQueryValueEx(hChgExeTskKey,MHPOPTION_KEY,NULL,&dwType,ByteTab,&isize)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(ByteTab,iTmp,FALSE);
		this->SetmHpOption(iTmp);
		// lpmHpMsg
		if(mHpPak_ReadLongStrRegVal(hChgExeTskKey,MHPMSG_KEY,lpTmp,MAX_RS_SIZE)) this->SetmHpMsg(lpTmp);
		else
		{	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
			this->SetmHpMsg(lpTmp);
		}
		if(RegOpenKeyEx(hChgExeTskKey,CHANGEEXE_KEY,0,KEY_READ,&hChgExeTskKey)==ERROR_SUCCESS)		
		{	// AppName
			if(!mHpPak_ReadLongStrRegVal(hChgExeTskKey,APPNAME_KEY,lpmHpExe,MAX_RS_SIZE)) return FALSE;
			// Command
			mHpPak_InitStr(lpCmdLine,MAX_RS_SIZE);
			if(mHpPak_ReadLongStrRegVal(hChgExeTskKey,COMMANDLINE_KEY,lpTmp,MAX_RS_SIZE))
			{	strncpy(lpCmdLine,lpTmp,MAX_RS_SIZE);}
			// NewExe
			isize=MAX_KEY_SIZE;dwType=REG_BINARY;
			if(RegQueryValueEx(hChgExeTskKey,NEWEXE_KEY,NULL,&dwType,ByteTab,&isize)!=ERROR_SUCCESS) return FALSE;
			mHpPak_FillIntFromData(ByteTab,iTmp,TRUE);
			bNewExe=(BOOL)iTmp;
			// HwndExe
			isize=MAX_KEY_SIZE;dwType=REG_DWORD;
			if(RegQueryValueEx(hChgExeTskKey,HWNDEXE_KEY,NULL,&dwType,ByteTab,&isize)==ERROR_SUCCESS)
			{	mHpPak_FillIntFromData(ByteTab,iTmp,FALSE);
				hWndExe=(HWND)iTmp;
			} 
			else hWndExe=NULL;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CChangeExeTask::WriteTaskKey()
{	DWORD opWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES lpSec;
	BYTE BTab[MAX_KEY_SIZE]={0};
	HKEY hTmpKey,hSubChgExeKey=NULL,hChgExeKey=NULL;
	int iSize;	
	BOOL bAclRes;
	CmHpTask::WriteCommonKey(FALSE,hSubChgExeKey,hChgExeKey);
	mHpPak_GetSecurityAtt(lpSec,bAclRes);
	if(hChgExeKey!=NULL)
	{	// imHpOption
		if(RegCreateKeyEx(hSubChgExeKey,MHPTASK_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
						&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
		iSize=mHpPak_FillDataFromNumber(BTab,this->GetmHpOption(),FALSE);
		RegSetValueEx(hTmpKey,MHPOPTION_KEY,0,REG_DWORD,BTab,iSize);
		RegCloseKey(hTmpKey);
		// lpmHpMsg
		if((strncmp(this->GetmHpMsg(),NULL_STRING,MAX_RS_SIZE)!=0)&&(!mHpPak_WriteLongStrRegVal(hSubChgExeKey,MHPTASK_KEY,MHPMSG_KEY,this->GetmHpMsg())))
		{	return FALSE;}
		// ChangeExe
		if(RegCreateKeyEx(hChgExeKey,CHANGEEXE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
						&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
		// AppName
		if(!mHpPak_WriteLongStrRegVal(hChgExeKey,CHANGEEXE_KEY,APPNAME_KEY,lpmHpExe)) return FALSE;
		// Command
		if((strncmp(lpCmdLine,NULL_STRING,MAX_RS_SIZE)!=0)&&(!mHpPak_WriteLongStrRegVal(hSubChgExeKey,MHPTASK_KEY,COMMANDLINE_KEY,lpCmdLine)))
		{	return FALSE;}
		// NewExe
		if(RegCreateKeyEx(hChgExeKey,CHANGEEXE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
						&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
		iSize=mHpPak_FillDataFromNumber(BTab,NO,(BOOL)bNewExe);
		RegSetValueEx(hTmpKey,NEWEXE_KEY,0,REG_BINARY,BTab,iSize);
		RegCloseKey(hTmpKey);
		// HwndExe
		if(hWndExe!=NULL)
		{	if(RegCreateKeyEx(hChgExeKey,CHANGEEXE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
							&hTmpKey,&opWord)!=ERROR_SUCCESS) return FALSE;
			iSize=mHpPak_FillDataFromNumber(BTab,(int)hWndExe,FALSE);
			RegSetValueEx(hTmpKey,HWNDEXE_KEY,0,REG_DWORD,BTab,iSize);
			RegCloseKey(hTmpKey);
		}
		return TRUE;
	}
	return FALSE;
}

char* CChangeExeTask::GetExeName()
{	return lpmHpExe;}

BOOL CChangeExeTask::GetNewExe()
{	return bNewExe;}

void CChangeExeTask::SetHwndExe(HWND hWndActive)
{	hWndExe=hWndActive;}

HWND CChangeExeTask::GetHwndExe()
{	return hWndExe;}

char* CChangeExeTask::GetCommandLine()
{	return lpCmdLine;}

void CChangeExeTask::SetCommandLine(char* lpNewCmd)
{	mHpPak_InitStr(lpCmdLine,MAX_RS_SIZE);
	if(lpNewCmd!=NULL) strncpy(lpCmdLine,lpNewCmd,MAX_RS_SIZE);
}

//****************************************************************************************************************MHPFILE//
// ChangeExe_ReadEvent
//-----------------------------------------------------//
BOOL ChangeExe_ReadEvent(HWND hWndActive,const std::string& strPostData,UINT &iCustIdx)
{	char lpAppli[MAX_RS_SIZE]={0};
	char lpCmdln[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	int iTmp,iOption,iNew;
	char chNext=END_RS_TASK;
	BOOL bError,bContinue;
	CmHpTask* ExeTask;
	// Option
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iOption,lpMsg,MAX_RS_SIZE,chNext,FALSE);
	if(iOption==NO) return TRUE; // Error
	// New
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iNew,lpMsg,MAX_RS_SIZE,chNext,FALSE);
	if(iNew==NO) return TRUE; // Error
	// Msg
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpMsg,MAX_RS_SIZE,chNext,TRUE);
	if(iTmp==NO) return TRUE; // Error
	// Appli
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpAppli,MAX_RS_SIZE,chNext,TRUE);
	if(iTmp==NO) return TRUE; // Error
	if(iNew)
	{
#ifdef MHELP_OLD
        // Command
		GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpCmdln,MAX_RS_SIZE,bContinue,bError);
		if((!bError)&&(!bContinue))
		{	if(lpCmdln[0]==RS_PARAM)
			{	iTmp=mHpPak_GetIntFromStr(lpCmdln,MAX_RS_SIZE,1);
				if(!DidItSelected(iTmp)) mHpPak_InitStr(lpCmdln,MAX_RS_SIZE);
				else
				{	GetStrEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0,lpCmdln,MAX_RS_SIZE);
					if(iTmp==NO) return TRUE;
				}
			}
		}
		else return TRUE; // Error
#else
        ////// TODO: Be able to remove lines below (state always in HTML)
		GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpCmdln,MAX_RS_SIZE,bContinue,bError);
		if((bError)||(bContinue))
		    return TRUE; // Error
        //////

        // Check if selected
        if(!IsHtmlCustSelected(strPostData,iCustIdx))
            mHpPak_InitStr(lpCmdln,MAX_RS_SIZE); // Can be unselected (change application can be removed)
        else
        {   // Get HTML user setting
            if(!GetHtmlCustValue(lpCmdln,
                                 MAX_RS_SIZE,
                                 strPostData,
                                 COMMANDLINE_KEY,
                                 iCustIdx))
            {   return TRUE; // Error: No state parameter
            }
        }
        // Increase number of custom task
        iCustIdx++;
#endif
	}
	ExeTask=new CChangeExeTask(iOption,(BOOL)iNew,lpMsg,lpAppli,lpCmdln);
	GetmHpVarStruct()->TaskList->AddCellule(ExeTask);
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// ChangeExe_WriteEvent
//-----------------------------------------------------//
void ChangeExe_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,BOOL bFolderParam,
                          char* lpCurExe,int ExeSize,int &IndexParam,BOOL &bRes)
{	// mHpID // iOption // bNewExe // lpmHpMsg // lpExeName
	if((!mHpPak_WriteParty(hWndActive,NEXT_TASK,EvntObj->GetmHpTask(Index,FALSE)->GetmHpID(),NULL,FALSE))|| // '_'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetmHpTask(Index,FALSE)->GetmHpOption(),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetNewExe(),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetmHpTask(Index,FALSE)->GetmHpMsg(),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetmHpTask(Index,FALSE)->GetExeName(),TRUE))) // '¤'
	{	bRes=FALSE;
		return;
	}
	if(EvntObj->GetmHpTask(Index,FALSE)->GetNewExe())
	{	if((bFolderParam)&&(EvntObj->GetCustomVisible(Index)))
		{	if((!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
			   (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
			{	bRes=FALSE;}
			IndexParam++;
		}
		else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrState(Index,TRUE),TRUE)) // lpCmdLine
		{	bRes=FALSE;}	
	}
	mHpPak_InitStr(lpCurExe,ExeSize);
	strncpy(lpCurExe,EvntObj->GetmHpTask(Index,FALSE)->GetExeName(),ExeSize);
}

//-----------------------------------------------------------------------------------------------------------------------//
// ChangeExe_WriteParam
//-----------------------------------------------------//
BOOL ChangeExe_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if(EvntObj->GetmHpTask(Index,FALSE)->GetNewExe())
	{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
		{	return FALSE;}
	}
	return TRUE;
}			
			
//*******************************************************************************************************************KEYS//
// ChangeExe_ReadEventKeys
//-----------------------------------------------------//
void ChangeExe_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	ReadTask=new CChangeExeTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL) EvntObj->AddEvent(ReadTask,NULL,NULL);
	else TaskList->AddCellule(ReadTask);
}

//*************************************************************************************************************PROPERTIES//
// ChangeExe_IsCustom
//-----------------------------------------------------//
void ChangeExe_IsCustom(CEventCell* EvntCell,int &LastID,BOOL &bcustom)
{	LastID=NO;
	if(EvntCell->mHpTask->GetNewExe()) bcustom=TRUE;
}

//-----------------------------------------------------//
// ChangeExe_StrState
//-----------------------------------------------------//
void ChangeExe_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
{	if(pCurCell->mHpTask->GetNewExe())
	{	if(pWriteOp) strncpy(lpState,pCurCell->mHpTask->GetCommandLine(),StateSize);
		else _snprintf(lpState,StateSize,"%s%s%s%s",pCurCell->mHpTask->GetExeName(),MHPPAK_EXE_EXTENSION," ",pCurCell->mHpTask->GetCommandLine());
	}
	else
	{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_NO_STATE,lpState,StateSize);}
}

//-----------------------------------------------------//
// ChangeExe_ParamType
//-----------------------------------------------------//
int ChangeExe_ParamType(CEventCell* EvntCell)
{	if(EvntCell->mHpTask->GetNewExe()) return 4; // S 
	else return NO;
}

//-----------------------------------------------------//
// ChangeExe_DefaultBound
//-----------------------------------------------------//
void ChangeExe_DefaultBound(CEventCell* EvntCell,char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	if(EvntCell->mHpTask->GetNewExe())
	{	strncpy(pStrBound,STRING_LIMITE,BoundSize);
		LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
		strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
	}
}

//-----------------------------------------------------//
// ChangeExe_IsCustomized
//-----------------------------------------------------//
BOOL ChangeExe_IsCustomized(CEventCell* EvntCell)
{	if(EvntCell->mHpTask->GetNewExe()) return TRUE;
	return FALSE;
}

//-----------------------------------------------------//
// ChangeExe_IsNoStateMsg
//-----------------------------------------------------//
BOOL ChangeExe_IsNoStateMsg()
{	return FALSE;}

//-----------------------------------------------------//
// ChangeExe_GetHtmlTag
//-----------------------------------------------------//
#define CHANGEEXE_HTML_TAG  "\t<input type=\"text\" name=\"%d-%s\" maxlength=\"100\" value=\"%s\"/>\r\n"

void ChangeExe_GetHtmlTag(CEventCell* EvntCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{
    if(EvntCell->mHpTask->GetNewExe())
    {
        _snprintf(lpHtmlTag,
                  iMaxSize,
                  CHANGEEXE_HTML_TAG,
                  iCustIndex,                           // Custom index
                  COMMANDLINE_KEY,                      // State label
                  EvntCell->mHpTask->GetCommandLine()); // State value
    }
}

//***************************************************************************************************************CFGTOOLS//
// ChangeExe_ShowCfgTools
//---------------------------------------------------//
void ChangeExe_ShowCfgTools(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndState=GetDlgItem(hWndActive,IDC_STATIC_STATE);
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	MoveWindow(hWndState,18,43,256,16,TRUE);
	MoveWindow(hWndEditState,18,59,276,20,TRUE);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_COMMANDLINE,lpTmp,MAX_RS_SIZE);
	SetWindowText(hWndState,lpTmp);
	ShowWindow(hWndState,SW_SHOW);
	ShowWindow(hWndEditState,SW_SHOW);
}				

//---------------------------------------------------//
// ChangeExe_HideCfgTools
//---------------------------------------------------//
void ChangeExe_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
}				

//---------------------------------------------------//
// ChangeExe_SaveCfg
//---------------------------------------------------//
void ChangeExe_SaveCfg(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	GetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lpTmp,MAX_RS_SIZE);
	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetCommandLine(lpTmp);
}

//---------------------------------------------------//
// ChangeExe_LoadCfg
//---------------------------------------------------//
void ChangeExe_LoadCfg(HWND hWndActive)
{	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCommandLine());
}

//-----------------------------------------------------//
// ChangeExe_GetInfosCfg
//-----------------------------------------------------//
void ChangeExe_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl,HWND hWndNextView,HWND hWndViewNo,HWND hWndWaitEnable)
{	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_CHANGEEXE,lpTmp,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_EXE)),0);
	SetWindowText(hFmCtrl,lpTmp);
	SetWindowText(hProd,NULL_STRING);
	SetWindowText(hVers,NULL_STRING);
	SetWindowText(hDescrip,NULL_STRING);
	EnableWindow(hWndNextView,FALSE);
	EnableWindow(hWndViewNo,FALSE);
	EnableWindow(hWndWaitEnable,FALSE);
}

//---------------------------------------------------//
// ChangeExe_ProcCfg
//---------------------------------------------------//
BOOL ChangeExe_ProcCfg(UINT Message,WPARAM wParam,HWND hWndChng)
{	if(Message==WM_COMMAND)
	{	if(LOWORD(wParam)==IDC_EDIT_STATE)
		{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
			return TRUE;
		}
	}
	return FALSE;
}

//****************************************************************************************************************PROCESS//
// ChangeExe_mHpProcess
//-----------------------------------------------------//
void ChangeExe_mHpProcess(HWND hWndActive)
{}

//-----------------------------------------------------//
// ChangeExe_StartProc
//-----------------------------------------------------//
BOOL ChangeExe_StartProc(HWND hWndActive)
{	char* lpArgs[MAX_KEY_SIZE]={0};
	char lpPathCmd[MAX_KEY_SIZE]={0};
	char lpTmp[MAX_KEY_SIZE]={0};
	int index=0,iArg=0,iTmp=0;
	SendMessage(hWndActive,WM_CANCELMODE,0,0);
	if(GetmHpVarStruct()->CourantTask->GetNewExe())
	{	if(mHpPak_ReadExePathKey(lpPathCmd,MAX_KEY_SIZE,GetmHpVarStruct()->CourantTask->GetExeName()))
		{	strncat(lpPathCmd,MHPPAK_STR_ANTISHLASS,1);
			strncat(lpPathCmd,GetmHpVarStruct()->CourantTask->GetExeName(),
					mHpPak_FindStrLenght(GetmHpVarStruct()->CourantTask->GetExeName(),MAX_RS_SIZE));
			strncat(lpPathCmd,MHPPAK_EXE_EXTENSION,4);
			EndObjects(hWndActive);
			//if(GetmHpVarStruct()->TaskList->IsFirstExeChange()) GetmHpVarStruct()->bNoInactiveMsg=TRUE;
			GetmHpVarStruct()->bNoInactiveMsg=TRUE;
			GetmHpVarStruct()->bLockUnWaMsg=TRUE;
			mHpPak_ToolTipsActivate(TRUE);
			if(strncmp(GetmHpVarStruct()->CourantTask->GetCommandLine(),NULL_STRING,MAX_RS_SIZE)!=0)
			{	while(GetmHpVarStruct()->CourantTask->GetCommandLine()[index]!=END_LIST)
				{	if(GetmHpVarStruct()->CourantTask->GetCommandLine()[index]!=' ')
					{	lpTmp[iTmp]=GetmHpVarStruct()->CourantTask->GetCommandLine()[index];
						iTmp++;
					}
					else
					{	lpArgs[iArg]=(char*)malloc(iTmp+1);
						strncpy(lpArgs[iArg],lpTmp,iTmp);
						lpArgs[iArg][iTmp]=END_LIST;
						iArg++;
						iTmp=0;
					}
					index++;
				}
				lpArgs[iArg]=(char*)malloc(iTmp+1);
				strncpy(lpArgs[iArg],lpTmp,iTmp);
				lpArgs[iArg][iTmp]=END_LIST;
				iArg++;
				lpArgs[iArg]=NULL;
			}
			else
			{	iTmp=mHpPak_FindStrLenght(lpPathCmd,MAX_KEY_SIZE);
				lpArgs[iArg]=(char*)malloc(iTmp+3);
				_snprintf(lpArgs[iArg],iTmp+2,"%s%s%s","\"",lpPathCmd,"\"");
				lpArgs[iArg][iTmp+2]=END_LIST;
				iArg++;
				lpArgs[iArg]=NULL;
			}
			if(_spawnv(_P_NOWAIT,lpPathCmd,lpArgs)!=NO)
			{	if((GetmHpVarStruct()->imHpMode!=1)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()!=NO))
				{	WriteCursorPoskey(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur);}
				iTmp=0;
				while(iArg!=0)
				{	free(lpArgs[iTmp]);
					iTmp++;
					iArg--;
				}
				return TRUE;
			}
			iTmp=0;
			while(iArg!=0)
			{	free(lpArgs[iTmp]);
				iTmp++;
				iArg--;
			}
		}
	}
	else 
	{	EndObjects(hWndActive);
		GetmHpVarStruct()->bNoInactiveMsg=TRUE;
		GetmHpVarStruct()->bLockUnWaMsg=TRUE;
		mHpPak_ToolTipsActivate(TRUE);
		if(BringWindowToTop(GetmHpVarStruct()->TaskList->GetLastHwndExe(GetmHpVarStruct()->CourantTask->GetExeName())))
		{	if((GetmHpVarStruct()->imHpMode!=1)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()!=NO))
			{	WriteCursorPoskey(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur);}
			return TRUE;
		}
	}
	return FALSE;
}

// ToolBar.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <ToolBar\ToolBar.h>

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
// KeyName
#define		TOOLBAR_KEY					"ToolBar"

//-----------------------------------------------------//
// ValueKeys
#define		COMMANDID_KEY				"CommandID"
#define		CHKSTATE_KEY				"ChkState"
#define		CBOSELECT_KEY				"CboSelect"
#define		DROPSELECT_KEY				"MnuSelID"

//-----------------------------------------------------//
// Class name
//#define     DOTNET_TASK_TOOLBAR         "WindowsForms10.ToolbarWindow32.app"    // DotNet control

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles
BOOL bRadItmLinked;
HWND hWndToolRadLst;
BOOL bDropDownOp;

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration
int GetToolbarStyle(HWND,UINT);
void PutToolBarVisible(HWND,RECT&);

//----------------------------------------------------------------------------------------------------------------GENERAL//
// GetToolbarStyle
//-----------------------------------------------------//
int GetToolbarStyle(HWND hWndCtrl,UINT iID)
{	TBBUTTONINFO BtnInfoStr;
	BtnInfoStr.dwMask=TBIF_STYLE;
	BtnInfoStr.idCommand=iID;
	BtnInfoStr.cbSize=sizeof(TBBUTTONINFO);
	SendMessage(hWndCtrl,TB_GETBUTTONINFO,(WPARAM)(INT)iID,(LPARAM)(LPTBBUTTONINFO)&BtnInfoStr);
	if(((BtnInfoStr.fsStyle&TBSTYLE_CHECKGROUP)==TBSTYLE_CHECKGROUP)||
	   (((BtnInfoStr.fsStyle&TBSTYLE_CHECK)==TBSTYLE_CHECK)&&((BtnInfoStr.fsStyle&TBSTYLE_GROUP)==TBSTYLE_GROUP)))
	{	return 16;} // Radiobox
	if((BtnInfoStr.fsStyle&TBSTYLE_CHECK)==TBSTYLE_CHECK)
	{	return 15;} // Checkbox
	if((BtnInfoStr.fsStyle&TBSTYLE_DROPDOWN)==TBSTYLE_DROPDOWN)
	{	return 18;} // DropDown
	return 14; // Button
}

//-----------------------------------------------------//
// PutToolBarVisible
//-----------------------------------------------------//
void PutToolBarVisible(HWND hWndTool,RECT &hRectWnd)
{	
	//
	GetWindowRect(hWndTool,&hRectWnd);
	//
}

//****************************************************************************************************************OBJECTS//
// class CToolBarTask
CToolBarTask::CToolBarTask(int imHpId,char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,UINT CmdID)
			 :CmHpTask(imHpId,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,NULL)
{	CommandID=CmdID;
}

CToolBarTask::CToolBarTask()
			 :CmHpTask()
{}

UINT CToolBarTask::GetCommandID()
{	return CommandID;
}

BOOL CToolBarTask::ReadCommonKey(int iPos,HKEY &SubmHpKeyTask,HKEY &mHpKeyTask)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;
	int iTmp;
	CmHpTask::ReadCommonKey(iPos,TRUE,SubmHpKeyTask,mHpKeyTask);
	if((mHpKeyTask!=NULL)&&(RegOpenKeyEx(mHpKeyTask,TOOLBAR_KEY,0,KEY_READ,&mHpKeyTask)==ERROR_SUCCESS))
	{	// CommandID
		if(RegQueryValueEx(mHpKeyTask,COMMANDID_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iTmp,FALSE);
		CommandID=(UINT)iTmp;
		return TRUE;
	}
	return FALSE;
}

BOOL CToolBarTask::WriteCommonKey(HKEY &SubmHpKeyTask,HKEY &mHpKeyTask)
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size,iTmp;
	BOOL bAclRes;
	CmHpTask::WriteCommonKey(TRUE,SubmHpKeyTask,mHpKeyTask);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(mHpKeyTask!=NULL)
	{	// ToolBar
		if(RegCreateKeyEx(mHpKeyTask,TOOLBAR_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// CommandID
		if(RegCreateKeyEx(mHpKeyTask,TOOLBAR_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		iTmp=(int)CommandID;
		size=mHpPak_FillDataFromNumber(bTab,iTmp,FALSE);
		RegSetValueEx(hTmpKey,COMMANDID_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

// Virtuals -------------------------------------------------------------------------------------------------------------//
BOOL CToolBarTask::ReadTaskKey(int i){ return NULL;}
BOOL CToolBarTask::WriteTaskKey(){ return NULL;}
// CToolChckTask --------------------------------------//
int CToolBarTask::GetBtnState(){ return NULL;}
void CToolBarTask::SetBtnState(int i){ NULL;}
// CToolCboTask ---------------------------------------//
void CToolBarTask::AddMultiIndex(char* s){ NULL;}
char* CToolBarTask::GetMultiIndex(int){ return NULL;}
void CToolBarTask::InitMultiIndex(){ NULL;}
// CToolDrDwTask --------------------------------------//
UINT CToolBarTask::GetMenuSelect(){ return NULL;}
void CToolBarTask::SetMenuSelect(UINT ui){ NULL;}

// class CToolBtnTask /////////////////////////////////////////////////////////////////////////////////////////////////////
CToolBtnTask::CToolBtnTask(char* TaskMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,UINT CmdID)
			 :CToolBarTask(14,TaskMsg,NULL,CurWnd,CntWnd,Option,lpCtrl,CmdID)
{}

CToolBtnTask::CToolBtnTask()
			 :CToolBarTask()
{}

BOOL CToolBtnTask::ReadTaskKey(int index)
{	HKEY hSubTaskKey=NULL,hTaskKey=NULL;
	return CToolBarTask::ReadCommonKey(index,hSubTaskKey,hTaskKey);
}

BOOL CToolBtnTask::WriteTaskKey()
{	HKEY hSubKey=NULL,hKey=NULL;
	return CToolBarTask::WriteCommonKey(hSubKey,hKey);
}

// CToolChckTask //////////////////////////////////////////////////////////////////////////////////////////////////////////
CToolChckTask::CToolChckTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,UINT CmdID,int iState)
			  :CToolBarTask(15,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,CmdID)
{	iChkState=iState;
}

CToolChckTask::CToolChckTask()
			  :CToolBarTask()
{}

BOOL CToolChckTask::ReadTaskKey(int index)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	HKEY hSubChkTaskKey=NULL,hChkTaskKey=NULL;
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;
	CToolBarTask::ReadCommonKey(index,hSubChkTaskKey,hChkTaskKey);
	if(hChkTaskKey!=NULL)
	{	// iChkState
		if(RegQueryValueEx(hChkTaskKey,CHKSTATE_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iChkState,FALSE);
		return TRUE;
	}
	return FALSE;
}

BOOL CToolChckTask::WriteTaskKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubChkKey=NULL,hChkKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size;
	BOOL bAclRes;
	CToolBarTask::WriteCommonKey(hSubChkKey,hChkKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hChkKey!=NULL)
	{	// iChkState
		if(RegCreateKeyEx(hChkKey,TOOLBAR_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iChkState,FALSE);
		RegSetValueEx(hTmpKey,CHKSTATE_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

int CToolChckTask::GetBtnState()
{	return iChkState;
}

void CToolChckTask::SetBtnState(int iNewState)
{	iChkState=iNewState;
}

// CToolRadTask ///////////////////////////////////////////////////////////////////////////////////////////////////////////
CToolRadTask::CToolRadTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,UINT CmdID)
			 :CToolBarTask(16,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,CmdID)
{}

CToolRadTask::CToolRadTask()
			 :CToolBarTask()
{}

BOOL CToolRadTask::ReadTaskKey(int index)
{	HKEY hSubTaskKey=NULL,hTaskKey=NULL;
	return CToolBarTask::ReadCommonKey(index,hSubTaskKey,hTaskKey);
}

BOOL CToolRadTask::WriteTaskKey()
{	HKEY hSubKey=NULL,hKey=NULL;
	return CToolBarTask::WriteCommonKey(hSubKey,hKey);
}

// CToolCboTask ///////////////////////////////////////////////////////////////////////////////////////////////////////////
CToolCboTask::CToolCboTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,UINT CmdID,char* lpCboSel)
			 :CToolBarTask(17,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,CmdID)
{	mHpPak_InitStr(lpToolCbo,MAX_RS_SIZE);
	strncpy(lpToolCbo,lpCboSel,MAX_RS_SIZE);
}

CToolCboTask::CToolCboTask()
			 :CToolBarTask()
{}

BOOL CToolCboTask::ReadTaskKey(int index)
{	HKEY hSubCboTaskKey=NULL,hCboTaskKey=NULL;
	DWORD Type=REG_DWORD;
	CToolBarTask::ReadCommonKey(index,hSubCboTaskKey,hCboTaskKey);
	if(hCboTaskKey!=NULL)
	{	// lpToolCbo
		if(!mHpPak_ReadLongStrRegVal(hCboTaskKey,CBOSELECT_KEY,lpToolCbo,MAX_RS_SIZE))
		{	mHpPak_InitStr(lpToolCbo,MAX_RS_SIZE);}
		return TRUE;
	}
	return FALSE;
}

BOOL CToolCboTask::WriteTaskKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hSubCboKey=NULL,hCboKey=NULL;
	BOOL bAclRes;
	CToolBarTask::WriteCommonKey(hSubCboKey,hCboKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hCboKey!=NULL)
	{	// lpToolCbo
		if((strncmp(lpToolCbo,NULL_STRING,MAX_RS_SIZE)!=0)&&(!mHpPak_WriteLongStrRegVal(hCboKey,TOOLBAR_KEY,CBOSELECT_KEY,lpToolCbo)))
		{	return FALSE;}
		return TRUE;
	}
	return FALSE;
}

void CToolCboTask::AddMultiIndex(char* lpNewCbo)
{	strncpy(lpToolCbo,lpNewCbo,MAX_RS_SIZE);
}

char* CToolCboTask::GetMultiIndex(int index)
{	return lpToolCbo;
}

void CToolCboTask::InitMultiIndex()
{	mHpPak_InitStr(lpToolCbo,MAX_RS_SIZE);
}

// CToolDrDwTask //////////////////////////////////////////////////////////////////////////////////////////////////////////
CToolDrDwTask::CToolDrDwTask(char* TaskMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,UINT CmdID,UINT iMenuSel)
			  :CToolBarTask(18,TaskMsg,NULL,CurWnd,CntWnd,Option,lpCtrl,CmdID)
{	MenuSelID=iMenuSel;
}

CToolDrDwTask::CToolDrDwTask()
			  :CToolBarTask()
{}

BOOL CToolDrDwTask::ReadTaskKey(int index)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	HKEY hSubDrDwTaskKey=NULL,hDrDwTaskKey=NULL;
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;
	int iTmp;
	CToolBarTask::ReadCommonKey(index,hSubDrDwTaskKey,hDrDwTaskKey);
	if(hDrDwTaskKey!=NULL)
	{	// MenuSelID
		if(RegQueryValueEx(hDrDwTaskKey,DROPSELECT_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iTmp,FALSE);
		MenuSelID=(UINT)iTmp;
		return TRUE;
	}
	return FALSE;
}

BOOL CToolDrDwTask::WriteTaskKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubDrDwKey=NULL,hDrDwKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size,iTmp;
	BOOL bAclRes;
	CToolBarTask::WriteCommonKey(hSubDrDwKey,hDrDwKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hDrDwKey!=NULL)
	{	// MenuSelID
		if(RegCreateKeyEx(hDrDwKey,TOOLBAR_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		iTmp=(int)MenuSelID;
		size=mHpPak_FillDataFromNumber(bTab,iTmp,FALSE);
		RegSetValueEx(hTmpKey,DROPSELECT_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

UINT CToolDrDwTask::GetMenuSelect()
{	return MenuSelID;
}

void CToolDrDwTask::SetMenuSelect(UINT pMenuSel)
{	MenuSelID=pMenuSel;
}

// class CToolRadWrite ////////////////////////////////////////////////////////////////////////////////////////////////////
CToolRadWrite::CToolRadWrite()
{}

// class CToolCboWrite ////////////////////////////////////////////////////////////////////////////////////////////////////
CToolCboWrite::CToolCboWrite(int iStyle)
			  :CStyleWrite(iStyle)
{}

CToolCboWrite::CToolCboWrite()
			  :CStyleWrite()
{}

//****************************************************************************************************************MHPFILE//
// ToolBar_ReadEvent
//---------------------------------------------------//
BOOL ToolBar_ReadEvent(HWND hWndActive,int idynId,const std::string& strPostData,UINT &iCustIdx)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpCbo[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	int iCur,iCnt,iTmp,iOpt,iCmdID,iState,iMnuSel;
	char chNext=END_RS_TASK;
	BOOL bSel=TRUE,bError,bContinue;
	CToolBarTask *ToolTask;
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
	// CommandID
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iCmdID,lpTmp,MAX_RS_SIZE,chNext,FALSE);
	if(iCmdID==NO) return TRUE; // Error
	switch(idynId)
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	ToolTask=new CToolBtnTask(lpMsg,iCur,iCnt,iOpt,lpctrl,(UINT)iCmdID);
			break;
		}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{
#ifdef MHELP_OLD
            // iChkState
			mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
			if(iState==NO)
			{	if((lpTmp[0]==END_LIST)||(lpTmp[0]!=RS_PARAM)) return TRUE; // Error
				iTmp=mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,1);
				if(!DidItSelected(iTmp)) bSel=FALSE;
				else
				{	iState=GetIntBoolEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0);
					if(iState==NO) return TRUE; // Error
				}
			}
#else
            ////// TODO: Be able to remove lines below (state always in HTML)
			mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
			if(iState==NO)
                return TRUE; // Error
            //////

            // Check if selected
            if(!IsHtmlCustSelected(strPostData,iCustIdx))
            {   bSel=FALSE;
                iState=0;
            }
            else
            {   // Get HTML user setting
                char lpIntState[MAX_SHT_SIZE]={0};
                if(!GetHtmlCustValue(lpIntState,
                                     MAX_SHT_SIZE,
                                     strPostData,
                                     CHKSTATE_KEY,
                                     iCustIdx))
                {   return TRUE; // Error: No state parameter
                }
                iState=mHpPak_GetIntFromStr(lpIntState,MAX_SHT_SIZE,0);
            }
            // Increase number of custom task
            iCustIdx++;
#endif
			ToolTask=new CToolChckTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,(UINT)iCmdID,iState);
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(chNext==TREE_PARAM_GROUP)
			{	// Radiobox rang
				mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
				if((lpTmp[0]==END_LIST)||(lpTmp[0]!=RS_PARAM)) return TRUE; // Error
				iTmp=mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,1);
				if(!DidItSelected(iTmp)) bSel=FALSE;
				else
				{	iState=GetIntBoolEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0);
					if(iState==NO) return TRUE; // Error
					if(GetmHpVarStruct()->iLnkSel!=iTmp)
					{	GetmHpVarStruct()->iLnkSel=iTmp;
						GetmHpVarStruct()->iLnkLag=NO;
					}
					if(GetmHpVarStruct()->iLnkLag!=NO) GetmHpVarStruct()->iLnkLag++;
					else GetmHpVarStruct()->iLnkLag=1;
					if(GetmHpVarStruct()->iLnkLag!=iState) bSel=FALSE;
				}
			}
			ToolTask=new CToolRadTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,(UINT)iCmdID);
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{
#ifdef MHELP_OLD
            // lpToolCbo
			GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpCbo,MAX_RS_SIZE,bContinue,bError);
			if((!bError)&&(!bContinue))
			{	if(lpCbo[0]==RS_PARAM)
				{	iTmp=mHpPak_GetIntFromStr(lpCbo,MAX_RS_SIZE,1);
					if(!DidItSelected(iTmp)) bSel=FALSE;
					else
					{	GetStrEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0,lpCbo,MAX_RS_SIZE);
						if(iTmp==NO) return TRUE; // Error
					}
				}
			}
			else return TRUE;
#else
            ////// TODO: Be able to remove lines below (state always in HTML)
			GetSelectedString(hWndActive,GetmHpVarStruct()->mHpFileName,lpCbo,MAX_RS_SIZE,bContinue,bError);
			if((bError)||(bContinue))
			    return TRUE; // Error
            //////

            // Check if selected
            if(!IsHtmlCustSelected(strPostData,iCustIdx))
                bSel=FALSE;
            else
            {   // Get HTML user setting
                if(!GetHtmlCustValue(lpCbo,
                                     MAX_RS_SIZE,
                                     strPostData,
                                     CBOSELECT_KEY,
                                     iCustIdx))
                {   return TRUE; // Error: No state parameter
                }
            }
            // Increase number of custom task
            iCustIdx++;
#endif
			ToolTask=new CToolCboTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,(UINT)iCmdID,lpCbo);
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	// MenuSelID
			mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iMnuSel,lpTmp,MAX_RS_SIZE,chNext,FALSE);
			if(iMnuSel==NO) return TRUE; // Error
			ToolTask=new CToolDrDwTask(lpMsg,iCur,iCnt,iOpt,lpctrl,(UINT)iCmdID,(UINT)iMnuSel);
			break;
		}
	}
	if(bSel) GetmHpVarStruct()->TaskList->AddCellule(ToolTask);
	else delete(ToolTask);
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// ToolBar_WriteEvent
//---------------------------------------------------//
void ToolBar_WriteEvent(HWND hWndActive,int &Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
                        BOOL bFolderParam,char* lpCurExe,int &IndexParam,BOOL &bLnkFirst,BOOL &bRes)
{	static int iLnkLag=0;
	static int iLnkCons=0;
	static int iLnkIndex=0;
	static int iRowSel=0;
	static BOOL bNoCustom=FALSE;
	switch(EvntObj->GetmHpTask(Index,FALSE)->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))||
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE)))
			{	bRes=FALSE;}
			break;
		}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))||
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE)))
			{	bRes=FALSE;}
			else if((bFolderParam)&&(EvntObj->GetCustomVisible(Index)))
			{	if((!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
				   (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
				{	bRes=FALSE;}
				IndexParam++;
			}
			else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrState(Index,TRUE),FALSE)) // '¤'
			{	bRes=FALSE;}
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(bLnkFirst)
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
					   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE))||
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
					{	if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))||
						   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE)))
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
					   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE))||
					   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
					   (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
					{	bRes=FALSE;}
				}
				else
				{	iRowSel=EvntObj->GetmHpWrite(Index,FALSE)->GetRadioRow();
					if(iRowSel==1)
					{	if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))||
						   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE)))
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
				{	if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))||
					   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE)))
					{	bRes=FALSE;}
				}
				else
				{	bNoCustom=TRUE;
					iLnkLag=EvntObj->GetmHpWrite(Index,FALSE)->GetLinked()-1;
					EvntObj->GetmHpWrite(Index,FALSE)->SetLinked(NO);
				}
			}
			else if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))||
					(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE)))
			{	bRes=FALSE;}
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))||
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE)))
			{	bRes=FALSE;}
			else if((bFolderParam)&&(EvntObj->GetCustomVisible(Index)))
			{	if((!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
				   (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
				{	bRes=FALSE;}
				IndexParam++;
			}
			else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrState(Index,TRUE),TRUE))
			{	bRes=FALSE;}
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	if((!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))||
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetCommandID(),NULL,FALSE)))
			{	bRes=FALSE;}
			else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,(int)EvntObj->GetmHpTask(Index,FALSE)->GetMenuSelect(),NULL,FALSE))
			{	bRes=FALSE;}
			break;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------//
// ToolBar_WriteParam
//---------------------------------------------------//
BOOL ToolBar_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	switch(EvntObj->GetmHpTask(Index,FALSE)->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
			{	return FALSE;}
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(EvntObj->GetmHpWrite(Index,FALSE)->GetLinked()!=NO)
			{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),FALSE))|| // '§'
				   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
				   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
				   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
				{	return FALSE;}
			}
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
			{	return FALSE;}
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
	return TRUE;
}			

//*******************************************************************************************************************KEYS//
// ToolBar_ReadEventKeys
//---------------------------------------------------//
void ToolBar_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList,int idynID)
{	CmHpTask* ReadTask;
	CmHpWrite* ReadWrite;
	CmHpParam* ReadParam;
	switch(idynID)
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	ReadTask=new CToolBtnTask();
			ReadTask->ReadTaskKey(Index);
			if(EvntObj!=NULL) EvntObj->AddEvent(ReadTask,NULL,NULL);
			else TaskList->AddCellule(ReadTask);
			break;
		}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	ReadTask=new CToolChckTask();
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
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	ReadTask=new CToolRadTask();
			ReadTask->ReadTaskKey(Index);
			if(EvntObj!=NULL)
			{	ReadWrite=new CToolRadWrite();
				ReadParam=new CmHpParam();
				ReadWrite->ReadEventKey(Index);
				if(!ReadParam->ReadParamKey(Index))
				{	delete(ReadParam);
					ReadParam=NULL;
				}
				EvntObj->AddEvent(ReadTask,ReadWrite,ReadParam);
			}
			else TaskList->AddCellule(ReadTask);
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	ReadTask=new CToolCboTask();
			ReadTask->ReadTaskKey(Index);
			if(EvntObj!=NULL)
			{	ReadWrite=new CToolCboWrite();
				ReadParam=new CmHpParam();
				ReadWrite->ReadEventKey(Index);
				if(!ReadParam->ReadParamKey(Index))
				{	delete(ReadParam);
					ReadParam=NULL;
				}
				EvntObj->AddEvent(ReadTask,ReadWrite,ReadParam);
			}
			else TaskList->AddCellule(ReadTask);
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	ReadTask=new CToolDrDwTask();
			ReadTask->ReadTaskKey(Index);
			if(EvntObj!=NULL) EvntObj->AddEvent(ReadTask,NULL,NULL);
			else TaskList->AddCellule(ReadTask);
			break;
		}
	}
}

//********************************************************************************************************************ADD//
// ToolBar_AddEvent
//---------------------------------------------------//
void ToolBar_AddEvent(HWND hWndActive,HWND hWndToolBar,UINT iTaskID,int iCurWnd,int iCntWnd,BOOL bWrite,int pdynId)
{	char lpCboSel[MAX_RS_SIZE]={0};
	int lpctrl[MAX_PART_TREE]={0};
	int iSel;
	CmHpTask* Item;
	CmHpWrite* wItem;
	switch(pdynId)
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	//AddToolCtrlID(hWndActive,hWndToolBar,lpctrl,MAX_PART_TREE);
			mHpPak_EmptyTab(lpctrl,MAX_PART_TREE);
			mHpPak_AddIndex(lpctrl,(int)GetWindowLongPtr(hWndToolBar,GWLP_ID));
			Item=new CToolBtnTask(NULL,iCurWnd,iCntWnd,0,lpctrl,iTaskID);
			Item->WriteTaskKey();
			GetmHpVarStruct()->EventList->AddEvent(Item,NULL,NULL);
			break;
		}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	//AddToolCtrlID(hWndActive,hWndToolBar,lpctrl,MAX_PART_TREE);
			mHpPak_EmptyTab(lpctrl,MAX_PART_TREE);
			mHpPak_AddIndex(lpctrl,(int)GetWindowLongPtr(hWndToolBar,GWLP_ID));
			if((SendMessage(hWndToolBar,TB_GETSTATE,(WPARAM)iTaskID,0)&TBSTATE_CHECKED)==TBSTATE_CHECKED)
			{	Item=new CToolChckTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,iTaskID,1);}
			else Item=new CToolChckTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,iTaskID,0);
			if(bWrite)
			{	Item->WriteTaskKey();
				GetmHpVarStruct()->EventList->AddEvent(Item,NULL,NULL);
			}
			else
			{	RemoveLastKeyPos();
				Item->WriteTaskKey();
				GetmHpVarStruct()->EventList->ReplaceEvent(Item,NULL);
			}
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	//AddToolCtrlID(hWndActive,hWndToolBar,lpctrl,MAX_PART_TREE);
			mHpPak_EmptyTab(lpctrl,MAX_PART_TREE);
			mHpPak_AddIndex(lpctrl,(int)GetWindowLongPtr(hWndToolBar,GWLP_ID));
			Item=new CToolRadTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,iTaskID);
			wItem=new CToolRadWrite();
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
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	// hWndActive -> ToolBar, hWndToolBar -> hWndCbo
			//AddToolCtrlID(GetActiveWindow(),hWndActive,lpctrl,MAX_PART_TREE);
			mHpPak_EmptyTab(lpctrl,MAX_PART_TREE);
			mHpPak_AddIndex(lpctrl,(int)GetWindowLongPtr(hWndActive,GWLP_ID));
			iSel=(int)SendMessage(hWndToolBar,CB_GETCURSEL,0,0);
			if(iSel==CB_ERR) GetWindowText(hWndToolBar,lpCboSel,MAX_RS_SIZE);
			else SendMessage(hWndToolBar,CB_GETLBTEXT,(WPARAM)iSel,(LPARAM)lpCboSel);
			Item=new CToolCboTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,iTaskID,lpCboSel);
			if(IsWndStyle(hWndToolBar,CBS_DROPDOWNLIST)==CBS_DROPDOWNLIST)
			{	wItem=new CToolCboWrite(1);}
			else wItem=new CToolCboWrite(0);
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
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	if(!ToolBar_IsItDropDown(GetmHpVarStruct()->EventList))
			{	//AddToolCtrlID(hWndActive,hWndToolBar,lpctrl,MAX_PART_TREE);
				mHpPak_EmptyTab(lpctrl,MAX_PART_TREE);
				mHpPak_AddIndex(lpctrl,(int)GetWindowLongPtr(hWndToolBar,GWLP_ID));
				Item=new CToolDrDwTask(NULL,iCurWnd,iCntWnd,0,lpctrl,iTaskID,0);
				Item->WriteTaskKey();
				GetmHpVarStruct()->EventList->AddEvent(Item,NULL,NULL);
			}
			else
			{	iSel=GetmHpVarStruct()->EventList->GetEventCount()-2;
				RemoveLastKeyPos();
				GetmHpVarStruct()->EventList->GetCellFromPos(iSel,FALSE)->mHpTask->SetMenuSelect(iTaskID);
				GetmHpVarStruct()->EventList->GetCellFromPos(iSel,FALSE)->mHpTask->WriteTaskKey();
			}
			break;
		}
	}
}

//*************************************************************************************************************PROPERTIES//
// ToolBar_IsClassName
//---------------------------------------------------//
int ToolBar_IsClassName(HWND hCtrlWnd,char* CtrlClass,int ClassSize,UINT CommandID)
{	if((!strncmp(CtrlClass,CPP_TASK_TOOLBAR,ClassSize))||
	   (!strncmp(CtrlClass,VB_TASK_TOOLBAR_A,ClassSize))||
	   (!strncmp(CtrlClass,VB_TASK_TOOLBAR_B,ClassSize))) //||
       //(!strncmp(CtrlClass,DOTNET_TASK_TOOLBAR,sizeof(DOTNET_TASK_TOOLBAR)-1)))
	{	if(hCtrlWnd==NULL) return 17; // ComboBox
		return GetToolbarStyle(hCtrlWnd,CommandID);
	}
	return 0;
}

//---------------------------------------------------//
// ToolBar_IsCustom
//---------------------------------------------------//
void ToolBar_IsCustom(CEventCell* EvntCell,BOOL &bcustom,int &LastID)
{	switch(EvntCell->mHpTask->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	LastID=NO;
			break;
		}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	bcustom=TRUE;
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(EvntCell->mHpWrite->GetLinked()!=NO) bcustom=TRUE;
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	bcustom=TRUE;
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	LastID=NO;
			break;
		}
	}
}

//---------------------------------------------------//
// ToolBar_StrState
//---------------------------------------------------//
void ToolBar_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
{	switch(pCurCell->mHpTask->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_NO_STATE,lpState,StateSize);
			break;
		}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
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
					{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHANGED_STATE,lpState,StateSize);
						break;
					}
				}
			}
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(pWriteOp) _snprintf(lpState,StateSize,"%d",pCurCell->mHpWrite->GetRadioRow());
			else
			{	if(pCurCell->mHpWrite->GetLinked()!=NO)
				{	_snprintf(lpState,StateSize,"%d°",pCurCell->mHpWrite->GetRadioRow());
					strncat(lpState," radiobox",9);
				}
				else LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_NO_STATE,lpState,StateSize);
			}
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	strncpy(lpState,pCurCell->mHpTask->GetMultiIndex(NO),StateSize);
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_NO_STATE,lpState,StateSize);
			break;
		}
	}
}

//---------------------------------------------------//
// ToolBar_ParamType
//---------------------------------------------------//
int ToolBar_ParamType(int pdynId)
{	switch(pdynId)
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	return 3; // B
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	return 0; // I
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	return 4; // S
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
	return NO;
}

//-----------------------------------------------------//
// ToolBar_DefaultBound
//-----------------------------------------------------//
void ToolBar_DefaultBound(CEventCell* pCurEvent,char* pStrBound,int BoundSize)
{	char lpChar[100]={0};
	switch(pCurEvent->mHpTask->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	strncpy(pStrBound,CHECKBOX_LIMITE,BoundSize);
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(pCurEvent->mHpWrite->GetLinked()!=NO)
			{	_snprintf(pStrBound,BoundSize,"[1;%d]",pCurEvent->mHpWrite->GetLinked());}
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	strncpy(pStrBound,STRING_LIMITE,BoundSize);
			LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHARACTERS,lpChar,100);
			strncat(pStrBound,lpChar,mHpPak_FindStrLenght(lpChar,100));
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
}

//-----------------------------------------------------//
// ToolBar_IsCustomized
//-----------------------------------------------------//
BOOL ToolBar_IsCustomized(CEventCell* pCurEvnt,int Indice,BOOL pCustom)
{	switch(pCurEvnt->mHpTask->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	return FALSE;
		}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(pCurEvnt->mHpWrite->GetLinked()==NO)
			{	if(((GetmHpVarStruct()->EventList->GetCellFromPos((Indice+1),FALSE)!=NULL)&&
					(GetmHpVarStruct()->EventList->GetCellFromPos((Indice+1),FALSE)->mHpTask->GetmHpID()==16)&&
					(GetmHpVarStruct()->EventList->GetCellFromPos((Indice+1),FALSE)->mHpWrite->GetLinked()==NO))||
				   ((GetmHpVarStruct()->EventList->GetCellFromPos((Indice-1),FALSE)!=NULL)&&
					(GetmHpVarStruct()->EventList->GetCellFromPos((Indice-1),FALSE)->mHpTask->GetmHpID()==16)&&
					(GetmHpVarStruct()->EventList->GetCellFromPos((Indice-1),FALSE)->mHpWrite->GetLinked()==NO)))
				{	if(!pCustom) return TRUE;
				}
				return FALSE;
			}
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	return FALSE;
		}
	}
	return TRUE;
}

//-----------------------------------------------------//
// ToolBar_IsNoStateMsg
//-----------------------------------------------------//
BOOL ToolBar_IsNoStateMsg(CEventCell* pCurEvnt)
{	switch(pCurEvnt->mHpTask->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(pCurEvnt->mHpWrite->GetCtrlStyle()==1) return TRUE;
            break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
	return FALSE;
}

//-----------------------------------------------------//
// ToolBar_GetHtmlTag
//-----------------------------------------------------//
#define TOOLBAR_CHECKBOX_TAGS   "\t<input type=\"radio\" name=\"%d-%s\" value=\"1\"%s/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font><br>\r\n" \
                                "\t<input type=\"radio\" name=\"%d-%s\" value=\"0\"%s/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font><br>\r\n" \
                                "\t<input type=\"radio\" name=\"%d-%s\" value=\"2\"%s/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">%s</font><br>\r\n"

#define TOOLBAR_COMBO_TAGS      "\t<input type=\"text\" name=\"%d-%s\" maxlength=\"100\" value=\"%s\"/>\r\n"

void ToolBar_GetHtmlTag(CEventCell* pCurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{	switch(pCurCell->mHpTask->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
        {
            // Get state label
            char lpChecked[64];
            LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_ACTIVE,lpChecked,64);
            char lpUnchecked[64];
            LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_INACTIVE,lpUnchecked,64);
            char lpIndeterminate[64];
            LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CHANGED_STATE,lpIndeterminate,64);

            // Return HTML tags
            _snprintf(lpHtmlTag,
                      iMaxSize,
                      TOOLBAR_CHECKBOX_TAGS,
                      iCustIndex,
                      CHKSTATE_KEY,
                      (pCurCell->mHpTask->GetBtnState()==1)? " checked":"", // BST_CHECKED
                      lpChecked,
                      iCustIndex,
                      CHKSTATE_KEY,
                      (pCurCell->mHpTask->GetBtnState()==0)? " checked":"", // BST_UNCHECKED
                      lpUnchecked,
                      iCustIndex,
                      CHKSTATE_KEY,
                      (pCurCell->mHpTask->GetBtnState()==2)? " checked":"", // BST_INDETERMINATE (or change state)
                      lpIndeterminate);
            break;
        }
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{
            _snprintf(lpHtmlTag,
                      iMaxSize,
                      TOOLBAR_COMBO_TAGS,
                      iCustIndex,
                      CBOSELECT_KEY,
                      pCurCell->mHpTask->GetMultiIndex(NO));
            break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
}

//***************************************************************************************************************CFGTOOLS//
// ToolBar_ShowCfgTools
//---------------------------------------------------//
void ToolBar_ShowCfgTools(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndState=GetDlgItem(hWndActive,IDC_STATIC_STATE);
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	HWND hWndChkActive=GetDlgItem(hWndActive,IDC_CHECK_ACTIVE);
	HWND hWndChkInactive=GetDlgItem(hWndActive,IDC_CHECK_INACTIVE);
	HWND hWndChkChgInt=GetDlgItem(hWndActive,IDC_CHECK_CHANGEINT);
	HWND hWndLinkItm=GetDlgItem(hWndActive,IDC_LINK_ITEM);
	HWND hWndUnlinkItm=GetDlgItem(hWndActive,IDC_UNLINK_ITEM);
	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	MoveWindow(hWndState,118,43,120,16,TRUE);
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
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	MoveWindow(hWndState,188,100,105,16,TRUE);
			MoveWindow(hWndEditState,188,118,107,20,TRUE);
			MoveWindow(hWndToolRadLst,14,40,163,134,TRUE);
			MoveWindow(hWndLinkItm,188,42,107,25,TRUE);
			MoveWindow(hWndUnlinkItm,188,72,107,25,TRUE);
			LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_CTRLROW,lpTmp,MAX_RS_SIZE);
			SetWindowText(hWndState,lpTmp);
			ShowWindow(hWndToolRadLst,SW_SHOW);
			ShowWindow(hWndLinkItm,SW_SHOW);
			ShowWindow(hWndUnlinkItm,SW_SHOW);
			ShowWindow(hWndState,SW_SHOW);
			ShowWindow(hWndEditState,SW_SHOW);
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	MoveWindow(hWndState,18,43,256,16,TRUE);
			MoveWindow(hWndEditState,18,59,276,20,TRUE);
			LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VALUE_STRSEL,lpTmp,MAX_RS_SIZE);
			SetWindowText(hWndState,lpTmp);
			ShowWindow(hWndState,SW_SHOW);
			ShowWindow(hWndEditState,SW_SHOW);
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
}

//---------------------------------------------------//
// ToolBar_HideCfgTools
//---------------------------------------------------//
void ToolBar_HideCfgTools(HWND hWndActive)
{	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_CHECK_ACTIVE),SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_CHECK_INACTIVE),SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_CHECK_CHANGEINT),SW_HIDE);
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	ShowWindow(hWndToolRadLst,SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_UNLINK_ITEM),SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
			ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
}

//---------------------------------------------------//
// ToolBar_SaveCfg
//---------------------------------------------------//
void ToolBar_SaveCfg(HWND hWndActive,BOOL &bErreur)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(SendMessage(GetDlgItem(hWndActive,IDC_CHECK_INACTIVE),BM_GETCHECK,0,0)==BST_CHECKED)
			{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetBtnState(0);}
			else if(SendMessage(GetDlgItem(hWndActive,IDC_CHECK_ACTIVE),BM_GETCHECK,0,0)==BST_CHECKED)
			{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetBtnState(1);}
			else
			{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetBtnState(2);}
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(bRadItmLinked)
			{	GetWindowText(hWndEditState,lpTmp,MAX_RS_SIZE);
				if((mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,0)>((int)ListView_GetSelectedCount(hWndToolRadLst)))||
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
			ApplyLnkCtrlChange(hWndActive,hWndToolRadLst,bRadItmLinked);
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	GetWindowText(hWndEditState,lpTmp,MAX_RS_SIZE);
			GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->InitMultiIndex();
			GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->AddMultiIndex(lpTmp);
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
}

//---------------------------------------------------//
// ToolBar_LoadCfg
//---------------------------------------------------//
void ToolBar_LoadCfg(HWND hWndActive,HWND pWndTabCtrl,HWND pWndDecrit,HWND pWndBound)
{	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	HWND hWndLinkItm=GetDlgItem(hWndActive,IDC_LINK_ITEM);
	HWND hWndUnlinkItm=GetDlgItem(hWndActive,IDC_UNLINK_ITEM);
	HWND hWndChkChgInt=GetDlgItem(hWndActive,IDC_CHECK_CHANGEINT);
	char szbuff[MAX_RS_SIZE]={0};
	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		{	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetBtnState())
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
				case 2: // Change state
				{	SendMessage(hWndChkChgInt,BM_SETCHECK,BST_CHECKED,0);
					SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),STRING_TWO);
					break;
				}
			}
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	hWndToolRadLst=GetDlgItem(hWndActive,IDC_LIST_RADIO);
			CreateTaskObjects();
			SetParent(hWndToolRadLst,pWndTabCtrl);
			SetEditStyleAsNumber(hWndEditState);
			FillLnkCtrlLst(hWndToolRadLst,16,IDI_ICON_TOOLBAR,IDS_VIEW_TOOLBAR_RAD);
			EnableWindow(hWndLinkItm,FALSE);
			if(GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetLinked()==NO)
			{	bRadItmLinked=FALSE;
				EnableWindow(hWndEditState,FALSE);
				EnableWindow(hWndUnlinkItm,FALSE);
				if(GetmHpVarStruct()->bCustomFolder)
				{	EnableWindow(pWndDecrit,FALSE);
					EnableWindow(pWndBound,FALSE);
				}
			}
			else
			{	bRadItmLinked=TRUE;
				_snprintf(szbuff,MAX_RS_SIZE,"%d",GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetRadioRow());
				SetWindowText(hWndEditState,szbuff);
				EnableWindow(hWndLinkItm,FALSE);
				EnableWindow(hWndToolRadLst,FALSE);
				if(GetmHpVarStruct()->bCustomFolder) EnableWindow(hWndUnlinkItm,FALSE);
			}
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	SetWindowText(hWndEditState,GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMultiIndex(NO));
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
}

//---------------------------------------------------//
// ToolBar_GetInfosCfg
//---------------------------------------------------//
void ToolBar_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_TOOLBAR,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_TOOLBAR,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_TOOLBAR,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_TOOLBAR,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TOOLBAR)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// ToolBar_ProcCfg
//---------------------------------------------------//
BOOL ToolBar_ProcCfg(HWND hWndActive,UINT Message,WPARAM wParam,LPARAM lParam,HWND hDecrit,HWND hLimite,HWND hWndChng)
{	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		{	break;}
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
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
			break;
		}
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	switch(Message)
			{	case WM_NOTIFY:
				{	if(((LPNMHDR)lParam)->idFrom==IDC_LIST_RADIO)
					{	if(((LPNMHDR)lParam)->code==LVN_ITEMCHANGED)
						{	if(ListView_GetSelectedCount(hWndToolRadLst)>=2) EnableWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),TRUE);
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
							LinkItems(hWndToolRadLst);
							SetWindowText(hWndEditState,STRING_ONE);
							EnableWindow(hWndEditState,TRUE);
							EnableWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),FALSE);
							EnableWindow(GetDlgItem(hWndActive,IDC_UNLINK_ITEM),TRUE);
							EnableWindow(hWndToolRadLst,FALSE);
							_snprintf(lpTmp,MAX_RS_SIZE,"[1;%d]",ListView_GetSelectedCount(hWndToolRadLst));
							SetWindowText(hLimite,lpTmp);
							if(GetmHpVarStruct()->bCustomFolder)
							{	EnableWindow(hLimite,TRUE);
								EnableWindow(hDecrit,TRUE);
							}
							bRadItmLinked=TRUE;
							EnableWindow(hWndChng,TRUE);
							return TRUE;
						}
						case IDC_UNLINK_ITEM:
						{	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
							GetmHpVarStruct()->CtrlLink->UnLinkItms();
							SetWindowText(hLimite,NULL_STRING);
							SetWindowText(hDecrit,NULL_STRING);
							SetWindowText(hWndEditState,NULL_STRING);
							EnableWindow(hWndToolRadLst,TRUE);
							EnableWindow(GetDlgItem(hWndActive,IDC_LINK_ITEM),TRUE);
							EnableWindow(GetDlgItem(hWndActive,IDC_UNLINK_ITEM),FALSE);
							EnableWindow(hWndEditState,FALSE);
							if(GetmHpVarStruct()->bCustomFolder)
							{	EnableWindow(hLimite,FALSE);
								EnableWindow(hDecrit,FALSE);
							}
							bRadItmLinked=FALSE;
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
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if(Message==WM_COMMAND)
			{	if(LOWORD(wParam)==IDC_EDIT_STATE)
				{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
					return TRUE;
				}
			}
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	break;}
	}
	return FALSE;
}

//****************************************************************************************************************PROCESS//
// ToolBar_CaptureMsg
//---------------------------------------------------//
void ToolBar_CaptureMsg(HWND hWndActive,HWND hWndCtrl,LPARAM lpParam,WPARAM wpParam,HWND hToolWnd,int iCurOp,
						CEventListe* EventObj,char* lpNameApp,UINT &pIdLast,int pCurwnd,int pCntwnd,int pmHpCtrlID,
						BOOL &pActionUser,int &pResCapt)
{	int iWriteRes=0;
	BOOL bWrite=FALSE;
	switch(pmHpCtrlID)
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	bWrite=TRUE;
			WriteImmediatTask(hWndActive,NULL,hWndCtrl,hToolWnd,NULL,iCurOp,pmHpCtrlID,(UINT)((int)LOWORD(wpParam)),
                              EventObj,NULL,NULL,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if((HIWORD(wpParam)==CBN_SELCHANGE)||(HIWORD(wpParam)==CBN_EDITCHANGE))
			{	bWrite=TRUE;
				WriteImmediatTask(hWndActive,NULL,hWndCtrl,hToolWnd,NULL,iCurOp,pmHpCtrlID,(UINT)((int)LOWORD(wpParam)),
                                  EventObj,NULL,NULL,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
			}
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	if(!ToolBar_IsItDropDown(EventObj))
			{	if(((LPNMTOOLBAR)lpParam)->hdr.code==TBN_DROPDOWN)
				{	bWrite=TRUE;
					WriteImmediatTask(hWndActive,NULL,hWndCtrl,hToolWnd,NULL,iCurOp,pmHpCtrlID,(UINT)(((LPNMTOOLBAR)lpParam)->iItem),
									  EventObj,NULL,NULL,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
				}
			}
			else
			{	if((HIWORD(wpParam)==0)&&(lpParam==NULL)) // Menu
				{	bWrite=TRUE;
					WriteImmediatTask(hWndActive,NULL,hWndCtrl,hToolWnd,NULL,iCurOp,pmHpCtrlID,(UINT)((int)LOWORD(wpParam)),
                                      EventObj,NULL,NULL,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
				}
			}
			break;
		}
	}
	if(bWrite)
	{	switch(iWriteRes)
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
// ToolBar_mHpProcess
//---------------------------------------------------//
void ToolBar_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	Button_mHpProcess(hWndActive,bChangeWnd,iCurrentWnd,iCountWnd);
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	Combo_mHpProcess(hWndActive,bChangeWnd,iCurrentWnd,iCountWnd);
			break;
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	if(bDropDownOp) Menu_mHpProcess(hWndActive,bChangeWnd,iCurrentWnd,iCountWnd);
			else switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
			{	case 2:
				{	if(GetmHpVarStruct()->imHpMode==1) 
					{	GetmHpVarStruct()->CourantTask->SetmHpStep(4);
						SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
						break;
					}
					GetmHpVarStruct()->bRefreshDDraw=TRUE;
					GetmHpVarStruct()->CursorType=1;
					SetTimer(hWndActive,ID_TIMER_DDRAW,100,(TIMERPROC)mHpTimerProc);
					break;
				}
				case 3:
				{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
					SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
					break;
				}
				case 4:
				{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
					GetmHpVarStruct()->bRefreshDDraw=TRUE;
					SetDDrawObj(hWndActive,0);
					SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
					break;
				}
				case 5:
				{	RECT rect;
					HWND hWndCtl=GetToolCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
					GetmHpVarStruct()->bRefreshDDraw=TRUE;
					EndObjects(hWndActive);
					DoInit(hWndActive);
					SendMessage(hWndCtl,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&rect);
					SetTimer(hWndActive,ID_TIMER_DELAI,500,(TIMERPROC)mHpTimerProc);
					SendMessage(hWndCtl,WM_LBUTTONDOWN,0,MAKELPARAM((rect.left+4),(rect.top+4)));
					SendMessage(hWndCtl,WM_LBUTTONUP,0,MAKELPARAM((rect.left+4),(rect.top+4)));
					break;
				}
				case 6:
				{	int iRes;
					GetmHpVarStruct()->CourantTask->SetmHpStep(1);
					KillTimer(hWndActive,ID_TIMER_WAIT);
					KillTimer(hWndActive,ID_TIMER_MAXDELAY);
					GetmHpVarStruct()->DelayLoop=0;
					GetmHpVarStruct()->bNewDepl=TRUE;
					iRes=(int)Menu_StartProc(hWndActive);
					bDropDownOp=TRUE;
					ProcessStartProcRes(hWndActive,iRes);
					break;
				}
				default: NULL;
			}
			break;
		}
	}
}

//---------------------------------------------------//
// ToolBar_StartProc
//---------------------------------------------------//
BOOL ToolBar_StartProc(HWND hWndActive)
{	RECT rect;
	RECT BtnRect;
	GetmHpVarStruct()->hWndCurCtrl=GetToolCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
	if((GetmHpVarStruct()->hWndCurCtrl==NULL)||(!IsControlReady(GetmHpVarStruct()->hWndCurCtrl))) return FALSE;
	PutToolBarVisible(GetmHpVarStruct()->hWndCurCtrl,rect);
	switch(GetmHpVarStruct()->CourantTask->GetmHpID())
	{	case 14: // Button ////////////////////////////////////////////////////////////////////////////////////////////////
		case 15: // Checkbox //////////////////////////////////////////////////////////////////////////////////////////////
		case 16: // Radiobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	if((SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETSTATE,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),0)&TBSTATE_ENABLED)==TBSTATE_ENABLED)
			{	SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&BtnRect);
				GetmHpVarStruct()->XnextPos=(int)(rect.left+BtnRect.right-5);
				GetmHpVarStruct()->YnextPos=(int)(rect.top+BtnRect.bottom-10);
				//if(IsWndStyle(hWndItem,CBRS_GRIPPER)==CBRS_GRIPPER) GetmHpVarStruct()->XnextPos+=7;
				return TRUE;
			}
			break;
		}
		case 17: // Combobox //////////////////////////////////////////////////////////////////////////////////////////////
		{	return Combo_StartProc(hWndActive);
		}
		case 18: // Button DropDown ///////////////////////////////////////////////////////////////////////////////////////
		{	bDropDownOp=FALSE;
			if((SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETSTATE,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),0)&TBSTATE_ENABLED)==TBSTATE_ENABLED)
			{	SendMessage(GetmHpVarStruct()->hWndCurCtrl,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&BtnRect);
				GetmHpVarStruct()->XnextPos=(int)(rect.left+BtnRect.right-5);
				GetmHpVarStruct()->YnextPos=(int)(rect.top+BtnRect.bottom-10);
				//if(IsWndStyle(hWndItem,CBRS_GRIPPER)==CBRS_GRIPPER) GetmHpVarStruct()->XnextPos+=7;
				return TRUE;
			}
		}
	}
	return FALSE;
}

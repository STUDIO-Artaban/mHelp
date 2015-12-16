// SliderCtrl.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Slider\SlidCtrl.h>

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
#define		CPP_TASK_SLIDER				"msctls_trackbar32"                     // C/C++ control
#define		VB_TASK_SLIDERA				"SliderWndClass"                        // VB control
#define		VB_TASK_SLIDERB				"Slider20WndClass"                      // VB control
#define     DOTNET_TASK_SLIDER          "WindowsForms10.msctls_trackbar32.app"  // DotNet control

//-----------------------------------------------------//
// KeyName
#define		SLIDER_KEY					"Slider"

//-----------------------------------------------------//
// ValueKeys
#define		SLIDERPOS_KEY				"SliderPos"
//-----------------------------------------------------//
#define		MINPOS_KEY					"MinPos"
#define		MAXPOS_KEY					"MaxPos"

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles
int TrackPos=0;						// Position courante du slider d'un trackbar
long LastTrackPos=0L;				// Dernière position du slider d'un trackbar (Mode==2)

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration
void NextTrackBarStep(HWND);
void PositionOnTrack(HWND);

//-----------------------------------------------------//
// NextTrackBarStep
//-----------------------------------------------------//
void NextTrackBarStep(HWND hWndActive)
{	static int EcartMove=0;
	static BOOL bSwap=FALSE;
	if(GetmHpVarStruct()->CourantTask->GetSliderPos()==TrackPos)
	{	PositionOnTrack(hWndActive);
		EcartMove=0;	
		bSwap=FALSE;
	}
	else
	{	if(!EcartMove)
		{	RECT rect;
			int PageSize;
			SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETCHANNELRECT,0,(LPARAM)&rect);
			PageSize=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETRANGEMAX,0,0);
			PageSize-=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETRANGEMIN,0,0);
			EcartMove=((int)(rect.right-rect.left))/PageSize;
		}
		if(GetmHpVarStruct()->CourantTask->GetSliderPos()>TrackPos)
		{	if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,TBS_VERT)!=TBS_VERT)
			{	if((EcartMove%2)!=0)
				{	if(bSwap)
					{	PositionOnTrack(hWndActive);
						bSwap=FALSE;
					}
					else
					{	GetmHpVarStruct()->XnextPos+=(EcartMove/2);
						bSwap=TRUE;
					}
				}
				else GetmHpVarStruct()->XnextPos+=(EcartMove/2);
			}
			else
			{	if((EcartMove%2)!=0)
				{	if(bSwap)
					{	PositionOnTrack(hWndActive);
						bSwap=FALSE;
					}
					else
					{	GetmHpVarStruct()->YnextPos+=(EcartMove/2);
						bSwap=TRUE;
					}
				}
				else GetmHpVarStruct()->YnextPos+=(EcartMove/2);
			}
		}
		else 
		{	if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,TBS_VERT)!=TBS_VERT)
			{	if((EcartMove%2)!=0)
				{	if(bSwap)
					{	PositionOnTrack(hWndActive);
						bSwap=FALSE;
					}
					else
					{	GetmHpVarStruct()->XnextPos-=(EcartMove/2);
						bSwap=TRUE;
					}
				}
				else GetmHpVarStruct()->XnextPos-=(EcartMove/2);
			}
			else
			{	if((EcartMove%2)!=0)
				{	if(bSwap)
					{	PositionOnTrack(hWndActive);
						bSwap=FALSE;
					}
					else
					{	GetmHpVarStruct()->YnextPos-=(EcartMove/2);
						bSwap=TRUE;
					}
				}
				else GetmHpVarStruct()->YnextPos-=(EcartMove/2);
			}
		}
	}
}

//-----------------------------------------------------//
// PositionOnTrack
//-----------------------------------------------------//
void PositionOnTrack(HWND hWndActive)
{	RECT rect,rect2;
	GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&rect);
	SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETTHUMBRECT,0,(LPARAM)&rect2);
	GetmHpVarStruct()->XnextPos=(int)((rect2.left+rect2.right)/2);
	GetmHpVarStruct()->XnextPos+=(int)(rect.left);
	GetmHpVarStruct()->YnextPos=(int)((rect2.bottom+rect2.top)/2);
	GetmHpVarStruct()->YnextPos+=(int)(rect.top);
	TrackPos=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETPOS,0,0);
	if(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,TBS_VERT)==TBS_VERT) GetmHpVarStruct()->YnextPos-=4;
}

//****************************************************************************************************************OBJECTS//
// Class CSliderTask
CSliderTask::CSliderTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpCtrl,char* lpCtrlName,int SliderPos)
 			:CmHpTask(5,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpCtrl,lpCtrlName)
{	iSliderPos=SliderPos;}

CSliderTask::CSliderTask()
 			:CmHpTask()
{}

BOOL CSliderTask::ReadTaskKey(int index)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	HKEY hSubSldTaskKey=NULL,hSldTaskKey=NULL;
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;
	CmHpTask::ReadCommonKey(index,TRUE,hSubSldTaskKey,hSldTaskKey);
	if((hSldTaskKey!=NULL)&&(RegOpenKeyEx(hSldTaskKey,SLIDER_KEY,0,KEY_READ,&hSldTaskKey)==ERROR_SUCCESS))
	{	// iSliderPos
		if(RegQueryValueEx(hSldTaskKey,SLIDERPOS_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iSliderPos,FALSE);		
		return TRUE;
	}
	return FALSE;
}

BOOL CSliderTask::WriteTaskKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubSldKey=NULL,hSldKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size;
	BOOL bAclRes;
	CmHpTask::WriteCommonKey(TRUE,hSubSldKey,hSldKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hSldKey!=NULL)
	{	// Slider
		if(RegCreateKeyEx(hSldKey,SLIDER_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// iSliderPos
		if(RegCreateKeyEx(hSldKey,SLIDER_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iSliderPos,FALSE);
		RegSetValueEx(hTmpKey,SLIDERPOS_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

int CSliderTask::GetSliderPos()
{	return iSliderPos;}

void CSliderTask::SetSliderPos(int iNewPos)
{	iSliderPos=iNewPos;}

// class CSliderWrite /////////////////////////////////////////////////////////////////////////////////////////////////////
CSliderWrite::CSliderWrite(int iMin,int iMax)
			 :CmHpWrite()
{	iMinPos=iMin;
	iMaxPos=iMax;
}

CSliderWrite::CSliderWrite()
			 :CmHpWrite()
{}

BOOL CSliderWrite::ReadEventKey(int index)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;	
	HKEY SubSliderKey=NULL,SliderKey=NULL;
	ReadmHpWriteKey(index,SubSliderKey,SliderKey);
	if(SliderKey!=NULL)
	{	// iMinPos
		if(RegQueryValueEx(SliderKey,MINPOS_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iMinPos,FALSE);		
		Size=MAX_KEY_SIZE;Type=REG_DWORD;
		// iMinPos
		if(RegQueryValueEx(SliderKey,MAXPOS_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iMaxPos,FALSE);		
		return TRUE;
	}
	return FALSE;
}

BOOL CSliderWrite::WriteEventKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubSldWrtKey=NULL,hSldWrtKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size;	
	BOOL bAclRes;
	CmHpWrite::WritemHpWriteKey(hSubSldWrtKey,hSldWrtKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hSldWrtKey!=NULL)
	{	// iMinPos
		if(RegCreateKeyEx(hSubSldWrtKey,MHPWRITE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iMinPos,FALSE);
		RegSetValueEx(hTmpKey,MINPOS_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		// iMaxPos
		if(RegCreateKeyEx(hSubSldWrtKey,MHPWRITE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iMaxPos,FALSE);
		RegSetValueEx(hTmpKey,MAXPOS_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

int CSliderWrite::GetMinPos()
{	return iMinPos;}

int CSliderWrite::GetMaxPos()
{	return iMaxPos;}

//****************************************************************************************************************MHPFILE//
// Slider_ReadEvent
//---------------------------------------------------//
BOOL Slider_ReadEvent(HWND hWndActive)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	int iCur,iCnt,iState,iTmp,iOpt;
	char chNext=END_RS_TASK;
	BOOL bSel=TRUE;
	CSliderTask *SliderTask;
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
	// iSliderPos // ** Param type: Booléan **
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
	if(iState==NO)
	{	if((lpTmp[0]==END_LIST)||(lpTmp[0]!=RS_PARAM)) return TRUE;
		iTmp=mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,1);
		if(!DidItSelected(iTmp)) bSel=FALSE;
		else
		{	iState=GetIntBoolEvoluParam(iTmp,GetmHpVarStruct()->RepeatLoop,0);
			if(iState==NO) return TRUE;
		}
	}
	if(bSel)
	{	if(*lpCtrlName!=0) SliderTask=new CSliderTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,NULL,lpCtrlName,iState);
        else SliderTask=new CSliderTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,NULL,iState);
		GetmHpVarStruct()->TaskList->AddCellule(SliderTask);
	}
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// Slider_WriteEvent
//---------------------------------------------------//
void Slider_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
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
// Slider_WriteParam
//---------------------------------------------------//
BOOL Slider_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
	   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	{	return FALSE;}
	return TRUE;
}			
	
//*******************************************************************************************************************KEYS//
// Slider_ReadEventKeys
//---------------------------------------------------//
void Slider_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	CmHpWrite* ReadWrite;
	CmHpParam* ReadParam;
	ReadTask=new CSliderTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL)
	{	ReadWrite=new CSliderWrite();
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
// Slider_AddEvent
//---------------------------------------------------//
void Slider_AddEvent(HWND hWndActive,char* lpCtrlName,HWND hWndSlider,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
	int iPos=(int)SendMessage(hWndSlider,TBM_GETPOS,0,0);
	if(*lpCtrlName!=0)
	    Item=new CSliderTask(NULL,NULL,iCurWnd,iCntWnd,0,NULL,lpCtrlName,iPos);
    else
    {   int lpctrl[MAX_PART_TREE]={0};
        AddmHpCtrlID(hWndActive,lpctrl,MAX_PART_TREE,(UINT)GetDlgCtrlID(hWndSlider));
        Item=new CSliderTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,NULL,iPos);
    }
	CmHpWrite* wItem;
	wItem=new CSliderWrite((int)SendMessage(hWndSlider,TBM_GETRANGEMIN,0,0),
						   (int)SendMessage(hWndSlider,TBM_GETRANGEMAX,0,0));
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
// Slider_IsClassName
//---------------------------------------------------//
BOOL Slider_IsClassName(char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_SLIDER,ClassSize))||(!strncmp(CtrlClass,VB_TASK_SLIDERA,ClassSize))||
	   (!strncmp(CtrlClass,VB_TASK_SLIDERB,ClassSize))||
       (!strncmp(CtrlClass,DOTNET_TASK_SLIDER,sizeof(DOTNET_TASK_SLIDER)-1)))
	    return TRUE;
	return FALSE;
}

//---------------------------------------------------//
// Slider_IsCustom
//---------------------------------------------------//
void Slider_IsCustom(BOOL &bcustom)
{	bcustom=TRUE;}

//---------------------------------------------------//
// Slider_StrState
//---------------------------------------------------//
void Slider_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
{	if(pWriteOp) _snprintf(lpState,StateSize,"%d",pCurCell->mHpTask->GetSliderPos());
	else _snprintf(lpState,StateSize,"%d%s",pCurCell->mHpTask->GetSliderPos(),"° position");
}

//---------------------------------------------------//
// Slider_ParamType
//---------------------------------------------------//
int Slider_ParamType()
{	return 0; // I
}

//---------------------------------------------------//
// Slider_DefaultBound
//---------------------------------------------------//
void Slider_DefaultBound(CEventCell* pCurEvent,char* pStrBound,int BoundSize)
{	_snprintf(pStrBound,BoundSize,"[%d;%d]",pCurEvent->mHpWrite->GetMinPos(),pCurEvent->mHpWrite->GetMaxPos());}

//-----------------------------------------------------//
// Slider_IsCustomized
//-----------------------------------------------------//
BOOL Slider_IsCustomized()
{	return TRUE;}

//-----------------------------------------------------//
// Slider_IsNoStateMsg
//-----------------------------------------------------//
BOOL Slider_IsNoStateMsg()
{	return TRUE;}

//-----------------------------------------------------//
// Slider_GetHtmlTag
//-----------------------------------------------------//
void Slider_GetHtmlTag(CEventCell*,char*,UINT,int)
{}

//***************************************************************************************************************CFGTOOLS//
// Slider_ShowCfgTools
//---------------------------------------------------//
void Slider_ShowCfgTools(HWND hWndActive)
{	char lpTmp[MAX_RS_SIZE]={0};
	HWND hWndState=GetDlgItem(hWndActive,IDC_STATIC_STATE);
	HWND hWndEditState=GetDlgItem(hWndActive,IDC_EDIT_STATE);
	HWND hWndSliderCfg=GetDlgItem(hWndActive,IDC_SLIDER_CFG);
	MoveWindow(hWndState,18,78,110,16,TRUE);
	MoveWindow(hWndEditState,18,94,110,20,TRUE);
	MoveWindow(hWndSliderCfg,11,43,290,32,TRUE);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_SLIDER_VALUE,lpTmp,MAX_RS_SIZE);
	SetWindowText(GetDlgItem(hWndActive,IDC_STATIC_STATE),lpTmp);
	EnableWindow(hWndEditState,FALSE);
	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_SHOW);
	ShowWindow(hWndEditState,SW_SHOW);
	ShowWindow(hWndSliderCfg,SW_SHOW);
}

//---------------------------------------------------//
// Slider_HideCfgTools
//---------------------------------------------------//
void Slider_HideCfgTools(HWND hWndActive)
{	ShowWindow(GetDlgItem(hWndActive,IDC_STATIC_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_EDIT_STATE),SW_HIDE);
	ShowWindow(GetDlgItem(hWndActive,IDC_SLIDER_CFG),SW_HIDE);
}

//---------------------------------------------------//
// Slider_SaveCfg
//---------------------------------------------------//
void Slider_SaveCfg(HWND hWndActive)
{	GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->SetSliderPos((int)SendMessage(GetDlgItem(hWndActive,IDC_SLIDER_CFG),TBM_GETPOS,0,0));
}

//---------------------------------------------------//
// Slider_LoadCfg
//---------------------------------------------------//
void Slider_LoadCfg(HWND hWndActive)
{	HWND hWndSliderCfg=GetDlgItem(hWndActive,IDC_SLIDER_CFG);
	char lptmp[MAX_RS_SIZE]={0};
	SendMessage(hWndSliderCfg,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMaxPos());
	SendMessage(hWndSliderCfg,TBM_SETRANGEMIN,(WPARAM)TRUE,(LPARAM)GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->iCtrlSel,FALSE)->GetMinPos());
	SendMessage(hWndSliderCfg,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetSliderPos());
	_snprintf(lptmp,MAX_RS_SIZE,"%d",GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetSliderPos());
	SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lptmp);
}

//---------------------------------------------------//
// Slider_GetInfosCfg
//---------------------------------------------------//
void Slider_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRC[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_SLIDER,lpRC,MAX_RS_SIZE);
	SetWindowText(hProd,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_SLIDER,lpRC,MAX_RS_SIZE);
	SetWindowText(hVers,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_SLIDER,lpRC,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRC);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_SLIDER,lpRC,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_SLIDER)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRC,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRC);
}

//---------------------------------------------------//
// Slider_ProcCfg
//---------------------------------------------------//
BOOL Slider_ProcCfg(HWND hWndActive,UINT Message,WPARAM wParam,LPARAM lParam,HWND hWndChng)
{	switch(Message)
	{	case WM_NOTIFY:
		{	switch(((LPNMHDR)lParam)->idFrom)
			{	case IDC_SLIDER_CFG:
				{	switch(((LPNMHDR)lParam)->code)
					{	case NM_RELEASEDCAPTURE:
						{	char lpValue[MAX_RS_SIZE]={0};
							_snprintf(lpValue,MAX_RS_SIZE,"%d",SendMessage(GetDlgItem(hWndActive,IDC_SLIDER_CFG),TBM_GETPOS,0,0));
							SetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lpValue);
							EnableWindow(hWndChng,TRUE);
							break;
						}
					}
					break;
				}
				default:
				{	return (BOOL)DefWindowProc(hWndActive,Message,wParam,lParam);}
			}
			break;
		}
		case WM_COMMAND:
		{	if(LOWORD(wParam)==IDC_EDIT_STATE)
			{	if(HIWORD(wParam)==EN_CHANGE) EnableWindow(hWndChng,TRUE);
				return TRUE;
			}
			break;
		}
		default:
		{	break;}
	}
	return FALSE;
}

//****************************************************************************************************************PROCESS//
// Slider_CaptureMsg
//---------------------------------------------------//
void Slider_CaptureMsg(HWND hWndActive,char* lpCtrlName,LPARAM lpParam,HWND hToolWnd,int iCurOp,CEventListe* EventObj,
					   char* lpNameApp,UINT &pIdLast,int pCurwnd,int pCntwnd,BOOL &pActionUser,int &pResCapt,
					   BOOL bScrollMsg)
{	int iWriteRes=0;
	if(!bScrollMsg)
	{	if(((LPNMHDR)lpParam)->code==NM_RELEASEDCAPTURE)
		{	WriteImmediatTask(hWndActive,lpCtrlName,((LPNMHDR)lpParam)->hwndFrom,hToolWnd,NULL,iCurOp,5,
                              (UINT)((LPNMHDR)lpParam)->idFrom,EventObj,NULL,NULL,lpNameApp,pIdLast,pCurwnd,
                              pCntwnd,iWriteRes);
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
	else
	{	WriteImmediatTask(hWndActive,lpCtrlName,(HWND)lpParam,hToolWnd,NULL,iCurOp,5,
                          GetDlgCtrlID((HWND)lpParam),EventObj,NULL,NULL,lpNameApp,pIdLast,
                          pCurwnd,pCntwnd,iWriteRes);
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
// Slider_mHpProcess
//---------------------------------------------------//
void Slider_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	static BOOL bReplace=FALSE;
	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2:
		{	if(GetmHpVarStruct()->imHpMode==1) 
			{	SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)GetmHpVarStruct()->CourantTask->GetSliderPos());
				ForceVHScroll(hWndActive,
                              SB_THUMBPOSITION,
                              (long)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETPOS,0,0),
                              (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,TBS_VERT)==TBS_VERT)? TRUE:FALSE);
				ForceNotifyMsg(hWndActive,NM_RELEASEDCAPTURE);
				GetmHpVarStruct()->CourantTask->SetmHpStep(6);
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
				break;
			}
			if(GetmHpVarStruct()->imHpMode==2)
			{	bReplace=TRUE;
				LastTrackPos=(long)(SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETPOS,0,0));
			}
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=0;
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	SetTimer(hWndActive,ID_TIMER_DELAI,200,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 4:
		{	GetmHpVarStruct()->CursorType=1;
			NextTrackBarStep(hWndActive);
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 5:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
			if(GetmHpVarStruct()->CourantTask->GetSliderPos()<TrackPos) TrackPos--;
			else TrackPos++;
			EndObjects(hWndActive);
			SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)TrackPos);
			ForceVHScroll(hWndActive,SB_THUMBPOSITION,
						  (long)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETPOS,0,0),
						  (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,TBS_VERT)==TBS_VERT)? TRUE:FALSE);
			ForceNotifyMsg(hWndActive,NM_RELEASEDCAPTURE);
			DoInit(hWndActive);
			NextTrackBarStep(hWndActive);
			if(GetmHpVarStruct()->CourantTask->GetSliderPos()!=TrackPos)
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(3);
				GetmHpVarStruct()->iSkipSnd=1;
			}
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 6:
		{	int iMin=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETRANGEMIN,0,0);
			int iMax=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETRANGEMAX,0,0);
			if(((iMin>GetmHpVarStruct()->CourantTask->GetSliderPos())||
			    (iMax<GetmHpVarStruct()->CourantTask->GetSliderPos()))&&
			    (GetmHpVarStruct()->CourantTask->GetmHpOption()>=10))
			{	ViewCtrlNotReadyMsg(hWndActive,4);
			}
			else SetTimer(hWndActive,ID_TIMER_DELAI,200,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 7:
		{	PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			if(bReplace)
			{	EndObjects(hWndActive);
				SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)LastTrackPos);
				DoInit(hWndActive);
				bReplace=FALSE;
			}
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

//---------------------------------------------------//
// Slider_StartProc
//---------------------------------------------------//
BOOL Slider_StartProc(HWND hWndActive)
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
	int Min=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETRANGEMIN,0,0);
	int Max=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,TBM_GETRANGEMAX,0,0);
	PositionOnTrack(hWndActive);
	if((TrackPos==GetmHpVarStruct()->CourantTask->GetSliderPos())||
	   (Min>GetmHpVarStruct()->CourantTask->GetSliderPos())||
	   (Max<GetmHpVarStruct()->CourantTask->GetSliderPos()))
	{	GetmHpVarStruct()->CourantTask->SetmHpStep(5);
		GetmHpVarStruct()->iSkipSnd=1;
	}
	return TRUE;
}

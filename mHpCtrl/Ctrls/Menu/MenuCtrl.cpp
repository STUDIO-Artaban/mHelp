// MenuCtrl.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <Menu\MenuCtrl.h>

//---------------------------------------------------//
#include "mHp2D.h"
#include "resource.h"

#ifndef  __MHPSOUND_H
 #include "mHpSnd.h"
#endif

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

//---------------------------------------------------//
// KeyName
#define		MENU_KEY					"Menu"

//---------------------------------------------------//
// ValueKeys
#define		MENUSTATE_KEY				"MenuState"
//
#define		TOOLBAR_X_MNU_LAG			7
#define		TOOLBAR_Y_MNU_LAG			6

//-----------------------------------------------------------------------------------------------------------------------//
// Variables
int iPopUpMenu=NO;						// Index PopUpMenu
int xDecalage;							// Decalage de la Xpos du DDrawCur for task after CMenu
int yDecalage;							// Decalage de la Ypos du DDrawCur for task after CPopUpMenu
BOOL bSkipAutorise;						// TRUE: l'aide peut continuer, FALSE: l'aide n'est pas prète (user skip)
int lpPopUpMenu[MAX_PART_TREE]={0};		// Liste des indexs de l'arborescence du Menu
int iMenuItmID;							// Item Menu Identifier == CourantTask->GetCtrlID() | CourantTask->GetMenuSelect()
HMENU hMenuCtrl;						// Menu en cours d'utilisation

//----------------------------------------------------------------------------------------------------------------GENERAL//
// Déclarations
void SetMenuDecalage(HWND);
void GetPopUpMenuRect(HWND,LPRECT,BOOL);
BOOL IsAllMenuItemsReady();
void GetMenuTaskPos(HMENU,int*,int&,UINT,BOOL&);
int GetMenuBoutonType(HWND,UINT);
void SetPopUpDecalage();

//----------------------------------------------------------------------------------------------------------------GENERAL//
// SetMenuDecalage
//-----------------------------------------------------//
void SetMenuDecalage(HWND hWndActive)
{	RECT Rct2,Rct3;
	int iTemp=iPopUpMenu;
	iPopUpMenu--;
	GetPopUpMenuRect(hWndActive,&Rct2,FALSE);
	xDecalage+=(int)(Rct2.right);
	if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////////////////
	{	// Horizontal - Haut
		yDecalage+=(int)(Rct2.top);
		xDecalage-=TOOLBAR_X_MNU_LAG;
		GetWindowRect(hWndActive,&Rct3);
		yDecalage-=(int)(Rct3.top);
		xDecalage-=(int)(Rct3.left);
		yDecalage-=TOOLBAR_Y_MNU_LAG;
		SendMessage(hWndActive,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&Rct3);
		if(Rct3.top>0) yDecalage-=3; // Vertical - Gauche
		// Horizontal - Bas
		// Vertical - Droit
	}
	else // Menu //////////////////////////////////////////////////////////////////////////////////////////////////////////
	{	iPopUpMenu=0;
		GetPopUpMenuRect(hWndActive,&Rct3,FALSE);
		yDecalage+=(int)(Rct2.top-Rct3.bottom)-1;
	}
	iPopUpMenu=iTemp;
}

//-----------------------------------------------------//
// GetPopUpMenuRect
//-----------------------------------------------------//
void GetPopUpMenuRect(HWND hWndActive,LPRECT pRect,BOOL bMenuBouton)
{	HMENU hMenuTmp=hMenuCtrl;
	int iPos=NO;
	for(int i=0;i<=iPopUpMenu;i++)
	{	if((!bMenuBouton)&&(!mHpPak_IsMenuItemReady(hMenuTmp,lpPopUpMenu[i],NO)))
		{	pRect->bottom=NO;
			return;
		}
		if(!GetMenuItemRect(hWndActive,
							hMenuTmp,
							lpPopUpMenu[i],
							pRect))
		{	pRect->bottom=NO;
			return;
		}
		pRect->left+=3;
		pRect->top+=2;
		pRect->right+=2;
		pRect->bottom+=1;
		if(lpPopUpMenu[i+1]!=NO)
		{	hMenuTmp=GetSubMenu(hMenuTmp,lpPopUpMenu[i]);}
		else if(bMenuBouton)
		{	iPos=NO;
			hMenuTmp=GetSubMenu(hMenuTmp,lpPopUpMenu[i]);
			for(int j=0;j<GetMenuItemCount(hMenuTmp);j++)
			{	if(((UINT)iMenuItmID)==GetMenuItemID(hMenuTmp,j)) iPos=j;
			}
			if((iPos!=NO)&&(!mHpPak_IsMenuItemReady(hMenuTmp,iPos,NO)))
			{	pRect->bottom=NO;
				return;
			}
			if(!GetMenuItemRect(hWndActive,hMenuTmp,iPos,pRect))
			{	pRect->bottom=NO;
				return;
			}
			pRect->left+=3;
			pRect->top+=2;
			pRect->right+=2;
			pRect->bottom+=1;
			return;
		}
	}
}

//-----------------------------------------------------//
// IsAllMenuItemsReady
//-----------------------------------------------------//
BOOL IsAllMenuItemsReady()
{	int iPopUp=0;
	HMENU hmenu=hMenuCtrl;
	if(iMenuItmID!=NO)
	{	while(lpPopUpMenu[iPopUp]!=NO)
		{	if(!mHpPak_IsMenuItemReady(hmenu,lpPopUpMenu[iPopUp],NO)) return FALSE;
			hmenu=GetSubMenu(hmenu,lpPopUpMenu[iPopUp]);
			iPopUp++;
		}
		if(!mHpPak_IsMenuItemReady(hmenu,NO,iMenuItmID)) return FALSE;
	}
	else if(!mHpPak_IsMenuItemReady(hmenu,NO,lpPopUpMenu[0])) return FALSE;
	return TRUE;
}

//-----------------------------------------------------//
// GetMenuTaskPos
//-----------------------------------------------------//
void GetMenuTaskPos(HMENU phMenu,int *iPopUpTab,int &Index,UINT piTaskID,BOOL &bFind)
{	for(int i=0;i<GetMenuItemCount(phMenu);i++)
	{	if(piTaskID==GetMenuItemID(phMenu,i))
		{	bFind=TRUE;
			if(!Index)
			{	iPopUpTab[Index]=(int)piTaskID;
				iPopUpTab[(Index+1)]=NO;
			}
			return; // OK
		}
		if(GetSubMenu(phMenu,i)!=NULL)
		{	iPopUpTab[Index]=i;
			Index++;
			iPopUpTab[Index]=NO;
			GetMenuTaskPos(GetSubMenu(phMenu,i),iPopUpTab,Index,piTaskID,bFind);
			if(bFind) return; // OK
			else 
			{	Index--;
				iPopUpTab[Index]=NO;
			}
		}
	}
}

//-----------------------------------------------------//
// GetMenuBoutonType
//-----------------------------------------------------//
/*int GetMenuBoutonType(HWND hWndActive,UINT pType)
{	MENUITEMINFO itemInfo;
	HMENU hMenu=GetMenu(hWndActive);
	int iTemp=0;
	itemInfo.fMask=MIIM_TYPE;
	itemInfo.cbSize=sizeof(itemInfo);
	if(GetmHpVarStruct()->CourantTask->GetMenuBouton()!=NO)
	{	while(CourantTask->GetPopupMenu(iTemp)!=NO)
		{	hMenu=GetSubMenu(hMenu,CourantTask->GetPopupMenu(iTemp));
			iTemp++;
		}
		if(GetMenuItemInfo(hMenu,CourantTask->GetMenuBouton(),FALSE,&itemInfo))
		{	if((itemInfo.fType&pType)==pType) return 1;
			return 0;
		}
	}
	else
	{	if(GetMenuItemInfo(hMenu,CourantTask->GetPopupMenu(0),FALSE,&itemInfo))
		{	if((itemInfo.fType&pType)==pType) return 1;
			return 0;
		}
	}
	return NO;
}*/

//-----------------------------------------------------//
// SetPopUpDecalage
//-----------------------------------------------------//
void SetPopUpDecalage()
{	if(lpPopUpMenu[(iPopUpMenu+1)]!=NO)
	{	if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////////////
		{	xDecalage-=2;
			yDecalage-=1;
			if(!iPopUpMenu)
			{	yDecalage-=5;
				xDecalage+=1;
			}
			else if(iPopUpMenu>=1) xDecalage+=1;
		}
		else // Menu //////////////////////////////////////////////////////////////////////////////////////////////////////
		{	if(iPopUpMenu>1)
			{	xDecalage-=2;
				yDecalage-=1;
			}
		}
	}
	else if((GetmHpVarStruct()->CourantTask->GetmHpID()==18)&&(!iPopUpMenu)) // ToolBar - DropDown ////////////////////////
	{	xDecalage-=1;
		yDecalage-=2;
	}
}

//*****************************************************************************************************************OBJECT//
// class CMenuTask
CMenuTask::CMenuTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,int* lpMenuID,int MenuState)
		  :CmHpTask(1,TaskMsg,StateMsg,CurWnd,CntWnd,Option,lpMenuID,NULL)
{	iMenuState=MenuState;}

CMenuTask::CMenuTask()
		  :CmHpTask()
{}

BOOL CMenuTask::ReadTaskKey(int index)
{	BYTE BtTab[MAX_KEY_SIZE]={0};
	HKEY hSubMnuTaskKey=NULL,hMnuTaskKey=NULL;
	DWORD Size=MAX_KEY_SIZE;
	DWORD Type=REG_DWORD;
	CmHpTask::ReadCommonKey(index,TRUE,hSubMnuTaskKey,hMnuTaskKey);
	if((hMnuTaskKey!=NULL)&&(RegOpenKeyEx(hMnuTaskKey,MENU_KEY,0,KEY_READ,&hMnuTaskKey)==ERROR_SUCCESS))
	{	// iMenuState
		if(RegQueryValueEx(hMnuTaskKey,MENUSTATE_KEY,NULL,&Type,BtTab,&Size)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(BtTab,iMenuState,FALSE);		
		return TRUE;
	}
	return FALSE;
}

BOOL CMenuTask::WriteTaskKey()
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES SecAtt;
	HKEY hTmpKey,hSubMenuKey=NULL,hMenuKey=NULL;
	BYTE bTab[MAX_KEY_SIZE]={0};
	int size;
	BOOL bAclRes;
	CmHpTask::WriteCommonKey(TRUE,hSubMenuKey,hMenuKey);
	mHpPak_GetSecurityAtt(SecAtt,bAclRes);
	if(hMenuKey!=NULL)
	{	// Menu
		if(RegCreateKeyEx(hMenuKey,MENU_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		// iMenuState
		if(RegCreateKeyEx(hMenuKey,MENU_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
						&hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromNumber(bTab,iMenuState,FALSE);
		RegSetValueEx(hTmpKey,MENUSTATE_KEY,0,REG_DWORD,bTab,size);
		RegCloseKey(hTmpKey);
		return TRUE;
	}
	return FALSE;
}

int CMenuTask::GetMenuState()
{	return iMenuState;}

//****************************************************************************************************************MHPFILE//
// Menu_ReadEvent
//-----------------------------------------------------//
BOOL Menu_ReadEvent(HWND hWndActive)
{	int lpctrl[MAX_PART_TREE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char lpMenuID[MAX_RS_SIZE]={0};
	char lpMsg[MAX_RS_SIZE]={0};
	char lpStateMsg[MAX_RS_SIZE]={0};
	int iCur,iCnt,iState=0,iTmp,iOpt;
	char chNext=END_RS_TASK;
	CMenuTask *MenuTask;	
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
	if(chNext==TREE_PARAM_GROUP)
	{	// iMenuState // ** Param Type: Booléan ** ????? //
		mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
		if(iState==NO) return TRUE;
	}
	MenuTask=new CMenuTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,lpctrl,iState);
	GetmHpVarStruct()->TaskList->AddCellule(MenuTask);
	return FALSE; // OK
}

//-----------------------------------------------------------------------------------------------------------------------//
// Menu_WriteEvent
//-----------------------------------------------------//
void Menu_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
                     char* lpCurExe,BOOL &bRes)
{	if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))
	{	bRes=FALSE;}
}

//-----------------------------------------------------------------------------------------------------------------------//
// Menu_WriteParam
//-----------------------------------------------------//
BOOL Menu_WriteParam()
{	return TRUE;}

//*******************************************************************************************************************KEYS//
// Menu_ReadEventKeys
//-----------------------------------------------------//
void Menu_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{	CmHpTask* ReadTask;
	ReadTask=new CMenuTask();
	ReadTask->ReadTaskKey(Index);
	if(EvntObj!=NULL) EvntObj->AddEvent(ReadTask,NULL,NULL);
	else TaskList->AddCellule(ReadTask);
}

//********************************************************************************************************************ADD//
// Menu_AddEvent
//-----------------------------------------------------//
void Menu_AddEvent(HWND hWndActive,UINT iTaskID,int iCurWnd,int iCntWnd)
{	int lpctrl[MAX_PART_TREE]={0};
	CmHpTask* Item;
	mHpPak_EmptyTab(lpctrl,MAX_PART_TREE);
	lpctrl[0]=(int)iTaskID;
	lpctrl[1]=NO;
	// NO -> Pour MenuType (non gérer)
	Item=new CMenuTask(NULL,NULL,iCurWnd,iCntWnd,0,lpctrl,0);
	Item->WriteTaskKey();
	GetmHpVarStruct()->EventList->AddEvent(Item,NULL,NULL);
}

//*************************************************************************************************************PROPERTIES//
// Menu_IsClassName
//-----------------------------------------------------//
BOOL Menu_IsClassName(HWND hWndActive,char* CtrlClass,int ClassSize,UINT iCtrlID)
{	int iPopUp[MAX_PART_TREE]={0};		
	int index=0;
	BOOL bFind=FALSE;
	if(!strncmp(CtrlClass,NULL_STRING,ClassSize))
	{	iPopUp[0]=NO;
		GetMenuTaskPos(GetMenu(hWndActive),iPopUp,index,iCtrlID,bFind);
		if(bFind) return TRUE;
		else
		{	index=0;
			mHpPak_EmptyTab(iPopUp,MAX_PART_TREE);
			GetMenuTaskPos(GetmHpVarStruct()->hCurMenu,iPopUp,index,iCtrlID,bFind);
			if(bFind) return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------//
// Menu_IsCustom
//-----------------------------------------------------//
void Menu_IsCustom(int &LastID)
{	LastID=NO;}

//-----------------------------------------------------//
// Menu_StrState
//-----------------------------------------------------//
void Menu_StrState(CEventCell* pCurCell,BOOL pWriteOp,char* lpState,int StateSize)
{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_NO_STATE,lpState,StateSize);}

//-----------------------------------------------------//
// Menu_ParamType
//-----------------------------------------------------//
int Menu_ParamType()
{	return NO;}

//-----------------------------------------------------//
// Menu_DefaultBound
//-----------------------------------------------------//
void Menu_DefaultBound()
{	NULL;}

//-----------------------------------------------------//
// Menu_IsCustomized
//-----------------------------------------------------//
BOOL Menu_IsCustomized(CEventCell* pCurEvnt,int Indice,BOOL pCustom)
{	return FALSE;}

//-----------------------------------------------------//
// Menu_IsNoStateMsg
//-----------------------------------------------------//
BOOL Menu_IsNoStateMsg()
{	return FALSE;}

//-----------------------------------------------------//
// Menu_GetHtmlTag
//-----------------------------------------------------//
void Menu_GetHtmlTag(CEventCell*,char*,UINT,int)
{}

//***************************************************************************************************************CFGTOOLS//
// Menu_GetInfosCfg
//-----------------------------------------------------//
void Menu_GetInfosCfg(HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{	char lpRc[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_PRODUCT_MENU,lpRc,MAX_RS_SIZE);
	SetWindowText(hProd,lpRc);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_VERSION_MENU,lpRc,MAX_RS_SIZE);
	SetWindowText(hVers,lpRc);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_DESCRIPTION_MENU,lpRc,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpRc);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTRL_MENU,lpRc,MAX_RS_SIZE);
	SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_MENU)),0);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpRc,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpRc);
}

//****************************************************************************************************************PROCESS//
// Menu_CaptureMsg
//---------------------------------------------------//
void Menu_CaptureMsg(HWND hWndActive,char* lpCtrlName,WPARAM wpParam,HWND hToolWnd,int iCurOp,CEventListe* EventObj,char* lpNameApp,
					 UINT &pIdLast,int pCurwnd,int pCntwnd,int pmHpCtrlID,BOOL &pActionUser,int &pResCapt)
{	int iWriteRes=0;
    WriteImmediatTask(hWndActive,NULL,NULL,hToolWnd,NULL,iCurOp,pmHpCtrlID,(UINT)((int)LOWORD(wpParam)),
                      EventObj,NULL,0,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
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

//-----------------------------------------------------//
// Menu_mHpProcess
//-----------------------------------------------------//
void Menu_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2:
		{	if(GetmHpVarStruct()->imHpMode==1) 
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(13);
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
				break;
			}
			GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			EndObjects(hWndActive);
			SetRealCursor(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur,FALSE);
			if(!DoInit(hWndActive))
                StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,100,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 3:
		{	if(!PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS)))
			{	SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);}
			else SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
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
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			EndObjects(hWndActive);
			if(!DoInit(hWndActive))
                StopmHelp(hWndActive,TRUE,TRUE);
			else if(iMenuItmID!=NO)
			{	bSkipAutorise=FALSE;
				SetTimer(hWndActive,ID_TIMER_MENU,100,(TIMERPROC)mHpTimerProc);
			}
			else
			{	GetmHpVarStruct()->CourantTask->SetmHpStep(13);
				GetmHpVarStruct()->iSkipSnd=1;
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 6:
		{	RECT Rct1;
			HWND hWind;
			GetmHpVarStruct()->bNewDepl=TRUE;
			SetDDrawObj(hWndActive,0);
			iPopUpMenu++;
			if((iPopUpMenu==1)&&(GetmHpVarStruct()->CourantTask->GetmHpID()!=18)) yDecalage=0;
			if(lpPopUpMenu[iPopUpMenu]!=NO)
			{	// Popupmenu
				if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////
				{	hWind=GetToolCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
				}
				else hWind=hWndActive; // Menu ////////////////////////////////////////////////////////////////////////////
				GetPopUpMenuRect(hWind,&Rct1,FALSE);
				if(Rct1.bottom!=NO)
				{	if((iPopUpMenu>1)||(GetmHpVarStruct()->CourantTask->GetmHpID()==18))
					{	SetMenuDecalage(hWind);
					}
					GetmHpVarStruct()->XnextPos=((int)(Rct1.right-40)+xDecalage);
					GetmHpVarStruct()->YnextPos=((int)(Rct1.bottom-7)+yDecalage);
					SetTimer(hWndActive,ID_TIMER_DDRAW,100,(TIMERPROC)mHpTimerProc);
				}
				else ViewCtrlNotReadyMsg(hWndActive,0);
			}
			else
			{	// Menubouton
				GetmHpVarStruct()->CourantTask->SetmHpStep(10);
				SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
			}
			break;
		}
		case 7:
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			EndObjects(hWndActive);
			SetRealCursor(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur,FALSE);
			if(!DoInit(hWndActive))
                StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 8:
		{	GetmHpVarStruct()->bNewDepl=TRUE;
			SetDDrawObj(hWndActive,0);
			SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 9:
		{	// Attend l'affichage du PopUpMenu -> BAD!!! Use WM_MENUSELECT!!!
			SetTimer(hWndActive,ID_TIMER_DELAI,350,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 10:
		{	EndObjects(hWndActive);
			if(DoInit(hWndActive))
            {   GetmHpVarStruct()->XnextPos+=55;
			    /* RUSTINE: améliore la précision *************/
			    SetPopUpDecalage();
			    /**********************************************/
			    GetmHpVarStruct()->CourantTask->SetmHpStep(5);
			    SetTimer(hWndActive,ID_TIMER_DDRAW,100,(TIMERPROC)mHpTimerProc);
            }
            else StopmHelp(hWndActive,TRUE,TRUE);
			break;
		}
		case 11: // MenuBouton
		{	RECT rct1;
			HWND hWndTmp;
			GetmHpVarStruct()->bNewDepl=TRUE;
			SetDDrawObj(hWndActive,0);			
			if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////////
			{	hWndTmp=GetToolCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
			}
			else hWndTmp=hWndActive; // Menu //////////////////////////////////////////////////////////////////////////////
			GetPopUpMenuRect(hWndTmp,&rct1,TRUE);			
			if(rct1.bottom!=NO)
			{	if((iPopUpMenu>1)||(GetmHpVarStruct()->CourantTask->GetmHpID()==18))
				{	SetMenuDecalage(hWndTmp);
					GetmHpVarStruct()->XnextPos=(((int)(rct1.right-14))+xDecalage);
				}
				else
				{	iPopUpMenu=0;
					GetmHpVarStruct()->XnextPos=(((int)(rct1.right-rct1.left-8))+xDecalage);
				}
				GetmHpVarStruct()->YnextPos=((int)(rct1.bottom-11)+yDecalage);
				SetTimer(hWndActive,ID_TIMER_DDRAW,100,(TIMERPROC)mHpTimerProc);
			}
			else ViewCtrlNotReadyMsg(hWndActive,0);
			break;
		}
		case 12:
		{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
			GetmHpVarStruct()->CursorType=1;
			EndObjects(hWndActive);
			SetRealCursor(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur,FALSE);
			if(!DoInit(hWndActive))
                StopmHelp(hWndActive,TRUE,TRUE);
			else SetTimer(hWndActive,ID_TIMER_DDRAW,200,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 13:
		{	if(!PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS)))
			{	SetTimer(hWndActive,ID_TIMER_DELAI,400,(TIMERPROC)mHpTimerProc);}
			else SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
			break;
		}
		case 14: // BUG: Changement d'applications sous Windows NT/2000/XP
		{	bSkipAutorise=TRUE;
            if(!RestoreBackBuffer(hWndActive)) // Restore back buffer now before WM_CANCELMODE message call
                                               // Impossible to know what be should be restored after!
                StopmHelp(hWndActive,TRUE,TRUE);
            else
            {   RemoveBackBuffer(); // No back buffer at next EndObjects() call
                SendMessage(hWndActive,WM_CANCELMODE,0,0);
			    SetTimer(hWndActive,ID_TIMER_DELAI,10,(TIMERPROC)mHpTimerProc);
            }
			break;
		}
		case 15:
		{	/*int iCheckType=GetMenuBoutonType(hWndActive,MFT_RADIOCHECK);
			if(iCheckType==NO) ViewCtrlNotReadyMsg(hWndActive);
			else
			{	if(!iCheckType) // Bouton
				{*/
            PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
			bSkipAutorise=TRUE;
			if((GetmHpVarStruct()->imHpMode==2)&&(GetmHpVarStruct()->TaskList->GetNextmHpID()==NO))
			{	GetmHpVarStruct()->bRefreshDDraw=TRUE;
				SetDDrawObj(hWndActive,0);
				SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
			}
			else	
			{	if(iMenuItmID!=NO) GetmHpVarStruct()->CourantIDCommand=iMenuItmID;
				else GetmHpVarStruct()->CourantIDCommand=lpPopUpMenu[0];
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
                if(bChangeWnd)
				{	mHpPak_ToolTipsActivate(TRUE);
					SendMessage(hWndActive,WM_COMMAND,MAKELONG(GetmHpVarStruct()->CourantIDCommand,0),0);
				}
				else
				{	SendMessage(hWndActive,WM_COMMAND,MAKELONG(GetmHpVarStruct()->CourantIDCommand,0),0);
					SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
				}
			}
				/*}
				else // Check/Radio
				{	if(GetmHpMode()!=2)
					{	SendDlgItemMessage(hWndActive,
									   CourantTask->GetItemIdent(),
									   CourantTask->GetMessageIdent(),
									   CourantTask->GetwParam(),
									   CourantTask->GetlParam());
					}
					bRefreshDDraw=TRUE;
					SetDDrawObj(hWndActive,0);
					SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
				}
			}*/
			break;
		}
		case 16:
		{	SetRealCursor((int)(GetmHpVarStruct()->xResolution/2),(int)(GetmHpVarStruct()->yResolution/2),FALSE);
			if(GetmHpVarStruct()->TaskList->NextCellule())
			{	GetmHpVarStruct()->bNewDepl=TRUE;
				SetDDrawObj(hWndActive,0);
				SetCursorPos((GetmHpVarStruct()->xResolution/2),(GetmHpVarStruct()->yResolution/2));
				GetmHpVarStruct()->CourantTask=GetmHpVarStruct()->TaskList->GetCurrentTask();
				if((GetmHpVarStruct()->CourantTask->GetmHpID()==1)||(!iMenuItmID))
				{	SendMessage(hWndActive,WM_CANCELMODE,0,0);
				}
                GetNextPos(hWndActive);
			}
			else NextRepeatAssistProc(hWndActive);
			break;
		}
		default: NULL;
	}
}

//-----------------------------------------------------//
// Menu_StartProc
//-----------------------------------------------------//
BOOL Menu_StartProc(HWND hWndActive)
{	RECT rect;
	BOOL bMenuFind=FALSE;
	HWND hWindow;
	int iPos=NO,i=0;
	iPopUpMenu=0;
	if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////////////////
	{	iMenuItmID=GetmHpVarStruct()->CourantTask->GetMenuSelect();
		hMenuCtrl=GetmHpVarStruct()->hCurMenu;
	}
	else // Menu //////////////////////////////////////////////////////////////////////////////////////////////////////////
	{	iMenuItmID=GetmHpVarStruct()->CourantTask->GetCtrlID();
		hMenuCtrl=GetMenu(hWndActive);
	}
	if(!iMenuItmID)
	{	GetmHpVarStruct()->CourantTask->SetmHpStep(15);
		return TRUE;
	}
	mHpPak_EmptyTab(lpPopUpMenu,MAX_PART_TREE);
	//hMenuCtrl=GetMenu(hWndActive);
	GetMenuTaskPos(hMenuCtrl,lpPopUpMenu,i,iMenuItmID,bMenuFind);
	if(!bMenuFind) return FALSE;
	/*if(!bMenuFind)
	{	i=0;
		mHpPak_EmptyTab(lpPopUpMenu,MAX_PART_TREE);
		GetMenuTaskPos(GetmHpVarStruct()->hCurMenu,lpPopUpMenu,i,iMenuItmID,bMenuFind);
		if(!bMenuFind) return FALSE;
		else hMenuCtrl=GetmHpVarStruct()->hCurMenu;
	}*/
	if(i==0) iMenuItmID=NO;
	// Bug du "GetMenuItemInfo"
	//if(!IsAllMenuItemsReady(hWndActive)) return FALSE;
	// PB si le menu est désactivé au cours de la 1° loop !!!
	if((!IsAllMenuItemsReady())&&(!GetmHpVarStruct()->RepeatLoop)) return FALSE;
	if(iMenuItmID!=NO)
	{	if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////////////
		{	hWindow=GetToolCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
		}
		else hWindow=hWndActive; // Menu //////////////////////////////////////////////////////////////////////////////////
		GetPopUpMenuRect(hWindow,&rect,FALSE);
		if(rect.bottom!=NO)
		{	if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////////
			{	// PopUp
				GetmHpVarStruct()->XnextPos=(int)(rect.right-33);
				GetmHpVarStruct()->YnextPos=(int)(rect.bottom-5);
				SendMessage(hWindow,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&rect);
				GetmHpVarStruct()->XnextPos+=(int)(rect.left-TOOLBAR_X_MNU_LAG);
				GetmHpVarStruct()->YnextPos+=(int)(rect.bottom-1);
				xDecalage=(int)(rect.left);
				yDecalage=(int)(rect.bottom+TOOLBAR_Y_MNU_LAG);
				GetmHpVarStruct()->CourantTask->SetmHpStep(6);
			}
			else // Menu //////////////////////////////////////////////////////////////////////////////////////////////////
			{	GetmHpVarStruct()->XnextPos=(int)(rect.right-10);
				GetmHpVarStruct()->YnextPos=(int)(rect.bottom-12);
				xDecalage=(int)(rect.left)-3;
				yDecalage=(int)(rect.bottom)-1;
			}
		}
		// Bug du "GetMenuItemInfo"
		//else return FALSE;
		// PB si le menu est désactivé au cours de la 1° loop !!!
		else if(!GetmHpVarStruct()->RepeatLoop) return FALSE;
	}
	else
	{	for(i=0;i<GetMenuItemCount(hMenuCtrl);i++)
		{	if(lpPopUpMenu[0]==((int)GetMenuItemID(hMenuCtrl,i))) iPos=i;
		}
		if((iPos!=NO)&&(mHpPak_IsMenuItemReady(hMenuCtrl,iPos,NO)))
		{	if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////////
			{	hWindow=GetToolCtrlWnd(hWndActive,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
			}
			else hWindow=hWndActive; // Menu //////////////////////////////////////////////////////////////////////////////
			if(!GetMenuItemRect(hWindow,hMenuCtrl,iPos,&rect))
			{	return FALSE;}
			GetmHpVarStruct()->XnextPos=(int)(rect.right-8);
			GetmHpVarStruct()->YnextPos=(int)(rect.bottom-11);
			if(GetmHpVarStruct()->CourantTask->GetmHpID()==18) // ToolBar - DropDown //////////////////////////////////////
			{	SendMessage(hWindow,TB_GETRECT,(WPARAM)GetmHpVarStruct()->CourantTask->GetCommandID(),(LPARAM)&rect);
				GetmHpVarStruct()->XnextPos+=(int)(rect.left-TOOLBAR_X_MNU_LAG+4);
				GetmHpVarStruct()->YnextPos+=(int)(rect.bottom+2);
			}
		}
		// Bug du "GetMenuItemInfo"
		//else return FALSE;
		// PB si le menu est désactivé au cours de la 1° loop !!!
		else if(!GetmHpVarStruct()->RepeatLoop) return FALSE;
	}
	return TRUE;
}

//**************************************************************************************************************TIMERPROC//
// Menu_TimerProc
//-----------------------------------------------------//
void Menu_TimerProc(HWND hWndActive)
{	// Attend l'affichage du submenu
	SetTimer(hWndActive,ID_TIMER_DDRAW,500,(TIMERPROC)mHpTimerProc);
	TrackPopupMenuEx(GetSubMenu(hMenuCtrl,lpPopUpMenu[0]),
					 TPM_VERTICAL,xDecalage,yDecalage,hWndActive,NULL);
	if(!bSkipAutorise) StopmHelp(hWndActive,TRUE,TRUE);
}

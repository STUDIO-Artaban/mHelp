// mHpComm.cpp
//

#include "stdafx.h"
#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

//-------------------------------//
#include <ComCtrl.h>

#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

//-------------------------------//
#include "mHpCtrl.h"
#include "CtrlMgmt.h"

//-------------------------------//
#include "mHp2D.h"

//-----------------------------------------------------//
// Variables
char lpCtrlMsg[MAX_RS_SIZE]={0};			// Control Not Ready Message text
HIMAGELIST hLnkCtrlImgLst=NULL;

//-----------------------------------------------------//
// Déclarations
HWND GetCommonCtrlWnd(HWND,int&,int*);
void AscensTimer(HWND);
void CreateTaskListe(HWND,int&,char*,int);
int ViewCtrlNotReadyDlg(HWND,char*,UINT);
LRESULT CALLBACK ViewMsgProc(HWND,UINT,WPARAM,LPARAM);
BOOL IsItNearCtrlSel(int,int);
BOOL NextStepWaitProc(HWND,BOOL);

//-----------------------------------------------------------------------------------------------------------------------//
// GetCommonCtrlWnd
//-----------------------------------------------------//
HWND GetCommonCtrlWnd(HWND hWndActive,int &index,int* lpCtrlID)
{	HWND hWndCtrl=GetWindow(hWndActive,GW_CHILD);
    while(hWndCtrl!=NULL)
    {   if(lpCtrlID[index]==(int)GetWindowLongPtr(hWndCtrl,GWLP_ID))
        {   if(lpCtrlID[++index]==NO)
                return hWndCtrl;
            HWND hWndPrev=hWndCtrl;
            hWndCtrl=GetCommonCtrlWnd(hWndCtrl,index,lpCtrlID);
            if(hWndCtrl!=NULL)
                return hWndCtrl;
            hWndCtrl=hWndPrev;
            index--;
        }
        hWndCtrl=GetWindow(hWndCtrl,GW_HWNDNEXT);
    }
    return NULL;
}

//-----------------------------------------------------//
// AscensTimer
//-----------------------------------------------------//
void AscensTimer(HWND hWndActive)
{	int TopIndex;
    int SelIndex;
    if((GetmHpVarStruct()->CourantTask->GetmHpID()==4)||(GetmHpVarStruct()->CourantTask->GetmHpID()==17))
	{	// Combo
		TopIndex=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETTOPINDEX,0,0);
		SelIndex=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,
                                  CB_FINDSTRING,
                                  (WPARAM)NO,
					              (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
	}
	else
	{	// Liste
		TopIndex=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETTOPINDEX,0,0);
		SelIndex=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,
                                  LB_FINDSTRING,
                                  (WPARAM)NO,
					              (LPARAM)GetmHpVarStruct()->CourantTask->GetMultiIndex(NO));
	}
	if(GetmHpVarStruct()->AscensSens==1)
	{	if(TopIndex==SelIndex)
		{	EndObjects(hWndActive);
			if(!DoInit(hWndActive))
			{	StopmHelp(hWndActive,TRUE,TRUE);
				return;
			}
			NextStepTaskProc(hWndActive,FALSE);
			return;
		}
		else TopIndex--;
	}
	else // AscensSens==2
	{	if((TopIndex+(GetmHpVarStruct()->ItemsVisu-1))==SelIndex)
		{	UpdateBackBuffer(FALSE);
			NextStepTaskProc(hWndActive,FALSE);
			return;
		}
		else TopIndex++;
	}
    if(!RestoreBackBuffer(hWndActive))
    {   StopmHelp(hWndActive,TRUE,TRUE);
        return;
    }
    UpdateBackBuffer(TRUE);
	if((GetmHpVarStruct()->CourantTask->GetmHpID()==4)||(GetmHpVarStruct()->CourantTask->GetmHpID()==17))
	{	SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_SETTOPINDEX,TopIndex,0);
	}
	else SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_SETTOPINDEX,TopIndex,0);
	RedrawWindow(GetmHpVarStruct()->hWndCurCtrl,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
    if(!UpdateFrame(hWndActive))
        StopmHelp(hWndActive,TRUE,TRUE);
	SetTimer(hWndActive,ID_TIMER_ASCENS,20,(TIMERPROC)mHpTimerProc);
}

//-----------------------------------------------------//
// CreateTaskListe
//-----------------------------------------------------//
void CreateTaskListe(HWND hWndActive,int &iResult,char* lpReadExe,int ExeSize)
{	CmHpTask* AppTask=CreateChangeExeObj(TRUE,NULL,GetmHpVarStruct()->lpCurApp,NULL);
	int iTaskLine=NO,mHpID=NO,itmp=0;
	BOOL bError=FALSE;
	char chNext=END_RS_TASK;
	iResult=0;
	AppTask->SetHwndExe(GetmHpVarStruct()->hHandleExeWnd);
	GetmHpVarStruct()->iLnkSel=NO;
	GetmHpVarStruct()->iLnkLag=NO;
	GetmHpVarStruct()->TaskList->EmptyListe();
	GetmHpVarStruct()->TaskList->AddCellule(AppTask);
    if(!mHpPak_GetControlList(hWndActive,GetmHpVarStruct()->mHpFileName))
	{	iResult=1;
		return;
	}
	if(!mHpPak_GetWindowList(hWndActive,GetmHpVarStruct()->mHpFileName,mHpPak_ProcWndLst))
	{	iResult=1;
		return;
	}
	mHpPak_FindEventLine(hWndActive,GetmHpVarStruct()->mHpFileName,GetmHpVarStruct()->IndexTab,iTaskLine);
	if(iTaskLine==NO)
	{	iResult=1;
		return;
	}
#ifndef MHELP_OLD
    while(mHpCtrl_IndexTask>0)
    {   iTaskLine++;
        mHpPak_FindEventLine(hWndActive,GetmHpVarStruct()->mHpFileName,GetmHpVarStruct()->IndexTab,iTaskLine);
        mHpCtrl_IndexTask--;
        if((!mHpCtrl_IndexTask)&&(iTaskLine==NO))
	    {	iResult=1;
		    return;
	    }
    }
#endif
	mHpPak_InitStr(lpReadExe,ExeSize);
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,itmp,lpReadExe,ExeSize,chNext,TRUE);
	if(itmp==NO)
	{	iResult=1;
		return;
	}
	//if(_strnicmp(AppTask->GetExeName(),lpReadExe,MAX_RS_SIZE)!=0) // In case where the 'ExeName' case sensitive has changed
    if(strncmp(AppTask->GetExeName(),lpReadExe,MAX_RS_SIZE)!=0)     // and can be different than in an existing mHelp file
	{	iResult=2;
		return;
	}
    ////// mHelp Control Management /////////////////////////////////////////
    UINT iPlatformID,iParamIdx=0;
    int iCtrlClass,iCtrlStyle,iCtrlMsgID;

	mHpPak_GetNextEvent(hWndActive,GetmHpVarStruct()->mHpFileName,mHpID,iPlatformID,iCtrlClass,iCtrlStyle,iCtrlMsgID,bError);
	while((!bError)&&(mHpID!=NO))
	{   // ReadEvent --------------------------------------------------------------**//
		bError=ComCtrl_ReadEvent(hWndActive,mHpID,iPlatformID,iCtrlClass,iCtrlStyle,iCtrlMsgID,iParamIdx);
		//-------------------------------------------------------------------------**//
		if(!bError)
            mHpPak_GetNextEvent(hWndActive,GetmHpVarStruct()->mHpFileName,mHpID,iPlatformID,iCtrlClass,iCtrlStyle,iCtrlMsgID,bError);
	}
	mHpPak_ClosemHpFile();
	if(bError)
	{	GetmHpVarStruct()->TaskList->EmptyListe();
		iResult=1;
		return;
	}
	if(!GetmHpVarStruct()->TaskList->NextCellule())
	{	iResult=1;
		return;
	}
	if(GetmHpVarStruct()->TaskList->IsChangeExeTask())
	{	if((!GetmHpVarStruct()->TaskList->WriteTaskListKey())||
		   (!mHpPak_WriteSettingsKey(GetmHpVarStruct()->imHpMode,GetmHpVarStruct()->imHpRepeat,GetmHpVarStruct()->RepeatLoop,GetmHpVarStruct()->imHpMouseSpeed,GetmHpVarStruct()->imHpDelay))||
		   (!mHpPak_ProcWndLst->WriteAppWndKey()))
		{	iResult=1;
			return;
		}
	}
}

//-----------------------------------------------------//
// ViewCtrlNotReadyDlg
//-----------------------------------------------------//
int ViewCtrlNotReadyDlg(HWND hWndActive,char* lpMessage,UINT uStringID)
{	FARPROC lpfnViewDlg;
	char lpBuff[255]={0};
	int iResult=0;
	mHpPak_InitStr(lpCtrlMsg,MAX_RS_SIZE);
	//if(uStringID==0) sprintf(lpCtrlMsg,lpMessage);
	if(lpMessage!=NULL) strncpy(lpCtrlMsg,lpMessage,MAX_RS_SIZE);
	else
	{	LoadString(GetmHpVarStruct()->hmHpCtrlInst,uStringID,lpBuff,255);
		strncpy(lpCtrlMsg,lpBuff,MAX_RS_SIZE);
	}
	// creation de la boite de dialogue MHP Help
    lpfnViewDlg=MakeProcInstance((FARPROC)ViewMsgProc,GetmHpVarStruct()->hmHpCtrlInst);
	iResult = (int)DialogBox(GetmHpVarStruct()->hmHpCtrlInst,
			                 MAKEINTRESOURCE(IDD_DLG_MESSAGE),
						     hWndActive,
						     (DLGPROC)ViewMsgProc);
    FreeProcInstance(lpfnViewDlg);
	// Test la création de la boite de dialogue
	return iResult;
}

//-----------------------------------------------------//
// ViewMsgProc()
//
//-----------------------------------------------------//
LRESULT CALLBACK ViewMsgProc(HWND hWndView,UINT Message,WPARAM wParam,LPARAM lParam)
{	static HWND hWndViewEdit=NULL;
	static HWND hWndContinue=NULL;
	switch(Message)
	{	case WM_INITDIALOG:
		{	char lpStop[MAX_RS_SIZE]={0};
			hWndViewEdit=GetDlgItem(hWndView,IDC_EDIT_VIEWMSG);
			hWndContinue=GetDlgItem(hWndView,IDC_CONTINUE_BTN);
			ShowWindow(hWndContinue,SW_HIDE);
			if(GetmHpVarStruct()->iCurOption==1)
			{	ShowWindow(hWndContinue,SW_SHOW);
				if(!GetmHpVarStruct()->bContinueProc)				
				{	EnableWindow(hWndContinue,FALSE);}
				LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STOP_PROCESS,lpStop,MAX_RS_SIZE);
				SetWindowText(GetDlgItem(hWndView,IDOK),lpStop);
			}
			SetWindowText(hWndViewEdit,lpCtrlMsg);
			SetFocus(hWndViewEdit);
			SendMessage(hWndViewEdit,EM_SETSEL,0,NO);
			break;
		}
		case WM_COMMAND:
		{	switch(LOWORD(wParam))
			{	case IDOK: return EndDialog(hWndView,IDOK);
				case IDC_CONTINUE_BTN:
				{	mHpPak_bLockChangeWnd=TRUE;
					return EndDialog(hWndView,IDC_CONTINUE_BTN);
				}
			}
		}
	}
	return FALSE;
}

//-----------------------------------------------------//
// IsItNearCtrlSel
//-----------------------------------------------------//
BOOL IsItNearCtrlSel(int Indice,int imHpId)
{	int i=Indice;
	int iCur=GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCurrentWnd();
	int iCnt=GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetCountWnd();
	while(i!=GetmHpVarStruct()->iCtrlSel)
	{	if((GetmHpVarStruct()->EventList->GetmHpTask(i,FALSE)->GetmHpID()!=imHpId)||
		   (GetmHpVarStruct()->EventList->GetmHpWrite(i,FALSE)->GetLinked()!=NO)||
		   (iCur!=GetmHpVarStruct()->EventList->GetmHpTask(i,FALSE)->GetCurrentWnd())||
		   (iCnt!=GetmHpVarStruct()->EventList->GetmHpTask(i,FALSE)->GetCountWnd()))
		{	return FALSE;}
		if(i>GetmHpVarStruct()->iCtrlSel) i--;
		else i++;
	}
	return TRUE;
}

//-----------------------------------------------------//
// NextStepWaitProc
//-----------------------------------------------------//
BOOL NextStepWaitProc(HWND hWndActive,BOOL bWaitForce)
{	static int LastDDrawBmp=GetmHpVarStruct()->DDrawBitmap;
	static int LastCursorType=GetmHpVarStruct()->CursorType;
	static BOOL bWait=FALSE;
	if(GetmHpVarStruct()->DDrawBitmap==4)
	{	if(GetmHpVarStruct()->CursorType==3)
		{	// StartProc -------------------------------------------------------------**//
			if((bWait)||(mHpPak_IsWaitingCursor())||(mHpCtrl_bChangeWnd)||
               ((GetmHpVarStruct()->bWaitCtrl)&&(ComCtrl_StartProc(hWndActive)!=1)))
			{	//--------------------------------------------------------------------**//
				bWait=FALSE;
				GetmHpVarStruct()->CursorType++;
				SetTimer(hWndActive,ID_TIMER_WAIT,100,(TIMERPROC)mHpTimerProc);
			}
			else
			{	EndObjects(hWndActive);
				DoInit(hWndActive);
				GetmHpVarStruct()->CursorType=LastCursorType;
				SetDDrawObj(hWndActive,LastDDrawBmp);
				SetTimer(hWndActive,ID_TIMER_WAIT,1,(TIMERPROC)mHpTimerProc);
			}
		}
		else
		{	if(GetmHpVarStruct()->CursorType==6) GetmHpVarStruct()->CursorType=3;
			else GetmHpVarStruct()->CursorType++;
			SetTimer(hWndActive,ID_TIMER_WAIT,100,(TIMERPROC)mHpTimerProc);
		}
		return TRUE;
	}
	else if((bWaitForce)||(mHpPak_IsWaitingCursor()))
	{	EndObjects(hWndActive);
		DoInit(hWndActive);
		LastDDrawBmp=GetmHpVarStruct()->DDrawBitmap;
		LastCursorType=GetmHpVarStruct()->CursorType;
		SetDDrawObj(hWndActive,4);
		GetmHpVarStruct()->CursorType=3;
		bWait=TRUE;
		SetTimer(hWndActive,ID_TIMER_WAIT,100,(TIMERPROC)mHpTimerProc);
		return TRUE;
	}
	return FALSE;
}

//****************************************************************************************************************GENERAL//
// GetCtrlWnd
//-----------------------------------------------------//
HWND GetCtrlWnd(HWND hWndActive,int* lpCtrlID)
{	HWND hWndCtrl=hWndActive;
	int index=0;
    bool bNoID=false;   // ID == 0 (eg lpCtrlID={0,0,0,1121,1,NO})
	while((lpCtrlID[index]!=NO)&&(hWndCtrl!=NULL))
	{	if(!lpCtrlID[index])
            bNoID=true;
        hWndCtrl=GetDlgItem(hWndCtrl,lpCtrlID[index]);
		index++;
    }
    if((!hWndCtrl)&&(bNoID)) // CFileDialog (ID == 0)
    {   int Idx=0;
        hWndCtrl=GetCommonCtrlWnd(hWndActive,Idx,GetmHpVarStruct()->CourantTask->GetmHpCtrlID());
    }
	return hWndCtrl;
}

//-----------------------------------------------------//
// GetToolCtrlWnd
//-----------------------------------------------------//
HWND GetToolCtrlWnd(HWND hWndActive,int* lpCtrlID)
{	/*long lID;
	int index=0;
	HWND hWndCtrl=GetWindow(hWndActive,GW_CHILD);
	while((lpCtrlID[index]!=NO)&&(hWndCtrl!=NULL))
	{	lID=(long)GetWindowLongPtr(hWndCtrl,GWLP_ID);
		while((lpCtrlID[index]!=((int)lID))&&(hWndCtrl!=NULL))
		{	hWndCtrl=GetWindow(hWndCtrl,GW_HWNDNEXT);
			lID=(long)GetWindowLongPtr(hWndCtrl,GWLP_ID);
		}
		index++;
		if(lpCtrlID[index]!=NO) hWndCtrl=GetWindow(hWndCtrl,GW_CHILD);
	}*/
	HWND hWndInCtrl,hWndCtrl=GetWindow(hWndActive,GW_CHILD);
	while(hWndCtrl!=NULL)
	{	int iID=(int)GetWindowLongPtr(hWndCtrl,GWLP_ID);
		char lpClss[MAX_RS_SIZE]={0};
        GetClassName(hWndCtrl,lpClss,MAX_RS_SIZE);
		if((lpCtrlID[0]==iID)&&
		   ((!strncmp(lpClss,CPP_TASK_TOOLBAR,MAX_RS_SIZE))||
		   (!strncmp(lpClss,VB_TASK_TOOLBAR_A,MAX_RS_SIZE))||
		   (!strncmp(lpClss,VB_TASK_TOOLBAR_B,MAX_RS_SIZE))))
		{	return hWndCtrl;
		}
		else
		{	hWndInCtrl=GetToolCtrlWnd(hWndCtrl,lpCtrlID);
			if(hWndInCtrl!=NULL) return hWndInCtrl;
			else hWndCtrl=GetWindow(hWndCtrl,GW_HWNDNEXT);
		}
	}
	return hWndCtrl;
}

//-----------------------------------------------------//
// ForceMessage
//-----------------------------------------------------//
void ForceMessage(HWND hWndActive,DWORD MsgID)
{	if(GetmHpVarStruct()->imHpMode!=2)
	{	if(GetmHpVarStruct()->CourantTask->GetmHpID()==17) // ToolBar - ComboBox //////////////////////////////////////////
		{	SendMessage(GetParent(GetmHpVarStruct()->hWndCurCtrl),
						WM_COMMAND,
						(WPARAM)(MAKELONG((WORD)GetmHpVarStruct()->CourantTask->GetCommandID(),MsgID)),
						(LPARAM)GetmHpVarStruct()->hWndCurCtrl);
		}
		else // Common ////////////////////////////////////////////////////////////////////////////////////////////////////
		{	SendMessage(GetParent(GetmHpVarStruct()->hWndCurCtrl),
						WM_COMMAND,
						(WPARAM)(MAKELONG((WORD)GetmHpVarStruct()->CourantTask->GetCtrlID(),MsgID)),
						(LPARAM)GetmHpVarStruct()->hWndCurCtrl);
		}
	}
}

//-----------------------------------------------------//
// ForceNotifyMsg
//-----------------------------------------------------//
void ForceNotifyMsg(HWND hWndActive,UINT NotifyCode)
{	NMHDR NotifyStruct;
	NotifyStruct.code=NotifyCode;
	NotifyStruct.hwndFrom=GetmHpVarStruct()->hWndCurCtrl;
	NotifyStruct.idFrom=GetmHpVarStruct()->CourantTask->GetCtrlID();
	if(GetmHpVarStruct()->imHpMode!=2)
	{	SendMessage(GetParent(GetmHpVarStruct()->hWndCurCtrl),
					WM_NOTIFY,
					(WPARAM)((int)GetmHpVarStruct()->CourantTask->GetmHpCtrlID()),
					(LPARAM)&NotifyStruct);
	}
}

//-----------------------------------------------------//
// ForceVHScroll
//-----------------------------------------------------//
void ForceVHScroll(HWND hWndActive,UINT sbMsg,long sbPos,BOOL bVorH)
{	if(GetmHpVarStruct()->imHpMode==2) return;
	short int shTemp;
	LPARAM lParam;
	WPARAM wParam;
	shTemp=(short int)sbPos;
	wParam=(WPARAM)shTemp;
	wParam<<=16;
	wParam+=sbMsg;
	lParam=(LPARAM)GetmHpVarStruct()->hWndCurCtrl;
	if(!bVorH) SendMessage(GetParent(GetmHpVarStruct()->hWndCurCtrl),WM_HSCROLL,wParam,lParam);
	else SendMessage(GetParent(GetmHpVarStruct()->hWndCurCtrl),WM_VSCROLL,wParam,lParam);
}

//-----------------------------------------------------//
// SetRealCursor
//-----------------------------------------------------//
void SetRealCursor(int Xpos,int Ypos,BOOL bShow)
{	if(!bShow)
	{	CONST RECT Rect={(LONG)Xpos,(LONG)Ypos,(LONG)(Xpos+1),(LONG)(Ypos+1)};
		// Positionne le curseur
		SetCursorPos(Xpos,Ypos);
		// Tronquer le déplacement du curseur
		ClipCursor(&Rect);
	}
	else
	{	// Autorise le déplacement du curseur
		ClipCursor(NULL);
		SetCursorPos(Xpos,Ypos);
	}
}

//-----------------------------------------------------//
// IsCtrlReady
//-----------------------------------------------------//
BOOL IsCtrlReady(HWND hWndActive,HWND hWndCtrl)
{	RECT RectCtrl,RectWnd;
	if(hWndCtrl==NULL) return FALSE;
	if(!IsControlReady(hWndCtrl)) return FALSE;
	else
	{	GetWindowRect(hWndActive,&RectWnd);
		GetWindowRect(hWndCtrl,&RectCtrl);
		// Tolérance: 5 pixels
		if(((RectCtrl.top-RectWnd.top)<(-5))||
		   ((RectCtrl.left-RectWnd.left)<(-5))||
		   ((RectWnd.bottom-RectCtrl.bottom)<(-5))||
		   ((RectWnd.right-RectCtrl.right)<(-5))) return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------//
// GetItemsInVisu
//-----------------------------------------------------//
void GetItemsInVisu(HWND hWndActive,BOOL bComboOrList)
{	int Temp,Temp1;
	RECT rect,DropRect;
	GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&rect);
    if(bComboOrList)
	{	Temp1=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETCOUNT,0,0);
		if((IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_SIMPLE)==CBS_SIMPLE)&&
		   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWNLIST)!=CBS_DROPDOWNLIST)&&
		   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWN)!=CBS_DROPDOWN))
		{	Temp=(int)rect.bottom;
			Temp-=(int)(rect.top+20);
		}
		else
		{	SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETDROPPEDCONTROLRECT,0,(LPARAM)&DropRect);
            Temp=(int)(DropRect.bottom-rect.bottom);
			//** BUG VB (Problème méthode Combo1.FontSize !!) **
			if(Temp==0)
			{	COMBOBOXINFO CboBoxInfo;
                memset(&CboBoxInfo,0,sizeof(COMBOBOXINFO));
                CboBoxInfo.cbSize=sizeof(COMBOBOXINFO);
                if(SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETCOMBOBOXINFO,0,(LPARAM)&CboBoxInfo))
                {   RECT lstRect;
                    GetWindowRect(CboBoxInfo.hwndList,&lstRect);
                    Temp=(int)(lstRect.bottom-lstRect.top);
                }
                else // BUG: No height but width!!!
                    Temp=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETDROPPEDWIDTH,0,0);
			}
		}
	}
	else
	{	Temp1=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETCOUNT,0,0);
		Temp=(int)(rect.bottom-rect.top);
        //Temp-=4;
        BYTE Style=(IsWndExStyle(GetmHpVarStruct()->hWndCurCtrl,WS_EX_STATICEDGE)==WS_EX_STATICEDGE)? 0x01:0x00;
        Style|=(IsWndExStyle(GetmHpVarStruct()->hWndCurCtrl,WS_EX_CLIENTEDGE)==WS_EX_CLIENTEDGE)? 0x02:0x00;
        Style|=(IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,WS_BORDER)==WS_BORDER)? 0x04:0x00;
        switch(Style)
        {   case 1: // WS_EX_STATICEDGE
            {   Temp-=2;
                break;
            }
            case 2: // WS_EX_CLIENTEDGE
            case 4: // WS_BORDER
            case 6: // WS_BORDER | WS_EX_CLIENTEDGE
            {   Temp-=4;
                break;
            }
            case 3: // WS_EX_CLIENTEDGE | WS_EX_STATICEDGE
            case 5: // WS_BORDER | WS_EX_STATICEDGE
            case 7: // WS_BORDER | WS_EX_STATICEDGE | WS_EX_CLIENTEDGE
            {   Temp-=6;
                break;
            }
        }
	}
	GetmHpVarStruct()->ItemsVisu=(Temp/ITEM_ECART);
    if(Temp1<=GetmHpVarStruct()->ItemsVisu) GetmHpVarStruct()->AscensSens=0;
	else GetmHpVarStruct()->AscensSens=1;
}

//-----------------------------------------------------//
// ComboListBoxStatusPos
//-----------------------------------------------------//
void ComboListBoxStatusPos(HWND hWndActive,BOOL &Used,BOOL ComboList,int iIndex)
{	int iTemp;
	RECT Rect;
	GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&Rect);
	GetItemsInVisu(hWndActive,ComboList);
	iTemp=iIndex+1;
	if(!GetmHpVarStruct()->AscensSens)
	{	Used=FALSE;
		GetmHpVarStruct()->XnextPos=(int)(Rect.right-8);
		if(ComboList)
		{	if((IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_SIMPLE)==CBS_SIMPLE)&&
			   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWNLIST)!=CBS_DROPDOWNLIST)&&
			   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWN)!=CBS_DROPDOWN))
			{	GetmHpVarStruct()->YnextPos=(int)(Rect.top+18);}
			else GetmHpVarStruct()->YnextPos=(int)(Rect.bottom-8);
		}
		else GetmHpVarStruct()->YnextPos=(int)(Rect.top-8);
		GetmHpVarStruct()->YnextPos+=(ITEM_ECART*iTemp);
	}
	else
	{	int iTemp1;
		if(ComboList)
			iTemp1=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,CB_GETTOPINDEX,0,0);
		else
			iTemp1=(int)SendMessage(GetmHpVarStruct()->hWndCurCtrl,LB_GETTOPINDEX,0,0);
		iTemp1++;
		if(iTemp1>iTemp) Used=TRUE;
		else if(iTemp<(iTemp1+GetmHpVarStruct()->ItemsVisu))
		{	int iTemp2;
			Used=FALSE;
			iTemp2=iTemp-iTemp1;
			iTemp2++;
			GetmHpVarStruct()->XnextPos=(int)(Rect.right-8);
			GetmHpVarStruct()->XnextPos-=ASCENS_ECART;
			if(ComboList)
			{	if((IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_SIMPLE)==CBS_SIMPLE)&&
				   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWNLIST)!=CBS_DROPDOWNLIST)&&
				   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWN)!=CBS_DROPDOWN))
				{	GetmHpVarStruct()->YnextPos=(int)(Rect.top+18);}
				else GetmHpVarStruct()->YnextPos=(int)(Rect.bottom-8);
			}
			else GetmHpVarStruct()->YnextPos=(int)(Rect.top-8);
			GetmHpVarStruct()->YnextPos+=(ITEM_ECART*iTemp2);
		}
		else
		{	Used=TRUE;
			GetmHpVarStruct()->AscensSens=2;
		}
	}
}

//-----------------------------------------------------//
// ListTreeStatusPos
//-----------------------------------------------------//
void ListTreeStatusPos(HWND hWndActive,RECT itmRect)
{	RECT ctrlRect;
	GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&ctrlRect);
	GetmHpVarStruct()->AscensSens=0;
	if(ctrlRect.right<(itmRect.right+ctrlRect.left)) GetmHpVarStruct()->AscensSens=3;       // Droite
	else if(ctrlRect.left>(itmRect.left+ctrlRect.left)) GetmHpVarStruct()->AscensSens=4;    // Gauche
	else if(ctrlRect.top>(itmRect.top+ctrlRect.top)) GetmHpVarStruct()->AscensSens=1;       // Monte
	else if(ctrlRect.bottom<(itmRect.bottom+ctrlRect.top)) GetmHpVarStruct()->AscensSens=2; // Descend
}

//-----------------------------------------------------//
// GetAscensNextPos
//-----------------------------------------------------//
void GetAscensNextPos(HWND hWndActive,BOOL bStep,BOOL bComboList)
{	RECT rect;
	GetWindowRect(GetmHpVarStruct()->hWndCurCtrl,&rect);
	GetmHpVarStruct()->XnextPos=(int)(rect.right-8);
	if(bComboList)
	{	if((IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_SIMPLE)==CBS_SIMPLE)&&
		   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWNLIST)!=CBS_DROPDOWNLIST)&&
		   (IsWndStyle(GetmHpVarStruct()->hWndCurCtrl,CBS_DROPDOWN)!=CBS_DROPDOWN))
		{	GetmHpVarStruct()->YnextPos=(int)(rect.top+18);}
		else GetmHpVarStruct()->YnextPos=(int)(rect.bottom-8);
	}
	else GetmHpVarStruct()->YnextPos=(int)(rect.top-8);
	if(GetmHpVarStruct()->AscensSens==1)
        GetmHpVarStruct()->YnextPos+=(ITEM_ECART-2);
	else
        GetmHpVarStruct()->YnextPos+=(ITEM_ECART*GetmHpVarStruct()->ItemsVisu);
	if(!bStep) GetmHpVarStruct()->XnextPos-=ASCENS_ECART;
	else if((!bComboList)&&(GetmHpVarStruct()->AscensSens==2))
	{	GetmHpVarStruct()->YnextPos=(int)(rect.bottom-ITEM_ECART);}
}

//-----------------------------------------------------//
// ViewCtrlNotReadyMsg
//-----------------------------------------------------//
int ViewCtrlNotReadyMsg(HWND hWndActive,int iMsgID)
{	char lpCtrlMsg[MAX_RS_SIZE]={0};
	UINT IdsMsg=IDS_CTRL_INACTIF;
	switch(iMsgID)
	{	case 1:
		{	IdsMsg=IDS_EXE_NOTFOUND;
			break;
		}
		case 2:
		{	IdsMsg=IDS_DIFF_WINDOW;
			break;
		}
		case 3:
		{	IdsMsg=IDS_MAX_DELAY;
			break;
		}
		case 4:
		{	IdsMsg=IDS_CTRL_NOSTATE;
			break;
		}
		default:
		{	break;}
	}
	if(iMsgID==4) strncpy(lpCtrlMsg,GetmHpVarStruct()->CourantTask->GetmHpStateMsg(),MAX_RS_SIZE);
	else
	{	strncpy(lpCtrlMsg,GetmHpVarStruct()->CourantTask->GetmHpMsg(),MAX_RS_SIZE);
		if(iMsgID!=3)
		{	GetmHpVarStruct()->iCurOption=GetmHpVarStruct()->CourantTask->GetmHpOption();
			if(GetmHpVarStruct()->iCurOption>=10) GetmHpVarStruct()->iCurOption-=10;
			GetmHpVarStruct()->bContinueProc=GetmHpVarStruct()->TaskList->GetContinueProc(mHpCtrl_iCurrentWnd,mHpCtrl_iCountWnd);
			GetmHpVarStruct()->iCurObj=(GetmHpVarStruct()->TaskList->GetCurrentObj()+1);
			if(GetmHpVarStruct()->TaskList->GetNextmHpID()!=NO) GetmHpVarStruct()->bNextmHpID=TRUE;
			else GetmHpVarStruct()->bNextmHpID=FALSE;
			StopmHelp(hWndActive,TRUE,TRUE);
			if(!strncmp(lpCtrlMsg,NULL_STRING,MAX_RS_SIZE)) return ViewCtrlNotReadyDlg(hWndActive,NULL,IdsMsg);
			return ViewCtrlNotReadyDlg(hWndActive,lpCtrlMsg,0);
		}
	}
	GetmHpVarStruct()->iCurOption=0;
	StopmHelp(hWndActive,TRUE,TRUE);
	if(!strncmp(lpCtrlMsg,NULL_STRING,MAX_RS_SIZE)) return ViewCtrlNotReadyDlg(hWndActive,NULL,IdsMsg);
	return ViewCtrlNotReadyDlg(hWndActive,lpCtrlMsg,0);
}

//-----------------------------------------------------//
// GetCommonEvent
//-----------------------------------------------------//
void GetCommonEvent(HWND hWndActive,int &iCur,int &iCnt,int &iOption,char* lpMessage,int MsgSize,char* lpStateMsg,
					int MsgStateSize,char &cNext,int &iRes)
{	int iTmp;
	char lpTmp[MAX_RS_SIZE]={0};
	iRes=NO;
	// iCurrentWnd
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iCur,lpTmp,MAX_RS_SIZE,cNext,FALSE);
	//if(iCur==NO) return; 
	// iCountWnd
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iCnt,lpTmp,MAX_RS_SIZE,cNext,FALSE);
	if(iCnt==NO) return; 
	// iOption
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iOption,lpTmp,MAX_RS_SIZE,cNext,FALSE);
	if(iOption==NO) return; 
	if(iOption>=10)
	{	// lpmHpStateMsg
		mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpStateMsg,MsgStateSize,cNext,TRUE);
	}
	else mHpPak_InitStr(lpStateMsg,MsgStateSize);
	// lpmHpMsg	
	mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpMessage,MsgSize,cNext,TRUE);
	if(iTmp==NO) return;
	iRes=0; // OK
}

//-----------------------------------------------------//
// GetSelectedString
//-----------------------------------------------------//
void GetSelectedString(HWND hWndActive,char* mHpFileName,char* lpSel,int SelSize,BOOL &bContinue,BOOL &bError)
{	int iTmp;
	char chNext=ITEM_GROUP;
	bError=TRUE;
	bContinue=FALSE;
	// lpSelected
	mHpPak_GetEventPart(hWndActive,mHpFileName,iTmp,lpSel,SelSize,chNext,TRUE);
	if(iTmp==NO) return;
	if(chNext==ITEM_GROUP) bContinue=TRUE;
	bError=FALSE;
}

//-----------------------------------------------------//
// GetNextAutoAss
//-----------------------------------------------------//
BOOL GetNextAutoAss()
{	static int iFavPos=1;
	char lpTmp[MAX_RS_SIZE]={0};
	int iTmp=GetmHpVarStruct()->imHpRepeat;
	int iTmp1=GetmHpVarStruct()->imHpMode;
	if(mHpPak_bAutoAss)
	{	mHpPak_ChangeAutoAss(lpTmp,MAX_RS_SIZE,iFavPos,iTmp1,GetmHpVarStruct()->IndexTab,MAX_PART_TREE,iTmp);
		GetmHpVarStruct()->imHpRepeat=iTmp;
		GetmHpVarStruct()->imHpMode=iTmp1;
		mHpPak_InitStr(GetmHpVarStruct()->mHpFileName,MAX_RS_SIZE);
		strncpy(GetmHpVarStruct()->mHpFileName,lpTmp,MAX_RS_SIZE);
		iFavPos++;
	}
	return mHpPak_bAutoAss;
}

//-----------------------------------------------------//
// AddItemList
//-----------------------------------------------------//
void AddItemList(HWND hWndCtrl,LPSTR lpstr,int IconIndex,int Rang)
{	LV_ITEM lvi;
	lvi.mask  = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.state = lvi.stateMask = 0;
	lvi.iItem       = Rang;
	lvi.iSubItem    = 0;
	lvi.pszText     = lpstr;
	lvi.cchTextMax  = 0;
	lvi.iImage      = IconIndex;
	ListView_InsertItem(hWndCtrl,&lvi);
}

//-----------------------------------------------------//
// RefreshControl
//-----------------------------------------------------//
/*void RefreshControl(HWND hWndActive,UINT uiCtrlID)
{	RECT CtrlRect;
	GetWindowRect(GetCtrlWnd(hWndActive,uiCtrlID),&CtrlRect);
	MoveWindow(GetCtrlWnd(hWndActive,uiCtrlID),CtrlRect.left,CtrlRect.top,
			   (CtrlRect.right-CtrlRect.left),
			   (CtrlRect.bottom-CtrlRect.top),TRUE);
}*/

//-----------------------------------------------------//
// HideLockCursor
//-----------------------------------------------------//
void HideLockCursor(HWND hWndActive)
{	RECT WndRect;
	GetmHpVarStruct()->xResolution=GetDeviceCaps(GetDC(hWndActive),HORZRES);
	GetmHpVarStruct()->yResolution=GetDeviceCaps(GetDC(hWndActive),VERTRES);
	// Initialise la position du curseur
	if(!GetmHpVarStruct()->iCurObj)
	{	GetmHpVarStruct()->XposCur=(int)(GetmHpVarStruct()->xResolution/2);
		GetmHpVarStruct()->YposCur=(int)(GetmHpVarStruct()->yResolution/2);
	}
	// Curseur réel
	if(!IsZoomed(hWndActive))
	{	GetWindowRect(hWndActive,&WndRect);
		SetRealCursor((int)(((WndRect.right-WndRect.left)/2)+WndRect.left),
					  (int)(((WndRect.bottom-WndRect.top)/2)+WndRect.top),
					  FALSE);
	}
	else SetRealCursor(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur,FALSE);
	ShowCursor(FALSE);
}

//-----------------------------------------------------//
// WriteCommonTask
//-----------------------------------------------------//
BOOL WriteCommonTask(HWND hWndActive,CEventListe* CEventLst,CWindowListe* hCurWnd,char* ExeAppName,int Index)
{	int indice=0;
    CmHpTask* WriteTask=CEventLst->GetmHpTask(Index,FALSE);
	// mHpID | CtrlPlatformID, CtrlClassIdx, CtrlStyle & CtrlMsgID
    if(WriteTask->GetmHpID()!=MHPID_CTRL_MAN)
    {   // mHpID
        if(!mHpPak_WriteParty(hWndActive,NEXT_TASK,WriteTask->GetmHpID(),NULL,FALSE)) // '_'
            return FALSE;
    }
    else
    {   if(!WriteTask->GetCtrlMgmt())
            return FALSE;
        // CtrlPlatformID, CtrlClassIdx, CtrlStyle & CtrlMsgID
        int iCtrlPlatformID=(int)WriteTask->GetCtrlPlatformID();
        int iCtrlStyle=WriteTask->GetCtrlMgmt()->GetStyle();
        int iCtrlMsgID=WriteTask->GetCtrlMgmt()->GetCaptMsgID();
        const char* lpCtrlClass=WriteTask->GetCtrlMgmt()->GetCtrlClass();
        if((iCtrlStyle==NO)||(iCtrlMsgID==NO)||(lpCtrlClass==NULL))
            return FALSE;
        // Get control class index
        int iCtrlClassIdx=mHpPak_ProcCtrlList->GetClassIndex((UINT)iCtrlPlatformID,lpCtrlClass);
        if(iCtrlClassIdx==NO)
        {
            ////// TODO: Check why 'lpVersion' is empty for Qt control if I don't call 'FillQtData' below ?????
            WriteTask->GetCtrlMgmt()->FillQtData(); // Can return FALSE if not a Qt control!!!
            //////

            // Add control class into control list
            const char* lpVersion=WriteTask->GetCtrlMgmt()->GetDllVersion();
            iCtrlClassIdx=mHpPak_ProcCtrlList->AddControl((UINT)iCtrlPlatformID,lpCtrlClass,(!lpVersion)?"":lpVersion);
            mHpPak_ProcCtrlList->SetUpdated(TRUE); // Control list updated in file
        }
        if((!mHpPak_WriteParty(hWndActive,NEXT_TASK,iCtrlPlatformID,NULL,FALSE))|| // '_'
           (!mHpPak_WriteParty(hWndActive,NEXT_RS,iCtrlClassIdx,NULL,FALSE))|| // '|'
           (!mHpPak_WriteParty(hWndActive,NEXT_RS,iCtrlStyle,NULL,FALSE))|| // '|'
           (!mHpPak_WriteParty(hWndActive,NEXT_RS,iCtrlMsgID,NULL,FALSE))) // '|'
            return FALSE;
    }
    // iCurrentWnd // iCountWnd // iOption // lpmHpMsg
    if(WriteTask->GetCurrentWnd()!=NO)
    {   if((!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,hCurWnd->GetCurWnd(ExeAppName)[WriteTask->GetCurrentWnd()],NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,WriteTask->GetCountWnd(),NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,WriteTask->GetmHpOption(),NULL,FALSE))) // '¤'
		{	return FALSE;}
		if(WriteTask->GetmHpOption()>=10)
		{	if((!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteTask->GetmHpStateMsg(),TRUE))|| // '¤'
			   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteTask->GetmHpMsg(),TRUE))) // '¤'
			{	return FALSE;}
		}
		else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteTask->GetmHpMsg(),TRUE)) // '¤'
		{	return FALSE;}
	}
	else
	{   if((!mHpPak_WriteParty(hWndActive,NEXT_TASK,WriteTask->GetmHpID(),NULL,FALSE))|| // '_'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,WriteTask->GetCountWnd(),NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,WriteTask->GetmHpOption(),NULL,FALSE))|| // '¤'
		   (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteTask->GetmHpMsg(),TRUE))) // '¤'
		{	return FALSE;}
	}
    if(WriteTask->GetmHpID()!=MHPID_CTRL_MAN)
    {   // DotNetName
        if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,WriteTask->GetmHpCtrlName(),TRUE)) // '¤'
	    {	return FALSE;}
	    // mHpCtrlID
        if(WriteTask->GetmHpCtrlID()[indice]==NO)
        {   if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,TRUE)) // '¤'
	        {	return FALSE;}
        }
        else
        {   while(WriteTask->GetmHpCtrlID()[indice]!=NO)
	        {	if(!indice)
		        {	if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,WriteTask->GetmHpCtrlID()[indice],NULL,FALSE)) // '¤'
			        {	return FALSE;}
		        }
		        else
		        {	if(!mHpPak_WriteParty(hWndActive,ITEM_GROUP,WriteTask->GetmHpCtrlID()[indice],NULL,FALSE)) // '^'
			        {	return FALSE;}
		        }
		        indice++;
	        }
        }
    }
	return TRUE;
}

//-----------------------------------------------------//
// WriteImmediatTask
//-----------------------------------------------------//
void WriteImmediatTask(HWND hWndActive,char* lpCtrlName,HWND hWndctrl,HWND hWndTool,WPARAM wParam,int iReadOp,int imHpCtrlID,
                       UINT iTaskID,CEventListe* EventObject,ImHpCtrlMgmt* pCtrlMgmt,UINT iPlatformID,char* AppName,
                       UINT &pLastID,int pCur,int pCnt,int &iRes)
{	int iTmpID;
	GetmHpVarStruct()->EventList=EventObject;
	switch(iReadOp)
	{	case 2:
		{	AddIfChangeExe(EventObject,AppName);
			if(GetmHpVarStruct()->imHpProcID==mHpPak_ReadmHpProcIDKey())
			{	mHpPak_InitStr(GetmHpVarStruct()->lpCurApp,MAX_RS_SIZE);
				strncpy(GetmHpVarStruct()->lpCurApp,AppName,MAX_RS_SIZE);
				iTmpID=(int)pLastID;
				// AddEvent -------------------------------------------------------------------------------------------**//
				ComCtrl_AddEvent(hWndActive,
                                 lpCtrlName,
                                 pCtrlMgmt,
                                 iPlatformID,
								 hWndctrl,
                                 wParam,
								 imHpCtrlID,
								 iTmpID,
								 iTaskID,
								 pCur,
								 pCnt);
				//-----------------------------------------------------------------------------------------------------**//
				pLastID=(UINT)iTmpID;
				iRes=0;
			}
			else
			{	mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_ADD_FINISHED,NULL,0,0,MB_ICONSTOP);
				SendMessage(hWndTool,WM_CLOSE,0,0);
				GetmHpVarStruct()->bNoClearmHpKeys=TRUE;
				StopmHelp(hWndActive,TRUE,TRUE);
				iRes=2;
			}
			break;
		}
		case 3:
		{	mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_ADD_CUSTOMING,NULL,0,0,MB_ICONSTOP);
			iRes=1;
			break;
		}
		default:
		{	mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_ADD_FINISHED,NULL,0,0,MB_ICONSTOP);
			SendMessage(hWndTool,WM_CLOSE,0,0);
			StopmHelp(hWndActive,TRUE,TRUE);
			iRes=2;
			break;
		}
	}
}

//-----------------------------------------------------//
// LinkItems
//-----------------------------------------------------//
void LinkItems(HWND hWndLnkLst)
{	int iCnt=ListView_GetItemCount(hWndLnkLst);
	for(int i=0;i<iCnt;i++)
	{	if(ListView_GetItemState(hWndLnkLst,i,LVIS_SELECTED)==LVIS_SELECTED)
		{	GetmHpVarStruct()->CtrlLink->LinkItm(i);}
	}
}

//-----------------------------------------------------//
// SetEditStyleAsNumber
//-----------------------------------------------------//
void SetEditStyleAsNumber(HWND hEditWnd)
{	long lstyle=(long)GetWindowLongPtr(hEditWnd,GWL_STYLE);
	lstyle|=ES_NUMBER;
	SetWindowLongPtr(hEditWnd,GWL_STYLE,(LONG_PTR)lstyle);
}

//-----------------------------------------------------//
// FillLnkCtrlLst
//-----------------------------------------------------//
void FillLnkCtrlLst(HWND hWndLstLnk,int mHpID,WORD wIconID,WORD idsView)
{	LVCOLUMN lvcolumn;
	CEventCell* cLnkCell;
	char lpRC[MAX_RS_SIZE]={0};
	char lpLvcolumn[MAX_RS_SIZE]={0};
	char lpRow[MAX_RS_SIZE]={0};
	int indice=0,iRow=0,iLag=0,iLnkLag=0;
	// Header
	if(hLnkCtrlImgLst!=NULL) ImageList_Destroy(hLnkCtrlImgLst);
	hLnkCtrlImgLst=ImageList_Create(16,16,ILC_MASK|ILC_COLOR16,6,1);
	ImageList_AddIcon(hLnkCtrlImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_SEP)));
	ImageList_AddIcon(hLnkCtrlImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(wIconID)));
	ListView_SetImageList(hWndLstLnk,hLnkCtrlImgLst,LVSIL_SMALL);
	lvcolumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvcolumn.fmt=LVCFMT_LEFT;
	lvcolumn.cx=120;
	lvcolumn.iSubItem=0;
	lvcolumn.cchTextMax=MAX_RS_SIZE;
	lvcolumn.pszText=lpLvcolumn;
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_UNLINKED,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndLstLnk,0,&lvcolumn);
	lvcolumn.cx=39;
	lvcolumn.iSubItem=1;
	mHpPak_InitStr(lpRC,MAX_RS_SIZE);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_COLUMN_ROW,lpRC,MAX_RS_SIZE);
	strncpy(lvcolumn.pszText,lpRC,MAX_RS_SIZE);
	ListView_InsertColumn(hWndLstLnk,1,&lvcolumn);
	// Fill
	mHpPak_InitStr(lpRC,MAX_RS_SIZE);
	LoadString(GetmHpVarStruct()->hmHpCtrlInst,idsView,lpRC,MAX_RS_SIZE);
	GetmHpVarStruct()->CtrlLink->EmptyLinkList();
	while(GetmHpVarStruct()->EventList->GetmHpTask(indice,FALSE)!=NULL)
	{	if(GetmHpVarStruct()->EventList->GetmHpTask(indice,FALSE)->GetmHpID()==mHpID)
		{	if((GetmHpVarStruct()->EventList->GetmHpWrite(indice,FALSE)->GetLinked()!=NO)&&(GetmHpVarStruct()->iCtrlSel==indice))
			{	iLag=GetmHpVarStruct()->EventList->GetmHpWrite(indice,FALSE)->GetLinked();
				cLnkCell=GetmHpVarStruct()->EventList->GetCellFromPos(indice,FALSE);
				mHpPak_InitStr(lpRow,MAX_RS_SIZE);
				_snprintf(lpRow,MAX_RS_SIZE,"%d",(indice+1));
				while(iLag!=0)
				{	if(mHpID==8) // Radiobox button
					{	AddItemList(hWndLstLnk,cLnkCell->mHpWrite->GetRadioName(),1,iRow);
						// Get the next Radiobox event
						cLnkCell=cLnkCell->NextEvent;
					}
					else AddItemList(hWndLstLnk,lpRC,1,iRow);
					ListView_SetItemText(hWndLstLnk,iRow,1,lpRow);
					ListView_SetItemState(hWndLstLnk,iRow,LVIS_SELECTED,LVIS_SELECTED);
					GetmHpVarStruct()->CtrlLink->AddItm(indice+iLnkLag);
					GetmHpVarStruct()->CtrlLink->LinkItm(iRow);
					iRow++;
					iLnkLag++;
					iLag--;
				}
			}
			else if((GetmHpVarStruct()->EventList->GetmHpWrite(indice,FALSE)->GetLinked()==NO)&&(IsItNearCtrlSel(indice,mHpID)))
			{	mHpPak_InitStr(lpRow,MAX_RS_SIZE);
				_snprintf(lpRow,MAX_RS_SIZE,"%d",(indice+1));
				if(mHpID==8) // Radiobox button
				{	AddItemList(hWndLstLnk,GetmHpVarStruct()->EventList->GetmHpWrite(indice,FALSE)->GetRadioName(),1,iRow);}
				else AddItemList(hWndLstLnk,lpRC,1,iRow);
				GetmHpVarStruct()->CtrlLink->AddItm(indice);
				ListView_SetItemText(hWndLstLnk,iRow,1,lpRow);
				if(GetmHpVarStruct()->iCtrlSel==indice) ListView_SetItemState(hWndLstLnk,iRow,LVIS_SELECTED,LVIS_SELECTED);
				iRow++;
			}
		}
		indice++;	
	}
}

//-----------------------------------------------------//
// ReadmHpCtrlID
//-----------------------------------------------------//
void ReadmHpCtrlID(HWND hWndActive,char* mHpFilePath,int* lpReadCtrl,int iReadSize,char &ReadNext)
{	char lpTmp[MAX_RS_SIZE]={0};
	int iRead=0;
	ReadNext=NEXT_RS;
	mHpPak_EmptyTab(lpReadCtrl,iReadSize);
	mHpPak_GetEventPart(hWndActive,mHpFilePath,lpReadCtrl[iRead],lpTmp,MAX_RS_SIZE,ReadNext,FALSE);
	while((lpReadCtrl[iRead]!=NO)&&(ReadNext==ITEM_GROUP))
	{	iRead++;
		ReadNext=NEXT_RS;
		mHpPak_GetEventPart(hWndActive,mHpFilePath,lpReadCtrl[iRead],lpTmp,MAX_RS_SIZE,ReadNext,FALSE);
	}
	if(lpReadCtrl[iRead]==NO)
	{	mHpPak_EmptyTab(lpReadCtrl,iReadSize);
		lpReadCtrl[0]=NO;
	}
	else lpReadCtrl[(iRead+1)]=NO;
}

//-----------------------------------------------------//
// AddmHpCtrlID
//-----------------------------------------------------//
void AddmHpCtrlID(HWND hWndActive,int* lpCtrlId,int iCtrlSize,UINT ictrlID)
{	char lpMainClass[MAX_RS_SIZE]={0};
	char lpMainTitle[MAX_RS_SIZE]={0};
	int lpAddCtrl[MAX_PART_TREE]={0};
	HWND hWindow=mHpPak_ProcWndLst->GetCurWindow();
	HWND hFindWnd=hWndActive;
	int index=0,iEnd=0;
	mHpPak_EmptyTab(lpCtrlId,iCtrlSize);
	mHpPak_EmptyTab(lpAddCtrl,MAX_PART_TREE);
	lpAddCtrl[index]=(int)ictrlID;
	lpCtrlId[index]=(int)ictrlID;
	index++;
	while((hWindow!=hFindWnd)&&(hFindWnd!=NULL))
	{	mHpPak_InitStr(lpMainClass,MAX_RS_SIZE);
		mHpPak_InitStr(lpMainTitle,MAX_RS_SIZE);
		GetClassName(hFindWnd,lpMainClass,MAX_RS_SIZE);
		GetWindowText(hFindWnd,lpMainTitle,MAX_RS_SIZE);
		if(!mHpPak_ProcWndLst->IsWndInList(GetmHpVarStruct()->lpCurApp,lpMainClass,lpMainTitle))
		{	lpAddCtrl[index]=GetDlgCtrlID(hFindWnd);
			index++;
		}
		hFindWnd=GetParent(hFindWnd);
	}
	lpAddCtrl[index]=NO;
	if(index!=1)
	{	// Inverse
		index=0;
		while(lpAddCtrl[iEnd]!=NO) iEnd++;
		iEnd--;
		while(iEnd!=NO)
		{	lpCtrlId[index]=lpAddCtrl[iEnd];
			iEnd--;
			index++;
		}
	}
	lpCtrlId[index]=NO;
}

//-----------------------------------------------------//
// AddToolCtrlID
//-----------------------------------------------------//
/*void AddToolCtrlID(HWND hWndActive,HWND hWndFromTool,int* lpCtrlId,int iCtrlSize)
{	int lpAddCtrl[MAX_PART_TREE]={0};
	int index=0,iEnd=0;
	HWND hWndTmp=hWndFromTool;
	mHpPak_EmptyTab(lpCtrlId,iCtrlSize);
	mHpPak_EmptyTab(lpAddCtrl,MAX_PART_TREE);
	lpAddCtrl[index]=(int)GetWindowLongPtr(hWndTmp,GWLP_ID);
	index++;
	while((hWndTmp!=NULL)&&(hWndTmp!=hWndActive))
	{	hWndTmp=GetParent(hWndTmp);
		if((hWndTmp!=NULL)&&(hWndTmp!=hWndActive))
		{	lpAddCtrl[index]=(int)GetWindowLongPtr(hWndTmp,GWLP_ID);
			index++;
		}
	}
	lpAddCtrl[index]=NO;
	if(index!=1)
	{	// Inverse
		index=0;
		while(lpAddCtrl[iEnd]!=NO) iEnd++;
		iEnd--;
		while(iEnd!=NO)
		{	lpCtrlId[index]=lpAddCtrl[iEnd];
			iEnd--;
			index++;
		}
	}
	lpCtrlId[index]=NO;
}*/

//-----------------------------------------------------//
// CreateTaskObjects
//-----------------------------------------------------//
void CreateTaskObjects()
{	if(!GetmHpVarStruct()->bCreateTaskObj)
	{	GetmHpVarStruct()->CtrlLink=new CLinkList();
		GetmHpVarStruct()->bCreateTaskObj=TRUE;
	}
}

//-----------------------------------------------------//
// CreateItemListe
//-----------------------------------------------------//
void CreateItemListe(HWND hWndActive,BOOL &bFindAll)
{	int iSelitem,indice,itemp;
	int iSelTab[MAX_RS_SIZE]={0};
	mHpPak_EmptyTab(iSelTab,MAX_RS_SIZE);
	GetmHpVarStruct()->SelItemListe->EmptyGroupListe();
	bFindAll=TRUE;
	while(GetmHpVarStruct()->CourantTask->GetNextIndex())
	{	// ItemIndexToSel --------------------------------------------------**//
		iSelitem=ComCtrl_ItemIndexToSel(hWndActive);
		//------------------------------------------------------------------**//
		if(iSelitem!=NO)
		{	indice=0;
			itemp=0;
			while((iSelTab[indice]!=NO)&&(iSelTab[indice]<iSelitem)) indice++;
			if(iSelTab[indice]==NO)
			{	iSelTab[indice]=iSelitem;
				iSelTab[(indice+1)]=NO;
			}
			else if(iSelTab[indice]!=iSelitem)
			{	while(itemp!=NO)
				{	itemp=iSelTab[indice];
					iSelTab[indice]=iSelitem;
					iSelitem=itemp;
					indice++;
				}
				iSelTab[indice]=NO;
			}
		}
		else bFindAll=FALSE;
	}
	indice=0;
	while(iSelTab[indice]!=NO)
	{	GetmHpVarStruct()->SelItemListe->AddItemIndex(iSelTab[indice]);
		indice++;
	}
}

//-----------------------------------------------------//
// ApplyLnkCtrlChange
//-----------------------------------------------------//
void ApplyLnkCtrlChange(HWND hWndActive,HWND hWndCfgList,BOOL bCfgItmLnk)
{	int iCnt=ListView_GetItemCount(hWndCfgList);
	char lpTmp[MAX_RS_SIZE]={0};
	BOOL bFirstLnk=FALSE;
	int i,iNextPos,iFirstPos=NO;
	// Unlink
	for(i=0;i<iCnt;i++)
	{	GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->CtrlLink->GetItmIndex(i),FALSE)->SetLinked(NO);
		GetmHpVarStruct()->EventList->GetmHpWrite(GetmHpVarStruct()->CtrlLink->GetItmIndex(i),FALSE)->SetRadioRow(1);
		GetmHpVarStruct()->EventList->DeletemHpParam(GetmHpVarStruct()->CtrlLink->GetItmIndex(i));
	}
	// Link
	if(bCfgItmLnk)
	{	for(i=0;i<iCnt;i++)
		{	if(GetmHpVarStruct()->CtrlLink->GetItmLink(i))
			{	if(!bFirstLnk)
				{	iFirstPos=GetmHpVarStruct()->CtrlLink->GetItmIndex(i);
					iNextPos=(GetmHpVarStruct()->CtrlLink->GetItmIndex(i)+1);
					bFirstLnk=TRUE;
				}
				else
				{	GetmHpVarStruct()->EventList->MovemHpCtrl(GetmHpVarStruct()->CtrlLink->GetItmIndex(i),iNextPos);
					iNextPos++;
				}
			}
		}
		if(iFirstPos!=NO)
		{	GetWindowText(GetDlgItem(hWndActive,IDC_EDIT_STATE),lpTmp,MAX_RS_SIZE);
			GetmHpVarStruct()->EventList->GetmHpWrite(iFirstPos,FALSE)->SetLinked(ListView_GetSelectedCount(hWndCfgList));
			GetmHpVarStruct()->EventList->GetmHpWrite(iFirstPos,FALSE)->SetRadioRow(mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,0));
		}
	}
	if(iFirstPos!=NO) GetmHpVarStruct()->iCtrlSel=iFirstPos;
}

//-----------------------------------------------------//
// ProcessStartProcRes
//-----------------------------------------------------//
void ProcessStartProcRes(HWND hWndActive,int iStartProcRes)
{	switch(iStartProcRes)
	{	case 0:
		{	int iOptTask=GetmHpVarStruct()->CourantTask->GetmHpOption();
			if(iOptTask>=10) iOptTask-=10;
			switch(iOptTask)
			{	case 0:
				case 1:
				{	int iRes=0;
					if(!GetmHpVarStruct()->CourantTask->GetmHpID()) iRes=ViewCtrlNotReadyMsg(hWndActive,1);
					else iRes=ViewCtrlNotReadyMsg(hWndActive,0);
					if(iRes==IDC_CONTINUE_BTN)
					{	if(GetmHpVarStruct()->bNextmHpID) SetTimer(hWndActive,ID_TIMER_RESTART,3,(TIMERPROC)mHpTimerProc);
						else
						{	GetmHpVarStruct()->iCurObj=0;
							NextRepeatAssistProc(hWndActive);
						}
					}
					else GetmHpVarStruct()->iCurObj=0;
					break;
				}
				case 2:
				{	if(GetmHpVarStruct()->TaskList->GetContinueProc(mHpCtrl_iCurrentWnd,mHpCtrl_iCountWnd))
					{	if(GetmHpVarStruct()->TaskList->NextCellule())
						{	GetmHpVarStruct()->CourantTask=GetmHpVarStruct()->TaskList->GetCurrentTask();
							GetNextPos(hWndActive);
						}
						else NextRepeatAssistProc(hWndActive);
					}
					else StopmHelp(hWndActive,TRUE,TRUE);
					break;
				}
				case 3:
				{	GetmHpVarStruct()->bWaitCtrl=TRUE;
					NextStepWaitProc(hWndActive,TRUE);
					break;
				}
				default:
				{	break;}
			}
			break;
		}
		case 1:
		{   if(GetmHpVarStruct()->CourantTask->GetmHpID()!=MHPID_CTRL_MAN)
            {   // Direct2D!!
			    SetDDrawObj(hWndActive,0);
			    EndObjects(hWndActive);
			    SetTimer(hWndActive,ID_TIMER_MAXDELAY,60000,(TIMERPROC)mHpTimerProc);
			    if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
			    else SetTimer(hWndActive,ID_TIMER_DDRAW,3,(TIMERPROC)mHpTimerProc);
            }
            ////// mHelp Control Management /////////////////////////////////
            else
                SetTimer(hWndActive,ID_TIMER_CTRLMAN,3,(TIMERPROC)mHpTimerProc);
			break;
		}
		default:
		{	break;}
	}
}

//****************************************************************************************************************CUSTOMS//
// DidItSelected
//-----------------------------------------------------//
BOOL DidItSelected(int iRang)
{	if(mHpPak_bAutoAss)
	{	if((mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(iRang)!=NULL))
		{	return TRUE;}
	}
	else if(mHpPak_bLnchProc)
	{	if((mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(iRang)!=NULL))
		{	return TRUE;}
	}
	else if(mHpPak_bLnchCustom)
	{	if((mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(iRang)!=NULL))
		{	return TRUE;}
	}
	else return mHpPak_ItemsList->GetSelection(iRang);
	return FALSE;
}

//-----------------------------------------------------//
// GetStrEvoluParam
//-----------------------------------------------------//
void GetStrEvoluParam(int &irang,int iRepeatLoop,int IndiceLoop,char *StrParam,int iParamSize)
{	if(mHpPak_bAutoAss)
	{	if((mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)!=NULL)&&
		   (mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetStrParam(iRepeatLoop,IndiceLoop)!=NULL))
		{	strncpy(StrParam,mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetStrParam(iRepeatLoop,IndiceLoop),iParamSize);}
		else irang=NO;	
	}
	else if(mHpPak_bLnchProc)
	{	if((mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)!=NULL)&&
		   (mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetStrParam(iRepeatLoop,IndiceLoop)!=NULL))
		{	strncpy(StrParam,mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetStrParam(iRepeatLoop,IndiceLoop),iParamSize);}
		else irang=NO;	
	}
	else if(mHpPak_bLnchCustom)
	{	if((mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)!=NULL)&&
		   (mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetStrParam(iRepeatLoop,IndiceLoop)!=NULL))
		{	strncpy(StrParam,mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetStrParam(iRepeatLoop,IndiceLoop),iParamSize);}
		else irang=NO;	
	}
	else
	{	if(mHpPak_ItemsList->GetParamString(irang,iRepeatLoop,IndiceLoop)!=NULL)
		{	strncpy(StrParam,mHpPak_ItemsList->GetParamString(irang,iRepeatLoop,IndiceLoop),iParamSize);}
		else irang=NO;
	}
}

//-----------------------------------------------------//
// GetIntBoolEvoluParam
//-----------------------------------------------------//
int GetIntBoolEvoluParam(int irang,int irepeat,int iloop)
{	if(mHpPak_bAutoAss)
	{	if((mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)!=NULL))
		{	return mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetIntParam(irepeat,iloop);}
	}
	else if(mHpPak_bLnchProc)
	{	if((mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)!=NULL))
		{	return mHpPak_LnchFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetIntParam(irepeat,iloop);}
	}
	else if(mHpPak_bLnchCustom)
	{	if((mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL)&&
		   (mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)!=NULL))
		{	return mHpPak_CustomFavLst->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList->GetCellFromItem(irang)->EvoluItem->GetIntParam(irepeat,iloop);}
	}
	else return mHpPak_ItemsList->GetParamInteger(irang,irepeat,iloop);
	return NO;
}

//*****************************************************************************************************************CREATE//
// StartmHelp
//-----------------------------------------------------//
void StartmHelp(HWND hWndActive,BOOL bFirstPass,BOOL bShared)
{	char lpReadApp[MAX_RS_SIZE]={0};
	int xTmp=0,yTmp=0,iRes=0;
    mHpPak_ProcCtrlList->EmptyList();
    if(bFirstPass) GetmHpVarStruct()->RepeatLoop=0;
	if(!bShared) CreateTaskListe(hWndActive,iRes,lpReadApp,MAX_RS_SIZE);
	if((bShared)||(!iRes))
	{	if((GetmHpVarStruct()->imHpMode==1)&&(GetmHpVarStruct()->imHpRepeat>1))
		{	// Test si "TaskWnd" initial == "TaskWnd" final
			if(!GetmHpVarStruct()->TaskList->IsRepeatListAutorized())
			{	StopmHelp(hWndActive,FALSE,TRUE);
				mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_TASKLISTE_NOREPEAT,NULL,0,0,MB_ICONSTOP);
				return;
			}
		}
		GetmHpVarStruct()->xResolution=GetDeviceCaps(GetDC(hWndActive),HORZRES);
		GetmHpVarStruct()->yResolution=GetDeviceCaps(GetDC(hWndActive),VERTRES);
		if(!bShared) HideLockCursor(hWndActive);
		else if(GetmHpVarStruct()->imHpMode!=1)
		{	ReadCursorPoskey(xTmp,yTmp);
			if((xTmp!=NO)&&(yTmp!=NO))
			{	GetmHpVarStruct()->XposCur=xTmp;
				GetmHpVarStruct()->YposCur=yTmp;
			}
			else
			{	GetmHpVarStruct()->XposCur=(int)(GetmHpVarStruct()->xResolution/2);
				GetmHpVarStruct()->YposCur=(int)(GetmHpVarStruct()->yResolution/2);
			}
		}
		mHpCtrl_bReadmHelp=TRUE;
		mHpCtrl_bChangeWnd=FALSE;
        //GetmHpVarStruct()->bLockDotNetMsg=FALSE;
		if((!bShared)&&(!GetmHpVarStruct()->iCurObj))
		{	mHpCtrl_iCurrentWnd=0;
			mHpCtrl_iCountWnd=0;
		}
		GetmHpVarStruct()->EcartDepl=GetmHpVarStruct()->imHpMouseSpeed;
		while(GetmHpVarStruct()->iCurObj!=0)
		{	GetmHpVarStruct()->TaskList->NextCellule();
			GetmHpVarStruct()->iCurObj--;
		}
		if(GetmHpVarStruct()->TaskList->GetCurrentTask()!=NULL)
		{	GetmHpVarStruct()->CourantTask=GetmHpVarStruct()->TaskList->GetCurrentTask();
			mHpPak_ToolTipsActivate(FALSE);
			GetNextPos(hWndActive);
		}
		else StopmHelp(hWndActive,TRUE,TRUE);
	}
	else
	{	mHpPak_ClearmHpSharedKey();
		switch(iRes)
		{	case 1:
			{	mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_TASKLISTE_CREATE,NULL,0,0,MB_ICONSTOP);
				break;
			}
			case 2:
			{	char lpPath[MAX_KEY_SIZE]={0};
				if(mHpPak_ReadExePathKey(lpPath,MAX_KEY_SIZE,lpReadApp))
				{	strncat(lpPath,MHPPAK_STR_ANTISHLASS,1);
					strncat(lpPath,lpReadApp,mHpPak_FindStrLenght(lpReadApp,MAX_RS_SIZE));
					strncat(lpPath,MHPPAK_EXE_EXTENSION,4);
					mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_TASKLISTE_DIFFEXE,lpPath,MAX_KEY_SIZE,0,MB_ICONEXCLAMATION);
				}
				else
				{	strncat(lpReadApp,MHPPAK_EXE_EXTENSION,4);
					mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_TASKLISTE_DIFFEXE,lpReadApp,MAX_RS_SIZE,0,MB_ICONSTOP);
				}
				break;
			}
			default:
			{	break;}
		}
	}
}

//-----------------------------------------------------//
// StopmHelp
//-----------------------------------------------------//
void StopmHelp(HWND hWndActive,BOOL bBeep,BOOL bShow)
{	RECT ActiRect;
	// Arret des differents timer
	KillTimer(hWndActive,ID_TIMER_MAXDELAY);
	KillTimer(hWndActive,ID_TIMER_ENDTASK);
	KillTimer(hWndActive,ID_TIMER_SHARED);
	KillTimer(hWndActive,ID_TIMER_AUTOASS);
	KillTimer(hWndActive,ID_TIMER_START);
	KillTimer(hWndActive,ID_TIMER_DDRAW);
	KillTimer(hWndActive,ID_TIMER_DELAI);
	KillTimer(hWndActive,ID_TIMER_ASCENS);
	KillTimer(hWndActive,ID_TIMER_NEXT);
	KillTimer(hWndActive,ID_TIMER_WAIT);
	// mHpCtrl's timer ------------------------------**//
	KillTimer(hWndActive,ID_TIMER_MENU);
	KillTimer(hWndActive,ID_TIMER_LISTE);
	KillTimer(hWndActive,ID_TIMER_EDIT);
	KillTimer(hWndActive,ID_TIMER_COMBO);
	KillTimer(hWndActive,ID_TIMER_CTRLMAN);
	//-----------------------------------------------**//
	GetmHpVarStruct()->bNoInactiveMsg=FALSE;
	GetmHpVarStruct()->bLockWaMsg=FALSE;
	GetmHpVarStruct()->bWaitCtrl=FALSE;
	mHpPak_ClosemHpFile();
	mHpCtrl_iLastID=NO;
	GetmHpVarStruct()->iLastCmdID=NO;
	mHpCtrl_bWritemHelp=FALSE;
	mHpCtrl_bLockmHelp=FALSE;
	if(!GetmHpVarStruct()->bNoClearmHpKeys) mHpPak_ClearmHpSharedKey();
	else GetmHpVarStruct()->bNoClearmHpKeys=FALSE;
	mHpPak_SetAppInfoKeys(GetmHpVarStruct()->lpCurApp,1,0);
	if(mHpCtrl_bReadmHelp)
	{	EndObjects(hWndActive);
        SendMessage(hWndActive,WM_CANCELMODE,0,0);
		mHpPak_ToolTipsActivate(TRUE);
		if(bShow)
		{	GetWindowRect(hWndActive,&ActiRect);
			SetRealCursor((int)(((ActiRect.right-ActiRect.left)/2)+ActiRect.left),
						  (int)(((ActiRect.bottom-ActiRect.top)/2)+ActiRect.top),TRUE);
			while(ShowCursor(TRUE)<0);
		}
		if(bBeep) MessageBeep(MB_ICONEXCLAMATION);
		SetActiveWindow(hWndActive);
		GetmHpVarStruct()->EditFlag=FALSE;
		GetmHpVarStruct()->TaskList->EmptyListe();
		GetmHpVarStruct()->SelItemListe->EmptyGroupListe();
		mHpCtrl_bReadmHelp=FALSE;
        //GetmHpVarStruct()->bLockDotNetMsg=FALSE;
		mHpPak_bLockChangeWnd=FALSE;
	}
    // Direct2D
    GetmHpVarStruct()->bNewDepl=TRUE;
	SetDDrawObj(hWndActive,0);
	GetmHpVarStruct()->bConserveBGD=FALSE;
	GetmHpVarStruct()->bRefreshDDraw=FALSE;
}

//****************************************************************************************************************PROCESS//
// NextRepeatAssistProc 
//-----------------------------------------------------//
void NextRepeatAssistProc(HWND hWndActive)
{	GetmHpVarStruct()->RepeatLoop++;
	if((GetmHpVarStruct()->imHpMode==1)&&(GetmHpVarStruct()->imHpRepeat>GetmHpVarStruct()->RepeatLoop))
	{	StopmHelp(hWndActive,FALSE,FALSE);
		mHpPak_ClearmHpSharedKey();
		SetOperationModeKey(1);
		SetTimer(hWndActive,ID_TIMER_RESTART,1,(TIMERPROC)mHpTimerProc);
	}
	else
	{	if(GetNextAutoAss())
		{	StopmHelp(hWndActive,FALSE,FALSE);
			SetTimer(hWndActive,ID_TIMER_AUTOASS,1000,(TIMERPROC)mHpTimerProc);
		}
		else StopmHelp(hWndActive,TRUE,TRUE);
	}
}

//-----------------------------------------------------//
// NextStepTaskProc 
//-----------------------------------------------------//
void NextStepTaskProc(HWND hWndActive,BOOL bForceWait)
{	if(!NextStepWaitProc(hWndActive,bForceWait))
	{	GetmHpVarStruct()->CourantTask->NextmHpStep();
		// mHpProcess --------------------------------------------------------**//
		ComCtrl_mHpProcess(hWndActive,
						   mHpCtrl_bChangeWnd,
						   mHpCtrl_iCurrentWnd,
						   mHpCtrl_iCountWnd);
		//--------------------------------------------------------------------**//
	}
}

//-----------------------------------------------------//
// GetNextPos
//-----------------------------------------------------//
void GetNextPos(HWND hWndActive)
{	int iStartRes;
	KillTimer(hWndActive,ID_TIMER_WAIT);
	KillTimer(hWndActive,ID_TIMER_MAXDELAY);
	GetmHpVarStruct()->DelayLoop=0;
	// StartProc -------------------------------------------------------------**//
	iStartRes=ComCtrl_StartProc(hWndActive);
	//------------------------------------------------------------------------**//
	ProcessStartProcRes(hWndActive,iStartRes);
}

//******************************************************************************************************************TIMER//
// mHpTimerProc
//-----------------------------------------------------//
void CALLBACK mHpTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{	if(uMsg==WM_TIMER)
	{	switch(idEvent)
		{	case ID_TIMER_MAXDELAY:
			{	KillTimer(hwnd,ID_TIMER_MAXDELAY);
				if(GetmHpVarStruct()->DelayLoop!=GetmHpVarStruct()->imHpDelay)
				{	GetmHpVarStruct()->DelayLoop++;
					SetTimer(hwnd,ID_TIMER_MAXDELAY,60000,(TIMERPROC)mHpTimerProc);
				}
				else ViewCtrlNotReadyMsg(hwnd,3);
				break;
			}
			case ID_TIMER_RESTART:
			{	KillTimer(hwnd,ID_TIMER_RESTART);
				StartmHelp(hwnd,FALSE,FALSE);
				break;
			}
			case ID_TIMER_ENDTASK:
			{	KillTimer(hwnd,ID_TIMER_ENDTASK);
				NextRepeatAssistProc(hwnd);
				break;
			}
			case ID_TIMER_START:
			case ID_TIMER_AUTOASS:
			{	KillTimer(hwnd,ID_TIMER_START);
				KillTimer(hwnd,ID_TIMER_AUTOASS);
				StartmHelp(hwnd,TRUE,FALSE);
				break;
			}
			case ID_TIMER_STOP:
			{	KillTimer(hwnd,ID_TIMER_STOP);
				StopmHelp(hwnd,TRUE,TRUE);
				break;
			}
			case ID_TIMER_DELAI:
			{	KillTimer(hwnd,ID_TIMER_DELAI);
				NextStepTaskProc(hwnd,FALSE);
				break;
			}
			case ID_TIMER_WAIT:
			{	KillTimer(hwnd,ID_TIMER_WAIT);
				if(!UpdateFrame(hwnd))
                    StopmHelp(hwnd,TRUE,TRUE);
				else if(GetmHpVarStruct()->bWaitCtrl)
				{	if(!NextStepWaitProc(hwnd,FALSE))
					{	KillTimer(hwnd,ID_TIMER_MAXDELAY);
						GetmHpVarStruct()->bWaitCtrl=FALSE;
						// Direct2D!!
						SetDDrawObj(hwnd,0);
						EndObjects(hwnd);
						GetmHpVarStruct()->DelayLoop=0;
						SetTimer(hwnd,ID_TIMER_MAXDELAY,60000,(TIMERPROC)mHpTimerProc);
						if(!DoInit(hwnd)) StopmHelp(hwnd,TRUE,TRUE);
						else SetTimer(hwnd,ID_TIMER_DDRAW,3,(TIMERPROC)mHpTimerProc);
					}
				}
				else NextStepTaskProc(hwnd,FALSE);
				break;
			}
			case ID_TIMER_NEXT:
			{	KillTimer(hwnd,ID_TIMER_NEXT);
				if(!mHpPak_IsWaitingCursor()) mHpCtrl_bUserAction=TRUE;
				else SetTimer(hwnd,ID_TIMER_NEXT,1,(TIMERPROC)mHpTimerProc);
				break;
			}
			case ID_TIMER_DDRAW:
			{	KillTimer(hwnd,ID_TIMER_DDRAW);
                if(GetmHpVarStruct()->bBlockUpdateFrame) NextStepTaskProc(hwnd,FALSE);
				else
				{	if(!UpdateFrame(hwnd))
                        StopmHelp(hwnd,TRUE,TRUE);
                    else if((!GetmHpVarStruct()->bRefreshDDraw)&&
                            //(GetmHpVarStruct()->XnextPos==GetmHpVarStruct()->XposCur)&&
					        //(GetmHpVarStruct()->YnextPos==GetmHpVarStruct()->YposCur)&&
                            (GetmHpVarStruct()->bMoveDone)&&
                            (!GetmHpVarStruct()->bLockAnim))
					{	if((!GetmHpVarStruct()->DDrawBitmap)&&(!GetmHpVarStruct()->CursorType))
                            Sleep(400);
                        NextStepTaskProc(hwnd,FALSE);
                    }
					else SetTimer(hwnd,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
				}
				break;
			}
			case ID_TIMER_ASCENS:
			{	KillTimer(hwnd,ID_TIMER_ASCENS);
				AscensTimer(hwnd);
				break;
			}
			case ID_TIMER_SHARED:
			{	KillTimer(hwnd,ID_TIMER_SHARED);
				StartmHelp(hwnd,FALSE,TRUE);
				break;
			}
			// TimerProc ----------------------------------------------------------------------------------------------**//
			default:
			{	ComCtrl_TimerProc(hwnd,idEvent);
				break;
			}
			//---------------------------------------------------------------------------------------------------------**//
		}
	}
}

//-----------------------------------------------------//
// GetmHpTimerID
//-----------------------------------------------------//
UINT GetmHpTimerID(int iIndex)
{	switch(iIndex)
	{	case 0: return ID_TIMER_START;		// START
		case 1: return ID_TIMER_STOP;		// STOP
		case 2: return ID_TIMER_DELAI;		// DELAI
		case 3: return ID_TIMER_WAIT;		// WAIT
		case 4: return ID_TIMER_AUTOASS;	// AUTOASS
		case 5: return ID_TIMER_DDRAW;		// DDRAW
		case 6: return ID_TIMER_NEXT;		// NEXT
		case 7: return ID_TIMER_ASCENS;		// ASCENS
		case 8: return ID_TIMER_SHARED;		// SHARED
		// mHpCtrl's timer ID ----------------------------------------------**//
		case 9: return ID_TIMER_MENU;		// MENU
		case 10: return ID_TIMER_LISTE;		// LISTE
		case 11: return ID_TIMER_EDIT;		// EDIT
		case 12: return ID_TIMER_COMBO;		// COMBO
		case 13: return ID_TIMER_CTRLMAN;	// CTRLMAN
		//------------------------------------------------------------------**//
	}
	return NO;
}

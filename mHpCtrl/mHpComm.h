// mHpComm.h
//

#ifndef __MHPCOMMON_H   //***********************************************************************************************//
#define __MHPCOMMON_H

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif

#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif

#ifndef  __COMMCTRL_H
 #include <commctrl.h>
#endif

#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif

//-----------------------------------------------------------------------------------------------------------------------//
// Macros
#define		IsWndStyle(pWndCtrl,lStyle)		((long)(GetWindowLongPtr(pWndCtrl,GWL_STYLE)&lStyle))
#define		IsWndExStyle(pWndCtrl,lExStyle)	((long)(GetWindowLongPtr(pWndCtrl,GWL_EXSTYLE)&lExStyle))
#define		IsControlReady(WndCtrl)		    ((BOOL)(IsWindowEnabled(WndCtrl)&IsWindowVisible(WndCtrl)))

// In mHpCtrl's class name ---------------------------**//
#define		CPP_TASK_TOOLBAR		"ToolbarWindow32"
#define		VB_TASK_TOOLBAR_A		"ToolbarWndClass"
#define		VB_TASK_TOOLBAR_B		"msvb_lib_toolbar"
//---------------------------------------------------**//

//-----------------------------------------------------------------------------------------------------------------------//
// Définitions
#define		ID_TIMER_MAXDELAY			97
#define		ID_TIMER_RESTART			98
#define		ID_TIMER_ENDTASK			99
#define		ID_TIMER_START				100 // 0
#define		ID_TIMER_STOP				101 // 1
#define		ID_TIMER_DELAI				102 // 2
#define		ID_TIMER_WAIT				103 // 3
#define		ID_TIMER_AUTOASS			104 // 4
#define		ID_TIMER_DDRAW				105 // 5
#define		ID_TIMER_NEXT				106 // 6
#define		ID_TIMER_ASCENS				107 // 7
#define		ID_TIMER_SHARED				108 // 8

// mHpCtrl's timer ----------------------------------**//
#define		ID_TIMER_MENU				109 // 9
#define		ID_TIMER_LISTE				110 // 10
#define		ID_TIMER_EDIT				111 // 11
#define		ID_TIMER_COMBO				112 // 12
#define		ID_TIMER_CTRLMAN			113 // 13

//---------------------------------------------------**//
#define		ITEM_ECART					13
#define		ASCENS_ECART				16
//-----------------------------------------------------//
#define		STRING_ZERO					"0"
#define		STRING_ONE					"1"
#define		STRING_TWO					"2"

//----------------------------------------------------------------------------------------------------------------GENERAL//
// Déclarations globales
HWND GetCtrlWnd(HWND,int*);
HWND GetToolCtrlWnd(HWND,int*);
void ForceMessage(HWND,DWORD);
void ForceNotifyMsg(HWND,UINT);
void ForceVHScroll(HWND,UINT,long,BOOL);
void SetRealCursor(int,int,BOOL);
BOOL IsCtrlReady(HWND,HWND);

// Non Générique //////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetItemsInVisu(HWND,BOOL);
void ComboListBoxStatusPos(HWND,BOOL&,BOOL,int);
void ListTreeStatusPos(HWND,RECT);
void GetAscensNextPos(HWND,BOOL,BOOL);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ViewCtrlNotReadyMsg(HWND,int);
void GetCommonEvent(HWND,int&,int&,int&,char*,int,char*,int,char&,int&);
void GetSelectedString(HWND,char*,char*,int,BOOL&,BOOL&);
BOOL GetNextAutoAss();
void AddItemList(HWND,LPSTR,int,int);
//void RefreshControl(HWND,UINT);
void HideLockCursor(HWND);
BOOL WriteCommonTask(HWND,CEventListe*,CWindowListe*,char*,int);
void WriteImmediatTask(HWND,char*,HWND,HWND,WPARAM,int,int,UINT,CEventListe*,ImHpCtrlMgmt*,UINT,char*,UINT&,int,int,int&);
void LinkItems(HWND);
void SetEditStyleAsNumber(HWND);
void FillLnkCtrlLst(HWND,int,WORD,WORD);
void ReadmHpCtrlID(HWND,char*,int*,int,char&);
void AddmHpCtrlID(HWND,int*,int,UINT);
//void AddToolCtrlID(HWND,HWND,int*,int);
void CreateTaskObjects();
void CreateItemListe(HWND,BOOL&);
void ApplyLnkCtrlChange(HWND,HWND,BOOL);
void ProcessStartProcRes(HWND,int);

//----------------------------------------------------------------------------------------------------------------CUSTOMS//
BOOL DidItSelected(int);
void GetStrEvoluParam(int&,int,int,char*,int);
int GetIntBoolEvoluParam(int,int,int);

//-----------------------------------------------------------------------------------------------------------------CREATE//
void StartmHelp(HWND,BOOL,BOOL);
void StopmHelp(HWND,BOOL,BOOL);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void NextRepeatAssistProc(HWND);
void NextStepTaskProc(HWND,BOOL);
void GetNextPos(HWND);

//------------------------------------------------------------------------------------------------------------------TIMER//
void CALLBACK mHpTimerProc(HWND,UINT,UINT,DWORD);
UINT GetmHpTimerID(int);

#endif //****************************************************************************************************************//

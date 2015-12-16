// ComCtrl.h
//

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

#include "resource.h"

//---------------------------------------------------------------------------//
// Constantes
// mHpCtrl's view name ----------------------**//
const static UINT CtrlTabName[19]=
{	IDS_VIEW_CHANGEEXE,
	IDS_VIEW_MENU,
	IDS_VIEW_BUTTON,
	IDS_VIEW_LIST_SINGLE,
	IDS_VIEW_COMBO,
	IDS_VIEW_SLIDER,
	IDS_VIEW_EDIT,
	IDS_VIEW_CHECK,
	IDS_VIEW_RADIO,
	IDS_VIEW_LIST_MULTI,
	IDS_VIEW_TABCTRL,
	IDS_VIEW_LISTVIEW,
	IDS_VIEW_LISTVIEW,
	IDS_VIEW_TREEVIEW,
	IDS_VIEW_TOOLBAR_BTN,
	IDS_VIEW_TOOLBAR_CHK,
	IDS_VIEW_TOOLBAR_RAD,
	IDS_VIEW_TOOLBAR_CBO,
	IDS_VIEW_TOOLBAR_DDW
};
//-------------------------------------------**//

//--------------------------------------------------------------------GENERAL//
void AddIfChangeExe(CEventListe*,char*);
void ReadEventKeys(CEventListe*);
CmHpTask* CreateChangeExeObj(BOOL,char*,char*,char*);

//--------------------------------------------------------------------COMCTRL//
BOOL ComCtrl_AddDotNetCtrlNotify(HWND);
BOOL ComCtrl_AddPlatformCtrlNotify(HWND);
//BOOL ComCtrl_IsNoNotifyDotNetCtrl(HWND);
//BOOL ComCtrl_LockNoNotifyDotNetCtrlMsg(UINT,LPARAM);
void ComCtrl_GetmHpID(HWND,HWND,CEventListe*,UINT,int&,char*);
void ComCtrl_AddIcon(HIMAGELIST);
void ComCtrl_AddCommonUnEvnt();
int ComCtrl_ItemIndexToSel(HWND);

//--------------------------------------------------------------------MHPFILE//
BOOL ComCtrl_ReadEvent(HWND,int,UINT,int,int,int,UINT&);
void ComCtrl_WriteTask(HWND,int&,char*,int,CEventListe*,CWindowListe*,BOOL,int&,BOOL&);
BOOL ComCtrl_WriteParam(HWND,int,CEventListe*);

//-----------------------------------------------------------------------KEYS//
void ComCtrl_ReadEventKeys(int,int,CEventListe*,CTaskListe*);

//------------------------------------------------------------------------ADD//
void ComCtrl_AddEvent(HWND,char*,ImHpCtrlMgmt*,UINT,HWND,WPARAM,int,int&,int,int,int);

//-----------------------------------------------------------------PROPERTIES//
void ComCtrl_IsCustom(CEventCell*,int&,BOOL&);
void ComCtrl_StrState(CEventCell*,BOOL,char*,int);
int ComCtrl_ParamType(CEventCell*);
void ComCtrl_DefaultBound(CEventCell*,char*,int);
BOOL ComCtrl_IsCustomized(CEventCell*,int,BOOL);
BOOL ComCtrl_IsNoStateMsg(CEventCell*);
void ComCtrl_GetHtmlTag(CEventCell*,char*,UINT,int);

//-------------------------------------------------------------------CFGTOOLS//
void ComCtrl_ShowCfgTools(HWND);
void ComCtrl_HideCfgTools(HWND);
void ComCtrl_SaveCfg(HWND,BOOL&);
void ComCtrl_LoadCfg(HWND,HWND,HWND,HWND);
void ComCtrl_GetInfosCfg(HWND,HWND,HWND,HWND,HWND,HWND,HWND,HWND,HWND,HWND);
BOOL ComCtrl_ProcCfg(HWND,UINT,WPARAM,LPARAM,HWND,HWND,HWND);

//-----------------------------------------------------------------------HTML//
void ComCtrl_SetStartPostData(const std::string& strPostData);

//--------------------------------------------------------------------PROCESS//
void ComCtrl_CaptureMsg(HWND,UINT,WPARAM,LPARAM,BOOL&,int,HWND,char*,CEventListe*,int&,UINT&,int,int);
void ComCtrl_mHpProcess(HWND,BOOL&,int,int);
int ComCtrl_StartProc(HWND);

//------------------------------------------------------------------TIMERPROC//
void ComCtrl_TimerProc(HWND,UINT);

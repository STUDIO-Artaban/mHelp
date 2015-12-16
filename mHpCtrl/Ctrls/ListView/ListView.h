// ListView.h
//

#ifndef  __LISTVIEWCTRL_H //*********************************************************************************************//
#define  __LISTVIEWCTRL_H

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

//-----------------------------------------------//
// Parent object
#include <ListBox\ListCtrl.h>

//-----------------------------------------------------------------------------------------------------------------OBJECT//
class CListViewTask : public CListTask
{	public:
		CListViewTask(int,char*,char*,int,int,int,int*,char*);
		CListViewTask();
};

//----------------------------------------------------------------------------------------------------------------GENERAL//
int MListView_ItemIndexToSel(HWND);

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL ListView_ReadEvent(HWND,int,const std::string&,UINT&);
void SListView_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
void MListView_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
BOOL SListView_WriteParam(HWND,CEventListe*,int);
BOOL MListView_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void ListView_ReadEventKeys(int,CEventListe*,CTaskListe*);

//--------------------------------------------------------------------------------------------------------------------ADD//
void SListView_AddEvent(HWND,char*,HWND,int,int,BOOL);
void MListView_AddEvent(HWND,char*,HWND,int,int,BOOL);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
BOOL SListView_IsClassName(HWND,char*,int);
BOOL MListView_IsClassName(HWND,char*,int);
void SListView_IsCustom(BOOL&);
void MListView_IsCustom(BOOL&);
void SListView_StrState(CEventCell*,char*,int);
void MListView_StrState(CEventCell*,BOOL,char*,int);
int SListView_ParamType();
int MListView_ParamType();
void SListView_DefaultBound(char*,int);
void MListView_DefaultBound(char*,int);
BOOL SListView_IsCustomized();
BOOL MListView_IsCustomized();
BOOL SListView_IsNoStateMsg();
BOOL MListView_IsNoStateMsg();
void SListView_GetHtmlTag(CEventCell*,char*,UINT,int);
void MListView_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void SListView_ShowCfgTools(HWND);
void MListView_ShowCfgTools(HWND);
void SListView_HideCfgTools(HWND);
void MListView_HideCfgTools(HWND);
void SListView_SaveCfg(HWND);
void MListView_SaveCfg(HWND);
void SListView_LoadCfg(HWND);
void MListView_LoadCfg(HWND);
void SListView_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
void MListView_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL SListView_ProcCfg(UINT,WPARAM,HWND);
BOOL MListView_ProcCfg(HWND,UINT,WPARAM,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void ListView_CaptureMsg(HWND,char*,WPARAM,LPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,int,BOOL&,int&);
void SListView_mHpProcess(HWND,BOOL&,int,int);
BOOL SListView_StartProc(HWND);
void MListView_mHpProcess(HWND,BOOL&,int,int);
BOOL MListView_StartProc(HWND);

#endif //****************************************************************************************************************//

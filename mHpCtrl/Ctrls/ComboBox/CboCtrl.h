// CboCtrl.h
//

#ifndef  __COMBOCTRL_H //************************************************************************************************//
#define  __COMBOCTRL_H

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

//----------------------------------------------------------------------------------------------------------------OBJECTS//
class CComboTask : public CListTask
{	public:
		CComboTask(char*,char*,int,int,int,int*,char*);
		CComboTask();
};
//-----------------------------------------------//
class CComboWrite : public CStyleWrite
{	public:
		CComboWrite(int);
		CComboWrite();
};

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL Combo_ReadEvent(HWND,const std::string&,UINT&);
void Combo_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
BOOL Combo_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void Combo_ReadEventKeys(int,CEventListe*,CTaskListe*);

//--------------------------------------------------------------------------------------------------------------------ADD//
void Combo_AddEvent(HWND,char*,HWND,int,int,BOOL);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
BOOL Combo_IsClassName(HWND,char*,int);
void Combo_IsCustom(BOOL&);
void Combo_StrState(CEventCell*,char*,int);
int Combo_ParamType();
void Combo_DefaultBound(char*,int);
BOOL Combo_IsCustomized();
BOOL Combo_IsNoStateMsg(CEventCell*);
void Combo_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void Combo_ShowCfgTools(HWND);
void Combo_HideCfgTools(HWND);
void Combo_SaveCfg(HWND);
void Combo_LoadCfg(HWND);
void Combo_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL Combo_ProcCfg(UINT,WPARAM,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void Combo_CaptureMsg(HWND,char*,HWND,WPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,BOOL&,int&);
void Combo_mHpProcess(HWND,BOOL&,int,int);
BOOL Combo_StartProc(HWND);

//--------------------------------------------------------------------------------------------------------------TIMERPROC//
void Combo_TimerProc(HWND);

#endif //****************************************************************************************************************//
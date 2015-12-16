// BtnCtrl.h
//

#ifndef  __BUTTONCTRL_H //***********************************************************************************************//
#define  __BUTTONCTRL_H

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

//-----------------------------------------------------//
// Class name
#define     DOTNET_TASK_BUTTON          "WindowsForms10.BUTTON.app" // DotNet control

//------------------------------------------------------------------------------------------------OBJECTS//
class CButtonTask : public CmHpTask
{	private:
		int iBtnState;
	public:
		CButtonTask(int,char*,char*,int,int,int,int*,char*,int);
		CButtonTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		int GetBtnState();
		void SetBtnState(int);
};
//-------------------------------------------------//
class CRadioWrite : public CmHpWrite
{	private:
		char lpRadName[MAX_RS_SIZE];
		int iLinked;
		int iRadioRow;
	public:
		CRadioWrite();
		CRadioWrite(char*);
		BOOL ReadEventKey(int);
		BOOL WriteEventKey();
		int GetLinked();
		void SetLinked(int);
		int GetRadioRow();
		void SetRadioRow(int);
		char* GetRadioName();
};
//-------------------------------------------------//
class CCheckWrite : public CStyleWrite
{	public:
		CCheckWrite(int);
		CCheckWrite();
};

//------------------------------------------------------------------------------------------------MHPFILE//
BOOL Button_ReadEvent(HWND);
BOOL CheckRadio_ReadEvent(HWND,int);
void Button_WriteEvent(HWND,int,CEventListe*,CWindowListe*,char*,BOOL&);
void Check_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
void Radio_WriteEvent(HWND,int&,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&,BOOL&);
BOOL Button_WriteParam();
BOOL Check_WriteParam(HWND,CEventListe*,int);
BOOL Radio_WriteParam(HWND,CEventListe*,int);

//---------------------------------------------------------------------------------------------------KEYS//
void Button_ReadEventKeys(int,CEventListe*,CTaskListe*);
void Check_ReadEventKeys(int,CEventListe*,CTaskListe*);
void Radio_ReadEventKeys(int,CEventListe*,CTaskListe*);

//----------------------------------------------------------------------------------------------------ADD//
void Button_AddEvent(HWND,char*,UINT,int,int);
void Check_AddEvent(HWND,char*,UINT,int,int,BOOL);
void Radio_AddEvent(HWND,char*,HWND,int,int,BOOL);

//---------------------------------------------------------------------------------------------PROPERTIES//
BOOL Button_IsClassName(HWND,char*,int);
BOOL Check_IsClassName(HWND,char*,int);
BOOL Radio_IsClassName(HWND,char*,int);
void Button_IsCustom(int&);
void Check_IsCustom(BOOL&);
void Radio_IsCustom(CEventCell*,BOOL&);
void Button_StrState(char*,int);
void Check_StrState(CEventCell*,BOOL,char*,int);
void Radio_StrState(CEventCell*,BOOL,char*,int);
int Button_ParamType();
int Check_ParamType();
int Radio_ParamType();
void Button_DefaultBound();
void Check_DefaultBound(char*,int);
void Radio_DefaultBound(CEventCell*,char*,int);
BOOL Button_IsCustomized();
BOOL Check_IsCustomized();
BOOL Radio_IsCustomized(CEventCell*,int,BOOL);
BOOL Button_IsNoStateMsg();
BOOL Check_IsNoStateMsg();
BOOL Radio_IsNoStateMsg();
void Button_GetHtmlTag(CEventCell*,char*,UINT,int);
void Check_GetHtmlTag(CEventCell*,char*,UINT,int);
void Radio_GetHtmlTag(CEventCell*,char*,UINT,int);

//-----------------------------------------------------------------------------------------------CFGTOOLS//
void Check_ShowCfgTools(HWND);
void Radio_ShowCfgTools(HWND);
void Check_HideCfgTools(HWND);
void Radio_HideCfgTools(HWND);
void Check_SaveCfg(HWND);
void Radio_SaveCfg(HWND,BOOL&);
void Check_LoadCfg(HWND);
void Radio_LoadCfg(HWND,HWND,HWND,HWND);
void Button_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
void Check_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
void Radio_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL Check_ProcCfg(HWND,UINT,WPARAM,LPARAM,HWND);
BOOL Radio_ProcCfg(HWND,UINT,WPARAM,LPARAM,HWND,HWND,HWND);

//------------------------------------------------------------------------------------------------PROCESS//
void Button_CaptureMsg(HWND,char*,HWND,WPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,int,BOOL&,int&);
void Button_mHpProcess(HWND,BOOL&,int,int);
BOOL Button_StartProc(HWND);

#endif //****************************************************************************************************************//
// EditCtrl.h
//

#ifndef  __EDITCTRL_H //*************************************************************************************************//
#define  __EDITCTRL_H

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

//----------------------------------------------------------------------------------------------------------------OBJECTS//
class CEditTask : public CmHpTask
{	private:
		char lpEdit[MAX_RS_SIZE];
		int iUserFixed;
	public:
		CEditTask(char*,char*,int,int,int,int*,char*,char*);
		CEditTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		char* GetEditValue();
		void SetEditValue(char*);
		int GetUserFixed();
		void SetUserFixed(int);
};
//-----------------------------------------------//
class CEditWrite : public CStyleWrite
{	public:
		CEditWrite(int);
		CEditWrite();
};

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL Edit_ReadEvent(HWND,const std::string&,UINT&);
void Edit_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
BOOL Edit_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void Edit_ReadEventKeys(int,CEventListe*,CTaskListe*);

//--------------------------------------------------------------------------------------------------------------------ADD//
void Edit_AddEvent(HWND,char*,UINT,int,int,BOOL);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
BOOL Edit_IsClassName(char*,int);
void Edit_IsCustom(CEventCell*,BOOL&);
void Edit_StrState(CEventCell*,char*,int);
int Edit_ParamType();
void Edit_DefaultBound(char*,int);
BOOL Edit_IsCustomized(CEventCell*,BOOL);
BOOL Edit_IsNoStateMsg();
void Edit_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void Edit_ShowCfgTools(HWND);
void Edit_HideCfgTools(HWND);
void Edit_SaveCfg(HWND);
void Edit_LoadCfg(HWND);
void Edit_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL Edit_ProcCfg(HWND,UINT,WPARAM,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void Edit_CaptureMsg(HWND,char*,WPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,BOOL&,int&);
void Edit_mHpProcess(HWND,BOOL&,int,int);
BOOL Edit_StartProc(HWND);

//--------------------------------------------------------------------------------------------------------------TIMERPROC//
void Edit_TimerProc(HWND);

#endif //****************************************************************************************************************//
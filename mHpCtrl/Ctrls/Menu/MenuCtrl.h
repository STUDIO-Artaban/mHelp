// MenuCtrl.h
//

#ifndef  __MENUCTRL_H //*************************************************************************************************//
#define  __MENUCTRL_H

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

//-----------------------------------------------------------------------------------------------------------------OBJECT//
class CMenuTask : public CmHpTask
{	private:
		int iMenuState;
	public:
		CMenuTask(char*,char*,int,int,int,int*,int);
		CMenuTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		int GetMenuState();
};

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL Menu_ReadEvent(HWND);
void Menu_WriteEvent(HWND,int,CEventListe*,CWindowListe*,char*,BOOL&);
BOOL Menu_WriteParam();

//-------------------------------------------------------------------------------------------------------------------KEYS//
void Menu_ReadEventKeys(int,CEventListe*,CTaskListe*);

//--------------------------------------------------------------------------------------------------------------------ADD//
void Menu_AddEvent(HWND,UINT,int,int);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
BOOL Menu_IsClassName(HWND,char*,int,UINT);
void Menu_IsCustom(int&);
void Menu_StrState(CEventCell*,BOOL,char*,int);
int Menu_ParamType();
void Menu_DefaultBound();
BOOL Menu_IsCustomized(CEventCell*,int,BOOL);
BOOL Menu_IsNoStateMsg();
void Menu_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void Menu_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void Menu_CaptureMsg(HWND,char*,WPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,int,BOOL&,int&);
void Menu_mHpProcess(HWND,BOOL&,int,int);
BOOL Menu_StartProc(HWND);

//--------------------------------------------------------------------------------------------------------------TIMERPROC//
void Menu_TimerProc(HWND);

#endif //****************************************************************************************************************//
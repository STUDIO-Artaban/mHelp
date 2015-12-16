// TabCtrl.h
//

#ifndef  __TABCTRL_H //**************************************************************************************************//
#define  __TABCTRL_H

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
class CTabCtrlTask : public CmHpTask
{	private:
		char lpTabSel[MAX_RS_SIZE];
	public:
		CTabCtrlTask(char*,char*,int,int,int,int*,char*,char*);
		CTabCtrlTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		char* GetTabSel();
		void SetTabSel(char*);
};

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL TabCtrl_ReadEvent(HWND,const std::string&,UINT&);
void TabCtrl_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
BOOL TabCtrl_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void TabCtrl_ReadEventKeys(int,CEventListe*,CTaskListe*);

//--------------------------------------------------------------------------------------------------------------------ADD//
void TabCtrl_AddEvent(HWND,char*,HWND,int,int,BOOL);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
BOOL TabCtrl_IsClassName(char*,int);
void TabCtrl_IsCustom(BOOL&);
void TabCtrl_StrState(CEventCell*,char*,int);
int TabCtrl_ParamType();
void TabCtrl_DefaultBound(char*,int);
BOOL TabCtrl_IsCustomized();
BOOL TabCtrl_IsNoStateMsg();
void TabCtrl_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void TabCtrl_ShowCfgTools(HWND);
void TabCtrl_HideCfgTools(HWND);
void TabCtrl_SaveCfg(HWND);
void TabCtrl_LoadCfg(HWND);
void TabCtrl_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL TabCtrl_ProcCfg(UINT,WPARAM,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void TabCtrl_CaptureMsg(HWND,char*,HWND,WPARAM,LPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,BOOL&,int&);
void TabCtrl_mHpProcess(HWND,BOOL&,int,int);
BOOL TabCtrl_StartProc(HWND);

#endif //****************************************************************************************************************//
// ChExCtrl.h
//

#ifndef  __CHANGEEXECTRL_H //********************************************************************************************//
#define  __CHANGEEXECTRL_H

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
class CChangeExeTask : public CmHpTask
{	private:
		char lpmHpExe[MAX_RS_SIZE];
		char lpCmdLine[MAX_RS_SIZE];
		BOOL bNewExe;
		HWND hWndExe;
	public:
		CChangeExeTask(int,BOOL,char*,char*,char*);
		CChangeExeTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		char* GetExeName();
		BOOL GetNewExe();
		void SetHwndExe(HWND);
		HWND GetHwndExe();
		char* GetCommandLine();
		void SetCommandLine(char*);
};

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL ChangeExe_ReadEvent(HWND,const std::string&,UINT&);
void ChangeExe_WriteEvent(HWND,int,CEventListe*,BOOL,char*,int,int&,BOOL&);
BOOL ChangeExe_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void ChangeExe_ReadEventKeys(int,CEventListe*,CTaskListe*);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
void ChangeExe_IsCustom(CEventCell*,int&,BOOL&);
void ChangeExe_StrState(CEventCell*,BOOL,char*,int);
int ChangeExe_ParamType(CEventCell*);
void ChangeExe_DefaultBound(CEventCell*,char*,int);
BOOL ChangeExe_IsCustomized(CEventCell*);
BOOL ChangeExe_IsNoStateMsg();
void ChangeExe_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void ChangeExe_ShowCfgTools(HWND);
void ChangeExe_HideCfgTools(HWND);
void ChangeExe_SaveCfg(HWND);
void ChangeExe_LoadCfg(HWND);
void ChangeExe_GetInfosCfg(HWND,HWND,HWND,HWND,HWND,HWND,HWND,HWND);
BOOL ChangeExe_ProcCfg(UINT,WPARAM,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void ChangeExe_mHpProcess(HWND);
BOOL ChangeExe_StartProc(HWND);

#endif //****************************************************************************************************************//
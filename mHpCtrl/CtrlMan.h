// CtrlMan.h
//

#ifndef  __CTRLMAN_H //******************************************************//
#define  __CTRLMAN_H

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

// ----------------------------------------------
// Définitions

//--------------------------------------------------------------------OBJECTS//
class CCtrlManTask : public CmHpTask
{	private:
		ImHpCtrlMgmt* mHpCtrlMgmt;
        UINT mHpCtrlPlatformID;
	public:
		CCtrlManTask(char*,char*,int,int,int,ImHpCtrlMgmt*,UINT);
		CCtrlManTask();
        virtual ~CCtrlManTask();
        //////
        BOOL ReadCtrlIDKey(HKEY);
        BOOL WriteCtrlIDKey(HKEY) const;
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
        //
        HICON GetCtrlIcon() const;
        void GetCtrlVers(char*,UINT) const;
        void GetCtrlDesc(char*,UINT) const;
        BOOL GetStrState(char*,int,BOOL) const;
        int GetParamType() const;
        ImHpCtrlMgmt* GetCtrlMgmt();
        UINT GetCtrlPlatformID() { return mHpCtrlPlatformID; }
};

// ----------------------------------------------
class CCtrlManWrite : public CmHpWrite
{	private:
		ImHpCtrlMgmt* mHpCtrlMgmt;
        UINT mHpCtrlPlatformID;
    public:
		CCtrlManWrite(ImHpCtrlMgmt*,UINT);
		CCtrlManWrite();
        //////
		BOOL ReadEventKey(int);
		BOOL WriteEventKey();
};

//--------------------------------------------------------------------MHPFILE//
BOOL CtrlMan_ReadEvent(HWND,UINT,int,int,int,const std::string&,UINT&);
void CtrlMan_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
BOOL CtrlMan_WriteParam(HWND,CEventListe*,int);

//-----------------------------------------------------------------------KEYS//
void CtrlMan_ReadEventKeys(int,CEventListe*,CTaskListe*);

//------------------------------------------------------------------------ADD//
void CtrlMan_AddEvent(HWND,ImHpCtrlMgmt*,UINT,char*,UINT,int,int,BOOL);

//-----------------------------------------------------------------PROPERTIES//
void CtrlMan_IsCustom(BOOL&,CEventCell*);
void CtrlMan_StrState(CEventCell*,BOOL,char*,int);
int CtrlMan_ParamType(CEventCell*);
void CtrlMan_DefaultBound(CEventCell*,char*,int);
BOOL CtrlMan_IsCustomized(CEventCell*);
BOOL CtrlMan_IsNoStateMsg(CEventCell*);
void CtrlMan_GetHtmlTag(CEventCell*,char*,UINT,int);

//-------------------------------------------------------------------CFGTOOLS//
void CtrlMan_ShowCfgTools(HWND);
void CtrlMan_HideCfgTools(HWND);
void CtrlMan_SaveCfg(HWND);
void CtrlMan_GetInfosCfg(HWND,HWND,HWND,HWND,HWND,HWND,HWND);
BOOL CtrlMan_ProcCfg(HWND,UINT);

//--------------------------------------------------------------------PROCESS//
void CtrlMan_CaptureMsg(HWND,WPARAM,LPARAM,ImHpCtrlMgmt*,UINT,HWND,int,CEventListe*,char*,UINT&,int,int,BOOL&,int&);
void CtrlMan_mHpProcess(HWND,BOOL&,int,int);
BOOL CtrlMan_StartProc(HWND);

//------------------------------------------------------------------TIMERPROC//
void CtrlMan_TimerProc(HWND);

#endif //********************************************************************//

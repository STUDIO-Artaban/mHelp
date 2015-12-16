// ToolBar.h
//

#ifndef  __TOOLBARCTRL_H //**********************************************************************************************//
#define  __TOOLBARCTRL_H

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
#include <button\btnctrl.h>
#include <combobox\cboctrl.h>
#include <menu\menuctrl.h>

//-----------------------------------------------------------------------------------------------------------------------//
// Macros
#define		ToolBar_IsItDropDown(EvntLst)	((BOOL)(((EvntLst->GetEventCount()-2)>=0)&&\
													 (EvntLst->GetCellFromPos((EvntLst->GetEventCount()-2),FALSE)!=NULL)&&\
													 (EvntLst->GetCellFromPos((EvntLst->GetEventCount()-2),FALSE)->mHpTask->GetmHpID()==18)&&\
													 (EvntLst->GetCellFromPos((EvntLst->GetEventCount()-2),FALSE)->mHpTask->GetMenuSelect()==0)))

//----------------------------------------------------------------------------------------------------------------OBJECTS//
class CToolBarTask : public CmHpTask
{	private:
		UINT CommandID;
	public:
		CToolBarTask(int,char*,char*,int,int,int,int*,UINT);
		CToolBarTask();
		UINT GetCommandID();
		BOOL ReadCommonKey(int,HKEY&,HKEY&);
		BOOL WriteCommonKey(HKEY&,HKEY&);
		// Virtual --------------------------------//
		virtual BOOL ReadTaskKey(int);
		virtual BOOL WriteTaskKey();
		// CToolChckTask
		virtual int GetBtnState();
		virtual void SetBtnState(int);
		// CToolCboTask
		virtual void AddMultiIndex(char*);
		virtual char* GetMultiIndex(int);
		virtual void InitMultiIndex();
		// CToolDrDwTask
		virtual UINT GetMenuSelect();
		virtual void SetMenuSelect(UINT);
		//-----------------------------------------//
};
//-------------------------------------------------//
class CToolBtnTask : public CToolBarTask
{	public:
		CToolBtnTask(char*,int,int,int,int*,UINT);
		CToolBtnTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
};
//-------------------------------------------------//
class CToolChckTask : public CToolBarTask
{	private:
		int iChkState;
	public:
		CToolChckTask(char*,char*,int,int,int,int*,UINT,int);
		CToolChckTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		int GetBtnState();
		void SetBtnState(int);
};
//-------------------------------------------------//
class CToolRadTask : public CToolBarTask
{	public:
		CToolRadTask(char*,char*,int,int,int,int*,UINT);
		CToolRadTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
};
//-------------------------------------------------//
class CToolCboTask : public CToolBarTask
{	private:
		char lpToolCbo[MAX_RS_SIZE];
	public:
		CToolCboTask(char*,char*,int,int,int,int*,UINT,char*);
		CToolCboTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		void AddMultiIndex(char*);
		char* GetMultiIndex(int);
		void InitMultiIndex();
};
//-------------------------------------------------//
class CToolDrDwTask : public CToolBarTask
{	private:
		UINT MenuSelID;
	public:
		CToolDrDwTask(char*,int,int,int,int*,UINT,UINT);
		CToolDrDwTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		UINT GetMenuSelect();
		void SetMenuSelect(UINT);
};
//-----------------------------------------------------------------------------------------------------------------------//
class CToolRadWrite : public CRadioWrite
{	public:
		CToolRadWrite();
};
//-------------------------------------------------//
class CToolCboWrite : public CStyleWrite
{	public:
		CToolCboWrite(int);
		CToolCboWrite();
};

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL ToolBar_ReadEvent(HWND,int,const std::string&,UINT&);
void ToolBar_WriteEvent(HWND,int&,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&,BOOL&);
BOOL ToolBar_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void ToolBar_ReadEventKeys(int,CEventListe*,CTaskListe*,int);

//--------------------------------------------------------------------------------------------------------------------ADD//
void ToolBar_AddEvent(HWND,HWND,UINT,int,int,BOOL,int);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
int ToolBar_IsClassName(HWND,char*,int,UINT);
void ToolBar_IsCustom(CEventCell*,BOOL&,int&);
void ToolBar_StrState(CEventCell*,BOOL,char*,int);
int ToolBar_ParamType(int);
void ToolBar_DefaultBound(CEventCell*,char*,int);
BOOL ToolBar_IsCustomized(CEventCell*,int,BOOL);
BOOL ToolBar_IsNoStateMsg(CEventCell*);
void ToolBar_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void ToolBar_ShowCfgTools(HWND);
void ToolBar_HideCfgTools(HWND);
void ToolBar_SaveCfg(HWND,BOOL&);
void ToolBar_LoadCfg(HWND,HWND,HWND,HWND);
void ToolBar_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL ToolBar_ProcCfg(HWND,UINT,WPARAM,LPARAM,HWND,HWND,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void ToolBar_CaptureMsg(HWND,HWND,LPARAM,WPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,int,BOOL&,int&);
void ToolBar_mHpProcess(HWND,BOOL&,int,int);
BOOL ToolBar_StartProc(HWND);

#endif //****************************************************************************************************************//

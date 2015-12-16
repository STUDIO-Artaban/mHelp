// TreeView.h
//

#ifndef  __TREEVIEWCTRL_H //*********************************************************************************************//
#define  __TREEVIEWCTRL_H

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif

#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif

#ifndef  __COMMCTRL_H
 #include <commctrl.h>
#endif

//-----------------------------------------------//
// Parent object
#include <listbox\listctrl.h>

//-----------------------------------------------------//
// Class name
#define     DOTNET_TASK_TREEVIEW        "WindowsForms10.SysTreeView32.app"  // DotNet control

//-----------------------------------------------------------------------------------------------------------------OBJECT//
class CTreeViewTask : public CListTask
{	public:
		CTreeViewTask(char*,char*,int,int,int,int*,char*);
		CTreeViewTask();
};

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL TreeView_ReadEvent(HWND,const std::string&,UINT&);
void TreeView_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
BOOL TreeView_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void TreeView_ReadEventKeys(int,CEventListe*,CTaskListe*);

//--------------------------------------------------------------------------------------------------------------------ADD//
void TreeView_AddEvent(HWND,char*,HWND,WPARAM,int,int,BOOL);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
BOOL TreeView_IsClassName(char*,int);
void TreeView_IsCustom(BOOL&);
void TreeView_StrState(CEventCell*,BOOL,char*,int);
int TreeView_ParamType();
void TreeView_DefaultBound(char*,int);
BOOL TreeView_IsCustomized();
BOOL TreeView_IsNoStateMsg();
void TreeView_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void TreeView_ShowCfgTools(HWND);
void TreeView_HideCfgTools(HWND);
void TreeView_SaveCfg(HWND);
void TreeView_LoadCfg(HWND);
void TreeView_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL TreeView_ProcCfg(HWND,UINT,WPARAM,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void TreeView_CaptureMsg(HWND,char*,WPARAM,LPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,BOOL&,int&);
void TreeView_mHpProcess(HWND,BOOL&,int,int);
BOOL TreeView_StartProc(HWND);

#endif //****************************************************************************************************************//

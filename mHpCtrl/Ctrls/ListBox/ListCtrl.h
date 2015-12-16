// ListCtrl.h
//

#ifndef  __LISTCTRL_H //*************************************************************************************************//
#define  __LISTCTRL_H

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
// ValueKeys
#define		LISTSEL_KEY					"ListSel"

//-----------------------------------------------------------------------------------------------------------------OBJECT//
class CListTask : public CmHpTask
{	private:
		CTypeCell* HeadSel;
		CTypeCell* CurrentSel;
	public:
		CListTask(int,char*,char*,int,int,int,int*,char*);
		CListTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		void AddMultiIndex(char*);
		BOOL GetNextIndex();
		char* GetMultiIndex(int);
		void InitMultiIndex();
		void InverseMultiIndex();
};

//----------------------------------------------------------------------------------------------------------------GENERAL//
int MultiList_ItemIndexToSel(HWND);

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL List_ReadEvent(HWND,int,const std::string&,UINT&);
void SingleList_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
void MultiList_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
BOOL SingleList_WriteParam(HWND,CEventListe*,int);
BOOL MultiList_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void List_ReadEventKeys(int,CEventListe*,CTaskListe*);

//--------------------------------------------------------------------------------------------------------------------ADD//
void SingleList_AddEvent(HWND,char*,HWND,int,int,BOOL);
void MultiList_AddEvent(HWND,char*,HWND,int,int,BOOL);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
BOOL SingleList_IsClassName(HWND,char*,int);
BOOL MultiList_IsClassName(HWND,char*,int);
void SingleList_IsCustom(BOOL&);
void MultiList_IsCustom(BOOL&);
void SingleList_StrState(CEventCell*,char*,int);
void MultiList_StrState(CEventCell*,BOOL,char*,int);
int SingleList_ParamType();
int MultiList_ParamType();
void SingleList_DefaultBound(char*,int);
void MultiList_DefaultBound(char*,int);
BOOL SingleList_IsCustomized();
BOOL MultiList_IsCustomized();
BOOL SingleList_IsNoStateMsg();
BOOL MultiList_IsNoStateMsg();
void SingleList_GetHtmlTag(CEventCell*,char*,UINT,int);
void MultiList_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void SingleList_ShowCfgTools(HWND);
void MultiList_ShowCfgTools(HWND);
void SingleList_HideCfgTools(HWND);
void MultiList_HideCfgTools(HWND);
void SingleList_SaveCfg(HWND);
void MultiList_SaveCfg(HWND);
void SingleList_LoadCfg(HWND);
void MultiList_LoadCfg(HWND);
void SingleList_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
void MultiList_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL SingleList_ProcCfg(UINT,WPARAM,HWND);
BOOL MultiList_ProcCfg(HWND,UINT,WPARAM,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void List_CaptureMsg(HWND,char*,HWND,WPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,int,BOOL&,int&);
void SingleList_mHpProcess(HWND,BOOL&,int,int);
BOOL SingleList_StartProc(HWND);
void MultiList_mHpProcess(HWND,BOOL&,int,int);
BOOL MultiList_StartProc(HWND);

//--------------------------------------------------------------------------------------------------------------TIMERPROC//
void List_TimerProc(HWND);

#endif //****************************************************************************************************************//
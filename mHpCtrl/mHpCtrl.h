//---------------------------------------------------------------------------//
// mHpCtrl.h
//
//---------------------------------------------------------------------------//

#ifdef MHPCTRL_EXPORTS
#define MHPCTRL_API __declspec(dllexport)
#else
#define MHPCTRL_API __declspec(dllimport)
#endif

#ifndef   __MHELPCTRL_H //*************************************************//
#define  __MHELPCTRL_H

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

//------------------------------------------------------------------VARIABLES//
extern MHPCTRL_API UINT mHpCtrl_iLastID;			    // ID du dernier controle aillant émit un évènement
extern MHPCTRL_API BOOL mHpCtrl_bChangeWnd;		        // TRUE: "hWndActive" change...
extern MHPCTRL_API int  mHpCtrl_iCurrentWnd;		    // "iWnd" courant
extern MHPCTRL_API int  mHpCtrl_iCountWnd;			    // Nombre de fenêtres activées
extern MHPCTRL_API BOOL mHpCtrl_bUserAction;		    // Autorise la capture d'un tache en mode création
extern MHPCTRL_API BOOL mHpCtrl_bReadmHelp;             // TRUE: mHelp en cours...
extern MHPCTRL_API BOOL mHpCtrl_bWritemHelp;		    // TRUE: Serveur option Create/Add
extern MHPCTRL_API BOOL mHpCtrl_bLockmHelp;             // TRUE: bloque le démarrage de l'aide FALSE: sinon
extern MHPCTRL_API int  mHpCtrl_IndexTask;              // Task index in HTML file

//-------------------------------------------------------------OBJECTS-ACCESS//
class MHPCTRL_API CmHpCtrl 
{	public:
	CmHpCtrl();
	CTaskListe* mHpTaskList;
	CEventListe* mHpEventList;
	void NewTaskList();
	void NewEventList();
	void DeleteTaskList();
	void DeleteEventList();
};

//------------------------------------------------------------METHODES-ACCESS//
MHPCTRL_API BOOL mHpCtrl_IsCustomEvent(CEventListe*);
MHPCTRL_API void mHpCtrl_EmptyChange(CEventListe*);
MHPCTRL_API void mHpCtrl_EmptyEvents(CEventListe*);
MHPCTRL_API BOOL mHpCtrl_IsEmptyListe(CEventListe*);
MHPCTRL_API void mHpCtrl_ChangeEventWnd(CEventListe*,char*);
MHPCTRL_API void mHpCtrl_RemoveLastTask(CEventListe*);

//--------------------------------------------------------------CONFIGURATION//
MHPCTRL_API void mHpCtrl_GetConfig(CTaskListe*,int,int,int,int,char*,int*,char*,BOOL);

//--------------------------------------------------------------------GENERAL//
MHPCTRL_API void mHpCtrl_SetCtrlListHeader(HWND,HIMAGELIST);
MHPCTRL_API void mHpCtrl_SetCustomListHeader(HWND,HIMAGELIST);
MHPCTRL_API void mHpCtrl_FillCtrlList(HWND,CEventListe*);
MHPCTRL_API void mHpCtrl_FillCustomList(HWND,CEventListe*);
MHPCTRL_API void mHpCtrl_EnableCfgTools(CEventListe*,int,HWND,HWND);
MHPCTRL_API void mHpCtrl_MoveUp(CEventListe*,int);
MHPCTRL_API void mHpCtrl_MoveDown(CEventListe*,int);
MHPCTRL_API int  mHpCtrl_CreateCtrlCfgBox(HWND,CEventListe*,int,BOOL);
MHPCTRL_API void mHpCtrl_HideLockCursor(HWND);
MHPCTRL_API void mHpCtrl_GethWndActive(HWND);
MHPCTRL_API BOOL mHpCtrl_IsCustomVisible(CEventListe*,int);
MHPCTRL_API void mHpCtrl_ChangeCustomVisible(CEventListe*,int);
MHPCTRL_API void mHpCtrl_ViewDiffWndMsg(HWND);
MHPCTRL_API void mHpCtrl_FreemHpStruct();
MHPCTRL_API void mHpCtrl_AddCommonUnEvnt();
MHPCTRL_API void mHpCtrl_CreateCtrlMgmtList(HWND);
MHPCTRL_API BOOL mHpCtrl_FillCtrlMgmtImgList(HIMAGELIST);
MHPCTRL_API BOOL mHpCtrl_FillCtrlMgmtListView(HWND);
MHPCTRL_API void mHpCtrl_FreeCtrlMgmtList();

//-----------------------------------------------------------------------KEYS//
MHPCTRL_API void mHpCtrl_RefreshEventKeys(CEventListe*);
MHPCTRL_API BOOL mHpCtrl_WriteHwndExeKey(HWND,char*);
MHPCTRL_API int  mHpCtrl_ReadEventKeys(CEventListe*,BOOL);
MHPCTRL_API int  mHpCtrl_ReadTaskKeys(HWND,CTaskListe*,char*,BOOL);
MHPCTRL_API void mHpCtrl_WriteOperationKey(int);

//--------------------------------------------------------------------PROCESS//
MHPCTRL_API void mHpCtrl_NextStepProc(HWND,BOOL,char*);
MHPCTRL_API int  mHpCtrl_WndProc(HWND,UINT,WPARAM,LPARAM,HWND,CEventListe*,char*,BOOL);
MHPCTRL_API int  mHpCtrl_WriteAppName(CEventListe*,char*,BOOL);
MHPCTRL_API void mHpCtrl_AddIfChangeExe(CEventListe*,char*);
MHPCTRL_API BOOL mHpCtrl_SetDotNetCtrlNotify(HWND);
MHPCTRL_API BOOL mHpCtrl_SetPlatformCtrlNotify(HWND);

//----------------------------------------------------------------------WRITE//
MHPCTRL_API BOOL mHpCtrl_WriteCustomEvent(HWND,HWND,CEventListe*,char*,int,int);
MHPCTRL_API BOOL mHpCtrl_WriteEventTask(HWND,char*,CEventListe*,CWindowListe*,BOOL);
MHPCTRL_API BOOL mHpCtrl_WriteWindowLst(HWND,char*,CWindowListe*,BOOL);
MHPCTRL_API BOOL mHpCtrl_WriteCtrlList(HWND,char*,BOOL);
MHPCTRL_API BOOL mHpCtrl_WriteFavsFile(HWND,char*,CFavListe*,int);
MHPCTRL_API void mHpCtrl_InitLastCtrlID();

//-----------------------------------------------------------------------HTML//
MHPCTRL_API BOOL mHpCtrl_InsertHtmlTag(HWND,CEventListe*,const char*,char*,int*,const char*);
MHPCTRL_API void mHpCtrl_SetStartPostData(const std::string& strPostData);

//----------------------------------------------------------------------TIMER//
MHPCTRL_API int  mHpCtrl_SetTimer(HWND,int,int);
MHPCTRL_API BOOL mHpCtrl_KillTimer(HWND,int);

#endif //********************************************************************//

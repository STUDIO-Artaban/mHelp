// mHpVar.h
//

#ifndef  __MHPVAR_H //*******************************************************//
#define  __MHPVAR_H

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif

#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif

#ifndef  __COMMCTRL_H
 #include <commctrl.h>
#endif

//MHPPAK---------------------------------------//
#ifndef  __MHELPPAK_H
 #include <mHpPak.h>
#endif
//---------------------------------------------//

#ifndef  _INC_CTRLMGMTLIST
 #include "CtrlMgmt.h"
#endif
#include <string>

//---------------------------------------------------------------------------//
// Définitions
#define		CHECKBOX_LIMITE			"[0;2]"
#define		STRING_LIMITE			"500"
#define		LISTSTR_LIMITE			"500*500"
//---------------------------------------------//
#define		MHPTASK_KEY				"mHpTask"
#define		MHPWRITE_KEY			"mHpWrite"
#define		MHPOPTION_KEY			"mHpOption"
#define		MHPMSG_KEY				"mHpMsg"
#define     MHPNAME_KEY             "mHpCtrlName"
#define		MHPSTATEMSG_KEY			"mHpStateMsg"
#define		MHPCTRLID_KEY		    "mHpCtrlID"
#define		LINKED_KEY				"Linked"
//---------------------------------------------//
#define     MAX_CTRL_NAME           MAX_KEY_SIZE
//---------------------------------------------//
// WriteHwndExeKey
#define		APPNAME_KEY				"AppName"
#define		HWNDEXE_KEY				"HwndExe"
#define		CHANGEEXE_KEY			"ChangeExe"

class CLinkCell ///////////////////////////////////////////////////////////////
{	
public:
	BOOL bLink;
	int iIndex;
	CLinkCell* NextCell;
	CLinkCell(int);
};
class CLinkList ///////////////////////////////////////////////////////////////
{
private:
	CLinkCell* LinkHead;
	CLinkCell* GetLinkCell(int);
public:
	CLinkList();
	void AddItm(int);
	int GetItmIndex(int);
	void LinkItm(int);
	void UnLinkItms();
	BOOL GetItmLink(int);
	void EmptyLinkList();
};

class ItemsCellule ////////////////////////////////////////////////////////////
{
public:
	int ItemIndex;
	ItemsCellule *ItemSuivant;
	ItemsCellule(int);
};

class CGroupListe /////////////////////////////////////////////////////////////
{
private:
	ItemsCellule* ItemHead;
	ItemsCellule* ItemCurrent;
public:
	CGroupListe();
	void AddItemIndex(int);
	BOOL IsListeEmpty();
	int GetItemIndex();
	void EmptyGroupListe();
	void RemoveIndexItem();
	void RemoveLastIndexItem();
	BOOL InsertIndexItem(int);
	int ItemIndexSuivant();
	void ReplaceCurrent();
	BOOL IsNextIndexExist();
};

//-------------------------------------------------------------------CmHpTask//
class CmHpTask
{	private:
		char lpmHpMsg[MAX_RS_SIZE];
		char lpmHpStateMsg[MAX_RS_SIZE];
		int imHpID;
		int iCurrentWnd;
		int iCountWnd;
		int imHpStep;
		int imHpOption;
        char lpmHpCtrlName[MAX_CTRL_NAME];
		int lpmHpCtrlID[MAX_PART_TREE];
	public:
		CmHpTask(int,char*,char*,int,int,int,int*,char*);
		CmHpTask();
        virtual ~CmHpTask();
		int GetmHpID();
		char* GetmHpMsg();
		void SetmHpMsg(char*);
		char* GetmHpStateMsg();
		void SetmHpStateMsg(char*);
		int GetmHpOption();
		void SetmHpOption(int);
		void NextmHpStep();
		int GetCurrentWnd();
		void SetCurrentWnd(int);
		int GetCountWnd();
		void SetCountWnd(int);
		int GetmHpStep();
		void SetmHpStep(int);
        char* GetmHpCtrlName();
		int* GetmHpCtrlID();
		int GetCtrlID();
		//-------------------------------------//
		BOOL WriteCommonKey(BOOL,HKEY&,HKEY&);
		BOOL ReadCommonKey(int,BOOL,HKEY&,HKEY&);
		// mHpCtrl's virtuals ---------------------------------------------**//
		virtual BOOL ReadTaskKey(int);
		virtual BOOL WriteTaskKey();
		// Menu control -----------------------//
		virtual int GetMenuState();
		// ListBox control --------------------//
		virtual void AddMultiIndex(char*);
		virtual BOOL GetNextIndex();
		virtual char* GetMultiIndex(int);
		virtual void InitMultiIndex();
		virtual void InverseMultiIndex();
		// EditBox control --------------------//
		virtual char* GetEditValue();
		virtual void SetEditValue(char*);
		virtual int GetUserFixed();
		virtual void SetUserFixed(int);
		// Slider control ---------------------//
		virtual int GetSliderPos();
		virtual void SetSliderPos(int);
		// Button control ---------------------//
		virtual int GetBtnState();
		virtual void SetBtnState(int);
		// ChangeExe --------------------------//
		virtual char* GetExeName();
		virtual BOOL GetNewExe();
		virtual void SetHwndExe(HWND);
		virtual HWND GetHwndExe();
		virtual char* GetCommandLine();
		virtual void SetCommandLine(char*);
		// TabCtrl ----------------------------//
		virtual char* GetTabSel();
		virtual void SetTabSel(char*);
		// ToolBar ----------------------------//
		virtual UINT GetCommandID();
		virtual UINT GetMenuSelect();
		virtual void SetMenuSelect(UINT);
        // CtrlMan ----------------------------//
        virtual BOOL WriteCtrlIDKey(HKEY) const;
        virtual BOOL ReadCtrlIDKey(HKEY);
        virtual HICON GetCtrlIcon() const;
        virtual void GetCtrlVers(char*,UINT) const;
        virtual void GetCtrlDesc(char*,UINT) const;
        virtual BOOL GetStrState(char*,int,BOOL) const;
        virtual int GetParamType() const;
        virtual ImHpCtrlMgmt* GetCtrlMgmt();
        virtual UINT GetCtrlPlatformID();
		//-----------------------------------------------------------------**//
};

//---------------------------------------------------------------CmHpTaskCell//
class CmHpTaskCell
{	public:
		CmHpTask *Element;
		CmHpTaskCell *Suivant;
		CmHpTaskCell(CmHpTask*);
        virtual ~CmHpTaskCell();
};

//-----------------------------------------------------------------CTaskListe//
class CTaskListe
{	private:
		CmHpTaskCell *Tete;
		CmHpTaskCell *Courant;
	public:
		CTaskListe();
		CTaskListe(CmHpTask*);
		BOOL WriteTaskListKey();
		//------------------------------//
		void AddCellule(CmHpTask*);
		BOOL NextCellule();
		CmHpTask* GetCurrentTask();
		void EmptyListe();
		int GetCountTask();
		int GetNextmHpID();
		void InsertList(CmHpTaskCell*);
		BOOL IsChangeExeTask();
		BOOL IsRepeatListAutorized();
		BOOL SetCurrentTask();
		int GetCurrentObj();
		HWND GetLastHwndExe(char*);
		BOOL GetContinueProc(int,int);
		//BOOL IsFirstExeChange();
};

//------------------------------------------------------------------CmHpParam//
class CmHpParam
{	private:
		char lpmHpLimite[MAX_RS_SIZE];
		char lpmHpDescription[MAX_RS_SIZE];
	public:
		CmHpParam(char*,char*);
		CmHpParam();
		BOOL WriteParamKey();
		BOOL ReadParamKey(int);
		//-----------------------------//
		char* GetmHpLimite();
		char* GetmHpDescription();
		void SetmHpLimite(char*);
		void SetmHpDescription(char*);
};

//------------------------------------------------------------------CmHpWrite//
class CmHpWrite
{	public:
		void WritemHpWriteKey(HKEY&,HKEY&);
		CmHpWrite();
		// mHpCtrl's virtuals ---------------------------------------------**//
		virtual BOOL ReadEventKey(int);
		virtual BOOL WriteEventKey();
		// Common -----------------------------//
		virtual int GetCtrlStyle();
		// Slider control ---------------------//
		virtual int GetMinPos();
		virtual int GetMaxPos();
		// Radiobox control -------------------//
		virtual int GetLinked();
		virtual void SetLinked(int);
		virtual int GetRadioRow();
		virtual void SetRadioRow(int);
		virtual char* GetRadioName();
		//-----------------------------------------------------------------**//
};
// Common -------------------------------------//
class CStyleWrite : public CmHpWrite
{	private:
		int iCtrlStyle;
	public:
		CStyleWrite(int);
		CStyleWrite();
		BOOL ReadEventKey(int);
		BOOL WriteEventKey();
		int GetCtrlStyle();
};

//-----------------------------------------------------------------CEventCell//
class CEventCell
{	public:
		CmHpParam* mHpParam;
		CmHpTask* mHpTask;
		CmHpWrite* mHpWrite;
		BOOL bViewParam;
		CEventCell* NextEvent;
		CEventCell(CmHpTask*,CmHpWrite*,CmHpParam*);
};

//----------------------------------------------------------------CEventListe//
class CEventListe
{	private:	
		char lpView[MAX_RS_LISTE];
		CEventCell *Head;
		CEventCell* GetPrevCell(CEventCell*);
		CEventCell* GetLastCell(CEventCell*);
	public:
		CEventListe();
		//-----------------------------//
		CEventCell* GetCellFromPos(int,BOOL);
		void AddEvent(CmHpTask*,CmHpWrite*,CmHpParam*);
		void ReplaceEvent(CmHpTask*,CmHpWrite*);
		BOOL IsEmptyListe();
		CmHpTask* GetmHpTask(int,BOOL);
		CmHpWrite* GetmHpWrite(int,BOOL);
		CmHpParam* GetmHpParam(int,BOOL);
		void ChangeEventWnd(int,int);
		void RemoveLastEvent();
		void EmptyEvents();
		UINT GetCustEventCount() const;
        BOOL GetNextHtmlTag(int,char*,UINT) const;
		char* GetStrState(int,BOOL);
		int GetParamType(int);
		char* GetStrLimite(int,BOOL,BOOL);
		char* GetStrDescription(int,BOOL);
		void EmptyChange();
		BOOL GetNextEvent(int,BOOL);
		char* GetLastExeName(int);
		BOOL IsCtrlState(int,BOOL);
		BOOL IsNoStateMsg(int);
		void SetStrLimite(int,char*);
		void SetStrDescription(int,char*);
		void SetMsg(int,char*);
		void SetStateMsg(int,char*);
		void MovemHpCtrl(int,int);
		void DeletemHpParam(int);
		int GetEventCount();
		BOOL IsAnAppActive();
		BOOL GetCustomVisible(int);
		void SetCustomVisible(int,BOOL);
};

//---------------------------------------------------------------------------//
// Structures
typedef struct mHpVarStruct
{	HINSTANCE hmHpCtrlInst;			    // hInstance of the mHpCtrlDll
	BOOL bBlockUpdateFrame;			    // TRUE: Pas de UpdateFrame pour le DDRAW_TIMER
	BOOL bRefreshDDraw;				    // TRUE: Autorise le rafraichissement "UpdateFrame"
	int XposCur;					    // Position du curseur/click/press/unpress DDraw
	int YposCur;					    // Position du curseur/click/press/unpress DDraw
	int XnextPos;					    // Next position du curseur DDraw
	int YnextPos;					    // Next position du curseur DDraw
	CmHpTask *CourantTask;			    // Evenement courant
	int RepeatLoop;					    // Assure la gestion de l'évolution des paramètres
	// Non Générique /////////////////////
	int AscensSens;					    // Sens de déplacement de l'ascenseur: 0->Pas d'ascenseur,1->monter,2->descendre,3->droite,4->gauche
	int ItemsVisu;					    // Nombre d'items en visuel d'une List/Combolist
	int iLastTopIndex;				    // Dernier TopIndex visible: Evite le bug lié à la précision de ItemsVisu
    HWND hWndCurCtrl;                   // Current control handle of the current task (Write & Read)
    //BOOL bLockDotNetMsg;              // TRUE: Lock no DotNet notification message, FALSE: Do not lock
    ImHpCtrlMgmt::ActType CurAct;       // Current mHelp Control Management action
    char szLastClass[CTRL_CLASS_SIZE];  // Last class name of control that have emitted an event
    int iLastStyle;                     // Last style ID of control that have emitted an event
    CCtrlID* pLastID;                   // Last ID instance of control that have emitted an event
	//////////////////////////////////////
	int CursorType;					    // 0->no button,1->left,2->right,3->middle
	int EcartDepl;				        // Ecart entre chaque deplacement du DDraw (MouseSpeed)
	BOOL bNewDepl;				        // TRUE: calcul du deplacement lineaire du curseur DDraw
	int iCurAnim;					    // Index de l'animation courante: 0->1° anim,1->2° anim,2->3° anim
	int iCntAnim;					    // Nombre de fois que l'animation a été répété
	int iMaxAnim;					    // Nombre de fois maximum que l'animation peut être répété
	BOOL bLockAnim;					    // TRUE: continue à afficher une animation, FALSE: fin de l'animation
    BOOL bMoveDone;                     // TRUE: If UpdateFrame() has finished to move D2D cursor in destination position
	int CboEditIndice;				    // Indice du texte à placer dans un Editbox ou une Combobox
	BOOL EditFlag;					    // L'utilisateur valide ce qu'il vient de taper par "enter" (TRUE: continue,FALSE: stop)
	int xResolution;				    // Resolution sur x du screen
	int yResolution;				    // Resolution sur y du screen
	int DDrawBitmap;				    // 0->"SOURIS",1->"CLICK",2->"PRESS",3->"UNPRESS"
	CGroupListe *SelItemListe;		    // Liste des items index a sélectionner dans une ListBox multi-selection
	WORD CourantIDCommand;			    // Identifiant de la commande pour un bouton simulé
	BOOL bConserveBGD;				    // TRUE: conserve le fond pour une real task
	int iCtrlSel;					    // Index of the selected control to be customized
	BOOL bCustomFolder;				    // TRUE: Param folder item, FALSE: Last item
	int iLnkLag;					    // Linked controls to select
	int iLnkSel;					    // Index of the current linked control
	HWND hHandleExeWnd;				    // Handle of the current application
	BOOL bNoInactiveMsg;			    // TRUE: do not process the WM_ACTIVEAPP deactivated in the "mHpCtrl_WndProc", FALSE: orelse
	BOOL bLockWaMsg;				    // TRUE: do not process the WM_ACTIVE active in the "mHpCtrl_WndProc", FALSE: orelse
	BOOL bCreateTaskObj;			    // TRUE: The Task Object has been created, FALSE: orelse
	CLinkList* CtrlLink;			    // Object for linked controls as radiobox
	int DelayLoop;					    // Current max loop delay
	int iCurObj;					    // Current task index
	int iCurOption;					    // Current task option
	BOOL bContinueProc;				    // TRUE: we can continue the process, FALSE: orelse
	BOOL bNextmHpID;				    // TRUE: there is a next task to continue the process, FALSE: orelse
	BOOL bWaitCtrl;					    // TRUE: we are waiting the control enable, FALSE: orelse
	int iSkipSnd;					    // Nombre de fois que le son n'est pas lu (IDR_WAVE_PRESS|IDR_WAVE_UNPRESS)
	BOOL bLockUnWaMsg;				    // TRUE: do not process the WM_ACTIVE inactive in the "mHpCtrl_WndProc", FALSE: orelse
	int iLastCmdID;					    // Dernier Command ID d'un "In" contrôle comme un ToolBar, qui a émit un évènement
	HMENU hCurMenu;					    // HMENU provenant de la capture d'un WM_INITMENU, dans le cas où celui-ci != GetMenu()
	int imHpProcID;					    // Identifiant du process de création de macros en cours
	BOOL bNoClearmHpKeys;			    // TRUE: No mHpPak_ClearmHpSharedKey in StopmHelp, FALSE: Do it
	//----------------------------------//
	int imHpMode;					    // 0:Tout actif,1:Assistance,2:Help
	int	imHpMouseSpeed;				    // Mouse speed configured
	int	imHpRepeat;					    // Loop option configured
	int imHpDelay;					    // Max loop delay option
	int IndexTab[MAX_PART_TREE];	    // Référence de la tâche à lancer
	char mHpFileName[MAX_RS_SIZE];	    // Nom du fichier ".mhf" où se trouve la tâche à lancer
	char lpCurApp[MAX_RS_SIZE];		    // Nom de l'application en cours (*.exe)
	CTaskListe *TaskList;			    // Liste des evenements constituant la tâche à lancer
	CEventListe *EventList;			    // Liste des evenements constituant la tâche à créer
    CCtrlMgmtList *CtrlMgmtList;        // Liste des controles gérés

} MHPVARSTRUCT;

//-----------------------------------------------------------------------HTML//
BOOL IsHtmlCustSelected(const std::string&,UINT);
BOOL GetHtmlCustValue(char*,UINT,const std::string&,const char*,UINT);

//-------------------------------------------------------------------KEYSPROC//
void GetmHpKeyFromPos(int,HKEY&,HKEY&,BOOL);
int GetNextKeyPos();
int GetmHpIDKey(int);
void RemoveLastKeyPos();
void SetOperationModeKey(int);
BOOL WriteCursorPoskey(int,int);
void ReadCursorPoskey(int&,int&);
int ReadCurObjKey();
BOOL WriteHwndExeKey(HWND,char*);
HWND ReadHwndExeKey(char*);
void ReadmHpWriteKey(int,HKEY&,HKEY&);

// Get ////////////////////////////////////////////////////////////////////////
MHPVARSTRUCT* GetmHpVarStruct();
// FREE ///////////////////////////////////////////////////////////////////////

void FreemHpVarStruct();

#endif //********************************************************************//
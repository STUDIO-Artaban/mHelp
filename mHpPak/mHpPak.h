// mHpPak.h
//

#ifdef MHPPAK_EXPORTS
#define MHPPAK_API __declspec(dllexport)
#else
#define MHPPAK_API __declspec(dllimport)
#endif

#ifndef  __MHELPPAK_H //***********************************************************************************************//
#define  __MHELPPAK_H

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif

#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif

#ifndef  __COMMCTRL_H
 #include <commctrl.h>
#endif

//-----------------------------------------------------------------------------------------------------------------------//
// Définitions
#define		MAX_RS_SIZE				512
#define		NO						(-1)
#define		MAX_RS_LISTE			65536
#define		MAX_LONG_SIZE           16384
#define		MAX_PART_TREE			500
#define		MAX_KEY_SIZE			256
#define		MAX_MID_SIZE			128
#define		MAX_SHT_SIZE			64
#define		NUMHEADERS				4
//------------------------------------------------------//
#define		ITEM_GROUP				'^'
#define		TREE_PARAM_GROUP		'¤'
#define		NEXT_RS					'|'
#define		END_RS_TASK				'\n'
#define		RS_PARAM				'§'
#define		END_LIST				'\0'
#define		NEXT_TASK				'_'
#define     START_CTRL              '@'
#define		START_TASK				'#'
#define		END_FILE				'¨'
#define		ANTI_SHLASS				'\\'
//------------------------------------------------------//
#define		NULL_STRING				""
#define		SEPARATED_STRING		"¤"
#define		VB_TASK_FRAME			"ThunderRT6Frame"
#define		TOOLTIPS_CLASS_VB		"msvb_lib_tooltips"
#define		TOOLTIPS_CLASS_A		"tooltips_class"
#define		TOOLTIPS_CLASS_B		"tooltips_class32"
//------------------------------------------------------//
#define		SOFTWARE_KEY			"Software"
#define		MHELP_KEY				"mHelp"
#define		MHPAPP_KEY				"mHpApp"
#define		MHPSHARED_KEY			"mHpShared"
#define		SETTINGS_KEY			"Settings"
#define		FAVORITES_KEY			"Favorites"
#define		AUTO_ASSIST_KEY			"AutoAssist"
#define		MHP_PATH_KEY			"mHpPath"
#define		REF_EVENT_KEY			"EventRef"
#define		READWRITE_KEY			"mHpProcess"
#define		LOOPOPTION_KEY			"LoopOption"
#define		MOUSESPEED_KEY			"MouseSpeed"
#define		EXEPATH_KEY				"ExePath"
//------------------------------------------------------//
#define		MHPPAK_SERVTITLE		"mHelp Tool"
#define		MHPPAK_DLGTITLE			"mHelp ?!"
#define		MHPPAK_DLGMSGTITLE		"mHelp Message"
//------------------------------------------------------//
#define		MHPPAK_DOC_WELCOME			0				// -> int_Welcome
#define		MHPPAK_DOC_MAIN				1				// +0 -> usr_MainIntFile
#define		MHPPAK_DOC_MAINCONT			2				// +1 -> usr_MainIntSom
														// +2 -> usr_MainIntCfg
														// +3 -> usr_MainIntSearch
														// +4 -> usr_MainIntIndex
#define		MHPPAK_DOC_MAINFAV			6				// -> usr_MainIntFav
#define		MHPPAK_DOC_MAINAUTO			7				// -> usr_MainIntAuto
#define		MHPPAK_DOC_FAV				8				// -> usr_MainIntFavDlg
#define		MHPPAK_DOC_EVOL				9				// -> usr_MainIntEvoDlg
#define		MHPPAK_DOC_SERV				11				// -1 -> usr_SaveIntCtrlCfg
														// +0 -> usr_SaveIntFile
														// +1 -> usr_SaveIntTree
														// +2 -> usr_SaveIntParam
#define		MHPPAK_DOC_DEFCFG			14				// -> usr_CfgInterface
#define		MHPPAK_DOC_CFGCOM			15				// -> usr_SaveIntCfgCom
#define		MHPPAK_DOC_CFGBOOL			16				// -> usr_SaveIntCfgBool
#define		MHPPAK_DOC_CFGINT			17				// -> usr_SaveIntCfgInt
#define		MHPPAK_DOC_CFGSTR			18				// -> usr_SaveIntCfgStr
#define		MHPPAK_DOC_CFGMLTSTR		19				// -> usr_SaveIntMltStr
#define		MHPPAK_DOC_DHPDGNST			20				// -> usr_DhpDgnst
//------------------------------------------------------//
#define		STR_SEPARATED_AUTOASS		";"
#define		CHR_SEPARATED_AUTOASS		';'
//------------------------------------------------------//
#define		MHPPAK_EXE_EXTENSION		".exe"
#define     MHPPAK_MHF_EXTENSION        ".mhf"
#define     MHPPAK_MHF_EXTFILTER        "*.mhf"
#define		MHPPAK_STR_ANTISHLASS		"\\"
//------------------------------------------------------//
#define		MHPPAK_QT_CLASS_NAME        "QWidget"
#define		MHPPAK_DOTNET_CLASS_NAME    "WindowsForms10.Window.8.app"
//------------------------------------------------------//
#define     MHPID_CTRL_MAN              999             // mHpID for managed control
#define     MAX_CLASS_SIZE              128             // Same as CTRL_CLASS_SIZE
#define     MAX_VERSION_SIZE            32              // Same as DLL_VERSION_SIZE

//-----------------------------------------------------------------------------------------------------------------------//
// Macros
#define	mHpPak_CharToInt(c)			((int)(c & 0x0F))	
#define	mHpPak_IsItANumber(c)		((BOOL)((c>=0x30)&(c<=0x39)))
#define	mHpPak_IsItHexNumber(c)		((BOOL)(((c>=0x30)&(c<=0x39))|(c=='a')|(c=='b')|(c=='c')|(c=='d')|(c=='e')|(c=='f')|\
									(c=='A')|(c=='B')|(c=='C')|(c=='D')|(c=='E')|(c=='F')))
#define	mHpPak_IsWaitingCursor()	((BOOL)(GetCursor()==LoadCursor(NULL,MAKEINTRESOURCE(IDC_WAIT))))

//-----------------------------------------------------------------------------------------------------------------------//
// Objects

class MHPPAK_API CType ////////////////////////////////////////////////////////////////////////////////////////////////////
{	
public:
	CType();
	virtual int GetInt();
	virtual char* GetStr();
	virtual void SetInt(int);
	virtual void SetStr(char*);
};

class MHPPAK_API CIntType : public CType //////////////////////////////////////////////////////////////////////////////////
{	
public:
	int IntType;
	CIntType(int);
	int GetInt();
	void SetInt(int);
};

class MHPPAK_API CStrType : public CType //////////////////////////////////////////////////////////////////////////////////
{	
public:
	char StrType[MAX_RS_SIZE];
	CStrType(char*);
	char* GetStr();
	void SetStr(char*);
};

class MHPPAK_API CTypeCell ////////////////////////////////////////////////////////////////////////////////////////////////
{	
public:
	CType *Type;
	CTypeCell *NextType;
	CTypeCell(CType*);
};

class MHPPAK_API CTypeList ////////////////////////////////////////////////////////////////////////////////////////////////
{	
private:
	CTypeCell *HeadList;
public:
	CTypeList();
	void AddType(CType*);
	// NO "EmptyTypeList()" PROBLEM *************************//
	void AddIntTypeNoEmptyBug(int);
	void AddStrTypeNoEmptyBug(char*);
	//*******************************************************//
	void RemoveType(int);
	void CreateTypeList(int,char*,int);
	void EmptyTypeList();
	int GetIntType(int);
	char* GetStrType(int);
	void SetIntType(int,int);
	void SetStrType(int,char*);
	int GetCount();
};

class MHPPAK_API CRepeatCell //////////////////////////////////////////////////////////////////////////////////////////////
{	
public:
	CTypeList *TypeList;
	CRepeatCell *NextList;
	CRepeatCell(CTypeList*);
};

class MHPPAK_API CRepeatList //////////////////////////////////////////////////////////////////////////////////////////////
{	
private:
	CRepeatCell *RepeatList;
	BOOL bEvoluParam;
	char *StrParam;
public:
	CRepeatList();
	~CRepeatList();
	void AddTypeList(int,char*,int);
	CRepeatCell* GetRepeatCell(int);
	void CreateRepeatList(BOOL,int,HWND,char*,int);
	void EmptyRepeatList();
	int GetIntParam(int,int);
	char* GetStrParam(int,int);
	BOOL GetEvoluFlag();
	void SetIntParam(int,int,int,int);
	void SetStrParam(int,int,int,char*);
	void SetEvoluFlag(BOOL);
	void FreeStrParam();
	char* GetStringParam(int,int);
	int GetListCount(int);
};

class MHPPAK_API CItem ////////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
	int ItemType; // 0 RepRS,1 DossierRS,2 ParamRS
	BOOL bSelected;
	CItem(int);
	void ChangeSelected(BOOL);
	BOOL GetSelect();
	virtual void ModifyStrParam(char*);
	virtual int GetTypeParam();
	virtual char* GetDescription();
	virtual char* GetLimite();
	virtual void FreeDescription();
	virtual void FreeLimite();
	//-----------------------//
	virtual BOOL GetRepeatFlag();
	virtual void FreeEvoluList();
	virtual char* GetStrFromParam(int);
	virtual void SetRepeatList(HWND);
	virtual int GetIntItem(int,int);
	virtual char* GetStrItem(int,int);
	virtual CRepeatList* GetEvoluList();
};

class MHPPAK_API CParam : public CItem ////////////////////////////////////////////////////////////////////////////////////
{
public:
	int ParamType;
	CRepeatList* EvoluList; 
	char* StrLimite;
	char* Description;
	CParam(int,char*,int,char*,char*);
	~CParam();
	void ModifyStrParam(char*);
	int GetTypeParam();
	char* GetDescription();
	char* GetLimite();
	void FreeDescription();
	void FreeLimite();
	//-----------------------//
	BOOL GetRepeatFlag();
	void FreeEvoluList();
	char* GetStrFromParam(int);
	void SetRepeatList(HWND);
	int GetIntItem(int,int);
	char* GetStrItem(int,int);
	CRepeatList* GetEvoluList();
};

class MHPPAK_API ItemCellule //////////////////////////////////////////////////////////////////////////////////////////////
{
public:
	CItem *Item;
	ItemCellule *ItemSuivant;
	ItemCellule(CItem*);
};

class MHPPAK_API CItemListe ///////////////////////////////////////////////////////////////////////////////////////////////
{
private:
	ItemCellule *TeteListe;
public:
	CItemListe();
	ItemCellule* GetCellFromRang(int);
	void InsertItemList(CItem*);
	BOOL WhatIsIt(int,BOOL);
	char* GetDetail(int,int,int,char*,int);
	int GetParameterType(int);
	void ModifyStrParam(int,char*);
	BOOL GetSelection(int);
	int GetItemCount();
	int GetRangSelected();
	void SelectItem(int);
	void NoSelection();
	void EmptyList();
	//------------------------//
	void AttachRepeatParam(int,HWND);
	int GetParamInteger(int,int,int);
	char* GetParamString(int,int,int);
	BOOL IsRepeating(int);
};

class MHPPAK_API CEvoluCell ///////////////////////////////////////////////////////////////////////////////////////////////
{	
public:
	int iSelItem;
	int iItemType;
	CRepeatList* EvoluItem;
	CEvoluCell* NextEvoluItem;
	CEvoluCell(int,int,CRepeatList*);
	BOOL CompareEvolu(int,int,CRepeatList*);
};

class MHPPAK_API CEvoluList ///////////////////////////////////////////////////////////////////////////////////////////////
{
private:
	CEvoluCell* EvoluList;
public:
	CEvoluList();
	void AddEvoluItem(int,int,CRepeatList*);
	int GetSelItem(int);
	CEvoluCell* GetCellFromItem(int);
	int GetItemType(int);
	int GetSelCount();
	void CopyEvoluLst(CEvoluList*);
	void EmptyEvoluList();
};

class MHPPAK_API CFavItem /////////////////////////////////////////////////////////////////////////////////////////////////
{	
public:
	int* iRefTab;
	char* szmHpPath;
	char* szEventStr;
	int iLoop;
	CEvoluList* ParamList;
	CFavItem(int*,int,char*,char*,CEvoluList*);
	~CFavItem();
	void RefreshLoopOpt(int);
};

class MHPPAK_API CFavCell /////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
	CFavItem* FavItem;
	CFavCell* FavNextItem;
	CFavCell(CFavItem*);
	BOOL CompareItem(CFavItem*);
};

class MHPPAK_API CFavListe ////////////////////////////////////////////////////////////////////////////////////////////////
{
private:
	CFavCell* FavList;
	char* lpEventRef;
	char* lpAutoAssist;
public:
	CFavListe();
	~CFavListe();
	CFavCell* GetItemFromRang(int);
	int AddFavItem(int*,int,char*,char*,CEvoluList*,BOOL);
	void RemoveFavItem(int);
	void SetAutoAssist(char*);
	int GetFavCount();
	void RefreshAutoDecl(int,int);
	BOOL RemoveFromFile(char*);
	BOOL ChangeRefFromFile(char*,int*);
	BOOL ChangeRefFromTask(char*,int*,int*);
	void EmptyFavList();
	//-------------------------//
	int GetLoopOption(int);
	char* GetmHpPath(int);
	char* GetEventRef(int);
	char* GetEventStr(int);
	char* GetAutoAssist();
};

class MHPPAK_API CControlCell /////////////////////////////////////////////////
{
public:
    UINT iPlatformID;
	char lpClassName[MAX_CLASS_SIZE];
	char lpVersionDll[MAX_VERSION_SIZE];
	CControlCell* NextCtrlCell;
	CControlCell(UINT,const char*,const char*);
};

class MHPPAK_API CControlList /////////////////////////////////////////////////
{
private:
	CControlCell* CtrlHead;
    BOOL bUpdated;
public:
	CControlList();
    ~CControlList();
    int AddControl(UINT,const char*,const char*);
    const char* GetClassName(int) const;
    const char* GetVersionDll(int) const;
    UINT GetPlatform(int) const;
    int GetClassIndex(UINT,const char*) const;
    const CControlCell* GetCtrlCell(UINT) const;
    void SetUpdated(BOOL);
    BOOL IsUpdated() const;
    BOOL IsEmpty() const;
	void EmptyList();
};

class MHPPAK_API CWindowCell //////////////////////////////////////////////////////////////////////////////////////////////
{
public:
	char lpWndClass[MAX_RS_SIZE];
	char lpWndTitle[MAX_RS_SIZE];
	BOOL bActive;
	CWindowCell* NextWndCell;
	CWindowCell(char*,char*,BOOL);
};

class MHPPAK_API CAppWindow ///////////////////////////////////////////////////////////////////////////////////////////////
{
public:
	CWindowCell* AppWndCell;
	char lpAppName[MAX_RS_SIZE];
	int iCurWndTab[MAX_PART_TREE];
	CAppWindow* NextAppCell;
	CAppWindow(char*,char*,char*,BOOL);
	void InitCurWndTab();
};

class MHPPAK_API CWindowListe /////////////////////////////////////////////////////////////////////////////////////////////
{
private:
	CAppWindow* AppWndHead;
	HWND hWndCurrent;
public:
	CWindowListe();
	CWindowCell* GetWndCellFrom(int,int);
	int AddWndInfo(char*,char*,char*,BOOL);
	void UpdateWndLst(char*,HWND,int&,int&);
	int InactiveWnd(char*,char*,char*);
	char* GetWndClass(int,int);
	char* GetWndTitle(int,int);
	BOOL GetWndActive(int,int);
	CAppWindow* GetAppWndHead();
	void FusionWndLst(CWindowListe*);
	void ConcatRealWndLst();
	char* GetWndAppName(int);
	int* GetCurWnd(char*);
	void InitCurWnd();
	void CopyWindowList(CWindowListe*);
	//void View();
	HWND GetCurWindow();
	BOOL ReadAppWndKey();
	BOOL WriteAppWndKey();
	BOOL IsWndInList(char*,char*,char*);
	void EmptyWndLst();
};

class MHPPAK_API CUnEventCell /////////////////////////////////////////////////////////////////////////////////////////////
{
public:
	char lpUnClass[MAX_RS_SIZE];
    bool bFullSearch;
	CUnEventCell* NextUnCell;
	CUnEventCell(char*,bool=true);
};

class MHPPAK_API CUnEventListe ////////////////////////////////////////////////////////////////////////////////////////////
{
private:
	CUnEventCell* HeadUnEvnt;
public:
	CUnEventListe();
	BOOL IsItUnEvent(char*);
	BOOL AddUnEvent(char*,bool=true);
	void EmptyUnEvent();
};

class MHPPAK_API CWndDocCell //////////////////////////////////////////////////////////////////////////////////////////////
{
public:
	HWND hWndDoc;
	LONG_PTR lDocWndProc;
	CWndDocCell* NextDocWnd;
	CWndDocCell(HWND,LONG_PTR);
};

class MHPPAK_API CWndDocList //////////////////////////////////////////////////////////////////////////////////////////////
{
private:
	CWndDocCell* MainCell;
public:
	CWndDocList();
	void AddDocWnd(HWND,LONG_PTR);
	void RemoveDocWnd(HWND);
	LONG_PTR GetDocWndProc(HWND);
};

//-----------------------------------------------------------------------------------------------------------------------//
// Variables
extern MHPPAK_API CItemListe* mHpPak_ItemsList;			// Liste des items RS, ou ParamRS
extern MHPPAK_API CFavListe* mHpPak_FavsList;			// Liste des FavItems
extern MHPPAK_API CFavListe* mHpPak_LnchFavLst;			// Liste dont le FavItem to launch
extern MHPPAK_API CFavListe* mHpPak_CustomFavLst;		// Liste dont le FavItem to launch
extern MHPPAK_API CWindowListe* mHpPak_ProcWndLst;		// Liste des Windows for dinhelp process
extern MHPPAK_API CControlList* mHpPak_ProcCtrlList;    // Liste des Control class name
extern MHPPAK_API CUnEventListe* mHpPak_UnEventList;	// Liste des contrôles non gérés
extern MHPPAK_API BOOL mHpPak_bAutoAss;					// TRUE: Auto assistance in progress, FALSE: or else
extern MHPPAK_API BOOL mHpPak_bLnchProc;				// TRUE: launch progress, FALSE: or else
extern MHPPAK_API BOOL mHpPak_bLnchCustom;				// TRUE: launch custom progress, FALSE: or else
extern MHPPAK_API int mHpPak_iFavIndex;					// Index du FavItem in progress
extern MHPPAK_API BOOL mHpPak_bLockChangeWnd;			// Lock the change window message (WA_ACTIVE/WA_INACTIVE)
//-----------------------------------------------------------------------------------------------------------------COMMON//
// Déclarations
MHPPAK_API int   mHpPak_GetIntFromStr(char*,int,int);
MHPPAK_API int   mHpPak_mHpMessage(HWND,HINSTANCE,UINT,char*,int,UINT,UINT);
MHPPAK_API void  mHpPak_SetViewMsgFlag(BOOL);
MHPPAK_API int   mHpPak_GetValueNumber(char*,int);
MHPPAK_API void  mHpPak_GetIndiceStr(int,char*,char*);
MHPPAK_API int   mHpPak_GetIndexFromSymbolPos(int,char*,char);
MHPPAK_API BOOL  mHpPak_IsMenuItemReady(HMENU,int,UINT);
MHPPAK_API char* mHpPak_GetStrStateType(int,char*,int);
MHPPAK_API int   mHpPak_GetTopicFromRef(char*,int);
MHPPAK_API void  mHpPak_ViewMsg(BOOL);
MHPPAK_API void  mHpPak_IncreaseProgressBar(HWND,int);
MHPPAK_API void  mHpPak_ToolTipsActivate(BOOL);
MHPPAK_API BOOL  mHpPak_OpenmHelpDoc(HWND,UINT);
//--------------------------------------------------------------------------------------------------------------KEYREPORT//
MHPPAK_API void  mHpPak_WriteKey(const char*);
//---------------------------------------------------------------------------------------------------------------STRTOOLS//
MHPPAK_API void  mHpPak_InitStr(char*,int);
MHPPAK_API int   mHpPak_FindStrLenght(char*,int);
MHPPAK_API void  mHpPak_ReadString(char*,int,char*);
MHPPAK_API void  mHpPak_WriteString(char*,int,const char*);
MHPPAK_API void  mHpPak_DecodeString(char*,UINT,const char*,int&);
//-----------------------------------------------------------------------------------------------------------------MHPNET//
MHPPAK_API BOOL  mHpPak_IsDotNetHwnd(HWND);
MHPPAK_API BOOL  mHpPak_AddDotNetCtrlEvent(HWND,HWND);
MHPPAK_API BOOL  mHpPak_GetDotNetCtrlInfo(HWND,HWND,int&,char*,UINT);
MHPPAK_API HWND  mHpPak_GetDotNetCtrlHandle(HWND,char*,int&,int&);
//------------------------------------------------------------------------------------------------------------------MHPQT//
MHPPAK_API BOOL  mHpPak_IsQtHwnd(HWND);
MHPPAK_API BOOL  mHpPak_AddQtCtrlsEvent(HWND);
MHPPAK_API BOOL  mHpPak_GetQtCtrlInfo(HWND,int&,char*,UINT);
MHPPAK_API HWND  mHpPak_GetQtCtrlHandle(HWND,char*,int);
//------------------------------------------------------------------------------------------------------------INTTABTOOLS//
MHPPAK_API void  mHpPak_EmptyTab(int*,int);
MHPPAK_API void  mHpPak_AddIndex(int*,int);
MHPPAK_API int   mHpPak_GetIndex(int*);
MHPPAK_API int   mHpPak_GetCountIndex(int*);
MHPPAK_API BOOL  mHpPak_CompareTab(int*,int*);
MHPPAK_API void  mHpPak_CopyTab(int*,int,int*);
//-----------------------------------------------------------------------------------------------------------------MHPKEY//
MHPPAK_API BOOL  mHpPak_DeleteSubKey(HKEY,char*);
MHPPAK_API void  mHpPak_GetSecurityAtt(SECURITY_ATTRIBUTES&,BOOL&);
MHPPAK_API void  mHpPak_DeleteSecAttStructs();
MHPPAK_API void  mHpPak_ClearmHpSharedKey();
MHPPAK_API void  mHpPak_FillIntFromData(BYTE*,int&,BOOL);
MHPPAK_API void  mHpPak_FillStringFromData(char*,int,BYTE*,int);
MHPPAK_API int   mHpPak_FillDataFromString(BYTE*,char*);
MHPPAK_API int   mHpPak_FillDataFromNumber(BYTE*,int,BOOL);
MHPPAK_API BOOL  mHpPak_WriteLongStrRegVal(HKEY,const char*,const char*,const char*);
MHPPAK_API BOOL  mHpPak_ReadLongStrRegVal(HKEY,const char*,char*,int);
MHPPAK_API int   mHpPak_ReadOperationKey();
MHPPAK_API BOOL  mHpPak_ReadSettingsKey(int&,int&,int&,int&,int&);
MHPPAK_API BOOL  mHpPak_WriteSettingsKey(int,int,int,int,int);
MHPPAK_API BOOL  mHpPak_ReadExePathKey(char*,int,char*);
MHPPAK_API void  mHpPak_WriteExePathKey(char*);
MHPPAK_API int   mHpPak_ReadmHpProcIDKey();
MHPPAK_API void  mHpPak_WritemHpProcIDKey(int);
MHPPAK_API void  mHpPak_GetNextAppInfoKeys(int,char*,int,int&,int&,BOOL&);
MHPPAK_API BOOL  mHpPak_SetAppInfoKeys(char*,int,int);
MHPPAK_API BOOL  mHpPak_DestroyAppKeys(char*);
//----------------------------------------------------------------------------------------------------------------MHPFILE//
MHPPAK_API void  mHpPak_AddmHpExtension(char*,int);
MHPPAK_API BOOL  mHpPak_GetNewmHpFile(HWND,HINSTANCE,UINT,char*,int);
MHPPAK_API void  mHpPak_FindEventLine(HWND,char*,int*,int&);
MHPPAK_API void  mHpPak_GetNextEvent(HWND,char*,int&,UINT&,int&,int&,int&,BOOL&);
MHPPAK_API void  mHpPak_ClosemHpFile();
MHPPAK_API void  mHpPak_GetEventPart(HWND,char*,int&,char*,int,char&,BOOL);
MHPPAK_API BOOL  mHpPak_PosCurSeekLine(HWND,char*,int,BOOL);
MHPPAK_API BOOL  mHpPak_GetTopicmHpFile(HWND,char*,char*,int);
MHPPAK_API int   mHpPak_GetTreeLineCnt(HWND,char*);
MHPPAK_API int   mHpPak_GetCtrlLineCnt(HWND,char*);
MHPPAK_API void  mHpPak_GetNextStrRS(HWND,char*,char*,int,int&,int&);
MHPPAK_API void  mHpPak_GetDetailParamRS(HWND,char*,char*,int,int&,char*,int,char*,int,BOOL&);
MHPPAK_API BOOL  mHpPak_IsFilePathExist(HWND,char*);
MHPPAK_API BOOL  mHpPak_InsertInmHpFile(HWND,char*,int,BOOL,BOOL);
MHPPAK_API BOOL  mHpPak_WriteParty(HWND,char,int,const char*,BOOL);
MHPPAK_API BOOL  mHpPak_ConcatInmHpFile(HWND,char*,int,BOOL);
MHPPAK_API int   mHpPak_GetLineFromTab(HWND,char*,int*,int);
MHPPAK_API int   mHpPak_GetTaskLine(HWND,char*,BOOL);
MHPPAK_API BOOL  mHpPak_DecalTaskRef(HWND,char*,int*);
MHPPAK_API int   mHpPak_CreatemHpFile(HWND,char*,char*);
MHPPAK_API BOOL  mHpPak_GetWindowList(HWND,char*,CWindowListe*);
MHPPAK_API BOOL  mHpPak_GetControlList(HWND,char*);
MHPPAK_API int   mHpPak_GetAppWindowCnt(HWND,char*);
MHPPAK_API int   mHpPak_GetFavsLineCnt(HWND,char*);
MHPPAK_API void  mHpPak_AddNewEndFile(HWND,char*,int&,BOOL&);
MHPPAK_API BOOL  mHpPak_GetFavoritesFile(HWND,char*,CFavListe*);
MHPPAK_API BOOL  mHpPak_DestroyTaskFile(HWND,char*,int*,int*,int*);
MHPPAK_API BOOL  mHpPak_EncodemHpFile(char*);
MHPPAK_API BOOL  mHpPak_DecodemHpFile(HWND,char*);
//-------------------------------------------------------------------------------------------------------------------HTML//
MHPPAK_API BOOL  mHpPak_InsertHtmlTag(HWND,const char*,const char*);
//----------------------------------------------------------------------------------------------------------------AUTOASS//
MHPPAK_API int   mHpPak_GetAutoAssRow(char*,int);
MHPPAK_API void  mHpPak_ChangeAutoAss(char*,int,int,int&,int*,int,int&);

#endif //****************************************************************************************************************//
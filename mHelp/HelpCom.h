//-----------------------------------------------------//
// Fichier HelpCom.h
//
// 
//-----------------------------------------------------//

#include <windowsx.h>
#include <commctrl.h>
#include <mHpCtrl.h>
#include <mHpPak.h>
#include <process.h>
#include "resource.h"

//-----------------------------------------------------//
// Définitions
#define		CURRENT_FILES_KEY		"CurrentFiles"
#define		HELP_CHECK_KEY			"HelpCheck"
#define		ASSIST_CHECK_KEY		"AssistCheck"
#define		HOUR_DELAY_KEY			"HourDelay"
#define		MINUTE_DELAY_KEY		"MinuteDelay"
#define		FAV_INDEX_KEY			"FavIndex"
#define		STR_EVENT_KEY			"EventStr"
#define		PARAM_INDEX_KEY			"ParamIndex"
#define		PARAM_TYPE_KEY			"ParamType"
#define		PARAM_VALUES_KEY		"ParamValues"

//-----------------------------------------------------//
// Constantes
const static UINT RSParamType[(NUMHEADERS+6)]=
{	IDS_SHTYPE_I,
	IDS_SHTYPE_R,
	IDS_SHTYPE_C,
	IDS_SHTYPE_B,
	IDS_SHTYPE_S,
	IDS_SHTYPE_LI,
	IDS_SHTYPE_LR,
	IDS_SHTYPE_LC,
	IDS_SHTYPE_LB,
	IDS_SHTYPE_LS
};

//-----------------------------------------------------//
// Objets
class CmHpFileCell //////////////////////////////////////
{
public:
	char* mHpFilePath;
	BOOL bNewFile;
	CmHpFileCell* NextmHpFile;
	CmHpFileCell(char*,BOOL);
	~CmHpFileCell();
};

class CmHpFileList //////////////////////////////////////
{
private:
	CmHpFileCell* mHpFileHead;
public:
	void AddmHpFile(char*,BOOL);
	BOOL RemovemHpFile(int);
	BOOL IsmHpFileExist(int);
	char* GetmHpFilePath(int);
	BOOL GetNewFile(int);
	void EmptymHpFileList();
	CmHpFileList();
};

class CModifyParam //////////////////////////////////////
{	
public:
	int RangPos;
	char* ModifParam;
	CModifyParam *ParamSuivant;
	CModifyParam(char*,int);
	~CModifyParam();
};

class CParamCell ////////////////////////////////////////
{	
public:
	int IndexRef[MAX_PART_TREE];
	CModifyParam *ParamList;
	CParamCell *CellSuivant;
	CParamCell(int*);
	BOOL IsSameRef(int*);
};

class CModifyList ///////////////////////////////////////
{
private:
	CParamCell *ModifyTete;
public:
	CModifyList();
	BOOL AddParam(BOOL,int*,char*,int);
	char* GetModifParam(int*,int);
	BOOL IsItEmpty(int*,int);
	void EmptyModifList();
};

class CButtonCell ////////////////////////////////////////
{	
public:
	int IndexPos;
	HWND hWndButton;
	CButtonCell *NextButton;
	CButtonCell(int,HWND);
};

class CButtonList ////////////////////////////////////////
{	
private:
	CButtonCell *ButtonList;
public:
	CButtonList();
	void EmptyButtonList();
	BOOL AddButton(int,long,HWND);	
	void RemoveButton(int);
	int GetIndexFromWnd(HWND);
	BOOL bExistButton(int);
};

class CItmCreate ////////////////////////////////////////
{	
public:
	int IndxRf[MAX_PART_TREE];
	char lpRS[MAX_RS_SIZE];
	int ItmType;
	CItmCreate *ItmSuivant;
	CItmCreate(int*,int);
};

class CItmList /////////////////////////////////////////
{	
private:
	CItmCreate *ItmTete;
public:
	CItmList();
	void AddItm(int,int*);
	CItmCreate* GetCellFromRefTab(int*);
	void bExistItm(int*,int&,BOOL&);
	void GetSetStrRS(int*,char*,int,BOOL);
	void EmptyItmList();
	BOOL RemoveItm(int*);
	void ChangeType(int*,int);
	BOOL IsEmpty();
//	void AfficheItm(HWND);
};

//-----------------------------------------------------//
// Structures
typedef struct HelpComStruct
{	HINSTANCE hInstServ;
	int iMode;						// 0:Tout actif,1:Assistance,2:mHphelp
	int MouseSpeed;					// Vitesse de deplacement du DDrawCursor
	int iNbBoucle;					// Repeat option
	int IndexTab[MAX_PART_TREE];	// IndexTab
	CmHpCtrl* mHpCtrlObj;			// Liste des mHpObjs
	BOOL bmHpFileOpened;			// TRUE: There is one or more than one opened file, FALSE: No opened file
	CmHpFileList* mHpFileList;		// Liste des fichiers ".mhf" ouverts
	BOOL bNoApp;					// TRUE: the user close the last application in created mode, FALSE: orelse
	BOOL mHpDlgCreatedObj;			// The mHpDlg Objects has been created
	BOOL ServDlgCreatedObj;			// The ServDlg Objects has been created
	CModifyList* ModifList;			// Custom Items Modifications Object
	CButtonList* listBtn;			// Evolution Button Object
	CItmList *AddList;				// Server TreeView Object
	int iHourDelay;					// Hour max delay config
	int iMinuteDelay;				// Minute max delay config
	HCURSOR hmHpCursor;				// Type de curseur affiché

} HELPCOMSTRUCT;

//----------------------------------------------------------------------------------------------------------------GENERAL//
void InverseTab(int*,BOOL);
void GetIndexFromTree(HTREEITEM,HWND,int*);
char* GetmHpFileName(HWND,int,char*,int);
HTREEITEM AddItem(HWND,HTREEITEM,int,int,LPSTR,LPARAM);
int ListView_GetItemSel(HWND);
void ReplacemHpLst(HWND,HWND);
void AddItemmHp(HWND,HWND,char*,int);
void SetmHpListFromCtrl(HWND,BOOL);
void ViewMsgAlreadyOpened(HWND,char*,int);
int IsItAlreadyOpened(HWND,char*,int);
int GetOpenmHpFiles(HWND,HWND);
void ReadFavRegKey(HKEY,CFavListe*);
void RemoveRegAppKey(char*,HKEY);
BOOL WriteFavRegKey(char*,HKEY,CFavListe*);
void WriteFavsRegKey(char*,CFavListe*);
void RemoveExistFavRegKey(char*,int*,int*);
BOOL RemoveExistFavsFile(HWND,char*,int*);
HCURSOR SetmHpCursor(HWND,int);
void SplitHtmlLinkRS(char*,UINT,const char*,char*,UINT);
BOOL SelectHtmlURL(char*,UINT,HWND,HWND,HTREEITEM,HTREEITEM);
//BOOL IsSystemNT();
void ProceedMsgQueue(HWND);//,int);
//--------------------------------------------------------------------------------------------------------------------GET//
HELPCOMSTRUCT* GetHelpComStruct();
//-------------------------------------------------------------------------------------------------------------------FREE//
void FreeHelpComStruct();

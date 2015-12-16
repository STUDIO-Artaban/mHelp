//-----------------------------------------------------//
// Fichier mHpDlg.h
//
// 
//-----------------------------------------------------//

#include <windowsx.h>
#include <commctrl.h>
#include <mHpCtrl.h>
#include <mHpPak.h>
#include "resource.h"

//-----------------------------------------------------//
// Stuctures/Definitions/Classes
#define		NUMHEADERS				4
#define		LIST					'L'
#define		SEPARATED_EVENT			"¤\\"
#define		NULL_STRING				""
#define		ONE_STRING				"1"

const static UINT RSHeaders[NUMHEADERS]=
{	IDS_PARAMETER,
	IDS_TYPE,
	IDS_DESCRIPTION,
	IDS_LIMITED
};

//-----------------------------------------------------//
// Déclaration
void CreateManageObj();
BOOL IsAutoAssCfg();
void GetExeName(char*);
char* GetCurrentmHpFile();
int CreatemHpDlg(HWND);

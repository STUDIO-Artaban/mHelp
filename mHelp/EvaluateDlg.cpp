//-----------------------------------------------------//
// Fichier EvaluateDlg.cpp
//
// 
//-----------------------------------------------------//

#include "stdafx.h"
#include <stdio.h>
#include "evaluatedlg.h"
#include "helpcom.h"
#include "resource.h"

//-----------------------------------------------------//
// Macros
#define	IntToChar(i)			((char)(i | 0x30))

//-----------------------------------------------------//
// Définitions
#define		ID_TIMER_NOSELECTION		333

//-----------------------------------------------------//
// Variables globales
HWND hWndLicense=NULL;
HWND hWndRestDay=NULL;
HWND hWndCancel=NULL;
BOOL bRegistered=FALSE;

//------------------------------------------------------//
// Déclarations globales
int GetRestDayAllowed();
LRESULT CALLBACK mHpEvaluateProc(HWND,UINT,WPARAM,LPARAM);

//-----------------------------------------------------------------------------------------------------------------------//
// GetRestDayAllowed
//---------------------------------------------------------//
int GetRestDayAllowed()
{	HKEY mHpEvalKey;
	BYTE szByteTab[MAX_KEY_SIZE]={0};
	char lpmHpDay[MAX_RS_SIZE]={0};
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_SZ;
	DWORD opWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES lpSec;
	SYSTEMTIME SystemTime;
	int iRestDay=0;
	BOOL bAclRes;
	WORD wRegDay=0,wRegMonth=0,wRegYear=0;
	GetSystemTime(&SystemTime);
	bRegistered=FALSE;
	// Alliance.Software
	if(RegOpenKeyEx(HKEY_CLASSES_ROOT,"Unknown.Software",0,KEY_READ,&mHpEvalKey)==ERROR_SUCCESS)
	{	// CLSID
		if(RegOpenKeyEx(mHpEvalKey,"CLSID",0,KEY_READ,&mHpEvalKey)==ERROR_SUCCESS)
		{	if(RegQueryValueEx(mHpEvalKey,NULL,NULL,&dwType,szByteTab,&iSize)==ERROR_SUCCESS)
			{	mHpPak_FillStringFromData(lpmHpDay,MAX_RS_SIZE,szByteTab,(int)iSize);
				//
				wRegDay=(WORD)mHpPak_CharToInt(lpmHpDay[2]);
				wRegDay+=(WORD)(mHpPak_CharToInt(lpmHpDay[1])*10);
				wRegMonth=(WORD)mHpPak_CharToInt(lpmHpDay[26]);
				wRegMonth+=(WORD)(mHpPak_CharToInt(lpmHpDay[25])*10);
				wRegYear=(WORD)(mHpPak_CharToInt(lpmHpDay[15])*1000);
				wRegYear+=(WORD)(mHpPak_CharToInt(lpmHpDay[16])*100);
				wRegYear+=(WORD)(mHpPak_CharToInt(lpmHpDay[22])*10);
				wRegYear+=(WORD)mHpPak_CharToInt(lpmHpDay[23]);
				if((wRegYear==9999)&&(wRegMonth==99)&&(wRegDay==99)) bRegistered=TRUE;
				else if(wRegYear<SystemTime.wYear) // Passage d'une année à une autre
				{	if((SystemTime.wMonth>1)||((SystemTime.wYear-wRegYear)>1)) return 0;
					else
					{	if(wRegMonth!=12) return 0;
						else return (int)(30-((31-wRegDay)+SystemTime.wDay));
					}
				}
				else
				{	if(wRegMonth<SystemTime.wMonth) // Passage d'un mois à un autre
					{	if((SystemTime.wMonth-wRegMonth)>1) return 0;
						else return (int)(30-((31-wRegDay)+SystemTime.wDay));
					}
					else return (int)(30-(SystemTime.wDay-wRegDay));
				}
			}
		}
	}
	else
	{	mHpPak_GetSecurityAtt(lpSec,bAclRes);
		if(RegCreateKeyEx(HKEY_CLASSES_ROOT,"Unknown.Software",0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
					&mHpEvalKey,&opWord)!=ERROR_SUCCESS) return 0;
		if(RegCreateKeyEx(mHpEvalKey,"CLSID",0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSec,
					&mHpEvalKey,&opWord)!=ERROR_SUCCESS) return 0;
		//
		lpmHpDay[0]='{';
		lpmHpDay[1]=IntToChar((int)(SystemTime.wDay/10));
		lpmHpDay[2]=IntToChar((int)(SystemTime.wDay%10));
		lpmHpDay[3]='A';
		lpmHpDay[4]='1';
		lpmHpDay[5]='C';
		lpmHpDay[6]='0';
		lpmHpDay[7]='8';
		lpmHpDay[8]='0';
		lpmHpDay[9]='-';
		lpmHpDay[10]='E';
		lpmHpDay[11]='5';
		lpmHpDay[12]='0';
		lpmHpDay[13]='5';
		lpmHpDay[14]='-';
		lpmHpDay[15]=IntToChar((int)(SystemTime.wYear/1000)); // Milliers
		lpmHpDay[16]=IntToChar((int)((SystemTime.wYear/100)%10)); // Centaines
		lpmHpDay[17]='D';
		lpmHpDay[18]='1';
		lpmHpDay[19]='-';
		lpmHpDay[20]='A';
		lpmHpDay[21]='A';
		lpmHpDay[22]=IntToChar((int)((SystemTime.wYear/10)%10)); // Dixaines
		lpmHpDay[23]=IntToChar((int)(SystemTime.wYear%10)); // Unités
		lpmHpDay[24]='-';
		lpmHpDay[25]=IntToChar((int)(SystemTime.wMonth/10));
		lpmHpDay[26]=IntToChar((int)(SystemTime.wMonth%10));
		lpmHpDay[27]='6';
		lpmHpDay[28]='0';
		lpmHpDay[29]='0';
		lpmHpDay[30]='8';
		lpmHpDay[31]='9';
		lpmHpDay[32]='5';
		lpmHpDay[33]='F';
		lpmHpDay[34]='B';
		lpmHpDay[35]='9';
		lpmHpDay[36]='9';
		lpmHpDay[37]='}';
		//
		iSize=mHpPak_FillDataFromString(szByteTab,lpmHpDay);
		RegSetValueEx(mHpEvalKey,NULL,0,REG_SZ,szByteTab,iSize);
	}
	return 30;
}

//***********************************************************************************************************************//
// CreateEvaluateDlg
//---------------------------------------------------------//
int CreateEvaluateDlg(HWND hWndParent)
{	FARPROC lpEvalmHpDlg;
	// creation de la boite de dialogue Evaluated
    lpEvalmHpDlg=MakeProcInstance((FARPROC)mHpEvaluateProc,GetHelpComStruct()->hInstServ);
	int iResult=(int)DialogBox(GetHelpComStruct()->hInstServ,
			                   MAKEINTRESOURCE(IDD_DLG_MHPEVALUATE),
							   hWndParent,
							   (DLGPROC)lpEvalmHpDlg);
    FreeProcInstance(lpEvalmHpDlg);
	// Test la création de la boite de dialogue
	return iResult;
}


//-----------------------------------------------------------------------------------------------------------------------//
// mHpEvaluateProc()
//
//-----------------------------------------------------//
LRESULT CALLBACK mHpEvaluateProc(HWND hWndEvaluate,UINT DlgMessage,WPARAM wParam,LPARAM lParam)
{	switch(DlgMessage)
	{	case WM_INITDIALOG:
		{	char lpClose[255]={0};
			char lpDay[10]={0};
			char lpRc[1100]={0};
			char lpLicense[1100]={0};
			int i=0,iLag=0,iDay=0;
			hWndLicense=GetDlgItem(hWndEvaluate,IDC_EDIT_LICENSE);
			hWndRestDay=GetDlgItem(hWndEvaluate,IDC_STATIC_RESTDAY);
			hWndCancel=GetDlgItem(hWndEvaluate,IDCANCEL);
			SetWindowText(hWndEvaluate,MHPPAK_DLGTITLE);
			LoadString(GetHelpComStruct()->hInstServ,IDS_LICENSE,lpRc,1100);
			while(lpRc[i]!=END_LIST)
			{	if(lpRc[i]==END_RS_TASK)
				{	lpLicense[i+iLag]='\r';
					iLag++;
					lpLicense[i+iLag]='\n';
				}
				else lpLicense[i+iLag]=lpRc[i];
				i++;
			}
			SetWindowText(hWndLicense,lpLicense);
			/* No more evaluated version *************/
			//iDay=GetRestDayAllowed();
			bRegistered=TRUE;
			/*****************************************/
			if(bRegistered)
			{	ShowWindow(GetDlgItem(hWndEvaluate,IDC_STATIC_NUMDAY),SW_HIDE);
				ShowWindow(hWndRestDay,SW_HIDE);
			}
			else
			{	if(iDay>0)
				{	_snprintf(lpDay,10,"%d",iDay);
					SetWindowText(hWndRestDay,lpDay);
				}
				else
				{	SetWindowText(hWndRestDay,"0");
					ShowWindow(GetDlgItem(hWndEvaluate,IDOK),SW_HIDE);
					LoadString(GetHelpComStruct()->hInstServ,IDS_CLOSE,lpClose,255);
					SetWindowText(hWndCancel,lpClose);
					SetTimer(hWndEvaluate,ID_TIMER_NOSELECTION,1,NULL);
				}
			}
			return TRUE;
		}
		case WM_TIMER:
		{	switch(LOWORD(wParam))
			{	case ID_TIMER_NOSELECTION:
				{	KillTimer(hWndEvaluate,ID_TIMER_NOSELECTION);
					SetFocus(hWndCancel);
					break;
				}
			}
		}
		case WM_COMMAND:
		{	switch(LOWORD(wParam))
			{	case IDOK:
				{	return EndDialog(hWndEvaluate,IDOK);
				}
				case IDCANCEL:
				{	return EndDialog(hWndEvaluate,IDCANCEL);
				}
			}
		}
	}
	return FALSE;
}

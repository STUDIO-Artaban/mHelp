// mHpPak.cpp
//

#include "stdafx.h"
#ifndef  __MHELPPAK_H
 #include "mHpPak.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <commdlg.h>
#include <aclapi.h>

#include "resource.h"
#include "mHpMsg.h"
#include "htmlhelp.h"

#include <assert.h>
#include <string>

//------------------------------------------------------//
// Définitions
#define		LAG_MASK					0x80
#define		MASK_LAG					0x01
//------------------------------------------------------//
#define		MHPMODE_KEY					"Mode"
#define		CURLOOP_KEY					"CurLoop"
#define		LOOP_DELAY_KEY				"LoopDelay"
//------------------------------------------------------//
#define		MHPWND_KEY					"mHpWnd"
#define		MHPWND_APP					"WndApp"
#define		MHPWND_CLASS				"WndClass"
#define		MHPWND_TITLE				"WndTitle"
#define		MHPWND_STATUS				"WndStatus"
#define		MHPPROCID_KEY				"mHpProcID"
#define		APPSTATUS_KEY				"AppStatus"
#define		MHPSTATUS_KEY				"mHpStatus"
//------------------------------------------------------//
#define		HTML_EXTENSION				".html"
#define		MHELP_DOCFILE				"\\HELP\\mHelp.CHM"
//------------------------------------------------------//
#define		QT_CROSS_DLL_NAME			"CrossQt.dll"
#define		QT_FUNC_ADDCTRLSEVENT		"mHpQt_AddCtrlsEvent"
#define		QT_FUNC_GETCTRLINFO	        "mHpQt_GetCtrlInfo"
#define		QT_FUNC_GETCTRLHANDLE		"mHpQt_GetCtrlHandle"
//------------------------------------------------------//
#define		DOTNET_CROSS_DLL_NAME		"CrossNET.dll"
#define		DOTNET_FUNC_ADDCTRLEVENT	"mHpNET_AddCtrlEvent"
#define		DOTNET_FUNC_GETCTRLINFO		"mHpNET_GetCtrlInfo"
#define		DOTNET_FUNC_GETCTRLHANDLE	"mHpNET_GetCtrlHandle"

//------------------------------------------------------//
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
const static UINT TypeTab[(NUMHEADERS+6)]=
{	IDS_TYPE_INTEGER,
	IDS_TYPE_REAL,
	IDS_TYPE_CHARACTER,
	IDS_TYPE_BOOLEAN,
	IDS_TYPE_STRING,
	IDS_TYPE_INTEGER_LIST,
	IDS_TYPE_REAL_LIST,
	IDS_TYPE_CHARACTER_LIST,
	IDS_TYPE_BOOLEAN_LIST,
	IDS_TYPE_STRING_LIST
};
const static char ClassStruct1[24]={'A','f','x',':','X','X','X','X','X','X',':','X',':','X','X','X','X',':','X',':','X','X','X','X'};
const static char ClassStruct2[28]={'A','f','x',':','X','X','X','X','X','X',':','X',':','X','X','X','X','X',':','X',':','X','X','X','X','X','X','X'};
const static char ClassStruct3[41]={'A','f','x',':','X','X','X','X','X','X','X','X',':','X',':','X','X','X','X','X','X','X','X',':','X','X','X','X','X','X','X','X',':','X','X','X','X','X','X','X','X'};
// DotNet
const static char ClassStruct4[44]={'W','i','n','d','o','w','s','F','o','r','m','s','1','0','.','W','i','n','d','o','w','.','X','.','a','p','p','.','X','.','X','X','X','X','X','X','_','r','X','X','_','a','d','X'};
const static char ClassStruct5[45]={'W','i','n','d','o','w','s','F','o','r','m','s','1','0','.','W','i','n','d','o','w','.','X','.','a','p','p','.','X','.','X','X','X','X','X','X','X','_','r','X','X','_','a','d','X'};
static const char* szmHelpDocPage[21]=
{	"int_Welcome",
	"usr_MainIntFile",
	"usr_MainIntSom",
	"usr_MainIntCfg",
	"usr_MainIntSearch",
	"usr_MainIntIndex",
	"usr_MainIntFav",
	"usr_MainIntAuto",
	"usr_MainIntFavDlg",
	"usr_MainIntEvoDlg",
	"usr_SaveIntCtrlCfg",
	"usr_SaveIntFile",
	"usr_SaveIntTree",
	"usr_SaveIntParam",
	"usr_CfgInterface",
	"usr_SaveIntCfgCom",
	"usr_SaveIntCfgBool",
	"usr_SaveIntCfgInt",
	"usr_SaveIntCfgStr",
	"usr_SaveIntCfgMltStr",
	"usr_DhpDgnstInt"
};
typedef BOOL (__cdecl *QTACEPROC)(HWND);
typedef BOOL (__cdecl *QTGCIPROC)(HWND,int&,char*,UINT);
typedef HWND (__cdecl *QTGCHPROC)(HWND,char*,int);
typedef BOOL (__cdecl *DOTNETACEPROC)(HWND,HWND);
typedef BOOL (__cdecl *DOTNETGCIPROC)(HWND,HWND,int&,char*,UINT);
typedef HWND (__cdecl *DOTNETGCHPROC)(HWND,char*,int&,int&);

//------------------------------------------------------//
// Variables
HINSTANCE hmHpPakInst;
char TmpFileName[MAX_PATH]={0};
FILE* mHpFile;	
FILE* pHmFile;
BOOL bFileOpen=FALSE;
FILE* TmpFile;	
BOOL bTmpFile=FALSE;
BOOL bSecStruct=FALSE;
PSECURITY_DESCRIPTOR pSD=NULL;
PSID pEveryoneSID=NULL;
PACL pACL=NULL;
BYTE wLast=0x00;
//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
int iBugLine=0;
int iBugLag=0;
BOOL bBugNew=FALSE;
//************************************************
HINSTANCE hmHpQtInst=NULL;
QTACEPROC QtAddCtrlsEvent=NULL;
QTGCIPROC QtGetCtrlInfo=NULL;
QTGCHPROC QtGetCtrlHandle=NULL;
HINSTANCE hmHpDotNetInst=NULL;
DOTNETACEPROC DotNetAddCtrlEvent=NULL;
DOTNETGCIPROC DotNetGetCtrlInfo=NULL;
DOTNETGCHPROC DotNetGetCtrlHandle=NULL;

//------------------------------------------------------//
// Déclarations
void ReadError(HWND,char*);
BOOL OpenmHelpFile(HWND,char*);
BOOL CreateTempFile(HWND);
int PutIntValue(HWND,char*,int);
BOOL DestroyAndRename(HWND,const char*);
void IsItAnEmptyFile(HWND,char*,BOOL&,BOOL&);
void FindTask(HWND,char*,int*,int&);
void GetNoneEvolClass(char*,int,char*);
void ReadCommonFavFile(HWND,char*,int&,char*,int,int*,int,char&);
void ReadCustomFavFile(HWND,char*,CEvoluList*,char&,BOOL&);
BOOL StructReadFile(HWND,char*,char*,int,FILE*,FILE*);
int StructWriteFile(char*,int);
void GetNewLagStrRef(int*,int*,int*,char*,int,int,BOOL,int&);
HINSTANCE LoadCrossPlatfromLibrary(HWND,char*);
//****************************************************************************************************************DLLMAIN//

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{   switch (ul_reason_for_call)
	{	case DLL_PROCESS_ATTACH:
		{	hmHpPakInst=(HINSTANCE)hModule;
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		{	break;}
    }
    return TRUE;
}

//----------------------------------------------------------------------------------------------------------------GENERAL//
// ReadError
//-----------------------------------------------------//
void ReadError(HWND hWndActive,char* lpFileError)
{	mHpPak_ClosemHpFile();
	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_READ,lpFileError,0,0,MB_ICONSTOP);
}

//-----------------------------------------------------//
// OpenmHelpFile
//-----------------------------------------------------//
BOOL OpenmHelpFile(HWND hWndActive,char* mHpFileName)
{	if(!bFileOpen)
	{	mHpFile=fopen(mHpFileName,"rb");
		if(mHpFile==NULL)
		{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
			return FALSE;
		}
		pHmFile=fopen(mHpFileName,"rb");
		if(pHmFile==NULL)
		{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
			return FALSE;
		}
		if(fseek(mHpFile,0L,SEEK_SET))
		{	ReadError(hWndActive,mHpFileName);
			return FALSE;
		}
		if(fseek(pHmFile,1L,SEEK_SET))
		{	ReadError(hWndActive,mHpFileName);
			return FALSE;
		}
	}
	bFileOpen=TRUE;
	return TRUE;
}

//-----------------------------------------------------//
// CreateTempFile
//-----------------------------------------------------//
BOOL CreateTempFile(HWND hWndActive)
{	char lpPath[MAX_PATH]={0};
	if(!bTmpFile)
	{	GetCurrentDirectory(MAX_PATH,lpPath);
		GetTempFileName(lpPath,"MHP",0,TmpFileName);
		TmpFile=fopen(TmpFileName,"wb");
		wLast=0x00;
		if(TmpFile==NULL)
		{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_CREATE,NULL,0,0,MB_ICONSTOP);
			return FALSE;
		}
		bTmpFile=TRUE;
	}
	return TRUE;
}

//-----------------------------------------------------//
// PutIntValue
//-----------------------------------------------------//
int PutIntValue(HWND hWndActive,char *lpDest,int iValue)
{	int iResult;
	char lpTmp[MAX_PATH]={0};
	_snprintf(lpTmp,MAX_PATH,"%d",iValue);
	iResult=mHpPak_FindStrLenght(lpTmp,MAX_PATH);
	strncat(lpDest,lpTmp,iResult);
	return (iResult+1);
}

//-----------------------------------------------------//
// DestroyAndRename
//-----------------------------------------------------//
BOOL DestroyAndRename(HWND hWndActive,const char* mHpFileName)
{	mHpPak_ClosemHpFile();
	// Détruit le fichier "mHpFileName"
	if(!DeleteFile(mHpFileName))
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_DELETE_MHPFILE,NULL,0,0,MB_ICONSTOP);}
	else
	{	// Renomme le fichier "TMP_FILE"->"mHpFileName"
		if(!MoveFile(TmpFileName,mHpFileName))
		{	mHpMessage(hWndActive,hmHpPakInst,IDS_RENAME_MHPFILE,NULL,0,0,MB_ICONSTOP);}
		else
		{	mHpPak_ClosemHpFile();
			return TRUE;
		}
	}
	mHpPak_ClosemHpFile();
	return FALSE;
}

//-----------------------------------------------------//
// IsItAnEmptyFile
//-----------------------------------------------------//
void IsItAnEmptyFile(HWND hWndActive,char* mHpFileName,BOOL &bEmpty,BOOL &Erreur)
{	char ch[1]={0};
	Erreur=FALSE;
	if(OpenmHelpFile(hWndActive,mHpFileName))
	{	if(mHpPak_PosCurSeekLine(hWndActive,mHpFileName,1,FALSE))
		{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
			{	if(ch[0]==END_FILE) bEmpty=TRUE;
				else bEmpty=FALSE;
				mHpPak_ClosemHpFile();
				return;
			}
		}
	}
	Erreur=TRUE;
	mHpPak_ClosemHpFile();
}

//-----------------------------------------------------//
// FindTask
//-----------------------------------------------------//
void FindTask(HWND hWndActive,char* mHpFileName,int *Tab,int &Line)
{	char chCompare[MAX_RS_SIZE]="#";
	char chTemp[MAX_RS_SIZE]={0};
	char chFile[MAX_RS_SIZE]={0};
	BOOL bContinue=TRUE,bAntiShl=FALSE;
	int itemp,indice=0;
	Line=mHpPak_GetTaskLine(hWndActive,mHpFileName,TRUE);
	itemp=Line;
	if(Line!=NO)
	{	if(mHpPak_PosCurSeekLine(hWndActive,mHpFileName,Line,FALSE))
		{	while(bContinue)
			{	_snprintf(chTemp,MAX_RS_SIZE,"%d",Tab[indice]);
				strncat(chCompare,chTemp,mHpPak_FindStrLenght(chTemp,MAX_RS_SIZE));
				indice++;
				if(Tab[indice]==NO)	bContinue=FALSE;
				else bContinue=TRUE;
				if(bContinue) strncat(chCompare,SEPARATED_STRING,1);
			}
			indice=0;
			while(1)
			{	if(!StructReadFile(hWndActive,mHpFileName,chTemp,1,mHpFile,pHmFile))
				{	Line=NO;
					return;
				}
				if((chTemp[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
				else
				{	if(!bAntiShl)
					{	if(chTemp[0]==NEXT_TASK)
						{	if(!strncmp(chFile,chCompare,MAX_RS_SIZE)) return; // OK
							else if(!mHpPak_PosCurSeekLine(hWndActive,mHpFileName,Line++,FALSE))
							{	Line=NO;
								return;
							}
							else
							{	mHpPak_InitStr(chFile,MAX_RS_SIZE);
								indice=0;
							}
						}
						else
						{	chFile[indice]=chTemp[0];
							indice++;
						}
					}
					else bAntiShl=FALSE;
				}
			}
			return;
		}
	}
	Line=NO;
}

//-----------------------------------------------------//
// GetNoneEvolClass
//-----------------------------------------------------//
void GetNoneEvolClass(char* NoneClass,int NoneSize,char* EvolClass)
{	BOOL bSame=TRUE;
	int indice=0;
	// ClassStruct1: Afx:400000:8:1466:0:36ff -> Afx:400000:8:1466:0:
	mHpPak_InitStr(NoneClass,NoneSize);
	while((indice<24)&&(bSame)&&(EvolClass[indice]!=END_LIST))
	{	if((EvolClass[indice]!=ClassStruct1[indice])&&
		   (((ClassStruct1[indice]!='X')||(!mHpPak_IsItHexNumber(EvolClass[indice])))))
		{	bSame=FALSE;}
		if(indice<20) NoneClass[indice]=EvolClass[indice];
		indice++;
	}
	// ClassStruct2: Afx:400000:8:10011:0:1290641 -> Afx:400000:8:10011:0:
	if(!bSame)
	{	bSame=TRUE;
		indice=0;
		mHpPak_InitStr(NoneClass,NoneSize);
		while((indice<28)&&(bSame)&&(EvolClass[indice]!=END_LIST))
		{	if((EvolClass[indice]!=ClassStruct2[indice])&&
			   (((ClassStruct2[indice]!='X')||(!mHpPak_IsItHexNumber(EvolClass[indice])))))
			{	bSame=FALSE;}
			if(indice<21) NoneClass[indice]=EvolClass[indice];
			indice++;
		}
	}
    // ClassStruct3: Afx:013C0000:8:00010005:00000000:10AA03A1 -> Afx:013C0000:8:00010005:00000000:
	if(!bSame)
	{	bSame=TRUE;
		indice=0;
		mHpPak_InitStr(NoneClass,NoneSize);
		while((indice<41)&&(bSame)&&(EvolClass[indice]!=END_LIST))
		{	if((EvolClass[indice]!=ClassStruct3[indice])&&
			   (((ClassStruct3[indice]!='X')||(!mHpPak_IsItHexNumber(EvolClass[indice])))))
			{	bSame=FALSE;}
			if(indice<33) NoneClass[indice]=EvolClass[indice];
			indice++;
		}
	}
    // ClassStruct4: WindowsForms10.Window.8.app.0.bf7d44_r16_ad1 -> WindowsForms10.Window.8.app.0.bf7d44_r
	if(!bSame)
	{	bSame=TRUE;
		indice=0;
		mHpPak_InitStr(NoneClass,NoneSize);
		while((indice<44)&&(bSame)&&(EvolClass[indice]!=END_LIST))
		{	if((EvolClass[indice]!=ClassStruct4[indice])&&
			   (((ClassStruct4[indice]!='X')||(!mHpPak_IsItHexNumber(EvolClass[indice])))))
			{	bSame=FALSE;}
			if(indice<38) NoneClass[indice]=EvolClass[indice];
			indice++;
		}
	}
    // ClassStruct5: WindowsForms10.Window.8.app.0.3ee13a2_r16_ad1 -> WindowsForms10.Window.8.app.0.3ee13a2_r
	if(!bSame)
	{	bSame=TRUE;
		indice=0;
		mHpPak_InitStr(NoneClass,NoneSize);
		while((indice<45)&&(bSame)&&(EvolClass[indice]!=END_LIST))
		{	if((EvolClass[indice]!=ClassStruct5[indice])&&
			   (((ClassStruct5[indice]!='X')||(!mHpPak_IsItHexNumber(EvolClass[indice])))))
			{	bSame=FALSE;}
			if(indice<39) NoneClass[indice]=EvolClass[indice];
			indice++;
		}
	}
	/////////////////////////////////////////////////////
	if(!bSame)
	{	mHpPak_InitStr(NoneClass,NoneSize);
		strncpy(NoneClass,EvolClass,NoneSize);
	}
}

//-----------------------------------------------------//
// ReadCommonFavFile
//-----------------------------------------------------//
void ReadCommonFavFile(HWND hWndActive,char* mHpFileName,int &iLoop,char* lpEvnt,int EvntSize,int* iTabRef,
					   int TabRefSize,char &chnext)
{	char ch[1]={0};
	BOOL bAntiShl=FALSE;
	int index=0,iStep=0;
	mHpPak_InitStr(lpEvnt,EvntSize);
	mHpPak_EmptyTab(iTabRef,TabRefSize);
	iTabRef[0]=0;
	iLoop=0;
	while((chnext!=NEXT_TASK)&&(chnext!=END_RS_TASK)&&(iLoop!=NO))
	{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
		{	if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
			else
			{	if(!bAntiShl)
				{	switch(ch[0])
					{	case TREE_PARAM_GROUP: // '¤'
						{	switch(iStep)
							{	case 0:
								{	iStep++;
									index=0;
									break;
								}
								case 1:
								{	iLoop=NO;
									break;
								}
								case 2:
								{	index++;
									iTabRef[index]=0;
									break;
								}
								default:
								{	break;}
							}
							break;
						}
						case NEXT_TASK: // '_'
						{	switch(iStep)
							{	case 0:
								{	iLoop=NO;
									break;
								}
								case 1:
								{	iStep++;
									index=0;
									break;
								}
								case 2:
								{	iTabRef[(index+1)]=NO;
									chnext=ch[0];
									break;
								}
								default:
								{	break;}
							}
							break;
						}
						case END_RS_TASK: // '\n'
						{	iTabRef[(index+1)]=NO;
							chnext=ch[0];
							break;
						}
						default:
						{	switch(iStep)
							{	case 0:
								{	if(mHpPak_IsItANumber(ch[0]))
									{	iLoop*=10;
										iLoop+=mHpPak_CharToInt(ch[0]);
									}
									else iLoop=NO;
									break;
								}
								case 1:
								{	lpEvnt[index]=ch[0];
									index++;
									break;
								}
								case 2:
								{	if(mHpPak_IsItANumber(ch[0]))
									{	iTabRef[index]*=10;
										iTabRef[index]+=mHpPak_CharToInt(ch[0]);
									}
									else iLoop=NO;
									break;
								}
								default:
								{	break;}
							}
							break;
						}
					}
				}
				else
				{	switch(iStep)
					{	case 0:
						{	iLoop=NO;
							break;
						}
						case 1:
						{	lpEvnt[index]=ch[0];
							index++;
							break;
						}
						case 2:
						{	iLoop=NO;
							break;
						}
						default:
						{	break;}
					}
					bAntiShl=FALSE;
				}
			}
		}
		else iLoop=NO;
	}
	if((iLoop!=NO)&&(chnext==END_RS_TASK))
	{	if(!StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile)) iLoop=NO;
		else chnext=ch[0];
	}
}

//-----------------------------------------------------//
// ReadCustomFavFile
//-----------------------------------------------------//
void ReadCustomFavFile(HWND hWndActive,char* mHpFileName,CEvoluList* EvolList,char &chnext,BOOL &bErreur)
{	char lpVal[MAX_RS_SIZE]={0};
	char ch[1]={0};
	BOOL bAntiShl=FALSE;
	CRepeatList* RepeatLst=new CRepeatList();
	int	iParamIndex=0,iParamType=0,index=0,iStep=0;
	bErreur=FALSE;
	while((chnext!=END_RS_TASK)&&(!bErreur))
	{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
		{	if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
			else
			{	if(!bAntiShl)
				{	switch(ch[0])
					{	case TREE_PARAM_GROUP: // '¤'
						{	switch(iStep)
							{	case 0:
								{	iStep++;
									break;
								}
								case 1:
								{	iStep++;
									break;
								}
								case 2:
								{	RepeatLst->AddTypeList(iParamType,lpVal,MAX_RS_SIZE);
									RepeatLst->SetEvoluFlag(TRUE);
									mHpPak_InitStr(lpVal,MAX_RS_SIZE);
									index=0;
									break;
								}
								default:
								{	break;}
							}
							break;
						}
						case NEXT_TASK: // '_'
						{	RepeatLst->AddTypeList(iParamType,lpVal,MAX_RS_SIZE);
							EvolList->AddEvoluItem(iParamIndex,iParamType,RepeatLst);
							RepeatLst=new CRepeatList();
							mHpPak_InitStr(lpVal,MAX_RS_SIZE);
							iParamIndex=0;
							iParamType=0;
							index=0;
							iStep=0;
							break;
						}
						case END_RS_TASK: // '\n'
						{	RepeatLst->AddTypeList(iParamType,lpVal,MAX_RS_SIZE);
							EvolList->AddEvoluItem(iParamIndex,iParamType,RepeatLst);
							chnext=ch[0];
							break;
						}
						default:
						{	switch(iStep)
							{	case 0:
								{	if(mHpPak_IsItANumber(ch[0]))
									{	iParamIndex*=10;
										iParamIndex+=mHpPak_CharToInt(ch[0]);
									}
									else bErreur=TRUE;
									break;
								}
								case 1:
								{	if(mHpPak_IsItANumber(ch[0]))
									{	iParamType*=10;
										iParamType+=mHpPak_CharToInt(ch[0]);
									}
									else bErreur=TRUE;
									break;
								}
								case 2:
								{	lpVal[index]=ch[0];
									index++;
									break;
								}
								default:
								{	break;}
							}
							break;
						}
					}
				}
				else
				{	switch(iStep)
					{	case 2:
						{	lpVal[index]=ch[0];
							index++;
							break;
						}
						default:
						{	bErreur=TRUE;
							break;
						}
					}
					bAntiShl=FALSE;
				}
			}
		}
		else bErreur=TRUE;
	}
	if((!bErreur)&&(chnext==END_RS_TASK))
	{	if(!StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile)) bErreur=TRUE;
		else chnext=ch[0];
	}
}

//-----------------------------------------------------//
// StructReadFile
//-----------------------------------------------------//
BOOL StructReadFile(HWND hWndActive,char* mHpFileName,char* String,int Size,FILE* ReadFile,FILE* LagFile)
{	BYTE chLag[1]={0};
	BYTE ch[1]={0};
	int indice=0;
	BOOL bRes=TRUE;
	BYTE wNew=0x00,wTmp=0x00;
	mHpPak_InitStr(String,Size);
	//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
	int i=0;
	if(fseek(ReadFile,0L,SEEK_CUR)) i=1;
	//************************************************
	while((bRes)&&(indice!=Size))
	{	if(fread(ch,sizeof(BYTE),1,ReadFile)==1)
		{	if(fread(chLag,sizeof(BYTE),1,LagFile)==1)
			{	wNew=ch[0];
				wNew<<=1;
				wTmp=chLag[0];
				wTmp&=LAG_MASK;
				wTmp>>=7;
				if(wTmp!=0x00) wNew|=0x01;
				String[indice]=(char)wNew;
				indice++;
			}
			else
			{	if((!feof(LagFile))||(fseek(LagFile,0L,SEEK_SET)))
				{   ReadError(hWndActive,mHpFileName);
					bRes=FALSE;
				}
				else
				{	if(fread(chLag,sizeof(BYTE),1,LagFile)==1)
					{	wNew=ch[0];
						wNew<<=1;
						wTmp=chLag[0];
						wTmp&=LAG_MASK;
						wTmp>>=7;
						if(wTmp!=0x00) wNew|=0x01;
						String[indice]=(char)wNew;
						indice++;
					}
					else
					{   ReadError(hWndActive,mHpFileName);
						bRes=FALSE;
					}
				}
			}
		}
		else
		{	//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
			if(!bBugNew)
			{	bFileOpen=FALSE;
				fclose(ReadFile);
				fclose(LagFile);
				if(!mHpPak_PosCurSeekLine(hWndActive,mHpFileName,iBugLine,TRUE)) bRes=FALSE;
				else
				{	i=0;
					while((bRes)&&(i!=iBugLag))
					{	if((fread(ch,sizeof(BYTE),1,ReadFile)==1)&&(fread(chLag,sizeof(BYTE),1,LagFile)==1)) i++;
						else bRes=FALSE;
					}
				}
			}
			else bRes=FALSE;
			//************************************************
			if(!bRes)
                ReadError(hWndActive,mHpFileName);
		}
	}
	return bRes;
}

//-----------------------------------------------------//
// StructWriteFile
//-----------------------------------------------------//
int StructWriteFile(char* String,int Size)
{	BYTE ch[1]={0};
	int indice=0;
	BOOL bRes=TRUE;
	BYTE wTmp,wNew;
	//** le BUG du "mHpPak_ConcatInmHpFile" (fwrite) **
	if(fseek(TmpFile,0L,SEEK_END))
	{	fclose(TmpFile);
		TmpFile=fopen(TmpFileName,"ab");
		if(TmpFile==NULL) bRes=FALSE;
		if(fseek(TmpFile,0L,SEEK_END)) bRes=FALSE;
	}
	//*************************************************
	while((bRes)&&(indice!=Size))
	{	wNew=(BYTE)String[indice];
		wTmp=wNew&MASK_LAG;
		wNew>>=1;
		if(wLast!=0x00) wNew|=0x80;
		wLast=wTmp;
		wLast<<=7;
		ch[0]=wNew;
		if(fwrite(ch,sizeof(BYTE),1,TmpFile)!=1)
		{	//** le BUG du "mHpPak_ConcatInmHpFile" (fwrite) **
			fclose(TmpFile);
			TmpFile=fopen(TmpFileName,"ab");
			if(TmpFile==NULL) bRes=FALSE;
			if(fseek(TmpFile,0L,SEEK_END)) bRes=FALSE;
			//*************************************************
			if((bRes)&&(fwrite(ch,sizeof(BYTE),1,TmpFile)!=1)) bRes=FALSE;
			else indice++;

		}
		else indice++;
	}
	return indice;
}

//-----------------------------------------------------//
// GetNewLagStrRef
//-----------------------------------------------------//
void GetNewLagStrRef(int* iReadRef,int* iTabRef,int* iTabLag,char* lpWriteRef,int iMaxRef,int iRefFavKey,
					 BOOL bParam,int &iWriteLn)
{	int iCpyRef[MAX_PART_TREE]={0};
	BOOL bSameRef=TRUE;
	int index=0;
	iWriteLn=0;
	if(iRefFavKey!=2) mHpPak_InitStr(lpWriteRef,iMaxRef);
	mHpPak_CopyTab(iCpyRef,MAX_PART_TREE,iReadRef);
	if(((iRefFavKey==1)||(iRefFavKey==2))&&(bParam)) mHpPak_AddIndex(iCpyRef,0);
	if(!mHpPak_CompareTab(iCpyRef,iTabRef)) 
	{	iWriteLn=1;
		while(iReadRef[index]!=NO)
		{	if(bSameRef)
			{	if((iTabLag[index]<iReadRef[index])&&(iTabLag[(index+1)]==NO))
				{	iReadRef[index]=iReadRef[index]-1;
					iWriteLn=2;
					bSameRef=FALSE;
				}
				else if(iTabLag[index]!=iReadRef[index]) bSameRef=FALSE;
			}
			if(!index)
			{	switch(iRefFavKey)
				{	case 0: // Ref
					{	_snprintf(lpWriteRef,iMaxRef,"#%d",iReadRef[index]);
						break;
					}
					case 1: // Fav
					{	_snprintf(lpWriteRef,iMaxRef,"_%d",iReadRef[index]);
						break;
					}
					/*case 2: // Key
					{	_snprintf(lpWriteRef,iMaxRef,"%d",iReadRef[index]);
						break;
					}*/
				}

			}
			else if(iRefFavKey!=2) _snprintf(lpWriteRef,iMaxRef,"%s¤%d",lpWriteRef,iReadRef[index]);
			index++;
		}		
		if(iRefFavKey!=2) strncat(lpWriteRef,"_",1);
	}
}

//-----------------------------------------------------//
// LoadCrossPlatfromLibrary
//-----------------------------------------------------//
HINSTANCE LoadCrossPlatfromLibrary(HWND hWndActive,char* lpDllName)
{	HINSTANCE hInstDll=LoadLibrary(lpDllName);
	if(!hInstDll)
	{	// Error: Failed to load library
		mHpMessage(hWndActive,hmHpPakInst,IDS_LOAD_LIBRARY1,lpDllName,MAX_KEY_SIZE,IDS_LOAD_LIBRARY2,MB_ICONSTOP);
	}
	return hInstDll;
}

//****************************************************************************************************************OBJECTS//

// class CType ////////////////////////////////////////////////////////////////////////////////////////////////////////////
CType::CType()
{	NULL;}

int CType::GetInt(){ return NO;}
char* CType::GetStr(){ return NULL;}
void CType::SetInt(int i){ NULL;}
void CType::SetStr(char*){ NULL;}

// class CIntType /////////////////////////////////////////////////////////////////////////////////////////////////////////
CIntType::CIntType(int intval)
	 :CType()
{	IntType=intval;}

int CIntType::GetInt()
{	return IntType;}

void CIntType::SetInt(int NewInt)
{	IntType=NewInt;}

// class CStrType /////////////////////////////////////////////////////////////////////////////////////////////////////////
CStrType::CStrType(char* strval)
	 :CType()
{	if(strval!=NULL) strncpy(StrType,strval,MAX_RS_SIZE);
	else mHpPak_InitStr(StrType,MAX_RS_SIZE);
}

char* CStrType::GetStr()
{	return StrType;}

void CStrType::SetStr(char* newstrval)
{	mHpPak_InitStr(StrType,MAX_RS_SIZE);
	strncpy(StrType,newstrval,MAX_RS_SIZE);
}

// class CTypeCell ////////////////////////////////////////////////////////////////////////////////////////////////////////
CTypeCell::CTypeCell(CType* typeval)
{	Type=typeval;
	NextType=NULL;
};

// class CTypeList ////////////////////////////////////////////////////////////////////////////////////////////////////////
CTypeList::CTypeList()
{	HeadList=NULL;}

void CTypeList::AddType(CType *newtype)
{	CTypeCell *temp;
	if(HeadList==NULL) HeadList=new CTypeCell(newtype);
	else
	{	temp=HeadList;
		while(temp->NextType!=NULL)
		{	temp=temp->NextType;}
		temp->NextType=new CTypeCell(newtype);
	}
}

// NO "EmptyTypeList()" PROBLEM *************************//
void CTypeList::AddIntTypeNoEmptyBug(int iNewVal)
{	CTypeCell *temp;
	CType* intType=new CIntType(iNewVal);
	if(HeadList==NULL) HeadList=new CTypeCell(intType);
	else
	{	temp=HeadList;
		while(temp->NextType!=NULL)
		{	temp=temp->NextType;}
		temp->NextType=new CTypeCell(intType);
	}
}

void CTypeList::AddStrTypeNoEmptyBug(char* lpNewVal)
{	CTypeCell *temp;
	CType* strType=new CStrType(lpNewVal);
	if(HeadList==NULL) HeadList=new CTypeCell(strType);
	else
	{	temp=HeadList;
		while(temp->NextType!=NULL)
		{	temp=temp->NextType;}
		temp->NextType=new CTypeCell(strType);
	}
}
//*******************************************************//

void CTypeList::RemoveType(int indice)
{	CTypeCell* temp=HeadList;
	CTypeCell* lastcell;
	int i=0;
	if(temp!=NULL)
	{	if(!indice)
		{	HeadList=HeadList->NextType;
			delete(temp);
		}
		else
		{	while((temp!=NULL)&&(i!=indice))
			{	lastcell=temp;
				temp=temp->NextType;
				i++;
			}
			if(temp!=NULL)
			{	lastcell->NextType=temp->NextType;
				delete(temp);
			}
		}
	}
}

void CTypeList::CreateTypeList(int iWhat,char* StrNewVal,int iSizeNewVal)
{	CType *NewType;
	int i,iNbVal;
	char lpVal[MAX_RS_SIZE]={0};
	if(iWhat>4) // Liste de liste
	{	iNbVal=mHpPak_GetValueNumber(StrNewVal,iSizeNewVal);
		switch(iWhat-5)
		{	case 0: // list int
			case 3: // liste bool
			{	for(i=0;i<iNbVal;i++)
				{	mHpPak_InitStr(lpVal,MAX_RS_SIZE);
					mHpPak_GetIndiceStr(i,StrNewVal,lpVal);
					NewType=new CIntType(mHpPak_GetIntFromStr(lpVal,MAX_RS_SIZE,0));
					this->AddType(NewType);
				}
				break;
			}
			case 1: // liste float
			case 2: // liste char
			case 4: // liste str
			{	for(i=0;i<iNbVal;i++)
				{	mHpPak_InitStr(lpVal,MAX_RS_SIZE);
					mHpPak_GetIndiceStr(i,StrNewVal,lpVal);
					NewType=new CStrType(lpVal);	
					this->AddType(NewType);
				}
				break;
			}
		}
	}
	else 
	{	switch(iWhat)
		{	case 0: // int
			case 3: // bool
			{	NewType=new CIntType(mHpPak_GetIntFromStr(StrNewVal,iSizeNewVal,0));
				break;
			}
			case 1: // float
			case 2: // char
			case 4: // str
			{	NewType=new CStrType(StrNewVal);
				break;
			}
		}
		this->AddType(NewType);
	}
}

void CTypeList::EmptyTypeList()
{	CTypeCell *temp=HeadList;
	while(HeadList!=NULL)
	{	HeadList=HeadList->NextType;
		// THE BUG : Asserts Valid... (_pBlockFirst==pHead) *****************//
		delete(temp->Type);
		//*******************************************************************//
		delete(temp);
		temp=HeadList;
	}
}

int CTypeList::GetIntType(int indice)
{	int i=0;
	CTypeCell *temp=HeadList;
	while((i!=indice)&&(temp!=NULL))
	{	temp=temp->NextType;
		i++;
	}
	if((i==indice)&&(temp!=NULL)) return (temp->Type->GetInt());
	return NO;
}

char* CTypeList::GetStrType(int indice)
{	int i=0;
	CTypeCell *temp=HeadList;
	while((i!=indice)&&(temp!=NULL))
	{	temp=temp->NextType;
		i++;
	}
	if((i==indice)&&(temp!=NULL)) return (temp->Type->GetStr());
	return NULL;
}

void CTypeList::SetIntType(int indice,int iValNew)
{	int i=0;
	CTypeCell *temp=HeadList;
	while((i!=indice)&&(temp!=NULL))
	{	temp=temp->NextType;
		i++;
	}
	if((i==indice)&&(temp!=NULL)) temp->Type->SetInt(iValNew);
}

void CTypeList::SetStrType(int indice,char* lpValNew)
{	int i=0;
	CTypeCell *temp=HeadList;
	while((i!=indice)&&(temp!=NULL))
	{	temp=temp->NextType;
		i++;
	}
	if((i==indice)&&(temp!=NULL)) temp->Type->SetStr(lpValNew);
}

int CTypeList::GetCount()
{	int iCnt=0;
	CTypeCell *temp=HeadList;
	while(temp!=NULL)
	{	temp=temp->NextType;
		iCnt++;
	}
	return iCnt;
}

// class CRepeatCell //////////////////////////////////////////////////////////////////////////////////////////////////////
CRepeatCell::CRepeatCell(CTypeList* typelist)
{	TypeList=typelist;
	NextList=NULL;
}

// class CRepeatList //////////////////////////////////////////////////////////////////////////////////////////////////////
CRepeatList::CRepeatList()
{	RepeatList=NULL;
	bEvoluParam=FALSE;
	StrParam=(char*)malloc(MAX_RS_LISTE);
}

CRepeatList::~CRepeatList()
{	free(StrParam);}

void CRepeatList::FreeStrParam()
{	strncpy(StrParam,NULL_STRING,MAX_RS_LISTE);}

void CRepeatList::AddTypeList(int Type,char *Text,int iSizeText)
{	CTypeList *addlist;
	CRepeatCell *temp;
	addlist=new CTypeList();
	addlist->CreateTypeList(Type,Text,iSizeText);
	if(RepeatList==NULL) RepeatList=new CRepeatCell(addlist);
	else
	{	temp=RepeatList;
		while(temp->NextList!=NULL)
		{	temp=temp->NextList;}
		temp->NextList=new CRepeatCell(addlist);
	}
}

CRepeatCell* CRepeatList::GetRepeatCell(int iRepeatIndice)
{	int i=0;
	CRepeatCell* temp=RepeatList;
	while((i!=iRepeatIndice)&&(temp!=NULL))
	{	temp=temp->NextList;
		i++;
	}
	return temp;
}

void CRepeatList::CreateRepeatList(BOOL bRepeat,int iType,HWND hWndListView,char* lpParam,int iSizeParam)
{	CTypeList *newlist;
	char lpText[MAX_RS_SIZE]={0};
	int ilimite;
	this->EmptyRepeatList();
	bEvoluParam=bRepeat;
	if(!bRepeat)
	{	newlist=new CTypeList();
		newlist->CreateTypeList(iType,lpParam,iSizeParam);
		RepeatList=new CRepeatCell(newlist);
	}
	else
	{	ilimite=ListView_GetItemCount(hWndListView);
		for(int i=0;i<ilimite;i++)
		{	ListView_GetItemText(hWndListView,i,0,lpText,MAX_RS_SIZE);
			this->AddTypeList(iType,lpText,MAX_RS_SIZE);
		}
	}
}

void CRepeatList::EmptyRepeatList()
{	CRepeatCell* temp=RepeatList;
	while(RepeatList!=NULL)
	{	RepeatList=RepeatList->NextList;
		temp->TypeList->EmptyTypeList();
		delete temp;
		temp=RepeatList;
	}
}

int CRepeatList::GetIntParam(int RepeatIndice,int TypeIndice)
{	CRepeatCell* temp=this->GetRepeatCell(RepeatIndice);
	if(temp!=NULL) return (temp->TypeList->GetIntType(TypeIndice));
	else if(RepeatList!=NULL)
	{	// Retourne la dernière valeur
		temp=RepeatList;
		while(temp->NextList!=NULL) temp=temp->NextList;
		return temp->TypeList->GetIntType(TypeIndice);
	}
	return NO;
}

char* CRepeatList::GetStrParam(int RepeatIndice,int TypeIndice)
{	CRepeatCell* temp=this->GetRepeatCell(RepeatIndice);
	if(temp!=NULL) return (temp->TypeList->GetStrType(TypeIndice));
	else if(RepeatList!=NULL)
	{	// Retourne la dernière valeur
		temp=RepeatList;
		while(temp->NextList!=NULL) temp=temp->NextList;
		return temp->TypeList->GetStrType(TypeIndice);
	}
	return NULL;
}

BOOL CRepeatList::GetEvoluFlag()
{	return bEvoluParam;}

void CRepeatList::SetIntParam(int RepeatIndice,int SelIndice,int TypeParam,int iNewVal)
{	CRepeatCell* temp=this->GetRepeatCell(RepeatIndice);
	if(temp!=NULL)
	{	switch(TypeParam)
		{	case 0: // I
			case 3: // B
			{	temp->TypeList->SetIntType(0,iNewVal);
				break;
			}
			case 5: // LI
			case 8: // LB
			{	temp->TypeList->SetIntType(SelIndice,iNewVal);
				break;
			}
		}
	}
}

void CRepeatList::SetStrParam(int RepeatIndice,int SelIndice,int TypeParam,char* lpNewVal)
{	CRepeatCell* temp=this->GetRepeatCell(RepeatIndice);
	if(temp!=NULL)
	{	switch(TypeParam)
		{	case 1: // R
			case 2: // C
			case 4: // S
			{	temp->TypeList->SetStrType(0,lpNewVal);
				break;
			}
			case 6: // LR
			case 7: // LC
			case 9: // LS
			{	temp->TypeList->SetStrType(SelIndice,lpNewVal);
				break;
			}
		}
	}
}

void CRepeatList::SetEvoluFlag(BOOL bNewFlag)
{	bEvoluParam=bNewFlag;}

char* CRepeatList::GetStringParam(int IndiceRepeat,int itype)
{	int i=0,iCntList=0;
	char lpTmp[MAX_RS_SIZE]={0};
	CRepeatCell* temp=this->GetRepeatCell(IndiceRepeat);
	mHpPak_InitStr(StrParam,MAX_RS_LISTE);
	if(temp!=NULL)
	{	if(itype>4) // Liste de liste
		{	switch(itype-5)
			{	case 0: // list int
				case 3: // liste bool
				{	while(temp->TypeList->GetIntType(i)!=NO)
					{	_snprintf(lpTmp,MAX_RS_SIZE,"%d",(temp->TypeList->GetIntType(i)));
						if(i!=0) strncat(StrParam,"^",1);
						strncat(StrParam,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
						mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
						i++;
					}
					return StrParam;
				}
				case 1: // liste float
				case 2: // liste char
				case 4: // liste str
				{	//if(temp->TypeList->GetStrType(i)==NULL) return NULL;
					iCntList=temp->TypeList->GetCount();
					while(i!=iCntList)
					{	if(!i) strncpy(StrParam,temp->TypeList->GetStrType(i),MAX_RS_LISTE);
						else
						{	strncat(StrParam,"^",1);
							strncat(StrParam,temp->TypeList->GetStrType(i),
									mHpPak_FindStrLenght(temp->TypeList->GetStrType(i),MAX_RS_SIZE));
						}
						i++;
					}
					return StrParam;
				}
			}
		}
		else 
		{	switch(itype)
			{	case 0: // int
				case 3: // bool
				{	_snprintf(StrParam,MAX_RS_LISTE,"%d",(temp->TypeList->GetIntType(0)));
					return StrParam;
				}
				case 1: // float
				case 2: // char
				case 4: // str
				{	return (temp->TypeList->GetStrType(0));}
			}
		}
	}
	return NULL;
}

int CRepeatList::GetListCount(int iRepeat)
{	CRepeatCell* temp=this->GetRepeatCell(iRepeat);
	if(temp!=NULL) return (temp->TypeList->GetCount());
	return NO;
}

//class CItem /////////////////////////////////////////////////////////////////////////////////////////////////////////////
CItem::CItem(int itemtype)
{	ItemType=itemtype;
	bSelected=FALSE;
}

void CItem::ChangeSelected(BOOL bNew)
{	bSelected=bNew;}

BOOL CItem::GetSelect()
{	return bSelected;}

void CItem::ModifyStrParam(char *lpRSParam){ NULL;}
int CItem::GetTypeParam(){ return NULL;}
char* CItem::GetDescription(){ return NULL;}
char* CItem::GetLimite(){ return NULL;}
void CItem::FreeDescription(){ NULL;}
void CItem::FreeLimite(){ NULL;}
BOOL CItem::GetRepeatFlag(){ return NULL;}
void CItem::FreeEvoluList(){ NULL;}
char* CItem::GetStrFromParam(int indice){ return NULL;}
void CItem::SetRepeatList(HWND hWndCtrl){ NULL;}
int CItem::GetIntItem(int i,int j){ return NULL;}
char* CItem::GetStrItem(int i,int j){ return NULL;}
CRepeatList* CItem::GetEvoluList(){ return NULL;}

//class CParam ////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParam::CParam(int itemType,char *strparam,int paramtype,
			   char *Decrit,char *strlimite)
	   :CItem(itemType)
{	ParamType=paramtype;
	EvoluList=new CRepeatList();
	EvoluList->CreateRepeatList(FALSE,paramtype,NULL,strparam,MAX_RS_SIZE);
	Description=(char*)malloc(MAX_RS_SIZE);
	strncpy(Description,Decrit,MAX_RS_SIZE);
	StrLimite=(char*)malloc(MAX_RS_SIZE);
	strncpy(StrLimite,strlimite,MAX_RS_SIZE);
}

CParam::~CParam()
{	free(Description);
	free(StrLimite);
}

void CParam::ModifyStrParam(char *lpRSParam)
{ EvoluList->CreateRepeatList(FALSE,ParamType,NULL,lpRSParam,MAX_RS_SIZE);}

int CParam::GetTypeParam()
{ return ParamType;}

char* CParam::GetLimite()
{ return StrLimite;}

char* CParam::GetDescription()
{ return Description;}

void CParam::FreeDescription()
{	strncpy(Description,NULL_STRING,MAX_RS_SIZE);}

void CParam::FreeLimite()
{	strncpy(StrLimite,NULL_STRING,MAX_RS_SIZE);}

BOOL CParam::GetRepeatFlag()
{ return EvoluList->GetEvoluFlag();}

void CParam::FreeEvoluList()
{	EvoluList->FreeStrParam();
	EvoluList->EmptyRepeatList();
}

char* CParam::GetStrFromParam(int irepeat)
{	return EvoluList->GetStringParam(irepeat,ParamType);}

void CParam::SetRepeatList(HWND hWndCtrlView)
{ EvoluList->CreateRepeatList(TRUE,ParamType,hWndCtrlView,NULL,0);}

int CParam::GetIntItem(int irepeat,int jindice)
{ return EvoluList->GetIntParam(irepeat,jindice);}

char* CParam::GetStrItem(int irepeat,int jindice)
{ return EvoluList->GetStrParam(irepeat,jindice);}

CRepeatList* CParam::GetEvoluList()
{ return EvoluList;}

//class ItemCellule ///////////////////////////////////////////////////////////////////////////////////////////////////////
ItemCellule::ItemCellule(CItem *item)
{	Item=item;
	ItemSuivant=NULL;
}

//class ItemListe /////////////////////////////////////////////////////////////////////////////////////////////////////////
CItemListe::CItemListe()
{	TeteListe=NULL;}

ItemCellule* CItemListe::GetCellFromRang(int irang)
{	ItemCellule *temp=TeteListe;
	int Rang=irang;
	while((temp!=NULL)&&(Rang))
	{	temp=temp->ItemSuivant;
		Rang--;
	}
	return temp;
}

void CItemListe::InsertItemList(CItem *item)
{	ItemCellule *NewCell=new ItemCellule(item);
	if(TeteListe==NULL)	TeteListe=NewCell;
	else
	{	ItemCellule *temp=TeteListe;
		while(temp->ItemSuivant!=NULL)
			temp=temp->ItemSuivant;
		temp->ItemSuivant=NewCell;
	}
}

BOOL CItemListe::WhatIsIt(int iRang,BOOL bWhat)
{	ItemCellule *temp=this->GetCellFromRang(iRang);
	if(temp!=NULL)
	{	if(bWhat) return (temp->Item->ItemType);
		else
		{	if(temp->Item->ItemType==2) return TRUE;
			else return FALSE;
		}
	}
	return FALSE;
}

char* CItemListe::GetDetail(int iRang,int iWhat,int iRepeat,char* lpRC,int iRCsize)
{	char lpResource[MAX_KEY_SIZE]={0};
	ItemCellule *temp=this->GetCellFromRang(iRang);
	if(temp!=NULL)
	{	if(temp->Item->ItemType==2)
		{	switch(iWhat)
			{	case 0:	return (temp->Item->GetStrFromParam(iRepeat));
				case 1: 
				{	LoadString(hmHpPakInst,RSParamType[(temp->Item->GetTypeParam())],lpResource,MAX_KEY_SIZE);
					strncpy(lpRC,lpResource,iRCsize);
					return lpRC;
				}
				case 2: return (temp->Item->GetLimite());
				case 3: return (temp->Item->GetDescription());
				case 4: 
				{	LoadString(hmHpPakInst,TypeTab[(temp->Item->GetTypeParam())],lpResource,MAX_KEY_SIZE);
					strncpy(lpRC,lpResource,iRCsize);
					return lpRC;
				}
			}
		}
	}
	return NULL;
}

int CItemListe::GetParameterType(int iRang)
{	ItemCellule *temp=this->GetCellFromRang(iRang);
	if(temp!=NULL) return temp->Item->GetTypeParam();
	return NO;
}

void CItemListe::ModifyStrParam(int iRang,char *NewstrParam)
{	ItemCellule *temp=this->GetCellFromRang(iRang);
	if(temp!=NULL) temp->Item->ModifyStrParam(NewstrParam);
}

BOOL CItemListe::GetSelection(int iRang)
{	ItemCellule *temp=this->GetCellFromRang(iRang);
	if(temp!=NULL) return (temp->Item->GetSelect());		
	return FALSE;
}

int CItemListe::GetItemCount()
{	ItemCellule *temp=TeteListe;
	int iRes=0;
	while(temp!=NULL)
	{	iRes++;
		temp=temp->ItemSuivant;
	}
	return iRes;
}

int CItemListe::GetRangSelected()
{	int Rang=0;
	ItemCellule *temp=TeteListe;
	while(temp!=NULL)
	{	if(temp->Item->GetSelect()) return Rang;
		temp=temp->ItemSuivant;
		Rang++;
	}
	return NO;
}

void CItemListe::SelectItem(int iRang)
{	ItemCellule *temp=this->GetCellFromRang(iRang);
	if(temp!=NULL) temp->Item->ChangeSelected(TRUE);
}

void CItemListe::NoSelection()
{	ItemCellule *temp=TeteListe;
	while(temp!=NULL)
	{	temp->Item->ChangeSelected(FALSE);
		temp=temp->ItemSuivant;
	}
}

void CItemListe::EmptyList()
{	ItemCellule *temp=TeteListe;
	int iIndex=0;
	while(TeteListe!=NULL)
	{	TeteListe=TeteListe->ItemSuivant;
		temp->Item->FreeEvoluList();
		temp->Item->FreeDescription();
		temp->Item->FreeLimite();
		delete temp;
		temp=TeteListe;
		iIndex++;
	}
}

void CItemListe::AttachRepeatParam(int ipos,HWND hWndViewlist)
{	ItemCellule *temp=this->GetCellFromRang(ipos);
	if(temp!=NULL) temp->Item->SetRepeatList(hWndViewlist);
}

int CItemListe::GetParamInteger(int irang,int irepeat,int indice)
{	ItemCellule *temp=this->GetCellFromRang(irang);
	if(temp!=NULL) return temp->Item->GetIntItem(irepeat,indice);
	return NO;
}

char* CItemListe::GetParamString(int irang,int irepeat,int indice)
{	ItemCellule *temp=this->GetCellFromRang(irang);
	if(temp!=NULL) return temp->Item->GetStrItem(irepeat,indice);
	return NULL;
}

BOOL CItemListe::IsRepeating(int irang)
{	ItemCellule *temp=this->GetCellFromRang(irang);
	if(temp!=NULL) return temp->Item->GetRepeatFlag();
	return FALSE;
}

// class CEvoluCell ///////////////////////////////////////////////////////////////////////////////////////////////////////
CEvoluCell::CEvoluCell(int NewItem,int NewType,CRepeatList* NewEvolu)
{	iSelItem=NewItem;
	EvoluItem=NewEvolu;
	iItemType=NewType;
	NextEvoluItem=NULL;
}

BOOL CEvoluCell::CompareEvolu(int iLoop,int CmpType,CRepeatList* CmpList)
{	int index=0,iList;
	if((iItemType==CmpType)&&(EvoluItem->GetEvoluFlag()==CmpList->GetEvoluFlag()))
	{	while(index!=iLoop)
		{	if(iItemType>4) // Liste de liste
			{	switch(iItemType-5)
				{	case 0: // list int
					{	if(EvoluItem->GetListCount(index)!=CmpList->GetListCount(index)) return FALSE;
						iList=EvoluItem->GetListCount(index);
						if(iList!=NO)
						{	while(iList!=0)
							{	if(EvoluItem->GetIntParam(index,(iList-1))!=CmpList->GetIntParam(index,(iList-1))) return FALSE;
								iList--;
							}
						}
						break;
					}
					case 1: // liste float
					case 2: // liste char
					case 3: // liste bool
					case 4: // liste str
					{	if(EvoluItem->GetListCount(index)!=CmpList->GetListCount(index)) return FALSE;
						iList=EvoluItem->GetListCount(index);
						if(iList!=NO)
						{	while(iList!=0)
							{	if(strncmp(EvoluItem->GetStrParam(index,(iList-1)),CmpList->GetStrParam(index,(iList-1)),MAX_RS_SIZE)!=0)
								{	return FALSE;
								}
								iList--;
							}
						}
						break;
					}
				}
			}
			else 
			{	switch(iItemType)
				{	case 0: // int
					case 3: // bool
					{	if(EvoluItem->GetIntParam(index,0)!=CmpList->GetIntParam(index,0)) return FALSE;
						break;
					}
					case 1: // float
					case 2: // char
					case 4: // str
					{	if(strncmp(EvoluItem->GetStrParam(index,0),CmpList->GetStrParam(index,0),MAX_RS_SIZE)!=0)
						{	return FALSE;
						}
						break;
					}
				}
			}
			index++;
			if(!EvoluItem->GetEvoluFlag()) index=iLoop;
		}
		return TRUE;
	}
	return FALSE;
}

// class CEvoluList ///////////////////////////////////////////////////////////////////////////////////////////////////////
CEvoluList::CEvoluList()
{	EvoluList=NULL;}

void CEvoluList::AddEvoluItem(int newItm,int newType,CRepeatList* newList)
{	if(EvoluList==NULL) EvoluList=new CEvoluCell(newItm,newType,newList);
	else
	{	CEvoluCell* temp=EvoluList;
		while(temp->NextEvoluItem!=NULL) temp=temp->NextEvoluItem;
		temp->NextEvoluItem=new CEvoluCell(newItm,newType,newList);
	}
}

int CEvoluList::GetSelItem(int iRang)
{	int indice=0;
	CEvoluCell* temp=EvoluList;
	while((indice!=iRang)&&(temp!=NULL))
	{	temp=temp->NextEvoluItem;
		indice++;
	}
	if(temp!=NULL) return (temp->iSelItem);
	return NO;
}

CEvoluCell* CEvoluList::GetCellFromItem(int iItem)
{	CEvoluCell* temp=EvoluList;
	while(temp!=NULL)
	{	if(temp->iSelItem==iItem) return (temp);
		temp=temp->NextEvoluItem;
	}
	return NULL;
}

int CEvoluList::GetItemType(int iRang)
{	int indice=0;
	CEvoluCell* temp=EvoluList;
	while((indice!=iRang)&&(temp!=NULL))
	{	temp=temp->NextEvoluItem;
		indice++;
	}
	if(temp!=NULL) return (temp->iItemType);
	return NO;
}

int CEvoluList::GetSelCount()
{	int iCnt=0;
	CEvoluCell* temp=EvoluList;
	while(temp!=NULL)
	{	temp=temp->NextEvoluItem;
		iCnt++;
	}
	return iCnt;
}

void CEvoluList::CopyEvoluLst(CEvoluList* CpyEvolLst)
{	CRepeatList* RptLst;
	int iSelItem,iTypeItem,iRepeat;
	this->EmptyEvoluList();
	for(int i=0;i<CpyEvolLst->GetSelCount();i++)
	{	iSelItem=CpyEvolLst->GetSelItem(i);
		iTypeItem=CpyEvolLst->GetItemType(i);
		RptLst=new CRepeatList();
		iRepeat=0;
		while(CpyEvolLst->GetCellFromItem(iSelItem)->EvoluItem->GetRepeatCell(iRepeat)!=NULL)
		{	RptLst->AddTypeList(iTypeItem,CpyEvolLst->GetCellFromItem(iSelItem)->EvoluItem->GetStringParam(iRepeat,iTypeItem),MAX_RS_LISTE);
			iRepeat++;
		}
		RptLst->SetEvoluFlag(CpyEvolLst->GetCellFromItem(iSelItem)->EvoluItem->GetEvoluFlag());
		this->AddEvoluItem(iSelItem,iTypeItem,RptLst);
	}
}

void CEvoluList::EmptyEvoluList()
{	CEvoluCell* temp=EvoluList;
	while(EvoluList!=NULL)
	{	EvoluList=EvoluList->NextEvoluItem;
		temp->EvoluItem->EmptyRepeatList();
		delete(temp);
		temp=EvoluList;
	}
}

// class CFavItem /////////////////////////////////////////////////////////////////////////////////////////////////////////
CFavItem::CFavItem(int* NewRef,int NewLoop,char* NewPath,char* NewEvent,CEvoluList* NewParam)
{	int indice=0;
	iRefTab=(int*)malloc(MAX_PART_TREE);
	if(NewRef!=NULL)
	{	while(NewRef[indice]!=NO)
		{	iRefTab[indice]=NewRef[indice];
			indice++;
		}
	}
	iRefTab[indice]=NO;
	szmHpPath=(char*)malloc(MAX_RS_SIZE);
	szEventStr=(char*)malloc(MAX_RS_SIZE);
	if(NewPath!=NULL) strncpy(szmHpPath,NewPath,MAX_RS_SIZE);
	if(NewEvent!=NULL) strncpy(szEventStr,NewEvent,MAX_RS_SIZE);
	iLoop=NewLoop;
	ParamList=NewParam;
}

CFavItem::~CFavItem()
{	free(szmHpPath);
	free(szEventStr);
	free(iRefTab);
}

void CFavItem::RefreshLoopOpt(int iNewLoop)
{	char lpLastVal[MAX_RS_LISTE]={0};
	int index,iLag;
	CEvoluList* EvolLst=ParamList;
	if(EvolLst!=NULL)
	{	if(iLoop<iNewLoop)
		{	for(index=0;index<EvolLst->GetSelCount();index++)
			{	if(EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetEvoluFlag())
				{	mHpPak_InitStr(lpLastVal,MAX_RS_LISTE);
					strncpy(lpLastVal,EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetStringParam((iLoop-1),EvolLst->GetItemType(index)),MAX_RS_LISTE);
					iLag=iNewLoop-iLoop;
					while(iLag!=0)
					{	EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->AddTypeList(EvolLst->GetItemType(index),lpLastVal,MAX_RS_LISTE);
						iLag--;
					}
				}
			}
		}
		else if(iLoop>iNewLoop)
		{	for(index=0;index<EvolLst->GetSelCount();index++)
			{	if(EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetEvoluFlag())
				{	iLag=iLoop-iNewLoop;
					while(iLag!=0)
					{	EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetRepeatCell((iNewLoop+iLag)-1)->TypeList->EmptyTypeList();
						delete(EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetRepeatCell((iNewLoop+iLag)-1)->TypeList);
						delete(EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetRepeatCell((iNewLoop+iLag)-1));
						iLag--;
						EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetRepeatCell((iNewLoop+iLag)-1)->NextList=NULL;
					}
				}
			}
		}
		else // AddFavItem -> No Compare -> mHpPak_CustomFavLst
		{	for(index=0;index<EvolLst->GetSelCount();index++)
			{	if(!EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetEvoluFlag())
				{	mHpPak_InitStr(lpLastVal,MAX_RS_LISTE);
					strncpy(lpLastVal,EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->GetStringParam(0,EvolLst->GetItemType(index)),MAX_RS_LISTE);
					iLag=iLoop-1;
					while(iLag!=0)
					{	EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->AddTypeList(EvolLst->GetItemType(index),lpLastVal,MAX_RS_LISTE);
						iLag--;
					}
					EvolLst->GetCellFromItem(EvolLst->GetSelItem(index))->EvoluItem->SetEvoluFlag(TRUE);
				}
			}
		}
	}
	iLoop=iNewLoop;
}

// class CFavCell /////////////////////////////////////////////////////////////////////////////////////////////////////////
CFavCell::CFavCell(CFavItem* NewFav)
{	FavItem=NewFav;
	FavNextItem=NULL;
}

BOOL CFavCell::CompareItem(CFavItem* CmpFav)
{	BOOL bSame=TRUE;
	int indice=0,iSel;
	CRepeatList* temp;
	if((!strncmp(FavItem->szmHpPath,CmpFav->szmHpPath,MAX_RS_SIZE))&&(FavItem->iLoop==CmpFav->iLoop))
	{	while((bSame)&&(FavItem->iRefTab[indice]!=NO))
		{	if(FavItem->iRefTab[indice]!=CmpFav->iRefTab[indice]) bSame=FALSE;
			indice++;
		}
		if((bSame)&&(CmpFav->iRefTab[indice]==NO))
		{	if((FavItem->ParamList!=NULL)&&(CmpFav->ParamList!=NULL))
			{	indice=0;
				while((bSame)&&(FavItem->ParamList->GetSelItem(indice)!=NO))
				{	iSel=FavItem->ParamList->GetSelItem(indice);
					if(iSel==CmpFav->ParamList->GetSelItem(indice))
					{	temp=CmpFav->ParamList->GetCellFromItem(iSel)->EvoluItem;
						bSame=FavItem->ParamList->GetCellFromItem(iSel)->CompareEvolu(CmpFav->iLoop,CmpFav->ParamList->GetItemType(indice),temp);
					}
					else bSame=FALSE;
					indice++;
				}
				if((bSame)&&(CmpFav->ParamList->GetSelItem(indice)==NO)) return TRUE;
			}
			else if((FavItem->ParamList==NULL)&&(CmpFav->ParamList==NULL)) return TRUE;
		}
	}
	return FALSE;
}

// class CFavListe ////////////////////////////////////////////////////////////////////////////////////////////////////////
CFavListe::CFavListe()
{	FavList=NULL;
	lpEventRef=(char*)malloc(MAX_RS_SIZE);
	lpAutoAssist=(char*)malloc(MAX_RS_SIZE);
	strncpy(lpAutoAssist,NULL_STRING,MAX_RS_SIZE);
}

CFavListe::~CFavListe()
{	free(lpEventRef);
	free(lpAutoAssist);
}

CFavCell* CFavListe::GetItemFromRang(int iRang)
{	int index=0;
	CFavCell* temp=FavList;
	while((index!=iRang)&&(temp!=NULL))
	{	temp=temp->FavNextItem;
		index++;
	}
	return temp;
}

int CFavListe::AddFavItem(int *RefTab,int Loop,char* Path,char* Event,CEvoluList* ListParam,BOOL bCompare)
{	int iFavIndice=0;
	BOOL bExist=FALSE;
	CFavCell* temp=FavList;
	CFavItem* NewItem=new CFavItem(RefTab,Loop,Path,Event,ListParam);
	if(bCompare)
	{	while((!bExist)&&(temp!=NULL))
		{	bExist=temp->CompareItem(NewItem);
			temp=temp->FavNextItem;
			iFavIndice++;
		}
	}
	else NewItem->RefreshLoopOpt(Loop);
	if(bExist) return (iFavIndice-1);
	if(FavList==NULL) FavList=new CFavCell(NewItem);
	else
	{	temp=FavList;
		while(temp->FavNextItem!=NULL) temp=temp->FavNextItem;
		temp->FavNextItem=new CFavCell(NewItem);
	}
	return NO;
}

void CFavListe::RemoveFavItem(int iRang)
{	CFavCell* Cell=FavList;
	CFavCell* PrevCell;
	if(!iRang)
	{	if(Cell!=NULL)
		{	FavList=Cell->FavNextItem;
			if(Cell->FavItem->ParamList!=NULL)
			{	Cell->FavItem->ParamList->EmptyEvoluList();}
			delete(Cell->FavItem);
			delete(Cell);
		}
	}
	else
	{	Cell=this->GetItemFromRang(iRang);
		PrevCell=this->GetItemFromRang((iRang-1));
		if((Cell!=NULL)&&(PrevCell!=NULL))
		{	PrevCell->FavNextItem=Cell->FavNextItem;
			if(Cell->FavItem->ParamList!=NULL)
			{	Cell->FavItem->ParamList->EmptyEvoluList();}
			delete(Cell->FavItem);
			delete(Cell);
		}
	}
}

void CFavListe::SetAutoAssist(char* lpNewAutoAss)
{	mHpPak_InitStr(lpAutoAssist,MAX_RS_SIZE);
	strncpy(lpAutoAssist,lpNewAutoAss,MAX_RS_SIZE);
}

int CFavListe::GetFavCount()
{	CFavCell* Cell=FavList;
	int iFavCnt=0;
	while(Cell!=NULL)
	{	Cell=Cell->FavNextItem;
		iFavCnt++;
	}
	return iFavCnt;
}

void CFavListe::RefreshAutoDecl(int iCurFav,int iRmvCnt)
{	char lpAutoDecl[MAX_RS_SIZE]={0};
	int iRow,indice=0;
	BOOL bFirst=TRUE;
	mHpPak_InitStr(lpAutoDecl,MAX_RS_SIZE);
	// Remove & Lag
	iRow=mHpPak_GetAutoAssRow(this->GetAutoAssist(),indice);
	while(iRow!=NO)
	{	if((iCurFav-iRmvCnt+2)!=iRow)
		{	if((iCurFav-iRmvCnt+2)<iRow) iRow--;
			if(bFirst)
			{	_snprintf(lpAutoDecl,MAX_RS_SIZE,"%d",iRow);
				bFirst=FALSE;
			}
			else _snprintf(lpAutoDecl,MAX_RS_SIZE,"%s%s%d",lpAutoDecl,STR_SEPARATED_AUTOASS,iRow);
		}
		indice++;
		iRow=mHpPak_GetAutoAssRow(this->GetAutoAssist(),indice);
	}
	this->SetAutoAssist(lpAutoDecl);
}			

BOOL CFavListe::RemoveFromFile(char* lpNewFile)
{	CFavCell* Cell=FavList;
	CFavCell* LastCell=FavList;
	BOOL bRemove=FALSE;
	int iFav=0,iRmv=0;
	while(Cell!=NULL)
	{	if(!strncmp(Cell->FavItem->szmHpPath,lpNewFile,MAX_RS_SIZE))
		{	if(LastCell==Cell)
			{	FavList=FavList->FavNextItem;
				LastCell=FavList;
				if(Cell->FavItem->ParamList!=NULL)
				{	Cell->FavItem->ParamList->EmptyEvoluList();}
				delete(Cell->FavItem);
				delete(Cell);
				Cell=FavList;
			}
			else
			{	LastCell->FavNextItem=Cell->FavNextItem;
				if(Cell->FavItem->ParamList!=NULL)
				{	Cell->FavItem->ParamList->EmptyEvoluList();}
				delete(Cell->FavItem);
				delete(Cell);
				Cell=LastCell->FavNextItem;
			}
			bRemove=TRUE;
			if(strncmp(this->lpAutoAssist,NULL_STRING,MAX_RS_SIZE)!=0)
			{	this->RefreshAutoDecl(iFav,iRmv++);}
		}
		else
		{	LastCell=Cell;
			Cell=Cell->FavNextItem;
		}
		iFav++;
	}
	return bRemove;
}

BOOL CFavListe::ChangeRefFromFile(char* lpFile,int* iChgRef)
{	CFavCell* Cell=FavList;
	BOOL bChg=FALSE;
	int index;
	while(Cell!=NULL)
	{	if((!strncmp(Cell->FavItem->szmHpPath,lpFile,MAX_RS_SIZE))&&(mHpPak_CompareTab(Cell->FavItem->iRefTab,iChgRef)))
		{	index=0;
			while(Cell->FavItem->iRefTab[index]!=NO) index++;
			Cell->FavItem->iRefTab[(index-1)]+=1;
			bChg=TRUE;
		}
		Cell=Cell->FavNextItem;
	}	
	return bChg;
}

BOOL CFavListe::ChangeRefFromTask(char* lpFile,int* iChgRef,int* iLgRef)
{	CFavCell* Cell=FavList;
	CFavCell* LastCell=FavList;
	BOOL bRmv,bChg=FALSE;
	int iWriteRes=0,iFav=0,iRmv=0;
	while(Cell!=NULL)
	{	bRmv=FALSE;
		if(!strncmp(Cell->FavItem->szmHpPath,lpFile,MAX_RS_SIZE))
		{	if(Cell->FavItem->ParamList!=NULL)
			{	GetNewLagStrRef(Cell->FavItem->iRefTab,iChgRef,iLgRef,NULL,0,2,TRUE,iWriteRes);}
			else GetNewLagStrRef(Cell->FavItem->iRefTab,iChgRef,iLgRef,NULL,0,2,FALSE,iWriteRes);
			switch(iWriteRes)
			{	case 0: // Remove
				{	if(Cell==FavList)
					{	FavList=FavList->FavNextItem;
						if(Cell->FavItem->ParamList!=NULL)
						{	Cell->FavItem->ParamList->EmptyEvoluList();}
						delete(Cell->FavItem);
						delete(Cell);
						Cell=FavList;
						LastCell=Cell;
					}
					else
					{	LastCell->FavNextItem=Cell->FavNextItem;
						if(Cell->FavItem->ParamList!=NULL)
						{	Cell->FavItem->ParamList->EmptyEvoluList();}
						delete(Cell->FavItem);
						delete(Cell);
						Cell=LastCell->FavNextItem;
					}
					bChg=TRUE;
					bRmv=TRUE;
					iRmv++;
					break;
				}
				case 1:	// No change
				{	LastCell=Cell;
					break;
				}
				case 2: // Changed
				{	LastCell=Cell;
					bChg=TRUE;
					break;
				}
			}
			if((!iWriteRes)&&(strncmp(this->lpAutoAssist,NULL_STRING,MAX_RS_SIZE)!=0))
			{	this->RefreshAutoDecl(iFav,iRmv);}
		}
		else LastCell=Cell;
		if((Cell!=NULL)&&(!bRmv)) Cell=Cell->FavNextItem;
		iFav++;
	}	
	return bChg;
}

void CFavListe::EmptyFavList()
{	CFavCell* Cell=FavList;
	strncpy(lpAutoAssist,NULL_STRING,MAX_RS_SIZE);
	while(FavList!=NULL)
	{	FavList=FavList->FavNextItem;
		if(Cell->FavItem->ParamList!=NULL)
		{	Cell->FavItem->ParamList->EmptyEvoluList();}
		delete(Cell->FavItem);
		delete(Cell);
		Cell=FavList;
	}
}

int CFavListe::GetLoopOption(int iRang)
{	CFavCell* Cell=this->GetItemFromRang(iRang);
	if(Cell!=NULL) return (Cell->FavItem->iLoop);
	return NO;
}

char* CFavListe::GetmHpPath(int iRang)
{	CFavCell* Cell=this->GetItemFromRang(iRang);
	if(Cell!=NULL) return (Cell->FavItem->szmHpPath);
	return NULL;
}

char* CFavListe::GetEventStr(int iRang)
{	CFavCell* Cell=this->GetItemFromRang(iRang);
	if(Cell!=NULL) return (Cell->FavItem->szEventStr);
	return NULL;
}

char* CFavListe::GetEventRef(int iRang)
{	CFavCell* Cell=this->GetItemFromRang(iRang);
	char lpTmp[MAX_RS_SIZE]={0};
	int iRef=0,iSep=0;
	mHpPak_InitStr(lpEventRef,MAX_RS_SIZE);
	if(Cell!=NULL)
	{	while(Cell->FavItem->iRefTab[iRef]!=NO)
		{	mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
			_snprintf(lpTmp,MAX_RS_SIZE,"%d",Cell->FavItem->iRefTab[iRef]);
			if(!iRef) strncpy(lpEventRef,lpTmp,MAX_RS_SIZE);
			else strncat(lpEventRef,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
			iRef++;
			if(Cell->FavItem->iRefTab[iRef]!=NO) strncat(lpEventRef,SEPARATED_STRING,1);
		}
		return lpEventRef;
	}
	return NULL;
}

char* CFavListe::GetAutoAssist()
{	return lpAutoAssist;}

// class CControlCell /////////////////////////////////////////////////////////
CControlCell::CControlCell(UINT iPlatform,const char* lpClass,const char* lpVersion) : NextCtrlCell(NULL)
{   iPlatformID=iPlatform;
    strncpy(lpClassName,lpClass,MAX_CLASS_SIZE);
    strncpy(lpVersionDll,lpVersion,MAX_VERSION_SIZE);
}

// class CControlList /////////////////////////////////////////////////////////
CControlList::CControlList() : CtrlHead(NULL), bUpdated(FALSE)
{}
CControlList::~CControlList()
{   EmptyList();
}

//////
int CControlList::AddControl(UINT iPlatform,const char* lpClass,const char* lpVersion)
{   int iRes=0;
    if(!CtrlHead)
        CtrlHead=new CControlCell(iPlatform,lpClass,lpVersion);
    else
    {   CControlCell* LastCtrlCell=CtrlHead;
        while(LastCtrlCell->NextCtrlCell!=NULL)
        {   LastCtrlCell=LastCtrlCell->NextCtrlCell;
            iRes++;
        }
        LastCtrlCell->NextCtrlCell=new CControlCell(iPlatform,lpClass,lpVersion);
        iRes++;
    }
    bUpdated=TRUE; // List updated
    return iRes;
}

const char* CControlList::GetClassName(int CtrlIdx) const
{   int WalkIdx=0;
    CControlCell* CurCtrlCell=CtrlHead;
    while(CurCtrlCell!=NULL)
    {   if(CtrlIdx==WalkIdx)
            return CurCtrlCell->lpClassName;
        CurCtrlCell=CurCtrlCell->NextCtrlCell;
        WalkIdx++;
    }
    return NULL;
}

const char* CControlList::GetVersionDll(int CtrlIdx) const
{   int WalkIdx=0;
    CControlCell* CurCtrlCell=CtrlHead;
    while(CurCtrlCell!=NULL)
    {   if(CtrlIdx==WalkIdx)
            return CurCtrlCell->lpVersionDll;
        CurCtrlCell=CurCtrlCell->NextCtrlCell;
        WalkIdx++;
    }
    return NULL;
}

UINT CControlList::GetPlatform(int CtrlIdx) const
{   int WalkIdx=0;
    CControlCell* CurCtrlCell=CtrlHead;
    while(CurCtrlCell!=NULL)
    {   if(CtrlIdx==WalkIdx)
            return CurCtrlCell->iPlatformID;
        CurCtrlCell=CurCtrlCell->NextCtrlCell;
        WalkIdx++;
    }
    return 0;
}

int CControlList::GetClassIndex(UINT iPlatform,const char* lpClass) const
{   int iResIdx=0;
    CControlCell* CurCtrlCell=CtrlHead;
    while(CurCtrlCell!=NULL)
    {   if((CurCtrlCell->iPlatformID==iPlatform)&&(!strncmp(CurCtrlCell->lpClassName,lpClass,MAX_CLASS_SIZE)))
            return iResIdx; // Found
        CurCtrlCell=CurCtrlCell->NextCtrlCell;
        iResIdx++;
    }
    return NO; // Not found
}

const CControlCell* CControlList::GetCtrlCell(UINT iIdx) const
{   CControlCell* CurCell=CtrlHead;
    while((CurCell!=NULL)&&(iIdx!=0))
    {   CurCell=CurCell->NextCtrlCell;
        iIdx--;
    }
    return CurCell;
}

void CControlList::SetUpdated(BOOL bUpdFlag)
{   bUpdated=bUpdFlag;
}

BOOL CControlList::IsUpdated() const
{   return bUpdated;
}

void CControlList::EmptyList()
{   CControlCell* CurCtrlCell=CtrlHead;
    while(CurCtrlCell!=NULL)
    {   CControlCell* NextCtrlCell=CurCtrlCell->NextCtrlCell;
        delete CurCtrlCell;
        CurCtrlCell=NextCtrlCell;
    }
    CtrlHead=NULL;
}

BOOL CControlList::IsEmpty() const
{   return (CtrlHead!=NULL)? FALSE:TRUE;
}

// class CWindowCell //////////////////////////////////////////////////////////////////////////////////////////////////////
CWindowCell::CWindowCell(char* lpClass,char* lpTitle,BOOL bWndActive)
{	mHpPak_InitStr(lpWndClass,MAX_RS_SIZE);
	strncpy(lpWndClass,lpClass,MAX_RS_SIZE);
	mHpPak_InitStr(lpWndTitle,MAX_RS_SIZE);
	strncpy(lpWndTitle,lpTitle,MAX_RS_SIZE);
	bActive=bWndActive;
	NextWndCell=NULL;
}

// class CAppWindow ///////////////////////////////////////////////////////////////////////////////////////////////////////
CAppWindow::CAppWindow(char* lpapp,char* lpclass,char* lptitle,BOOL bWndActive)
{	AppWndCell=new CWindowCell(lpclass,lptitle,bWndActive);
	mHpPak_InitStr(lpAppName,MAX_RS_SIZE);
	strncpy(lpAppName,lpapp,MAX_RS_SIZE);
	this->InitCurWndTab();
	NextAppCell=NULL;
}

void CAppWindow::InitCurWndTab()
{	for(int i=0;i<MAX_PART_TREE;i++) iCurWndTab[i]=i;
}

// class CWindowListe /////////////////////////////////////////////////////////////////////////////////////////////////////
CWindowListe::CWindowListe()
{	AppWndHead=NULL;
	hWndCurrent=NULL;
}

CWindowCell* CWindowListe::GetWndCellFrom(int iApp,int iWnd)
{	CAppWindow* tmp=AppWndHead;
	CWindowCell* cell=NULL;
	int iwnd=0,iapp=0;
	while((tmp!=NULL)&&(iapp!=iApp))
	{	iapp++;
		tmp=tmp->NextAppCell;
	}
	if(tmp!=NULL)
	{	cell=tmp->AppWndCell;
		while((cell!=NULL)&&(iwnd!=iWnd))
		{	iwnd++;
			cell=cell->NextWndCell;
		}
	}
	return cell;
}

int CWindowListe::AddWndInfo(char* AppName,char* Class,char* Title,BOOL bActiveWnd)
{	char TmpClass[MAX_RS_SIZE]={0};
	CAppWindow* temp;
	CAppWindow* tmp=AppWndHead;
	CWindowCell* cell=NULL;
	int iPos=0;
	GetNoneEvolClass(TmpClass,MAX_RS_SIZE,Class);
	if(AppWndHead==NULL) AppWndHead=new CAppWindow(AppName,TmpClass,Title,bActiveWnd);
	else
	{	while((tmp!=NULL)&&(strncmp(tmp->lpAppName,AppName,MAX_RS_SIZE)!=0))
		{	temp=tmp;
			tmp=tmp->NextAppCell;
		}
		if(tmp==NULL) temp->NextAppCell=new CAppWindow(AppName,TmpClass,Title,bActiveWnd);
		else
		{	cell=tmp->AppWndCell;
			while(cell->NextWndCell!=NULL)
			{	iPos++;
				cell=cell->NextWndCell;
			}
			cell->NextWndCell=new CWindowCell(TmpClass,Title,bActiveWnd);
			return (iPos+1);
		}
	}
	return 0;
}

void CWindowListe::UpdateWndLst(char* lpAppli,HWND hWndNew,int &iCurWind,int &iCntWind)
{	char lpTmpClass[MAX_RS_SIZE]={0};
	char lpTmpTitle[MAX_RS_SIZE]={0};
	CAppWindow* cell=AppWndHead;
	CWindowCell* tmp=NULL;
	long lstyle;
	iCurWind=0;
	//while((cell!=NULL)&&(_strnicmp(cell->lpAppName,lpAppli,MAX_RS_SIZE)!=0)) cell=cell->NextAppCell;  // In case where the 'ExeName' case sensitive has changed
    while((cell!=NULL)&&(strncmp(cell->lpAppName,lpAppli,MAX_RS_SIZE)!=0)) cell=cell->NextAppCell;     // and can be different than in an existing mHelp file
	if(cell!=NULL)
	{	lstyle=(long)GetWindowLongPtr(hWndNew,GWL_STYLE);
		if((lstyle&WS_GROUP)!=WS_GROUP)
		{	tmp=cell->AppWndCell;
			hWndCurrent=hWndNew;
			GetClassName(hWndNew,lpTmpClass,MAX_RS_SIZE);
			GetWindowText(hWndNew,lpTmpTitle,MAX_RS_SIZE);
			while(tmp!=NULL)
			{	if((strstr(lpTmpClass,tmp->lpWndClass)!=NULL)&&(!strncmp(tmp->lpWndTitle,lpTmpTitle,MAX_RS_SIZE)))
				{	if(!tmp->bActive)
					{	tmp->bActive=TRUE;
						iCntWind++;
					}
					return;
				}
				iCurWind++;
				tmp=tmp->NextWndCell;
			}
			this->AddWndInfo(lpAppli,lpTmpClass,lpTmpTitle,TRUE);
			iCntWind++;
		}
	}
}

int CWindowListe::InactiveWnd(char* lpAppli,char* lpCloseClass,char* lpCloseTitle)
{	CAppWindow* cell=AppWndHead;
	CWindowCell* tmp=NULL;
	int iResPos=0;
	while((cell!=NULL)&&(strncmp(cell->lpAppName,lpAppli,MAX_RS_SIZE)!=0)) cell=cell->NextAppCell;
	if(cell!=NULL)
	{	tmp=cell->AppWndCell;
		while(tmp!=NULL)
		{	if((strstr(lpCloseClass,tmp->lpWndClass)!=NULL)&&(!strncmp(tmp->lpWndTitle,lpCloseTitle,MAX_RS_SIZE)))
			{	tmp->bActive=FALSE;
				return iResPos;
			}
			tmp=tmp->NextWndCell;
			iResPos++;
		}
	}
	return NO;
}

char* CWindowListe::GetWndClass(int iApp,int iWnd)
{	CWindowCell* temp=this->GetWndCellFrom(iApp,iWnd);
	if(temp!=NULL) return temp->lpWndClass;
	return NULL;
}

char* CWindowListe::GetWndTitle(int iApp,int iWnd)
{	CWindowCell* temp=this->GetWndCellFrom(iApp,iWnd);
	if(temp!=NULL) return temp->lpWndTitle;
	return NULL;
}

BOOL CWindowListe::GetWndActive(int iApp,int iWnd)
{	CWindowCell* temp=this->GetWndCellFrom(iApp,iWnd);
	if(temp!=NULL) return temp->bActive;
	return FALSE;
}

CAppWindow* CWindowListe::GetAppWndHead()
{	return AppWndHead;}

void CWindowListe::FusionWndLst(CWindowListe* AppWndFile)
{	CAppWindow* tmp=AppWndHead;
	CAppWindow* rdtmp;
	CWindowCell* cell;
	CWindowCell* rdcell;
	int iCur,irdCur;
	BOOL bExist;
	AppWndFile->InitCurWnd();
	while(tmp!=NULL)
	{	bExist=FALSE;
		rdtmp=AppWndFile->GetAppWndHead();
		while(rdtmp!=NULL)
		{	if(!strncmp(rdtmp->lpAppName,tmp->lpAppName,MAX_RS_SIZE))
			{	cell=tmp->AppWndCell;
				iCur=0;
				while(cell!=NULL)
				{	rdcell=rdtmp->AppWndCell;
					irdCur=0;
					while(rdcell!=NULL)
					{	if((!strncmp(rdcell->lpWndClass,cell->lpWndClass,MAX_RS_SIZE))&&
						   (!strncmp(rdcell->lpWndTitle,cell->lpWndTitle,MAX_RS_SIZE)))
						{	AppWndFile->GetCurWnd(tmp->lpAppName)[iCur]=irdCur;
							bExist=TRUE;
						}
						irdCur++;
						rdcell=rdcell->NextWndCell;
					}
					if(!bExist)
					{	AppWndFile->GetCurWnd(tmp->lpAppName)[iCur]=AppWndFile->AddWndInfo(tmp->lpAppName,cell->lpWndClass,cell->lpWndTitle,TRUE);}
					bExist=FALSE;
					iCur++;
					cell=cell->NextWndCell;
				}
				bExist=TRUE;
			}
			rdtmp=rdtmp->NextAppCell;
		}
		if(!bExist)
		{	cell=tmp->AppWndCell;
			while(cell!=NULL)
			{	AppWndFile->AddWndInfo(tmp->lpAppName,cell->lpWndClass,cell->lpWndTitle,TRUE);
				cell=cell->NextWndCell;
			}
		}
		tmp=tmp->NextAppCell;	
	}
}

char* CWindowListe::GetWndAppName(int iAppl)
{	int iapp=0;
	CAppWindow* tmp=AppWndHead;
	while((tmp!=NULL)&&(iapp!=iAppl))
	{	iapp++;
		tmp=tmp->NextAppCell;
	}
	if(tmp!=NULL) return tmp->lpAppName;
	return NULL;
}

int* CWindowListe::GetCurWnd(char* lpExeName)
{	CAppWindow* tmp=AppWndHead;
	while((tmp!=NULL)&&(strncmp(tmp->lpAppName,lpExeName,MAX_RS_SIZE)!=0)) tmp=tmp->NextAppCell;
	if(tmp!=NULL) return tmp->iCurWndTab;
	return NULL;
}

void CWindowListe::InitCurWnd()
{	CAppWindow* tmp=AppWndHead;
	while(tmp!=NULL)
	{	tmp->InitCurWndTab();
		tmp=tmp->NextAppCell;
	}
}

void CWindowListe::CopyWindowList(CWindowListe* hWndListSrc)
{	CAppWindow* tmp=hWndListSrc->GetAppWndHead();
	CWindowCell* cell;
	this->EmptyWndLst();
	while(tmp!=NULL)
	{	cell=tmp->AppWndCell;
		while(cell!=NULL)
		{	this->AddWndInfo(tmp->lpAppName,cell->lpWndClass,cell->lpWndTitle,cell->bActive);
			cell=cell->NextWndCell;
		}
		tmp=tmp->NextAppCell;
	}
}

/*void CWindowListe::View()
{	CAppWindow* tmp=AppWndHead;
	CWindowCell* cell;
	while(tmp!=NULL)
	{	cell=tmp->AppWndCell;
		MessageBox(NULL,tmp->lpAppName,"AppName",MB_OK);
		while(cell!=NULL)
		{	MessageBox(NULL,cell->lpWndClass,"Class",MB_OK);
			MessageBox(NULL,cell->lpWndTitle,"Title",MB_OK);
			cell=cell->NextWndCell;
		}
		tmp=tmp->NextAppCell;
	}
}*/

HWND CWindowListe::GetCurWindow()
{	return hWndCurrent;}

BOOL CWindowListe::ReadAppWndKey()
{	HKEY AppKey,WndKey,CellKey;
	char lpTmp[MAX_KEY_SIZE]={0};
	char lpApp[MAX_RS_SIZE]={0};
	char lpClass[MAX_RS_SIZE]={0};
	char lpTitle[MAX_RS_SIZE]={0};
	int iApp=0,iWnd=0,iActiveReg=0;
	BYTE szBTab[MAX_KEY_SIZE]={0};
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_BINARY;
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&AppKey)!=ERROR_SUCCESS) return FALSE;
	// mHelp	
	if(RegOpenKeyEx(AppKey,MHELP_KEY,0,KEY_READ,&AppKey)!=ERROR_SUCCESS) return FALSE;
	// mHpShared
	if(RegOpenKeyEx(AppKey,MHPSHARED_KEY,0,KEY_READ,&AppKey)!=ERROR_SUCCESS) return FALSE;
	// mHpWnd
	if(RegOpenKeyEx(AppKey,MHPWND_KEY,0,KEY_READ,&AppKey)!=ERROR_SUCCESS) return FALSE;
	_snprintf(lpTmp,MAX_KEY_SIZE,"%d",iApp);
	// "mHpWnd" key found, now we can empty the "mHpPak_ProcWndLst"
	this->EmptyWndLst();
	//
	while(RegOpenKeyEx(AppKey,lpTmp,0,KEY_READ,&WndKey)==ERROR_SUCCESS)
	{	// lpAppName
		if(!mHpPak_ReadLongStrRegVal(WndKey,MHPWND_APP,lpApp,MAX_RS_SIZE))
		{	this->EmptyWndLst();
			return FALSE;
		}
		iWnd=0;
		mHpPak_InitStr(lpTmp,MAX_KEY_SIZE);
		_snprintf(lpTmp,MAX_KEY_SIZE,"%d",iWnd);
		while(RegOpenKeyEx(WndKey,lpTmp,0,KEY_READ,&CellKey)==ERROR_SUCCESS)
		{	// lpWndClass
			if(!mHpPak_ReadLongStrRegVal(CellKey,MHPWND_CLASS,lpClass,MAX_RS_SIZE))
			{	this->EmptyWndLst();
				return FALSE;
			}
			// lpWndTitle
			if(!mHpPak_ReadLongStrRegVal(CellKey,MHPWND_TITLE,lpTitle,MAX_RS_SIZE))
			{	this->EmptyWndLst();
				return FALSE;
			}
			// bActive
			if(RegQueryValueEx(CellKey,MHPWND_STATUS,NULL,&dwType,szBTab,&iSize)!=ERROR_SUCCESS)
			{	this->EmptyWndLst();
				return FALSE;
			}
			mHpPak_FillIntFromData(szBTab,iActiveReg,TRUE);
			this->AddWndInfo(lpApp,lpClass,lpTitle,(BOOL)iActiveReg);
			iWnd++;
			mHpPak_InitStr(lpTmp,MAX_KEY_SIZE);
			_snprintf(lpTmp,MAX_KEY_SIZE,"%d",iWnd);
			RegCloseKey(CellKey);
		}
		iApp++;
		mHpPak_InitStr(lpTmp,MAX_KEY_SIZE);
		_snprintf(lpTmp,MAX_KEY_SIZE,"%d",iApp);
		RegCloseKey(WndKey);
	}
	RegCloseKey(AppKey);
	return TRUE;
}

BOOL CWindowListe::WriteAppWndKey()
{	SECURITY_ATTRIBUTES lpSecuAtt;
	char lpAppli[MAX_KEY_SIZE]={0};
	char lpWind[MAX_KEY_SIZE]={0};
	HKEY lpNewKey,lpAppKey,lpWndKey;
	DWORD DWord=REG_OPENED_EXISTING_KEY;
	BYTE TabByte[MAX_KEY_SIZE]={0};
	int iapp=0,iwnd=0,iSize;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecuAtt,bAclRes);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHelp
	if(RegCreateKeyEx(lpAppKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHpShared
	if(RegCreateKeyEx(lpAppKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHpWnd
	if(RegCreateKeyEx(lpAppKey,MHPWND_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpNewKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	mHpPak_DeleteSubKey(lpAppKey,MHPWND_KEY);
	if(RegCreateKeyEx(lpAppKey,MHPWND_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	_snprintf(lpAppli,MAX_KEY_SIZE,"%d",iapp);
	while(this->GetWndCellFrom(iapp,0)!=NULL)
	{	if(RegCreateKeyEx(lpAppKey,lpAppli,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
						&lpWndKey,&DWord)!=ERROR_SUCCESS) return FALSE;
		// lpAppName
		if(!mHpPak_WriteLongStrRegVal(lpAppKey,lpAppli,MHPWND_APP,this->GetWndAppName(iapp))) return FALSE;
		iwnd=0;
		mHpPak_InitStr(lpWind,MAX_KEY_SIZE);
		_snprintf(lpWind,MAX_KEY_SIZE,"%d",iwnd);
		while(this->GetWndCellFrom(iapp,iwnd)!=NULL)
		{	if(RegCreateKeyEx(lpWndKey,lpWind,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
							&lpNewKey,&DWord)!=ERROR_SUCCESS) return FALSE;
			// lpWndClass
			if(!mHpPak_WriteLongStrRegVal(lpWndKey,lpWind,MHPWND_CLASS,this->GetWndClass(iapp,iwnd))) return FALSE;
			// lpWndTitle
			if(!mHpPak_WriteLongStrRegVal(lpWndKey,lpWind,MHPWND_TITLE,this->GetWndTitle(iapp,iwnd))) return FALSE;
			// bActive
			if(RegCreateKeyEx(lpWndKey,lpWind,0,NULL,REG_OPTION_NON_VOLATILE,
							KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
							&lpNewKey,&DWord)!=ERROR_SUCCESS) return FALSE;
			iSize=mHpPak_FillDataFromNumber(TabByte,NO,this->GetWndActive(iapp,iwnd));
			RegSetValueEx(lpNewKey,MHPWND_STATUS,0,REG_BINARY,TabByte,iSize);
			iwnd++;
			mHpPak_InitStr(lpWind,MAX_KEY_SIZE);
			_snprintf(lpWind,MAX_KEY_SIZE,"%d",iwnd);
			RegCloseKey(lpNewKey);
		}
		iapp++;
		mHpPak_InitStr(lpAppli,MAX_KEY_SIZE);
		_snprintf(lpAppli,MAX_KEY_SIZE,"%d",iapp);
		RegCloseKey(lpWndKey);
	}
	return TRUE;
}

BOOL CWindowListe::IsWndInList(char* lpExeStr,char* lpCmpClass,char* lpCmpTitle)
{	CAppWindow* cell=AppWndHead;
	CWindowCell* tmp=NULL;
	while((cell!=NULL)&&(strncmp(cell->lpAppName,lpExeStr,MAX_RS_SIZE)!=0)) cell=cell->NextAppCell;
	if(cell!=NULL)
	{	tmp=cell->AppWndCell;
		while(tmp!=NULL)
		{	if((strstr(lpCmpClass,tmp->lpWndClass)!=NULL)&&(!strncmp(tmp->lpWndTitle,lpCmpTitle,MAX_RS_SIZE)))
			{	return TRUE;}
			tmp=tmp->NextWndCell;
		}
	}
	return FALSE;
}

void CWindowListe::EmptyWndLst()
{	CAppWindow* Cell=AppWndHead;
	CWindowCell* WndCell;
	while(AppWndHead!=NULL)
	{	AppWndHead=AppWndHead->NextAppCell;
		WndCell=Cell->AppWndCell;
		while(Cell->AppWndCell!=NULL)
		{	Cell->AppWndCell=Cell->AppWndCell->NextWndCell;
			delete(WndCell);
			WndCell=Cell->AppWndCell;
		}
		delete(Cell);
		Cell=AppWndHead;
	}
}

// class CUnEventCell /////////////////////////////////////////////////////////////////////////////////////////////////////
CUnEventCell::CUnEventCell(char* lpNewUn,bool bFull)
{	mHpPak_InitStr(lpUnClass,MAX_RS_SIZE);
	strncpy(lpUnClass,lpNewUn,MAX_RS_SIZE);
    bFullSearch=bFull;
	NextUnCell=NULL;
}

// class CUnEventListe ////////////////////////////////////////////////////////////////////////////////////////////////////
CUnEventListe::CUnEventListe()
{	HeadUnEvnt=NULL;}

BOOL CUnEventListe::IsItUnEvent(char* lpCmpUn)
{	CUnEventCell* tmp=HeadUnEvnt;
	while(tmp!=NULL)
	{	if((tmp->bFullSearch)&&(!strncmp(tmp->lpUnClass,lpCmpUn,MAX_RS_SIZE))||
           (!tmp->bFullSearch)&&(!strncmp(tmp->lpUnClass,lpCmpUn,strlen(tmp->lpUnClass)))) return TRUE;
		tmp=tmp->NextUnCell;
	}
	return FALSE;
}

BOOL CUnEventListe::AddUnEvent(char* lpNewEvnt,bool bFull)
{	CUnEventCell* tmp=HeadUnEvnt;
	if(this->IsItUnEvent(lpNewEvnt)) return FALSE;
	if(tmp==NULL) HeadUnEvnt=new CUnEventCell(lpNewEvnt,bFull);
	else
	{	while(tmp->NextUnCell!=NULL) tmp=tmp->NextUnCell;
		tmp->NextUnCell=new CUnEventCell(lpNewEvnt,bFull);
	}
	return TRUE;
}

void CUnEventListe::EmptyUnEvent()
{	CUnEventCell* Cell=HeadUnEvnt;
	while(HeadUnEvnt!=NULL)
	{	HeadUnEvnt=HeadUnEvnt->NextUnCell;
		delete(Cell);
		Cell=HeadUnEvnt;
	}
}

// class CWndDocCell //////////////////////////////////////////////////////////////////////////////////////////////////////
CWndDocCell::CWndDocCell(HWND phWnd,LONG_PTR plProc)
{	hWndDoc=phWnd;
	lDocWndProc=plProc;
	NextDocWnd=NULL;
}

// class CWndDocList //////////////////////////////////////////////////////////////////////////////////////////////////////
CWndDocList::CWndDocList()
{	MainCell=NULL;}

void CWndDocList::AddDocWnd(HWND hNewWnd,LONG_PTR NewProc)
{	CWndDocCell* tmp=MainCell;
	if(tmp==NULL) MainCell=new CWndDocCell(hNewWnd,SetWindowLongPtr(hNewWnd,GWLP_WNDPROC,NewProc));
	else
	{	tmp=MainCell;
		while(tmp->NextDocWnd!=NULL) tmp=tmp->NextDocWnd;
		tmp->NextDocWnd=new CWndDocCell(hNewWnd,SetWindowLongPtr(hNewWnd,GWLP_WNDPROC,NewProc));
	}
}

void CWndDocList::RemoveDocWnd(HWND hDestroyWnd)
{	CWndDocCell *find=MainCell;
	CWndDocCell *move=MainCell;
	while(find!=NULL)
	{	if(find->hWndDoc==hDestroyWnd)
		{	if(move==find)
			{	MainCell=move->NextDocWnd;
				delete move;
			}
			else
			{	while(move->NextDocWnd!=find) move=move->NextDocWnd;
				move->NextDocWnd=find->NextDocWnd;
				delete find;
			}
			return;
		}
		find=find->NextDocWnd;
	}
}

LONG_PTR CWndDocList::GetDocWndProc(HWND hCurWnd)
{	CWndDocCell* tmp=MainCell;
	LONG_PTR lRes=0;
	while(tmp!=NULL)
	{	if(tmp->hWndDoc==hCurWnd) lRes=tmp->lDocWndProc;
		tmp=tmp->NextDocWnd;
	}
	return lRes;
}

//**************************************************************************************************************VARIABLES//

MHPPAK_API CItemListe* mHpPak_ItemsList=NULL;
MHPPAK_API CFavListe* mHpPak_FavsList=NULL;
MHPPAK_API CFavListe* mHpPak_LnchFavLst=NULL;
MHPPAK_API CFavListe* mHpPak_CustomFavLst=NULL;
MHPPAK_API CWindowListe* mHpPak_ProcWndLst=NULL;
MHPPAK_API CControlList* mHpPak_ProcCtrlList=NULL;
MHPPAK_API CUnEventListe* mHpPak_UnEventList=NULL;
MHPPAK_API BOOL mHpPak_bAutoAss=FALSE;
MHPPAK_API BOOL mHpPak_bLnchProc=FALSE;
MHPPAK_API BOOL mHpPak_bLnchCustom=FALSE;
MHPPAK_API int mHpPak_iFavIndex=0;
MHPPAK_API BOOL mHpPak_bLockChangeWnd=FALSE;

//*****************************************************************************************************************COMMON//
// mHpPak_GetIntFromStr
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetIntFromStr(char *lpStr,int iSizeStr,int decal)
{	int iDecal=decal,iVal=0,limite=mHpPak_FindStrLenght(lpStr,iSizeStr);
	for(int i=0;i<limite;i++)
	{	if(iDecal<=0)
		{	if(mHpPak_IsItANumber(lpStr[i]))
			{	iVal*=10;
				iVal+=mHpPak_CharToInt(lpStr[i]);
			}
			else return iVal;
		}
		else iDecal--;
	}
	return iVal;
}

//-----------------------------------------------------//
// mHpPak_mHpMessage
//-----------------------------------------------------//
MHPPAK_API int mHpPak_mHpMessage(HWND hWndActive,HINSTANCE hInst,UINT IDsA,char* MidStr,int MidSize,UINT IDsB,UINT uiIcon)
{	return mHpMessage(hWndActive,hInst,IDsA,MidStr,MidSize,IDsB,uiIcon);}

//-----------------------------------------------------//
// mHpPak_SetViewMsgFlag
//-----------------------------------------------------//
MHPPAK_API void mHpPak_SetViewMsgFlag(BOOL bView)
{   SetViewFlag(bView);}

//---------------------------------------------------------//
// mHpPak_GetValueNumber
//---------------------------------------------------------//
MHPPAK_API int mHpPak_GetValueNumber(char *lpValue,int iSizeValue)
{	int iNb=0,imax=mHpPak_FindStrLenght(lpValue,iSizeValue);
	BOOL bAntiShl=FALSE;
	for(int i=0;i<imax;i++)
	{	if((lpValue[i]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else
		{	if((lpValue[i]==ITEM_GROUP)&&(!bAntiShl)) iNb++;
			bAntiShl=FALSE;
		}
	}
	return (iNb+1);
}

//---------------------------------------------------------//
// mHpPak_GetIndiceStr
//---------------------------------------------------------//
MHPPAK_API void mHpPak_GetIndiceStr(int indice,char *StrSource,char *StrDest)
{	BOOL bAntiShl=FALSE;
	int i=0,j=0;
	while(j!=indice)
	{	if((StrSource[i]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else
		{	if((StrSource[i]==ITEM_GROUP)&&(!bAntiShl)) j++;
			bAntiShl=FALSE;
		}
		i++;
	}
	j=0;
	while((StrSource[i]!=END_LIST)&&((StrSource[i]!=ITEM_GROUP)||(bAntiShl)))
	{	if((StrSource[i]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else
		{	StrDest[j]=StrSource[i];
			j++;
			bAntiShl=FALSE;
		}
		i++;
	}
}

//----------------------------------------------------------------//
// mHpPak_GetIndexFromSymbolPos
//----------------------------------------------------------------//
MHPPAK_API int mHpPak_GetIndexFromSymbolPos(int Rang,char* lpStr,char Symbol)
{	int iRes=0,iPos=0;
	while((iPos!=Rang)&&(lpStr[iRes]!=END_LIST))
	{	if(lpStr[iRes]==Symbol) iPos++;
		iRes++;
	}
	if((iPos==Rang)&&(lpStr[iRes]!=END_LIST)) return iRes;
	return NO;
}

//-----------------------------------------------------//
// mHpPak_IsMenuItemReady
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_IsMenuItemReady(HMENU phMenu,int pPos,UINT ItemCmd)
{	MENUITEMINFO ItemInfo;
	ItemInfo.fMask=MIIM_STATE;
	ItemInfo.cbSize=sizeof(ItemInfo);
	if(pPos!=NO)
	{	if(!GetMenuItemInfo(phMenu,pPos,TRUE,&ItemInfo)) return FALSE;}
	else
    {	if(!GetMenuItemInfo(phMenu,ItemCmd,FALSE,&ItemInfo)) return FALSE;}
    if(ItemInfo.fState&MFS_DISABLED)
	{	return FALSE;}
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_GetStrStateType
//-----------------------------------------------------//
MHPPAK_API char* mHpPak_GetStrStateType(int iType,char* lpStrType,int iStrSize)
{	mHpPak_InitStr(lpStrType,iStrSize);
	LoadString(hmHpPakInst,TypeTab[iType],lpStrType,iStrSize);
	return lpStrType;
}

//-----------------------------------------------------//
// mHpPak_GetTopicFromRef
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetTopicFromRef(char* lpRef,int iIndex)
{	int indice=0,iCnt=0,iRes=0;
	BOOL bFind=FALSE;
	while(lpRef[indice]!=END_LIST)
	{	if(!bFind)
		{	if((lpRef[indice]==TREE_PARAM_GROUP)||(lpRef[indice]==NEXT_RS))
			{	if(iCnt==iIndex) bFind=TRUE;
				iCnt++;
			}
		}
		else
		{	iRes+=mHpPak_CharToInt(lpRef[indice]);
			if((lpRef[(indice+1)]!=END_LIST)&&
			   (lpRef[(indice+1)]!=TREE_PARAM_GROUP)&&
			   (lpRef[(indice+1)]!=NEXT_RS)) iRes*=10;
			else return iRes;
		}
		indice++;
	}
	return NO;
}

//-----------------------------------------------------//
// mHpPak_ViewMsg
//-----------------------------------------------------//
MHPPAK_API void mHpPak_ViewMsg(BOOL bViewFlag)
{	SetViewFlag(bViewFlag);}

//-----------------------------------------------------//
// mHpPak_IncreaseProgressBar
//-----------------------------------------------------//
MHPPAK_API void mHpPak_IncreaseProgressBar(HWND hWndProgCtrl,int iCntProgress)
{	PBRANGE Range;
	UINT iPos;
	SendMessage(hWndProgCtrl,PBM_GETRANGE,FALSE,(LPARAM)&Range);
	if(Range.iHigh!=iCntProgress)
	{	SendMessage(hWndProgCtrl,PBM_SETRANGE,0,MAKELPARAM(0,iCntProgress));
		SendMessage(hWndProgCtrl,PBM_SETPOS,0,0);
	}
	iPos=(UINT)SendMessage(hWndProgCtrl,PBM_GETPOS,0,0);
	if(((int)iPos)!=iCntProgress) SendMessage(hWndProgCtrl,PBM_DELTAPOS,1,0);
}

//-----------------------------------------------------//
// mHpPak_ToolTipsActivate
//-----------------------------------------------------//
MHPPAK_API void  mHpPak_ToolTipsActivate(BOOL bActive)
{	HWND hWndToolTips=FindWindowEx(NULL,NULL,TOOLTIPS_CLASS_VB,NULL_STRING);
	while(hWndToolTips!=NULL)
	{	SendMessage(hWndToolTips,TTM_ACTIVATE,(BOOL)bActive,0);
		hWndToolTips=FindWindowEx(NULL,hWndToolTips,TOOLTIPS_CLASS_VB,NULL_STRING);
	}
	hWndToolTips=FindWindowEx(NULL,NULL,TOOLTIPS_CLASS_A,NULL_STRING);
	while(hWndToolTips!=NULL)
	{	SendMessage(hWndToolTips,TTM_ACTIVATE,(BOOL)bActive,0);
		hWndToolTips=FindWindowEx(NULL,hWndToolTips,TOOLTIPS_CLASS_A,NULL_STRING);
	}
	hWndToolTips=FindWindowEx(NULL,NULL,TOOLTIPS_CLASS_B,NULL_STRING);
	while(hWndToolTips!=NULL)
	{	SendMessage(hWndToolTips,TTM_ACTIVATE,(BOOL)bActive,0);
		hWndToolTips=FindWindowEx(NULL,hWndToolTips,TOOLTIPS_CLASS_B,NULL_STRING);
	}
}

//-----------------------------------------------------//
// mHpPak_OpenmHelpDoc
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_OpenmHelpDoc(HWND hWndActive,UINT iViewPageID)
{	char lpDocFile[MAX_PATH]={0};
	char lpRC[MAX_PATH]={0};
	HWND hWndHh;
	BOOL bRes=FALSE;
	GetWindowsDirectory(lpDocFile,MAX_PATH);
	strncat(lpDocFile,MHELP_DOCFILE,MAX_PATH);
	// Check existing documentation
	if(GetFileAttributes(lpDocFile)!=0xFFFFFFFF)
	{	strncat(lpDocFile,"::/",MAX_PATH);
		strncat(lpDocFile,szmHelpDocPage[iViewPageID],MAX_PATH);
		strncat(lpDocFile,HTML_EXTENSION,MAX_PATH);
		hWndHh=HtmlHelp(GetDesktopWindow(),
				lpDocFile,
				HH_DISPLAY_TOPIC,
				NULL);
	}
	else
	{	LoadString(hmHpPakInst,IDS_DOC_NOTFOUND,lpRC,MAX_PATH);
		strncat(lpRC,lpDocFile,MAX_PATH);
		MessageBox(hWndActive,lpRC,MHPPAK_DLGMSGTITLE,MB_ICONEXCLAMATION);
	}
	return bRes;
}

//**************************************************************************************************************KEYREPORT//
// mHpPak_WriteKey
//-----------------------------------------------------//
MHPPAK_API void mHpPak_WriteKey(const char* lpReport)
{	int i=1000;
	char lpTmp[100]={0};
	char lp[MAX_KEY_SIZE]={0};
	SECURITY_ATTRIBUTES lpSecuAtt;
	HKEY lpNewKey,lpmHpKey;
	DWORD DWord=REG_OPENED_EXISTING_KEY;
	BYTE ValByte[MAX_KEY_SIZE]={0};
	int size;
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_SZ;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecuAtt,bAclRes);
	strncpy(lp,lpReport,MAX_KEY_SIZE);
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&lpmHpKey)!=ERROR_SUCCESS) return;
	// mHelp	
	if(RegOpenKeyEx(lpmHpKey,MHELP_KEY,0,KEY_READ,&lpmHpKey)!=ERROR_SUCCESS) return;
	_snprintf(lpTmp,100,"%d",i);
	while(RegQueryValueEx(lpmHpKey,lpTmp,NULL,&dwType,ValByte,&iSize)==ERROR_SUCCESS)
	{	iSize=MAX_KEY_SIZE;
		dwType=REG_SZ;
		i++;
		_snprintf(lpTmp,100,"%d",i);
	}
	RegCloseKey(lpmHpKey);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpmHpKey,&DWord)!=ERROR_SUCCESS) return;
	// mHelp
	if(RegCreateKeyEx(lpmHpKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpNewKey,&DWord)!=ERROR_SUCCESS) return;
	_snprintf(lpTmp,100,"%d",i);
	if(RegCreateKeyEx(lpmHpKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpNewKey,&DWord)!=ERROR_SUCCESS) return;
	size=mHpPak_FillDataFromString(ValByte,lp);
	RegSetValueEx(lpNewKey,lpTmp,0,REG_SZ,ValByte,size);
	RegCloseKey(lpNewKey);
}

//***************************************************************************************************************STRTOOLS//
// mHpPak_InitStr
//-----------------------------------------------------//
MHPPAK_API void mHpPak_InitStr(char *Tab,int limit)
{	for(int i=0;i<limit;i++) Tab[i]=END_LIST;}

//-----------------------------------------------------//
// mHpPak_FindStrLenght
//-----------------------------------------------------//
MHPPAK_API int mHpPak_FindStrLenght(char *lpStr,int iMaxStrSize)
{	int iLenght=0;
	while((lpStr[iLenght]!=END_LIST)&&(iLenght!=iMaxStrSize)) iLenght++;
	return iLenght;
}

//-----------------------------------------------------//
// mHpPak_ReadString
//-----------------------------------------------------//
MHPPAK_API void mHpPak_ReadString(char* lpDest,int DestSize,char* lpRead)
{	int indice=0,iDest=0;
	BOOL bShlass=FALSE;
	mHpPak_InitStr(lpDest,DestSize);
	while(lpRead[indice]!=END_LIST)
	{	if(bShlass)
		{	lpDest[iDest]=lpRead[indice];
			iDest++;
			bShlass=FALSE;
		}
		else
		{	if(lpRead[indice]==ANTI_SHLASS) bShlass=TRUE;
			else
			{	if(lpRead[indice]==NEXT_RS)
				{	lpDest[iDest]=END_RS_TASK;
					iDest++;
				}
				else
				{	lpDest[iDest]=lpRead[indice];
					iDest++;
				}
			}
		}
		indice++;
	}
}

//-----------------------------------------------------//
// mHpPak_WriteString
//-----------------------------------------------------//
MHPPAK_API void mHpPak_WriteString(char* lpDest,int DestSize,const char* lpWrite)
{	int indice=0,iDest=0;
	BOOL bEnter=FALSE;
	mHpPak_InitStr(lpDest,DestSize);
	while(lpWrite[indice]!=END_LIST)
	{	switch(lpWrite[indice])
		{	case ITEM_GROUP: // '^'
			case TREE_PARAM_GROUP: // '¤'
			case NEXT_RS: // '|'
			case RS_PARAM: // '§'
			case NEXT_TASK: // '_'
			case START_TASK: // '#'
			case END_FILE: // '¨'
			case START_CTRL: // '@'
			case ANTI_SHLASS: // '\\'
			{	lpDest[iDest]=ANTI_SHLASS;
				iDest++;
				lpDest[iDest]=lpWrite[indice];
				iDest++;
				break;
			}
			case END_RS_TASK: // '\n'
			{	lpDest[iDest]=NEXT_RS;
				iDest++;
				bEnter=TRUE;
			}
			default:
			{	if(!bEnter)
				{	lpDest[iDest]=lpWrite[indice];
					iDest++;
				}
				else bEnter=FALSE;
				break;
			}
		}
		indice++;
	}
}

//-----------------------------------------------------//
// mHpPak_DecodeString
//-----------------------------------------------------//
void mHpPak_DecodeString(char* lpVal,UINT nMaxSize,const char* lpStrArray,int &nResIdx)
{   // Return next string from encoded string array
    // e.g. "Lis\^tSel1^Li_s\\tSel2^ListSe¤l\^3" -> "Lis^tSel1" - "Li_s\tSel2" - "ListSe¤l^3"
    UINT nVal=0;
    BOOL bAntiShl=FALSE;
    mHpPak_InitStr(lpVal,(int)nMaxSize);
    while((nVal<nMaxSize)&&(lpStrArray[nResIdx]))
    {   if((lpStrArray[nResIdx]==ANTI_SHLASS)&&(!bAntiShl)) // '\\'
            bAntiShl=TRUE;
        else
        {   if(!bAntiShl)
            {   if(lpStrArray[nResIdx]==ITEM_GROUP) // '^'
                {   nResIdx++; // Next string exists
                    return;
                }
            }
            else
                bAntiShl=FALSE;
            lpVal[nVal++]=lpStrArray[nResIdx];
        }
        nResIdx++;
    }
    nResIdx=NO; // No more string
}

//*****************************************************************************************************************MHPNET//
// mHpPak_IsDotNetHwnd
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_IsDotNetHwnd(HWND hWndActive)
{	if(!hmHpDotNetInst)
    {   char lpDotNetClass[MAX_KEY_SIZE]={0};
	    GetClassName(hWndActive,lpDotNetClass,MAX_KEY_SIZE);
	    if(!strncmp(lpDotNetClass,MHPPAK_DOTNET_CLASS_NAME,sizeof(MHPPAK_DOTNET_CLASS_NAME)-1))
	    {	hmHpDotNetInst=LoadCrossPlatfromLibrary(hWndActive,DOTNET_CROSS_DLL_NAME);
            return (!hmHpDotNetInst)? FALSE:TRUE;
	    }
	    return FALSE;
    }
	return TRUE; // If hmHpDotNetInst is already defined that means that mHpPak_IsDotNetHwnd has ever succeeded
                 // or mHpPak_AddDotNetCtrlEvent has already been called (not sure hWndActive is not a control)
}

//-----------------------------------------------------//
// mHpPak_AddDotNetCtrlEvent
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_AddDotNetCtrlEvent(HWND hWndActive,HWND hWndCtrl)
{	if(!hmHpDotNetInst)
	{	hmHpDotNetInst=LoadCrossPlatfromLibrary(hWndActive,DOTNET_CROSS_DLL_NAME);
        if(!hmHpDotNetInst)
            return FALSE;
	}
	if(!DotNetAddCtrlEvent)
	{	DotNetAddCtrlEvent=(DOTNETACEPROC)GetProcAddress(hmHpDotNetInst,DOTNET_FUNC_ADDCTRLEVENT);
		if(!DotNetAddCtrlEvent)
		{	// Error: Failed to get mHpNET_AddCtrlEvent() function from CrossNET library
		    mHpMessage(hWndActive,hmHpPakInst,IDS_GET_PROCLIB1,"mHpNET_AddCtrlEvent()",MAX_KEY_SIZE,IDS_GET_PROCLIB2,MB_ICONSTOP);
			return FALSE;
		}
	}
	return DotNetAddCtrlEvent(hWndActive,hWndCtrl);
}

//-----------------------------------------------------//
// mHpPak_GetDotNetCtrlInfo
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_GetDotNetCtrlInfo(HWND hWndActive,HWND hWndCtrl,int& imHpID,char* lpCtrlName,UINT nSizeName)
{	if(!hmHpDotNetInst)
	{	hmHpDotNetInst=LoadCrossPlatfromLibrary(hWndActive,DOTNET_CROSS_DLL_NAME);
        if(!hmHpDotNetInst)
            return FALSE;
	}
	if(!DotNetGetCtrlInfo)
	{	DotNetGetCtrlInfo=(DOTNETGCIPROC)GetProcAddress(hmHpDotNetInst,DOTNET_FUNC_GETCTRLINFO);
		if(!DotNetGetCtrlInfo)
		{	// Error: Failed to get mHpNET_GetCtrlInfo() function from CrossNET library
		    mHpMessage(hWndActive,hmHpPakInst,IDS_GET_PROCLIB1,"mHpNET_GetCtrlInfo()",MAX_KEY_SIZE,IDS_GET_PROCLIB2,MB_ICONSTOP);
			return FALSE;
		}
	}
	return DotNetGetCtrlInfo(hWndActive,hWndCtrl,imHpID,lpCtrlName,nSizeName);
}

//-----------------------------------------------------//
// mHpPak_GetDotNetCtrlHandle
//-----------------------------------------------------//
MHPPAK_API HWND mHpPak_GetDotNetCtrlHandle(HWND hWndForm,char* lpCtrlName,int& iStyle,int& iState)
{	if(!hmHpDotNetInst)
	{	hmHpDotNetInst=LoadCrossPlatfromLibrary(hWndForm,DOTNET_CROSS_DLL_NAME);
        if(!hmHpDotNetInst)
            return FALSE;
	}
	if(!DotNetGetCtrlHandle)
	{	DotNetGetCtrlHandle=(DOTNETGCHPROC)GetProcAddress(hmHpDotNetInst,DOTNET_FUNC_GETCTRLHANDLE);
		if(!DotNetGetCtrlHandle)
		{	// Error: Failed to get mHpNET_GetCtrlHandle() function from CrossNET library
		    mHpMessage(hWndForm,hmHpPakInst,IDS_GET_PROCLIB1,"mHpNET_GetCtrlHandle()",MAX_KEY_SIZE,IDS_GET_PROCLIB2,MB_ICONSTOP);
			return FALSE;
		}
	}
	return DotNetGetCtrlHandle(hWndForm,lpCtrlName,iStyle,iState);
}

//******************************************************************************************************************MHPQT//
// mHpPak_IsQtHwnd
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_IsQtHwnd(HWND hWndActive)
{	if(!hmHpQtInst)
    {   char lpQtClass[MAX_KEY_SIZE]={0};
	    GetClassName(hWndActive,lpQtClass,MAX_KEY_SIZE);
	    if(!strncmp(lpQtClass,MHPPAK_QT_CLASS_NAME,sizeof(MHPPAK_QT_CLASS_NAME)))
	    {	hmHpQtInst=LoadCrossPlatfromLibrary(hWndActive,QT_CROSS_DLL_NAME);
            return (!hmHpQtInst)? FALSE:TRUE;
	    }
	    return FALSE;
    }
	return TRUE; // If hmHpQtInst is already defined that means that mHpPak_IsQtHwnd has ever succeeded (not
                 // sure that current hWndActive has Qt class name MHPPAK_QT_CLASS_NAME)
}

//-----------------------------------------------------//
// mHpPak_AddQtCtrlsEvent
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_AddQtCtrlsEvent(HWND hWndActive)
{	if(!hmHpQtInst)
	{	hmHpQtInst=LoadCrossPlatfromLibrary(hWndActive,QT_CROSS_DLL_NAME);
        if(!hmHpQtInst)
            return FALSE;
	}
	if(!QtAddCtrlsEvent)
	{	QtAddCtrlsEvent=(QTACEPROC)GetProcAddress(hmHpQtInst,QT_FUNC_ADDCTRLSEVENT);
		if(!QtAddCtrlsEvent)
		{	// Error: Failed to get mHpQt_AddCtrlsEvent() function from CrossQt library
		    mHpMessage(hWndActive,hmHpPakInst,IDS_GET_PROCLIB1,"mHpQt_AddCtrlsEvent()",MAX_KEY_SIZE,IDS_GET_PROCLIB2,MB_ICONSTOP);
			return FALSE;
		}
	}
	return QtAddCtrlsEvent(hWndActive);
}

//-----------------------------------------------------//
// mHpPak_GetQtCtrlInfo
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_GetQtCtrlInfo(HWND hWndCtrl,int& imHpID,char* lpCtrlName,UINT nSizeName)
{	if(!hmHpQtInst)
	{	hmHpQtInst=LoadCrossPlatfromLibrary(hWndCtrl,QT_CROSS_DLL_NAME);
        if(!hmHpQtInst)
            return FALSE;
	}
	if(!QtGetCtrlInfo)
	{	QtGetCtrlInfo=(QTGCIPROC)GetProcAddress(hmHpQtInst,QT_FUNC_GETCTRLINFO);
		if(!QtGetCtrlInfo)
		{	// Error: Failed to get mHpQt_GetCtrlInfo() function from CrossQt library
		    mHpMessage(hWndCtrl,hmHpPakInst,IDS_GET_PROCLIB1,"mHpQt_GetCtrlInfo()",MAX_KEY_SIZE,IDS_GET_PROCLIB2,MB_ICONSTOP);
			return FALSE;
		}
	}
	return QtGetCtrlInfo(hWndCtrl,imHpID,lpCtrlName,nSizeName);
}

//-----------------------------------------------------//
// mHpPak_GetQtCtrlHandle
//-----------------------------------------------------//
MHPPAK_API HWND mHpPak_GetQtCtrlHandle(HWND hWndActive,char* lpCtrlName,int imHpID)
{	if(!hmHpQtInst)
	{	hmHpQtInst=LoadCrossPlatfromLibrary(hWndActive,QT_CROSS_DLL_NAME);
        if(!hmHpQtInst)
            return FALSE;
	}
	if(!QtGetCtrlHandle)
	{	QtGetCtrlHandle=(QTGCHPROC)GetProcAddress(hmHpQtInst,QT_FUNC_GETCTRLHANDLE);
		if(!QtGetCtrlHandle)
		{	// Error: Failed to get mHpQt_GetCtrlHandle() function from CrossQt library
		    mHpMessage(hWndActive,hmHpPakInst,IDS_GET_PROCLIB1,"mHpQt_GetCtrlHandle()",MAX_KEY_SIZE,IDS_GET_PROCLIB2,MB_ICONSTOP);
			return FALSE;
		}
	}
	return QtGetCtrlHandle(hWndActive,lpCtrlName,imHpID);
}

//************************************************************************************************************INTTABTOOLS//
// mHpPak_EmptyTab
//-----------------------------------------------------//
MHPPAK_API void mHpPak_EmptyTab(int *Tab,int TabSize)
{	for(int i=0;i<TabSize;i++) Tab[i]=NULL;
	Tab[0]=NO;
}

//-----------------------------------------------------//
// mHpPak_AddIndex
//-----------------------------------------------------//
MHPPAK_API void mHpPak_AddIndex(int *Tab,int IndexNew)
{	int indice=0;
	while(Tab[indice]!=NO) indice++;
	Tab[indice]=IndexNew;
	Tab[(indice+1)]=NO;
}

//-----------------------------------------------------//
// mHpPak_GetIndex
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetIndex(int *Tab)
{	int indice=0;
	while(Tab[indice]!=NO) indice++;
	if(indice!=0) return (Tab[(indice-1)]);
	return NO;
}

//-----------------------------------------------------//
// mHpPak_GetCountIndex
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetCountIndex(int *Tab)
{	int iResult=0;
	int indice=0;
	while(Tab[indice]!=NO) 
	{	indice++;
		iResult++;
	}
	return iResult;
}

//-----------------------------------------------------//
// mHpPak_CompareTab
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_CompareTab(int *Tab,int *Comp)
{	int indice=0;
	while((Tab[indice]!=NO)&&(Comp[indice]!=NO))
	{	if(Tab[indice]==Comp[indice]) indice++;
		else return FALSE;
	}
	if((Tab[indice]==NO)&&(Comp[indice]==NO)) return TRUE;
	else return FALSE;
}

//-----------------------------------------------------//
// mHpPak_CopyTab
//-----------------------------------------------------//
MHPPAK_API void mHpPak_CopyTab(int *DestTab,int iDestSize,int *SrcTab)
{	int indice=0;
	mHpPak_EmptyTab(DestTab,iDestSize);
	while(SrcTab[indice]!=NO)
	{	DestTab[indice]=SrcTab[indice];
		indice++;
	}
	DestTab[indice]=NO;
}

//*****************************************************************************************************************MHPKEY//
// mHpPak_DeleteSubKey
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_DeleteSubKey(HKEY hRemoveKey,char* lpRemoveName)
{	SECURITY_ATTRIBUTES lpSecuAtt;
	char lpSubKey[MAX_KEY_SIZE]={0};
	DWORD iSize=MAX_KEY_SIZE;
	DWORD DWord=REG_OPENED_EXISTING_KEY;
	FILETIME lpWriteTime;
	HKEY hSubKey;
	BOOL bRes;
	mHpPak_GetSecurityAtt(lpSecuAtt,bRes);
	if(RegCreateKeyEx(hRemoveKey,lpRemoveName,0,NULL,REG_OPTION_NON_VOLATILE,
				KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
				&hSubKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	while(RegEnumKeyEx(hSubKey,0,lpSubKey,&iSize,NULL,NULL,NULL,&lpWriteTime)!=ERROR_NO_MORE_ITEMS)
	{	mHpPak_DeleteSubKey(hSubKey,lpSubKey);
		mHpPak_InitStr(lpSubKey,MAX_KEY_SIZE);
		iSize=MAX_KEY_SIZE;
	}
	RegDeleteKey(hRemoveKey,lpRemoveName);
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_GetSecurityAtt
//-----------------------------------------------------//
MHPPAK_API void mHpPak_GetSecurityAtt(SECURITY_ATTRIBUTES &lpEveryoneSecAtt,BOOL &bResult)
{	EXPLICIT_ACCESS ea;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld=SECURITY_WORLD_SID_AUTHORITY;
	bResult=FALSE;
	if(!bSecStruct)
	{	// Create a well-known SID for the Everyone group.
		if(AllocateAndInitializeSid(&SIDAuthWorld,1,SECURITY_WORLD_RID,0,0,0,0,0,0,0,&pEveryoneSID))
		{	// Initialize an EXPLICIT_ACCESS structure for an ACE.
			// The ACE will allow Everyone all access to the key.
			ZeroMemory(&ea,sizeof(EXPLICIT_ACCESS));
			ea.grfAccessPermissions=KEY_ALL_ACCESS;
			ea.grfAccessMode=SET_ACCESS;
			ea.grfInheritance=NO_INHERITANCE;
			ea.Trustee.TrusteeForm=TRUSTEE_IS_SID;
			ea.Trustee.TrusteeType=TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea.Trustee.ptstrName=(LPTSTR)pEveryoneSID;
			// Create a new ACL that contains the new ACEs.
			if(SetEntriesInAcl(1,&ea,NULL,&pACL)==ERROR_SUCCESS)
			{	// Initialize a security descriptor. & Add the ACL to the security descriptor.
				pSD=(PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,SECURITY_DESCRIPTOR_MIN_LENGTH);
				if((pSD!=NULL)&&(InitializeSecurityDescriptor(pSD,SECURITY_DESCRIPTOR_REVISION))&&
					(SetSecurityDescriptorDacl(pSD,TRUE,pACL,FALSE)))
				{	bResult=TRUE;}
			}
		}
	}
	else bResult=TRUE;
	lpEveryoneSecAtt.nLength=sizeof(SECURITY_ATTRIBUTES);
	lpEveryoneSecAtt.bInheritHandle=TRUE;
	if(bResult)
	{	lpEveryoneSecAtt.lpSecurityDescriptor=pSD;
		bSecStruct=TRUE;
	}
	else
	{	if(pEveryoneSID) FreeSid(pEveryoneSID);
		if(pACL) LocalFree(pACL);
		if(pSD) LocalFree(pSD);
		lpEveryoneSecAtt.lpSecurityDescriptor=NULL;
		bSecStruct=FALSE;
	}
}

//-----------------------------------------------------//
// mHpPak_DeleteSecAttStructs
//-----------------------------------------------------//
MHPPAK_API void mHpPak_DeleteSecAttStructs()
{	if(bSecStruct)
	{	if(pEveryoneSID) FreeSid(pEveryoneSID);
		if(pACL) LocalFree(pACL);
		if(pSD) LocalFree(pSD);
		bSecStruct=FALSE;
	}
}

//-----------------------------------------------------//
// mHpPak_ClearmHpSharedKey
//-----------------------------------------------------//
MHPPAK_API void mHpPak_ClearmHpSharedKey()
{	SECURITY_ATTRIBUTES lpSecuAtt;
	HKEY lpNewKey,lpmHpKey;
	DWORD DWord=REG_OPENED_EXISTING_KEY;
	BYTE TabByte[MAX_KEY_SIZE]={0};
	BOOL bAclRes;
	DWORD dwSize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	int iSize,iProcID=0;
	mHpPak_GetSecurityAtt(lpSecuAtt,bAclRes);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpmHpKey,&DWord)!=ERROR_SUCCESS) return;
	// mHelp
	if(RegCreateKeyEx(lpmHpKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpmHpKey,&DWord)!=ERROR_SUCCESS) return;
	// mHpShared
	if(RegCreateKeyEx(lpmHpKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpNewKey,&DWord)!=ERROR_SUCCESS) return;
	// Get last 'mHpProcID'
	if(RegQueryValueEx(lpNewKey,MHPPROCID_KEY,NULL,&dwType,TabByte,&dwSize)==ERROR_SUCCESS)
	{	mHpPak_FillIntFromData(TabByte,iProcID,FALSE);}
	// mHpShared
	mHpPak_DeleteSubKey(lpmHpKey,MHPSHARED_KEY);
	if(RegCreateKeyEx(lpmHpKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpNewKey,&DWord)!=ERROR_SUCCESS) return;
	// mHpProcess
	if(RegCreateKeyEx(lpmHpKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpNewKey,&DWord)!=ERROR_SUCCESS) return;
	iSize=mHpPak_FillDataFromNumber(TabByte,0,FALSE);
	RegSetValueEx(lpNewKey,READWRITE_KEY,0,REG_DWORD,TabByte,iSize);
	RegCloseKey(lpNewKey);
	// mHpProcID
	if(RegCreateKeyEx(lpmHpKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpNewKey,&DWord)!=ERROR_SUCCESS) return;
	iSize=mHpPak_FillDataFromNumber(TabByte,iProcID,FALSE);	
	RegSetValueEx(lpNewKey,MHPPROCID_KEY,0,REG_DWORD,TabByte,iSize);
	RegCloseKey(lpNewKey);
}

//-----------------------------------------------------//
// mHpPak_FillIntFromData
//-----------------------------------------------------//
MHPPAK_API void mHpPak_FillIntFromData(BYTE* DataTab,int &iData,BOOL bBin)
{	int iPart=0;
	if(bBin)
	{	iData=DataTab[0];
		return;
	}
	iData=DataTab[0];
	iPart=DataTab[1];
	iPart<<=8;
	iData+=iPart;
	iPart=DataTab[2];
	iPart<<=16;
	iData+=iPart;
	iPart=DataTab[3];
	iPart<<=24;
	iData+=iPart;
}

//-----------------------------------------------------//
// mHpPak_FillStringFromData
//-----------------------------------------------------//
MHPPAK_API void mHpPak_FillStringFromData(char* lpStr,int StrSize,BYTE* ByteTab,int SizeTab)
{	int index=0;
	mHpPak_InitStr(lpStr,StrSize);
	while(index!=SizeTab)
	{	lpStr[index]=(char)ByteTab[index];
		index++;
	}
}

//-----------------------------------------------------//
// mHpPak_FillDataFromString
//-----------------------------------------------------//
MHPPAK_API int mHpPak_FillDataFromString(BYTE* bByteTab,char* lpStrValue)
{	int iRes=0;
	for(int i=0;i<MAX_KEY_SIZE;i++) bByteTab[i]=END_LIST;
	while(lpStrValue[iRes]!=END_LIST)
	{	bByteTab[iRes]=lpStrValue[iRes];
		iRes++;
	}
	return iRes;
}

//-----------------------------------------------------//
// mHpPak_FillDataFromNumber
//-----------------------------------------------------//
MHPPAK_API int mHpPak_FillDataFromNumber(BYTE* bByteTab,int iValue,BOOL bBinVal)
{	int iRes=0,iPart=0;
	for(int i=0;i<MAX_KEY_SIZE;i++) bByteTab[i]=END_LIST;
	while(iRes!=4)
	{	switch(iRes)
		{	case 0:
			{	if(iValue==NO)
				{	bByteTab[iRes]=(int)bBinVal;
					return 1;
				}
				else iPart=iValue&0x000000FF;
				break;
			}
			case 1:
			{	iPart=iValue&0x0000FF00;
				iPart>>=8;
				break;
			}
			case 2:
			{	iPart=iValue&0x00FF0000;
				iPart>>=16;
				break;
			}
			case 3:
			{	iPart=iValue&0xFF000000;
				iPart>>=24;
				break;
			}
		}
		bByteTab[iRes]=iPart;
		iRes++;
	}
	return iRes;
}

//-----------------------------------------------------//
// mHpPak_WriteLongStrRegVal
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_WriteLongStrRegVal(HKEY CurrentKey,const char* lpSubKey,const char* lpValName,const char* lpValue)
{	DWORD DWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES lpSecVal;
	BYTE ValByte[MAX_KEY_SIZE]={0};
	char lpLongName[MAX_KEY_SIZE]={0};
	char lpCnt[MAX_KEY_SIZE]={0};
	char lpShortVal[MAX_KEY_SIZE]={0};
	int size,iLenVal,iVal=0,iCnt=0;
	HKEY lpNewVal;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecVal,bAclRes);
	while(lpValue[iVal]!=END_LIST)
	{	iLenVal=0;
		mHpPak_InitStr(lpShortVal,MAX_KEY_SIZE);
		mHpPak_InitStr(lpCnt,MAX_KEY_SIZE);
		mHpPak_InitStr(lpLongName,MAX_KEY_SIZE);
		_snprintf(lpCnt,MAX_KEY_SIZE,"%d",iCnt);
		strncpy(lpLongName,lpValName,MAX_KEY_SIZE);
		strncat(lpLongName,lpCnt,mHpPak_FindStrLenght(lpCnt,MAX_KEY_SIZE));
		while((iLenVal!=MAX_KEY_SIZE)&&(lpValue[iVal]!=END_LIST))
		{	lpShortVal[iLenVal]=lpValue[iVal];
			iVal++;
			iLenVal++;
		}
		if(RegCreateKeyEx(CurrentKey,lpSubKey,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecVal,
						&lpNewVal,&DWord)!=ERROR_SUCCESS) return FALSE;
		size=mHpPak_FillDataFromString(ValByte,lpShortVal);
		RegSetValueEx(lpNewVal,lpLongName,0,REG_SZ,ValByte,size);
		RegCloseKey(lpNewVal);
		iCnt++;
	}
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_ReadLongStrRegVal
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_ReadLongStrRegVal(HKEY ParentKey,const char* lpValName,char* lpValKey,int ValSize)
{	char lpTmp[MAX_RS_SIZE]={0};
	char lpCount[MAX_KEY_SIZE]={0};
	char lpNameKey[MAX_KEY_SIZE]={0};
	BYTE szBTab[MAX_KEY_SIZE]={0};
	int iCount=0;
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_SZ;
	mHpPak_InitStr(lpValKey,ValSize);
	strncpy(lpNameKey,lpValName,MAX_KEY_SIZE);
	_snprintf(lpCount,MAX_KEY_SIZE,"%d",iCount);
	strncat(lpNameKey,lpCount,mHpPak_FindStrLenght(lpCount,MAX_KEY_SIZE));
	while(RegQueryValueEx(ParentKey,lpNameKey,NULL,&dwType,szBTab,&iSize)==ERROR_SUCCESS)
	{	if(!iCount) mHpPak_FillStringFromData(lpValKey,ValSize,szBTab,iSize);
		else
		{	mHpPak_FillStringFromData(lpTmp,MAX_RS_SIZE,szBTab,iSize);
			strncat(lpValKey,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
		}
		iCount++;
		mHpPak_InitStr(lpNameKey,MAX_KEY_SIZE);
		mHpPak_InitStr(lpCount,MAX_KEY_SIZE);
		strncpy(lpNameKey,lpValName,MAX_KEY_SIZE);
		_snprintf(lpCount,MAX_KEY_SIZE,"%d",iCount);
		strncat(lpNameKey,lpCount,mHpPak_FindStrLenght(lpCount,MAX_KEY_SIZE));
		iSize=MAX_KEY_SIZE;
	}
	return ((BOOL)iCount);
}

//-----------------------------------------------------//
// mHpPak_ReadOperationKey
//-----------------------------------------------------//
MHPPAK_API int mHpPak_ReadOperationKey()
{	int iOpRes=0;
	BYTE lpBt[MAX_KEY_SIZE]={0};
	HKEY mHpKey;
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&mHpKey)==ERROR_SUCCESS)
	{	// mHelp	
		if(RegOpenKeyEx(mHpKey,MHELP_KEY,0,KEY_READ,&mHpKey)==ERROR_SUCCESS)
		{	// mHpShared
			if(RegOpenKeyEx(mHpKey,MHPSHARED_KEY,0,KEY_READ,&mHpKey)==ERROR_SUCCESS)
			{	// NoReadWrite
				if(RegQueryValueEx(mHpKey,READWRITE_KEY,NULL,&dwType,lpBt,&iSize)==ERROR_SUCCESS)
				{	mHpPak_FillIntFromData(lpBt,iOpRes,FALSE);
				}
			}
		}
	}
	return iOpRes;
}

//-----------------------------------------------------//
// mHpPak_ReadSettingsKey
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_ReadSettingsKey(int &mHpMode,int &mHpRepeat,int &iCurLoop,int &mHpSpeed,int &mHpDelay)
{	int indice=0;
	BYTE lpBt[MAX_KEY_SIZE]={0};
	HKEY SettingsKey;
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&SettingsKey)!=ERROR_SUCCESS) return FALSE;
	// mHelp	
	if(RegOpenKeyEx(SettingsKey,MHELP_KEY,0,KEY_READ,&SettingsKey)!=ERROR_SUCCESS) return FALSE;
	// mHpShared
	if(RegOpenKeyEx(SettingsKey,MHPSHARED_KEY,0,KEY_READ,&SettingsKey)!=ERROR_SUCCESS) return FALSE;
	// Settings
	if(RegOpenKeyEx(SettingsKey,SETTINGS_KEY,0,KEY_READ,&SettingsKey)!=ERROR_SUCCESS) return FALSE;
	// iMode
	if(RegQueryValueEx(SettingsKey,MHPMODE_KEY,NULL,&dwType,lpBt,&iSize)!=ERROR_SUCCESS) return FALSE;
	mHpPak_FillIntFromData(lpBt,mHpMode,FALSE);
	iSize=MAX_KEY_SIZE;dwType=REG_DWORD;
	// iLoopOption
	if(RegQueryValueEx(SettingsKey,LOOPOPTION_KEY,NULL,&dwType,lpBt,&iSize)!=ERROR_SUCCESS) return FALSE;
	mHpPak_FillIntFromData(lpBt,mHpRepeat,FALSE);
	iSize=MAX_KEY_SIZE;dwType=REG_DWORD;
	// iCurLoop
	if(RegQueryValueEx(SettingsKey,CURLOOP_KEY,NULL,&dwType,lpBt,&iSize)!=ERROR_SUCCESS) return FALSE;
	mHpPak_FillIntFromData(lpBt,iCurLoop,FALSE);
	iSize=MAX_KEY_SIZE;dwType=REG_DWORD;
	if((!mHpMode)||(mHpMode==2))
	{	// iMousSpeed
		if(RegQueryValueEx(SettingsKey,MOUSESPEED_KEY,NULL,&dwType,lpBt,&iSize)!=ERROR_SUCCESS) return FALSE;
		mHpPak_FillIntFromData(lpBt,mHpSpeed,FALSE);
		iSize=MAX_KEY_SIZE;dwType=REG_DWORD;
	}
	// iLoopDelay
	if(RegQueryValueEx(SettingsKey,LOOP_DELAY_KEY,NULL,&dwType,lpBt,&iSize)!=ERROR_SUCCESS) return FALSE;
	mHpPak_FillIntFromData(lpBt,mHpDelay,FALSE);
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_WriteSettingsKey
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_WriteSettingsKey(int mHpMode,int mHpRepeat,int iCurLoop,int mHpSpeed,int mHpDelay)
{	SECURITY_ATTRIBUTES lpSecuAtt;
	HKEY lpTmpKey,lpSettingKey;
	DWORD DWord=REG_OPENED_EXISTING_KEY;
	BYTE TabByte[MAX_KEY_SIZE]={0};
	int iSize;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecuAtt,bAclRes);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpSettingKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHelp
	if(RegCreateKeyEx(lpSettingKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpSettingKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHpShared
	if(RegCreateKeyEx(lpSettingKey,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpSettingKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// Settings
	if(RegCreateKeyEx(lpSettingKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// iMode
	if(RegCreateKeyEx(lpSettingKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	iSize=mHpPak_FillDataFromNumber(TabByte,mHpMode,FALSE);
	RegSetValueEx(lpTmpKey,MHPMODE_KEY,0,REG_DWORD,TabByte,iSize);
	RegCloseKey(lpTmpKey);
	// iLoopOption
	if(RegCreateKeyEx(lpSettingKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	iSize=mHpPak_FillDataFromNumber(TabByte,mHpRepeat,FALSE);
	RegSetValueEx(lpTmpKey,LOOPOPTION_KEY,0,REG_DWORD,TabByte,iSize);
	RegCloseKey(lpTmpKey);
	// iCurLoop
	if(RegCreateKeyEx(lpSettingKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	iSize=mHpPak_FillDataFromNumber(TabByte,iCurLoop,FALSE);
	RegSetValueEx(lpTmpKey,CURLOOP_KEY,0,REG_DWORD,TabByte,iSize);
	RegCloseKey(lpTmpKey);
	if((!mHpMode)||(mHpMode==2))
	{	// iMousSpeed
		if(RegCreateKeyEx(lpSettingKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
						KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
						&lpTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
		iSize=mHpPak_FillDataFromNumber(TabByte,mHpSpeed,FALSE);
		RegSetValueEx(lpTmpKey,MOUSESPEED_KEY,0,REG_DWORD,TabByte,iSize);
		RegCloseKey(lpTmpKey);
	}
	// iLoopDelay
	if(RegCreateKeyEx(lpSettingKey,SETTINGS_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	iSize=mHpPak_FillDataFromNumber(TabByte,mHpDelay,FALSE);
	RegSetValueEx(lpTmpKey,LOOP_DELAY_KEY,0,REG_DWORD,TabByte,iSize);
	RegCloseKey(lpTmpKey);
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_ReadExePathKey
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_ReadExePathKey(char* lpExePath,int ExeSize,char* lpExeName)
{	BYTE lpBt[MAX_KEY_SIZE]={0};
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_SZ;
	HKEY hAppKey;
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&hAppKey)!=ERROR_SUCCESS) return FALSE;
	// mHelp	
	if(RegOpenKeyEx(hAppKey,MHELP_KEY,0,KEY_READ,&hAppKey)!=ERROR_SUCCESS) return FALSE;
	// mHpApp
	if(RegOpenKeyEx(hAppKey,MHPAPP_KEY,0,KEY_READ,&hAppKey)!=ERROR_SUCCESS) return FALSE;
	// ExeName
	if(RegOpenKeyEx(hAppKey,lpExeName,0,KEY_READ,&hAppKey)!=ERROR_SUCCESS) return FALSE;
	// ExePath
	if(RegQueryValueEx(hAppKey,EXEPATH_KEY,NULL,&dwType,lpBt,&iSize)!=ERROR_SUCCESS) return FALSE;
	mHpPak_FillStringFromData(lpExePath,ExeSize,lpBt,(int)iSize);
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_WriteExePathKey
//-----------------------------------------------------//
MHPPAK_API void mHpPak_WriteExePathKey(char* ExeName)
{	SECURITY_ATTRIBUTES lpSecu;
	HKEY lpNewKey,lpAppKey,lpmHpKey;
	DWORD dWord=REG_OPENED_EXISTING_KEY;
	char lpmHpPath[MAX_KEY_SIZE]={0};
	BYTE ByteTab[MAX_KEY_SIZE]={0};
	int iSize;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecu,bAclRes);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpmHpKey,&dWord)!=ERROR_SUCCESS) return;
	// mHelp
	if(RegCreateKeyEx(lpmHpKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpmHpKey,&dWord)!=ERROR_SUCCESS) return;
	// mHpApp
	if(RegCreateKeyEx(lpmHpKey,MHPAPP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpmHpKey,&dWord)!=ERROR_SUCCESS) return;
	// AppName
	if(RegCreateKeyEx(lpmHpKey,ExeName,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpAppKey,&dWord)!=ERROR_SUCCESS) return;
	// ExePath
	if(RegCreateKeyEx(lpmHpKey,ExeName,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&lpNewKey,&dWord)!=ERROR_SUCCESS) return;
	GetCurrentDirectory(MAX_KEY_SIZE,lpmHpPath);
	iSize=mHpPak_FillDataFromString(ByteTab,lpmHpPath);
	RegSetValueEx(lpNewKey,EXEPATH_KEY,0,REG_SZ,ByteTab,iSize);
}

//-----------------------------------------------------//
// mHpPak_ReadmHpProcIDKey
//-----------------------------------------------------//
MHPPAK_API int mHpPak_ReadmHpProcIDKey()
{	int iProcID=0;
	BYTE lpBt[MAX_KEY_SIZE]={0};
	HKEY mHpKey;
	DWORD iSize=MAX_KEY_SIZE;
	DWORD dwType=REG_DWORD;
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&mHpKey)==ERROR_SUCCESS)
	{	// mHelp	
		if(RegOpenKeyEx(mHpKey,MHELP_KEY,0,KEY_READ,&mHpKey)==ERROR_SUCCESS)
		{	// mHpShared
			if(RegOpenKeyEx(mHpKey,MHPSHARED_KEY,0,KEY_READ,&mHpKey)==ERROR_SUCCESS)
			{	// mHpProcID
				if(RegQueryValueEx(mHpKey,MHPPROCID_KEY,NULL,&dwType,lpBt,&iSize)==ERROR_SUCCESS)
				{	mHpPak_FillIntFromData(lpBt,iProcID,FALSE);
				}
			}
		}
	}
	return iProcID;
}

//-----------------------------------------------------//
// mHpPak_WritemHpProcIDKey
//-----------------------------------------------------//
MHPPAK_API void mHpPak_WritemHpProcIDKey(int iProcNewID)
{	DWORD dWord=REG_OPENED_EXISTING_KEY;
	SECURITY_ATTRIBUTES lpSecu;
	BYTE bTab[MAX_KEY_SIZE]={0};
	HKEY hTmpKey,hSubShared;
	int Size;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecu,bAclRes);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&hSubShared,&dWord)!=ERROR_SUCCESS) return;
	// mHelp
	if(RegCreateKeyEx(hSubShared,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&hSubShared,&dWord)!=ERROR_SUCCESS) return;
	// mHpShared
	if(RegCreateKeyEx(hSubShared,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE|KEY_READ,&lpSecu,
					&hTmpKey,&dWord)!=ERROR_SUCCESS) return;
	RegCloseKey(hTmpKey);
	// mHpProcID
	if(RegCreateKeyEx(hSubShared,MHPSHARED_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecu,
					&hTmpKey,&dWord)!=ERROR_SUCCESS) return;
	Size=mHpPak_FillDataFromNumber(bTab,iProcNewID,FALSE);
	RegSetValueEx(hTmpKey,MHPPROCID_KEY,0,REG_DWORD,bTab,Size);
	RegCloseKey(hTmpKey);
}

//-----------------------------------------------------//
// mHpPak_GetNextAppInfoKeys
//-----------------------------------------------------//
MHPPAK_API void mHpPak_GetNextAppInfoKeys(int iAppIndex,char* lpAppName,int iMaxName,int &iAppStatus,int &imHpStatus,BOOL &bResult)
{	BYTE TabByte[MAX_KEY_SIZE]={0};
	FILETIME FileTime;
	HKEY hmHpAppKey;
	DWORD dwAppli=(DWORD)iAppIndex,dwSize=MAX_KEY_SIZE,dwType=REG_DWORD,dwAppSize=(DWORD)iMaxName;
	if(iAppIndex!=NO) mHpPak_InitStr(lpAppName,iMaxName);
	// Software
	if(RegOpenKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,KEY_READ,&hmHpAppKey)==ERROR_SUCCESS)
	{	// mHelp	
		if(RegOpenKeyEx(hmHpAppKey,MHELP_KEY,0,KEY_READ,&hmHpAppKey)==ERROR_SUCCESS)
		{	// mHpApp
			if(RegOpenKeyEx(hmHpAppKey,MHPAPP_KEY,0,KEY_READ,&hmHpAppKey)==ERROR_SUCCESS)
			{	if(((iAppIndex!=NO)&&(RegEnumKeyEx(hmHpAppKey,dwAppli,lpAppName,&dwAppSize,NULL,NULL,NULL,&FileTime)==ERROR_SUCCESS))||
				    (iAppIndex==NO))
				{	// AppName
					if(RegOpenKeyEx(hmHpAppKey,lpAppName,0,KEY_READ,&hmHpAppKey)==ERROR_SUCCESS)
					{	// AppStatus
						if(RegQueryValueEx(hmHpAppKey,APPSTATUS_KEY,NULL,&dwType,TabByte,&dwSize)==ERROR_SUCCESS)
						{	mHpPak_FillIntFromData(TabByte,iAppStatus,FALSE);}
						else iAppStatus=0;
						dwSize=MAX_KEY_SIZE;dwType=REG_DWORD;
						// mHpStatus
						if(RegQueryValueEx(hmHpAppKey,MHPSTATUS_KEY,NULL,&dwType,TabByte,&dwSize)==ERROR_SUCCESS)
						{	mHpPak_FillIntFromData(TabByte,imHpStatus,FALSE);}
						else imHpStatus=0;
						bResult=TRUE;
						return;
					}
				}
			}
		}
	}
	bResult=FALSE;
}

//-----------------------------------------------------//
// mHpPak_SetAppInfoKeys
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_SetAppInfoKeys(char* lpAppName,int iAppStatus,int imHpStatus)
{	BYTE TabByte[MAX_KEY_SIZE]={0};
	SECURITY_ATTRIBUTES lpSecuAtt;
	HKEY hmHpAppKey,hTmpKey;
	DWORD DWord=REG_OPENED_EXISTING_KEY;
	int iSize;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecuAtt,bAclRes);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
			  KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
			  &hmHpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHelp
	if(RegCreateKeyEx(hmHpAppKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
			  KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
			  &hmHpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHpApp
	if(RegCreateKeyEx(hmHpAppKey,MHPAPP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
			  KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
			  &hmHpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// AppName
	if(RegCreateKeyEx(hmHpAppKey,lpAppName,0,NULL,REG_OPTION_NON_VOLATILE,
			  KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
			  &hTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	RegCloseKey(hTmpKey);
	// AppStatus
	if(RegCreateKeyEx(hmHpAppKey,lpAppName,0,NULL,REG_OPTION_NON_VOLATILE,
			  KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
			  &hTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	iSize=mHpPak_FillDataFromNumber(TabByte,iAppStatus,FALSE);
	RegSetValueEx(hTmpKey,APPSTATUS_KEY,0,REG_DWORD,TabByte,iSize);
	RegCloseKey(hTmpKey);
	// mHpStatus
	if(RegCreateKeyEx(hmHpAppKey,lpAppName,0,NULL,REG_OPTION_NON_VOLATILE,
			  KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
			  &hTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	iSize=mHpPak_FillDataFromNumber(TabByte,imHpStatus,FALSE);
	RegSetValueEx(hTmpKey,MHPSTATUS_KEY,0,REG_DWORD,TabByte,iSize);
	RegCloseKey(hTmpKey);
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_DestroyAppKeys
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_DestroyAppKeys(char* lpDestroyApp)
{	SECURITY_ATTRIBUTES lpSecuAtt;
	HKEY lpAppKey,lpTmpKey;
	DWORD DWord=REG_OPENED_EXISTING_KEY;
	BOOL bAclRes;
	mHpPak_GetSecurityAtt(lpSecuAtt,bAclRes);
	// Software
	if(RegCreateKeyEx(HKEY_CURRENT_USER,SOFTWARE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHelp
	if(RegCreateKeyEx(lpAppKey,MHELP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// mHpApp
	if(RegCreateKeyEx(lpAppKey,MHPAPP_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpAppKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	// AppName
	if(RegCreateKeyEx(lpAppKey,lpDestroyApp,0,NULL,REG_OPTION_NON_VOLATILE,
					KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&lpSecuAtt,
					&lpTmpKey,&DWord)!=ERROR_SUCCESS) return FALSE;
	RegCloseKey(lpTmpKey);
	return mHpPak_DeleteSubKey(lpAppKey,lpDestroyApp);
}

//****************************************************************************************************************MHPFILE//
// mHpPak_AddmHpExtension
//---------------------------------------------------------//
void mHpPak_AddmHpExtension(char* lpFile,int iSizeFile)
{	char lpUpperFile[MAX_RS_SIZE]={0};
	char lpTmpFile[MAX_RS_SIZE]={0};
	int indice=0,iExt=0,iLen=mHpPak_FindStrLenght(lpFile,iSizeFile);
	strncpy(lpTmpFile,lpFile,MAX_RS_SIZE);
	if(iLen>4)
	{	strncpy(lpUpperFile,_strupr(lpTmpFile),MAX_RS_SIZE);
		while(lpUpperFile[indice]!=END_LIST)
		{	switch(iExt)
			{	case 0:
				{	if(lpUpperFile[indice]=='.') iExt++;
					break;
				}
				case 1:
				{	if(lpUpperFile[indice]=='M') iExt++;
					else iExt=0;
					break;
				}
				case 2:
				{	if(lpUpperFile[indice]=='H') iExt++;
					else iExt=0;
					break;
				}
				case 3:
				{	if(lpUpperFile[indice]=='F') iExt++;
					else iExt=0;
					break;
				}
			}
			indice++;
		}
		if(iExt==4) return;
	}
	strncat(lpFile,MHPPAK_MHF_EXTENSION,4);
}

//---------------------------------------------------------//
// mHpPak_GetNewmHpFile
//---------------------------------------------------------//
BOOL mHpPak_GetNewmHpFile(HWND hWndActive,HINSTANCE hInst,UINT idsFilter,char* lpNewFile,int FileSize)
{	OPENFILENAME NewFile;
	char szFilter[MAX_KEY_SIZE]={0};
	char szFile[MAX_RS_SIZE]={0};
	char szPath[MAX_RS_SIZE]={0};
	int indice1=0,indice2=0,iCount=NO;
	BOOL bExit=FALSE;
	BOOL bMultiSel=FALSE;
	LoadString(hInst,idsFilter,szFilter,MAX_KEY_SIZE);
    memcpy(&szFilter[strlen(szFilter)+1],MHPPAK_MHF_EXTFILTER,sizeof(MHPPAK_MHF_EXTFILTER));
	//strncpy(&szFilter[strlen(szFilter)+1],MHPPAK_DHF_EXTENSION,MAX_KEY_SIZE); // Stack corrupted!
	memset(&NewFile,0,sizeof(OPENFILENAME));
	NewFile.lStructSize=sizeof(OPENFILENAME);
	NewFile.hwndOwner=hWndActive;
	NewFile.lpstrFilter=szFilter;
	NewFile.nFilterIndex=1;
	NewFile.nMaxFile=MAX_RS_SIZE;
	NewFile.lpstrTitle=NULL;
	NewFile.lpstrFile=szFile;
	NewFile.Flags=OFN_ENABLESIZING|OFN_EXPLORER|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
	if(GetSaveFileName(&NewFile))
	{	strncpy(lpNewFile,szFile,FileSize);
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------//
// mHpPak_FindEventLine
//-----------------------------------------------------//
MHPPAK_API void mHpPak_FindEventLine(HWND hWndActive,char* mHpFileName,int *Tab,int &Line)
{	char chCompare[MAX_RS_SIZE]="#";
	char chTemp[MAX_RS_SIZE]={0};
	char chFile[MAX_RS_SIZE]={0};
	BOOL bContinue=TRUE;
	int itemp,indice=0;
    if(Line==NO)
	    Line=mHpPak_GetTaskLine(hWndActive,mHpFileName,TRUE);
	itemp=Line;
	if(Line!=NO)
	{	if(mHpPak_PosCurSeekLine(hWndActive,mHpFileName,Line,FALSE))
		{	while(bContinue)
			{	_snprintf(chTemp,MAX_RS_SIZE,"%d",Tab[indice]);
				strncat(chCompare,chTemp,mHpPak_FindStrLenght(chTemp,MAX_RS_SIZE));
				indice++;
				if(Tab[indice]==NO)	bContinue=FALSE;
				else bContinue=TRUE;
				if(bContinue) strncat(chCompare,SEPARATED_STRING,1);
			}
			indice=0;
			while(1)
			{	if(!StructReadFile(hWndActive,mHpFileName,chTemp,1,mHpFile,pHmFile))
				{	Line=NO;
					return;
				}
                switch(chTemp[0])
                {   case NEXT_TASK:
                    {	if(!strncmp(chFile,chCompare,MAX_RS_SIZE))
					    {	if(itemp!=Line) Line--;
						    return; // OK
					    }
					    else if(!mHpPak_PosCurSeekLine(hWndActive,mHpFileName,Line++,FALSE))
					    {	Line=NO;
						    return;
					    }
					    else
					    {	mHpPak_InitStr(chFile,MAX_RS_SIZE);
						    indice=0;
					    }
                        break;
                    }
                    case END_FILE:
					{	ReadError(hWndActive,mHpFileName);
                        Line=NO;
						return;
					}
                    default:
				    {	chFile[indice]=chTemp[0];
					    indice++;
                        break;
                    }
                }
			}
		}
	    Line=NO;
	}
}

//-----------------------------------------------------//
// mHpPak_GetNextEvent
//-----------------------------------------------------//
MHPPAK_API void mHpPak_GetNextEvent(HWND hWndActive,char* mHpFileName,int &Type,UINT &iPlatformID,int &iClass,int &iStyle,int &iMsgID,BOOL &Error)
{	char ch[1]={0};
	BOOL bNumber=FALSE;
	BOOL bNextTask=TRUE;
	int iCtrlMan=0;
	Error=TRUE;
    iPlatformID=0;
	iClass=iStyle=iMsgID=Type=NO;
	if(OpenmHelpFile(hWndActive,mHpFileName))
	{	while(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
		{	if(bNextTask)
			{	if(mHpPak_IsItANumber(ch[0]))
				{	if(!bNumber)
					{	Type=mHpPak_CharToInt(ch[0]);
						bNumber=TRUE;
					}
					else
					{	Type*=10;
						Type+=mHpPak_CharToInt(ch[0]);					
					}
				}
                else switch(ch[0])
                {   case TREE_PARAM_GROUP: // '¤'
                    case START_TASK: // '#'
                    case ITEM_GROUP: // '^'
                    case END_FILE: // '¨'
				    {   if(iCtrlMan)
                        {   iMsgID=Type; // Message ID
                            if((iClass!=NO)&&(iStyle!=NO)&&(iMsgID!=NO))
                                Type=MHPID_CTRL_MAN;
                            else
                                return; // Error
                        }
                        Error=FALSE;
					    return; // Ok
				    }
                    case NEXT_RS: // '|'
                    {   switch(iCtrlMan)
                        {   case 0: // Platform
                            {   iPlatformID=(UINT)Type;
                                iCtrlMan++;
                                break;
                            }
                            case 1: // Class
                            {   iClass=Type;
                                iCtrlMan++;
                                break;
                            }
                            case 2: // Style
                            {   iStyle=Type;
                                iCtrlMan++;
                                break;
                            }
                            case 3:
                                return; // Error
                        }
                        Type=NO;
                        bNumber=FALSE;
                        break;
                    }
                    default:
                    {   return; } // Error
                }
			}
			else if(ch[0]==NEXT_TASK) bNextTask=TRUE;
		}
	}
}

//-----------------------------------------------------//
// mHpPak_ClosemHpFile
//-----------------------------------------------------//
MHPPAK_API void mHpPak_ClosemHpFile()
{	bFileOpen=FALSE;
	bTmpFile=FALSE;
	if(mHpFile!=NULL) fclose(mHpFile);
	if(pHmFile!=NULL) fclose(pHmFile);
	if(TmpFile!=NULL) fclose(TmpFile);
}

//-----------------------------------------------------//
// mHpPak_GetEventPart
//-----------------------------------------------------//
MHPPAK_API void mHpPak_GetEventPart(HWND hWndActive,char* mHpFileName,int &iResult,char *lpResult,
                                    int ResSize,char &NextChar,BOOL bStrForce)
{   char ch[1]={0};
    int indice=0;
    BOOL bNumber=FALSE,bChar=bStrForce;
    iResult=NO;
    BOOL bAntiShl=FALSE;
    if(OpenmHelpFile(hWndActive,mHpFileName))
    {   mHpPak_InitStr(lpResult,ResSize);
        while(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
        {   if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
			else
			{   if((mHpPak_IsItANumber(ch[0]))&&(!bChar))
				{	if(bNumber)
					{	iResult*=10;
						iResult+=mHpPak_CharToInt(ch[0]);
					}
					else
					{	bNumber=TRUE;
						iResult=mHpPak_CharToInt(ch[0]);
					}
				}
				else if(!bAntiShl)
                {   BOOL bCommonExit=FALSE;
                    switch(ch[0])
					{	case START_CTRL: // '@'
                        case TREE_PARAM_GROUP: // '¤'
                        case NEXT_TASK: // '_'
                        case END_RS_TASK: // '\n'
                        case END_FILE: // '¨'
				        {	bCommonExit=TRUE;
                            break;
                        }
                        case NEXT_RS: // '|'
				        {	if(NextChar==NEXT_RS) // '|'
                            {	bCommonExit=TRUE;
                                break;
                            }
                            if(!bNumber) bChar=TRUE;
					        lpResult[indice]=END_RS_TASK; // Replace '|' with '\n'
					        indice++;
                            break;
                        }
                        case ITEM_GROUP: // '^'
                        {   switch(NextChar)
                            {   case ITEM_GROUP: // '^'
				                {	iResult=0;
					                return; ////// Return 'lpResult'
                                }
                                case NEXT_RS: // '|'
				                {	bCommonExit=TRUE;
                                    break;
                                }
                                default:
                                {   if(!bNumber) bChar=TRUE;
					                lpResult[indice]=ch[0];
					                indice++;
                                    break;
                                }
                            }
                            break;
                        }
                        default:
				        {	if(!bNumber) bChar=TRUE;
					        lpResult[indice]=ch[0];
					        indice++;
                            break;
                        }
					}
                    // Check common exit
                    if(bCommonExit)
				    {	if(bNumber) mHpPak_InitStr(lpResult,ResSize);
					    if(bStrForce) iResult=0;
					    NextChar=ch[0];
					    return; ////// Return 'iResult' or 'lpResult'
                    }
				}
				else // bAntiShl == TRUE
				{	if(!bNumber) bChar=TRUE;
					lpResult[indice]=ch[0];
					indice++;
				}
				bAntiShl=FALSE;
            }
		}
        // Error
		iResult=NO;
		mHpPak_InitStr(lpResult,ResSize);
	}
}

//-----------------------------------------------------//
// mHpPak_PosCurSeekLine
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_PosCurSeekLine(HWND hWndActive,char* mHpFileName,int iLine,BOOL bEndFile)
{	char charTemp[1]={0};
	int indice=0;
	int iTemp=iLine;
	BOOL bAntiShl=FALSE;
	if(!OpenmHelpFile(hWndActive,mHpFileName)) return FALSE;
	if(fseek(mHpFile,0L,SEEK_SET))
	{	ReadError(hWndActive,mHpFileName);
		return FALSE;
	}
	if(fseek(pHmFile,1L,SEEK_SET))
	{	ReadError(hWndActive,mHpFileName);
		return FALSE;
	}
	while(iTemp!=0)
	{	if(!StructReadFile(hWndActive,mHpFileName,charTemp,1,mHpFile,pHmFile)) return FALSE;
		if(charTemp[0]==END_RS_TASK) iTemp--;
		else if((charTemp[0]==END_FILE)&&(!bAntiShl)&&(!bEndFile))
		{   ReadError(hWndActive,mHpFileName);
			return FALSE;
		}
		if((charTemp[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else if(bAntiShl) bAntiShl=FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_GetTopicmHpFile
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_GetTopicmHpFile(HWND hWndActive,char* mHpFileName,char* lpTopic,int TopicSize)
{	FILE* TopicmHpFile;
	FILE* TopicpHmFile;
	BOOL bExit=FALSE;
	char szTemp[MAX_RS_SIZE]={0};
	char ch[1]={0};
	int indice=0;
	TopicmHpFile=fopen(mHpFileName,"rb");
	TopicpHmFile=fopen(mHpFileName,"rb");
	mHpPak_InitStr(lpTopic,TopicSize);
	if(TopicmHpFile==NULL)
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return FALSE;
	}
	if(TopicpHmFile==NULL)
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return FALSE;
	}
	if(fseek(TopicmHpFile,0L,SEEK_SET))
	{	ReadError(hWndActive,mHpFileName); // Error	
		return FALSE;
	}
	if(fseek(TopicpHmFile,1L,SEEK_SET))
	{	ReadError(hWndActive,mHpFileName); // Error	
		return FALSE;
	}
	while(!bExit)
	{	if(!StructReadFile(hWndActive,mHpFileName,ch,1,TopicmHpFile,TopicpHmFile)) return FALSE;
		if(ch[0]!=END_RS_TASK)
		{	szTemp[indice]=ch[0];
			indice++;
		}
		else bExit=TRUE;
	}
	strncpy(lpTopic,szTemp,TopicSize);
	fclose(TopicmHpFile);
	fclose(TopicpHmFile);
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_GetTreeLineCnt
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetTreeLineCnt(HWND hWndActive,char* mHpFileName)
{	FILE* TmpmHpFile;
	FILE* TmppHmFile;
	BOOL bExit=FALSE;
	char ch[1]={0};
	int iCount=0;
	BOOL bAntiShl=FALSE;
	TmpmHpFile=fopen(mHpFileName,"rb");
	if(TmpmHpFile==NULL)
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return NO;
	}
	TmppHmFile=fopen(mHpFileName,"rb");
	if((TmppHmFile==NULL)||(fseek(TmppHmFile,1L,SEEK_SET)))
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		fclose(TmpmHpFile);
		return NO;
	}
	if(!mHpPak_PosCurSeekLine(hWndActive,mHpFileName,1,FALSE))
	{	fclose(TmpmHpFile);
		fclose(TmppHmFile);
		return NO;
	}
	while(!bExit)
	{	if(!StructReadFile(hWndActive,mHpFileName,ch,1,TmpmHpFile,TmppHmFile))
		{	fclose(TmpmHpFile);
			fclose(TmppHmFile);
			return NO;
		}
		if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else
		{	if(!bAntiShl)
			{	switch(ch[0])
				{	case END_RS_TASK: // '\n'
					{	iCount++;
						break;
					}
					case END_FILE: // '¨'
					case START_TASK: // '#'
					{	bExit=TRUE;
						break;
					}
				}
			}
			else bAntiShl=FALSE;
		}
	}
	fclose(TmpmHpFile);
	fclose(TmppHmFile);
	return iCount;
}

//-----------------------------------------------------//
// mHpPak_GetTreeLineCnt
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetCtrlLineCnt(HWND hWndActive,char* mHpFileName)
{	FILE* TmpmHpFile;
	FILE* TmppHmFile;
	BOOL bExit=FALSE;
	char ch[1]={0};
	int iCount=0;
	BOOL bAntiShl=FALSE;
	TmpmHpFile=fopen(mHpFileName,"rb");
	if(TmpmHpFile==NULL)
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return NO;
	}
	TmppHmFile=fopen(mHpFileName,"rb");
	if((TmppHmFile==NULL)||(fseek(TmppHmFile,1L,SEEK_SET)))
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		fclose(TmpmHpFile);
		return NO;
	}
	if(!mHpPak_PosCurSeekLine(hWndActive,mHpFileName,1,FALSE))
	{	fclose(TmpmHpFile);
		fclose(TmppHmFile);
		return NO;
	}
	while(!bExit)
	{	if(!StructReadFile(hWndActive,mHpFileName,ch,1,TmpmHpFile,TmppHmFile))
		{	fclose(TmpmHpFile);
			fclose(TmppHmFile);
			return NO;
		}
		if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else
		{	if(!bAntiShl)
			{	switch(ch[0])
				{	case END_RS_TASK: // '\n'
					{	iCount++;
						break;
					}
					case END_FILE: // '¨'
					case START_CTRL: // '@'
					{	bExit=TRUE;
						break;
					}
				}
			}
			else bAntiShl=FALSE;
		}
	}
	fclose(TmpmHpFile);
	fclose(TmppHmFile);
	return iCount;
}

//-----------------------------------------------------//
// mHpPak_GetNextStrRS
//-----------------------------------------------------//
MHPPAK_API void mHpPak_GetNextStrRS(HWND hWndActive,char* mHpFileName,char* StrRS,int iRSsize,int &iPos,int &iEndRSParam)
{	char ch[1]={0};
	char RS[MAX_RS_SIZE]={0};
	int indice=0;
	BOOL bAntiShl=FALSE;
	if(OpenmHelpFile(hWndActive,mHpFileName))
	{	iEndRSParam=0;
		iPos=0;
		mHpPak_InitStr(StrRS,iRSsize);
		while(indice<MAX_RS_SIZE)
		{	if(!StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
			{	iPos=NO; // Error 
				return;
			}
			if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
			else
			{	if(!bAntiShl)
				{	switch(ch[0])
					{	case NEXT_RS: // '|'
						{	iPos++;
							break;
						}
						case RS_PARAM: // '§'
						{	iEndRSParam=1; // 1: ParamRS
							return;							
						}
#ifdef MHELP_OLD
						case TREE_PARAM_GROUP: // '¤'
						{	return;
						}
#endif
						case END_RS_TASK: // '\n'
						{	strncpy(StrRS,RS,iRSsize);
							return;
						}
                        case START_CTRL: // '@'
						case START_TASK: // '#'
						case END_FILE: // '¨'
						{	iEndRSParam=2; // 2: EndRS
							return;
						}
						default:
						{	RS[indice]=ch[0];
							indice++;
							break;
						}
					}
				}
				else
				{	RS[indice]=ch[0];
					indice++;
					bAntiShl=FALSE;
				}
			}
		}
	}
}

//-----------------------------------------------------//
// mHpPak_GetDetailParamRS
//-----------------------------------------------------//
MHPPAK_API void mHpPak_GetDetailParamRS(HWND hWndActive,char* mHpFileName,char *strParam,int ParamSize,int &itype,
										char *strlimit,int LimitSize,char *strDecrit,int DecritSize,BOOL &bNext)
{	char lp[MAX_RS_SIZE]={0};
	char ch[1]={0};
	int State=0,indice=0;
	BOOL bAntiShl=FALSE;
	if(OpenmHelpFile(hWndActive,mHpFileName))
	{	bNext=FALSE;
		mHpPak_InitStr(strParam,ParamSize);
		mHpPak_InitStr(strlimit,LimitSize);
		mHpPak_InitStr(strDecrit,DecritSize);
		while(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
		{	if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
			else
			{	if(!bAntiShl)
				{	switch(ch[0])
					{	case TREE_PARAM_GROUP: //'¤'
						{	if(State)
							{	State++;
								indice=0;
								if(State==4)
								{	strncpy(strlimit,lp,LimitSize);
									mHpPak_InitStr(lp,MAX_RS_SIZE);
								}
							}
							else
							{	strncpy(strParam,lp,ParamSize);
								mHpPak_InitStr(lp,MAX_RS_SIZE);
								State++;
								indice=0;
							}
							break;
						}
						case RS_PARAM: //'§'
						case END_RS_TASK: //'\n'
						{	if(State!=4)
							{	itype=NO;
								ReadError(hWndActive,mHpFileName);
								return;
							}
							strncpy(strDecrit,lp,DecritSize);
							if(ch[0]==RS_PARAM) bNext=TRUE;
							return;
						}
						case END_FILE: // '¨'
						{	itype=NO;
							ReadError(hWndActive,mHpFileName);
							return;
						}
						default:
						{	if((mHpPak_IsItANumber(ch[0]))&&(State==1))
							{	itype=mHpPak_CharToInt(ch[0]);
								State++;
							}
							else
							{	lp[indice]=ch[0];
								indice++;
							}
						}
					}
				}
				else
				{	lp[indice]=ch[0];
					indice++;
					bAntiShl=FALSE;
				}
			}
		}
		itype=NO;
	}
}

//-----------------------------------------------------//
// mHpPak_IsFilePathExist
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_IsFilePathExist(HWND hWndActive,char *mHpFileName)
{	FILE* TempFile;
	TempFile=fopen(mHpFileName,"rb");
	if(TempFile==NULL)
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONEXCLAMATION);
		return FALSE;
	}
	fclose(TempFile);
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_InsertInmHpFile
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_InsertInmHpFile(HWND hWndActive,char* mHpFileName,int LineRead,BOOL bEndLine,BOOL bEndFile)
{	char ch[1]={0};
	int iLine=LineRead,iLag=0;
	if(OpenmHelpFile(hWndActive,mHpFileName))
	{	if(CreateTempFile(hWndActive))	
		{	if(!fseek(mHpFile,0L,SEEK_SET))
			{	if(!fseek(pHmFile,1L,SEEK_SET))
				{	while(iLine!=0)	
					{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
						{	if(StructWriteFile(ch,1)==1)
							{	if(ch[0]==END_RS_TASK) iLine--; // '\n'
							}
							else 
							{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_WRITE,NULL,0,0,MB_ICONSTOP);
								mHpPak_ClosemHpFile();
								return FALSE;
							}
						}
						else return FALSE;
                        //Sleep(1);
					}
					if(bEndLine)
					{	while((StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))&&(ch[0]!=END_RS_TASK))
						{	if(StructWriteFile(ch,1)!=1)
							{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_WRITE,NULL,0,0,MB_ICONSTOP);
								mHpPak_ClosemHpFile();
								return FALSE;
							}
							iLag++;
                            //Sleep(1);
						}
						if(ch[0]==END_RS_TASK) // '\n'
						{	if(!mHpPak_PosCurSeekLine(hWndActive,mHpFileName,LineRead,bEndFile))
                                return FALSE;
							while(iLag!=0)
							{	if(!StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
                                    return FALSE;
								iLag--;
							}
						}
						else return FALSE;
					}
					return TRUE;
				}
				else ReadError(hWndActive,mHpFileName);
			}
			else ReadError(hWndActive,mHpFileName);
		}
	}
	mHpPak_ClosemHpFile();
	return FALSE;
}

//-----------------------------------------------------//
// mHpPak_WriteParty
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_WriteParty(HWND hWndActive,char SymbolID,int iValue,const char *StrValue,BOOL bNoCmd)
{	char lpTemp[MAX_RS_SIZE]={0};
	char strval[MAX_RS_SIZE]={0};
	int iLen,indice=0;
	lpTemp[indice]=SymbolID;
	if((iValue==NO)&&(StrValue==NULL))
	{	if(StructWriteFile(lpTemp,(indice+1))!=(indice+1)) mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_WRITE,NULL,0,0,MB_ICONSTOP);
		else return TRUE;
	}
	else 
	{	if((SymbolID==END_FILE)&&(StrValue!=NULL))
		{	if(bNoCmd) mHpPak_WriteString(strval,MAX_RS_SIZE,StrValue);
			else strncpy(strval,StrValue,MAX_RS_SIZE);
			iLen=mHpPak_FindStrLenght(strval,MAX_RS_SIZE);
			if(StructWriteFile(strval,iLen)!=iLen)
			{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_WRITE,NULL,0,0,MB_ICONSTOP);}
			else return TRUE;
		}
		else if(StrValue!=NULL)
		{	if(StructWriteFile(lpTemp,(indice+1))!=(indice+1))
			{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_WRITE,NULL,0,0,MB_ICONSTOP);}
			if(bNoCmd) mHpPak_WriteString(strval,MAX_RS_SIZE,StrValue);
			else strncpy(strval,StrValue,MAX_RS_SIZE);
			iLen=mHpPak_FindStrLenght(strval,MAX_RS_SIZE);
			if(StructWriteFile(strval,iLen)!=iLen)
			{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_WRITE,NULL,0,0,MB_ICONSTOP);}
			else return TRUE;
		}
		else if(iValue!=NO)
		{	indice+=PutIntValue(hWndActive,lpTemp,iValue);
			if(StructWriteFile(lpTemp,indice)!=indice)
			{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_WRITE,NULL,0,0,MB_ICONSTOP);}
			else return TRUE;
		}
	}
	mHpPak_ClosemHpFile();
	return FALSE;
}
	
//-----------------------------------------------------//
// mHpPak_ConcatInmHpFile
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_ConcatInmHpFile(HWND hWndActive,char* mHpFileName,int iLineRead,BOOL bEndFile)
{	char lpTmp[2]={0};
	char ch[1]={0};
	BOOL bAntiShl=FALSE,bNoFav=TRUE;
	int iEndCnt=1;
	if(OpenmHelpFile(hWndActive,mHpFileName))
	{	iEndCnt=mHpPak_GetFavsLineCnt(hWndActive,mHpFileName);
		if(iEndCnt==NO) return FALSE;
		mHpPak_AddNewEndFile(hWndActive,mHpFileName,iEndCnt,bNoFav);
		if(iEndCnt==NO) return FALSE;
		if((!bNoFav)&&(iLineRead<iEndCnt)) iEndCnt=2;
		else iEndCnt=1;
		if(mHpPak_PosCurSeekLine(hWndActive,mHpFileName,iLineRead,bEndFile))
		{	//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
			iBugLine=iLineRead;
			iBugLag=0;
			//************************************************
			while((StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))&&((ch[0]!=END_FILE)||(bAntiShl)||(iEndCnt==2)))
			{	if((ch[0]==END_FILE)&&(!bAntiShl)) iEndCnt--;
				if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
				else bAntiShl=FALSE;
				//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
				iBugLag++;
				//************************************************
				if(StructWriteFile(ch,1)!=1)
				{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
					mHpPak_ClosemHpFile();
					return FALSE;
				}
			}
			if((ch[0]==END_FILE)&&(!bAntiShl)&&(iEndCnt!=2))
			{	lpTmp[0]=END_FILE;
				lpTmp[1]=END_RS_TASK;
				if(StructWriteFile(lpTmp,2)!=2)
				{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
					mHpPak_ClosemHpFile();
					return FALSE;
				}
				return DestroyAndRename(hWndActive,mHpFileName);
			}
		}
	}
	return FALSE;
}

//-----------------------------------------------------//
// mHpPak_GetLineFromTab
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetLineFromTab(HWND hWndActive,char* mHpFileName,int* iTabRef,int iDecalRef)
{	int iReadTab[MAX_PART_TREE]={0};
	BOOL bNewLine,bError,bFind=FALSE;
	char ch[1]={0};
	int iRef=0,iLineRes=1;
	if(iDecalRef) iTabRef[(mHpPak_GetCountIndex(iTabRef)-1)]-=iDecalRef;
	else iTabRef[(mHpPak_GetCountIndex(iTabRef)-1)]--;
	IsItAnEmptyFile(hWndActive,mHpFileName,bNewLine,bError);
	if((!bError)&&(OpenmHelpFile(hWndActive,mHpFileName)))
	{	if(bNewLine) return iLineRes;
		if(mHpPak_PosCurSeekLine(hWndActive,mHpFileName,1,FALSE))
		{	mHpPak_EmptyTab(iReadTab,MAX_PART_TREE);
			bNewLine=TRUE;
			while(1)
			{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
				{	if(bNewLine)
					{	if(ch[0]==NEXT_RS) iRef++;
						else
						{	iReadTab[iRef]++;
							iReadTab[(iRef+1)]=NO;
							if(bFind)
							{	if((mHpPak_GetCountIndex(iTabRef)>=mHpPak_GetCountIndex(iReadTab))||(ch[0]==START_TASK))
								{	return (iLineRes-1);}
							}
							else if(mHpPak_CompareTab(iReadTab,iTabRef))
							{	if(iDecalRef) iTabRef[(mHpPak_GetCountIndex(iTabRef)-1)]+=iDecalRef;
								else
								{	iTabRef[(mHpPak_GetCountIndex(iTabRef)-1)]++;
									iLineRes++;
								}
								bFind=TRUE;
							}
							bNewLine=FALSE;
							iRef=0;
						}
					}
					if(ch[0]==END_RS_TASK)
					{	bNewLine=TRUE;
						iLineRes++;
					}
				}
				else return NO;
			}
		}
	}
	mHpPak_ClosemHpFile();
	return NO;
}

//-----------------------------------------------------//
// mHpPak_GetTaskLine
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetTaskLine(HWND hWndActive,char* mHpFileName,BOOL bFirst)
{	int iResult=1;
	char ch[1]={0};
	BOOL bAntiShl=FALSE,bFind=FALSE;
	if(!mHpPak_PosCurSeekLine(hWndActive,mHpFileName,1,FALSE)) return NO;
	while(!bFind)
	{	if(!StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile)) return NO;
		if(ch[0]==END_RS_TASK) iResult++;
		if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else
		{	if(!bAntiShl)
			{	if(bFirst)
				{	if(ch[0]==START_TASK) bFind=TRUE;
				}
				else
				{	if(ch[0]==END_FILE) bFind=TRUE;
				}
			}
			else bAntiShl=FALSE;
		}
	}
	return iResult;
}

//-----------------------------------------------------//
// mHpPak_DecalTaskRef
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_DecalTaskRef(HWND hWndActive,char* mHpFileName,int* DecalTab)
{	char lpTemp[MAX_RS_SIZE]={0};
	char ch[1]={0};
	int iDecal=mHpPak_GetCountIndex(DecalTab)-1;
	int iLen,iLineEnd,iline=0;
	BOOL bWrite=FALSE,bAntiShl=FALSE,bEmptyFav=FALSE;
	if(OpenmHelpFile(hWndActive,mHpFileName))
	{	iLineEnd=mHpPak_GetFavsLineCnt(hWndActive,mHpFileName);
		if(iLineEnd==NO) return FALSE;
		mHpPak_AddNewEndFile(hWndActive,mHpFileName,iLineEnd,bEmptyFav);
		if(iLineEnd==NO) return FALSE;
		if(!bEmptyFav) iLineEnd=2;
		else iLineEnd=1;
		if(CreateTempFile(hWndActive))	
		{	FindTask(hWndActive,mHpFileName,DecalTab,iline);
			if(iline!=NO)
			{	if(fseek(mHpFile,0L,SEEK_SET))
				{	ReadError(hWndActive,mHpFileName);
					return FALSE;
				}
				if(fseek(pHmFile,1L,SEEK_SET))
				{	ReadError(hWndActive,mHpFileName);
					return FALSE;
				}
				while((iline!=1)&&(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile)))
				{	if(ch[0]==END_RS_TASK) iline--;
					if(StructWriteFile(ch,1)!=1)
					{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
						mHpPak_ClosemHpFile();
						return FALSE;
					}
				}
				if(iline==1)
				{	while((StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))&&(iDecal!=0))
					{	if((ch[0]==TREE_PARAM_GROUP)&&(!bAntiShl)) iDecal--;
						if(StructWriteFile(ch,1)!=1)
						{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
							mHpPak_ClosemHpFile();
							return FALSE;
						}
						if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
						else bAntiShl=FALSE;
					}
					if(iDecal==0)
					{	_snprintf(lpTemp,MAX_RS_SIZE,"%d",(mHpPak_GetIndex(DecalTab)+1));
						iLen=mHpPak_FindStrLenght(lpTemp,MAX_RS_SIZE);
						if(StructWriteFile(lpTemp,iLen)!=iLen)
						{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
							mHpPak_ClosemHpFile();
							return FALSE;
						}
						while((StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))&&((ch[0]!=END_FILE)||(bAntiShl)||(iLineEnd==2)))
						{	if((ch[0]==END_FILE)&&(!bAntiShl)) iLineEnd--;
							if((ch[0]==NEXT_TASK)&&(!bAntiShl)) bWrite=TRUE;
							if((bWrite)&&(StructWriteFile(ch,1)!=1))
							{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
								mHpPak_ClosemHpFile();
								return FALSE;
							}
							if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
							else bAntiShl=FALSE;
						}
						if(ch[0]==END_FILE)
						{	mHpPak_InitStr(lpTemp,MAX_RS_SIZE);
							lpTemp[0]=END_FILE;lpTemp[1]=END_RS_TASK;
							if(StructWriteFile(lpTemp,2)!=2)
							{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
								mHpPak_ClosemHpFile();
								return FALSE;
							}
							if(DestroyAndRename(hWndActive,mHpFileName)) return TRUE;
						}
					}
					else return FALSE;
				}
			}
			else return FALSE;
		}
	}
	ReadError(hWndActive,mHpFileName);
	return FALSE;
}

//-----------------------------------------------------//
// mHpPak_CreatemHpFile
//-----------------------------------------------------//
MHPPAK_API int mHpPak_CreatemHpFile(HWND hWndActive,char* lpPathFile,char* lpTopic)
{	char lpInfo[MAX_RS_SIZE]={0};
	char lpNoCmdTopic[MAX_RS_SIZE]={0};
	char ch[1]={0};
	int iLen;
	SetViewFlag(FALSE);
	if(OpenmHelpFile(hWndActive,lpPathFile))
	{	SetViewFlag(TRUE);
		if(mHpMessage(hWndActive,hmHpPakInst,IDS_MHPFILE_ALREADYEXIST,NULL,0,0,MB_ICONEXCLAMATION|MB_YESNO)!=IDYES)
		{	mHpPak_ClosemHpFile();
			return 2;
		}
		mHpPak_ClosemHpFile();
		if(!DeleteFile(lpPathFile))
		{	mHpMessage(hWndActive,hmHpPakInst,IDS_DELETE_MHPFILE,NULL,0,0,MB_ICONSTOP);
			return 0;
		}
	}
	SetViewFlag(TRUE);
	TmpFile=fopen(lpPathFile,"w+b");
	if(TmpFile==NULL)
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_CREATE,NULL,0,0,MB_ICONSTOP);
		return 0;
	}
	wLast=0x00;
	mHpPak_WriteString(lpNoCmdTopic,MAX_RS_SIZE,lpTopic);
	strncpy(lpInfo,lpNoCmdTopic,MAX_RS_SIZE);
	strncat(lpInfo,"\n¨\n",3);
	iLen=mHpPak_FindStrLenght(lpInfo,MAX_RS_SIZE);
	if(StructWriteFile(lpInfo,iLen)!=iLen)
	{	mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_WRITE,NULL,0,0,MB_ICONSTOP);
		mHpPak_ClosemHpFile();
		return 0;
	}
	mHpPak_ClosemHpFile();
	return 1;
}

//-----------------------------------------------------//
// mHpPak_GetWindowList
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_GetWindowList(HWND hWndActive,char* lpmHpFileName,CWindowListe* hWindowList)
{	char lpApp[MAX_RS_SIZE]={0};
	char lpClss[MAX_RS_SIZE]={0};
	char lpTmp[MAX_RS_SIZE]={0};
	char ch[1]={0};
	int iCntApp=mHpPak_GetAppWindowCnt(hWndActive,lpmHpFileName);
	int iLine=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFileName);
	int iStep=0,indice=0;
	BOOL bAntiShl=FALSE;
	hWindowList->EmptyWndLst();
	if((iLine==NO)||(iCntApp==NO)) return FALSE;
	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFileName,iLine,FALSE)) return FALSE;
	while(StructReadFile(hWndActive,lpmHpFileName,ch,1,mHpFile,pHmFile))
	{	if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else
		{	if(!bAntiShl)
			{	switch(ch[0])
				{	case START_TASK: // '#'
					{	iStep=0;
						break;
					}
					case NEXT_TASK: // '_'
					{	switch(iStep)
						{	case 0:
							{	iStep++;
								mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
								indice=0;
								break;
							}
							case 1:
							{	iStep++;
								mHpPak_InitStr(lpApp,MAX_RS_SIZE);
								strncpy(lpApp,lpTmp,MAX_RS_SIZE);
								mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
								indice=0;
								break;
							}
							case 2:
							{	hWindowList->AddWndInfo(lpApp,lpClss,lpTmp,TRUE);
								mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
								indice=0;
								break;
							}
						}
						break;
					}
					case TREE_PARAM_GROUP: // '¤'
					{	mHpPak_InitStr(lpClss,MAX_RS_SIZE);
						strncpy(lpClss,lpTmp,MAX_RS_SIZE);
						mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
						indice=0;
						break;
					}
					case END_RS_TASK: // '\n'
					{	hWindowList->AddWndInfo(lpApp,lpClss,lpTmp,TRUE);
						mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
						indice=0;
						if(!iCntApp)
						{	mHpPak_ClosemHpFile();
							return TRUE;
						}
						iCntApp--;
						break;
					}
					default:
					{	lpTmp[indice]=ch[0];
						indice++;
						break;
					}
				}
			}
			else
			{	lpTmp[indice]=ch[0];
				indice++;
				bAntiShl=FALSE;
			}
		}
	}
	return FALSE;
}

//---------------------------------------------//
// mHpPak_GetControlList
//---------------------------------------------//
MHPPAK_API BOOL mHpPak_GetControlList(HWND hWndActive,char* lpmHpFileName)
{   mHpPak_ProcCtrlList->EmptyList();
    int iLine=mHpPak_GetCtrlLineCnt(hWndActive,lpmHpFileName);
    if(iLine==NO) return FALSE;
    if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFileName,iLine,FALSE))
        return FALSE;
    char ch[1]={0};
    char lpTmp[MAX_RS_SIZE]={0};
    char lpClss[MAX_RS_SIZE]={0};
    UINT iPlatform=0;
    BOOL bAntiShl=FALSE;
    int iStep=0,indice=0;
    while(StructReadFile(hWndActive,lpmHpFileName,ch,1,mHpFile,pHmFile))
    {   if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
        else
        {   if(!bAntiShl)
            {   switch(ch[0])
                {   case START_CTRL: // '@'
                    {   if((iStep)||(indice))
                        {   mHpPak_ClosemHpFile();
                            return FALSE;
                        }   
                        break;
                    }
					case NEXT_TASK: // '_'
                    {   switch(iStep)
                        {   case 0:
                            {   iStep++;
                                mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
                                indice=0;
                                break;
                            }
                            case 3:
                            {   mHpPak_ProcCtrlList->AddControl(iPlatform,lpClss,lpTmp);
                                mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
                                indice=0;
                                iStep=1;
                                break;
                            }
                            default:
                            {   mHpPak_ClosemHpFile();
                                return FALSE;
                            }
                        }
                        break;
                    }
					case TREE_PARAM_GROUP: // '¤'
                    {   switch(iStep)
                        {   case 1: // PlatformID
                            {   iStep++;
                                iPlatform=(UINT)mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,0);
                                mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
                                indice=0;
                                break;
                            }
                            case 2: // Class
                            {   iStep++;
                                mHpPak_InitStr(lpClss,MAX_RS_SIZE);
                                strncpy(lpClss,lpTmp,MAX_RS_SIZE);
                                mHpPak_InitStr(lpTmp,MAX_RS_SIZE);
                                indice=0;
                                break;
                            }
                            default:
                            {   mHpPak_ClosemHpFile();
                                return FALSE;
                            }
                        }
                        break;
                    }
					case END_RS_TASK: // '\n'
                    {   mHpPak_ClosemHpFile();
                        if(iStep!=3)
                            return FALSE;
                        mHpPak_ProcCtrlList->AddControl(iPlatform,lpClss,lpTmp);
                        return TRUE;
                    }
                    case END_FILE: // '¨'
                    {   mHpPak_ClosemHpFile();
                        if((!iStep)&&(!indice))
                            return TRUE; // No control list found
                                         // TODO: Return FALSE when only managed control will be available!
                        return FALSE;
                    }
                    default:
                    {   lpTmp[indice]=ch[0];
                        indice++;
                        break;
                    }
                }
            }
            else
            {   lpTmp[indice]=ch[0];
                indice++;
                bAntiShl=FALSE;
            }
        }
    }
    return FALSE;
}

//-----------------------------------------------------//
// mHpPak_GetAppWindowCnt
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetAppWindowCnt(HWND hWndActive,char* lpmHpFileName)
{	char ch[1]={0};
	int iLine=mHpPak_GetTreeLineCnt(hWndActive,lpmHpFileName);
	int iRes=NO,iStep=0;
	BOOL bNoNext=FALSE,bAntiShl=FALSE;
	if(iLine==NO) return FALSE;
	if(!mHpPak_PosCurSeekLine(hWndActive,lpmHpFileName,iLine,FALSE)) return NO;
	while(StructReadFile(hWndActive,lpmHpFileName,ch,1,mHpFile,pHmFile))
	{	if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
		else
		{	if(!bAntiShl)
			{	switch(ch[0])
				{	case START_TASK: // '#'
					{	if(!iStep) iStep++;
						else
						{	ReadError(hWndActive,lpmHpFileName); // Error
							return NO;
						}
						break;
					}
					case NEXT_TASK: // '_'
					{	if(iStep==1)
						{	iStep++;
							bNoNext=TRUE;
						}
						else if(!bNoNext)
						{	ReadError(hWndActive,lpmHpFileName); // Error
							return NO;
						}
						break;
					}
					case END_RS_TASK: // '\n'
					{	if(iStep==2)
						{	iStep=0;
							iRes++;
							bNoNext=FALSE;
						}
						else
						{	ReadError(hWndActive,lpmHpFileName); // Error
							return NO;
						}
						break;
					}
					default:
					{	if(iStep==1)
						{	if(iRes==NO) ReadError(hWndActive,lpmHpFileName); // Error
							return iRes;
						}
						break;
					}
				}
			}
			else
			{	if(iStep==1)
				{	if(iRes==NO) ReadError(hWndActive,lpmHpFileName); // Error
					return iRes;
				}
				bAntiShl=FALSE;
			}
		}
	}
	return NO;
}

//-----------------------------------------------------//
// mHpPak_GetFavsLineCnt
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetFavsLineCnt(HWND hWndActive,char* lpmHpFile)
{	char ch[1]={0};
	BOOL bAntiShl=FALSE;
	int iLineRes=0;
	if(OpenmHelpFile(hWndActive,lpmHpFile))
	{	if(!fseek(mHpFile,0L,SEEK_SET))
		{	if(!fseek(pHmFile,1L,SEEK_SET))
			{	while((StructReadFile(hWndActive,lpmHpFile,ch,1,mHpFile,pHmFile))&&((ch[0]!=END_FILE)||(bAntiShl)))
				{	if((ch[0]==ANTI_SHLASS)&&(!bAntiShl)) bAntiShl=TRUE;
					else
					{	if(ch[0]==END_RS_TASK) iLineRes++;
						bAntiShl=FALSE;
					}
				}
				if((ch[0]==END_FILE)&&(!bAntiShl))
				{	bFileOpen=FALSE;
					fclose(mHpFile);
					fclose(pHmFile);
					return (iLineRes+1);
				}
			}
		}
	}
	return NO;
}

//-----------------------------------------------------//
// mHpPak_AddNewEndFile
//-----------------------------------------------------//
MHPPAK_API void mHpPak_AddNewEndFile(HWND hWndActive,char* lpmHpFile,int &iLine,BOOL &bEmpty)
{	char ch[1]={0};
	bEmpty=FALSE;
	if(mHpPak_PosCurSeekLine(hWndActive,lpmHpFile,iLine,TRUE))
	{	SetViewFlag(FALSE);
		//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
		bBugNew=TRUE;
		//************************************************
		if(!StructReadFile(hWndActive,lpmHpFile,ch,1,mHpFile,pHmFile))
		{	bEmpty=TRUE;
			iLine--;
		}
		SetViewFlag(TRUE);
		//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
		bBugNew=FALSE;
		//************************************************
		bFileOpen=FALSE;
		fclose(mHpFile);
		fclose(pHmFile);
		return;
	}
	ReadError(hWndActive,lpmHpFile); // Error
	iLine=NO;
}

//-----------------------------------------------------//
// mHpPak_GetFavoritesFile
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_GetFavoritesFile(HWND hWndActive,char* lpmHpFile,CFavListe* ReadFavsLst)
{	int iReference[MAX_PART_TREE]={0};
	char lpEvent[MAX_RS_SIZE]={0};
	BOOL bError=FALSE;
	CEvoluList* EvoluLst=NULL;
	char ch[1]={0};
	int iRepeat=0,iline;
	ReadFavsLst->EmptyFavList();
	iline=mHpPak_GetFavsLineCnt(hWndActive,lpmHpFile);
	if(iline!=NO)
	{	if(mHpPak_PosCurSeekLine(hWndActive,lpmHpFile,iline,TRUE))
		{	SetViewFlag(FALSE);
			//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
			bBugNew=TRUE;
			//************************************************
			if(!StructReadFile(hWndActive,lpmHpFile,ch,1,mHpFile,pHmFile))
			{	//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
				bBugNew=FALSE;
				//************************************************
				SetViewFlag(TRUE);
				mHpPak_ClosemHpFile();
				return TRUE;
			}
			else
			{	//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
				bBugNew=FALSE;
				//************************************************
				SetViewFlag(TRUE);
				if(ch[0]==START_TASK)
				{	while((ch[0]!=END_FILE)&&(!bError))
					{	ReadCommonFavFile(hWndActive,lpmHpFile,iRepeat,lpEvent,MAX_RS_SIZE,iReference,MAX_PART_TREE,ch[0]);
						if(iRepeat==NO) bError=TRUE;
						else
						{	if(ch[0]==NEXT_TASK)
							{	EvoluLst=new CEvoluList();
								ReadCustomFavFile(hWndActive,lpmHpFile,EvoluLst,ch[0],bError);
								if(bError)
								{	EvoluLst->EmptyEvoluList();
									delete(EvoluLst);
								}
								else ReadFavsLst->AddFavItem(iReference,iRepeat,lpmHpFile,lpEvent,EvoluLst,TRUE);
							}
							else ReadFavsLst->AddFavItem(iReference,iRepeat,lpmHpFile,lpEvent,NULL,TRUE);
						}
					}
					if(!bError)
					{	mHpPak_ClosemHpFile();
						return TRUE;
					}
				}
			}
		}
		else
		{	ReadError(hWndActive,lpmHpFile); // Error
			return FALSE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------//
// mHpPak_DestroyTaskFile
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_DestroyTaskFile(HWND hWndActive,char* mHpFileName,int* iDeleteTab,int* iRefTab,int* iLagTab)
{	char lpTopic[MAX_RS_SIZE]={0};
	char lpRef[MAX_RS_SIZE]={0};
	char lpReadFav[MAX_RS_SIZE]={0};
	char ch[1]={0};
	int iReadRef[MAX_PART_TREE]={0};
	int iLnCnt=0,iDelRef=0,iTaskLn=0,iReadFav=0,iTopic=0;
	BOOL bWriteAll=FALSE,iWrite=1,bAntiShl=FALSE,bEndFile=FALSE,bNumber=FALSE,bContinue=TRUE,bEmpty=TRUE;
	if((OpenmHelpFile(hWndActive,mHpFileName))&&(CreateTempFile(hWndActive)))
	{	mHpPak_InitStr(lpTopic,MAX_RS_SIZE);
		iTaskLn=mHpPak_GetTaskLine(hWndActive,mHpFileName,TRUE);
		iTaskLn++;
		if(!mHpPak_PosCurSeekLine(hWndActive,mHpFileName,0,FALSE)) return FALSE;
		while(!bEndFile)
		{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
			{	if((ch[0]==ANTI_SHLASS)&&(!bAntiShl))
				{	if((iWrite)&&(StructWriteFile(ch,1)!=1))
					{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
						mHpPak_ClosemHpFile();
						return FALSE;
					}
					bAntiShl=TRUE;
				}
				else
				{	if(!bAntiShl)
					{	switch(ch[0])
						{	case END_RS_TASK: // '\n'
							{	if(iTaskLn>=(iLnCnt+1))
								{	iWrite=1;
									iLnCnt++;
									if(iDeleteTab[iDelRef]==iLnCnt)
									{	iDelRef++;
										iWrite=0;
									}
									else if(StructWriteFile(ch,1)!=1)
									{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
										mHpPak_ClosemHpFile();
										return FALSE;
									}
								}
								else
								{	iLnCnt++;
									if(!iWrite) iWrite=1;
									else if(StructWriteFile(ch,1)!=1)
									{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
										mHpPak_ClosemHpFile();
										return FALSE;
									}
								}
								break;
							}
							case END_FILE: // '¨'
							{	if(!bEmpty)
								{	if(StructWriteFile(ch,1)!=1)
									{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
										mHpPak_ClosemHpFile();
										return FALSE;
									}
									if(!StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile)) return FALSE;
									if(StructWriteFile(ch,1)!=1)
									{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
										mHpPak_ClosemHpFile();
										return FALSE;
									}
									bEndFile=TRUE;
								}
								else
								{	mHpPak_ClosemHpFile();
									if((!DeleteFile(mHpFileName))||(!DeleteFile(TmpFileName)))
									{	mHpMessage(hWndActive,hmHpPakInst,IDS_DELETE_MHPFILE,NULL,0,0,MB_ICONSTOP);
										return FALSE;
									}
									if(!mHpPak_CreatemHpFile(hWndActive,mHpFileName,lpTopic)) return FALSE;
									return TRUE;
								}
								break;
							}
							case START_TASK: // '#'
							{	if(iTaskLn<=iLnCnt)
								{	mHpPak_EmptyTab(iReadRef,MAX_PART_TREE);
									iDelRef=0;
									bContinue=TRUE;
									bNumber=FALSE;
									while(bContinue)
									{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
										{	if(mHpPak_IsItANumber(ch[0]))
											{	if(bNumber) iReadRef[iDelRef]=(iReadRef[iDelRef]*10)+mHpPak_CharToInt(ch[0]);
												else iReadRef[iDelRef]=mHpPak_CharToInt(ch[0]);
											}
											else
											{	bNumber=FALSE;
												iDelRef++;
												if(ch[0]==NEXT_TASK) // '_'
												{	iReadRef[iDelRef]=NO;
													bContinue=FALSE;
												}
											}
										}
										else return FALSE;
									}
									if(iReadRef[0]!=NO) // Check no more application
									{	GetNewLagStrRef(iReadRef,iRefTab,iLagTab,lpRef,MAX_RS_SIZE,0,FALSE,iWrite);
										if(iWrite)
										{	bEmpty=FALSE;
											iDelRef=mHpPak_FindStrLenght(lpRef,MAX_RS_SIZE);
											if(StructWriteFile(lpRef,iDelRef)!=iDelRef)
											{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
												mHpPak_ClosemHpFile();
												return FALSE;
											}
										}
									}
									else if(StructWriteFile("#_",2)!=2)
									{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
										mHpPak_ClosemHpFile();
										return FALSE;
									}
								}
								else if(StructWriteFile(ch,1)!=1)
								{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
									mHpPak_ClosemHpFile();
									return FALSE;
								}
								break;
							}
							default:
							{	if(!iLnCnt) 
								{	lpTopic[iTopic]=ch[0];
									iTopic++;
								}
								if((iWrite)&&(StructWriteFile(ch,1)!=1))
								{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
									mHpPak_ClosemHpFile();
									return FALSE;
								}
								break;
							}
						}
					}
					else
					{	if(!iLnCnt) 
						{	lpTopic[iTopic]=ch[0];
							iTopic++;
						}
						if((iWrite)&&(StructWriteFile(ch,1)!=1))
						{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
							mHpPak_ClosemHpFile();
							return FALSE;
						}
						bAntiShl=FALSE;
					}
				}
			}
			else return FALSE;
		}
		SetViewFlag(FALSE);
		//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
		bBugNew=TRUE;
		//************************************************
		if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
		{	SetViewFlag(TRUE);
			//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
			bBugNew=FALSE;
			//************************************************
			mHpPak_InitStr(lpReadFav,MAX_RS_SIZE);
			lpReadFav[iReadFav]=START_TASK;
			iReadFav++;
			iLnCnt=0;
			bEndFile=FALSE;
			iWrite=1;
			while(!bEndFile)
			{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
				{	if((ch[0]==ANTI_SHLASS)&&(!bAntiShl))
					{	lpReadFav[iReadFav]=ch[0];
						iReadFav++;
						bAntiShl=TRUE;
					}
					else
					{	if(!bAntiShl)
						{	switch(ch[0])
							{	case END_RS_TASK: // '\n'
								{	if(iWrite)
									{	iLnCnt++;
										if((StructWriteFile(lpReadFav,iReadFav)!=iReadFav)||(StructWriteFile(ch,1)!=1))
										{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
											mHpPak_ClosemHpFile();
											return FALSE;
										}
									}
									else iWrite=1;
									mHpPak_InitStr(lpReadFav,MAX_RS_SIZE);
									iReadFav=0;
									bWriteAll=FALSE;
									break;
								}
								case END_FILE: // '¨'
								{	if(iLnCnt!=0)
									{	if(StructWriteFile(ch,1)!=1)
										{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
											mHpPak_ClosemHpFile();
											return FALSE;
										}
										if(!StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile)) return FALSE;
										if(StructWriteFile(ch,1)!=1)
										{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
											mHpPak_ClosemHpFile();
											return FALSE;
										}
									}
									bEndFile=TRUE;
									break;
								}
								case NEXT_TASK: // '_'
								{	if(!bWriteAll)
									{	mHpPak_EmptyTab(iReadRef,MAX_PART_TREE);
										iDelRef=0;
										bContinue=TRUE;
										bNumber=FALSE;
										while(bContinue)
										{	if(StructReadFile(hWndActive,mHpFileName,ch,1,mHpFile,pHmFile))
											{	if(mHpPak_IsItANumber(ch[0]))
												{	if(bNumber) iReadRef[iDelRef]=(iReadRef[iDelRef]*10)+mHpPak_CharToInt(ch[0]);
													else iReadRef[iDelRef]=mHpPak_CharToInt(ch[0]);
												}
												else
												{	bNumber=FALSE;
													iDelRef++;
													if((ch[0]==NEXT_TASK)|| // '_'
													   (ch[0]==END_RS_TASK)) // '\n'
													{	iReadRef[iDelRef]=NO;
														bContinue=FALSE;
													}
												}
											}
											else return FALSE;
										}
										if(ch[0]==NEXT_TASK) // '_'
										{	GetNewLagStrRef(iReadRef,iRefTab,iLagTab,lpRef,MAX_RS_SIZE,1,TRUE,iWrite);
											if(iWrite)
											{	iDelRef=mHpPak_FindStrLenght(lpRef,MAX_RS_SIZE);
												strncat(lpReadFav,lpRef,iDelRef);
												iReadFav+=iDelRef;
											}
											bWriteAll=TRUE;
										}
										else
										{	GetNewLagStrRef(iReadRef,iRefTab,iLagTab,lpRef,MAX_RS_SIZE,1,FALSE,iWrite);
											if(iWrite)
											{	iLnCnt++;
												iDelRef=mHpPak_FindStrLenght(lpRef,MAX_RS_SIZE);
												strncat(lpReadFav,lpRef,iDelRef);
												iReadFav+=(iDelRef-1);
												if((StructWriteFile(lpReadFav,iReadFav)!=iReadFav)||(StructWriteFile(ch,1)!=1))
												{	mHpMessage(hWndActive,hmHpPakInst,IDS_WRITE_TMPFILE,NULL,0,0,MB_ICONSTOP);
													mHpPak_ClosemHpFile();
													return FALSE;
												}
											}
											else iWrite=1;
											mHpPak_InitStr(lpReadFav,MAX_RS_SIZE);
											iReadFav=0;
										}
									}
									else
									{	lpReadFav[iReadFav]=ch[0];
										iReadFav++;
									}
									break;
								}
								default:
								{	lpReadFav[iReadFav]=ch[0];
									iReadFav++;
									break;
								}
							}
						}
						else
						{	lpReadFav[iReadFav]=ch[0];
							iReadFav++;
							bAntiShl=FALSE;
						}
					}
				}
				else return FALSE;
			}
		}
		else
		{	SetViewFlag(TRUE);
			//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
			bBugNew=FALSE;
			//************************************************
		}
		return DestroyAndRename(hWndActive,mHpFileName);
	}
	return FALSE;
}

//-----------------------------------------------------//
// mHpPak_EncodemHpFile
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_EncodemHpFile(char* lpmHpFile)
{	char ch[1]={0};
	FILE* ReadFile;
	ReadFile=fopen(lpmHpFile,"r");
	TmpFile=fopen("temp.mhf","w+b");
	if((TmpFile==NULL)||(ReadFile==NULL)) return FALSE;
	while(fread(ch,sizeof(char),1,ReadFile)==1)
	{	if(StructWriteFile(ch,1)!=1)
		{	fclose(ReadFile);
			fclose(TmpFile);
			return FALSE;
		}
	}
	fclose(ReadFile);
	fclose(TmpFile);
	return TRUE;
}

//-----------------------------------------------------//
// mHpPak_DecodemHpFile
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_DecodemHpFile(HWND hWndActive,char* lpmHpFile)
{	char ch[1]={0};
	TmpFile=fopen("temp.tmp","w+");
	if((TmpFile!=NULL)&&(OpenmHelpFile(hWndActive,lpmHpFile)))
	{	//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
        char lpBugFile[MAX_RS_SIZE];
		mHpPak_InitStr(lpBugFile,MAX_RS_SIZE);
		strncpy(lpBugFile,lpmHpFile,MAX_RS_SIZE);
		iBugLine=0;
		iBugLag=0;
		//************************************************
		SetViewFlag(FALSE);
		while(1)
		{	if(!StructReadFile(hWndActive,lpmHpFile,ch,1,mHpFile,pHmFile))
			{	mHpPak_ClosemHpFile();
				SetViewFlag(TRUE);
				return TRUE;
			}
			else
			{	if(fwrite(ch,sizeof(char),1,TmpFile)!=1)
				{	mHpPak_ClosemHpFile();
					SetViewFlag(TRUE);
					return FALSE;
				}
				//** le BUG du "mHpPak_ConcatInmHpFile" (fread) **
				iBugLag++;
				if(ch[1]==END_RS_TASK)
				{	iBugLine++;
					iBugLag=0;
				}
				//************************************************
			}
		}
	}
	return FALSE;
}

//*******************************************************************************************************************HTML//
#define HTML_BODY_TAG           "</body>"

//-----------------------------------------------------//
// mHpPak_InsertHtmlTag
//-----------------------------------------------------//
MHPPAK_API BOOL mHpPak_InsertHtmlTag(HWND hWndActive,const char* lpHtmlFile,const char* lpHtmlTag)
{
    // Open HTML file
    FILE* pHTML;
	pHTML=fopen(lpHtmlFile,"rb");
	if(pHTML==NULL)
	{
        //
        // TODO: Display corresponding error message
        // -> Failed to open HTML file: 'lpHtmlFile'
        //

        mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return FALSE;
	}

    // Fill buffer
    std::string strHTML;
    while(!feof(pHTML))
    {   char lpRead[MAX_RS_SIZE]={0};
        if(fread(lpRead,sizeof(char),MAX_RS_SIZE-1,pHTML))
            strHTML.append(lpRead);
    }
    fclose(pHTML);

    // Insert HTML tag into buffer (before </body> HTML tag)
    size_t iBodyPos=strHTML.rfind(HTML_BODY_TAG);
    if(iBodyPos==std::string::npos)
    {
        //
        // TODO: Display corresponding error message
        // -> Failed to find body close tag
        //

        mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return FALSE;
    }
    strHTML.insert(iBodyPos,lpHtmlTag);

    // Get HTML file path
    char lpPath[MAX_PATH]={0};
    strncpy(lpPath,lpHtmlFile,MAX_PATH);
    UINT iPathPos=0;
    for(UINT i=0; (lpPath[i]!=0); i++)
    {   if(lpPath[i]=='\\')
            iPathPos=i;
    }
    lpPath[iPathPos]=0;

    // Save buffer into temporay file (in the HTML file path)
    GetTempFileName(lpPath,"MHP",0,TmpFileName);
    FILE* pTMP=fopen(TmpFileName,"wb");
    if(strHTML.length()!=fwrite(strHTML.c_str(),sizeof(char),strHTML.length(),pTMP))
    {
        //
        // TODO: Display corresponding error message
        // -> Failed to update HTML file
        //

        mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return FALSE;
    }
    fclose(pTMP);

    // Rename HTML file to make a backup
    char lpBackup[MAX_PATH];
    _snprintf(lpBackup,MAX_PATH,"%s.backup",lpHtmlFile);
    if(!CopyFile(lpHtmlFile,lpBackup,TRUE))
    {
        //
        // TODO: Display corresponding error message
        // -> Failed to make a backup file
        //

        mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return FALSE;
    }

    // Delete HTML file
    if(!DeleteFile(lpHtmlFile))
    {
        //
        // TODO: Display corresponding error message
        // -> Failed to replace HTML file
        //

        mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return FALSE;
    }

    // Replace temporary with HTML file
    if(!MoveFile(TmpFileName,lpHtmlFile))
    {
        //
        // TODO: Display corresponding error message
        // -> Failed to rename backup file
        //

        mHpMessage(hWndActive,hmHpPakInst,IDS_OPENHLP_OPEN,NULL,0,0,MB_ICONSTOP);
		return FALSE;
    }

    // Delete backup file
    if(!DeleteFile(lpBackup))
	    mHpMessage(hWndActive,hmHpPakInst,IDS_DELETE_MHPFILE,NULL,0,0,MB_ICONSTOP); // TODO: Failed to delete backup file

    return TRUE;
}

//****************************************************************************************************************AUTOASS//
// mHpPak_GetAutoAssRow
//-----------------------------------------------------//
MHPPAK_API int mHpPak_GetAutoAssRow(char* lpAutoAss,int iPos)
{	int iRang=0,iRes=0,index=0;
	while(lpAutoAss[index]!=END_LIST)
	{	if(lpAutoAss[index]==CHR_SEPARATED_AUTOASS)
		{	if(iRang!=iPos)
			{	iRang++;
				iRes=0;
			}
			else return iRes;
		}
		else
		{	iRes+=mHpPak_CharToInt(lpAutoAss[index]);
			if((lpAutoAss[(index+1)]!=END_LIST)&&(lpAutoAss[(index+1)]!=CHR_SEPARATED_AUTOASS)) iRes*=10;
		}
		index++;
	}
	if(iRang!=iPos) return NO;
	return iRes;
}

//-----------------------------------------------------//
// mHpPak_ChangeAutoAss
//-----------------------------------------------------//
MHPPAK_API void mHpPak_ChangeAutoAss(char* lpmHpFileName,int FileSize,int iPosition,int &iMode,int* IndexTab,int IndexSize,
									 int &iNbBoucle)
{	int index=0;
	mHpPak_bAutoAss=FALSE;
	mHpPak_iFavIndex=mHpPak_GetAutoAssRow(mHpPak_FavsList->GetAutoAssist(),iPosition);
	mHpPak_iFavIndex--;
	if((mHpPak_iFavIndex>=0)&&(mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)!=NULL))
	{	iMode=1;
		mHpPak_bAutoAss=TRUE;
		mHpPak_EmptyTab(IndexTab,IndexSize);
		while(mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->iRefTab[index]!=NO)
		{	mHpPak_AddIndex(IndexTab,mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->iRefTab[index]);
			index++;
		}
		if(mHpPak_FavsList->GetItemFromRang(mHpPak_iFavIndex)->FavItem->ParamList!=NULL) mHpPak_AddIndex(IndexTab,0);
		iNbBoucle=mHpPak_FavsList->GetLoopOption(mHpPak_iFavIndex);
		mHpPak_InitStr(lpmHpFileName,FileSize);
		strncpy(lpmHpFileName,mHpPak_FavsList->GetmHpPath(mHpPak_iFavIndex),FileSize);
	}
}

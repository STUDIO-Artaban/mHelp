// EditRich.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <RichEdit.h>

//-------------------------------//
#include "mHp2D.h"
#include "resource.h"

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif

//-----------------------------------------------------//
// C/C++ control class name
#define		CPP_TASK_RICHEDIT			"RICHEDIT"

//-----------------------------------------------------//
// VB control class name
#define		VB_TASK_RICHEDIT			"RichTextWndClass"

//-----------------------------------------------------------------------------------------------------------------------//
// Macros

//-----------------------------------------------------------------------------------------------------------------------//
// Varaibles

//-----------------------------------------------------------------------------------------------------------------------//
// Déclaration

//****************************************************************************************************************MHPFILE//
// RichEdit_ReadEvent
//---------------------------------------------------//
BOOL RichEdit_ReadEvent(HWND hWndActive)
{	
	return FALSE; // OK
}

//********************************************************************************************************************ADD//
// RichEdit_AddEvent
//---------------------------------------------------//
void RichEdit_AddEvent(HWND hWndActive,UINT iTaskID,int iCurWnd,int iCntWnd,BOOL bWrite)
{	
}

//**************************************************************************************************************CLASSNAME//
// RichEdit_IsClassName
//---------------------------------------------------//
BOOL RichEdit_IsClassName(char* CtrlClass,int ClassSize)
{	if((!strncmp(CtrlClass,CPP_TASK_RICHEDIT,ClassSize))||(!strncmp(CtrlClass,VB_TASK_RICHEDIT,ClassSize)))
	{	return TRUE;}
	return FALSE;
}

//*************************************************************************************************************MHPPROCESS//
// RichEdit_mHpProcess
//---------------------------------------------------//
void RichEdit_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
	{	case 2:
		{	break;
		}
		case 7:
		{	if(GetmHpVarStruct()->TaskList->NextCellule())
			{	GetmHpVarStruct()->bNewDepl=TRUE;
				SetDDrawObj(hWndActive,0);
				GetmHpVarStruct()->CourantTask=GetmHpVarStruct()->TaskList->GetCurrentTask();
				GetNextPos(hWndActive);
			}
			else NextRepeatAssistProc(hWndActive);
			break;
		}
		default: NULL;
	}
}

//---------------------------------------------------//
// RichEdit_StartProc
//---------------------------------------------------//
BOOL RichEdit_StartProc(HWND hWndActive)
{	
	return TRUE;
}

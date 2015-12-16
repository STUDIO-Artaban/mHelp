// mHpMsg.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include "mHpMsg.h"

//-----------------------------------------------------//
#include "mHpPak.h"

//-----------------------------------------------------//
// Variables
BOOL bViewMsg=TRUE;

//----------------------------------------------------------------------------------------------------------------GENERAL//
// mHpMessage
//-----------------------------------------------------//
int mHpMessage(HWND hWnd,HINSTANCE hInstance,UINT StringIDA,char* lpMid,int iMidSize,UINT StringIDB,UINT BoxStyle)
{	if(bViewMsg)
    {   int StringSize;
	    char szBuff[MAX_RS_SIZE]={0};
	    char szTmp[MAX_RS_SIZE]={0};
	    if((StringIDA!=0)&&(lpMid!=NULL)&&(StringIDB!=0))
	    {	StringSize=LoadString(hInstance,StringIDA,szBuff,MAX_RS_SIZE);
		    strncat(szBuff,lpMid,mHpPak_FindStrLenght(lpMid,iMidSize));
		    StringSize=LoadString(hInstance,StringIDB,szTmp,MAX_RS_SIZE);
		    strncat(szBuff,szTmp,mHpPak_FindStrLenght(szTmp,MAX_RS_SIZE));
	    }
	    else if((StringIDA!=0)&&(lpMid!=NULL))
	    {	StringSize=LoadString(hInstance,StringIDA,szBuff,MAX_RS_SIZE);
		    strncat(szBuff,lpMid,mHpPak_FindStrLenght(lpMid,iMidSize));
	    }
	    else if((lpMid!=NULL)&&(StringIDB!=0))
	    {	strncpy(szBuff,lpMid,iMidSize);
		    StringSize=LoadString(hInstance,StringIDB,szTmp,MAX_RS_SIZE);
		    strncat(szBuff,szTmp,mHpPak_FindStrLenght(szTmp,MAX_RS_SIZE));
	    }
	    else
	    {	StringSize=LoadString(hInstance,StringIDA,szBuff,MAX_RS_SIZE);
		    StringSize=LoadString(hInstance,StringIDB,szTmp,MAX_RS_SIZE);
		    strncat(szBuff,szTmp,mHpPak_FindStrLenght(szTmp,MAX_RS_SIZE));
	    }
	    return MessageBox(hWnd,szBuff,MHPPAK_DLGMSGTITLE,BoxStyle);
    }
	return 0;
}

//-----------------------------------------------------//
// SetViewFlag
//-----------------------------------------------------//
void SetViewFlag(BOOL bNewFlag)
{	bViewMsg=bNewFlag;}

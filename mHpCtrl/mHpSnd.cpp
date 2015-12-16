// mHpSnd.cpp
//

#include "stdafx.h"
#include "mHpSnd.h"
#include <mmsystem.h>

#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif

#include "resource.h"

//-----------------------------------------------------------------------------------------------------------------------//
// PlayResourceWAV
//-----------------------------------------------------//
BOOL PlayResourceWAV(char* lpRcWav)
{	BOOL bRes;
	LPSTR lpResource;
	HANDLE hResInfo,hRes;
	if((GetmHpVarStruct()->imHpMode==1)||(GetmHpVarStruct()->iSkipSnd!=0))
	{	if(GetmHpVarStruct()->iSkipSnd!=0) GetmHpVarStruct()->iSkipSnd--;
		return FALSE;
	}
	// Find the WAVE resource.
	hResInfo=FindResource(GetmHpVarStruct()->hmHpCtrlInst,lpRcWav,"WAVE");
	if(hResInfo==NULL) return FALSE;
	// Load the WAVE resource.
	hRes=(HANDLE)LoadResource(GetmHpVarStruct()->hmHpCtrlInst,(HRSRC)hResInfo);
	if(hRes==NULL) return FALSE;
	// Lock the WAVE resource and play it.
	lpResource=(LPSTR)LockResource(hRes);
	if(lpResource!=NULL)
	{	bRes=sndPlaySound(lpResource,SND_MEMORY|SND_SYNC|SND_NODEFAULT);
		UnlockResource(hRes);
	}
	else bRes=FALSE;
	// Free the WAVE resource and return success or failure.
	FreeResource(hRes);
	return bRes;
}

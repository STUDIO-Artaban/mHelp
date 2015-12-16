// CtrlMan.cpp
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "CtrlMan.h"

//---------------------------------------------//
#include "mHp2D.h"
#include "resource.h"

#ifndef  __MHPSOUND_H
 #include "mHpSnd.h"
#endif

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

//---------------------------------------------//
// Définitions
#define CTRL_MAN_PLATFORMKEY            "CtrlPlatform"
#define CTRL_MAN_CLASSKEY               "CtrlClass"
#define CTRL_MAN_STYLEKEY               "CtrlStyle"
#define CTRL_MAN_MSGIDKEY               "CaptMsgID"

//---------------------------------------------//
// Macros

//---------------------------------------------//
// Déclarations
void EncodeString(char*,int,const char*);
BOOL WriteCtrlIDTask(HWND,CEventListe*,int);
BOOL ReadCtrlIDTask(HWND,ImHpCtrlMgmt*,UINT,char&);

//---------------------------------------------//
// Variables
HWND hWndUI=NULL;

// Add encoded string into string array
void EncodeString(char* lpStrArray,int nMaxSize,const char* lpVal)
{   // e.g. "Lis^tSel1" - "Li_s\tSel2" - "ListSe¤l^3" -> "Lis\^tSel1^Li_s\\tSel2^ListSe¤l\^3"
    char lpEncodeStr[MAX_RS_SIZE]={0};
    mHpPak_WriteString(lpEncodeStr,MAX_RS_SIZE,lpVal);
    if(*lpStrArray)
        strncat(lpStrArray,"^",1);
    strncat(lpStrArray,lpEncodeStr,nMaxSize);
}

// Write control ID in file
BOOL WriteCtrlIDTask(HWND hWndActive,CEventListe* EvntObj,int Index)
{
    CCtrlID* CurCtrlID=EvntObj->GetmHpTask(Index,FALSE)->GetCtrlMgmt()->GetCtrlID();
    // Check valid control ID
    if(!CurCtrlID)
        return FALSE;
    ////// Control ID
    switch(CurCtrlID->GetType())
    {   case CData::Integer: // INTEGER ///////////////////////////////
        {
            const int* iID=(const int*)CurCtrlID->GetValue();
            if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,*iID,NULL,FALSE)) // '¤'
            {   return FALSE;
            }
            break;
        }
        case CData::String: // STRING /////////////////////////////////
        {
            const char* lpID=(const char*)CurCtrlID->GetValue();
            if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,lpID,TRUE)) // '¤'
            {   return FALSE;
            }
            break;
        }
        case CData::IntArray: // INTEGER ARRAY ////////////////////////
        {
            UINT iIdIdx=0;
            const int* iID=(const int*)CurCtrlID->GetValue(iIdIdx);
            while(iID!=NULL)
            {   if(!iIdIdx)
                {   if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,*iID,NULL,FALSE)) // '¤'
                    {   return FALSE;
                    }
                }
                else if(!mHpPak_WriteParty(hWndActive,ITEM_GROUP,*iID,NULL,FALSE)) // '^'
                {   return FALSE;
                }
                iID=(const int*)CurCtrlID->GetValue(++iIdIdx);
            }
            break;
        }
        case CData::StrArray: // STRING ARRAY /////////////////////////
        {
            UINT iIdIdx=0;
            char lpIdStr[MAX_RS_SIZE]={0};
            const char* lpID=(const char*)CurCtrlID->GetValue(iIdIdx);
            while(lpID!=NULL)
            {   EncodeString(lpIdStr,MAX_RS_SIZE,lpID);
                lpID=(const char*)CurCtrlID->GetValue(++iIdIdx);
            }
            if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,lpIdStr,FALSE)) // '¤'
            {   return FALSE;
            }
            break;
        }
        case CData::Undefined: // UNDEFINED ///////////////////////////
        {   return FALSE; }
    }
    return TRUE;
}

BOOL ReadCtrlIDTask(HWND hWndActive,ImHpCtrlMgmt* pCtrlMgmt,UINT iPlatformID,char &chNext)
{
    // Check control platform
    switch(iPlatformID)
    {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
        case PLATFORM_ID_WPF: ////// WPF Platform
        {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlMgmt);
            pDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataID,FALSE);
            if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
            {
                assert(NULL);

                //
                // TODO: Display DotNET error message
                // -> Stop mHelp processus
                //

                pDotNetCtrl->GetLastErr();
                return FALSE;
            }
            break;
        }
        case PLATFORM_ID_QT: ////// Qt Platform
        {   if(!pCtrlMgmt->FillQtData())
            {
                assert(NULL);

                //
                // TODO: Display error message
                // -> Stop mHelp processus
                //

                return FALSE;
            }
            break;
        }
    }

    // Check valid empty control ID (that defines control ID type)
    if(!pCtrlMgmt->GetCtrlID())
        return FALSE;
    ////// Control ID
    switch(pCtrlMgmt->GetCtrlID()->GetType())
    {   case CData::Integer: // INTEGER ///////////////////////////////
        {
            int iID=0;
            char lpTmp[MAX_RS_SIZE]={0};
            mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iID,lpTmp,MAX_RS_SIZE,chNext,FALSE);
            if(iID!=NO)
                pCtrlMgmt->GetCtrlID()->SetValue(iID);
            else
                return FALSE;
            break;
        }
        case CData::String: // STRING /////////////////////////////////
        {
            int iTmp=0;
            char lpID[MAX_RS_SIZE]={0};
            mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpID,MAX_RS_SIZE,chNext,TRUE);
            if(iTmp==NO)
                return FALSE;
            pCtrlMgmt->GetCtrlID()->SetValue(lpID);
            break;
        }
        case CData::IntArray: // INTEGER ARRAY ////////////////////////
        {
            do
            {   int iID=0;
                char lpTmp[MAX_RS_SIZE]={0};
                chNext=NEXT_RS; // '|': Force to get integer value (see mHpPak_GetEventPart)
                mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iID,lpTmp,MAX_RS_SIZE,chNext,FALSE);
                if(iID!=NO)
                    pCtrlMgmt->GetCtrlID()->AddValue(iID);
                else
                    return FALSE;

            } while(chNext==ITEM_GROUP); // '^'
            break;
        }
        case CData::StrArray: // STRING ARRAY /////////////////////////
        {
            chNext=ITEM_GROUP; // Exit at next ITEM_GROUP found (see mHpPak_GetEventPart)
            while(chNext==ITEM_GROUP)
            {   int iTmp=0;
                char lpID[MAX_RS_SIZE]={0};
                mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpID,MAX_RS_SIZE,chNext,TRUE);
                if(iTmp!=NO)
                    pCtrlMgmt->GetCtrlID()->AddValue(lpID);
                else
                    return FALSE;
            }
            break;
        }
        case CData::Undefined: // UNDEFINED ///////////////////////////
        {   return FALSE; }
    }

    // Check control platform
    switch(iPlatformID)
    {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
        case PLATFORM_ID_WPF: ////// WPF Platform
        {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlMgmt);
            // Update DotNET data
            pDotNetCtrl->UpdateDotNetData(CmHpDotNetCtrl::DataID);
            if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
            {
                assert(NULL);

                //
                // TODO: Display DotNET error message
                // -> Stop mHelp processus
                //

                pDotNetCtrl->GetLastErr();
                return FALSE;
            }
            break;
        }
        case PLATFORM_ID_QT: ////// Qt Platform
        {   if(!pCtrlMgmt->UpdateQtData())
            {
                assert(NULL);

                //
                // TODO: Display error message
                // -> Stop mHelp processus
                //

                return FALSE;
            }
            break;
        }
    }
    return TRUE;
}

//********************************************************************OBJECTS//
// class CCtrlManTask
CCtrlManTask::CCtrlManTask(char* TaskMsg,char* StateMsg,int CurWnd,int CntWnd,int Option,ImHpCtrlMgmt* pCtrlMgmt,UINT iPlatformID)
 			 :CmHpTask(MHPID_CTRL_MAN,TaskMsg,StateMsg,CurWnd,CntWnd,Option,NULL,NULL)
{   mHpCtrlMgmt=pCtrlMgmt;
    mHpCtrlPlatformID=iPlatformID;
}
CCtrlManTask::CCtrlManTask()
 			 :CmHpTask()
{   mHpCtrlMgmt=NULL;
    mHpCtrlPlatformID=0;
}
CCtrlManTask::~CCtrlManTask()
{   if(mHpCtrlMgmt)
        delete mHpCtrlMgmt;
}

//////
BOOL CCtrlManTask::ReadCtrlIDKey(HKEY hTaskKey)
{
    // Check control platform
    switch(mHpCtrlPlatformID)
    {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
        case PLATFORM_ID_WPF: ////// WPF Platform
        {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(mHpCtrlMgmt);
            pDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataID,FALSE);
            if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
            {
                assert(NULL);

                //
                // TODO: Display DotNET error message
                // -> Stop mHelp processus
                //

                pDotNetCtrl->GetLastErr();
                return FALSE;
            }
            break;
        }
        case PLATFORM_ID_QT: ////// Qt Platform
        {   if(!mHpCtrlMgmt->FillQtData())
            {
                assert(NULL);

                //
                // TODO: Display error message
                // -> Stop mHelp processus
                //

                return FALSE;
            }
            break;
        }
    }

    // Check control ID
    if(!mHpCtrlMgmt->GetCtrlID())
        return FALSE;
    DWORD Size=MAX_KEY_SIZE;
    DWORD Type=REG_DWORD;
    HKEY mHpCtrlKey=NULL;
    if(RegOpenKey(hTaskKey,MHPCTRLID_KEY,&mHpCtrlKey)!=ERROR_SUCCESS)
        return FALSE;
    ////// Control ID
    switch(mHpCtrlMgmt->GetCtrlID()->GetType())
    {   case CData::Integer: // INTEGER ///////////////////////////////
        {
            BYTE oTab[MAX_KEY_SIZE]={0};
            if(RegQueryValueEx(mHpCtrlKey,MHPCTRLID_KEY,NULL,&Type,oTab,&Size)!=ERROR_SUCCESS)
                return FALSE;
            int iID;
            mHpPak_FillIntFromData(oTab,iID,FALSE);
            mHpCtrlMgmt->GetCtrlID()->SetValue(iID);
            break;
        }
        case CData::String: // STRING /////////////////////////////////
        {
            char lpID[MAX_RS_SIZE]={0};
            if(!mHpPak_ReadLongStrRegVal(mHpCtrlKey,mHpCtrlMgmt->GetCtrlID()->GetName(),lpID,MAX_RS_SIZE))
                return FALSE;
            mHpCtrlMgmt->GetCtrlID()->SetValue(lpID);
            break;
        }
        case CData::IntArray: // INTEGER ARRAY ////////////////////////
        {
            Type=REG_DWORD;
            Size=MAX_KEY_SIZE;
            HKEY hIdIdxKey=NULL;
            int iIdIdx=0;
            char lpIdIdx[MAX_KEY_SIZE]={0};
            _snprintf(lpIdIdx,MAX_KEY_SIZE,"%d",iIdIdx);
            while(RegOpenKey(mHpCtrlKey,lpIdIdx,&hIdIdxKey)==ERROR_SUCCESS)
            {   BYTE oTab[MAX_KEY_SIZE]={0};
                if(RegQueryValueEx(hIdIdxKey,mHpCtrlMgmt->GetCtrlID()->GetName(),NULL,&Type,oTab,&Size)==ERROR_SUCCESS)
                {   int iID=0;
                    mHpPak_FillIntFromData(oTab,iID,FALSE);
                    mHpCtrlMgmt->GetCtrlID()->AddValue(iID);
                }
                else
                    return FALSE;
                Type=REG_DWORD;
                Size=MAX_KEY_SIZE;
                mHpPak_InitStr(lpIdIdx,MAX_KEY_SIZE);
                _snprintf(lpIdIdx,MAX_KEY_SIZE,"%d",++iIdIdx);
                RegCloseKey(hIdIdxKey);
            }
            break;
        }
        case CData::StrArray: // STRING ARRAY /////////////////////////
        {
            HKEY hIdIdxKey=NULL;
            int iIdIdx=0;
            char lpIdIdx[MAX_KEY_SIZE]={0};
            _snprintf(lpIdIdx,MAX_KEY_SIZE,"%d",iIdIdx);
            while(RegOpenKey(mHpCtrlKey,lpIdIdx,&hIdIdxKey)==ERROR_SUCCESS)
            {   char lpID[MAX_RS_SIZE]={0};
                if(!mHpPak_ReadLongStrRegVal(mHpCtrlKey,mHpCtrlMgmt->GetCtrlID()->GetName(),lpID,MAX_RS_SIZE))
                    return FALSE;
                mHpCtrlMgmt->GetCtrlID()->AddValue(lpID);
                mHpPak_InitStr(lpIdIdx,MAX_KEY_SIZE);
                _snprintf(lpIdIdx,MAX_KEY_SIZE,"%d",++iIdIdx);
                RegCloseKey(hIdIdxKey);
            }
            break;
        }
        case CData::Undefined: // UNDEFINED ///////////////////////////
        {   return FALSE; }
    }

    // Check control platform
    switch(mHpCtrlPlatformID)
    {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
        case PLATFORM_ID_WPF: ////// WPF Platform
        {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(mHpCtrlMgmt);
            // Update DotNET data
            pDotNetCtrl->UpdateDotNetData(CmHpDotNetCtrl::DataID);
            if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
            {
                assert(NULL);

                //
                // TODO: Display DotNET error message
                // -> Stop mHelp processus
                //

                pDotNetCtrl->GetLastErr();
                return FALSE;
            }
            break;
        }
        case PLATFORM_ID_QT: ////// Qt Platform
        {   if(!mHpCtrlMgmt->UpdateQtData())
            {
                assert(NULL);

                //
                // TODO: Display error message
                // -> Stop mHelp processus
                //

                return FALSE;
            }
            break;
        }
    }
    return TRUE;
}

BOOL CCtrlManTask::WriteCtrlIDKey(HKEY hTaskKey) const
{
    // Check control ID
    if(!mHpCtrlMgmt->GetCtrlID())
        return FALSE;
    SECURITY_ATTRIBUTES SecAtt;
    BOOL bAclRes;
    mHpPak_GetSecurityAtt(SecAtt,bAclRes);
    DWORD dWord=REG_OPENED_EXISTING_KEY;
    // mHpCtrlID
    HKEY hmHpCtrlKey=NULL;
    if(RegCreateKeyEx(hTaskKey,MHPCTRLID_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
                      KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                      &hmHpCtrlKey,&dWord)!=ERROR_SUCCESS) return FALSE;
    ////// Control ID
    switch(mHpCtrlMgmt->GetCtrlID()->GetType())
    {   case CData::Integer: // INTEGER ///////////////////////////////
        {
            HKEY hIdKey=NULL;
            if(RegCreateKeyEx(hmHpCtrlKey,MHPCTRLID_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
                              KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                              &hIdKey,&dWord)!=ERROR_SUCCESS) return FALSE;
            const int* iIdVal=(const int*)mHpCtrlMgmt->GetCtrlID()->GetValue();
            BYTE oTab[MAX_KEY_SIZE]={0};
            int size=mHpPak_FillDataFromNumber(oTab,*iIdVal,FALSE);
            RegSetValueEx(hIdKey,mHpCtrlMgmt->GetCtrlID()->GetName(),0,REG_DWORD,oTab,size);
            RegCloseKey(hIdKey);
            break;
        }
        case CData::String: // STRING /////////////////////////////////
        {   
            const char* lpIdVal=(const char*)mHpCtrlMgmt->GetCtrlID()->GetValue();
            if(!mHpPak_WriteLongStrRegVal(hmHpCtrlKey,MHPCTRLID_KEY,
                                          mHpCtrlMgmt->GetCtrlID()->GetName(),lpIdVal))
            {   return FALSE;
            }
            break;
        }
        case CData::IntArray: // INTEGER ARRAY ////////////////////////
        {
            UINT iIdIdx=0;
            char lpIdIdx[MAX_KEY_SIZE]={0};
            _snprintf(lpIdIdx,MAX_KEY_SIZE,"%d",iIdIdx);
            const int* iIdVal=(const int*)mHpCtrlMgmt->GetCtrlID()->GetValue(iIdIdx);
            while(iIdVal!=NULL)
            {   HKEY hTmpKey=NULL;
                if(RegCreateKeyEx(hmHpCtrlKey,lpIdIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                  KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                  &hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                HKEY hIdIdxKey=NULL;
                if(RegCreateKeyEx(hmHpCtrlKey,lpIdIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                  KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                  &hIdIdxKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                BYTE oTab[MAX_KEY_SIZE]={0};
                int size=mHpPak_FillDataFromNumber(oTab,*iIdVal,FALSE);
                RegSetValueEx(hIdIdxKey,mHpCtrlMgmt->GetCtrlID()->GetName(),0,REG_DWORD,oTab,size);
                RegCloseKey(hTmpKey);
                RegCloseKey(hIdIdxKey);
                mHpPak_InitStr(lpIdIdx,MAX_KEY_SIZE);
                _snprintf(lpIdIdx,MAX_KEY_SIZE,"%d",++iIdIdx);
                iIdVal=(const int*)mHpCtrlMgmt->GetCtrlID()->GetValue(iIdIdx);
            }
            break;
        }
        case CData::StrArray: // STRING ARRAY /////////////////////////
        {
            UINT iIdIdx=0;
            char lpIdIdx[MAX_KEY_SIZE]={0};
            _snprintf(lpIdIdx,MAX_KEY_SIZE,"%d",iIdIdx);
            const char* lpIdVal=(const char*)mHpCtrlMgmt->GetCtrlID()->GetValue(iIdIdx);
            while(lpIdVal!=NULL)
            {   HKEY hIdIdxKey=NULL;
                if(RegCreateKeyEx(hmHpCtrlKey,lpIdIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                  KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                  &hIdIdxKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                if(!mHpPak_WriteLongStrRegVal(hmHpCtrlKey,lpIdIdx,
                                              mHpCtrlMgmt->GetCtrlID()->GetName(),lpIdVal))
                {   return FALSE;
                }
                RegCloseKey(hIdIdxKey);
                mHpPak_InitStr(lpIdIdx,MAX_KEY_SIZE);
                _snprintf(lpIdIdx,MAX_KEY_SIZE,"%d",++iIdIdx);
                lpIdVal=(const char*)mHpCtrlMgmt->GetCtrlID()->GetValue(iIdIdx);
            }
            break;
        }
        case CData::Undefined: // UNDEFINED ///////////////////////////
        {   return FALSE; }
    }
    return TRUE;
}

BOOL CCtrlManTask::ReadTaskKey(int index)
{   HKEY hSubTaskKey=NULL,hTaskKey=NULL;
    if(!CmHpTask::ReadCommonKey(index,TRUE,hSubTaskKey,hTaskKey))
        return FALSE;
    // Platform
    DWORD Size=MAX_KEY_SIZE;
    DWORD Type=REG_DWORD;
    BYTE oTab[MAX_KEY_SIZE]={0};
    if(RegQueryValueEx(hTaskKey,CTRL_MAN_PLATFORMKEY,NULL,&Type,oTab,&Size)!=ERROR_SUCCESS)
        return FALSE;
    int iKeyPlatformID;
    mHpPak_FillIntFromData(oTab,iKeyPlatformID,FALSE);
    // Class name
    char lpKeyClass[MAX_KEY_SIZE]={0};
	if(!mHpPak_ReadLongStrRegVal(hTaskKey,CTRL_MAN_CLASSKEY,lpKeyClass,MAX_KEY_SIZE))
        return FALSE;
    // Style
    Size=MAX_KEY_SIZE;Type=REG_DWORD;
    if(RegQueryValueEx(hTaskKey,CTRL_MAN_STYLEKEY,NULL,&Type,oTab,&Size)!=ERROR_SUCCESS)
        return FALSE;
    int iKeyStyle;
    mHpPak_FillIntFromData(oTab,iKeyStyle,FALSE);
    // Message ID
    if(RegQueryValueEx(hTaskKey,CTRL_MAN_MSGIDKEY,NULL,&Type,oTab,&Size)!=ERROR_SUCCESS)
        return FALSE;
    int iMsgID;
    mHpPak_FillIntFromData(oTab,iMsgID,FALSE);
    // mHpCtrlMgmt
    mHpCtrlMgmt=GetmHpVarStruct()->CtrlMgmtList->ReadEvent(lpKeyClass,iKeyPlatformID);
    if((!mHpCtrlMgmt)||(!mHpCtrlMgmt->SetStyle(iKeyStyle))||(!mHpCtrlMgmt->SetCaptMsgID(iMsgID)))
        return FALSE;
    // Set control platform
    mHpCtrlPlatformID=iKeyPlatformID;
    // Fill control class name to mHpCtrlMgmt (no CaptureMessage call that fill it)
    mHpCtrlMgmt->SetCtrlClass(lpKeyClass);
    // Unique control ID
    if(!ReadCtrlIDKey(hTaskKey))
        return FALSE;
    // Control name: Check existing state(s)
    if(RegOpenKeyEx(hTaskKey,mHpCtrlMgmt->GetCtrlName(),0,KEY_READ,&hTaskKey)==ERROR_SUCCESS)
    {   if(!mHpCtrlMgmt->GetStateCount())
            return FALSE; // Key above should not be defined

        // Check control platform
        switch(mHpCtrlPlatformID)
        {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(mHpCtrlMgmt);
                pDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataState,FALSE);
                if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display DotNET error message
                    // -> Stop mHelp processus
                    //

                    pDotNetCtrl->GetLastErr();
                    return FALSE;
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {   if(!mHpCtrlMgmt->FillQtData())
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus
                    //

                    return FALSE;
                }
                break;
            }
        }

        ////// State
        for(UINT i=0; i<mHpCtrlMgmt->GetStateCount(); i++)
        {
            // Check existing state
            if(!mHpCtrlMgmt->GetState(i))
                return FALSE; // Inconsistent data
            // Check state type
            switch(mHpCtrlMgmt->GetState(i)->GetType())
            {   case CData::Integer: // INTEGER ///////////////////////////////
                {   
                    if(RegQueryValueEx(hTaskKey,mHpCtrlMgmt->GetState(i)->GetName(),NULL,&Type,oTab,&Size)!=ERROR_SUCCESS)
                        return FALSE;
                    int iState;
                    mHpPak_FillIntFromData(oTab,iState,FALSE);
                    mHpCtrlMgmt->GetState(i)->SetValue(iState);
                    break;
                }
                case CData::String: // STRING /////////////////////////////////
                {   
                    char lpState[MAX_RS_SIZE]={0};
                    if(!mHpPak_ReadLongStrRegVal(hTaskKey,mHpCtrlMgmt->GetState(i)->GetName(),lpState,MAX_RS_SIZE))
                        return FALSE;
                    mHpCtrlMgmt->GetState(i)->SetValue(lpState);
                    break;
                }
                case CData::IntArray: // INTEGER ARRAY ////////////////////////
                {
                    HKEY hStateKey=NULL;
                    if(RegOpenKey(hTaskKey,mHpCtrlMgmt->GetState(i)->GetName(),&hStateKey)==ERROR_SUCCESS)
                    {   Type=REG_DWORD;
                        Size=MAX_KEY_SIZE;
                        HKEY hStateIdxKey=NULL;
                        int iStateIdx=0;
                        char lpStateIdx[MAX_KEY_SIZE]={0};
                        _snprintf(lpStateIdx,MAX_KEY_SIZE,"%d",iStateIdx);
                        while(RegOpenKey(hStateKey,lpStateIdx,&hStateIdxKey)==ERROR_SUCCESS)
                        {   if(RegQueryValueEx(hStateIdxKey,mHpCtrlMgmt->GetState(i)->GetName(),NULL,&Type,oTab,&Size)==ERROR_SUCCESS)
                            {   int iState=0;
                                mHpPak_FillIntFromData(oTab,iState,FALSE);
                                mHpCtrlMgmt->GetState(i)->AddValue(iState);
                            }
                            else
                                return FALSE;
                            Type=REG_DWORD;
                            Size=MAX_KEY_SIZE;
                            mHpPak_InitStr(lpStateIdx,MAX_KEY_SIZE);
                            _snprintf(lpStateIdx,MAX_KEY_SIZE,"%d",++iStateIdx);
                            RegCloseKey(hStateIdxKey);
                        }
                        RegCloseKey(hStateKey);
                    }
			        break;
                }
                case CData::StrArray: // STRING ARRAY /////////////////////////
                {
                    HKEY hStateKey=NULL;
                    if(RegOpenKey(hTaskKey,mHpCtrlMgmt->GetState(i)->GetName(),&hStateKey)==ERROR_SUCCESS)
                    {   HKEY hStateIdxKey=NULL;
                        int iStateIdx=0;
                        char lpStateIdx[MAX_KEY_SIZE]={0};
                        _snprintf(lpStateIdx,MAX_KEY_SIZE,"%d",iStateIdx);
                        while(RegOpenKey(hStateKey,lpStateIdx,&hStateIdxKey)==ERROR_SUCCESS)
                        {   char lpState[MAX_RS_SIZE]={0};
                            if(!mHpPak_ReadLongStrRegVal(hStateIdxKey,mHpCtrlMgmt->GetState(i)->GetName(),lpState,MAX_RS_SIZE))
                                return FALSE;
                            mHpCtrlMgmt->GetState(i)->AddValue(lpState);
                            mHpPak_InitStr(lpStateIdx,MAX_KEY_SIZE);
                            _snprintf(lpStateIdx,MAX_KEY_SIZE,"%d",++iStateIdx);
                            RegCloseKey(hStateIdxKey);
                        }
                        RegCloseKey(hStateKey);
                    }
			        break;
                }
                case CData::Undefined: // UNDEFINED ///////////////////////////
                {   return FALSE; }
            }
        }

        // Check control platform
        switch(mHpCtrlPlatformID)
        {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(mHpCtrlMgmt);
                // Update DotNET data
                pDotNetCtrl->UpdateDotNetData(CmHpDotNetCtrl::DataState);
                if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display DotNET error message
                    // -> Stop mHelp processus
                    //

                    pDotNetCtrl->GetLastErr();
                    return FALSE;
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {   if(!mHpCtrlMgmt->UpdateQtData())
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus
                    //

                    return FALSE;
                }
                break;
            }
        }
        return TRUE;
    }
    return (!mHpCtrlMgmt->GetStateCount())? TRUE:FALSE;
}

BOOL CCtrlManTask::WriteTaskKey()
{   if(!mHpCtrlMgmt)
        return FALSE;
    HKEY hSubKey=NULL,hKey=NULL;
    if(!CmHpTask::WriteCommonKey(TRUE,hSubKey,hKey))
        return FALSE;
    // Unique control ID
    if(!WriteCtrlIDKey(hKey))
        return FALSE;
    // Platform
    SECURITY_ATTRIBUTES SecAtt;
    BOOL bAclRes;
    mHpPak_GetSecurityAtt(SecAtt,bAclRes);
    HKEY hTmpKey;
    BYTE oTab[MAX_KEY_SIZE]={0};
    DWORD dWord=REG_OPENED_EXISTING_KEY;
    if(RegCreateKeyEx(hSubKey,MHPTASK_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
                        KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                        &hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
    int size=mHpPak_FillDataFromNumber(oTab,mHpCtrlPlatformID,FALSE);
    RegSetValueEx(hTmpKey,CTRL_MAN_PLATFORMKEY,0,REG_DWORD,oTab,size);
    // Class name
    if(!mHpPak_WriteLongStrRegVal(hSubKey,MHPTASK_KEY,CTRL_MAN_CLASSKEY,mHpCtrlMgmt->GetCtrlClass()))
        return FALSE;
    // Style
    size=mHpPak_FillDataFromNumber(oTab,mHpCtrlMgmt->GetStyle(),FALSE);
    RegSetValueEx(hTmpKey,CTRL_MAN_STYLEKEY,0,REG_DWORD,oTab,size);
    // Message ID
    size=mHpPak_FillDataFromNumber(oTab,mHpCtrlMgmt->GetCaptMsgID(),FALSE);
    RegSetValueEx(hTmpKey,CTRL_MAN_MSGIDKEY,0,REG_DWORD,oTab,size);
    RegCloseKey(hTmpKey);
    // Check existing state(s)
    if(mHpCtrlMgmt->GetStateCount())
    {   // Control name
        HKEY hCtrlKey=NULL;
        if(RegCreateKeyEx(hKey,mHpCtrlMgmt->GetCtrlName(),0,NULL,REG_OPTION_NON_VOLATILE,
                          KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                          &hCtrlKey,&dWord)!=ERROR_SUCCESS) return FALSE;

        ////// State
        for(UINT i=0; i<mHpCtrlMgmt->GetStateCount(); i++)
        {
            // Check existing state
            if(!mHpCtrlMgmt->GetState(i))
                return FALSE; // Inconsistent data
            // Check state type
            switch(mHpCtrlMgmt->GetState(i)->GetType())
            {   case CData::Integer: // INTEGER ///////////////////////////////
                {
                    const int* iStateVal=(const int*)mHpCtrlMgmt->GetState(i)->GetValue();
                    int size=mHpPak_FillDataFromNumber(oTab,*iStateVal,FALSE);
                    RegSetValueEx(hCtrlKey,mHpCtrlMgmt->GetState(i)->GetName(),0,REG_DWORD,oTab,size);
                    break;
                }
                case CData::String: // STRING /////////////////////////////////
                {   
                    const char* lpStateVal=(const char*)mHpCtrlMgmt->GetState(i)->GetValue();
                    if(!mHpPak_WriteLongStrRegVal(hCtrlKey,mHpCtrlMgmt->GetCtrlName(),
                                                  mHpCtrlMgmt->GetState(i)->GetName(),lpStateVal))
                    {   return FALSE;
                    }
                    break;
                }
                case CData::IntArray: // INTEGER ARRAY ////////////////////////
                {
                    UINT iStateIdx=0;
                    char lpStateIdx[MAX_KEY_SIZE]={0};
                    _snprintf(lpStateIdx,MAX_KEY_SIZE,"%d",iStateIdx);
                    const int* iStateVal=(const int*)mHpCtrlMgmt->GetState(i)->GetValue(iStateIdx);
                    while(iStateVal!=NULL)
                    {   if(RegCreateKeyEx(hCtrlKey,lpStateIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                          KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                          &hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                        HKEY hStateIdxKey=NULL;
                        if(RegCreateKeyEx(hCtrlKey,lpStateIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                          KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                          &hStateIdxKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                        int size=mHpPak_FillDataFromNumber(oTab,*iStateVal,FALSE);
                        RegSetValueEx(hStateIdxKey,mHpCtrlMgmt->GetState(i)->GetName(),0,REG_DWORD,oTab,size);
                        RegCloseKey(hTmpKey);
                        RegCloseKey(hStateIdxKey);
                        mHpPak_InitStr(lpStateIdx,MAX_KEY_SIZE);
                        _snprintf(lpStateIdx,MAX_KEY_SIZE,"%d",++iStateIdx);
                        iStateVal=(const int*)mHpCtrlMgmt->GetState(i)->GetValue(iStateIdx);
                    }
                    break;
                }
                case CData::StrArray: // STRING ARRAY /////////////////////////
                {
                    UINT iStateIdx=0;
                    char lpStateIdx[MAX_KEY_SIZE]={0};
                    _snprintf(lpStateIdx,MAX_KEY_SIZE,"%d",iStateIdx);
                    const char* lpStateVal=(const char*)mHpCtrlMgmt->GetState(i)->GetValue(iStateIdx);
                    while(lpStateVal!=NULL)
                    {   HKEY hStateIdxKey=NULL;
                        if(RegCreateKeyEx(hCtrlKey,lpStateIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                          KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                          &hStateIdxKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                        if(!mHpPak_WriteLongStrRegVal(hStateIdxKey,lpStateIdx,
                                                      mHpCtrlMgmt->GetState(i)->GetName(),lpStateVal))
                        {   return FALSE;
                        }
                        RegCloseKey(hStateIdxKey);
                        mHpPak_InitStr(lpStateIdx,MAX_KEY_SIZE);
                        _snprintf(lpStateIdx,MAX_KEY_SIZE,"%d",++iStateIdx);
                        lpStateVal=(const char*)mHpCtrlMgmt->GetState(i)->GetValue(iStateIdx);
                    }
                    break;
                }
                case CData::Undefined: // UNDEFINED ///////////////////////////
                {   return FALSE; }
            }
        }
        RegCloseKey(hCtrlKey);
    }
    return TRUE;
}

BOOL CCtrlManTask::GetStrState(char* lpStrState,int nMaxSize,BOOL bWrite) const
{   // e.g. 4 states: 0 - 34 - "T^ext" - "Mul|ti1"^"Mu¤lti@2"
    // !bWrite: LET'S DLL MAKES ITS OWN STATE REPRESENTATION...
    // bWrite:  "0@34@T\^ext@Mul\|ti1^Mu\¤lti\@2"
    if(!mHpCtrlMgmt)
        return FALSE;
    // Check to display state representation
    if(!bWrite)
    {   const char* lpDisplay=mHpCtrlMgmt->DisplayState();
        if(!lpDisplay)
            LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_UNDEFINED_STATE,lpStrState,nMaxSize);
        else
            strncpy(lpStrState,lpDisplay,nMaxSize);
        return TRUE;
    }

    // Check control platform
    switch(mHpCtrlPlatformID)
    {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
        case PLATFORM_ID_WPF: ////// WPF Platform
        {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(mHpCtrlMgmt);
            pDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataState);
            if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
            {
                assert(NULL);

                //
                // TODO: Display DotNET error message
                // -> Stop mHelp processus
                //

                pDotNetCtrl->GetLastErr();
                return FALSE;
            }
            break;
        }
        case PLATFORM_ID_QT: ////// Qt Platform
        {   if(!mHpCtrlMgmt->FillQtData())
            {
                assert(NULL);

                //
                // TODO: Display error message
                // -> Stop mHelp processus
                //

                return FALSE;
            }
            break;
        }
    }

    ////// State
    for(UINT i=0; i<mHpCtrlMgmt->GetStateCount(); i++)
    {
        // Check existing state
        if(!mHpCtrlMgmt->GetState(i))
            return FALSE; // Inconsistent data
        // Check state type
        switch(mHpCtrlMgmt->GetState(i)->GetType())
        {   case CData::Integer: // INTEGER ///////////////////////////////////
            {   
                const int* iStateVal=(const int*)mHpCtrlMgmt->GetState(i)->GetValue();
                if(*lpStrState)
                {   char lpTmp[MAX_RS_SIZE]={0};
                    _snprintf(lpTmp,MAX_RS_SIZE,"%c%d",START_CTRL,(*iStateVal)); // '@'
                    strncat(lpStrState,lpTmp,nMaxSize);
                }
                else
                    _snprintf(lpStrState,nMaxSize,"%d",(*iStateVal));
                break;
            }
            case CData::String: // STRING /////////////////////////////////////
            {   
                if(*lpStrState)
                {   char szNextState[1]={0};
                    *szNextState=START_CTRL; // '@'
                    strncat(lpStrState,szNextState,nMaxSize);
                }
                const char* lpStateVal=(const char*)mHpCtrlMgmt->GetState(i)->GetValue();
                // Check write in mHelp file
                if(*lpStrState)
                {   char lpTmp[MAX_RS_SIZE]={0};
                    mHpPak_WriteString(lpTmp,MAX_RS_SIZE,lpStateVal);
                    strncat(lpStrState,lpTmp,nMaxSize);
                }
                else
                    mHpPak_WriteString(lpStrState,nMaxSize,lpStateVal);
                break;
            }
            case CData::IntArray: // INTEGER ARRAY ////////////////////////////
            {   
                if(*lpStrState)
                {   char szNextState[1]={0};
                    *szNextState=START_CTRL; // '@'
                    strncat(lpStrState,szNextState,nMaxSize);
                }
                UINT nIntIdx=0;
                const int* iStateVal=(const int*)mHpCtrlMgmt->GetState(i)->GetValue(nIntIdx);
                while(iStateVal!=NULL)
                {   // Check to add integer separator
                    if(nIntIdx)
                    {   char szNextState[1]={0};
                        *szNextState=ITEM_GROUP; // '^'
                        strncat(lpStrState,szNextState,nMaxSize);
                    }
                    char lpTmp[MAX_RS_SIZE]={0};
                    _snprintf(lpTmp,MAX_RS_SIZE,"%d",(*iStateVal));
                    strncat(lpStrState,lpTmp,nMaxSize);
                    // Next integer
                    iStateVal=(const int*)mHpCtrlMgmt->GetState(i)->GetValue(++nIntIdx);
                }
                break;
            }
            case CData::StrArray: // STRING ARRAY /////////////////////////////
            {   
                if(*lpStrState)
                {   char szNextState[1]={0};
                    *szNextState=START_CTRL; // '@'
                    strncat(lpStrState,szNextState,nMaxSize);
                }
                UINT nStrIdx=0;
                const char* lpStateVal=(const char*)mHpCtrlMgmt->GetState(i)->GetValue(nStrIdx);
                while(lpStateVal!=NULL)
                {   // Check to add string separator
                    if(nStrIdx)
                    {   char szNextState[1]={0};
                        *szNextState=ITEM_GROUP; // '^'
                        strncat(lpStrState,szNextState,nMaxSize);
                    }
                    // Check write in mHelp file
                    if(*lpStrState)
                    {   char lpTmp[MAX_RS_SIZE]={0};
                        mHpPak_WriteString(lpTmp,MAX_RS_SIZE,lpStateVal);
                        strncat(lpStrState,lpTmp,nMaxSize);
                    }
                    else
                        mHpPak_WriteString(lpStrState,nMaxSize,lpStateVal);
                    // Next string
                    lpStateVal=(const char*)mHpCtrlMgmt->GetState(i)->GetValue(++nStrIdx);
                }
                break;
            }
            case CData::Undefined: // UNDEFINED ///////////////////////////////
            {   return FALSE; }
        }
    }
    return TRUE;
}

HICON CCtrlManTask::GetCtrlIcon() const
{   if(mHpCtrlMgmt!=NULL)
        return mHpCtrlMgmt->GetIconList(mHpCtrlMgmt->GetIconIdx());
    return NULL;
}

void CCtrlManTask::GetCtrlVers(char* lpVers,UINT iMaxSize) const
{   *lpVers=0;
    if(mHpCtrlMgmt!=NULL)
        mHpCtrlMgmt->GetCtrlVers(lpVers,iMaxSize);
}

void CCtrlManTask::GetCtrlDesc(char* lpDesc,UINT iMaxSize) const
{   *lpDesc=0;
    if(mHpCtrlMgmt!=NULL)
        mHpCtrlMgmt->GetCtrlDesc(lpDesc,iMaxSize);
}

int CCtrlManTask::GetParamType() const
{   if((mHpCtrlMgmt!=NULL)&&(mHpCtrlMgmt->GetStateCount()))
        return 9; // LS
    return NO;
}

ImHpCtrlMgmt* CCtrlManTask::GetCtrlMgmt()
{   return mHpCtrlMgmt;
}

// class CCtrlManWrite ////////////////////////////////////////////////////////
CCtrlManWrite::CCtrlManWrite(ImHpCtrlMgmt* pCtrlMgmt,UINT iPlatformID)
              :CmHpWrite()
{   mHpCtrlMgmt=pCtrlMgmt;
    mHpCtrlPlatformID=iPlatformID;
}
CCtrlManWrite::CCtrlManWrite()
              :CmHpWrite()
{   mHpCtrlMgmt=NULL;
    mHpCtrlPlatformID=0;
}

//////
BOOL CCtrlManWrite::ReadEventKey(int index)
{   if(!mHpCtrlMgmt) // Should be created via CCtrlManTask::ReadTaskKey() call
        return FALSE;
    // Check existing config
    if(!mHpCtrlMgmt->GetCfgCount())
        return TRUE;
    HKEY hSubWriteKey=NULL,hWriteKey=NULL;
    ReadmHpWriteKey(index,hSubWriteKey,hWriteKey);
    if(hWriteKey!=NULL)
    {
        // Check control platform
        switch(mHpCtrlPlatformID)
        {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(mHpCtrlMgmt);
                pDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataCfg,FALSE);
                if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display DotNET error message
                    // -> Stop mHelp processus
                    //

                    pDotNetCtrl->GetLastErr();
                    return FALSE;
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {   if(!mHpCtrlMgmt->FillQtData())
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus
                    //

                    return FALSE;
                }
                break;
            }
        }

        ////// Config
        for(UINT i=0; i<mHpCtrlMgmt->GetCfgCount(); i++)
        {
            // Check existing config
            if(!mHpCtrlMgmt->GetConfig(i))
                return FALSE; // Inconsistent data
            // Check config type
            switch(mHpCtrlMgmt->GetConfig(i)->GetType())
            {   case CData::Integer: // INTEGER ///////////////////////////////
                {   
                    BYTE oTab[MAX_KEY_SIZE]={0};
                    DWORD Size=MAX_KEY_SIZE,Type=REG_DWORD;
                    if(RegQueryValueEx(hWriteKey,mHpCtrlMgmt->GetConfig(i)->GetName(),NULL,&Type,oTab,&Size)!=ERROR_SUCCESS)
                        return FALSE;
                    int iVal=0;
                    mHpPak_FillIntFromData(oTab,iVal,FALSE);
                    mHpCtrlMgmt->GetConfig(i)->SetValue(iVal);
                    break;
                }
                case CData::String: // STRING /////////////////////////////////
                {   
                    char lpVal[MAX_RS_SIZE]={0};
                    if(!mHpPak_ReadLongStrRegVal(hWriteKey,mHpCtrlMgmt->GetConfig(i)->GetName(),lpVal,MAX_RS_SIZE))
                        return FALSE;
                    mHpCtrlMgmt->GetConfig(i)->SetValue(lpVal);
                    break;
                }
                case CData::IntArray: // INTEGER ARRAY ////////////////////////
                {
                    HKEY hCfgKey=NULL;
                    if(RegOpenKey(hWriteKey,mHpCtrlMgmt->GetConfig(i)->GetName(),&hCfgKey)==ERROR_SUCCESS)
                    {   DWORD Type=REG_DWORD;
                        DWORD Size=MAX_KEY_SIZE;
                        HKEY hCfgIdxKey=NULL;
                        int iCfgIdx=0;
                        char lpCfgIdx[MAX_KEY_SIZE]={0};
                        _snprintf(lpCfgIdx,MAX_KEY_SIZE,"%d",iCfgIdx);
                        while(RegOpenKey(hCfgKey,lpCfgIdx,&hCfgIdxKey)==ERROR_SUCCESS)
                        {   BYTE oTab[MAX_KEY_SIZE]={0};
                            if(RegQueryValueEx(hCfgIdxKey,mHpCtrlMgmt->GetConfig(i)->GetName(),NULL,&Type,oTab,&Size)==ERROR_SUCCESS)
                            {   int iCfg=0;
                                mHpPak_FillIntFromData(oTab,iCfg,FALSE);
                                mHpCtrlMgmt->GetConfig(i)->AddValue(iCfg);
                            }
                            else
                                return FALSE;
                            Type=REG_DWORD;
                            Size=MAX_KEY_SIZE;
                            mHpPak_InitStr(lpCfgIdx,MAX_KEY_SIZE);
                            _snprintf(lpCfgIdx,MAX_KEY_SIZE,"%d",++iCfgIdx);
                            RegCloseKey(hCfgIdxKey);
                        }
                        RegCloseKey(hCfgKey);
                    }
			        break;
                }
                case CData::StrArray: // STRING ARRAY /////////////////////////
                {
                    HKEY hCfgKey=NULL;
                    if(RegOpenKey(hWriteKey,mHpCtrlMgmt->GetConfig(i)->GetName(),&hCfgKey)==ERROR_SUCCESS)
                    {   HKEY hCfgIdxKey=NULL;
                        int iCfgIdx=0;
                        char lpCfgIdx[MAX_KEY_SIZE]={0};
                        _snprintf(lpCfgIdx,MAX_KEY_SIZE,"%d",iCfgIdx);
                        while(RegOpenKey(hCfgKey,lpCfgIdx,&hCfgIdxKey)==ERROR_SUCCESS)
                        {   char lpCfg[MAX_RS_SIZE]={0};
                            if(!mHpPak_ReadLongStrRegVal(hCfgIdxKey,mHpCtrlMgmt->GetConfig(i)->GetName(),lpCfg,MAX_RS_SIZE))
                                return FALSE;
                            mHpCtrlMgmt->GetConfig(i)->AddValue(lpCfg);
                            mHpPak_InitStr(lpCfgIdx,MAX_KEY_SIZE);
                            _snprintf(lpCfgIdx,MAX_KEY_SIZE,"%d",++iCfgIdx);
                            RegCloseKey(hCfgIdxKey);
                        }
                        RegCloseKey(hCfgKey);
                    }
                    break;
                }
                case CData::Undefined: // UNDEFINED ///////////////////////////
                {   return FALSE; }
            }
        }

        // Check control platform
        switch(mHpCtrlPlatformID)
        {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(mHpCtrlMgmt);
                // Update DotNET data
                pDotNetCtrl->UpdateDotNetData(CmHpDotNetCtrl::DataCfg);
                if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display DotNET error message
                    // -> Stop mHelp processus
                    //

                    pDotNetCtrl->GetLastErr();
                    return FALSE;
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {   if(!mHpCtrlMgmt->UpdateQtData())
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus
                    //

                    return FALSE;
                }
                break;
            }
        }
        return TRUE;
    }
    return FALSE;
}

BOOL CCtrlManWrite::WriteEventKey()
{   if(!mHpCtrlMgmt)
        return FALSE; // Should be defined
    // Check existing config
    if(!mHpCtrlMgmt->GetCfgCount())
        return TRUE;
    HKEY hSubWriteKey=NULL,hWriteKey=NULL;
    CmHpWrite::WritemHpWriteKey(hSubWriteKey,hWriteKey);
    if(hWriteKey!=NULL)
    {   BOOL bAclRes;
        SECURITY_ATTRIBUTES SecAtt;
        mHpPak_GetSecurityAtt(SecAtt,bAclRes);
        //
        HKEY hCtrlKey=NULL;
        BYTE oTab[MAX_KEY_SIZE]={0};
        DWORD dWord=REG_OPENED_EXISTING_KEY;
        if(RegCreateKeyEx(hSubWriteKey,MHPWRITE_KEY,0,NULL,REG_OPTION_NON_VOLATILE,
                          KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                          &hCtrlKey,&dWord)!=ERROR_SUCCESS) return FALSE;

        ////// Config
        for(UINT i=0; i<mHpCtrlMgmt->GetCfgCount(); i++)
        {
            // Check existing config
            if(!mHpCtrlMgmt->GetConfig(i))
                return FALSE; // Inconsistent data
            // Check config type
            switch(mHpCtrlMgmt->GetConfig(i)->GetType())
            {   case CData::Integer: // INTEGER ///////////////////////////////
                {   
                    const int* iCfgVal=(const int*)mHpCtrlMgmt->GetConfig(i)->GetValue();
                    int size=mHpPak_FillDataFromNumber(oTab,*iCfgVal,FALSE);
                    RegSetValueEx(hCtrlKey,mHpCtrlMgmt->GetConfig(i)->GetName(),0,REG_DWORD,oTab,size);
                    break;
                }
                case CData::String: // STRING /////////////////////////////////
                {   
                    const char* lpCfgVal=(const char*)mHpCtrlMgmt->GetConfig(i)->GetValue();
                    if(!mHpPak_WriteLongStrRegVal(hCtrlKey,MHPWRITE_KEY,
                                                  mHpCtrlMgmt->GetConfig(i)->GetName(),lpCfgVal))
                    {   return FALSE;
                    }
                    break;
                }
                case CData::IntArray: // INTEGER ARRAY ////////////////////////
                {
                    HKEY hTmpKey;
                    UINT iCfgIdx=0;
                    char lpCfgIdx[MAX_KEY_SIZE]={0};
                    _snprintf(lpCfgIdx,MAX_KEY_SIZE,"%d",iCfgIdx);
                    const int* iCfgVal=(const int*)mHpCtrlMgmt->GetConfig(i)->GetValue(iCfgIdx);
                    while(iCfgVal!=NULL)
                    {   if(RegCreateKeyEx(hCtrlKey,lpCfgIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                          KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                          &hTmpKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                        HKEY hCfgIdxKey=NULL;
                        if(RegCreateKeyEx(hCtrlKey,lpCfgIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                          KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                          &hCfgIdxKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                        int size=mHpPak_FillDataFromNumber(oTab,*iCfgVal,FALSE);
                        RegSetValueEx(hCfgIdxKey,mHpCtrlMgmt->GetConfig(i)->GetName(),0,REG_DWORD,oTab,size);
                        RegCloseKey(hTmpKey);
                        RegCloseKey(hCfgIdxKey);
                        mHpPak_InitStr(lpCfgIdx,MAX_KEY_SIZE);
                        _snprintf(lpCfgIdx,MAX_KEY_SIZE,"%d",++iCfgIdx);
                        iCfgVal=(const int*)mHpCtrlMgmt->GetConfig(i)->GetValue(iCfgIdx);
                    }
                    break;
                }
                case CData::StrArray: // STRING ARRAY /////////////////////////
                {
                    UINT iCfgIdx=0;
                    char lpCfgIdx[MAX_KEY_SIZE]={0};
                    _snprintf(lpCfgIdx,MAX_KEY_SIZE,"%d",iCfgIdx);
                    const char* lpCfgVal=(const char*)mHpCtrlMgmt->GetConfig(i)->GetValue(iCfgIdx);
                    while(lpCfgVal!=NULL)
                    {   HKEY hCfgIdxKey=NULL;
                        if(RegCreateKeyEx(hCtrlKey,lpCfgIdx,0,NULL,REG_OPTION_NON_VOLATILE,
                                          KEY_CREATE_SUB_KEY|KEY_EXECUTE|KEY_QUERY_VALUE|KEY_SET_VALUE,&SecAtt,
                                          &hCfgIdxKey,&dWord)!=ERROR_SUCCESS) return FALSE;
                        if(!mHpPak_WriteLongStrRegVal(hCfgIdxKey,lpCfgIdx,
                                                      mHpCtrlMgmt->GetConfig(i)->GetName(),lpCfgVal))
                        {   return FALSE;
                        }
                        RegCloseKey(hCfgIdxKey);
                        mHpPak_InitStr(lpCfgIdx,MAX_KEY_SIZE);
                        _snprintf(lpCfgIdx,MAX_KEY_SIZE,"%d",++iCfgIdx);
                        lpCfgVal=(const char*)mHpCtrlMgmt->GetConfig(i)->GetValue(iCfgIdx);
                    }
                    break;
                }
                case CData::Undefined: // UNDEFINED ///////////////////////////
                {   return FALSE; }
            }
        }
        RegCloseKey(hCtrlKey);
        return TRUE;
    }
    return FALSE;
}

//********************************************************************MHPFILE//
// CtrlMan_ReadEvent
//---------------------------------------------//
BOOL CtrlMan_ReadEvent(HWND hWndActive,UINT iPlatformID,int iClassIdx,int iCtrlStyle,int iCtrlMsgID,
                       const std::string& strPostData,UINT &iCustIdx)
{	int iCur,iCnt,iTmp,iOpt;
    char chNext=END_RS_TASK;
    char lpMsg[MAX_RS_SIZE]={0};
    char lpStateMsg[MAX_RS_SIZE]={0};
    // iCurrentWnd,iCountWnd,iOption,lpmHpMsg
    GetCommonEvent(hWndActive,iCur,iCnt,iOpt,lpMsg,MAX_RS_SIZE,lpStateMsg,MAX_RS_SIZE,chNext,iTmp);
    if(iTmp==NO)
        return TRUE;
    // mHpCtrlMgmt
    const char* lpCtrlClass=mHpPak_ProcCtrlList->GetClassName(iClassIdx);
    if(!lpCtrlClass)
        return TRUE; // Error: Class index not exists in the control list
    ImHpCtrlMgmt* pCtrlMgmt=GetmHpVarStruct()->CtrlMgmtList->ReadEvent(lpCtrlClass,iPlatformID);
    if(!pCtrlMgmt)
        return TRUE; // Error: Control Dll not found
    if(!pCtrlMgmt->SetStyle(iCtrlStyle))
        return TRUE; // Error: Wrong control style
    if(!pCtrlMgmt->SetCaptMsgID(iCtrlMsgID))
        return TRUE; // Error: Wrong message ID
    // Unique control ID
    if(!ReadCtrlIDTask(hWndActive,pCtrlMgmt,iPlatformID,chNext))
        return TRUE; // Error: Failed to get control ID
    BOOL bSel=TRUE;
    if(pCtrlMgmt->GetStateCount())
    {   if(chNext!=TREE_PARAM_GROUP) // '¤'
        {   delete pCtrlMgmt;
            return TRUE; // Error: Missing state
        }

        // Check control platform
        switch(iPlatformID)
        {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlMgmt);
                pDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataState,FALSE);
                if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display DotNET error message
                    // -> Stop mHelp processus
                    //

                    pDotNetCtrl->GetLastErr();
                    return TRUE;
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {   if(!pCtrlMgmt->FillQtData())
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus
                    //

                    return TRUE;
                }
                break;
            }
        }

        ////// State
        int ParamIdx=NO;
        for(UINT i=0; (i<pCtrlMgmt->GetStateCount()&&(bSel)); i++)
        {
            // Check existing state
            if(!pCtrlMgmt->GetState(i))
                return TRUE; // Error: Inconsistent data
            // Check state type
            switch(pCtrlMgmt->GetState(i)->GetType())
            {   case CData::Integer: // INTEGER ///////////////////////////////
                {   
                    int iState=NO;
#ifdef MHELP_OLD
                    if(ParamIdx==NO)
                    {   char lpTmp[MAX_RS_SIZE]={0};
                        mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
                        if(iState==NO)
                        {   // Check state param
                            if((lpTmp[0]==END_LIST)||(lpTmp[0]!=RS_PARAM)) // '§'
                            {   delete pCtrlMgmt;
                                return TRUE; // Error: Not an integer state or state parameter
                            }
                            // Get param index and check selection
                            ParamIdx=mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,1);
                            if(!DidItSelected(ParamIdx))
                            {   bSel=FALSE;
                                break; // Exit switch: Param not selected
                            }
                        }
                    }
                    // Check state param
                    if(iState==NO) // ParamIdx!=NO
                    {   char lpIntState[MAX_RS_SIZE]={0};
                        // Get state from parameter list
                        GetStrEvoluParam(ParamIdx,GetmHpVarStruct()->RepeatLoop,i,lpIntState,MAX_RS_SIZE);
                        if(ParamIdx!=NO)
                            iState=mHpPak_GetIntFromStr(lpIntState,MAX_RS_SIZE,0);
                        else
                        {   delete pCtrlMgmt;
                            return TRUE; // Error: No state found in parameter list
                        }
                    }
#else
                    ////// TODO: Be able to remove lines below (state always in HTML)
                    char lpTmp[MAX_RS_SIZE]={0};
                    mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
                    if(iState==NO)
                    {   delete pCtrlMgmt;
                        return TRUE; // Error: Not an integer state or state parameter
                    }
                    //////

                    // Check if selected
                    if(!IsHtmlCustSelected(strPostData,iCustIdx))
                    {   ParamIdx=0; // !=NO
                        bSel=FALSE;
                        break; // Exit switch: Param not selected
                    }
                    // Get HTML user setting
                    char lpIntState[MAX_SHT_SIZE]={0};
                    if(!GetHtmlCustValue(lpIntState,
                                         MAX_SHT_SIZE,
                                         strPostData,
                                         pCtrlMgmt->GetState(i)->GetName(),
                                         iCustIdx))
                    {   delete pCtrlMgmt;
                        return TRUE; // Error: No state parameter
                    }
                    iState=mHpPak_GetIntFromStr(lpIntState,MAX_SHT_SIZE,0);
#endif
                    // Assign state
                    pCtrlMgmt->GetState(i)->SetValue(iState);
                    break;
                }
                case CData::String: // STRING /////////////////////////////////
                {   
                    char lpStrState[MAX_RS_SIZE]={0};
#ifdef MHELP_OLD
                    if(ParamIdx==NO)
                    {   mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpStrState,MAX_RS_SIZE,chNext,TRUE);
                        if(iTmp==NO)
                        {   delete pCtrlMgmt;
                            return TRUE; // Error: No state found
                        }
                        // Check state param
                        if(lpStrState[0]==RS_PARAM) // '§'
                        {   // Get param index and check selection
                            ParamIdx=mHpPak_GetIntFromStr(lpStrState,MAX_RS_SIZE,1);
                            if(!DidItSelected(ParamIdx))
                            {   bSel=FALSE;
                                break; // Exit switch: Param not selected
                            }
                        }
                    }
                    // Check state param
                    if(ParamIdx!=NO)
                    {   // Get state from parameter list
                        GetStrEvoluParam(ParamIdx,GetmHpVarStruct()->RepeatLoop,i,lpStrState,MAX_RS_SIZE);
                        if(ParamIdx==NO)
                        {   delete pCtrlMgmt;
                            return TRUE; // Error: No state found in parameter list
                        }
                    }
#else
                    ////// TODO: Be able to remove lines below (state always in HTML)
                    mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpStrState,MAX_RS_SIZE,chNext,TRUE);
                    if(iTmp==NO)
                    {   delete pCtrlMgmt;
                        return TRUE; // Error: No state found
                    }
                    //////

                    // Check if selected
                    if(!IsHtmlCustSelected(strPostData,iCustIdx))
                    {   ParamIdx=0; // !=NO
                        bSel=FALSE;
                        break; // Exit switch: Param not selected
                    }
                    // Get HTML user setting
                    if(!GetHtmlCustValue(lpStrState,
                                         MAX_RS_SIZE,
                                         strPostData,
                                         pCtrlMgmt->GetState(i)->GetName(),
                                         iCustIdx))
                    {   delete pCtrlMgmt;
                        return TRUE; // Error: No state parameter
                    }
#endif
                    // Assign state
                    pCtrlMgmt->GetState(i)->SetValue(lpStrState);
                    break;
                }
                case CData::IntArray: // INTEGER ARRAY ////////////////////////
                {
#ifdef MHELP_OLD
                    if(ParamIdx==NO)
                    {   int iState=0;
                        char lpTmp[MAX_RS_SIZE]={0};
                        do
                        {   chNext=NEXT_RS; // '|': Force to get integer value (see mHpPak_GetEventPart)
                            mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
                            if(iState==NO)
                            {   // Check state param
                                if(lpTmp[0]==RS_PARAM) // '§'
                                {   // Get param index and check selection
                                    ParamIdx=mHpPak_GetIntFromStr(lpTmp,MAX_RS_SIZE,1);
                                    if(!DidItSelected(ParamIdx))
                                        bSel=FALSE;
                                    break; // Break loop
                                }
                                else
                                {   delete pCtrlMgmt;
                                    return TRUE; // Error: Wrong state type found (string found)
                                }
                            }
                            // Assign state
                            pCtrlMgmt->GetState(i)->AddValue(iState);

                        } while(chNext==ITEM_GROUP); // '^'
                    }
                    // Check state param and selection
                    if((ParamIdx!=NO)&&(bSel))
                    {   char lpTmp[MAX_RS_SIZE]={0};
                        // Get state from parameter list (all integer defined in the array)
                        GetStrEvoluParam(ParamIdx,GetmHpVarStruct()->RepeatLoop,i,lpTmp,MAX_RS_SIZE);
                        if(ParamIdx==NO)
                        {   delete pCtrlMgmt;
                            return TRUE; // Error: No state found in parameter list
                        }
                        // Assign state
                        int iIntIdx=0;
                        while(lpTmp[iIntIdx])
                        {   int iState=0;
                            // lpTmp: "10^2^5^8"
                            while((lpTmp[iIntIdx])&&(lpTmp[iIntIdx]!=ITEM_GROUP)) // '^'
                            {   if(mHpPak_IsItANumber(lpTmp[iIntIdx]))
                                {   if(iState)
                                    {   iState*=10;
                                        iState+=mHpPak_CharToInt(lpTmp[iIntIdx]);
                                    }
                                    else
                                        iState=mHpPak_CharToInt(lpTmp[iIntIdx]);
                                    iIntIdx++;
                                }
                                else
                                {   delete pCtrlMgmt;
                                    return TRUE; // Error: Wrong state type found (string found)
                                }
                            }
                            // Assign state
                            pCtrlMgmt->GetState(i)->AddValue(iState);
                        }
                    }
#else
                    ////// TODO: Be able to remove lines below (state always in HTML)
                    int iState=0;
                    char lpTmp[MAX_RS_SIZE]={0};
                    do
                    {   chNext=NEXT_RS; // '|': Force to get integer value (see mHpPak_GetEventPart)
                        mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iState,lpTmp,MAX_RS_SIZE,chNext,FALSE);
                        if(iState==NO)
                        {   delete pCtrlMgmt;
                            return TRUE; // Error: Wrong state type found (string found)
                        }
                        // Assign state
                        //pCtrlMgmt->GetState(i)->AddValue(iState);

                    } while(chNext==ITEM_GROUP); // '^'
                    //////

                    // Check if selected
                    if(!IsHtmlCustSelected(strPostData,iCustIdx))
                    {   ParamIdx=0; // !=NO
                        bSel=FALSE;
                        break; // Exit switch: Param not selected
                    }
                    // Get HTML user setting
                    if(!GetHtmlCustValue(lpTmp,
                                         MAX_RS_SIZE,
                                         strPostData,
                                         pCtrlMgmt->GetState(i)->GetName(),
                                         iCustIdx))
                    {   delete pCtrlMgmt;
                        return TRUE; // Error: No state parameter
                    }
                    // Assign state
                    int iIntIdx=0;
                    while(lpTmp[iIntIdx])
                    {   iState=0;
                        // lpTmp: "10^2^5^8"
                        while((lpTmp[iIntIdx])&&(lpTmp[iIntIdx]!=ITEM_GROUP)) // '^'
                        {   if(mHpPak_IsItANumber(lpTmp[iIntIdx]))
                            {   if(iState)
                                {   iState*=10;
                                    iState+=mHpPak_CharToInt(lpTmp[iIntIdx]);
                                }
                                else
                                    iState=mHpPak_CharToInt(lpTmp[iIntIdx]);
                                iIntIdx++;
                            }
                            else
                            {   delete pCtrlMgmt;
                                return TRUE; // Error: Wrong state type found (string found)
                            }
                        }
                        // Assign state
                        pCtrlMgmt->GetState(i)->AddValue(iState);
                    }
#endif
                    break;
                }
                case CData::StrArray: // STRING ARRAY /////////////////////////
                {
#ifdef MHELP_OLD
                    if(ParamIdx==NO)
                    {   chNext=ITEM_GROUP; // Exit at next ITEM_GROUP found (see mHpPak_GetEventPart)
                        while(chNext==ITEM_GROUP)
                        {   char lpStrState[MAX_RS_SIZE]={0};
                            mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpStrState,MAX_RS_SIZE,chNext,TRUE);
                            if(iTmp==NO)
                            {   delete pCtrlMgmt;
                                return TRUE; // Error: No state found
                            }
                            // Check state param
                            if(lpStrState[0]==RS_PARAM) // '§'
                            {   // Get param index and check selection
                                ParamIdx=mHpPak_GetIntFromStr(lpStrState,MAX_RS_SIZE,1);
                                if(!DidItSelected(ParamIdx))
                                    bSel=FALSE;
                                break; // Break loop
                            }
                            else // Assign state
                                pCtrlMgmt->GetState(i)->AddValue(lpStrState);
                        }
                    }
                    // Check state param and selection
                    if((ParamIdx!=NO)&&(bSel))
                    {   char lpTmp[MAX_RS_SIZE]={0};
                        // Get state from parameter list (all string defined in the array)
                        GetStrEvoluParam(ParamIdx,GetmHpVarStruct()->RepeatLoop,i,lpTmp,MAX_RS_SIZE);
                        if(ParamIdx==NO)
                        {   delete pCtrlMgmt;
                            return TRUE; // Error: No state found in parameter list
                        }
                        // Assign state
                        int iStrIdx=0;
                        while(iStrIdx!=NO)
                        {   char lpStrState[MAX_RS_SIZE]={0};
                            mHpPak_DecodeString(lpStrState,MAX_RS_SIZE,lpTmp,iStrIdx);
                            pCtrlMgmt->GetState(i)->AddValue(lpStrState);
                        }
                    }
#else
                    ////// TODO: Be able to remove lines below (state always in HTML)
                    chNext=ITEM_GROUP; // Exit at next ITEM_GROUP found (see mHpPak_GetEventPart)
                    while(chNext==ITEM_GROUP)
                    {   char lpState[MAX_RS_SIZE]={0};
                        mHpPak_GetEventPart(hWndActive,GetmHpVarStruct()->mHpFileName,iTmp,lpState,MAX_RS_SIZE,chNext,TRUE);
                        if(iTmp==NO)
                        {   delete pCtrlMgmt;
                            return TRUE; // Error: No state found
                        }
                        // Assign state
                        //pCtrlMgmt->GetState(i)->AddValue(lpState);
                    }
                    //////

                    // Check if selected
                    if(!IsHtmlCustSelected(strPostData,iCustIdx))
                    {   ParamIdx=0; // !=NO
                        bSel=FALSE;
                        break; // Exit switch: Param not selected
                    }
                    // Get HTML user setting
                    char lpStrState[MAX_LONG_SIZE]={0};
                    if(!GetHtmlCustValue(lpStrState,
                                         MAX_LONG_SIZE,
                                         strPostData,
                                         pCtrlMgmt->GetState(i)->GetName(),
                                         iCustIdx))
                    {   delete pCtrlMgmt;
                        return TRUE; // Error: No state parameter
                    }
                    // Assign state
                    int iStrIdx=0;
                    while(iStrIdx!=NO)
                    {   char lpState[MAX_RS_SIZE]={0};
                        mHpPak_DecodeString(lpState,MAX_RS_SIZE,lpStrState,iStrIdx);
                        pCtrlMgmt->GetState(i)->AddValue(lpState);
                    }
#endif
                    break;
                }
                case CData::Undefined: // UNDEFINED ///////////////////////////
                {   delete pCtrlMgmt;
                    return TRUE; // Error: Undefined state type
                }
            }
            // Check state as state param
            if(ParamIdx!=NO)
            {   if(!bSel)
                    break; // Exit loop: Param not selected
            }
            else // Check [no]next state if [not]exists in mHelp file
            if((((i+1)==pCtrlMgmt->GetStateCount())&&(chNext==START_CTRL))|| // No other state but next state found in file
                (((i+1)<pCtrlMgmt->GetStateCount())&&(chNext!=START_CTRL))) // Other state but no other state found in file
            {   delete pCtrlMgmt;
                return TRUE; // Error: [Un]Expected next state [not]found
            }
        }

        // Check control platform
        switch(iPlatformID)
        {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlMgmt);
                // Update DotNET data
                pDotNetCtrl->UpdateDotNetData(CmHpDotNetCtrl::DataState);
                if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display DotNET error message
                    // -> Stop mHelp processus
                    //

                    pDotNetCtrl->GetLastErr();
                    return TRUE;
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {   if(!pCtrlMgmt->UpdateQtData())
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus
                    //

                    return TRUE;
                }
                break;
            }
        }

        // Increase number of custom task
        iCustIdx++;
    }
    if(bSel)
    {
        // Check control platform
        switch(iPlatformID)
        {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlMgmt);
                pDotNetCtrl->FillDotNetProperties();
                if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display DotNET error message
                    // -> Stop mHelp processus
                    //

                    pDotNetCtrl->GetLastErr();
                    return TRUE;
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {   if(!pCtrlMgmt->FillQtData())
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus
                    //

                    return TRUE;
                }
                break;
            }
        }

        pCtrlMgmt->SetCtrlClass(lpCtrlClass); // Done here cos 'FillDotNetProperties' empty 'szClass'
        CCtrlManTask* CtrlTask=new CCtrlManTask(lpMsg,lpStateMsg,iCur,iCnt,iOpt,pCtrlMgmt,iPlatformID);
        GetmHpVarStruct()->TaskList->AddCellule(CtrlTask);
    }
    else
        delete pCtrlMgmt;

    return FALSE; // OK
}

//---------------------------------------------//
// CtrlMan_WriteEvent
//---------------------------------------------//
void CtrlMan_WriteEvent(HWND hWndActive,int Index,CEventListe* EvntObj,CWindowListe* WndCurLst,
                        BOOL bFolderParam,char* lpCurExe,int &IndexParam,BOOL &bRes)
{   if(!WriteCommonTask(hWndActive,EvntObj,WndCurLst,lpCurExe,Index))
    {   bRes=FALSE;
        return;
    }
    // Write unique control ID
    if(!WriteCtrlIDTask(hWndActive,EvntObj,Index))
    {   bRes=FALSE;
        return;
    }
    // Check existing state
    if(EvntObj->GetmHpTask(Index,FALSE)->GetParamType()!=NO)
    {   if((bFolderParam)&&(EvntObj->GetCustomVisible(Index)))
	    {   if((!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,NULL,FALSE))|| // '¤'
               (!mHpPak_WriteParty(hWndActive,RS_PARAM,IndexParam,NULL,FALSE))) // §iParam
            {   bRes=FALSE;}
            IndexParam++;
        }
        else if(!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrState(Index,TRUE),FALSE)) // '¤'
        {   bRes=FALSE;}
    }
}

//---------------------------------------------//
// CtrlMan_WriteParam
//---------------------------------------------//
BOOL CtrlMan_WriteParam(HWND hWndActive,CEventListe* EvntObj,int Index)
{	// Check existing state
    if(EvntObj->GetParamType(Index)!=NO)
    {   if((!mHpPak_WriteParty(hWndActive,RS_PARAM,NO,EvntObj->GetStrState(Index,TRUE),TRUE))|| // '§'
           (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,EvntObj->GetParamType(Index),NULL,FALSE))|| // '¤'
           (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrLimite(Index,TRUE,FALSE),TRUE))|| // '¤'
           (!mHpPak_WriteParty(hWndActive,TREE_PARAM_GROUP,NO,EvntObj->GetStrDescription(Index,FALSE),TRUE))) // '¤'
	    {   return FALSE;}
    }
    return TRUE;
}			

//***********************************************************************KEYS//
// CtrlMan_ReadEventKeys
//---------------------------------------------//
void CtrlMan_ReadEventKeys(int Index,CEventListe* EvntObj,CTaskListe* TaskList)
{   CmHpTask* ReadTask;
    CmHpWrite* ReadWrite;
    CmHpParam* ReadParam;
    ReadTask=new CCtrlManTask();
    ReadTask->ReadTaskKey(Index);
    if(EvntObj!=NULL)
    {   ReadWrite=new CCtrlManWrite(ReadTask->GetCtrlMgmt(),ReadTask->GetCtrlPlatformID());
        ReadParam=new CmHpParam();
        ReadWrite->ReadEventKey(Index);
        if(!ReadParam->ReadParamKey(Index))
        {   delete(ReadParam);
            ReadParam=NULL;
        }
        EvntObj->AddEvent(ReadTask,ReadWrite,ReadParam);
    }
    else TaskList->AddCellule(ReadTask);
}

//************************************************************************ADD//
// CtrlMan_AddEvent
//---------------------------------------------//
void CtrlMan_AddEvent(HWND hWndActive,ImHpCtrlMgmt* pCtrlMgmt,UINT iPlatformID,char* lpCtrlName,
                      UINT iTaskID,int iCurWnd,int iCntWnd,BOOL bWrite)
{	CmHpTask* Item;
	CmHpWrite* wItem;
    Item=new CCtrlManTask(NULL,NULL,iCurWnd,iCntWnd,0,pCtrlMgmt,iPlatformID);
	wItem=new CCtrlManWrite(pCtrlMgmt,iPlatformID);
	if(bWrite)
	{	Item->WriteTaskKey();
		wItem->WriteEventKey();
		GetmHpVarStruct()->EventList->AddEvent(Item,wItem,NULL);
	}
	else
	{	RemoveLastKeyPos();
		Item->WriteTaskKey();
		wItem->WriteEventKey();
		GetmHpVarStruct()->EventList->ReplaceEvent(Item,wItem);
	}
}

//*****************************************************************PROPERTIES//
// CtrlMan_IsCustom
//---------------------------------------------//
void CtrlMan_IsCustom(BOOL &bcustom,CEventCell* EvntCell)
{   bcustom=(EvntCell->mHpTask->GetParamType()!=NO)? TRUE:FALSE;
}

//---------------------------------------------//
// CtrlMan_StrState
//---------------------------------------------//
void CtrlMan_StrState(CEventCell* EvntCell,BOOL pWriteOp,char* lpState,int StateSize)
{   EvntCell->mHpTask->GetStrState(lpState,StateSize,pWriteOp);
}

//---------------------------------------------//
// CtrlMan_ParamType
//---------------------------------------------//
int CtrlMan_ParamType(CEventCell* EvntCell)
{   return EvntCell->mHpTask->GetParamType();
}

//---------------------------------------------//
// CtrlMan_DefaultBound
//---------------------------------------------//
void CtrlMan_DefaultBound(CEventCell* EvntCell,char* pStrBound,int BoundSize)
{   // Check existing state
    if(EvntCell->mHpTask->GetParamType()!=NO)
    {   if(EvntCell->mHpTask->GetCtrlMgmt()->GetStateBound()!=NULL)
        {   strncpy(pStrBound,EvntCell->mHpTask->GetCtrlMgmt()->GetStateBound(),BoundSize);
        }
    }
    else
        *pStrBound=0;
}

//---------------------------------------------//
// CtrlMan_IsCustomized
//---------------------------------------------//
BOOL CtrlMan_IsCustomized(CEventCell* EvntCell)
{	return (EvntCell->mHpTask->GetParamType()!=NO)? TRUE:FALSE;
}

//---------------------------------------------//
// CtrlMan_IsNoStateMsg
//---------------------------------------------//
BOOL CtrlMan_IsNoStateMsg(CEventCell* EvntCell)
{	if(EvntCell->mHpTask->GetCtrlMgmt()!=NULL)
        return EvntCell->mHpTask->GetCtrlMgmt()->NeedToCheckState();
    return FALSE;
}

//---------------------------------------------//
// CtrlMan_GetHtmlTag
//---------------------------------------------//
void CtrlMan_GetHtmlTag(CEventCell* EvntCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{
    ImHpCtrlMgmt* pCtrlMgmt=EvntCell->mHpTask->GetCtrlMgmt();
    if(pCtrlMgmt)
    {
        // Get managed control HTML tag
        pCtrlMgmt->GetHtmlTags(lpHtmlTag,iMaxSize,(UINT)iCustIndex);
    }
}

//*******************************************************************CFGTOOLS//
// CtrlMan_ShowCfgTools
//---------------------------------------------//
void CtrlMan_ShowCfgTools(HWND hWndActive)
{   if((hWndUI!=NULL)&&(IsWindow(hWndUI)))
        ShowWindow(hWndUI,SW_SHOW);
}

//---------------------------------------------//
// CtrlMan_HideCfgTools
//---------------------------------------------//
void CtrlMan_HideCfgTools(HWND hWndActive)
{   if((hWndUI!=NULL)&&(IsWindow(hWndUI)))
        ShowWindow(hWndUI,SW_HIDE);
}

//---------------------------------------------//
// CtrlMan_SaveCfg
//---------------------------------------------//
void CtrlMan_SaveCfg(HWND hWndActive)
{   if((hWndUI!=NULL)&&(IsWindow(hWndUI)))
    {   CmHpTask* CurTask=GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE);
        CurTask->GetCtrlMgmt()->SaveUI(hWndUI);

        // Check control platform
        switch(CurTask->GetCtrlPlatformID())
        {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(CurTask->GetCtrlMgmt());
                pDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataState);
                if(pDotNetCtrl->CheckDotNetErr()) // Always check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display DotNET error message
                    // -> Stop mHelp processus
                    //

                    pDotNetCtrl->GetLastErr();
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {   if(!CurTask->GetCtrlMgmt()->FillQtData())
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus
                    //

                }
                break;
            }
        }
    }
}

//---------------------------------------------//
// CtrlMan_GetInfosCfg
//---------------------------------------------//
void CtrlMan_GetInfosCfg(HWND hWndDlgCfg,HWND hWndApply,HWND hIcon,HWND hProd,HWND hVers,HWND hDescrip,HWND hFmCtrl)
{   CmHpTask* pCtrlTask=GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE);
    if(!pCtrlTask->GetCtrlMgmt())
        return; // Error: Should be defined
    // Product name
    char lpCtrlName[MAX_RS_SIZE]={0};
    if(pCtrlTask->GetCtrlMgmt()->GetCtrlName()!=NULL)
        strncpy(lpCtrlName,pCtrlTask->GetCtrlMgmt()->GetCtrlName(),MAX_RS_SIZE);
    else
        strncpy(lpCtrlName,"?",MAX_RS_SIZE);
    SetWindowText(hProd,lpCtrlName);
    // Control version
    char lpInfo[MAX_RS_SIZE]={0};
    pCtrlTask->GetCtrlVers(lpInfo,MAX_RS_SIZE);
	SetWindowText(hVers,lpInfo);
    // Control description
    pCtrlTask->GetCtrlDesc(lpInfo,MAX_RS_SIZE);
	SetWindowText(hDescrip,lpInfo);
    // Control icon
    HICON hCtrlIcon=pCtrlTask->GetCtrlIcon();
    if(hCtrlIcon!=NULL)
        SendMessage(hIcon,STM_SETICON,(WPARAM)hCtrlIcon,0);
    else
        SendMessage(hIcon,STM_SETICON,(WPARAM)LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_DEFAULT)),0);
    // Control name:
    char lpTmp[MAX_RS_SIZE]={0};
    LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_STRING_CTRL,lpTmp,MAX_RS_SIZE);
	strncat(lpCtrlName,lpTmp,mHpPak_FindStrLenght(lpTmp,MAX_RS_SIZE));
	SetWindowText(hFmCtrl,lpCtrlName);

    //////
    hWndUI=pCtrlTask->GetCtrlMgmt()->CreateUI(hWndDlgCfg,hWndApply);
    if((hWndUI!=NULL)&&(IsWindow(hWndUI)))
    {
        ////// Check DotNET control platform
        if(pCtrlTask->GetCtrlPlatformID()==PLATFORM_ID_DOTNET)
        {   CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlTask->GetCtrlMgmt());
            pDotNetCtrl->MoveDotNetUI(hWndDlgCfg,hWndUI);
        }
        else ////// Win32 control Platform
            SetWindowPos(hWndUI,hWndApply,13,41,286,139,SWP_HIDEWINDOW);
    }
}

//---------------------------------------------//
// CtrlMan_ProcCfg
//---------------------------------------------//
BOOL CtrlMan_ProcCfg(HWND hCfgDlg,UINT CfgMessage)
{
    CmHpTask* pCtrlTask=GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE);

    ////// Check control platform
    switch(pCtrlTask->GetCtrlPlatformID())
    {   case PLATFORM_ID_DOTNET: ////// DotNET Platform
        {
            CmHpDotNetCtrl* pDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlTask->GetCtrlMgmt());

            // Check control config window message
            switch(CfgMessage)
            {   case WM_MOVE:
                {   pDotNetCtrl->MoveDotNetUI(hCfgDlg,hWndUI);
                    break;
                }
                case WM_DESTROY:
                {   pDotNetCtrl->CloseDotNetUI();
                    return TRUE;
                }
            }
            break;
        }
        case PLATFORM_ID_QT: ////// Qt Platform
        {   if(CfgMessage==WM_DESTROY)
                pCtrlTask->GetCtrlMgmt()->CloseQtUI();
            break;
        }
    }
    return FALSE;
}

//********************************************************************PROCESS//
// CtrlMan_CaptureMsg
//---------------------------------------------//
void CtrlMan_CaptureMsg(HWND hWndActive,WPARAM wpParam,LPARAM lpParam,ImHpCtrlMgmt* pCtrlMgmt,UINT iPlatformID,
                        HWND hToolWnd,int iCurOp,CEventListe* EventObj,char* lpNameApp,UINT &pIdLast,
                        int pCurwnd,int pCntwnd,BOOL &pActionUser,int &pResCapt)
{	int iWriteRes=0;
    WriteImmediatTask(hWndActive,NULL,NULL,hToolWnd,NULL,iCurOp,MHPID_CTRL_MAN,0,
                      EventObj,pCtrlMgmt,iPlatformID,lpNameApp,pIdLast,pCurwnd,pCntwnd,iWriteRes);
    switch(iWriteRes)
    {	case 0:
        {   pActionUser=FALSE;
            SetTimer(hWndActive,ID_TIMER_NEXT,1,(TIMERPROC)mHpTimerProc);
            break;
        }
        case 1:
        {   pResCapt=0;
            break;
        }
        default:
        {   break;}
    }
}

//---------------------------------------------//
// CtrlMan_mHpProcess
//---------------------------------------------//
void CtrlMan_mHpProcess(HWND hWndActive,BOOL &bChangeWnd,int iCurrentWnd,int iCountWnd)
{
    // Switch on current action
    switch(GetmHpVarStruct()->CurAct)
    {
        case ImHpCtrlMgmt::ActMove: ////// MOVE
        {   
            // The Direct2D cursor is in place -> Move event call
            if(!GetmHpVarStruct()->CourantTask->GetCtrlMgmt()->MoveEvent(hWndActive))
                StopmHelp(hWndActive,TRUE,TRUE);
            else // Next action
                SetTimer(hWndActive,ID_TIMER_CTRLMAN,10,(TIMERPROC)mHpTimerProc);
            break;
        }
        case ImHpCtrlMgmt::ActLeftPress: ////// LEFTPRESS
        {   
            // Switch on current mHpTask step
            switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
            {   case 1:
                {   PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_PRESS));
                    SetTimer(hWndActive,ID_TIMER_DELAI,150,(TIMERPROC)mHpTimerProc);
                    break;
                }
                case 2:
                {
                    ////// Left press event call
                    if(!GetmHpVarStruct()->CourantTask->GetCtrlMgmt()->LeftPressEvent(hWndActive))
                        StopmHelp(hWndActive,TRUE,TRUE);
                    else // Next action
                        SetTimer(hWndActive,ID_TIMER_CTRLMAN,10,(TIMERPROC)mHpTimerProc);
                    break;
                }
            }
            break;
        }
        case ImHpCtrlMgmt::ActLeftUnpress: ////// LEFTUNPRESS
        {
            // Switch on current mHpTask step
            switch(GetmHpVarStruct()->CourantTask->GetmHpStep())
            {   case 1:
                {   PlayResourceWAV(MAKEINTRESOURCE(IDR_WAVE_UNPRESS));
		            if(!RestoreBackBuffer(hWndActive))
                    {   StopmHelp(hWndActive,TRUE,TRUE);
                        break;
                    }
                    UpdateBackBuffer(TRUE);
                    // Check if action will change the dialog/window/form
                    if((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=NO)&&
                       ((GetmHpVarStruct()->CourantTask->GetCurrentWnd()!=iCurrentWnd)||
                        (GetmHpVarStruct()->CourantTask->GetCountWnd()!=iCountWnd)))
                    {   bChangeWnd=TRUE;
                    }
                    if(GetmHpVarStruct()->CourantTask->GetCurrentWnd()==NO)
                    {   GetmHpVarStruct()->bNoInactiveMsg=TRUE;
                        if(GetmHpVarStruct()->TaskList->GetNextmHpID()!=NO)
                        {   GetmHpVarStruct()->TaskList->NextCellule();
                            if(GetmHpVarStruct()->imHpMode!=1)
                                WriteCursorPoskey(GetmHpVarStruct()->XposCur,GetmHpVarStruct()->YposCur);
                        }
                        else
                        {   StopmHelp(hWndActive,TRUE,TRUE);
                            break;
                        }
                    }
                    GetmHpVarStruct()->bRefreshDDraw=TRUE;
                    SetDDrawObj(hWndActive,0);
                    if(bChangeWnd)
                        mHpPak_ToolTipsActivate(TRUE);

                    ////// Left unpress event call
                    if(!GetmHpVarStruct()->CourantTask->GetCtrlMgmt()->LeftUnpressEvent(hWndActive))
                        StopmHelp(hWndActive,TRUE,TRUE);
                    else if(!bChangeWnd)
                        SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
                    break;
                }
                case 2:
                {   // Next action
                    SetTimer(hWndActive,ID_TIMER_CTRLMAN,10,(TIMERPROC)mHpTimerProc);
                    break;
                }
            }
            break;
        }
        case ImHpCtrlMgmt::ActRightPress: ////// RIGHTPRESS
        case ImHpCtrlMgmt::ActRightUnpress: ////// RIGHTUNPRESS
        case ImHpCtrlMgmt::ActLeftDblClick: ////// LEFTDBLCLICK
        case ImHpCtrlMgmt::ActRightDblClick: ////// RIGHTDBLCLICK
        case ImHpCtrlMgmt::ActEdit: ////// EDIT
        {
            break;
        }
        case ImHpCtrlMgmt::ActWait: ////// WAIT
        {
            Sleep(100); // Wait a litte

            ////// Wait until event
            if(!GetmHpVarStruct()->CourantTask->GetCtrlMgmt()->WaitUntil(hWndActive))
                StopmHelp(hWndActive,TRUE,TRUE);
            else // Next action
                SetTimer(hWndActive,ID_TIMER_CTRLMAN,10,(TIMERPROC)mHpTimerProc);
            break;
        }
    }
}

//---------------------------------------------//
// CtrlMan_StartProc
//---------------------------------------------//
BOOL CtrlMan_StartProc(HWND hWndActive)
{








    if(GetActiveWindow()!=hWndActive)
        int u=0;
    else
        int u=0;







    ////// Start
    if(!GetmHpVarStruct()->CourantTask->GetCtrlMgmt()->Start(hWndActive))
        return FALSE; // - Failed to get control from control ID
                      // - Not a platform control
                      // - Not a valid window
                      // - Not visible
                      // - Not enabled
                      // - Different control style
                      // - Different class name
                      // - Invalid message ID

    // Check if state can be set to the control
    if((!GetmHpVarStruct()->CourantTask->GetCtrlMgmt()->CheckState(hWndActive))&&
       (GetmHpVarStruct()->CourantTask->GetmHpOption()>=10))
    {   ViewCtrlNotReadyMsg(hWndActive,4);
    }
    else
    {
        //
        // TODO: Add condition
        // -> Not needed when pure assistance (no Help option)
        
        // Check control visible in parent window
        RECT RectWnd;
        GetWindowRect(hWndActive,&RectWnd);
        LONG lx,ly,lcx,lcy;
        if(!GetmHpVarStruct()->CourantTask->GetCtrlMgmt()->GetCtrlPos(hWndActive,lx,ly,lcx,lcy))
            return FALSE; // Failed to get control position in the srceen




        // Tolérance: 5 pixels
        RECT RectCtrl={lx,ly,lx+lcx,ly+lcy};
        if(((RectCtrl.top-RectWnd.top)<(-5))||
           ((RectCtrl.left-RectWnd.left)<(-5))||
           ((RectWnd.bottom-RectCtrl.bottom)<(-5))||
           ((RectWnd.right-RectCtrl.right)<(-5)))
        {
        
            //
            // TODO: Use parent ScrollBar if any to find it
            //
        
        
            return FALSE;
        }




        // Call ID_TIMER_CTRLMAN timer below
        return TRUE;
    }
    return FALSE;
}

//---------------------------------------------//
// CtrlMan_TimerProc
//---------------------------------------------//
void CtrlMan_TimerProc(HWND hWndActive)
{
    ////// NextAction
    ImHpCtrlMgmt* CurCtrlMgmt=GetmHpVarStruct()->CourantTask->GetCtrlMgmt();
    GetmHpVarStruct()->CurAct=CurCtrlMgmt->NextAction(hWndActive);

    // Check next action consistent with previous action
    ImHpCtrlMgmt::ActType PrevActType=CurCtrlMgmt->GetPrevAct();
    if(!CurCtrlMgmt->CheckNextAct(GetmHpVarStruct()->CurAct))
    {
        assert(NULL);

        //
        // TODO: Display an error message
        //
                    
        StopmHelp(hWndActive,TRUE,TRUE);
        return;
    }

    // Reset mHpTask step
    GetmHpVarStruct()->CourantTask->SetmHpStep(0);
    switch(GetmHpVarStruct()->CurAct)
    {
        case ImHpCtrlMgmt::ActMove: ////// MOVE
        {
            if(!CurCtrlMgmt->MoveTo(hWndActive,GetmHpVarStruct()->XnextPos,GetmHpVarStruct()->YnextPos))
                StopmHelp(hWndActive,TRUE,TRUE);
            else // Direct2D: Move
            {   SetDDrawObj(hWndActive,0);
                // Check which Direct2D cursor to use (using previous action)
                switch(PrevActType)
                {   case ImHpCtrlMgmt::ActLeftPress: // Move with mouse left button press
                    {   GetmHpVarStruct()->CursorType=1;
                        break;
                    }
                    case ImHpCtrlMgmt::ActRightPress: // Move with mouse right button press
                    {   GetmHpVarStruct()->CursorType=2;
                        break;
                    }
                }
                EndObjects(hWndActive);
                SetTimer(hWndActive,ID_TIMER_MAXDELAY,60000,(TIMERPROC)mHpTimerProc);
                if(!DoInit(hWndActive)) StopmHelp(hWndActive,TRUE,TRUE);
                else
                    SetTimer(hWndActive,ID_TIMER_DDRAW,1,(TIMERPROC)mHpTimerProc);
            }
            break;
        }
        case ImHpCtrlMgmt::ActLeftPress: ////// LEFTPRESS
        {
            // Press Direct2D left button mouse
            GetmHpVarStruct()->bRefreshDDraw=TRUE;
            GetmHpVarStruct()->CursorType=1;
            SetTimer(hWndActive,ID_TIMER_DDRAW,200,(TIMERPROC)mHpTimerProc);
            break;
        }
        case ImHpCtrlMgmt::ActLeftUnpress: ////// LEFTUNPRESS
        {
            // Call CtrlMan_mHpProcess()
            SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
            break;
        }
        case ImHpCtrlMgmt::ActRightPress: ////// RIGHTPRESS
        {
            break;
        }
        case ImHpCtrlMgmt::ActRightUnpress: ////// RIGHTUNPRESS
        {
            break;
        }
        case ImHpCtrlMgmt::ActLeftDblClick: ////// LEFTDBLCLICK
        {
            break;
        }
        case ImHpCtrlMgmt::ActRightDblClick: ////// RIGHTDBLCLICK
        {
            break;
        }
        case ImHpCtrlMgmt::ActEdit: ////// EDIT
        {
            break;
        }
        case ImHpCtrlMgmt::ActWait: ////// WAIT
        {
            // Call CtrlMan_mHpProcess()
            SetTimer(hWndActive,ID_TIMER_DELAI,1,(TIMERPROC)mHpTimerProc);
            break;
        }
        case ImHpCtrlMgmt::ActEnd: ////// END
		{
            if(GetmHpVarStruct()->TaskList->NextCellule())
            {   GetmHpVarStruct()->bNewDepl=TRUE;
                SetDDrawObj(hWndActive,0);
                GetmHpVarStruct()->CourantTask=GetmHpVarStruct()->TaskList->GetCurrentTask();
                GetNextPos(hWndActive);
            }
            else
                NextRepeatAssistProc(hWndActive);
            break;
        }
        case ImHpCtrlMgmt::ActError: ////// ERROR
        {

            //
            // TODO: Display an error message
            //

            StopmHelp(hWndActive,TRUE,TRUE);
            break;
        }
    }
}

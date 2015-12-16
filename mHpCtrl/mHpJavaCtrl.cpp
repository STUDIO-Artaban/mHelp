// Copyright (c) Unknown Corporation.  All rights reserved.
//

#include "mHpJavaCtrl.h"

// --------------------------------
// Java methods definition
//
static const char* MHPJC_METHOD_ADDJAVAEVENTS[] = {"AddJavaEvents",     "(Z)Z" };
static const char* MHPJC_METHOD_SETDLLVERSION[] = {"GetJarVersion",     "()Ljava/lang/String;" };
static const char* MHPJC_METHOD_GETCTRLCLASS[]  = {"GetCtrlClass",      "(I)Ljava/lang/String;" };
static const char* MHPJC_METHOD_GETCTRLNAME[]   = {"GetCtrlName",       "(I)Ljava/lang/String;" };

//
// TODO: Define all methods here
//

///////////////////////////////////////////////////////////////////////////////
// ImHpCtrlMgmt class implemetation
CmHpJavaCtrl::CmHpJavaCtrl(UINT nIdx,JNIEnv* pEnv,jclass NewCtrlClss,jobject NewCtrlObj) : ImHpCtrlMgmt(nIdx,CData::StrArray),
              jenv(pEnv), jclssCtrl(NewCtrlClss), jobjCtrl(NewCtrlObj),
              jmthd_AddJavaEvents(NULL), jmthd_SetDllVersion(NULL), jmthd_GetCtrlClass(NULL),
              jmthd_GetCtrlName(NULL)
{}
CmHpJavaCtrl::~CmHpJavaCtrl()
{
    // Free Java control object
    jenv->DeleteLocalRef(jobjCtrl);
}

// --------------------------------
// Java tools
//
BOOL CmHpJavaCtrl::InitJavaVM()
{
    // Get mHelp Java control methods
    jmthd_AddJavaEvents=jenv->GetMethodID(jclssCtrl,MHPJC_METHOD_ADDJAVAEVENTS[0],MHPJC_METHOD_ADDJAVAEVENTS[1]);
    if(!jmthd_AddJavaEvents)
    {
        assert(NULL);

        //
        // TODO: Add error message (szErrorMsg)
        //

        return FALSE;
    }
    jmthd_SetDllVersion=jenv->GetMethodID(jclssCtrl,MHPJC_METHOD_SETDLLVERSION[0],MHPJC_METHOD_SETDLLVERSION[1]);
    if(!jmthd_SetDllVersion)
    {
        assert(NULL);

        //
        // TODO: Add error message (szErrorMsg)
        //

        return FALSE;
    }
    jmthd_GetCtrlClass=jenv->GetMethodID(jclssCtrl,MHPJC_METHOD_GETCTRLCLASS[0],MHPJC_METHOD_GETCTRLCLASS[1]);
    if(!jmthd_GetCtrlClass)
    {
        assert(NULL);

        //
        // TODO: Add error message (szErrorMsg)
        //

        return FALSE;
    }
    jmthd_GetCtrlName=jenv->GetMethodID(jclssCtrl,MHPJC_METHOD_GETCTRLNAME[0],MHPJC_METHOD_GETCTRLNAME[1]);
    if(!jmthd_GetCtrlName)
    {
        assert(NULL);

        //
        // TODO: Add error message (szErrorMsg)
        //

        return FALSE;
    }

    // Set Dll version
    jstring jDllVersion=(jstring)jenv->CallObjectMethod(jobjCtrl,jmthd_SetDllVersion,NULL);
    if(jDllVersion)
    {   const char* pDllVersion=jenv->GetStringUTFChars(jDllVersion,0);
        strncpy(szDllVersion,pDllVersion,DLL_VERSION_SIZE);
        jenv->ReleaseStringUTFChars(jDllVersion,pDllVersion);
    }

    return TRUE;
}

BOOL CmHpJavaCtrl::AddJavaEvents(BOOL bCreatedVM) const
{   jboolean jCreatedVM=(jboolean)bCreatedVM;
    return (BOOL)jenv->CallBooleanMethod(jobjCtrl,jmthd_AddJavaEvents,jCreatedVM);
}

// --------------------------------
// Properties
//

////// Class name
BOOL CmHpJavaCtrl::IsCtrlClass(const char* lpClass) const { return FALSE; }
const char* CmHpJavaCtrl::GetCtrlClass(UINT iClassIdx) const
{
    jint jClassIdx=(jint)iClassIdx;
    jstring jClassName=(jstring)jenv->CallObjectMethod(jobjCtrl,jmthd_GetCtrlClass,jClassIdx);
    if(jClassName)
    {   const char* pClassName=jenv->GetStringUTFChars(jClassName,0);
        strncpy(szClass,pClassName,CTRL_CLASS_SIZE);
        jenv->ReleaseStringUTFChars(jClassName,pClassName);
        return szClass;
    }
    return NULL;
}

////// Name
const char* CmHpJavaCtrl::GetCtrlName(int iNameIdx) const
{
    jint jNameIdx=iNameIdx;
    jstring jCtrlName=(jstring)jenv->CallObjectMethod(jobjCtrl,jmthd_GetCtrlName,jNameIdx);
    if(jCtrlName)
    {   const char* pCtrlName=jenv->GetStringUTFChars(jCtrlName,0);
        strncpy(szJavaName,pCtrlName,DATA_NAME_SIZE);
        jenv->ReleaseStringUTFChars(jCtrlName,pCtrlName);
        return szJavaName;
    }
    return NULL;
}

////// Info
void CmHpJavaCtrl::GetCtrlVers(char* lpVers,UINT iMaxSize) const { }
void CmHpJavaCtrl::GetCtrlDesc(char* lpDesc,UINT iMaxSize) const { }
HICON CmHpJavaCtrl::GetIconList(UINT iIconIdx) const { return NULL; }
int CmHpJavaCtrl::GetIconIdx() const { return 0; }

////// User Interface (UI)
HWND CmHpJavaCtrl::CreateUI(HWND hParentUI,HWND hSaveBtn) const { return NULL; }
void CmHpJavaCtrl::SaveUI(HWND hUI) const { }

////// Style
BOOL CmHpJavaCtrl::SetStyle(int iStyleID) { return FALSE; }

////// State
UINT CmHpJavaCtrl::GetStateCount() const { return 0; }
CCtrlState* CmHpJavaCtrl::GetState(UINT iStateIdx) { return NULL; }
BOOL CmHpJavaCtrl::NeedToCheckState() const { return FALSE; }
const char* CmHpJavaCtrl::DisplayState() { return NULL; }

////// Config
UINT CmHpJavaCtrl::GetCfgCount() const { return 0; }
CCtrlCfg* CmHpJavaCtrl::GetConfig(UINT iCfgIdx) { return NULL; }

////// HTML tags
void CmHpJavaCtrl::GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const { }

// --------------------------------
// Capture processus
//
BOOL CmHpJavaCtrl::SetCaptMsgID(int iMsgID) { return FALSE; }
BOOL CmHpJavaCtrl::CaptureWin32(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam) { return FALSE; }

// --------------------------------
// Reproduce processus
//
BOOL CmHpJavaCtrl::GetCtrlPos(HWND hDlg,LONG &x,LONG &y,LONG &cx,LONG &cy) const { return FALSE; }
BOOL CmHpJavaCtrl::CheckState(HWND hDlg) const { return FALSE; }

ImHpCtrlMgmt::ActType CmHpJavaCtrl::NextAction(HWND hDlg) { return ActError; }

BOOL CmHpJavaCtrl::Start(HWND hDlg) { return FALSE; }
BOOL CmHpJavaCtrl::End(HWND hDlg) { return FALSE; }

BOOL CmHpJavaCtrl::MoveTo(HWND hDlg,int &xPos,int &yPos) { return FALSE; }
BOOL CmHpJavaCtrl::MoveEvent(HWND hDlg) { return FALSE; }
BOOL CmHpJavaCtrl::WaitUntil(HWND hDlg) { return FALSE; }

BOOL CmHpJavaCtrl::LeftPressEvent(HWND hDlg) { return FALSE; }
BOOL CmHpJavaCtrl::LeftUnpressEvent(HWND hDlg) { return FALSE; }
BOOL CmHpJavaCtrl::RightPressEvent(HWND hDlg) { return FALSE; }
BOOL CmHpJavaCtrl::RightUnpressEvent(HWND hDlg) { return FALSE; }
BOOL CmHpJavaCtrl::EditEvent(HWND hDlg) { return FALSE; }

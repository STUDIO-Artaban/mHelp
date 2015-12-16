// mHpQtCtrl.cpp - mHelp Qt Controls Management Interface implementation
//

#include "mHpQtCtrlMgmt.h"
#include "mhpqtctrlmgmt_global.h"

#include <QtGui>
#include <QMetaMethod>
#include <qobject_p.h>

////// ImHpQtCtrlMgmt interface ///////////////////////////////////////////////
ImHpQtCtrlMgmt* ImHpQtCtrlMgmt::getInstance(UINT iIdx, const char* lpDllPath) {

    return new CmHpQtCtrl(iIdx, lpDllPath);
}

////// CmHpQtCtrlMgmt class ///////////////////////////////////////////////////
CmHpQtCtrlMgmt::CmHpQtCtrlMgmt(UINT iIdx, CData::Type TypeID) : ImHpCtrlMgmt(iIdx, TypeID) {
    QtCtrlMgmt = 0;
    iStateCount = 0;
    iCfgCount = 0;
    *szName = 0;
}
CmHpQtCtrlMgmt::~CmHpQtCtrlMgmt() {
    if (QtCtrlMgmt)
        delete QtCtrlMgmt;
}

// --------------------------------
// Qt tools
//
BOOL CmHpQtCtrlMgmt::FillQtData() {
    if (QtCtrlMgmt) {

        // Object name
        QString strClass(QtCtrlMgmt->GetCtrlObject());
        if ((!strClass.isNull()) && (!strClass.isEmpty()))
            strncpy(szClass, strClass.toAscii().constData(), CTRL_CLASS_SIZE);
        else
            *szClass = 0;

        // Control style ID & Captured signal ID
        iCtrlStyleID = QtCtrlMgmt->GetStyle();
        iCaptMsgID = QtCtrlMgmt->GetCaptSgnID();

        // Check if control style is defined
        // -> Otherwise state, config & ID will not be defined
        if (iCtrlStyleID != NO_VALUE) {

            // State
            if (pState) {
                assert(iStateCount != 0);
                for (UINT i = 0; i < iStateCount; i++)
                    delete pState[i];
                delete [] pState;
                pState = NULL;
            }
            iStateCount = QtCtrlMgmt->GetStateCount();
            if (iStateCount) {
                pState = new CCtrlState*[iStateCount];
                for (UINT i = 0; i < iStateCount; i ++) {
                    QString strStateName(QtCtrlMgmt->GetState(i)->GetName());
                    pState[i] = new CCtrlState(strStateName.toAscii().constData(),
                                               (CData::Type)QtCtrlMgmt->GetState(i)->GetType());
                    switch (pState[i]->GetType()) {
                        case CData::Integer: {
                            if (QtCtrlMgmt->GetState(i)->GetValue() != 0) {
                                const int* pIntVal = (const int*)QtCtrlMgmt->GetState(i)->GetValue();
                                pState[i]->SetValue(*pIntVal);
                            }
                            break;
                        }
                        case CData::IntArray: {
                            for (UINT j = 0; j < QtCtrlMgmt->GetState(i)->GetCount(); j++) {
                                const int* pIntVal = (const int*)QtCtrlMgmt->GetState(i)->GetValue(j);
                                pState[i]->AddValue(*pIntVal);
                            }
                            break;
                        }
                        case CData::String: {
                            if (QtCtrlMgmt->GetState(i)->GetValue() != 0) {
                                const QString* pStrVal = (const QString*)QtCtrlMgmt->GetState(i)->GetValue();
                                pState[i]->SetValue(pStrVal->toAscii().constData());
                            }
                            break;
                        }
                        case CData::StrArray: {
                            for (UINT j = 0; j < QtCtrlMgmt->GetState(i)->GetCount(); j++) {
                                const QString* pStrVal = (const QString*)QtCtrlMgmt->GetState(i)->GetValue(j);
                                pState[i]->AddValue(pStrVal->toAscii().constData());
                            }
                            break;
                        }
                    }
                }
            }

            // Config
            if (pCfg) {
                assert(iCfgCount != 0);
                for (UINT i = 0; i < iCfgCount; i++)
                    delete pCfg[i];
                delete [] pCfg;
                pCfg = NULL;
            }
            iCfgCount = QtCtrlMgmt->GetCfgCount();
            if (iCfgCount) {
                pCfg = new CCtrlCfg*[iCfgCount];
                for (UINT i = 0; i < iCfgCount; i ++) {
                    QString strCfgName(QtCtrlMgmt->GetConfig(i)->GetName());
                    pCfg[i] = new CCtrlCfg(strCfgName.toAscii().constData(),
                                           (CData::Type)QtCtrlMgmt->GetConfig(i)->GetType());
                    switch (pCfg[i]->GetType()) {
                        case CData::Integer: {
                            if (QtCtrlMgmt->GetConfig(i)->GetValue() != 0) {
                                const int* pIntVal = (const int*)QtCtrlMgmt->GetConfig(i)->GetValue();
                                pCfg[i]->SetValue(*pIntVal);
                            }
                            break;
                        }
                        case CData::IntArray: {
                            for (UINT j = 0; j < QtCtrlMgmt->GetConfig(i)->GetCount(); j++) {
                                const int* pIntVal = (const int*)QtCtrlMgmt->GetConfig(i)->GetValue(j);
                                pCfg[i]->AddValue(*pIntVal);
                            }
                            break;
                        }
                        case CData::String: {
                            if (QtCtrlMgmt->GetConfig(i)->GetValue() != 0) {
                                const QString* pStrVal = (const QString*)QtCtrlMgmt->GetConfig(i)->GetValue();
                                pCfg[i]->SetValue(pStrVal->toAscii().constData());
                            }
                            break;
                        }
                        case CData::StrArray: {
                            for (UINT j = 0; j < QtCtrlMgmt->GetConfig(i)->GetCount(); j++) {
                                const QString* pStrVal = (const QString*)QtCtrlMgmt->GetConfig(i)->GetValue(j);
                                pCfg[i]->AddValue(pStrVal->toAscii().constData());
                            }
                            break;
                        }
                    }
                }
            }

            // ID
            if (pID) {
                delete pID;
            }
            pID = new CCtrlID(CTRL_ID_NAME, CData::StrArray);
            for (UINT j = 0; j < QtCtrlMgmt->GetCtrlID()->GetCount(); j++) {
                const QString* pStrID = (const QString*)QtCtrlMgmt->GetCtrlID()->GetValue(j);
                pID->AddValue(pStrID->toAscii().constData());
            }
        }

        // Default state bound
        QString strStateBound(QtCtrlMgmt->GetStateBound());
        if ((!strStateBound.isNull()) && (!strStateBound.isEmpty()))
            strncpy(szStateBound, strStateBound.toAscii().constData(), STATE_BOUND_SIZE);
        else
            *szStateBound = 0;

        // Dll version
        QString strDllVers(QtCtrlMgmt->GetDllVersion());
        if ((!strDllVers.isNull()) && (!strDllVers.isEmpty()))
            strncpy(szDllVersion, strDllVers.toAscii().constData(), DLL_VERSION_SIZE);
        else
            *szDllVersion = 0;

        return TRUE;
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::UpdateQtData() {
    if (QtCtrlMgmt) {
        
        // Set Qt control style
        if (!QtCtrlMgmt->SetStyle(iCtrlStyleID))
            return FALSE;

        // Set Qt control object name
        QString strObject(szClass);
        QtCtrlMgmt->SetCtrlObject(strObject);

        // Set Qt control state bound
        QString strSateBound(szStateBound);
        QtCtrlMgmt->SetStateBound(strSateBound);

        // Set Qt capture event identifier
        if(!QtCtrlMgmt->SetCaptSgnID(iCaptMsgID))
            return FALSE;

        // Set Qt state
        // -> Qt state should be defined after a valid 'SetStyle' call
        if (pState) {
            for (UINT i = 0; i < iStateCount; i++) {
                if (!QtCtrlMgmt->GetState(i))
                    return FALSE; // Unconsistent data definition
                switch (pState[i]->GetType()) {
                    case CData::Integer: {
                        if (QtCtrlMgmt->GetState(i)->GetType() != QmHpData::Integer)
                            return FALSE; // Unconsistent data type
                        QtCtrlMgmt->GetState(i)->ClearValue();
                        if (pState[i]->GetCount()) {
                            const int* iState = (const int*)pState[i]->GetValue();
                            QtCtrlMgmt->GetState(i)->SetValue(*iState);
                        }
                        //else // No data yet
                        break;
                    }
                    case QmHpData::IntArray: {
                        if (QtCtrlMgmt->GetState(i)->GetType() != QmHpData::IntArray)
                            return FALSE; // Unconsistent data type
                        QtCtrlMgmt->GetState(i)->ClearValue();
                        for (UINT j = 0; j < pState[i]->GetCount(); j++) {
                            const int* iState = (const int*)pState[i]->GetValue(j);
                            QtCtrlMgmt->GetState(i)->AddValue(*iState);
                        }
                        break;
                    }
                    case CData::String: {
                        if (QtCtrlMgmt->GetState(i)->GetType() != QmHpData::String)
                            return FALSE; // Unconsistent data type
                        QtCtrlMgmt->GetState(i)->ClearValue();
                        if (pState[i]->GetCount()) {
                            const char* lpState = (const char*)pState[i]->GetValue();
                            QString strState(lpState);
                            QtCtrlMgmt->GetState(i)->SetValue(strState);
                        }
                        //else // No data yet
                        break;
                    }
                    case QmHpData::StrArray: {
                        if (QtCtrlMgmt->GetState(i)->GetType() != QmHpData::StrArray)
                            return FALSE; // Unconsistent data type
                        QtCtrlMgmt->GetState(i)->ClearValue();
                        for (UINT j = 0; j < pState[i]->GetCount(); j++) {
                            const char* lpState = (const char*)pState[i]->GetValue(j);
                            QString strState(lpState);
                            QtCtrlMgmt->GetState(i)->AddValue(strState);
                        }
                        break;
                    }
                }
            }
        }

        // Set Qt config
        // -> Qt config should be defined after a valid 'SetStyle' call
        if (pCfg) {
            for (UINT i = 0; i < iCfgCount; i++) {
                if (!QtCtrlMgmt->GetConfig(i))
                    return FALSE; // Unconsistent data definition
                switch (pCfg[i]->GetType()) {
                    case CData::Integer: {
                        if (QtCtrlMgmt->GetConfig(i)->GetType() != QmHpData::Integer)
                            return FALSE; // Unconsistent data type
                        QtCtrlMgmt->GetConfig(i)->ClearValue();
                        if (pCfg[i]->GetCount()) {
                            const int* iCfg = (const int*)pCfg[i]->GetValue();
                            QtCtrlMgmt->GetConfig(i)->SetValue(*iCfg);
                        }
                        // else // No data yet
                        break;
                    }
                    case QmHpData::IntArray: {
                        if (QtCtrlMgmt->GetConfig(i)->GetType() != QmHpData::IntArray)
                            return FALSE; // Unconsistent data type
                        QtCtrlMgmt->GetConfig(i)->ClearValue();
                        for (UINT j = 0; j < pCfg[i]->GetCount(); j++) {
                            const int* iCfg = (const int*)pCfg[i]->GetValue(j);
                            QtCtrlMgmt->GetConfig(i)->AddValue(*iCfg);
                        }
                        break;
                    }
                    case CData::String: {
                        if (QtCtrlMgmt->GetConfig(i)->GetType() != QmHpData::String)
                            return FALSE; // Unconsistent data type
                        QtCtrlMgmt->GetConfig(i)->ClearValue();
                        if (pCfg[i]->GetCount()) {
                            const char* lpCfg = (const char*)pCfg[i]->GetValue();
                            QString strCfg(lpCfg);
                            QtCtrlMgmt->GetConfig(i)->SetValue(strCfg);
                        }
                        // else // No data yet
                        break;
                    }
                    case QmHpData::StrArray: {
                        if (QtCtrlMgmt->GetConfig(i)->GetType() != QmHpData::StrArray)
                            return FALSE; // Unconsistent data type
                        QtCtrlMgmt->GetConfig(i)->ClearValue();
                        for (UINT j = 0; j < pCfg[i]->GetCount(); j++) {
                            const char* lpCfg = (const char*)pCfg[i]->GetValue(j);
                            QString strCfg(lpCfg);
                            QtCtrlMgmt->GetConfig(i)->AddValue(strCfg);
                        }
                        break;
                    }
                }
            }
        }

        // Set Qt control ID
        // -> Qt control ID should be defined in Qt constructor
        if (pID) {
            if (!QtCtrlMgmt->GetCtrlID())
                return FALSE; // Unconsistent data definition
            if (QtCtrlMgmt->GetCtrlID()->GetType() != QmHpData::StrArray)
                return FALSE; // Unconsistent data definition
            QtCtrlMgmt->GetCtrlID()->ClearValue();
            for (UINT j = 0; j < pID->GetCount(); j++) {
                const char* lpID = (const char*)pID->GetValue(j);
                QString strID(lpID);
                QtCtrlMgmt->GetCtrlID()->AddValue(strID);
            }
        }

        return TRUE;
    }
    return FALSE;
}
void CmHpQtCtrlMgmt::CloseQtUI() {
    if (QtCtrlMgmt)
        QtCtrlMgmt->CloseUI();
}
void CmHpQtCtrlMgmt::SetQtCtrlMgmt(QmHpCtrlMgmt* pQtCtrl) {
    QtCtrlMgmt = pQtCtrl;
}

// --------------------------------
// Properties
//

////// Object name
BOOL CmHpQtCtrlMgmt::IsCtrlClass(const char* lpClass) const {
    if (QtCtrlMgmt) {
        QString strClass(lpClass);
        return QtCtrlMgmt->IsCtrlObject(strClass);
    }
    return FALSE;
}
const char* CmHpQtCtrlMgmt::GetCtrlClass(UINT iClassIdx) const {
    if (QtCtrlMgmt) {
        QString strClass(QtCtrlMgmt->GetCtrlObject(iClassIdx));
        strncpy(szClass, strClass.toAscii().constData(), CTRL_CLASS_SIZE);
        return szClass;
    }
    return NULL;
}

////// Name
const char* CmHpQtCtrlMgmt::GetCtrlName(int iNameIdx) const {
    if (QtCtrlMgmt) {
        QString strName(QtCtrlMgmt->GetCtrlName(iNameIdx));
        if ((strName.isNull()) || (strName.isEmpty()))
            return NULL;
        strncpy(szName, strName.toAscii().constData(), DATA_NAME_SIZE);
        return szName;
    }
    return NULL;
}

////// Info
void CmHpQtCtrlMgmt::GetCtrlVers(char* lpVers, UINT iMaxSize) const {
    if (QtCtrlMgmt) {
        QString strVersion;
        QtCtrlMgmt->GetCtrlVers(strVersion);
        if ((strVersion.isNull()) || (strVersion.isEmpty()))
            *lpVers = 0;
        else
            strncpy(lpVers, strVersion.toAscii().constData(), iMaxSize);
    }
    else
        *lpVers = 0;
}
void CmHpQtCtrlMgmt::GetCtrlDesc(char* lpDesc, UINT iMaxSize) const {
    if (QtCtrlMgmt) {
        QString strDesc;
        QtCtrlMgmt->GetCtrlDesc(strDesc);
        if ((strDesc.isNull()) || (strDesc.isEmpty()))
            *lpDesc = 0;
        else
            strncpy(lpDesc, strDesc.toAscii().constData(), iMaxSize);
    }
    else
        *lpDesc = 0;
}
HICON CmHpQtCtrlMgmt::GetIconList(UINT iIconIdx) const {
    if (QtCtrlMgmt) {
        HICON hCtrlIcon = QtCtrlMgmt->GetIconList(iIconIdx);
        if (hCtrlIcon) {
            // Check valid icon
            ICONINFO iconInfo;
            if (GetIconInfo(hCtrlIcon, &iconInfo))
                // Check expected icon size
                if ((iconInfo.xHotspot == 16) && (iconInfo.yHotspot == 16))
                    return hCtrlIcon;
        }
    }
    return NULL;
}
int CmHpQtCtrlMgmt::GetIconIdx() const {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->GetIconIdx();
    }
    return NO_VALUE;
}

////// User Interface (UI)
HWND CmHpQtCtrlMgmt::CreateUI(HWND hParentUI,HWND hSaveBtn) const {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->CreateUI(hParentUI, hSaveBtn);
    }
    return NULL;
}
void CmHpQtCtrlMgmt::SaveUI(HWND hUI) const {
    if (QtCtrlMgmt) {
        QtCtrlMgmt->SaveUI(hUI);
    }
}

////// Style
BOOL CmHpQtCtrlMgmt::SetStyle(int iStyleID) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->SetStyle(iStyleID);
    }
    return FALSE;
}

////// State
UINT CmHpQtCtrlMgmt::GetStateCount() const {
    if (QtCtrlMgmt) {
        return iStateCount;
    }
    return 0;
}
CCtrlState* CmHpQtCtrlMgmt::GetState(UINT iStateIdx) {
    if (QtCtrlMgmt) {
        if (iStateIdx < iStateCount)
            return pState[iStateIdx];
    }
    return NULL;
}
BOOL CmHpQtCtrlMgmt::NeedToCheckState() const {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->NeedToCheckState();
    }
    return FALSE;
}
const char* CmHpQtCtrlMgmt::DisplayState() {
    if (QtCtrlMgmt) {
        QString strDisplay(QtCtrlMgmt->DisplayState());
        if (szDisplayState)
            delete [] szDisplayState;
        szDisplayState = new char[strDisplay.length() + 1];
        strncpy(szDisplayState, strDisplay.toAscii().constData(), strDisplay.length() + 1);
        return szDisplayState;
    }
    return NULL;
}

void CmHpQtCtrlMgmt::SetStateCount(UINT iCount) {
    iStateCount = iCount;
}

////// Config
UINT CmHpQtCtrlMgmt::GetCfgCount() const {
    if (QtCtrlMgmt) {
        return iCfgCount;
    }
    return 0;
}
CCtrlCfg* CmHpQtCtrlMgmt::GetConfig(UINT iCfgIdx) {
    if (QtCtrlMgmt) {
        if (iCfgIdx < iCfgCount)
            return pCfg[iCfgIdx];
    }
    return NULL;
}

void CmHpQtCtrlMgmt::SetCfgCount(UINT iCount) {
    iCfgCount = iCount;
}

////// HTML tags
void CmHpQtCtrlMgmt::GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const {
    if (QtCtrlMgmt) {
        QString strTags;
        QtCtrlMgmt->GetHtmlTags(strTags, iTaskIdx);
        if ((strTags.isNull()) || (strTags.isEmpty()))
            *lpTags = 0;
        else
            strncpy(lpTags, strTags.toAscii().constData(), iMaxSize);
    }
    else
        *lpTags = 0;
}

// --------------------------------
// Capture processus
//
BOOL CmHpQtCtrlMgmt::SetCaptMsgID(int iMsgID) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->SetCaptSgnID(iMsgID);
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::CaptureWin32(HWND, UINT Message, WPARAM, LPARAM) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->CaptureSignal(Message);
    }
    return FALSE;
}

// --------------------------------
// Reproduce processus
//
BOOL CmHpQtCtrlMgmt::GetCtrlPos(HWND, LONG &x,LONG &y,LONG &cx,LONG &cy) const {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->GetCtrlPos(x, y, cx, cy);
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::CheckState(HWND) const {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->CheckState();
    }
    return FALSE;
}

ImHpCtrlMgmt::ActType CmHpQtCtrlMgmt::NextAction(HWND) {
    if (QtCtrlMgmt) {
        return (ImHpCtrlMgmt::ActType)QtCtrlMgmt->NextAction();
    }
    return ActError;
}

BOOL CmHpQtCtrlMgmt::Start(HWND hDlg) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->Start(hDlg);
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::End(HWND) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->End();
    }
    return FALSE;
}

BOOL CmHpQtCtrlMgmt::MoveTo(HWND, int &x, int &y) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->MoveTo(x, y);
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::MoveEvent(HWND) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->MoveEvent();
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::WaitUntil(HWND) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->WaitUntil();
    }
    return FALSE;
}

BOOL CmHpQtCtrlMgmt::LeftPressEvent(HWND) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->LeftPressEvent();
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::LeftUnpressEvent(HWND) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->LeftUnpressEvent();
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::RightPressEvent(HWND) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->RightPressEvent();
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::RightUnpressEvent(HWND) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->RightUnpressEvent();
    }
    return FALSE;
}
BOOL CmHpQtCtrlMgmt::EditEvent(HWND) {
    if (QtCtrlMgmt) {
        return QtCtrlMgmt->EditEvent();
    }
    return FALSE;
}

////// CmHpQtCtrl class ///////////////////////////////////////////////////////
CmHpQtCtrl::CmHpQtCtrl(UINT iIdx, const char* lpDllPath, QObject* parent) : QObject(parent) {

    strncpy(szDllPath,lpDllPath,MAX_DLL_PATH);
    iIndex=iIdx;

    QtCtrlLib = 0;
    QtCtrlInst = 0;
}
CmHpQtCtrl::~CmHpQtCtrl() {
    if (QtCtrlLib) {
        if (QtCtrlLib->isLoaded())
            QtCtrlLib->unload();
        delete QtCtrlLib;
    }
    if (QtCtrlInst)
        delete QtCtrlInst;
    if (mHpQtCtrlInst)
        delete mHpQtCtrlInst;
}

//////
BOOL CmHpQtCtrl::LoadQtLibrary() {

    // Load Qt library
    QtCtrlLib = new QLibrary(szDllPath);

    PFN_CREATENEWQTINST NewQtInst = (PFN_CREATENEWQTINST)QtCtrlLib->resolve("CreateNewInstance");
    if (NewQtInst) {

        QtCtrlInst = (QmHpCtrlMgmt*)NewQtInst(iIndex); 
        if (QtCtrlInst) {

            mHpQtCtrlInst = new CmHpQtCtrlMgmt(iIndex, (CData::Type)QtCtrlInst->GetCtrlID()->GetType());
            mHpQtCtrlInst->SetQtCtrlMgmt(QtCtrlInst);
            return TRUE;
        }
    }

    // Failed
    delete QtCtrlLib;
    QtCtrlLib = 0;

    return FALSE;
}
void CmHpQtCtrl::ClearQtData() {

    // Check QmHpCtrlMgmt instance
    if (QtCtrlInst)
        QtCtrlInst->ClearData();
}
void CmHpQtCtrl::CopyQtData(const ImHpCtrlMgmt& CopyData) {

    // Check CmHpQtCtrlMgmt instance
    if (mHpQtCtrlInst) {

        // Copy CmHpQtCtrlMgmt data
        mHpQtCtrlInst->SetStateCount(CopyData.GetStateCount());
        mHpQtCtrlInst->SetCfgCount(CopyData.GetCfgCount());

        // Copy ImHpCtrlMgmt data
        CopymHpCtrlMgmt(*mHpQtCtrlInst,
                        CopyData,
                        CopyData.GetStateCount(),
                        CopyData.GetCfgCount());
    }
}
BOOL CmHpQtCtrl::AddQtSlots(HWND hDlg) {

    // Check QmHpCtrlMgmt & CmHpQtCtrlMgmt instances
    if ((!QtCtrlInst) || (!mHpQtCtrlInst))
        return FALSE;

    return QtCtrlInst->AddmHpSlots(hDlg);
}
ImHpCtrlMgmt* CmHpQtCtrl::GetQtCtrlMgmt() {
    return dynamic_cast<ImHpCtrlMgmt*>(mHpQtCtrlInst);
}

///////////////////////////////////////////////////////////////////////////////
void* CreateNewQtInst(UINT iIdx,LPCSTR lpDllPath)
{
    return static_cast<void*>(ImHpQtCtrlMgmt::getInstance(iIdx,lpDllPath));
}

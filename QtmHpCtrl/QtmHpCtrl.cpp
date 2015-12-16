// QtmHpCtrl.cpp
//

#include "QtmHpCtrl.h"
#include <assert.h>

///////////////////////////////////////////////////////////////////////////////
QmHpData::QmHpData(const QString& strDataName, Type eDataType, QObject* parent) : QObject(parent),
          DataType(eDataType), pValue(0), iCount(0) {
    strName = strDataName;
    strName.resize(DATA_NAME_SIZE);
}
QmHpData::QmHpData(QObject* parent) : QObject(parent),
          DataType(Undefined), pValue(0), iCount(0) {
}
QmHpData::~QmHpData() {
    ClearValue();
}

////// Common
const QString& QmHpData::GetName() const { return strName; }
QmHpData::Type QmHpData::GetType() const { return DataType; }
UINT QmHpData::GetCount() const { return iCount; }
const void* QmHpData::GetValue(UINT iIdx) const { return (iIdx<iCount)? pValue[iIdx]:0; }

void QmHpData::SetValue(int iNewVal,UINT iIdx) {
    switch (DataType) {
        case Integer: {
            assert(!iIdx);
            if (iIdx)
                return; // Failed: Not an array

            if (pValue != 0) {
                delete pValue[0];
                pValue[0] = new int(iNewVal);
            }
            else {
                // Define one integer pointer
                pValue = new void*[1];
                pValue[0] = new int(iNewVal);
                iCount = 1;
            }
            break;
        }
        case IntArray: {
            assert(iIdx < iCount);
            if (iIdx >= iCount)
                return; // Failed: Wrong index

            delete pValue[iIdx];
            pValue[iIdx] = new int(iNewVal);
            break;
        }
        case String:
        case StrArray:
        case Undefined: {
            break; // Failed: Wrong data type
        }
    }
}
void QmHpData::SetValue(const QString& strNewVal, UINT iIdx) {
    switch (DataType) {
        case String: {
            assert(!iIdx);
            if (iIdx)
                return; // Failed: Not an array

            if (pValue != 0) {
                delete [] pValue[0];
                pValue[0] = new QString(strNewVal);
            }
            else {
                // Define one string pointer
                pValue = new void*[1];
                pValue[0] = new QString(strNewVal);
                iCount = 1;
            }
            break;
        }
        case StrArray: {
            assert(iIdx < iCount);
            if (iIdx >= iCount)
                return; // Failed: Wrong index

            delete [] pValue[iIdx];
            pValue[iIdx] = new QString(strNewVal);
            break;
        }
        case Integer:
        case IntArray:
        case Undefined: {
            break; // Failed: Wrong data type
        }
    }
}
void QmHpData::ClearValue() {
    if (pValue) {
        for (UINT i = 0; i < iCount; i++) {
            if (pValue[i]) {
                delete pValue[i];
            }
        }
        delete [] pValue;
    }
    pValue = 0;
}

////// Array
void QmHpData::AddValue(int iNewVal) {
    assert(DataType == IntArray);
    if (DataType != IntArray)
        return; // Failed: Integer array only

    if (pValue != 0) {
        void** pLastVal = pValue;
        pValue = new void*[iCount + 1];
        for (UINT i = 0; i < iCount; i++)
            pValue[i] = pLastVal[i];
        delete [] pLastVal;

        pValue[iCount] = new int(iNewVal);
        iCount++;
    }
    else {
        pValue = new void*[1];
        pValue[0] = new int(iNewVal);
        iCount = 1;
    }
}
void QmHpData::AddValue(const QString& strNewVal) {
    assert(DataType == StrArray);
    if (DataType != StrArray)
        return; // Failed: String array only

    if (pValue != 0) {
        void** pLastVal = pValue;
        pValue = new void*[iCount + 1];
        for (UINT i = 0; i < iCount; i++)
            pValue[i] = pLastVal[i];
        delete [] pLastVal;

        pValue[iCount] = new QString(strNewVal);
        iCount++;
    }
    else {
        pValue = new void*[1];
        pValue[0] = new QString(strNewVal);
        iCount = 1;
    }
}
void QmHpData::Reverse() {
    assert((DataType == IntArray) || (DataType == StrArray));
    if ((DataType != IntArray) && (DataType != StrArray))
        return; // Failed: Not an array

    for (UINT i = 0; i < (iCount/2); i++) {
        void* pVal = pValue[i];
        pValue[i] = pValue[iCount - i - 1];
        pValue[iCount - i - 1] = pVal;
    }
}

///////////////////////////////////////////////////////////////////////////////
QmHpCtrlMgmt::QmHpCtrlMgmt(UINT iIdx, QmHpData::Type TypeID, QObject* parent) : QObject(parent),
                           iIndex(iIdx), iCtrlStyleID(NO_VALUE),
                           iCaptSgnID(NO_VALUE), strErrorMsg(0), strDisplayState(0), iActStep(0),
                           Control(0), pName(0), pState(0), pCfg(0) {
    pID = new QCtrlID(CTRL_ID_NAME, TypeID);
}
QmHpCtrlMgmt::~QmHpCtrlMgmt() {
    if (strErrorMsg)
        delete strErrorMsg;
    if (strDisplayState)
        delete strDisplayState;
    if (pID)
        delete pID;
}

//////
UINT QmHpCtrlMgmt::GetIndex() const { return iIndex; }
const QString& QmHpCtrlMgmt::GetLastErr() const { return *strErrorMsg; }
void QmHpCtrlMgmt::ClearData() {

    assert(iCtrlStyleID != NO_VALUE); // State/config should be defined!

    // Clear state & config
    if (GetStateCount()) {
        for (UINT i = 0; i < GetStateCount(); i++) {
            delete pState[i];
        }
        delete [] pState;
        pState = 0;
    }
    if (GetCfgCount()) {
        for (UINT i = 0; i < GetCfgCount(); i++) {
            delete pCfg[i];
        }
        delete [] pCfg;
        pCfg = 0;
    }

    // Clear properties
    iCtrlStyleID = NO_VALUE;
    iCaptSgnID = NO_VALUE;
    strObject = NULL_QSTRING;
    if (strDisplayState) {
        delete strDisplayState;
        strDisplayState = 0;
    }

    // Clear control identifier
    pID->ClearValue();
}

// --------------------------------
// Qt tools
//
BOOL QmHpCtrlMgmt::SetQtCtrlID(QWidget* Sender) {

    // Check 'pID' definition
    if ((!pID) || (pID->GetType() != QmHpData::StrArray))
        return FALSE;
    pID->ClearValue();

    // Set standard Qt control identifier (using 'QWidget.objectName')
    pID->AddValue(Sender->objectName());

    QWidget* ParentWidget = Sender;
    while (ParentWidget->parentWidget()) {

        ParentWidget = ParentWidget->parentWidget();
        if (ParentWidget->parentWidget()) // Not the widget Form (parentWidget == 0)
            pID->AddValue(ParentWidget->objectName());
    }

    pID->Reverse();
    return TRUE;
}
BOOL QmHpCtrlMgmt::GetQtCtrlObject(HWND hDlg) {

    Control = 0;

    // Get widget Form from handle
    QWidget* ParentWidget = QWidget::find(hDlg);

    // Check valid widget Form and control identifier definition
    if ((ParentWidget) && (pID) && (pID->GetType() == QmHpData::StrArray)) {

        // Set widget control (or 0 if failed)
        Control = ParentWidget;
        UINT iIdIdx = 0;
        do {
            // Find child widget using 'pID'
            const QString* strWidgetID = (const QString*)pID->GetValue(iIdIdx++);
            Control = Control->findChild<QWidget*>(*strWidgetID);

        } while ((iIdIdx < pID->GetCount()) && (Control));

        if (Control != 0)
            return TRUE; // Control widget found
    }

    // Failed to find widget control
    return FALSE;
}

// --------------------------------
// Properties
//
const QString& QmHpCtrlMgmt::GetDllVersion() const { return strDllVersion; }

////// ID
QCtrlID* QmHpCtrlMgmt::GetCtrlID() { return pID; }

////// Qt Object name
const QString& QmHpCtrlMgmt::GetCtrlObject() const { return strObject; }
void QmHpCtrlMgmt::SetCtrlObject(const QString& strObj) {
    strObject = strObj;
    strObject.resize(CTRL_OBJECT_SIZE);
}

////// Style
int QmHpCtrlMgmt::GetStyle() const { return iCtrlStyleID; }

////// State
const QString& QmHpCtrlMgmt::GetStateBound() const { return strStateBound; }
void QmHpCtrlMgmt::SetStateBound(const QString& strBound) { strStateBound = strBound; }

// --------------------------------
// Capture processus
//
BOOL QmHpCtrlMgmt::AddmHpSlot(const QString& strmHpSgn) {

    // Check if mHelp window message already exists
    if (hashmHpSgn.contains(strmHpSgn))
        return TRUE;

    // Create new window message to inform mHelp a control has emit a signal
    UINT imHpWndMsg = RegisterWindowMessage(strmHpSgn.toAscii().constData());
    if (!imHpWndMsg)
        return FALSE; // Failed to create window message

    // Add new mHelp window message
    hashmHpSgn[strmHpSgn] = imHpWndMsg;

    return TRUE;
}
void QmHpCtrlMgmt::SendmHpSignal(QWidget* Sender, const QString& strmHpSgn) {

    // Check existing signal to inform mHelp a control signal occurred (set 'iCaptSgnID')
    iCaptSgnID = lstmHpSignal.indexOf(strmHpSgn);
    if (iCaptSgnID == NO_VALUE)
        return; // Failed: mHelp signal not exist!

    // Check mHelp window message to send (associated to signal)
    if (!hashmHpSgn.contains(strmHpSgn))
        return; // Failed: No associated mHelp window message!

    UINT imHpWndMsg = hashmHpSgn[strmHpSgn];

    // Get parent Form handle
    QWidget* ParentWidget = Sender;
    while (ParentWidget->parentWidget())
        ParentWidget = ParentWidget->parentWidget();

    // Send message to inform mHelp a Qt control signal has occurred
    SendMessage(ParentWidget->winId(), imHpWndMsg, 0, 0);
}

int QmHpCtrlMgmt::GetCaptSgnID() const { return iCaptSgnID; }
BOOL QmHpCtrlMgmt::SetCaptSgnID(int iSgnID) {

    iCaptSgnID = NO_VALUE;

    // Check existing mHelp signal
    if (lstmHpSignal.isEmpty())
        return FALSE;

    // Check valid signal identifier (index of mHelp signal list)
    if ((iSgnID < 0) || (iSgnID >= lstmHpSignal.count()))
        return FALSE; // Invalid signal identifier

    // Assign signal identifier
    iCaptSgnID = iSgnID;

    return TRUE;
}
BOOL QmHpCtrlMgmt::CaptureSignal(UINT imHpMsg) const {

    // Check if mHelp window message match with existing Qt control signal
    if (hashmHpSgn.key(imHpMsg, NULL_QSTRING) != NULL_QSTRING)
        return TRUE;

    return FALSE;
}

// --------------------------------
// Reproduce processus
//
const QString& QmHpCtrlMgmt::GetCtrlSignal() const {

    // Return control signal to be reproduced (according 'lstmHpSignal' definition)
    return lstmHpSignal[iCaptSgnID];    // -> This method should not be called if 'SetCaptSgnID' failed
                                        //    but if succeed 'iCaptSgnID' should be a valid index
}

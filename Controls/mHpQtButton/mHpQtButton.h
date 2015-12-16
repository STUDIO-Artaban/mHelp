/*****************************************************************************\
mHpQtButton.h - Interface for the mHelp Qt Button Control Management
Plateform - Qt Framework 4.8.3
Version - 1.0
Date - 01/18/2013

Copyright(c) - Unknown Corporation. All rights reserved.
\*****************************************************************************/

#ifndef MHPQTBUTTON_H
#define MHPQTBUTTON_H

#include <QObject>
#include <QApplication>
#include <QIcon>
#include <QPushButton>

#include <assert.h>
#include "mhpqtbutton_global.h"

////// mHelp Qt Control Management Interface
#include <QtmHpCtrl.h>
#include "mHpQtButtonUI.h"

// --------------------------------
// Button style ID
//
#define ID_STYLE_BUTTON             0
#define ID_STYLE_CHECKABLE          1

static const QString BUTTON_STATE_NAME("BtnCheckState");

// --------------------------------
// Captured signal ID
//
static const QString SIGNAL_BUTTON_CLICKED("mHpButtonSignal_clicked"); // 'clicked' signal

class mHpQtButton : public QmHpCtrlMgmt { /////////////////////////////////////
    Q_OBJECT

private:
    // Qt Dll application (needed to use resources)
    QApplication* QtDllApp;

    HICON hBtnIcon;     // Button style icon handle (Win32)
    HICON hBtnChckIcon; // Checkable Button style icon handle (Win32)
    QIcon* iconBtn;     // Button style icon
    QIcon* iconBtnChck; // Checkable Button style icon

    QString strBtnObjName;
    mutable mHpQtButtonUI* pDlgChckStateUI;

public:
    mHpQtButton(UINT iIdx) : QmHpCtrlMgmt(iIdx, QmHpData::StrArray), pDlgChckStateUI(0) {

        // Check non-Qt parent application
        if (!QCoreApplication::instance()) {

            // Set Qt Dll application
            int argc = 0;
            QtDllApp = new QApplication(argc, 0);
        }
        else
            QtDllApp = 0;

        Q_INIT_RESOURCE(mHpQtButton);

        // Set control icon(s)
        iconBtn = new QIcon(":/mHpButtonIcon");
        hBtnIcon = iconBtn->pixmap(32).toWinHICON();

        iconBtnChck = new QIcon(":/mHpBtnCheckIcon");
        hBtnChckIcon = iconBtnChck->pixmap(32).toWinHICON();

        // Set control names
        pName=new QCtrlName*[2];
        pName[0] = new QCtrlName("Button", QmHpData::Undefined);
        pName[1] = new QCtrlName("Checkable button", QmHpData::Undefined);

        // Set DLL version
        strDllVersion = "1.0.0.0";

        // Add signal(s) to reproduce
        lstmHpSignal.append(SIGNAL_BUTTON_CLICKED); // One signal for Button
                                                    // -> 'clicked' signal

        // Set Qt Button object name
        strBtnObjName = QPushButton::staticMetaObject.className(); // QPushButton
    }
    virtual ~mHpQtButton() {

        // Name
        delete pName[0];
        delete pName[1];
        delete [] pName;

        // Config
        if (pCfg) {
            delete pCfg[0];
            delete [] pCfg;
        }

        // State
        if (pState) {
            delete pState[0];
            delete [] pState;
        }

        // Icon
        if (hBtnIcon)
            DestroyIcon(hBtnIcon);
        if (iconBtn)
            delete iconBtn;

        // Qt Dll application
        if (QtDllApp)
            delete QtDllApp;

        // UI object
        if (pDlgChckStateUI)
            delete pDlgChckStateUI;
    }

    // --------------------------------
    // Properties
    //

    ////// Object name
    BOOL IsCtrlObject(const QString& strObject) const {

        // Check Button object name
        if (strObject != strBtnObjName)
            return FALSE;
        return TRUE;
    }
    const QString& GetCtrlObject(UINT iObjIdx) const {

        // Return managed Button object name
        if (iObjIdx < 1)
            return strBtnObjName;
        return NULL_QSTRING;
    }

    ////// Name
    const QString& GetCtrlName(int iNameIdx = NO_VALUE) const {

        // Return the name of the control...
        if (iNameIdx <= NO_VALUE) { // ...according control style

            if (iCtrlStyleID == ID_STYLE_BUTTON)
                return pName[0]->GetName();
            if (iCtrlStyleID == ID_STYLE_CHECKABLE)
                return pName[1]->GetName();

            return NULL_QSTRING;
        }

        // ...according index parameter
        if (iNameIdx < 1) // One control name is available
            return pName[iNameIdx]->GetName();
        return NULL_QSTRING;
    }

    ////// Info
    void GetCtrlVers(QString &strVers) const;
    void GetCtrlDesc(QString &strDesc) const;
    HICON GetIconList(UINT iIconIdx) const;
    int GetIconIdx() const;

    ////// User Interface (UI)
    HWND CreateUI(HWND hParentUI,HWND hSaveBtn) const;
    void SaveUI(HWND hUI) const;
    void CloseUI();

    ////// Style
    BOOL SetStyle(int iStyleID) {

        // Check valid style
        assert((iStyleID == ID_STYLE_BUTTON) || (iStyleID == ID_STYLE_CHECKABLE));
        if ((iStyleID != ID_STYLE_BUTTON) && (iStyleID != ID_STYLE_CHECKABLE))
            return FALSE;

        // Assign control style
        iCtrlStyleID = iStyleID;

        // Define state & config class instance according control style
        if ((iCtrlStyleID == ID_STYLE_BUTTON) && (!pCfg)) {
            pCfg = new QCtrlCfg*[1];
            pCfg[0] = new QCtrlCfg("ButtonLabel", QmHpData::String);
        }
        if ((iCtrlStyleID == ID_STYLE_CHECKABLE) && (!pState)) {
            pState = new QCtrlState*[1];
            pState[0] = new QCtrlCfg(BUTTON_STATE_NAME, QmHpData::Integer);
        }

        return TRUE;
    }

    ////// State
    UINT GetStateCount() const {

        // Return state count according control style
        switch (iCtrlStyleID) {
            case ID_STYLE_BUTTON: {
                return 0; // No state for Button style
            }
            case ID_STYLE_CHECKABLE: {
                return 1; // One state available for checkable Button style
            }
            case NO_VALUE: // Control style should be defined!
            {   assert(NULL);
                break;
            }
        }
        return 0;
    }
    QCtrlState* GetState(UINT iStateIdx) {

        // Return state according control style
        switch (iCtrlStyleID) {
            case ID_STYLE_BUTTON: {
                return 0; // No state for Button style
            }
            case ID_STYLE_CHECKABLE: {

                // Check valid state index
                assert(iStateIdx == 0);
                if (iStateIdx < 1)
                    return pState[iStateIdx];
                break;
            }
            case NO_VALUE: // Control style should be defined!
            {   assert(NULL);
                break;
            }
        }
        return 0;
    }
    BOOL NeedToCheckState() const { return FALSE; }
    const QString& DisplayState();

    ////// Config
    UINT GetCfgCount() const {

        // Return config count according control style
        switch (iCtrlStyleID) {
            case ID_STYLE_BUTTON: {
                return 1; // One config available for Button style (Button label)
            }
            case ID_STYLE_CHECKABLE: {
                return 0; // No config available for checkable Button style
            }
            case NO_VALUE: // Control style should be defined!
            {   assert(NULL);
                break;
            }
        }
        return 0;
    }
    QCtrlCfg* GetConfig(UINT iCfgIdx) {

        // Return config according control style
        switch (iCtrlStyleID) {
            case ID_STYLE_BUTTON: {

                // Check valid config index
                assert(iCfgIdx == 0);
                if (iCfgIdx < 1)
                    return pCfg[iCfgIdx];
                break;
            }
            case ID_STYLE_CHECKABLE: {
                return 0; // No config available for checkable Button style
            }
            case NO_VALUE: // Control style should be defined!
            {   assert(NULL);
                break;
            }
        }
        return 0;
    }

    ////// HTML tags
    void GetHtmlTags(QString &strTags,UINT iTaskIdx) const;

    // --------------------------------
    // Capture processus
    //
public slots:
    void mHpQtButtonClickedSlot();

public:
    BOOL AddmHpSlots(HWND hDlg);

    // --------------------------------
    // Reproduce processus
    //
public:
    BOOL GetCtrlPos(LONG &x,LONG &y,LONG &cx,LONG &cy) const;
    BOOL CheckState() const { return TRUE; }

    ActType NextAction();

    BOOL Start(HWND hDlg);
    BOOL End() { return TRUE; }

    BOOL MoveTo(int &x,int &y);
    BOOL MoveEvent() { return TRUE; }
    BOOL WaitUntil() { return TRUE; }

    BOOL LeftPressEvent();
    BOOL LeftUnpressEvent();
    BOOL RightPressEvent() { return TRUE; }
    BOOL RightUnpressEvent() { return TRUE; }
    BOOL EditEvent() { return TRUE; }

};

//////
MHPQTBUTTON_EXPORT void* CreateNewInstance(UINT);

#endif // MHPQTBUTTON_H

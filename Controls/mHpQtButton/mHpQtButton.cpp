// Copyright (c) Unknown Corporation.  All rights reserved.
//

#include "mHpQtButton.h"

#include <QMetaMethod>
#include <qobject_p.h>

///////////////////////////////////////////////////////////////////////////////
// ImHpCtrlMgmt class implemetation

// --------------------------------
// Properties
//

////// Info
void mHpQtButton::GetCtrlVers(QString &strVers) const {
    strVers = "4.8.3";
}
void mHpQtButton::GetCtrlDesc(QString &strDesc) const {
    strDesc = "Qt Button";
}
HICON mHpQtButton::GetIconList(UINT iIconIdx) const {

    // Check existing icon index
    switch (iIconIdx) {
        case 0: { // Button style icon
            return hBtnIcon;
        }
        case 1: { // Checkable Button style icon
            return hBtnChckIcon;
        }
    }
    return NULL;
}
int mHpQtButton::GetIconIdx() const {

    // Return icon index according current button style (see 'GetIconList' method)
    if (iCtrlStyleID == ID_STYLE_BUTTON)
        return 0;
    if (iCtrlStyleID == ID_STYLE_CHECKABLE)
        return 1;

    return NO_VALUE;
}

////// User Interface (UI)
HWND mHpQtButton::CreateUI(HWND hParentUI,HWND hSaveBtn) const {

    assert(iCtrlStyleID == ID_STYLE_CHECKABLE);
    assert(pState);

    if (pDlgChckStateUI)
        delete pDlgChckStateUI;

    // Create checkable Button UI
    pDlgChckStateUI = new mHpQtButtonUI();

    // Set checkable Button state
    const int* pBtnChck = (const int*)pState[0]->GetValue();
    pDlgChckStateUI->setState(*pBtnChck);
    pDlgChckStateUI->setSaveHandle(hSaveBtn);

    pDlgChckStateUI->show();
    pDlgChckStateUI->move(0, 0);
    
    return NULL;
}
void mHpQtButton::SaveUI(HWND hUI) const {

    assert(iCtrlStyleID == ID_STYLE_CHECKABLE);
    assert(pState);

    // Assign configured state
    pState[0]->SetValue(pDlgChckStateUI->getState());
}
void mHpQtButton::CloseUI() {

    assert(iCtrlStyleID == ID_STYLE_CHECKABLE);
    assert(pState);

    // Close and delete UI (if any)
    if (pDlgChckStateUI) {
        pDlgChckStateUI->close();
        delete pDlgChckStateUI;
        pDlgChckStateUI = 0;
    }
}

////// State
const QString& mHpQtButton::DisplayState() {

    assert(iCtrlStyleID != NO_VALUE);

    // Check existing display state
    if (strDisplayState)
        delete strDisplayState;

    // Return state according Button style
    switch (iCtrlStyleID) {
        case ID_STYLE_BUTTON: {

            // Display Button label
            const QString* pStrLabel = (const QString*)pCfg[0]->GetValue();
            strDisplayState = new QString("No state (" + (*pStrLabel) + ")");
            break;
        }
        case ID_STYLE_CHECKABLE: {

            // Display checkable Button state
            const int* pChkState = (const int*)pState[0]->GetValue();
            switch (*pChkState) {
                case 0: { // Unchecked
                    strDisplayState = new QString("Unchecked");
                    break;
                }
                case 1: { // Checked
                    strDisplayState = new QString("Checked");
                    break;
                }
                case 2: { // Change state
                    strDisplayState = new QString("Change state");
                    break;
                }
            }
            break;
        }
    }

    return *strDisplayState;
}

////// HTML tags
void mHpQtButton::GetHtmlTags(QString &strTags,UINT iTaskIdx) const {

    // Use HTML radio input type to set Button state (if any)
    // -> According Button style
    if (iCtrlStyleID == ID_STYLE_CHECKABLE) {

        strTags = "\t<input type=\"radio\" name=\"" + QString::number(iTaskIdx);
        strTags += "-" + BUTTON_STATE_NAME + "\" value=\"1\"";
        const int* pChkState = (const int*)pState[0]->GetValue();
        if (*pChkState == 1) // Checked
            strTags += " checked"; // Select current state
        strTags += "/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">Checked</font><br>\r\n";
        strTags += "\t<input type=\"radio\" name=\"" + QString::number(iTaskIdx);
        strTags += "-" + BUTTON_STATE_NAME + "\" value=\"0\"";
        if (*pChkState == 0) // Unchecked
            strTags += " checked"; // Select current state
        strTags += "/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">Unchecked</font><br>\r\n";
        strTags += "\t<input type=\"radio\" name=\"" + QString::number(iTaskIdx);
        strTags += "-" + BUTTON_STATE_NAME + "\" value=\"2\"";
        if (*pChkState == 2) // Change state
            strTags += " checked"; // Select current state
        strTags += "/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">Change state</font><br>\r\n";

        // Will return following for a checkable Button style with an unchecked current state:

        //  <input type="radio" name="0-CheckState" value="1"/>Checked<br>
        //  <input type="radio" name="0-CheckState" value="0" checked/>Unchecked<br>
        //  <input type="radio" name="0-CheckState" value="2"/>Change state<br>

    }
    else { // No state available for common Button style

        strTags = QString::null;
    }
}

// --------------------------------
// Capture processus
//
void mHpQtButton::mHpQtButtonClickedSlot() {

    // Get Qt Button object that send signal
    QPushButton* pushBtn = qobject_cast<QPushButton*>(QObject::sender());

    ////// WARNING: Always check triggered user signal!
    // -> Using code below
    if (!pushBtn->hasFocus())
        return;

    // Set Qt Button control identifier
    // -> Using common control identifier (QWidget.objectName)
    QWidget* CtrlWidget = qobject_cast<QWidget*>(QObject::sender());
    if (!SetQtCtrlID(CtrlWidget))
        return;

    // Set current control style
    if (pushBtn->isCheckable())
        SetStyle(ID_STYLE_CHECKABLE); // Checkable Button style available
    else
        SetStyle(ID_STYLE_BUTTON); // Button style
    
    // Set Qt Button control config (call 'SetStyle' first)
    if (iCtrlStyleID == ID_STYLE_BUTTON)
        pCfg[0]->SetValue(pushBtn->text());

    // Set Qt Button state (call 'SetStyle' first)
    if (iCtrlStyleID == ID_STYLE_CHECKABLE)
        pState[0]->SetValue((pushBtn->isChecked())? 1:0);

    // Set current control object name
    strObject = pushBtn->metaObject()->className(); // QPushButton

    // Set current state bound
    if (iCtrlStyleID == ID_STYLE_CHECKABLE)
        strStateBound = "[0;2]";

    ////// WARNING: Always call function below for all Qt Button signal occurence
    // -> Send window message in order to inform mHelp a Qt Button 'clicked' signal has occurred

    SendmHpSignal(CtrlWidget, SIGNAL_BUTTON_CLICKED); // 'clicked' signal
}

BOOL mHpQtButton::AddmHpSlots(HWND hDlg) {

    ////// Add mHelp slot(s) for expected signal(s) on all specific Qt controls defined in the Form

    // Get & Check widget Form
    QWidget* WidgetForm = QWidget::find(hDlg);
    if (!WidgetForm)
        return TRUE; ////// WARNING: Return FALSE only if QPushButton control and failed to add mHelp slot(s)
                     // -> Not a widget Form

    ////// WARNING: Always define mHelp window message(s) for control each signal
    // -> 'clicked' signal
    if (!AddmHpSlot(SIGNAL_BUTTON_CLICKED))
        return FALSE;

    // Get all QPushButton control defined into the Form
    QList<QPushButton*> lstPushBtn = WidgetForm->findChildren<QPushButton*>();
    if (!lstPushBtn.empty()) {

        // Loop on all QPushButton control found
        QList<QPushButton*>::const_iterator itPushBtn;
        for (itPushBtn = lstPushBtn.constBegin(); itPushBtn != lstPushBtn.constEnd(); itPushBtn++) {

            ////// Add required mHelp slot on current control signal(s)
            QPushButton* pushBtn = *itPushBtn;

            // Remove all previous 'clicked' signal(s)
            QObjectPrivate* prvObj = QObjectPrivate::get(pushBtn);
            QObjectList rcvList = prvObj->receiverList("clicked()");

            int iSignal = pushBtn->metaObject()->indexOfSignal("clicked()");
            QList<int> iSlotList;
            QList<QObject*> objRcvList;

            ////// Loop on 'clicked' signal receiver(s)
            QObjectList::const_iterator itRcv;
            for (itRcv = rcvList.constBegin(); itRcv != rcvList.constEnd(); itRcv++) {

                QObject* curRcv = *itRcv;

                ////// Loop on 'clicked' receiver slot(s)
                const QMetaObject* metaRcv = curRcv->metaObject();
                for (int iSlot = metaRcv->methodOffset(); iSlot < metaRcv->methodCount(); iSlot++) {

                    // Disconnect 'clicked' signal
                    if (QObject::disconnect(pushBtn,
                                            pushBtn->metaObject()->method(iSignal),
                                            curRcv,
                                            metaRcv->method(iSlot))) {
                        iSlotList.append(iSlot);
                        objRcvList.append(curRcv);
                    }
                }
            }

            ////// Add mHelp QPushButton 'clicked' slot (in first position)
            QObject::connect(pushBtn, SIGNAL(clicked()), this, SLOT(mHpQtButtonClickedSlot()), Qt::DirectConnection);

            // Restore all previous 'clicked' slot(s) after mHelp slot added above
            for (int i = 0; i < iSlotList.size(); i++) {

                ////// WARNING: Connection type Qt::AutoConnection as default !!!
                QObject::connect(pushBtn,
                                 pushBtn->metaObject()->method(iSignal),
                                 objRcvList.at(i),
                                 objRcvList.at(i)->metaObject()->method(iSlotList.at(i)));
            }
        }
    }

    // mHelp slot(s) added
    // -> If any
    return TRUE;
}

// --------------------------------
// Reproduce processus
//
BOOL mHpQtButton::GetCtrlPos(LONG &x,LONG &y,LONG &cx,LONG &cy) const {

    assert(Control);

    // Return Qt Button screen coordonates
    QPoint posCtrl = Control->mapToGlobal(QPoint(0, 0));
    x = (LONG)posCtrl.x();
    y = (LONG)posCtrl.y();

    cx = (LONG)Control->width();
    cy = (LONG)Control->height();

    return TRUE;
}

QmHpCtrlMgmt::ActType mHpQtButton::NextAction() {

    assert(Control);

    // Next action step
    iActStep++;

    // Switch on signal to reproduce ('clicked' signal)
    if (GetCtrlSignal() == SIGNAL_BUTTON_CLICKED) {

        // Switch on current action step
        switch (iActStep) {
            case 1: { return QmHpCtrlMgmt::ActMove; } // Move mHelp cursor over the control
            case 2: {

                // Check checkable Button style
                if (iCtrlStyleID == ID_STYLE_CHECKABLE) {

                    // Get Qt Button object
                    QPushButton* pushBtn = qobject_cast<QPushButton*>(Control);

                    assert(pState);
                    const int* pBtnChck = (const int*)pState[0]->GetValue();
                    switch (*pBtnChck) {
                        case 0: {
                            // Check expected checkable Button state
                            if (!pushBtn->isChecked())
                                return  QmHpCtrlMgmt::ActEnd; // Finish
                            break;
                        }
                        case 1: {
                            // Check expected checkable Button state
                            if (pushBtn->isChecked())
                                return  QmHpCtrlMgmt::ActEnd; // Finish
                            break;
                        }
                    }
                }

                return QmHpCtrlMgmt::ActLeftPress; // Press left mHelp cursor button
            }
            case 3: { return QmHpCtrlMgmt::ActLeftUnpress; } // Unpress left mHelp cursor button
            case 4: { return  QmHpCtrlMgmt::ActEnd; } // Finished
        }
    }

    // Error
    return QmHpCtrlMgmt::ActError;
}

BOOL mHpQtButton::Start(HWND hDlg) {

    // Get Qt control object (using common Qt control identifier)
    if (!GetQtCtrlObject(hDlg))
        return FALSE; // Failed to get Button object in the Form
    //else
    // -> Qt control object found (stored in 'Control')

    // Get Qt Button object
    QPushButton* pushBtn = qobject_cast<QPushButton*>(Control);

    // Check Button status...
    if ((!pushBtn->isVisible()) ||  // ...visible
        (!pushBtn->isEnabled()))    // ...enabled
        return FALSE;

    // Check Button style
    if ((iCtrlStyleID != ID_STYLE_BUTTON) && (iCtrlStyleID != ID_STYLE_CHECKABLE))
        return FALSE; // Wrong style

    // Reset action step
    iActStep = 0;

    return TRUE;
}

BOOL mHpQtButton::MoveTo(int &x,int &y) {

    assert(Control);
    assert(iActStep == 1);

    // Return new mHelp cursor screen position
    QPoint posCtrl = Control->mapToGlobal(QPoint(0, 0));
    x = posCtrl.x() + Control->width() - 9;
    y = posCtrl.y() + Control->height() - 10;

    return TRUE;
}

BOOL mHpQtButton::LeftPressEvent() {

    assert(Control);

    // Set focus on Button
    Control->setFocus();

    return TRUE;
}
BOOL mHpQtButton::LeftUnpressEvent() {

    assert(Control);

    // Get Qt Button object
    QPushButton* pushBtn = qobject_cast<QPushButton*>(Control);

    // Check checkable Button style to set state
    if (iCtrlStyleID == ID_STYLE_CHECKABLE) {

        assert(pState);

        // Set expected state
        const int* pBtnChck = (const int*)pState[0]->GetValue();
        if (*pBtnChck)
            pushBtn->setChecked(true);
        else
            pushBtn->setChecked(false);
    }

    // Launch 'clicked' slot(s) programmatically
    QMetaObject::invokeMethod(pushBtn, "clicked", Qt::QueuedConnection);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void* CreateNewInstance(UINT iIdx) {   

    return static_cast<void*>(new mHpQtButton(iIdx));
}

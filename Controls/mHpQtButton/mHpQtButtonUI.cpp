// mHpQtButtonUI.cpp - Dialog implementation for mHelp Qt checkable Button control state
//

#include "mHpQtButtonUI.h"
#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////
mHpQtButtonUI::mHpQtButtonUI(QWidget *parent, Qt::WFlags flags)
              :QDialog(parent, flags), iChckState(1), hApplyChange(NULL) {

    ui.setupUi(this);

    connect(ui.RadChecked, SIGNAL(clicked()), this, SLOT(OnRadChecked()));
    connect(ui.RadUnchecked, SIGNAL(clicked()), this, SLOT(OnRadUnchecked()));
    connect(ui.RadChange, SIGNAL(clicked()), this, SLOT(OnRadChange()));

    //
    this->setModal(false);
    this->setWindowFlags(Qt::FramelessWindowHint);
}
mHpQtButtonUI::~mHpQtButtonUI() { }

//////
void mHpQtButtonUI::setState(int iState) {

    // Assign current checkable Button state
    iChckState = iState;

    // Set state text
    switch (iChckState) {
        case 0: { // Uncheck
            ui.RadUnchecked->setChecked(true);
            ui.EdtState->setText("0");
            break;
        }
        case 1: { // Check
            ui.RadChecked->setChecked(true);
            ui.EdtState->setText("1");
            break;
        }
        case 2: { // Change
            ui.RadChange->setChecked(true);
            ui.EdtState->setText("2");
            break;
        }
    }
}

//////
void mHpQtButtonUI::OnRadChecked() {

    // Assign configured state
    iChckState = 1;

    // Set state text and allow user to save change
    ui.EdtState->setText("1");
    EnableWindow(hApplyChange, TRUE);
}
void mHpQtButtonUI::OnRadUnchecked() {

    // Assign configured state
    iChckState = 0;

    // Set state text and allow user to save change
    ui.EdtState->setText("0");
    EnableWindow(hApplyChange, TRUE);
}
void mHpQtButtonUI::OnRadChange() {

    // Assign configured state
    iChckState = 2;

    // Set state text and allow user to save change
    ui.EdtState->setText("2");
    EnableWindow(hApplyChange, TRUE);
}

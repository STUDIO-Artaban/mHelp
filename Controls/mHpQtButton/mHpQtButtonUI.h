// mHpQtButtonUI.h - Dialog definition for mHelp Qt checkable Button control state
//

#ifndef MHPQTBUTTONUI_H
#define MHPQTBUTTONUI_H

#include <qobject_p.h>
#include "ui_mHpQtButton.h"

class mHpQtButtonUI : public QDialog
{
    Q_OBJECT

public:
    mHpQtButtonUI(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~mHpQtButtonUI();

    void setState(int iState);
    void setSaveHandle(HWND hSaveBtn) { hApplyChange = hSaveBtn; }
    int getState() { return iChckState; }

private slots:
    void OnRadChecked();
    void OnRadUnchecked();
    void OnRadChange();

private:
    Ui::mHpQtButton ui;

    int iChckState;
    HWND hApplyChange;

};

#endif // MHPQTBUTTONUI_H

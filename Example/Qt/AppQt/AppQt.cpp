#include "AppQt.h"
#include <mHelp.h>

AppQt::AppQt(QWidget *parent, Qt::WFlags flags)
      :QDialog(parent, flags)
{
    ui.setupUi(this);

    iCount = 0;
    connect(ui.BtnmHelp, SIGNAL(clicked()), this, SLOT(OnBtnmHelp()));
    connect(ui.BtnTest, SIGNAL(clicked()), this, SLOT(OnBtnTest()));
    connect(ui.BtnCheck, SIGNAL(clicked()), this, SLOT(OnBtnCheck()));
}

AppQt::~AppQt()
{

}

//////
void AppQt::changeEvent(QEvent* e) {

    if (e->type() == QEvent::ActivationChange) {
        if (isActiveWindow()) {
            mHelpInit(this->winId(), "AppQT", true);
        }
    }
    QDialog::changeEvent(e);
}

//////
void AppQt::OnBtnmHelp() {

    mHelpOpen(this->winId());
}
void AppQt::OnBtnTest() {

    iCount++;
    ui.lblTest->setText("...Test " + QString::number(iCount));
}
void AppQt::OnBtnCheck() {

    iCount++;
    if (ui.BtnCheck->isChecked())
        ui.lblCheck->setText("...Checked " + QString::number(iCount));
    else
        ui.lblCheck->setText("...Unchecked " + QString::number(iCount));
}

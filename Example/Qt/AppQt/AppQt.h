#ifndef APPQT_H
#define APPQT_H

#include <QtGui/QDialog>
#include "ui_AppQt.h"

class AppQt : public QDialog
{
    Q_OBJECT

private:
    int iCount;

public:
    AppQt(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~AppQt();

protected:
    void changeEvent(QEvent* e);

private slots:
    void OnBtnmHelp();
    void OnBtnTest();
    void OnBtnCheck();

private:
    Ui::AppQtClass ui;

};

#endif // APPQT_H

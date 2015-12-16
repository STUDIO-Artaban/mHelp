// mHpQtCtrl_Global.h
//

#ifndef MHPQTCTRL_GLOBAL_H
#define MHPQTCTRL_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QLibrary>

#include "mHpQtCtrlMgmt.h"

#ifndef QMHPCTRLMGMT_H
  #include <QtmHpCtrl.h>
#endif

class CmHpQtCtrlMgmt : public ImHpCtrlMgmt { //////////////////////////////////

private:
    QmHpCtrlMgmt* QtCtrlMgmt;

    UINT iStateCount;
    UINT iCfgCount;

    mutable char szName[DATA_NAME_SIZE];

public:
    CmHpQtCtrlMgmt(UINT iIdx, CData::Type TypeID);
    ~CmHpQtCtrlMgmt();

    // --------------------------------
    // Qt tools (declared in ImHpCtrlMgmt interface)
    //
    BOOL FillQtData();
    BOOL UpdateQtData();
    void CloseQtUI();

    void SetQtCtrlMgmt(QmHpCtrlMgmt* pQtCtrl);

    // --------------------------------
    // Properties
    //

    ////// Object name
    BOOL IsCtrlClass(const char* lpClass) const;
    const char* GetCtrlClass(UINT iClassIdx) const;

    ////// Name
    const char* GetCtrlName(int iNameIdx = NO_VALUE) const;

    ////// Info
    void GetCtrlVers(char* lpVers, UINT iMaxSize) const;
    void GetCtrlDesc(char* lpDesc, UINT iMaxSize) const;
    HICON GetIconList(UINT iIconIdx) const;
    int GetIconIdx() const;

    ////// User Interface (UI)
    HWND CreateUI(HWND hParentUI,HWND hSaveBtn) const;
    void SaveUI(HWND hUI) const;

    ////// Style
    BOOL SetStyle(int iStyleID);

    ////// State
    UINT GetStateCount() const;
    CCtrlState* GetState(UINT iStateIdx);
    BOOL NeedToCheckState() const;
    const char* DisplayState();

    void SetStateCount(UINT iCount);

    ////// Config
    UINT GetCfgCount() const;
    CCtrlCfg* GetConfig(UINT iCfgIdx);

    void SetCfgCount(UINT iCount);

    ////// HTML tags
    void GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const;

    // --------------------------------
    // Capture processus
    //
    BOOL SetCaptMsgID(int iMsgID);
    BOOL CaptureWin32(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);

    // --------------------------------
    // Reproduce processus
    //
    BOOL GetCtrlPos(HWND hDlg,LONG &x,LONG &y,LONG &cx,LONG &cy) const;
    BOOL CheckState(HWND hDlg) const;

    ActType NextAction(HWND hDlg);

    BOOL Start(HWND hDlg);
    BOOL End(HWND hDlg);

    BOOL MoveTo(HWND hDlg,int &x,int &y);
    BOOL MoveEvent(HWND hDlg);
    BOOL WaitUntil(HWND hDlg);

    BOOL LeftPressEvent(HWND hDlg);
    BOOL LeftUnpressEvent(HWND hDlg);
    BOOL RightPressEvent(HWND hDlg);
    BOOL RightUnpressEvent(HWND hDlg);
    BOOL EditEvent(HWND hDlg);

};

class CmHpQtCtrl : public QObject, public ImHpQtCtrlMgmt { ////////////////////
    Q_OBJECT

private:
    char szDllPath[MAX_DLL_PATH];
    UINT iIndex;

    QLibrary* QtCtrlLib;
    CmHpQtCtrlMgmt* mHpQtCtrlInst;
    QmHpCtrlMgmt* QtCtrlInst;

public:
    CmHpQtCtrl(UINT iIdx,const char* lpDllPath, QObject* parent = 0);
    virtual ~CmHpQtCtrl();

    ////// ImHpQtCtrlMgmt interface implementation
    BOOL LoadQtLibrary();
    void ClearQtData();
    void CopyQtData(const ImHpCtrlMgmt& CopyData);
    BOOL AddQtSlots(HWND hDlg);
    ImHpCtrlMgmt* GetQtCtrlMgmt();

};

#endif // MHPQTCTRL_GLOBAL_H

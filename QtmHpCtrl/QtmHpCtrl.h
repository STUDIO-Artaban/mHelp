/*****************************************************************************\
QtmHpCtrl.h - Interface for mHelp Qt Controls Management
mHelp version - 1.1
Date - 01/20/2013

Copyright(c) - Unknown Corporation. All rights reserved.
\*****************************************************************************/

#pragma once
#ifndef QMHPCTRLMGMT_H
#define QMHPCTRLMGMT_H

#include <windows.h>
#include <QObject>
#include <QWidget>
#include <QHash>
#include <QList>

//////
#define DATA_NAME_SIZE          64      // Max data name size
#define CTRL_OBJECT_SIZE        128     // Max control object name size (CTRL_CLASS_SIZE)
#define DLL_VERSION_SIZE        32      // Max DLL version size
#define STATE_BOUND_SIZE        64      // Max state bound size

#define CTRL_ID_NAME            "CtrlID"
#define NO_VALUE                (-1)

static const QString NULL_QSTRING;

class QmHpData : public QObject { /////////////////////////////////////////////
    Q_OBJECT

public:
    enum Type : int {
        
        Undefined = 0,  // Not defined (no value available)
        Integer,        // Integer value
        String,         // String value
        IntArray,       // Integer array value
        StrArray        // String array value
    
    }; // Data types

private:
    QString strName;    // Name of the data
    Type DataType;      // Value type
    UINT iCount;        // Value count (for IntArray|StrArray)
    void** pValue;      // Value

public:
    QmHpData(const QString& strDataName, Type eDataType, QObject* parent = 0);
    QmHpData(QObject* parent = 0);
    virtual ~QmHpData();

    ////// Common
    const QString& GetName() const; // Return data name 'strName'
    Type GetType() const;           // Return value type 'DataType'
    UINT GetCount() const;          // Return value count 'iCount'
    const void* GetValue(UINT iIdx = 0) const; // Return indexed value

    void SetValue(int iNewVal, UINT iIdx = 0);
    void SetValue(const QString& strNewVal, UINT iIdx = 0);

    void ClearValue();

    ////// Array
    void AddValue(int iNewVal);
    void AddValue(const QString& strNewVal);

    void Reverse();

};

typedef QmHpData QCtrlName;
typedef QmHpData QCtrlState;
typedef QmHpData QCtrlCfg;
typedef QmHpData QCtrlID;

class QmHpCtrlMgmt : public QObject { /////////////////////////////////////////
    Q_OBJECT

protected:
    // Control object, name, state & config
    QString strObject;
    QCtrlName** pName;
    QCtrlState** pState;
    QCtrlCfg** pCfg;

    // Unique control identifier in parent dialog/window/Form
    QCtrlID* pID;

    // Default state bound description
    QString strStateBound;

    int iCtrlStyleID;               // Control style ID
    UINT iActStep;                  // Current action step
    QWidget* Control;               // Current control
    QString* strErrorMsg;           // Error message
    QString* strDisplayState;       // String to display state
    QList<QString> lstmHpSignal;    // mHelp signal(s) identifier to reproduce

    // Dll version
    QString strDllVersion;

public:
    enum ActType : int {

        ActError = 0,       // Error (display 'szErrorMsg' if any and stop process)
        ActMove,            // Move mouse (move to 'MoveTo' definition and call 'MoveEvent' when positionned)
        ActWait,            // Wait (call 'WaitUntil')
        ActLeftPress,       // Left mouse button press (call 'LeftPressEvent')
        ActLeftUnpress,     // Left mouse button unpress (call 'LeftUnpressEvent')
        ActRightPress,      // Right mouse button press (call 'RightPressEvent')
        ActRightUnpress,    // Right mouse button unpress (call 'RightUnpressEvent')
        ActLeftDblClick,    // Left mouse button double click (call 'LeftPressEvent', 'LeftUnpressEvent', 'LeftPressEvent' and 'LeftUnpressEvent' successively)
        ActRightDblClick,   // Right mouse button double click (call 'RightPressEvent', 'RightUnpressEvent', 'RightPressEvent' and 'RightUnpressEvent' successively)
        ActEdit,            // Edit text (call 'EditEvent')
        ActEnd              // End (call 'End' - Finish process)

        ////// After all action mHelp processus call 'NextAction' until 'ActEnd' or 'ActError' are returned

    }; // Action types

private:
    int iCaptSgnID;                     // Captured signal ID
    QHash<QString, UINT> hashmHpSgn;    // mHelp window message(s)/mHelp signal name(s)

    // Index of the control management in mHelp
    UINT iIndex;    // Reserved

public:
    QmHpCtrlMgmt(UINT iIdx, QmHpData::Type TypeID, QObject* parent = 0);
    virtual ~QmHpCtrlMgmt();

    //////
    UINT GetIndex() const;
    virtual const QString& GetLastErr() const;
    void ClearData();

    // --------------------------------
    // Qt tools
    //
protected:
    BOOL SetQtCtrlID(QWidget* Sender);
    BOOL GetQtCtrlObject(HWND hDlg);

    // --------------------------------
    // Properties
    //
public:
    const QString& GetDllVersion() const;

    ////// ID
    QCtrlID* GetCtrlID();

    ////// Object name
    const QString& GetCtrlObject() const;
    void SetCtrlObject(const QString& strObject);
    virtual BOOL IsCtrlObject(const QString& strObject) const = 0;
    virtual const QString& GetCtrlObject(UINT iObjIdx) const = 0;

    ////// Name
    virtual const QString& GetCtrlName(int iNameIdx = NO_VALUE) const = 0;

    ////// Info
    virtual void GetCtrlVers(QString &strVers) const = 0;
    virtual void GetCtrlDesc(QString &strDesc) const = 0;
    virtual HICON GetIconList(UINT iIconIdx) const = 0;
    virtual int GetIconIdx() const = 0;

    ////// User Interface (UI)
    virtual HWND CreateUI(HWND hParentUI,HWND hSaveBtn) const = 0;
    virtual void SaveUI(HWND hUI) const = 0;
    virtual void CloseUI() = 0;

    ////// Style
    int GetStyle() const;
    virtual BOOL SetStyle(int iStyleID) = 0;

    ////// State
    virtual UINT GetStateCount() const = 0;
    virtual QCtrlState* GetState(UINT iStateIdx) = 0;
    virtual BOOL NeedToCheckState() const = 0;
    const QString& GetStateBound() const;
    void SetStateBound(const QString& strBound);
    virtual const QString& DisplayState() = 0;

    ////// Config
    virtual UINT GetCfgCount() const = 0;
    virtual QCtrlCfg* GetConfig(UINT iStateIdx) = 0;

    ////// HTML tags
    virtual void GetHtmlTags(QString &strTags,UINT iTaskIdx) const = 0;

    // --------------------------------
    // Capture processus
    //
protected:
    BOOL AddmHpSlot(const QString& strmHpSgn);
    void SendmHpSignal(QWidget* Sender, const QString& strmHpSgn);

public:
    int GetCaptSgnID() const;
    BOOL SetCaptSgnID(int iSgnID);
    BOOL CaptureSignal(UINT imHpMsg) const;

    virtual BOOL AddmHpSlots(HWND hDlg) = 0;

    // --------------------------------
    // Reproduce processus
    //
protected:
    const QString& GetCtrlSignal() const;

public:
    virtual BOOL GetCtrlPos(LONG &x,LONG &y,LONG &cx,LONG &cy) const = 0;
    virtual BOOL CheckState() const = 0;

    virtual ActType NextAction() = 0;

    virtual BOOL Start(HWND hDlg) = 0;
    virtual BOOL End() = 0;

    virtual BOOL MoveTo(int &x,int &y) = 0;
    virtual BOOL MoveEvent() = 0;
    virtual BOOL WaitUntil() = 0;

    virtual BOOL LeftPressEvent() = 0;
    virtual BOOL LeftUnpressEvent() = 0;
    virtual BOOL RightPressEvent() = 0;
    virtual BOOL RightUnpressEvent() = 0;
    virtual BOOL EditEvent() = 0;

};

//////
typedef void* (*PFN_CREATENEWQTINST)(UINT);

#endif // QTMHPCTRL_H

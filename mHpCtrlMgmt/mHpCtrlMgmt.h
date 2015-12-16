/*****************************************************************************\
mHpCtrlMgmt.h - Interface for the mHelp Controls Management
mHelp version - 1.1
Date - 12/03/2012

Copyright(c) - Unknown Corporation. All rights reserved.
\*****************************************************************************/

#pragma once
#ifndef _INC_MHPCTRLMGMT
#define _INC_MHPCTRLMGMT

#include <windows.h>
#include <commctrl.h>
#include <assert.h>

//////
#define DATA_NAME_SIZE          64      // Max state name size
#define CTRL_CLASS_SIZE         128     // Max control class name size
#define DLL_VERSION_SIZE        32      // Max DLL version size
#define STATE_BOUND_SIZE        64      // Max state bound size
#define MAX_DLL_PATH            256     // Max Dll path

#define CTRL_ID_NAME            "CtrlID"
#define NO_VALUE                (-1)

#define CLASS_SEP(Class)        Class " & "

class CData ///////////////////////////////////////////////////////////////////
{
public:
    enum Type : int {
        
        Undefined = 0,  // Not defined (no value available)
        Integer,        // Integer value
        String,         // String value
        IntArray,       // Integer array value
        StrArray        // String array value
    
    }; // Data types

private:
    char szName[DATA_NAME_SIZE];    // Name of the data
    Type DataType;                  // Value type
    UINT iCount;                    // Value count (for IntArray|StrArray)
    void** pValue;                  // Value

public:
    CData(const char* szDataName,Type eDataType);
    CData(const CData& CopyData);
    CData();
    virtual ~CData();

    ////// Operators
    const CData& operator=(const CData& CopyData);
    bool operator==(const CData& CmpData) const;

    ////// Common
    const char* GetName() const;    // Return data name 'szName'
    Type GetType() const;           // Return value type 'DataType'
    UINT GetCount() const;          // Return value count 'iCount'
    const void* GetValue(UINT iIdx = 0) const;  // Return indexed value

    void SetValue(int iNewVal,UINT iIdx = 0);
    void SetValue(const char* szNewVal,UINT iIdx = 0);

    void ClearValue();

    ////// Array
    void AddValue(int iNewVal);
    void AddValue(const char* szNewVal);

    void Reverse();

};

typedef CData CCtrlName;
typedef CData CCtrlState;
typedef CData CCtrlCfg;
typedef CData CCtrlID;

class ImHpCtrlMgmt ////////////////////////////////////////////////////////////
{
protected:
    // Control class, name, state & config
    mutable char szClass[CTRL_CLASS_SIZE];
    CCtrlName** pName;
    CCtrlState** pState;
    CCtrlCfg** pCfg;

    // Unique control identifier in parent dialog/window/Form
    CCtrlID* pID;

    // Default state bound description
    char szStateBound[STATE_BOUND_SIZE];

    int iCtrlStyleID;           // Control style ID
    int iCaptMsgID;             // Captured message ID
    UINT iActStep;              // Current action step
    HWND hControl;              // Current control handle
    mutable char* szErrorMsg;   // Error message
    char* szDisplayState;       // String to display state

    // Dll version
    char szDllVersion[DLL_VERSION_SIZE];

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
    UINT iIndex;        // Index of the control management in mHelp (Reserved)
    ActType PrevAct;    // Previous action

public:
    ImHpCtrlMgmt(UINT iIdx,CData::Type TypeID);
    virtual ~ImHpCtrlMgmt();

    //////
    UINT GetIndex() const;
    virtual const char* GetLastErr() const;

    friend void CopymHpCtrlMgmt(ImHpCtrlMgmt&,const ImHpCtrlMgmt&,UINT,UINT);

    // --------------------------------
    // Win32 tools
    //
    void SetWin32CtrlID(HWND hDlg);
    void GetWin32CtrlHandle(HWND hDlg);
    void ForceWin32ScrollMsg(HWND hDlg,int iScrollPos,UINT iNotifyCode,BOOL bVertical);
    void ForceWin32NotifyMsg(HWND hDlg,UINT iNotifyCode);

    // --------------------------------
    // Qt tools (Reserved)
    //
    virtual BOOL FillQtData();
    virtual BOOL UpdateQtData();
    virtual void CloseQtUI();

    // --------------------------------
    // Properties
    //
    const char* GetDllVersion() const;

    ////// ID
    CCtrlID* GetCtrlID();

    ////// Class name
    const char* GetCtrlClass() const;
    void SetCtrlClass(const char* lpClass);
    virtual BOOL IsCtrlClass(const char* lpClass) const = 0;
    virtual const char* GetCtrlClass(UINT iClassIdx) const = 0;

    ////// Name
    virtual const char* GetCtrlName(int iNameIdx = NO_VALUE) const = 0;

    ////// Info
    virtual void GetCtrlVers(char* lpVers,UINT iMaxSize) const = 0;
    virtual void GetCtrlDesc(char* lpDesc,UINT iMaxSize) const = 0;
    virtual HICON GetIconList(UINT iIconIdx) const = 0;
    virtual int GetIconIdx() const = 0;

    ////// User Interface (UI)
    virtual HWND CreateUI(HWND hParentUI,HWND hSaveBtn) const = 0;
    virtual void SaveUI(HWND hUI) const = 0;

    ////// Style
    int GetStyle() const;
    virtual BOOL SetStyle(int iStyleID) = 0;

    ////// State
    virtual UINT GetStateCount() const = 0;
    virtual CCtrlState* GetState(UINT iStateIdx) = 0;
    virtual BOOL NeedToCheckState() const = 0;
    const char* GetStateBound() const;
    virtual const char* DisplayState() = 0;

    ////// Config
    virtual UINT GetCfgCount() const = 0;
    virtual CCtrlCfg* GetConfig(UINT iCfgIdx) = 0;

    ////// HTML tags
    virtual void GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const = 0;

    // --------------------------------
    // Capture processus
    //
    int GetCaptMsgID() const;
    virtual BOOL SetCaptMsgID(int iMsgID) = 0;
    virtual BOOL CaptureWin32(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam) = 0;

    // --------------------------------
    // Reproduce processus
    //
    virtual BOOL GetCtrlPos(HWND hDlg,LONG &x,LONG &y,LONG &cx,LONG &cy) const = 0;
    virtual BOOL CheckState(HWND hDlg) const = 0;

    BOOL CheckNextAct(ActType NextAct);
    ActType GetPrevAct() const;
    virtual ActType NextAction(HWND hDlg) = 0;

    virtual BOOL Start(HWND hDlg) = 0;
    virtual BOOL End(HWND hDlg) = 0;

    virtual BOOL MoveTo(HWND hDlg,int &x,int &y) = 0;
    virtual BOOL MoveEvent(HWND hDlg) = 0;
    virtual BOOL WaitUntil(HWND hDlg) = 0;

    virtual BOOL LeftPressEvent(HWND hDlg) = 0;
    virtual BOOL LeftUnpressEvent(HWND hDlg) = 0;
    virtual BOOL RightPressEvent(HWND hDlg) = 0;
    virtual BOOL RightUnpressEvent(HWND hDlg) = 0;
    virtual BOOL EditEvent(HWND hDlg) = 0;

};

//////
typedef void* (*PFN_CREATENEWINSTANCE)(UINT,LPCSTR);

#endif /* _INC_MHPCTRLMGMT */

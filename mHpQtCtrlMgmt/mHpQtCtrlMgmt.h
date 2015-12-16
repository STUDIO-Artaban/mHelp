// mHpQtCtrlMgmt.h - Interface for mHelp Qt Controls Management
// mHelp version - 1.1
// Date - 01/18/2013
// Copyright(c) - Unknown Corporation. All rights reserved.

#pragma once
#ifndef MHPQTCTRLMGMT_H
#define MHPQTCTRLMGMT_H

#include <mHpCtrlMgmt.h>

////// ImHpQtCtrlMgmt interface ///////////////////////////////////////////////
class ImHpQtCtrlMgmt {

public:
    virtual ~ImHpQtCtrlMgmt() {}
    static ImHpQtCtrlMgmt* getInstance(UINT iIdx, const char* lpDllPath);

public:
    virtual BOOL LoadQtLibrary() = 0;
    virtual void ClearQtData() = 0;
    virtual void CopyQtData(const ImHpCtrlMgmt& CopyData) = 0;
    virtual BOOL AddQtSlots(HWND hDlg) = 0;
    virtual ImHpCtrlMgmt* GetQtCtrlMgmt() = 0;

};

///////////////////////////////////////////////////////////////////////////////
__declspec(dllexport) void* CreateNewQtInst(UINT,LPCSTR);

#endif // MHPQTCTRLMGMT_H

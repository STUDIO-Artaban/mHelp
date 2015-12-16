// mHpDotNetCtrl.h
//

#ifndef MHPDOTNETCTRL_H
#define MHPDOTNETCTRL_H

#ifndef _INC_MHPCTRLMGMT
 #include <mHpCtrlMgmt.h> // mHelp control management interface
#endif

// --------------------------------
// Include DotNET CLR
//
#pragma region Includes and Imports
#include <windows.h>
#include <metahost.h>

#pragma comment(lib, "mscoree.lib")

// Import mscorlib.tlb (Microsoft Common Language Runtime Class Library).
#import "mscorlib.tlb" raw_interfaces_only \
    high_property_prefixes("_get","_put","_putref") \
    rename("ReportEvent", "InteropServices_ReportEvent")
using namespace mscorlib;
#pragma endregion

//////
#define MAX_DLL_PATH            256     // Max lenght for DLL path

class CmHpDotNetCtrl : public ImHpCtrlMgmt ////////////////////////////////////
{
private:
    char szDllPath[MAX_DLL_PATH];

    // ICorRuntimeHost and ICLRRuntimeHost are the two CLR hosting interfaces
    // supported by CLR 4.0. Here we demo the ICorRuntimeHost interface that 
    // was provided in .NET v1.x, and is compatible with all .NET Frameworks. 
    ICorRuntimeHost* pCorRuntimeHost;

    _TypePtr spType;
    variant_t vtObjInst;

    mutable BOOL bDotNetCallErr;
    mutable char* szDotNetErr;
    mutable char szDotNetName[DATA_NAME_SIZE];

public:
    CmHpDotNetCtrl(UINT nIdx,const char* lpDllPath);
    ~CmHpDotNetCtrl();

//////
private:
    void FillDotNetErr(const char* lpFunct,long iLine,const char* lpErrMsg) const;

public:
    //////
    const char* GetLastErr() const;

    // --------------------------------
    // DotNET tools
    //
    enum DotNetDataType { DataID, DataState, DataCfg };

    BOOL InitDotNetCLR();
    BOOL LoadDotNetLibrary();
    BOOL CheckDotNetErr() const;
    BOOL FillDotNetData(DotNetDataType dataType,BOOL bData=TRUE);
    BOOL UpdateDotNetData(DotNetDataType dataType) const;
    BOOL AddDotNetEvents(HWND hForm,HWND hCtrl) const;
    BOOL FillDotNetProperties();
    BOOL UpdateDotNetProperties();
    void ClearDotNetData() const;

    friend void CopyDotNetData(CmHpDotNetCtrl& DestData,const CmHpDotNetCtrl& CopyData);

    // --------------------------------
    // Properties
    //

//////
private:
    UINT iStateCount;
    UINT iCfgCount;

    BOOL FillData(UINT iDataIdx,CData::Type valType,DotNetDataType dataType);
    BOOL UpdateData(UINT iDataIdx,CData::Type valType,DotNetDataType dataType) const;

    BOOL SetDllVersion();
    BOOL GetCtrlObject();
    BOOL GetCtrlStyle();
    BOOL GetStateBound();
    BOOL GetCaptEvent();

    BOOL UpdateCtrlObject() const;
    BOOL UpdateStateBound() const;

    ////// Class name
public:
    BOOL IsCtrlClass(const char* lpClass) const;
    const char* GetCtrlClass(UINT iClassIdx) const;

    ////// Name
    const char* GetCtrlName(int iNameIdx = NO_VALUE) const;

    ////// Info
    void GetCtrlVers(char* lpVers,UINT iMaxSize) const;
    void GetCtrlDesc(char* lpDesc,UINT iMaxSize) const;
    HICON GetIconList(UINT iIconIdx) const;
    int GetIconIdx() const;

    ////// User Interface (UI)
    HWND CreateUI(HWND hParentUI,HWND hSaveBtn) const;
    void SaveUI(HWND hUI) const;

    void CloseDotNetUI() const;
    BOOL MoveDotNetUI(HWND hCfgDlg,HWND hCtrlUI) const;

    ////// Style
    BOOL SetStyle(int iStyleID);

//////
private:
    void GetDataName(char* lpName,UINT iDataIdx,const char* lpMethod) const;
    int GetDataType(UINT iDataIdx,const char* lpMethod) const;
    int GetDataCount(UINT iDataIdx,const char* lpMethod) const;
    BOOL GetDataIntVal(UINT iDataIdx,CData* pData,CData::Type dataType,int iValCount,const char* lpMethod) const;
    BOOL GetDataStrVal(UINT iDataIdx,CData* pData,CData::Type dataType,int iValCount,const char* lpMethod) const;

    BOOL SetDataIntVal(UINT iDataIdx,UINT iArrayIdx,int iValue,const char* lpMethod) const;
    BOOL SetDataStrVal(UINT iDataIdx,UINT iArrayIdx,const char* lpValue,const char* lpMethod) const;

    int GetIdCount() const;
    BOOL GetIdStrVal(int iValCount);
    BOOL SetIdStrVal(UINT iArrayIdx,const char* lpValue) const;

    ////// State
public:
    UINT GetStateCount() const;
    CCtrlState* GetState(UINT iStateIdx);
    BOOL NeedToCheckState() const;
    const char* DisplayState();

    ////// Config
    UINT GetCfgCount() const;
    CCtrlCfg* GetConfig(UINT iCfgIdx);

    ////// HTML tags
    void GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const;

    // --------------------------------
    // Capture processus
    //
private:
    int GetCtrlLoc(const char* lpMethod) const;

public:
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

    BOOL MoveTo(HWND hDlg,int &xPos,int &yPos);
    BOOL MoveEvent(HWND hDlg);
    BOOL WaitUntil(HWND hDlg);

    BOOL LeftPressEvent(HWND hDlg);
    BOOL LeftUnpressEvent(HWND hDlg);
    BOOL RightPressEvent(HWND hDlg);
    BOOL RightUnpressEvent(HWND hDlg);
    BOOL EditEvent(HWND hDlg);

};

#endif /* MHPDOTNETCTRL_H */

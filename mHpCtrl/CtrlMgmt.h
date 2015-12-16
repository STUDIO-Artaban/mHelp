// CtrlMgmt.h - CCtrlMgmtList declaration
//

#ifndef _INC_CTRLMGMTLIST
#define _INC_CTRLMGMTLIST

#ifndef _INC_MHPCTRLMGMT
 #include <mHpCtrlMgmt.h>
#endif

#ifndef MHPDOTNETCTRL_H
 #include "mHpDotNetCtrl.h"
#endif

#ifndef MHPQTCTRLMGMT_H
  #include <mHpQtCtrlMgmt.h>
#endif

#include <jni.h>
#ifndef MHPJAVACTRL_H
 #include "mHpJavaCtrl.h"
#endif

#ifndef  __MHELPPAK_H
 #include <mHpPak.h>
#endif

// --------------------------------
// Platforms
//
#define PLATFORM_ID_WIN32       0
#define PLATFORM_ID_DOTNET      1
#define PLATFORM_ID_QT          2
#define PLATFORM_ID_WPF         3
#define PLATFORM_ID_JAVA        4

#define PLATFORM_ID_SIZE        PLATFORM_ID_JAVA + 1

#define PLATFORM_WIN32          "Win32/MFC/VB6/Delphi"
#define PLATFORM_DOTNET         ".NET Framework 3.5"
#define PLATFORM_QT             "Qt Framework 4.8.3"
#define PLATFORM_WPF            "WPF (Framework 3.5)"
#define PLATFORM_JAVA           "Java - JDK 1.7"

static const LPCSTR CtrlPlatformID[PLATFORM_ID_SIZE] = {
    PLATFORM_WIN32,
    PLATFORM_DOTNET,
    PLATFORM_QT,
    PLATFORM_WPF,
    PLATFORM_JAVA
};

class CCtrlMgmtCell ///////////////////////////////////////////////////////////
{
private:
    HMODULE hmHpCtrlDll;
    PFN_CREATENEWINSTANCE CreatemHpCtrlFunc;
    ImHpCtrlMgmt* CurmHpCtrlMgmt;
    ImHpQtCtrlMgmt* CurmHpQtCtrlMgmt;
    jclass jCtrlClass;
    jmethodID jCtrlConstMthd;
    UINT iCtrlPlatformID;
    char szDllPath[MAX_DLL_PATH];

public:
    CCtrlMgmtCell* NextCell;    // Next control management cell
    UINT iIconCount;            // Number of icon loaded from current control management

public:
    CCtrlMgmtCell(HMODULE hDllModule,
                  PFN_CREATENEWINSTANCE CreatemHpCtrlFunc,
                  ImHpCtrlMgmt* pmHpCtrlMgmt,
                  ImHpQtCtrlMgmt* pmHpQtCtrl,
                  jclass pJavaClassCtrl,
                  jmethodID pJavaCtrlConstMthd,
                  UINT iPlatformID,
                  const char* lpDllPath);
    ~CCtrlMgmtCell();

    //////
    HMODULE GetCtrlDllModule() { return hmHpCtrlDll; }
    ImHpCtrlMgmt* GetmHpCtrlMgmt() { return CurmHpCtrlMgmt; }
    ImHpQtCtrlMgmt* GetmHpQtCtrlMgmt() { return CurmHpQtCtrlMgmt; }
    const char* GetCtrlPlatform() const { return CtrlPlatformID[iCtrlPlatformID]; }
    UINT GetCtrlPlatformID() const { return iCtrlPlatformID; }

    // Return the current mHpCtrlMgmt instance and assign a new empty one to the CurmHpCtrlMgmt
    // those because of CaptureMessage() method change the current mHpCtrlMgmt instance
    ImHpCtrlMgmt* CreatemHpCtrlMgmt(UINT);

    // Return a new empty DotNET/Qt mHpCtrlMgmt instance and clear all data of the CurmHpCtrlMgmt
    // -> Need to keep CurmHpCtrlMgmt that contains all mHelp event handler
    ImHpCtrlMgmt* CreatemHpDotNetCtrl(UINT);
    ImHpQtCtrlMgmt* CreatemHpQtCtrl(UINT);

};

class CCtrlDisplayCell ////////////////////////////////////////////////////////
{
private:
    char lpCtrlClass[CTRL_CLASS_SIZE];          // Control class/object name
    char lpCtrlDllStatus[MAX_MID_SIZE];         // Control DLL load status
    char lpCtrlName[DATA_NAME_SIZE];            // Control name
    char lpCtrlDllName[MAX_SHT_SIZE];           // Control DLL name
    char lpCtrlDllVersion[DLL_VERSION_SIZE];    // Control DLL version
    const char* lpCtrlPltfrm;                   // Control platform
    HICON hCtrlIcon;                            // Control icon

public:
    CCtrlDisplayCell* NextCell;   // Next control management ListView display cell

public:
    CCtrlDisplayCell();
    ~CCtrlDisplayCell();

    void FillDllLoadedErr(const char* lpPlatform,const char* lpDllName,const char* lpErrMsg);
    void FillDllLoadedSucceed(HICON hIcon,
                              const char* lpName,
                              const char* lpPlatform,
                              const char* lpClass,
                              const char* lpDll,
                              const char* lpVersion);

    //////
    HICON GetIcon() const { return hCtrlIcon; }
    const char* GetName() const { return lpCtrlName; }
    const char* GetPlatform() const { return lpCtrlPltfrm; }
    const char* GetClass() const { return lpCtrlClass; }
    const char* GetDll() const { return lpCtrlDllName; }
    const char* GetVersion() const { return lpCtrlDllVersion; }
    const char* GetStatus() const { return lpCtrlDllStatus; }

};

//------------------------------------------------------------ CCtrlMgmtList //
class CCtrlMgmtList
{
private:
    CCtrlMgmtCell* HeadCtrlMgmt;
    CCtrlDisplayCell* HeadCtrlLV;
    HMODULE hmHpQtModule;
    JNIEnv* jEnv;
    JavaVM* jVM;
    jint iCreatedVM;
    char* szJavaClassPath;

public:
    CCtrlMgmtList();
    ~CCtrlMgmtList();

    //////
    void FillList(HWND);
    void FillListView(HWND) const;
    void FillImgList(HIMAGELIST) const;
    ImHpCtrlMgmt* ReadEvent(const char*,UINT);
    void CaptureEvent(HWND,UINT,WPARAM,LPARAM,ImHpCtrlMgmt*&,UINT&) const;
    CCtrlMgmtCell* GetCtrlMgmtCell(UINT);
    void EmptyList();

    ////// DotNET Platform
    BOOL AddDotNetCtrlEvents(HWND,HWND);

    ////// Qt, WPF & Java Platforms
    BOOL AddCtrlEvents(HWND);

private:
    void FillDisplayList(CCtrlDisplayCell*);

};

#endif
// mHpJavaCtrl.h
//

#ifndef MHPJAVACTRL_H
#define MHPJAVACTRL_H

#ifndef _INC_MHPCTRLMGMT
 #include <mHpCtrlMgmt.h> // mHelp control management interface
#endif

//////
#define JAVA_JNI_VERSION        "-Djava.version=1.6" // JNI version option
#define JAVA_CLASS_PATH         "-Djava.class.path=" // Java class path option

// --------------------------------
// Include JNI (Java Native Interface)
//
#include <jni.h>

class CmHpJavaCtrl : public ImHpCtrlMgmt //////////////////////////////////////
{
private:
    JNIEnv* jenv;
    jclass jclssCtrl;
    jobject jobjCtrl;

    mutable char szJavaName[DATA_NAME_SIZE];

    //////
    jmethodID jmthd_AddJavaEvents;
    jmethodID jmthd_SetDllVersion;
    jmethodID jmthd_GetCtrlClass;
    jmethodID jmthd_GetCtrlName;

    //
    // TODO: Define all methods here
    //

public:
    CmHpJavaCtrl(UINT nIdx,JNIEnv* pEnv,jclass NewCtrlClss,jobject NewCtrlObj);
    virtual ~CmHpJavaCtrl();

    // --------------------------------
    // Java tools
    //
    BOOL InitJavaVM();
    BOOL AddJavaEvents(BOOL bCreatedVM) const;

    // --------------------------------
    // Properties
    //

    ////// Class name
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

    ////// Style
    BOOL SetStyle(int iStyleID);

    ////// State
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

#endif /* MHPJAVACTRL_H */

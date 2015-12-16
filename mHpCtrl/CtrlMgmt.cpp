// CtrlMgmt.cpp - CCtrlMgmtList implementation
//

#include "stdafx.h"
#include <stdio.h>
#include <list>
#include "CtrlMgmt.h"
#include "resource.h"

#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif

#ifdef _WIN64
#define WIN64_SYSTEM_FOLDER         "SysWOW64"
#endif

#define EHQTCTRLMGMT_DLLNAME        "mHpQtCtrlMgmt.dll"

// class CCtrlMgmtCell ////////////////////////////////////////////////////////
CCtrlMgmtCell::CCtrlMgmtCell(HMODULE hDllModule,
                             PFN_CREATENEWINSTANCE pCreatemHpCtrlFunc,
                             ImHpCtrlMgmt* pmHpCtrlMgmt,
                             ImHpQtCtrlMgmt* pmHpQtCtrlMgmt,
                             jclass pJavaClassCtrl,
                             jmethodID pJavaCtrlConstMthd,
                             UINT iPlatformID,
                             const char* lpDllPath)
{   hmHpCtrlDll=hDllModule;
    CreatemHpCtrlFunc=pCreatemHpCtrlFunc;
    CurmHpCtrlMgmt=pmHpCtrlMgmt;
    CurmHpQtCtrlMgmt=pmHpQtCtrlMgmt;
    jCtrlClass=pJavaClassCtrl;
    jCtrlConstMthd=pJavaCtrlConstMthd;
    iIconCount=0;
    NextCell=NULL;
    iCtrlPlatformID=iPlatformID;
    if(lpDllPath!=NULL)
        strncpy(szDllPath,lpDllPath,MAX_DLL_PATH);
    else
        *szDllPath=0;
}
CCtrlMgmtCell::~CCtrlMgmtCell()
{   delete CurmHpCtrlMgmt;
}

//////
ImHpCtrlMgmt* CCtrlMgmtCell::CreatemHpCtrlMgmt(UINT iIdx)
{   ImHpCtrlMgmt* RetCtrlMgmt=CurmHpCtrlMgmt;
    CurmHpCtrlMgmt=static_cast<ImHpCtrlMgmt*>(CreatemHpCtrlFunc(iIdx,szDllPath));
    return RetCtrlMgmt;
}

ImHpCtrlMgmt* CCtrlMgmtCell::CreatemHpDotNetCtrl(UINT iIdx)
{   return static_cast<ImHpCtrlMgmt*>(new CmHpDotNetCtrl(iIdx,szDllPath));
}

ImHpQtCtrlMgmt* CCtrlMgmtCell::CreatemHpQtCtrl(UINT iIdx)
{   return static_cast<ImHpQtCtrlMgmt*>(CreatemHpCtrlFunc(iIdx,szDllPath));
}

// class CCtrlDisplayCell /////////////////////////////////////////////////////
CCtrlDisplayCell::CCtrlDisplayCell() : NextCell(NULL), hCtrlIcon(NULL), lpCtrlPltfrm(NULL)
{   *lpCtrlClass=0;
    *lpCtrlDllStatus=0;
    *lpCtrlName=0;
    *lpCtrlDllName=0;
    *lpCtrlDllVersion=0;
}
CCtrlDisplayCell::~CCtrlDisplayCell()
{}

//////
void CCtrlDisplayCell::FillDllLoadedErr(const char* lpPlatform,const char* lpDllName,const char* lpErrMsg)
{
    hCtrlIcon=LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_DEFAULT));
    *lpCtrlName='-';*(lpCtrlName+1)=0;
    lpCtrlPltfrm=lpPlatform;
    *lpCtrlClass='-';*(lpCtrlClass+1)=0;
    *lpCtrlDllVersion='-';*(lpCtrlDllVersion+1)=0;
    strncpy(lpCtrlDllName,lpDllName,MAX_SHT_SIZE);
    LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTLMGMT_LOAD_ERROR,lpCtrlDllStatus,MAX_MID_SIZE);
    strncat(lpCtrlDllStatus,lpErrMsg,strlen(lpErrMsg));
}

void CCtrlDisplayCell::FillDllLoadedSucceed(HICON hIcon,
                                            const char* lpName,
                                            const char* lpPlatform,
                                            const char* lpClass,
                                            const char* lpDll,
                                            const char* lpVersion)
{   BOOL bErr=FALSE;
    // Icon
    if(hIcon)
        hCtrlIcon=hIcon;
    else
        hCtrlIcon=LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_DEFAULT));
    // Control name
    if(lpName)
        strncpy(lpCtrlName,lpName,DATA_NAME_SIZE);
    else
    {   LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTLMGMT_LOAD_NONAME,lpCtrlName,DATA_NAME_SIZE);
        bErr=TRUE;
    }
    // Platform
    lpCtrlPltfrm=lpPlatform;
    // Control class name
    if(lpClass)
        strncpy(lpCtrlClass,lpClass,CTRL_CLASS_SIZE);
    else
    {   LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTLMGMT_LOAD_NOCLASS,lpCtrlClass,CTRL_CLASS_SIZE);
        bErr=TRUE;
    }
    // Dll name
    strncpy(lpCtrlDllName,lpDll,MAX_SHT_SIZE);
    // Dll version
    if(lpVersion)
        strncpy(lpCtrlDllVersion,lpVersion,DLL_VERSION_SIZE);
    else
    {   LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTLMGMT_LOAD_NOVERSION,lpCtrlDllVersion,DLL_VERSION_SIZE);
        bErr=TRUE;
    }
    // Status
    if(!bErr)
        LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTLMGMT_LOAD_SUCCEED,lpCtrlDllStatus,MAX_MID_SIZE);
    else
        LoadString(GetmHpVarStruct()->hmHpCtrlInst,IDS_CTLMGMT_LOAD_ERROR,lpCtrlDllStatus,MAX_MID_SIZE);
}

// class CCtrlMgmtList ////////////////////////////////////////////////////////
CCtrlMgmtList::CCtrlMgmtList() : HeadCtrlMgmt(NULL), HeadCtrlLV(NULL), hmHpQtModule(NULL), jEnv(NULL), jVM(NULL),
               iCreatedVM(NO), szJavaClassPath(NULL)
{}
CCtrlMgmtList::~CCtrlMgmtList()
{   EmptyList();
}

//////
void CCtrlMgmtList::FillList(HWND hWndActive)
{
    char lpSystem[MAX_PATH];
#ifdef _WIN64
    if(!GetWindowsDirectory(lpSystem,MAX_PATH))
    {   mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_ERROR_GETSYSFOLDER,NULL,0,0,MB_ICONERROR);
        return;
    }
    strncat(lpSystem,"\\",sizeof("\\"));
    strncat(lpSystem,WIN64_SYSTEM_FOLDER,sizeof(WIN64_SYSTEM_FOLDER));
#else
    if(!GetSystemDirectory(lpSystem,MAX_PATH))
    {   mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_ERROR_GETSYSFOLDER,NULL,0,0,MB_ICONERROR);
        return;
    }
#endif

    ////// Win32

    //
    // TODO: Check existing \mHelp\CtrlMgmt\Win32 folder
    //

    UINT iCtrlMgmtIdx=0;

    char lpDllFilter[MAX_PATH];
    _snprintf(lpDllFilter,MAX_PATH,"%s\\mHelp\\CtrlMgmt\\Win32\\*.dll",lpSystem);
    WIN32_FIND_DATA FindFileData;
    HANDLE hNextFile=FindFirstFile(lpDllFilter,&FindFileData);
    if(hNextFile!=INVALID_HANDLE_VALUE)
    {   do
        {   // Load library
            char lpDllPath[MAX_PATH];
            char lpErr[MAX_SHT_SIZE]={0};
            _snprintf(lpDllPath,MAX_PATH,"%s\\mHelp\\CtrlMgmt\\Win32\\%s",lpSystem,FindFileData.cFileName);
            HMODULE hDllModule=LoadLibrary(lpDllPath);
            if(hDllModule)
            {
                // Get function that create new mHpCtrlMgmt instance
                PFN_CREATENEWINSTANCE CreateNewCtrlMgmt=(PFN_CREATENEWINSTANCE)GetProcAddress(hDllModule,"CreateNewInstance");
                if(CreateNewCtrlMgmt)
                {
                    // Create instance of the mHpCtrlMgmt reference
                    ImHpCtrlMgmt* pRefCtrlMgmtInst=static_cast<ImHpCtrlMgmt*>(CreateNewCtrlMgmt(iCtrlMgmtIdx,NULL));
                    if(pRefCtrlMgmtInst)
                    {
                        // Add control management Dll infos
                        int iNameIdx=0;
                        while(pRefCtrlMgmtInst->GetCtrlName(iNameIdx))
                        {   CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                            pCurDisplayCell->FillDllLoadedSucceed(pRefCtrlMgmtInst->GetIconList((UINT)iNameIdx),
                                                                  pRefCtrlMgmtInst->GetCtrlName(iNameIdx),
                                                                  CtrlPlatformID[PLATFORM_ID_WIN32],
                                                                  pRefCtrlMgmtInst->GetCtrlClass(iNameIdx),
                                                                  FindFileData.cFileName,
                                                                  pRefCtrlMgmtInst->GetDllVersion());
                            // Add into display list
                            FillDisplayList(pCurDisplayCell);
                            iNameIdx++;
                        }
                        // Check existing one control name at least
                        if(iNameIdx)
                        {
                            ////// Add control management element
                            if(!HeadCtrlMgmt)
                                HeadCtrlMgmt=new CCtrlMgmtCell(hDllModule,
                                                               CreateNewCtrlMgmt,
                                                               pRefCtrlMgmtInst,
                                                               NULL,
                                                               NULL,
                                                               NULL,
                                                               PLATFORM_ID_WIN32,
                                                               NULL);
                            else
                            {   CCtrlMgmtCell* LastCell=HeadCtrlMgmt;
                                while(LastCell->NextCell != NULL)
                                    LastCell=LastCell->NextCell;
                                LastCell->NextCell=new CCtrlMgmtCell(hDllModule,
                                                                     CreateNewCtrlMgmt,
                                                                     pRefCtrlMgmtInst,
                                                                     NULL,
                                                                     NULL,
                                                                     NULL,
                                                                     PLATFORM_ID_WIN32,
                                                                     NULL);
                            }
                            iCtrlMgmtIdx++;
                        }
                        else
                        {
                            // Error: No control name defined (Add control management Dll error infos)
                            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                            pCurDisplayCell->FillDllLoadedSucceed(pRefCtrlMgmtInst->GetIconList(0),
                                                                  pRefCtrlMgmtInst->GetCtrlName(0),
                                                                  CtrlPlatformID[PLATFORM_ID_WIN32],
                                                                  pRefCtrlMgmtInst->GetCtrlClass(0),
                                                                  FindFileData.cFileName,
                                                                  pRefCtrlMgmtInst->GetDllVersion());
                            // Add into display list
                            FillDisplayList(pCurDisplayCell);

                            // Free library & delete instance
                            FreeLibrary(hDllModule);
                            delete pRefCtrlMgmtInst;
                        }
                        continue; // Next Win32 Dll file
                    }
                    else
                        strncpy(lpErr," (CreateNewInstance error)",MAX_SHT_SIZE);
                }
                else
                    _snprintf(lpErr,MAX_SHT_SIZE," (GetProcAddress error: %d)",GetLastError());

                // Free library
                FreeLibrary(hDllModule);
            }
            else
                _snprintf(lpErr,MAX_SHT_SIZE," (LoadLibrary error: %d)",GetLastError());

            // Error: Add Dll load error infos
            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
            pCurDisplayCell->FillDllLoadedErr(CtrlPlatformID[PLATFORM_ID_WIN32],FindFileData.cFileName,lpErr);

            // Add into display list
            FillDisplayList(pCurDisplayCell);

        } while(FindNextFile(hNextFile,&FindFileData));
        FindClose(hNextFile);
    }

    ////// DotNET

    //
    // TODO: Check existing \mHelp\CtrlMgmt\DotNET folder
    //

    _snprintf(lpDllFilter,MAX_PATH,"%s\\mHelp\\CtrlMgmt\\DotNET\\*.dll",lpSystem);
    hNextFile=FindFirstFile(lpDllFilter,&FindFileData);
    if(hNextFile!=INVALID_HANDLE_VALUE)
    {   do
        {   // Get DLL path
            char lpDllPath[MAX_DLL_PATH];
            _snprintf(lpDllPath,MAX_DLL_PATH,"%s\\mHelp\\CtrlMgmt\\DotNET\\%s",lpSystem,FindFileData.cFileName);

            // Create instance of the mHpCtrlMgmt reference
            char lpErr[MAX_SHT_SIZE]={0};
            ImHpCtrlMgmt* pRefCtrlMgmtInst=static_cast<ImHpCtrlMgmt*>(new CmHpDotNetCtrl(iCtrlMgmtIdx,lpDllPath));
            if(pRefCtrlMgmtInst)
            {
                // Initialize DotNET CLR
                CmHpDotNetCtrl* pRefDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pRefCtrlMgmtInst);
                if(pRefDotNetCtrl->InitDotNetCLR())
                {
                    // Load DotNET library
                    if(pRefDotNetCtrl->LoadDotNetLibrary())
                    {
                        // Add control management Dll infos
                        int iNameIdx=0;
                        while(pRefDotNetCtrl->GetCtrlName(iNameIdx))
                        {   CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                            pCurDisplayCell->FillDllLoadedSucceed(pRefDotNetCtrl->GetIconList((UINT)iNameIdx),
                                                                  pRefDotNetCtrl->GetCtrlName(iNameIdx),
                                                                  CtrlPlatformID[PLATFORM_ID_DOTNET],
                                                                  pRefDotNetCtrl->GetCtrlClass(iNameIdx),
                                                                  FindFileData.cFileName,
                                                                  pRefDotNetCtrl->GetDllVersion());
                            // Add into display list
                            FillDisplayList(pCurDisplayCell);
                            iNameIdx++;
                        }
                        // Check existing one control name at least
                        if(iNameIdx)
                        {
                            ////// Add control management element
                            if(!HeadCtrlMgmt)
                                HeadCtrlMgmt=new CCtrlMgmtCell(NULL,
                                                               NULL,
                                                               pRefCtrlMgmtInst,
                                                               NULL,
                                                               NULL,
                                                               NULL,
                                                               PLATFORM_ID_DOTNET,
                                                               lpDllPath);
                            else
                            {   CCtrlMgmtCell* LastCell=HeadCtrlMgmt;
                                while(LastCell->NextCell != NULL)
                                    LastCell=LastCell->NextCell;
                                LastCell->NextCell=new CCtrlMgmtCell(NULL,
                                                                     NULL,
                                                                     pRefCtrlMgmtInst,
                                                                     NULL,
                                                                     NULL,
                                                                     NULL,
                                                                     PLATFORM_ID_DOTNET,
                                                                     lpDllPath);
                            }
                            iCtrlMgmtIdx++;
                        }
                        else
                        {
                            // Error: No control name defined (Add control management Dll error infos)
                            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                            pCurDisplayCell->FillDllLoadedSucceed(pRefDotNetCtrl->GetIconList(0),
                                                                  pRefDotNetCtrl->GetCtrlName(0),
                                                                  CtrlPlatformID[PLATFORM_ID_DOTNET],
                                                                  pRefDotNetCtrl->GetCtrlClass(0),
                                                                  FindFileData.cFileName,
                                                                  pRefDotNetCtrl->GetDllVersion());
                            // Add into display list
                            FillDisplayList(pCurDisplayCell);

                            // Free library & delete instance
                            delete pRefDotNetCtrl;
                        }
                        continue; // Next DotNET Dll file
                    }
                    else
                        strncpy(lpErr," (LoadDotNetLibrary error)",MAX_SHT_SIZE);
                }
                else
                    strncpy(lpErr," (InitDotNetCLR error)",MAX_SHT_SIZE);

                //
                // TODO: Use error message from 'CmHpDotNetCtrl'
                //

                pRefDotNetCtrl->GetLastErr();
                delete pRefDotNetCtrl;
            }
            else
                strncpy(lpErr," (CreateNewInstance error)",MAX_SHT_SIZE);

            // Error: Add Dll load error infos
            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
            pCurDisplayCell->FillDllLoadedErr(CtrlPlatformID[PLATFORM_ID_DOTNET],FindFileData.cFileName,lpErr);

            // Add into display list
            FillDisplayList(pCurDisplayCell);

        } while(FindNextFile(hNextFile,&FindFileData));
        FindClose(hNextFile);
    }

    ////// Qt

    //
    // TODO: Check existing \mHelp\CtrlMgmt\Qt folder
    //

    _snprintf(lpDllFilter,MAX_PATH,"%s\\mHelp\\CtrlMgmt\\Qt\\*.dll",lpSystem);
    hNextFile=FindFirstFile(lpDllFilter,&FindFileData);
    if(hNextFile!=INVALID_HANDLE_VALUE)
    {
        SetThreadErrorMode(SEM_FAILCRITICALERRORS, NULL); //SetErrorMode(SEM_FAILCRITICALERRORS);
        // TODO: Check what happen on different Win7 system?

        PFN_CREATENEWINSTANCE CreateNewCtrlMgmt=NULL;
        hmHpQtModule=LoadLibrary(EHQTCTRLMGMT_DLLNAME);
        if(hmHpQtModule)
        {   // Get function that create new CmHpQtCtrl instance
            CreateNewCtrlMgmt=(PFN_CREATENEWINSTANCE)GetProcAddress(hmHpQtModule,"CreateNewQtInst");

            assert(CreateNewCtrlMgmt);
            if(!CreateNewCtrlMgmt)
            {   FreeLibrary(hmHpQtModule); // Free Qt library
                hmHpQtModule=NULL;
            }
        }

        do
        {   // Get DLL path
            char lpDllPath[MAX_DLL_PATH];
            char lpErr[MAX_SHT_SIZE]={0};
            _snprintf(lpDllPath,MAX_DLL_PATH,"%s\\mHelp\\CtrlMgmt\\Qt\\%s",lpSystem,FindFileData.cFileName);
            if(hmHpQtModule)
            {
                // Create instance of the CmHpQtCtrl reference
                ImHpQtCtrlMgmt* pRefCtrlMgmtInst=static_cast<ImHpQtCtrlMgmt*>(CreateNewCtrlMgmt(iCtrlMgmtIdx,lpDllPath));
                if(pRefCtrlMgmtInst)
                {
                    // Load Qt library
                    if(pRefCtrlMgmtInst->LoadQtLibrary())
                    {
                        ImHpCtrlMgmt* pRefQtCtrl=pRefCtrlMgmtInst->GetQtCtrlMgmt();
                        pRefQtCtrl->FillQtData(); // Set DLL version

                        // Add control management Dll infos
                        int iNameIdx=0;
                        while(pRefQtCtrl->GetCtrlName(iNameIdx))
                        {   CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                            pCurDisplayCell->FillDllLoadedSucceed(pRefQtCtrl->GetIconList((UINT)iNameIdx),
                                                                  pRefQtCtrl->GetCtrlName(iNameIdx),
                                                                  CtrlPlatformID[PLATFORM_ID_QT],
                                                                  pRefQtCtrl->GetCtrlClass(iNameIdx),
                                                                  FindFileData.cFileName,
                                                                  pRefQtCtrl->GetDllVersion());
                            // Add into display list
                            FillDisplayList(pCurDisplayCell);
                            iNameIdx++;
                        }
                        // Check existing one control name at least
                        if(iNameIdx)
                        {
                            ////// Add control management element
                            if(!HeadCtrlMgmt)
                                HeadCtrlMgmt=new CCtrlMgmtCell(NULL,
                                                               CreateNewCtrlMgmt,
                                                               NULL,
                                                               pRefCtrlMgmtInst,
                                                               NULL,
                                                               NULL,
                                                               PLATFORM_ID_QT,
                                                               lpDllPath);
                            else
                            {   CCtrlMgmtCell* LastCell=HeadCtrlMgmt;
                                while(LastCell->NextCell != NULL)
                                    LastCell=LastCell->NextCell;
                                LastCell->NextCell=new CCtrlMgmtCell(NULL,
                                                                     CreateNewCtrlMgmt,
                                                                     NULL,
                                                                     pRefCtrlMgmtInst,
                                                                     NULL,
                                                                     NULL,
                                                                     PLATFORM_ID_QT,
                                                                     lpDllPath);
                            }
                            iCtrlMgmtIdx++;
                        }
                        else
                        {
                            // Error: No control name defined (Add control management Dll error infos)
                            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                            pCurDisplayCell->FillDllLoadedSucceed(pRefQtCtrl->GetIconList(0),
                                                                    pRefQtCtrl->GetCtrlName(0),
                                                                    CtrlPlatformID[PLATFORM_ID_QT],
                                                                    pRefQtCtrl->GetCtrlClass(0),
                                                                    FindFileData.cFileName,
                                                                    pRefQtCtrl->GetDllVersion());
                            // Add into display list
                            FillDisplayList(pCurDisplayCell);

                            // Free library & delete instance
                            delete pRefCtrlMgmtInst;
                        }
                        continue; // Next Qt Dll file
                    }
                    else
                        strncpy(lpErr," (LoadQtLibrary error)",MAX_SHT_SIZE);

                    // Delete instance
                    delete pRefCtrlMgmtInst;
                }
                else
                    strncpy(lpErr," (CreateNewInstance error)",MAX_SHT_SIZE);
            }
            else
                strncpy(lpErr," (No Qt Framework 4.8.3 installed)",MAX_SHT_SIZE);

            // Error: Add Dll load error infos
            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
            pCurDisplayCell->FillDllLoadedErr(CtrlPlatformID[PLATFORM_ID_QT],FindFileData.cFileName,lpErr);

            // Add into display list
            FillDisplayList(pCurDisplayCell);
        }
        while(FindNextFile(hNextFile,&FindFileData));
        FindClose(hNextFile);
    }

    ////// WPF

    //
    // TODO: Check existing \mHelp\CtrlMgmt\WPF folder
    //

    _snprintf(lpDllFilter,MAX_PATH,"%s\\mHelp\\CtrlMgmt\\WPF\\*.dll",lpSystem);
    hNextFile=FindFirstFile(lpDllFilter,&FindFileData);
    if(hNextFile!=INVALID_HANDLE_VALUE)
    {   do
        {   // Get DLL path
            char lpDllPath[MAX_DLL_PATH];
            _snprintf(lpDllPath,MAX_DLL_PATH,"%s\\mHelp\\CtrlMgmt\\WPF\\%s",lpSystem,FindFileData.cFileName);

            // Create instance of the mHpCtrlMgmt reference
            char lpErr[MAX_SHT_SIZE]={0};
            ImHpCtrlMgmt* pRefCtrlMgmtInst=static_cast<ImHpCtrlMgmt*>(new CmHpDotNetCtrl(iCtrlMgmtIdx,lpDllPath));
            if(pRefCtrlMgmtInst)
            {
                // Initialize DotNET CLR
                CmHpDotNetCtrl* pRefDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pRefCtrlMgmtInst);
                if(pRefDotNetCtrl->InitDotNetCLR())
                {
                    // Load WPF library
                    if(pRefDotNetCtrl->LoadDotNetLibrary())
                    {
                        // Add control management Dll infos
                        int iNameIdx=0;
                        while(pRefDotNetCtrl->GetCtrlName(iNameIdx))
                        {   CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                            pCurDisplayCell->FillDllLoadedSucceed(pRefDotNetCtrl->GetIconList((UINT)iNameIdx),
                                                                  pRefDotNetCtrl->GetCtrlName(iNameIdx),
                                                                  CtrlPlatformID[PLATFORM_ID_WPF],
                                                                  pRefDotNetCtrl->GetCtrlClass(iNameIdx),
                                                                  FindFileData.cFileName,
                                                                  pRefDotNetCtrl->GetDllVersion());
                            // Add into display list
                            FillDisplayList(pCurDisplayCell);
                            iNameIdx++;
                        }
                        // Check existing one control name at least
                        if(iNameIdx)
                        {
                            ////// Add control management element
                            if(!HeadCtrlMgmt)
                                HeadCtrlMgmt=new CCtrlMgmtCell(NULL,
                                                               NULL,
                                                               pRefCtrlMgmtInst,
                                                               NULL,
                                                               NULL,
                                                               NULL,
                                                               PLATFORM_ID_WPF,
                                                               lpDllPath);
                            else
                            {   CCtrlMgmtCell* LastCell=HeadCtrlMgmt;
                                while(LastCell->NextCell != NULL)
                                    LastCell=LastCell->NextCell;
                                LastCell->NextCell=new CCtrlMgmtCell(NULL,
                                                                     NULL,
                                                                     pRefCtrlMgmtInst,
                                                                     NULL,
                                                                     NULL,
                                                                     NULL,
                                                                     PLATFORM_ID_WPF,
                                                                     lpDllPath);
                            }
                            iCtrlMgmtIdx++;
                        }
                        else
                        {
                            // Error: No control name defined (Add control management Dll error infos)
                            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                            pCurDisplayCell->FillDllLoadedSucceed(pRefDotNetCtrl->GetIconList(0),
                                                                  pRefDotNetCtrl->GetCtrlName(0),
                                                                  CtrlPlatformID[PLATFORM_ID_WPF],
                                                                  pRefDotNetCtrl->GetCtrlClass(0),
                                                                  FindFileData.cFileName,
                                                                  pRefDotNetCtrl->GetDllVersion());
                            // Add into display list
                            FillDisplayList(pCurDisplayCell);

                            // Free library & delete instance
                            delete pRefDotNetCtrl;
                        }
                        continue; // Next WPF Dll file
                    }
                    else
                        strncpy(lpErr," (LoadDotNetLibrary error)",MAX_SHT_SIZE);
                }
                else
                    strncpy(lpErr," (InitDotNetCLR error)",MAX_SHT_SIZE);

                //
                // TODO: Use error message from 'CmHpDotNetCtrl'
                //

                pRefDotNetCtrl->GetLastErr();
                delete pRefDotNetCtrl;
            }
            else
                strncpy(lpErr," (CreateNewInstance error)",MAX_SHT_SIZE);

            // Error: Add Dll load error infos
            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
            pCurDisplayCell->FillDllLoadedErr(CtrlPlatformID[PLATFORM_ID_WPF],FindFileData.cFileName,lpErr);

            // Add into display list
            FillDisplayList(pCurDisplayCell);

        } while(FindNextFile(hNextFile,&FindFileData));
        FindClose(hNextFile);
    }

    ////// Java

    //
    // TODO: Check existing \mHelp\CtrlMgmt\Java folder
    //

    _snprintf(lpDllFilter,MAX_PATH,"%s\\mHelp\\CtrlMgmt\\Java\\*.jar",lpSystem);
    hNextFile=FindFirstFile(lpDllFilter,&FindFileData);
    if(hNextFile!=INVALID_HANDLE_VALUE)
    {
        std::list<std::string> lstErrJarFile;
        std::list<std::string> lstClassName;
        std::string strJavaClassPath=JAVA_CLASS_PATH;
        do
        {   // Get JAR file path
            char lpJarFilePath[MAX_DLL_PATH];
            _snprintf(lpJarFilePath,MAX_DLL_PATH,"%s\\mHelp\\CtrlMgmt\\Java\\%s",lpSystem,FindFileData.cFileName);

            // Add JAR file for error message
            lstErrJarFile.push_back(FindFileData.cFileName);

            // Add JAR file path option
            if(strJavaClassPath.length()>sizeof(JAVA_CLASS_PATH))
                strJavaClassPath.append(";");
            strJavaClassPath.append(lpJarFilePath);

            // Add control class name
            size_t nFileSize=strlen(FindFileData.cFileName);
            char lpClassName[MAX_KEY_SIZE]={0};
            strncpy(lpClassName,FindFileData.cFileName,nFileSize);
            lpClassName[nFileSize-4]=0; // Remove .jar extension

            char lpCtrlClass[MAX_RS_SIZE];
            _snprintf(lpCtrlClass,MAX_RS_SIZE,"%s/%s",lpClassName,lpClassName); // -> FileName/FileName
            lstClassName.push_back(lpCtrlClass);

        } while(FindNextFile(hNextFile,&FindFileData));
        FindClose(hNextFile);

        // Get control class(es) name path
        szJavaClassPath=new char[strJavaClassPath.length()+2];
        strncpy(szJavaClassPath,strJavaClassPath.c_str(),strJavaClassPath.length()+1);

        // Add Java options:
        JavaVMOption jvmOptions[2];
        jvmOptions[0].optionString=szJavaClassPath; // Class path
        jvmOptions[1].optionString=JAVA_JNI_VERSION; // JNI version

        JavaVMInitArgs jvmArgs;
        jvmArgs.version=JNI_VERSION_1_6;
        jvmArgs.options=jvmOptions;
        jvmArgs.nOptions=2; // 2 options
        jvmArgs.ignoreUnrecognized=0;

        char lpErr[MAX_SHT_SIZE]={0};

        // Create Java VM (if not already exist)
        iCreatedVM=JNI_CreateJavaVM(&jVM,(void**)&jEnv,&jvmArgs);
        if(iCreatedVM<0)
        {
            jsize VMCount;
            JNI_GetCreatedJavaVMs(&jVM,1,&VMCount);
            if(jVM!=NULL)
            {   
                jVM->GetEnv((void**)&jEnv,JNI_VERSION_1_6);
                if(jEnv==NULL)
                    strncpy(lpErr," (GetEnv failed)",MAX_SHT_SIZE);
            }
            else
                strncpy(lpErr," (JNI_CreateJavaVM & JNI_GetCreatedJavaVMs failed)",MAX_SHT_SIZE);
        }

        // Loop on control class found
        int iErrJar=0;
        for (std::list<std::string>::iterator itrClass=lstClassName.begin();
             itrClass!=lstClassName.end();
             itrClass++, iErrJar++)
        {
            std::list<std::string>::iterator itrJarFile=lstErrJarFile.begin();
            std::advance(itrJarFile,iErrJar);

            // Check valid Java environment
            if(jEnv!=NULL)
            {
                // Find class
                jclass CtrlClass=jEnv->FindClass((*itrClass).c_str());
                if(CtrlClass!=NULL)
                {
                    // Get constructor method
                    jmethodID CtrlConstMthd=jEnv->GetMethodID(CtrlClass,"<init>","()V");
                    if(CtrlConstMthd!=NULL)
                    {
                        // Create control object instance
                        jobject CtrlObj=jEnv->NewObject(CtrlClass,CtrlConstMthd);
                        if(CtrlObj!=NULL)
                        {
                            // Create instance of the CmHpJavaCtrl reference
                            CmHpJavaCtrl* pRefJavaCtrlInst=new CmHpJavaCtrl(iCtrlMgmtIdx,jEnv,CtrlClass,CtrlObj);
                            if(pRefJavaCtrlInst)
                            {
                                // Initialize Java class
                                if(pRefJavaCtrlInst->InitJavaVM())
                                {
                                    // Add control management Dll infos
                                    int iNameIdx=0;
                                    while(pRefJavaCtrlInst->GetCtrlName(iNameIdx))
                                    {   CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                                        pCurDisplayCell->FillDllLoadedSucceed(pRefJavaCtrlInst->GetIconList((UINT)iNameIdx),
                                                                              pRefJavaCtrlInst->GetCtrlName(iNameIdx),
                                                                              CtrlPlatformID[PLATFORM_ID_JAVA],
                                                                              pRefJavaCtrlInst->GetCtrlClass(iNameIdx),
                                                                              (*itrJarFile).c_str(),
                                                                              pRefJavaCtrlInst->GetDllVersion());
                                        // Add into display list
                                        FillDisplayList(pCurDisplayCell);
                                        iNameIdx++;
                                    }
                                    // Check existing one control name at least
                                    if(iNameIdx)
                                    {
                                        ////// Add control management element
                                        if(!HeadCtrlMgmt)
                                            HeadCtrlMgmt=new CCtrlMgmtCell(NULL,
                                                                           NULL,
                                                                           (ImHpCtrlMgmt*)pRefJavaCtrlInst,
                                                                           NULL,
                                                                           CtrlClass,
                                                                           CtrlConstMthd,
                                                                           PLATFORM_ID_JAVA,
                                                                           NULL);
                                        else
                                        {   CCtrlMgmtCell* LastCell=HeadCtrlMgmt;
                                            while(LastCell->NextCell != NULL)
                                                LastCell=LastCell->NextCell;
                                            LastCell->NextCell=new CCtrlMgmtCell(NULL,
                                                                                 NULL,
                                                                                 (ImHpCtrlMgmt*)pRefJavaCtrlInst,
                                                                                 NULL,
                                                                                 CtrlClass,
                                                                                 CtrlConstMthd,
                                                                                 PLATFORM_ID_JAVA,
                                                                                 NULL);
                                        }
                                        iCtrlMgmtIdx++;
                                    }
                                    else
                                    {
                                        // Error: No control name defined (Add control management Dll error infos)
                                        CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
                                        pCurDisplayCell->FillDllLoadedSucceed(pRefJavaCtrlInst->GetIconList(0),
                                                                              pRefJavaCtrlInst->GetCtrlName(0),
                                                                              CtrlPlatformID[PLATFORM_ID_JAVA],
                                                                              pRefJavaCtrlInst->GetCtrlClass(0),
                                                                              (*itrJarFile).c_str(),
                                                                              pRefJavaCtrlInst->GetDllVersion());
                                        // Add into display list
                                        FillDisplayList(pCurDisplayCell);

                                        // Free library & delete instance
                                        delete pRefJavaCtrlInst;
                                    }
                                    continue; // Next Java JAR file
                                }
                                else
                                {
                                    //
                                    // TODO: Use pRefJavaCtrlInst->GetLastErr() instead below
                                    //

                                    strncpy(lpErr," (InitJavaVM error)",MAX_SHT_SIZE);
                                }
                            }
                            else
                                strncpy(lpErr," (CreateNewInstance error)",MAX_SHT_SIZE);

                            // Free library & delete instance
                            delete pRefJavaCtrlInst;
                        }
                        else
                            strncpy(lpErr," (NewObject failed)",MAX_SHT_SIZE);
                    }
                    else
                        strncpy(lpErr," (GetMethodID<init> failed)",MAX_SHT_SIZE);
                }
                else
                    strncpy(lpErr," (FindClass failed)",MAX_SHT_SIZE);
            }

            // Error: Add JAR load error infos
            CCtrlDisplayCell* pCurDisplayCell=new CCtrlDisplayCell();
            pCurDisplayCell->FillDllLoadedErr(CtrlPlatformID[PLATFORM_ID_JAVA],(*itrJarFile).c_str(),lpErr);

            // Add into display list
            FillDisplayList(pCurDisplayCell);
        }

        lstErrJarFile.clear();
        lstClassName.clear();
        strJavaClassPath.clear();
    }
}

void CCtrlMgmtList::FillListView(HWND hwndLV) const
{   char lpCtrlLV[MAX_KEY_SIZE];
    int iCtrlCnt=0;
    LV_ITEM lvi;
    lvi.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
    lvi.state=lvi.stateMask=0;
    lvi.cchTextMax=MAX_KEY_SIZE;
    lvi.pszText=lpCtrlLV;
    CCtrlDisplayCell* pCurCtrlInfo=HeadCtrlLV;
    while(pCurCtrlInfo!=NULL)
    {   lvi.iItem=iCtrlCnt;
        // Control name
        lvi.iSubItem=0;
        strncpy(lvi.pszText,pCurCtrlInfo->GetName(),MAX_KEY_SIZE);
        lvi.iImage=iCtrlCnt; // Icon is located in same position
        ListView_InsertItem(hwndLV,&lvi);
        // Platform
        lvi.iSubItem=1;
        strncpy(lvi.pszText,pCurCtrlInfo->GetPlatform(),MAX_KEY_SIZE);
        ListView_SetItem(hwndLV,&lvi);
        // Control class
        lvi.iSubItem=2;
        strncpy(lvi.pszText,pCurCtrlInfo->GetClass(),MAX_KEY_SIZE);
        ListView_SetItem(hwndLV,&lvi);
        // DLL
        lvi.iSubItem=3;
        strncpy(lvi.pszText,pCurCtrlInfo->GetDll(),MAX_KEY_SIZE);
        ListView_SetItem(hwndLV,&lvi);
        // Version
        lvi.iSubItem=4;
        strncpy(lvi.pszText,pCurCtrlInfo->GetVersion(),MAX_KEY_SIZE);
        ListView_SetItem(hwndLV,&lvi);
        // Status
        lvi.iSubItem=5;
        strncpy(lvi.pszText,pCurCtrlInfo->GetStatus(),MAX_KEY_SIZE);
        ListView_SetItem(hwndLV,&lvi);
        //
        pCurCtrlInfo=pCurCtrlInfo->NextCell;
        iCtrlCnt++;
    }
}

void CCtrlMgmtList::FillImgList(HIMAGELIST hImgList) const
{   CCtrlDisplayCell* pCurCtrlInfo=HeadCtrlLV;
    while(pCurCtrlInfo!=NULL)
    {   ImageList_AddIcon(hImgList,pCurCtrlInfo->GetIcon());
        pCurCtrlInfo=pCurCtrlInfo->NextCell;
    }
}

void CCtrlMgmtList::FillDisplayList(CCtrlDisplayCell* pNewCell)
{   if(!HeadCtrlLV)
        HeadCtrlLV=pNewCell;
    else
    {   CCtrlDisplayCell* LastCell=HeadCtrlLV;
        while(LastCell->NextCell!=NULL)
            LastCell=LastCell->NextCell;
        LastCell->NextCell=pNewCell;
    }
}

ImHpCtrlMgmt* CCtrlMgmtList::ReadEvent(const char* lpClass,UINT PlatformID)
{   UINT iCtrlMgmtIdx=0;
    CCtrlMgmtCell* CurCtrlCell=HeadCtrlMgmt;
    while(CurCtrlCell!=NULL)
    {
        // Check control platform ID
        if(CurCtrlCell->GetCtrlPlatformID()==PlatformID)
        {
            // Switch control platform
            switch(CurCtrlCell->GetCtrlPlatformID())
            {   case PLATFORM_ID_WIN32: ////// Win32 Platform
                {
                    // Check current control class name
                    if(CurCtrlCell->GetmHpCtrlMgmt()->IsCtrlClass(lpClass))
                        return CurCtrlCell->CreatemHpCtrlMgmt(iCtrlMgmtIdx);

                    break;
                }
                case PLATFORM_ID_DOTNET: ////// DotNET Platform 
                case PLATFORM_ID_WPF: ////// WPF Platform
                {
                    CmHpDotNetCtrl* DotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(CurCtrlCell->GetmHpCtrlMgmt());

                    // Check current control class name
                    if(DotNetCtrl->IsCtrlClass(lpClass))
                    {
                        ImHpCtrlMgmt* pCtrlMgmt=CurCtrlCell->CreatemHpDotNetCtrl(iCtrlMgmtIdx);
                        CmHpDotNetCtrl* NewDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlMgmt);

                        // Initialize DotNET CLR & Load DotNET library
                        if((!NewDotNetCtrl->InitDotNetCLR())||(!NewDotNetCtrl->LoadDotNetLibrary()))
                        {
                            assert(NULL);

                            //
                            // TODO: Display error message
                            // -> Stop mHelp processus & display 'NewDotNetCtrl->GetLastErr()'
                            //

                            NewDotNetCtrl->GetLastErr();

                            delete NewDotNetCtrl;
                            return NULL; // Error
                        }
                        return pCtrlMgmt;
                    }
                    else if(DotNetCtrl->CheckDotNetErr()) // Check DotNET error
                    {
                        assert(NULL);

                        //
                        // TODO: Display error message
                        // -> Stop mHelp processus & display 'CurCtrlCell->GetmHpCtrlMgmt()->GetLastErr()'
                        //

                        DotNetCtrl->GetLastErr();
                        return NULL; // Error
                    }
                    break;
                }
                case PLATFORM_ID_QT: ////// Qt Platform
                {
                    ImHpQtCtrlMgmt* CurQtCtrl=CurCtrlCell->GetmHpQtCtrlMgmt();

                    // Check current control class name
                    if(CurQtCtrl->GetQtCtrlMgmt()->IsCtrlClass(lpClass))
                    {
                        ImHpQtCtrlMgmt* NewQtCtrl=CurCtrlCell->CreatemHpQtCtrl(iCtrlMgmtIdx);

                        // Load Qt library
                        if(!NewQtCtrl->LoadQtLibrary())
                        {
                            assert(NULL);

                            //
                            // TODO: Display error message
                            // -> Stop mHelp processus & display error message
                            //

                            delete NewQtCtrl;
                            return NULL; // Error
                        }
                        return NewQtCtrl->GetQtCtrlMgmt();
                    }
                    break;
                }
            }
        }

        iCtrlMgmtIdx++;
        CurCtrlCell=CurCtrlCell->NextCell;
    }

    // Failed to find associated class control
    return NULL;
}

void CCtrlMgmtList::CaptureEvent(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam,ImHpCtrlMgmt* &pCtrlMgmt,UINT &iPlatformID) const
{   UINT iCtrlMgmtIdx=0;
    CCtrlMgmtCell* CurCtrlCell=HeadCtrlMgmt;
    while(CurCtrlCell!=NULL)
    {
        // Return control Platform (needed if control that emits this message will be found)
        iPlatformID=CurCtrlCell->GetCtrlPlatformID();

        // Switch on control platform
        switch(iPlatformID)
        {   case PLATFORM_ID_WIN32: ////// Win32 Platform
            {
                // Check message to capture on current managed control
                if(CurCtrlCell->GetmHpCtrlMgmt()->CaptureWin32(hwnd,Message,wParam,lParam))
                {
                    pCtrlMgmt=CurCtrlCell->CreatemHpCtrlMgmt(iCtrlMgmtIdx);

                    // The control that emits this message has been found
                    return; // Ok
                }
                break;
            }
            case PLATFORM_ID_DOTNET: ////// DotNET Platform
            case PLATFORM_ID_WPF: ////// WPF Platform
            {
                CmHpDotNetCtrl* CurDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(CurCtrlCell->GetmHpCtrlMgmt());

                // Check message to capture on current managed control
                if(CurDotNetCtrl->CaptureWin32(hwnd,Message,wParam,lParam))
                {
                    // Get DotNET control properties, state, config & ID
                    if((!CurDotNetCtrl->FillDotNetProperties()) ||
                       (!CurDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataState)) ||
                       (!CurDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataCfg)) ||
                       (!CurDotNetCtrl->FillDotNetData(CmHpDotNetCtrl::DataID)))
                    {
                        assert(NULL);

                        //
                        // TODO: Display error message
                        // -> Stop mHelp processus & display 'pCtrlMgmt->GetLastErr()'
                        //

                        CurDotNetCtrl->GetLastErr();
                        return; // Error
                    }

                    // Clear current DotNET instance data (to be ready to catch new event)
                    CurDotNetCtrl->ClearDotNetData();

                    // Create new DotNET control instance 
                    pCtrlMgmt=CurCtrlCell->CreatemHpDotNetCtrl(iCtrlMgmtIdx);
                    CmHpDotNetCtrl* NewDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(pCtrlMgmt);

                    // Initialize DotNET CLR & Load DotNET library
                    if((!NewDotNetCtrl->InitDotNetCLR())||(!NewDotNetCtrl->LoadDotNetLibrary()))
                    {
                        assert(NULL);

                        //
                        // TODO: Display error message
                        // -> Stop mHelp processus & display 'NewDotNetCtrl->GetLastErr()'
                        //

                        NewDotNetCtrl->GetLastErr();

                        delete NewDotNetCtrl;
                        pCtrlMgmt=NULL;
                        return; // Error
                    }

                    // Copy current control data into new one
                    CopyDotNetData(*NewDotNetCtrl,*CurDotNetCtrl);

                    // Fill new DotNET control instance properties, state, config & ID
                    if((!NewDotNetCtrl->UpdateDotNetProperties()) ||
                       (!NewDotNetCtrl->UpdateDotNetData(CmHpDotNetCtrl::DataState)) ||
                       (!NewDotNetCtrl->UpdateDotNetData(CmHpDotNetCtrl::DataCfg)) ||
                       (!NewDotNetCtrl->UpdateDotNetData(CmHpDotNetCtrl::DataID)))
                    {
                        assert(NULL);

                        //
                        // TODO: Display error message
                        // -> Stop mHelp processus & display 'pCtrlMgmt->GetLastErr()'
                        //

                        NewDotNetCtrl->GetLastErr();

                        delete NewDotNetCtrl;
                        pCtrlMgmt=NULL;
                        return; // Error
                    }

                    // The control that emits this message has been found
                    return; // Ok
                }
                else if(CurDotNetCtrl->CheckDotNetErr()) // Check DotNET error
                {
                    assert(NULL);

                    //
                    // TODO: Display error message
                    // -> Stop mHelp processus & display 'CurCtrlCell->GetmHpCtrlMgmt()->GetLastErr()'
                    //

                    CurDotNetCtrl->GetLastErr();
                    return; // Error
                }
                break;
            }
            case PLATFORM_ID_QT: ////// Qt Platform
            {
                ImHpQtCtrlMgmt* CurQtCtrl=CurCtrlCell->GetmHpQtCtrlMgmt();

                // Check message to capture on current managed control
                if(CurQtCtrl->GetQtCtrlMgmt()->CaptureWin32(hwnd, Message, wParam, lParam))
                {
                    // Fill ImHpCtrlMgmt instance with current Qt instance data
                    CurQtCtrl->GetQtCtrlMgmt()->FillQtData();

                    // Clear current Qt instance data
                    CurQtCtrl->ClearQtData();

                    // Create new Qt instance
                    ImHpQtCtrlMgmt* NewQtCtrl=CurCtrlCell->CreatemHpQtCtrl(iCtrlMgmtIdx);

                    // Load Qt library
                    if(!NewQtCtrl->LoadQtLibrary())
                    {
                        assert(NULL);

                        //
                        // TODO: Display error message
                        // -> Stop mHelp processus & display error message
                        //

                        delete NewQtCtrl;
                        return; // Error
                    }

                    // Copy current control data into new one
                    NewQtCtrl->CopyQtData(*CurQtCtrl->GetQtCtrlMgmt());

                    // Assign new ImHpCtrlMgmt instance
                    pCtrlMgmt=NewQtCtrl->GetQtCtrlMgmt();

                    // Update new Qt control instance data
                    pCtrlMgmt->UpdateQtData();

                    return; // Ok
                }
                break;
            }
        }

        iCtrlMgmtIdx++;
        CurCtrlCell=CurCtrlCell->NextCell;
    }
}

CCtrlMgmtCell* CCtrlMgmtList::GetCtrlMgmtCell(UINT iIdx)
{   CCtrlMgmtCell* CurCtrlCell=HeadCtrlMgmt;
    while((CurCtrlCell!=NULL)&&(iIdx!=0))
    {   CurCtrlCell=CurCtrlCell->NextCell;
        iIdx--;
    }
    return CurCtrlCell;
}

void CCtrlMgmtList::EmptyList()
{   CCtrlMgmtCell* CurCtrlCell=HeadCtrlMgmt;
    while(CurCtrlCell!=NULL)
    {   HMODULE hCurModule=CurCtrlCell->GetCtrlDllModule();
        CCtrlMgmtCell* NextCtrlCell=CurCtrlCell->NextCell;
        delete CurCtrlCell;
        if(hCurModule)
            FreeLibrary(hCurModule); // Free Win32 library
        CurCtrlCell=NextCtrlCell;
    }
    HeadCtrlMgmt=NULL;

    CCtrlDisplayCell* CurDisplayCell=HeadCtrlLV;
    while(CurDisplayCell!=NULL)
    {   CCtrlDisplayCell* NextDisplayCell=CurDisplayCell->NextCell;
        delete CurDisplayCell;
        CurDisplayCell=NextDisplayCell;
    }
    HeadCtrlLV=NULL;

    if(hmHpQtModule)
        FreeLibrary(hmHpQtModule); // Free Qt library
    hmHpQtModule=NULL;

    if((jVM!=NULL)&&(iCreatedVM>=0))
    {   jVM->DestroyJavaVM();
        jVM=NULL;
    }
    if(szJavaClassPath!=NULL)
    {   delete [] szJavaClassPath;
        szJavaClassPath=NULL;
    }
}

////// DotNET Platform
BOOL CCtrlMgmtList::AddDotNetCtrlEvents(HWND hParent,HWND hControl)
{   CCtrlMgmtCell* CurCtrlCell=HeadCtrlMgmt;
    while(CurCtrlCell!=NULL)
    {
        // Check DotNET control platform
        if(CurCtrlCell->GetCtrlPlatformID()==PLATFORM_ID_DOTNET)
        {
            CmHpDotNetCtrl* CurDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(CurCtrlCell->GetmHpCtrlMgmt());

            // Add mHelp DotNET event(s) handler
            if(!CurDotNetCtrl->AddDotNetEvents(hParent,hControl))
            {
                assert(NULL);

                //
                // TODO: Add error message (GetLastErr)
                // -> Control handle has been identified as a managed DotNET control but failed
                //    to add event(s)
                //

                return FALSE;
            }
        }
        CurCtrlCell=CurCtrlCell->NextCell;
    }
    return TRUE; // Done
}

////// Qt, WPF & Java Platforms
BOOL CCtrlMgmtList::AddCtrlEvents(HWND hParent)
{   CCtrlMgmtCell* CurCtrlCell=HeadCtrlMgmt;
    while(CurCtrlCell!=NULL)
    {
        // Switch on control platform
        switch(CurCtrlCell->GetCtrlPlatformID())
        {   case PLATFORM_ID_QT: ////// Qt Platform
            {
                // Add mHelp Qt slot(s)
                if(!CurCtrlCell->GetmHpQtCtrlMgmt()->AddQtSlots(hParent))
                {
                    assert(NULL);

                    //
                    // TODO: Add error message
                    // -> A Qt control at least has been found but failed to add slot(s)
                    //

                    return FALSE;
                }
                break;
            }
            case PLATFORM_ID_WPF: ////// WPF Platform
            {
                CmHpDotNetCtrl* CurDotNetCtrl=dynamic_cast<CmHpDotNetCtrl*>(CurCtrlCell->GetmHpCtrlMgmt());

                // Add mHelp DotNET event(s) handler
                if(!CurDotNetCtrl->AddDotNetEvents(hParent,NULL))
                {
                    assert(NULL);

                    //
                    // TODO: Add error message (GetLastErr)
                    // -> Control handle has been identified as a managed DotNET control but failed
                    //    to add event(s)
                    //

                    return FALSE;
                }
                break;
            }
            case PLATFORM_ID_JAVA: ////// Java Platform
            {
                CmHpJavaCtrl* CurJavaCtrl=dynamic_cast<CmHpJavaCtrl*>(CurCtrlCell->GetmHpCtrlMgmt());

                // Add mHelp Java event(s)
                if(!CurJavaCtrl->AddJavaEvents((iCreatedVM>=0)? TRUE:FALSE))
                {
                    assert(NULL);

                    //
                    // TODO: Add error message (GetLastErr)
                    // -> Control handle has been identified as a managed Java control but failed
                    //    to add event(s)
                    //

                    return FALSE;
                }
                break;
            }
        }
        CurCtrlCell=CurCtrlCell->NextCell;
    }
    return TRUE; // Done
}

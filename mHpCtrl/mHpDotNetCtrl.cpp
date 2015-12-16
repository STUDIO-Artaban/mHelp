// Copyright (c) Unknown Corporation.  All rights reserved.
//

//#include <mHpPak.h>

/*

//
// TODO: Below!
//

//////////
IsCtrlClass();
if((Platform==PLATFORM_DOTNET)&&(CheckDotNetErr())) // Always check DotNET error
    GetLastErr(); // Always call this method if CheckDotNetErr == TRUE
                  // -> Be able to set bDotNetCallErr = FALSE
else
    OK...

//////////
if(!Start())
    ErrMsg=GetLastErr();
    if(!ErrMsg)
        if((Platform==PLATFORM_DOTNET)&&(CheckDotNetErr()))
            GetLastErr();
    else if(!*ErrMsg)
        No error message: Display standard error message
    else
        Error message: Display error message
else
    OK...

*/

#include "mHpDotNetCtrl.h"

//////
#define BSTR_TOO_LONG                   "#BSTR2LONG#"

////// Error definitions
#define ERR_FRAMEWORK_NOTINSTALLED      ".NET Framework 3.5 is NOT installed"

#define ERR_SAFEARRAY_CREATE_VECTOR     "SafeArrayCreateVector"
#define ERR_SAFEARRAY_PUT_ELEMENT       "SafeArrayPutElement"
#define ERR_INVOKE_MEMBER_3             "InvokeMember_3"

#define ERR_VARIANT_RES_NOBSTR          "vrtRes.vt!=VT_BSTR"
#define ERR_VARIANT_RES_NOBOOL          "vrtRes.vt!=VT_BOOL"
#define ERR_VARIANT_RES_NOINT           "vrtRes.vt!=VT_INT"
#define ERR_VARIANT_RES_NOUINT          "vrtRes.vt!=VT_UINT"
#define ERR_VARIANT_RES_BSTR2LONG       BSTR_TOO_LONG

#define ERR_INVALID_DATA_TYPE           "mHpCtrlData::ValueType"
#define ERR_DATA_UNDEFINED              "Undefined"
#define ERR_VALUE_COUNT                 "ValCount"

// --------------------------------
// DotNET methods definition
//
#define MHPDC_METHOD_GETLASTERR         "GetLastErr"        // WIN32: GetLastErr
#define MHPDC_METHOD_CLEARDATA          "ClearData"         // WIN32: Not defined

////// Properties
#define MHPDC_METHOD_GETDLLVERSION      "GetDllVersion"     // WIN32: GetDllVersion
#define MHPDC_METHOD_GETCTRLOBJECT      "GetCtrlObject"     // WIN32: GetCtrlClass
#define MHPDC_METHOD_GETCTRLSTYLE       "GetCtrlStyle"      // WIN32: GetStyle
#define MHPDC_METHOD_GETSTATEBOUND      "GetStateBound"     // WIN32: GetStateBound
#define MHPDC_METHOD_GETCAPTEVENT       "GetCaptEvent"      // WIN32: GetCaptMsgID

////// ID
#define MHPDC_METHOD_GETIDCOUNT         "GetIdCount"        // WIN32: GetCtrlID
#define MHPDC_METHOD_GETSTRID           "GetStrId"          // WIN32: GetCtrlID
#define MHPDC_METHOD_SETSTRID           "SetStrId"          // WIN32: GetCtrlID

////// Class/Object
#define MHPDC_METHOD_ISCTRLCLASS        "IsCtrlObject"      // WIN32: IsCtrlClass
#define MHPDC_METHOD_GETCTRLCLASS       "GetCtrlObject"     // WIN32: GetCtrlClass(UINT)
#define MHPDC_METHOD_SETCTRLOBJECT      "SetCtrlObject"     // WIN32: Not defined

////// Name
#define MHPDC_METHOD_GETCTRLNAME        "GetCtrlName"       // WIN32: GetCtrlName

////// Info
#define MHPDC_METHOD_GETCTRLVERS        "GetCtrlVers"       // WIN32: GetCtrlVers
#define MHPDC_METHOD_GETCTRLDESC        "GetCtrlDesc"       // WIN32: GetCtrlDesc
#define MHPDC_METHOD_GETICONLIST        "GetIconList"       // WIN32: GetIconList
#define MHPDC_METHOD_GETICONIDX         "GetIconIdx"        // WIN32: GetIconIdx

////// User Interface (UI)
#define MHPDC_METHOD_CREATEUI           "CreateUI"          // WIN32: CreateUI
#define MHPDC_METHOD_SAVEUI             "SaveUI"            // WIN32: SaveUI
#define MHPDC_METHOD_CLOSEUI            "CloseUI"           // WIN32: Not defined
#define MHPDC_METHOD_MOVEUI             "MoveUI"            // WIN32: Not defined

////// Style
#define MHPDC_METHOD_SETSTYLE           "SetStyle"          // WIN32: SetStyle

////// State
#define MHPDC_METHOD_GETSTATECOUNT      "GetStateCount"     // WIN32: GetStateCount
#define MHPDC_METHOD_GETSTATETYPE       "GetStateType"      // WIN32: GetState
#define MHPDC_METHOD_GETSTATENAME       "GetStateName"      // WIN32: GetState
#define MHPDC_METHOD_GETINTSTATE        "GetIntState"       // WIN32: GetState
#define MHPDC_METHOD_GETSTRSTATE        "GetStrState"       // WIN32: GetState
#define MHPDC_METHOD_SETINTSTATE        "SetIntState"       // WIN32: Not defined
#define MHPDC_METHOD_SETSTRSTATE        "SetStrState"       // WIN32: Not defined
#define MHPDC_METHOD_NEEDTOCHECKSTATE   "NeedToCheckState"  // WIN32: NeedToCheckState
#define MHPDC_METHOD_DISPLAYSTATE       "DisplayState"      // WIN32: DisplayState
#define MHPDC_METHOD_SETSTATEBOUND      "SetStateBound"     // WIN32: Not defined

////// Config
#define MHPDC_METHOD_GETCFGCOUNT        "GetCfgCount"       // WIN32: GetCfgCount
#define MHPDC_METHOD_GETCFGTYPE         "GetCfgType"        // WIN32: GetState
#define MHPDC_METHOD_GETCFGNAME         "GetCfgName"        // WIN32: GetState
#define MHPDC_METHOD_GETINTCFG          "GetIntCfg"         // WIN32: GetConfig
#define MHPDC_METHOD_GETSTRCFG          "GetStrCfg"         // WIN32: GetConfig
#define MHPDC_METHOD_SETINTCFG          "SetIntCfg"         // WIN32: Not defined
#define MHPDC_METHOD_SETSTRCFG          "SetStrCfg"         // WIN32: Not defined

////// HTML tags
#define MHPDC_METHOD_GETHTMLTAGS        "GetHtmlTags"       // WIN32: Not defined

////// Capture processus
#define MHPDC_METHOD_SETCAPTEVENT       "SetCaptEvent"      // WIN32: SetCaptMsgID
#define MHPDC_METHOD_ADDMHPEVENTS       "AddmHpEvents"      // WIN32: Not defined
#define MHPDC_METHOD_CAPTUREEVENT       "CaptureEvent"      // WIN32: CaptureWin32

////// Reproduce processus
#define MHPDC_METHOD_GETCTRLLEFT        "GetCtrlLeft"       // WIN32: GetCtrlPos
#define MHPDC_METHOD_GETCTRLTOP         "GetCtrlTop"        // WIN32: GetCtrlPos
#define MHPDC_METHOD_GETCTRLWIDTH       "GetCtrlWidth"      // WIN32: GetCtrlPos
#define MHPDC_METHOD_GETCTRLHEIGHT      "GetCtrlHeight"     // WIN32: GetCtrlPos
#define MHPDC_METHOD_CHECKSTATE         "CheckState"        // WIN32: CheckState

#define MHPDC_METHOD_NEXTACTION         "NextAction"        // WIN32: NextAction

#define MHPDC_METHOD_START              "Start"             // WIN32: Start
#define MHPDC_METHOD_END                "End"               // WIN32: End

#define MHPDC_METHOD_MOVETOX            "MoveToX"           // WIN32: MoveTo
#define MHPDC_METHOD_MOVETOY            "MoveToY"           // WIN32: MoveTo

#define MHPDC_METHOD_MOVEACT            "MoveAct"           // WIN32: MoveEvent
#define MHPDC_METHOD_WAITUNTIL          "WaitUntil"         // WIN32: WaitUntil
#define MHPDC_METHOD_LEFTPRESSACT       "LeftPressAct"      // WIN32: LeftPressEvent
#define MHPDC_METHOD_LEFTUNPRESSACT     "LeftUnpressAct"    // WIN32: LeftUnpressEvent
#define MHPDC_METHOD_RIGHTPRESSACT      "RightPressAct"     // WIN32: RightPressEvent
#define MHPDC_METHOD_RIGHTUNPRESSACT    "RightUnpressAct"   // WIN32: RightUnpressEvent
#define MHPDC_METHOD_EDITACT            "EditAct"           // WIN32: EditEvent

//////
void ConvertAsciiToBSTR(BSTR* strDest,const char* lpAscii)
{   if(lpAscii!= NULL)
    {   ULONG len=(ULONG)strlen(lpAscii);
        int ResLen=MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,lpAscii,len,NULL,0);
        *strDest=SysAllocStringLen(NULL,ResLen);
        MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,lpAscii,len,*strDest,ResLen);
    }
}

void ConvertBstrToASCII(char* lpDest,UINT nDestSize,BSTR OleStr)
{   if(OleStr!=NULL)
    {   unsigned long len=WideCharToMultiByte(CP_ACP,0,OleStr,SysStringLen(OleStr),NULL,0,NULL,NULL)+1;
        char* ASCIIstr=new char[len];
        len=WideCharToMultiByte(CP_ACP,0,OleStr,SysStringLen(OleStr),reinterpret_cast<char*>(ASCIIstr),len,NULL,NULL);
        ASCIIstr[len]=0;
        if(len<nDestSize)
            strncpy(lpDest,ASCIIstr,len+1);
        else
            strncpy(lpDest,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG));
        delete [] ASCIIstr;
    }
    else
        *lpDest=0;
}

void ConvertBstrToASCII(char* &lpDest,BSTR OleStr)
{   if(OleStr!=NULL)
    {   unsigned long len=WideCharToMultiByte(CP_ACP,0,OleStr,SysStringLen(OleStr),NULL,0,NULL,NULL)+1;
        lpDest=new char[len];
        len=WideCharToMultiByte(CP_ACP,0,OleStr,SysStringLen(OleStr),reinterpret_cast<char*>(lpDest),len,NULL,NULL);
        lpDest[len]=0;
    }
    else
    {   if(lpDest)
            delete [] lpDest;
        lpDest=NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// ImHpCtrlMgmt class implemetation
CmHpDotNetCtrl::CmHpDotNetCtrl(UINT nIdx,const char* lpDllPath) : ImHpCtrlMgmt(nIdx,CData::StrArray),
                pCorRuntimeHost(NULL), spType(NULL), bDotNetCallErr(FALSE), szDotNetErr(NULL),
                iStateCount(0), iCfgCount(0)
{   strncpy(szDllPath,lpDllPath,MAX_DLL_PATH);
    VariantClear(&vtObjInst);
    *szDotNetName=0;
}
CmHpDotNetCtrl::~CmHpDotNetCtrl()
{
    // DotNET CLR
    if(pCorRuntimeHost)
    {   //pCorRuntimeHost->Stop();
        // ...to be able to reinitialize it
        pCorRuntimeHost->Release();
    }
    if(spType)
        spType->Release();

    // DotNET error message
    if(szDotNetErr)
        delete [] szDotNetErr;

    // State
    if(pState)
    {   assert(iStateCount!=0);
        for(UINT i=0; i<iStateCount; i++)
            delete pState[i];
        delete [] pState;
    }

    // Config
    if(pCfg)
    {   assert(iCfgCount!=0);
        for(UINT i=0; i<iCfgCount; i++)
            delete pCfg[i];
        delete [] pCfg;
    }
}

//////
void CmHpDotNetCtrl::FillDotNetErr(const char* lpFunct,long iLine,const char* lpErrMsg) const
{
    #define ERR_MODULE_INFO     "MODULE: CmHpDotNetCtrl"
    #define ERR_FUNCT_LABEL     "FUNC: "
    #define ERR_LINE_LABEL      "LINE: "
    #define ERR_ERROR_LABEL     "ERROR: "

    // Check previous error message
    if(szDotNetErr)
        delete [] szDotNetErr;

    // Check error message to be diaplyed into mHelp error message
    if(lpFunct)
    {
        // Check error message size
        char lpLine[64];
        _snprintf(lpLine,64,"%d",iLine);
        UINT iErrSize=(UINT)(strlen(lpLine) + 
                             strlen(lpFunct) +
                             sizeof(ERR_MODULE_INFO) +
                             sizeof(ERR_FUNCT_LABEL) +
                             sizeof(ERR_LINE_LABEL));

        if(lpErrMsg)
        {   iErrSize+=(UINT)(strlen(lpErrMsg) + sizeof(ERR_ERROR_LABEL));
            iErrSize+=6; // 6 == 3 * "\n"
        }
        else
            iErrSize+=4; // 4 == 2 * "\n"

        //
        szDotNetErr=new char[iErrSize];

        if(lpErrMsg)
            _snprintf(szDotNetErr,iErrSize,
                    "%s\n%s%s\n%s%s\n%s%s",
                    ERR_MODULE_INFO,
                    ERR_FUNCT_LABEL,lpFunct,
                    ERR_LINE_LABEL,lpLine,
                    ERR_ERROR_LABEL,lpErrMsg);
        else
            _snprintf(szDotNetErr,iErrSize,
                    "%s\n%s%s\n%s%s",
                    ERR_MODULE_INFO,
                    ERR_FUNCT_LABEL,lpFunct,
                    ERR_LINE_LABEL,lpLine);
    }
    else // Error message to be displayed into managed control listview
    {
        // Check error message size
        UINT iErrSize=(UINT)(strlen(lpErrMsg)+1);

        //
        szDotNetErr=new char[iErrSize];
        strncpy(szDotNetErr,lpErrMsg,iErrSize);
    }

    bDotNetCallErr=TRUE;
}

//////
const char* CmHpDotNetCtrl::GetLastErr() const
{
    // Check existing call error
    if(bDotNetCallErr)
    {   bDotNetCallErr=FALSE;
        return szDotNetErr;
    }

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NULL;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETLASTERR);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NULL;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: return NULL; // Empty string
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            return NULL;
        }
    }

    //
    if(szErrorMsg)
    {   delete [] szErrorMsg;
        szErrorMsg=NULL;
    }
    ConvertBstrToASCII(szErrorMsg,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!szErrorMsg)
    {   szErrorMsg=new char[1];
        *szErrorMsg=0;
    }

    // Return mHpCtrlMgmt last error message
    return szErrorMsg;
}

// --------------------------------
// DotNET tools
//
BOOL CmHpDotNetCtrl::InitDotNetCLR()
{
    ////// Check DotNET framework v3.5 installed
    HKEY DotNetKey=NULL;
    DWORD lastError=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v3.5",0,KEY_QUERY_VALUE,&DotNetKey);
    if(lastError!=ERROR_SUCCESS)
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,ERR_FRAMEWORK_NOTINSTALLED);
        return FALSE;
    }

    DWORD keyType;
    BYTE pbData[4];
    DWORD ValLen=sizeof(pbData);
    lastError=RegQueryValueEx(DotNetKey,"Install",NULL,&keyType,pbData,&ValLen);
    if(lastError!=ERROR_SUCCESS)
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,ERR_FRAMEWORK_NOTINSTALLED);
        RegCloseKey(DotNetKey);
        return FALSE;
    }
    RegCloseKey(DotNetKey);

    // Check Install key value (== 1)
    if(pbData[0]!=1)
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,ERR_FRAMEWORK_NOTINSTALLED);
        return FALSE;
    }

    ////// Load v3.5 Common Langage Runtime (CLR version 2.0 -> v2.0.50727)

    // 
    // Load and start the .NET runtime.
    // 

    ICLRMetaHost *pMetaHost=NULL;
    HRESULT hr=CLRCreateInstance(CLSID_CLRMetaHost,IID_PPV_ARGS(&pMetaHost));
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"CLRCreateInstance failed");
        return FALSE;
    }

    // Get the ICLRRuntimeInfo corresponding to a particular CLR version. It 
    // supersedes CorBindToRuntimeEx with STARTUP_LOADER_SAFEMODE.
    LPWSTR lpwVersion=L"v2.0.50727";
    ICLRRuntimeInfo *pRuntimeInfo=NULL;

    hr=pMetaHost->GetRuntime(lpwVersion,IID_PPV_ARGS(&pRuntimeInfo));
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"ICLRMetaHost::GetRuntime failed");
        goto InitDotNetErr;
    }

    // Check if the specified runtime can be loaded into the process. This 
    // method will take into account other runtimes that may already be 
    // loaded into the process and set pbLoadable to TRUE if this runtime can 
    // be loaded in an in-process side-by-side fashion. 
    BOOL fLoadable;
    hr=pRuntimeInfo->IsLoadable(&fLoadable);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"ICLRRuntimeInfo::IsLoadable failed");
        goto InitDotNetErr;
    }
    if(!fLoadable)
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,".NET runtime 2.0 cannot be loaded");
        goto InitDotNetErr;
    }

    // Load the CLR into the current process and return a runtime interface 
    // pointer. ICorRuntimeHost and ICLRRuntimeHost are the two CLR hosting  
    // interfaces supported by CLR 4.0. Here we demo the ICorRuntimeHost 
    // interface that was provided in .NET v1.x, and is compatible with all 
    // .NET Frameworks. 
    hr=pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_PPV_ARGS(&pCorRuntimeHost));
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"ICLRRuntimeInfo::GetInterface failed");
        goto InitDotNetErr;
    }

    // Start the CLR.
    hr=pCorRuntimeHost->Start();
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"CLR failed to start");
        goto InitDotNetErr;
    }
    return TRUE;

//////
InitDotNetErr:

    if(pMetaHost)
    {   pMetaHost->Release();
        pMetaHost=NULL;
    }
    if(pRuntimeInfo)
    {   pRuntimeInfo->Release();
        pRuntimeInfo=NULL;
    }
    if(pCorRuntimeHost)
    {   pCorRuntimeHost->Release();
        pCorRuntimeHost=NULL;
    }
    return FALSE;
}
BOOL CmHpDotNetCtrl::LoadDotNetLibrary()
{
    // 
    // Load the NET assembly. Call the static method GetStringLength of the 
    // class CSSimpleObject. Instantiate the class CSSimpleObject and call 
    // its instance method ToString.
    // 

    // The following C++ code does the same thing as this C# code:
    // 
    //   Assembly assembly = AppDomain.CurrentDomain.Load(pszAssemblyName);
    //   object length = type.InvokeMember("GetStringLength", 
    //       BindingFlags.InvokeMethod | BindingFlags.Static | 
    //       BindingFlags.Public, null, null, new object[] { "HelloWorld" });
    //   object obj = assembly.CreateInstance("CSClassLibrary.CSSimpleObject");
    //   object str = type.InvokeMember("ToString", 
    //       BindingFlags.InvokeMethod | BindingFlags.Instance | 
    //       BindingFlags.Public, null, obj, new object[] { });

    IUnknownPtr spAppDomainThunk=NULL;
    _AppDomainPtr spDefaultAppDomain=NULL;
    _AssemblyPtr spAssembly=NULL;

    // Get DLL assembly name (DllName.dll -> DllName)
    int iNameIdx=0;
    for(int i=0; szDllPath[i]!=0; i++)
        if(szDllPath[i]=='\\')
            iNameIdx=i+1;
    int iDllIdx=0;
    char lpDllName[64];
    while((iDllIdx<64)&&(szDllPath[iNameIdx]!='.'))
        lpDllName[iDllIdx++]=szDllPath[iNameIdx++];
    assert(iDllIdx<64);
    if(iDllIdx<64)
        lpDllName[iDllIdx]=0;

    // The .NET assembly to load.
    bstr_t bstrAssemblyName(lpDllName);

    // Get name space & class from DLL name (== DllName.CDllName)
    char lpmHpClass[128]={0};
    _snprintf(lpmHpClass,128,"%s.C%s",lpDllName,lpDllName); // 'lpDllName'.C'lpDllName'

    // The .NET class to instantiate.
    bstr_t bstrClassName(lpmHpClass);

    // Get a pointer to the default AppDomain in the CLR.
    HRESULT hr=pCorRuntimeHost->GetDefaultDomain(&spAppDomainThunk);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"ICorRuntimeHost::GetDefaultDomain failed");
        goto LoadDotNetErr;
    }

    hr=spAppDomainThunk->QueryInterface(IID_PPV_ARGS(&spDefaultAppDomain));
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"Failed to get default AppDomain");
        goto LoadDotNetErr;
    }

    ////// Load the .NET assembly.

    hr=spDefaultAppDomain->Load_2(bstrAssemblyName, &spAssembly);
    if(FAILED(hr))
    {   //assert(NULL);
        FillDotNetErr(NULL,__LINE__,"Failed to load the assembly");
        goto LoadDotNetErr;
    }

    // Get the Type of CmHpDotNetCtrl.
    hr=spAssembly->GetType_2(bstrClassName, &spType);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"Failed to get the Type interface");
        goto LoadDotNetErr;
    }

    // Instantiate the class.
    hr=spAssembly->CreateInstance(bstrClassName, &vtObjInst);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(NULL,__LINE__,"Assembly::CreateInstance failed");
        goto LoadDotNetErr;
    }

    // Set DotNET DLL version (GetDllVersion implementation)
    if(!SetDllVersion())
        return FALSE;

    return TRUE;

//////
LoadDotNetErr:

    if(spDefaultAppDomain)
        spDefaultAppDomain->Release();
    if(spAppDomainThunk)
        spAppDomainThunk->Release();
    if(pCorRuntimeHost)
    {   //pCorRuntimeHost->Stop();
        // ...to be able to reinitialize it
        pCorRuntimeHost->Release();
        pCorRuntimeHost = NULL;
    }
    return FALSE;
}
BOOL CmHpDotNetCtrl::CheckDotNetErr() const { return bDotNetCallErr; }
BOOL CmHpDotNetCtrl::FillDotNetData(DotNetDataType dataType,BOOL bData)
{
    // Check which data to fill
    switch(dataType)
    {   case DataID: ////// Fill 'pID' from DotNET 'CtrlId'
        {
            // Delete previous ID (if any)
            if(pID)
            {   delete pID;
                pID=NULL;
            }

            // Define 'pID'
            pID=new CCtrlID(CTRL_ID_NAME,CData::StrArray);

            // Fill 'pID'
            if(bData)
                if(!FillData(0,CData::StrArray,DataID))
                    return FALSE;

            break;
        }
        case DataState: ////// Fill 'pState' from DotNET 'CtrlState'
        {
            // Delete previous state (if any)
            if(pState)
            {   assert(iStateCount!=0);
                for(UINT i=0; i<iStateCount; i++)
                    delete pState[i];
                delete [] pState;
                pState=NULL;
            }

            // Get state count
            iStateCount=GetStateCount();
            if(bDotNetCallErr)
                return FALSE;

            // Check state count
            if(!iStateCount)
                return TRUE; // No state

            // Define 'pState'
            pState=new CCtrlState*[iStateCount];
            pState[0]=NULL;

            // Loop on state count
            for(UINT iDataIdx=0; iDataIdx<iStateCount; iDataIdx++)
            {
                // Get state type
                int iValType=GetDataType(iDataIdx,MHPDC_METHOD_GETSTATETYPE);
                if(iValType==NO_VALUE)
                    return FALSE;

                // Get state name
                char lpStateName[DATA_NAME_SIZE];
                GetDataName(lpStateName,iDataIdx,MHPDC_METHOD_GETSTATENAME);
                if(!*lpStateName)
                    return FALSE;

                // Define 'pState'
                pState[iDataIdx]=new CCtrlState(lpStateName,(CData::Type)iValType);

                // Check if need to fill 'pState' data
                if(bData)
                    if(!FillData(iDataIdx,(CData::Type)iValType,DataState))
                        return FALSE;
            }
            break;
        }
        case DataCfg: ////// Fill 'pCfg' from DotNET 'CtrlCfg'
        {
            // Delete previous config (if any)
            if(pCfg)
            {   assert(iCfgCount!=0);
                for(UINT i=0; i<iCfgCount; i++)
                    delete pCfg[i];
                delete [] pCfg;
                pCfg=NULL;
            }

            // Get config count
            iCfgCount=GetCfgCount();
            if(bDotNetCallErr)
                return FALSE;

            // Check config count
            if(!iCfgCount)
                return TRUE; // No config

            // Define 'pCfg'
            pCfg=new CCtrlCfg*[iCfgCount];
            pCfg[0]=NULL;

            // Loop on config count
            for(UINT iDataIdx=0; iDataIdx<iCfgCount; iDataIdx++)
            {
                // Get config type
                int iValType=GetDataType(iDataIdx,MHPDC_METHOD_GETCFGTYPE);
                if(iValType==NO_VALUE)
                    return FALSE;

                // Get config name
                char lpCfgName[DATA_NAME_SIZE];
                GetDataName(lpCfgName,iDataIdx,MHPDC_METHOD_GETCFGNAME);
                if(!*lpCfgName)
                    return FALSE;

                // Define 'pCfg'
                pCfg[iDataIdx]=new CCtrlCfg(lpCfgName,(CData::Type)iValType);

                // Check if need to fill 'pCfg' data
                if(bData)
                    if(!FillData(iDataIdx,(CData::Type)iValType,DataCfg))
                        return FALSE;
            }
            break;
        }
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::UpdateDotNetData(DotNetDataType dataType) const
{
    // Check which data to fill
    switch(dataType)
    {   case DataID: ////// Update DotNET 'CtrlId' from 'pID'
        {
            // Check ID definition
            if(!pID)
            {   assert(NULL);
                return FALSE;
            }

            // Update DotNET 'CtrlId'
            if(!UpdateData(0,CData::StrArray,DataID))
                return FALSE;

            break;
        }
        case DataState: ////// Update DotNET 'CtrlState' from 'pState'
        {
            // Check existing state
            if(!iStateCount)
                return TRUE; // No state

            // Loop on state count
            for(UINT iDataIdx=0; iDataIdx<iStateCount; iDataIdx++)
            {
                // Update DotNET 'CtrlState'
                if(!UpdateData(iDataIdx,pState[iDataIdx]->GetType(),DataState))
                    return FALSE;
            }
            break;
        }
        case DataCfg: ////// Update DotNET 'CtrlCfg' from 'pCfg'
        {
            // Check existing config
            if(!iCfgCount)
                return TRUE; // No config

            // Loop on config count
            for(UINT iDataIdx=0; iDataIdx<iCfgCount; iDataIdx++)
            {
                // Update DotNET 'CtrlCfg'
                if(!UpdateData(iDataIdx,pCfg[iDataIdx]->GetType(),DataCfg))
                    return FALSE;
            }
            break;
        }
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::AddDotNetEvents(HWND hForm,HWND hCtrl) const
{
    // Create & Assign parameters
    VARIANT vrtForm;
    VariantInit(&vrtForm);
    vrtForm.vt=VT_INT;

    vrtForm.intVal=(int)hForm; ////// hForm

    VARIANT vrtCtrl;
    VariantInit(&vrtCtrl);
    vrtCtrl.vt=VT_INT;

    vrtCtrl.intVal=(int)hCtrl; ////// hCtrl

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,2);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtForm)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx=1;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtCtrl)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_ADDMHPEVENTS);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::FillDotNetProperties()
{
    // Get DotNET control style (GetStyle)
    if(!GetCtrlStyle())
        return FALSE;

    // Get DotNET control object name (GetCtrlClass)
    if(!GetCtrlObject())
        return FALSE;

    // Get DotNET control state bound (GetStateBound)
    if(!GetStateBound())
        return FALSE;

    // Get DotNET capture event identifier (GetCaptMsgID)
    if(!GetCaptEvent())
        return FALSE;

    return TRUE;
}
BOOL CmHpDotNetCtrl::UpdateDotNetProperties()
{
    // Set DotNET control style (CtrlStyle)
    if(!SetStyle(iCtrlStyleID))
        return FALSE;

    // Set DotNET control object name (CtrlObjName)
    if(!UpdateCtrlObject()) // With 'szClass'
        return FALSE;

    // Set DotNET control state bound (StateBound)
    if(!UpdateStateBound()) // With 'szStateBound'
        return FALSE;

    // Set DotNET capture event identifier (CtrlEventId)
    if(!SetCaptMsgID(iCaptMsgID))
        return FALSE;

    return TRUE;
}
void CmHpDotNetCtrl::ClearDotNetData() const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_CLEARDATA);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
    }
}
void CopyDotNetData(CmHpDotNetCtrl& DestData,const CmHpDotNetCtrl& CopyData)
{
    // Copy CmHpDotNetCtrl data
    DestData.iStateCount=CopyData.iStateCount;
    DestData.iCfgCount=CopyData.iCfgCount;

    // Copy ImHpCtrlMgmt data
    CopymHpCtrlMgmt(DestData,CopyData,CopyData.iStateCount,CopyData.iCfgCount);
}

// --------------------------------
// Properties
//

BOOL CmHpDotNetCtrl::FillData(UINT iDataIdx,CData::Type valType,DotNetDataType dataType)
{
    // Check which data to fill
    switch(dataType)
    {   case DataID: ////// Fill 'pID'
        {
            assert(!iDataIdx);
            assert(valType==CData::StrArray);

            // Get ID value count
            int iValCount=GetIdCount();
            if(iValCount==NO_VALUE)
                return FALSE;

            // Get ID string value(s)
            if(!GetIdStrVal(iValCount))
                return FALSE;

            break;
        }
        case DataState: ////// Fill 'pState'
        {
            assert(iDataIdx<iStateCount);

            // Get state value count defined
            int iValCount=GetDataCount(iDataIdx,MHPDC_METHOD_GETSTATECOUNT);
            if(iValCount==NO_VALUE)
                return FALSE;

            // Get state value according state value type
            switch(valType)
            {
                case CData::Integer: // Integer
                case CData::IntArray: // Integer array
                {
                    // Get Integer value(s)
                    if(!GetDataIntVal(iDataIdx,pState[iDataIdx],valType,iValCount,MHPDC_METHOD_GETINTSTATE))
                        return FALSE;
                    break;
                }
                case CData::String: // String
                case CData::StrArray: // String array
                {
                    // Get String value(s)
                    if(!GetDataStrVal(iDataIdx,pState[iDataIdx],valType,iValCount,MHPDC_METHOD_GETSTRSTATE))
                        return FALSE;
                    break;
                }
                default:
                {   assert(NULL);
                    FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVALID_DATA_TYPE);
                    return FALSE;
                }
            }
            break;
        }
        case DataCfg: ////// Fill 'pCfg'
        {
            assert(iDataIdx<iCfgCount);

            // Get config value count
            int iValCount=GetDataCount(iDataIdx,MHPDC_METHOD_GETCFGCOUNT);
            if(iValCount==NO_VALUE)
                return FALSE;

            // Get config value according config value type
            switch(valType)
            {
                case CData::Integer: // Integer
                case CData::IntArray: // Integer array
                {
                    // Get Integer value(s)
                    if(!GetDataIntVal(iDataIdx,pCfg[iDataIdx],valType,iValCount,MHPDC_METHOD_GETINTCFG))
                        return FALSE;
                    break;
                }
                case CData::String: // String
                case CData::StrArray: // String array
                {
                    // Get String value(s)
                    if(!GetDataStrVal(iDataIdx,pCfg[iDataIdx],valType,iValCount,MHPDC_METHOD_GETSTRCFG))
                        return FALSE;
                    break;
                }
                default:
                {   assert(NULL);
                    FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVALID_DATA_TYPE);
                    return FALSE;
                }
            }
            break;
        }
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::UpdateData(UINT iDataIdx,CData::Type valType,DotNetDataType dataType) const
{
    // Check which data to update
    switch(dataType)
    {   case DataID: ////// Update DotNET 'pID'
        {
            // Set ID string value(s)
            for(UINT iArrayIdx=0; iArrayIdx<pID->GetCount(); iArrayIdx++)
            {
                const char* lpValue=(const char*)pID->GetValue(iArrayIdx);

                // Check consistent ID type
                if(!SetIdStrVal(iArrayIdx,lpValue))
                    return FALSE;
            }
            break;
        }
        case DataState: ////// Update DotNET 'pState'
        {
            // Set state value according state value type
            switch(pState[iDataIdx]->GetType())
            {
                case CData::Integer: // Integer
                case CData::IntArray: // Integer array
                {
                    // Set Integer value(s)
                    // -> 'pState[0]->GetCount' == 1 for CData::Type::Integer
                    for(UINT iArrayIdx=0; iArrayIdx<pState[iDataIdx]->GetCount(); iArrayIdx++)
                    {
                        const int* iValue=(const int*)pState[iDataIdx]->GetValue(iArrayIdx);

                        // Check consistent state type
                        if(!SetDataIntVal(iDataIdx,iArrayIdx,*iValue,MHPDC_METHOD_SETINTSTATE))
                            return FALSE;
                    }
                    break;
                }
                case CData::String: // String
                case CData::StrArray: // String array
                {
                    // Set String value(s)
                    // -> 'pState[0]->GetCount' == 1 for CData::Type::String
                    for(UINT iArrayIdx=0; iArrayIdx<pState[iDataIdx]->GetCount(); iArrayIdx++)
                    {
                        const char* lpValue=(const char*)pState[iDataIdx]->GetValue(iArrayIdx);

                        // Check consistent state type
                        if(!SetDataStrVal(iDataIdx,iArrayIdx,lpValue,MHPDC_METHOD_SETSTRSTATE))
                            return FALSE;
                    }
                    break;
                }
            }
            break;
        }
        case DataCfg: ////// Update DotNET 'pCfg'
        {
            // Set config value according config value type
            switch(pCfg[iDataIdx]->GetType())
            {
                case CData::Integer: // Integer
                case CData::IntArray: // Integer array
                {
                    // Set Integer value(s)
                    // -> 'pCfg[0]->GetCount' == 1 for CData::Type::Integer
                    for(UINT iArrayIdx=0; iArrayIdx<pCfg[iDataIdx]->GetCount(); iArrayIdx++)
                    {
                        const int* iValue=(const int*)pCfg[iDataIdx]->GetValue(iArrayIdx);

                        // Check consistent config type
                        if(!SetDataIntVal(iDataIdx,iArrayIdx,*iValue,MHPDC_METHOD_SETINTCFG))
                            return FALSE;
                    }
                    break;
                }
                case CData::String: // String
                case CData::StrArray: // String array
                {
                    // Set String value(s)
                    // -> 'pCfg[0]->GetCount' == 1 for CData::Type::String
                    for(UINT iArrayIdx=0; iArrayIdx<pCfg[iDataIdx]->GetCount(); iArrayIdx++)
                    {
                        const char* lpValue=(const char*)pCfg[iDataIdx]->GetValue(iArrayIdx);

                        // Check consistent config type
                        if(!SetDataStrVal(iDataIdx,iArrayIdx,lpValue,MHPDC_METHOD_SETSTRCFG))
                            return FALSE;
                    }
                    break;
                }
            }
            break;
        }
    }
    return TRUE;
}

BOOL CmHpDotNetCtrl::SetDllVersion()
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETDLLVERSION);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: // Empty string
        {   *szDllVersion=0;
            break;
        }
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            return FALSE;
        }
    }

    //
    ConvertBstrToASCII(szDllVersion,DLL_VERSION_SIZE,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(szDllVersion,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        return FALSE;
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::GetCtrlObject()
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETCTRLOBJECT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: // Empty string
        {   *szClass=0;
            break;
        }
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            return FALSE;
        }
    }

    //
    ConvertBstrToASCII(szClass,CTRL_CLASS_SIZE,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(szClass,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        return FALSE;
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::GetCtrlStyle()
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETCTRLSTYLE);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        return FALSE;
    }

    //
    iCtrlStyleID=vrtRes.intVal;
    return TRUE;
}
BOOL CmHpDotNetCtrl::GetStateBound()
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETSTATEBOUND);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY:
        {   *szStateBound=0;
            return TRUE; // Empty string
        }
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            return NULL;
        }
    }

    //
    ConvertBstrToASCII(szStateBound,STATE_BOUND_SIZE,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(szStateBound,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        return FALSE;
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::GetCaptEvent()
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETCAPTEVENT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        return FALSE;
    }

    //
    iCaptMsgID=vrtRes.intVal;
    return TRUE;
}
BOOL CmHpDotNetCtrl::UpdateCtrlObject() const
{
    // Create & Assign parameters
    VARIANT vrtStrObj;
    VariantInit(&vrtStrObj);
    vrtStrObj.vt=VT_BSTR;

    ConvertAsciiToBSTR(&vrtStrObj.bstrVal,szClass); ////// szClass

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        SysFreeString(vrtStrObj.bstrVal);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtStrObj)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SysFreeString(vrtStrObj.bstrVal);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_SETCTRLOBJECT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SysFreeString(vrtStrObj.bstrVal);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::UpdateStateBound() const
{
    // Create & Assign parameters
    VARIANT vrtStrObj;
    VariantInit(&vrtStrObj);
    vrtStrObj.vt=VT_BSTR;

    ConvertAsciiToBSTR(&vrtStrObj.bstrVal,szStateBound); ////// szStateBound

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        SysFreeString(vrtStrObj.bstrVal);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtStrObj)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SysFreeString(vrtStrObj.bstrVal);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_SETSTATEBOUND);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SysFreeString(vrtStrObj.bstrVal);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    return TRUE;
}

////// Class name
BOOL CmHpDotNetCtrl::IsCtrlClass(const char* lpClass) const
{
    // Create & Assign parameters
    VARIANT vrtStrClass;
    VariantInit(&vrtStrClass);
    vrtStrClass.vt=VT_BSTR;

    ConvertAsciiToBSTR(&vrtStrClass.bstrVal,lpClass); ////// lpClass

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        SysFreeString(vrtStrClass.bstrVal);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtStrClass)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SysFreeString(vrtStrClass.bstrVal);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_ISCTRLCLASS);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SysFreeString(vrtStrClass.bstrVal);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
const char* CmHpDotNetCtrl::GetCtrlClass(UINT iClassIdx) const
{
    // Create & Assign parameters
    VARIANT vrtClassIdx;
    VariantInit(&vrtClassIdx);
    vrtClassIdx.vt=VT_UINT;

    vrtClassIdx.uintVal=iClassIdx; ////// iClassIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NULL;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtClassIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return NULL;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETCTRLCLASS);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NULL;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: return NULL; // Empty string
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            return NULL;
        }
    }

    //
    ConvertBstrToASCII(szClass,CTRL_CLASS_SIZE,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(szClass,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        return NULL;
    }

    // Return indexed DotNET control object name
    if(*szClass)
        return szClass;
    return NULL;
}

////// Name
const char* CmHpDotNetCtrl::GetCtrlName(int iNameIdx) const
{
    // Create & Assign parameters
    VARIANT vrtNameIdx;
    VariantInit(&vrtNameIdx);
    vrtNameIdx.vt=VT_INT;

    vrtNameIdx.intVal=iNameIdx; ////// iNameIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NULL;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtNameIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return NULL;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETCTRLNAME);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NULL;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: return NULL; // Empty string
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            return NULL;
        }
    }

    //
    ConvertBstrToASCII(szDotNetName,DATA_NAME_SIZE,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(szDotNetName,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        return NULL;
    }

    // Return indexed DotNET control name
    if(*szDotNetName)
        return szDotNetName;
    return NULL;
}

////// Info
void CmHpDotNetCtrl::GetCtrlVers(char* lpVers,UINT iMaxSize) const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        *lpVers=0;
        return;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETCTRLVERS);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        *lpVers=0;
        return;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: // Empty string
        {   *lpVers=0;
            break;
        }
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            *lpVers=0;
            return;
        }
    }

    //
    ConvertBstrToASCII(lpVers,iMaxSize,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(lpVers,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        *lpVers=0;
    }
}
void CmHpDotNetCtrl::GetCtrlDesc(char* lpDesc,UINT iMaxSize) const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        *lpDesc=0;
        return;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETCTRLDESC);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        *lpDesc=0;
        return;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: // Empty string
        {   *lpDesc=0;
            break;
        }
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            *lpDesc=0;
            return;
        }
    }

    //
    ConvertBstrToASCII(lpDesc,iMaxSize,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(lpDesc,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        *lpDesc=0;
    }
}
HICON CmHpDotNetCtrl::GetIconList(UINT iIconIdx) const
{
    // Create & Assign parameters
    VARIANT vrtIconIdx;
    VariantInit(&vrtIconIdx);
    vrtIconIdx.vt=VT_UINT;

    vrtIconIdx.uintVal=iIconIdx; ////// iIconIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NULL;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtIconIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return NULL;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETICONLIST);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NULL;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        return NULL;
    }

    //
    HICON hIcon=(HICON)vrtRes.intVal;
    if(!hIcon)
        return NULL; // No indexed icon found

    // Check valid icon
    ICONINFO iconInfo;
    if(GetIconInfo(hIcon,&iconInfo))
    {
        // Check expected icon size
        if((iconInfo.xHotspot==16)&&(iconInfo.yHotspot==16))
            return hIcon;

        assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,"IconSize");
        return NULL;
    }

    assert(NULL);
    FillDotNetErr(__FUNCTION__,__LINE__,"IconHandle");
    return NULL;
}
int CmHpDotNetCtrl::GetIconIdx() const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NO_VALUE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETICONIDX);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NO_VALUE;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        return NO_VALUE;
    }
    return vrtRes.intVal;
}

////// User Interface (UI)
HWND CmHpDotNetCtrl::CreateUI(HWND hParentUI,HWND hSaveBtn) const
{
    // Create & Assign parameters
    VARIANT vrtSaveBtn;
    VariantInit(&vrtSaveBtn);
    vrtSaveBtn.vt=VT_INT;

    vrtSaveBtn.intVal=(int)hSaveBtn; ////// hSaveBtn

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NULL;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtSaveBtn)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return NULL;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_CREATEUI);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NULL;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        return NULL;
    }

    //
    return (HWND)vrtRes.intVal; // Can be NULL
}
void CmHpDotNetCtrl::SaveUI(HWND hUI) const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_SAVEUI);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
    }
}

void CmHpDotNetCtrl::CloseDotNetUI() const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_CLOSEUI);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
    }
}
BOOL CmHpDotNetCtrl::MoveDotNetUI(HWND hCfgDlg,HWND hCtrlUI) const
{
    // Create & Assign parameters
    VARIANT vrtCfgUI;
    VariantInit(&vrtCfgUI);
    vrtCfgUI.vt=VT_INT;

    vrtCfgUI.intVal=(int)hCtrlUI; ////// hCtrlUI

    // Get control UI position according config dialog position
    RECT CfgRect;
    GetWindowRect(hCfgDlg,&CfgRect);

    VARIANT vrtxPos;
    VariantInit(&vrtxPos);
    vrtxPos.vt=VT_INT;

    vrtxPos.intVal=(int)CfgRect.left+16; ////// xPos

    VARIANT vrtyPos;
    VariantInit(&vrtyPos);
    vrtyPos.vt=VT_INT;

    vrtyPos.intVal=(int)CfgRect.top+63; ////// yPos

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,3);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtCfgUI)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx++;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtxPos)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx++;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtyPos)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_MOVEUI);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}

////// Style
BOOL CmHpDotNetCtrl::SetStyle(int iStyleID)
{
    // Create & Assign parameters
    VARIANT vrtStyleID;
    VariantInit(&vrtStyleID);
    vrtStyleID.vt=VT_INT;

    vrtStyleID.intVal=iStyleID; ////// iStyleID

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtStyleID)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_SETSTYLE);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}

//////
void CmHpDotNetCtrl::GetDataName(char* lpName,UINT iDataIdx,const char* lpMethod) const
{
    // Create & Assign parameters
    VARIANT vrtDataIdx;
    VariantInit(&vrtDataIdx);
    vrtDataIdx.vt=VT_UINT;

    vrtDataIdx.uintVal=iDataIdx; ////// iDataIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        *lpName=0;
        return;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        *lpName=0;
        return;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(lpMethod);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
        *lpName=0;
        return;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: // Empty string
        {   assert(NULL); // Should be defined
            FillDotNetErr(lpMethod,__LINE__,ERR_DATA_UNDEFINED);
            *lpName=0;
            return;
        }
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOBSTR);
            *lpName=0;
            return;
        }
    }

    //
    ConvertBstrToASCII(lpName,DATA_NAME_SIZE,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(lpName,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        *lpName=0;
    }
}
int CmHpDotNetCtrl::GetDataType(UINT iDataIdx,const char* lpMethod) const
{
    // Create & Assign parameters
    VARIANT vrtDataIdx;
    VariantInit(&vrtDataIdx);
    vrtDataIdx.vt=VT_UINT;

    vrtDataIdx.uintVal=iDataIdx; ////// iDataIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NO_VALUE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return NO_VALUE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(lpMethod);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
        return NO_VALUE;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOINT);
        return NO_VALUE;
    }
    return vrtRes.intVal;
}
int CmHpDotNetCtrl::GetDataCount(UINT iDataIdx,const char* lpMethod) const
{
    // Create & Assign parameters
    VARIANT vrtDataIdx;
    VariantInit(&vrtDataIdx);
    vrtDataIdx.vt=VT_UINT;

    vrtDataIdx.uintVal=iDataIdx; ////// iDataIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NO_VALUE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return NO_VALUE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(lpMethod);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
        return NO_VALUE;
    }
    if((vrtRes.vt!=VT_UI4)&&(vrtRes.vt!=VT_UINT))
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOUINT);
        return NO_VALUE;
    }
    return (int)vrtRes.uintVal;
}
BOOL CmHpDotNetCtrl::GetDataIntVal(UINT iDataIdx,CData* pData,CData::Type dataType,int iValCount,const char* lpMethod) const
{
    // Create & Assign parameters
    VARIANT vrtDataIdx;
    VariantInit(&vrtDataIdx);
    vrtDataIdx.vt=VT_UINT;

    vrtDataIdx.uintVal=iDataIdx; ////// iDataIdx

    VARIANT vrtArrayIdx;
    VariantInit(&vrtArrayIdx);
    vrtArrayIdx.vt=VT_UINT;

    vrtArrayIdx.uintVal=0; ////// iArrayIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,2);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx=1;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtArrayIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    // Get Integer value according value type
    switch(dataType)
    {   case CData::Integer: ////// Integer
        {
            // Check expected value count (==1)
            if(iValCount!=1)
            {   assert(NULL);
                FillDotNetErr(lpMethod,__LINE__,ERR_VALUE_COUNT);
                SafeArrayDestroy(psa);
                return FALSE;
            }

            ////// Invoke DotNET control class method
            variant_t vrtRes;
            bstr_t bstrMethod(lpMethod);
            HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                              static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                              NULL,vtObjInst,psa,&vrtRes);
            SafeArrayDestroy(psa);
            if(FAILED(hr))
            {   assert(NULL);
                FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
                return FALSE;
            }
            if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
            {   assert(NULL);
                FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOINT);
                return FALSE;
            }

            // Assign data
            pData->SetValue(vrtRes.intVal);
            break;
        }
        case CData::IntArray: ////// Integer array
        {
            // Loop on array lenght
            for(UINT iArrayIdx=0; iArrayIdx<((UINT)iValCount); iArrayIdx++)
            {
                // Fill parameter array
                vrtArrayIdx.uintVal=iArrayIdx; ////// iArrayIdx

                // Delete, Create & Fill parameter array
                SafeArrayDestroy(psa);
                psa=SafeArrayCreateVector(VT_VARIANT,0,2);
                if(!psa)
                {   assert(NULL);
                    FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
                    return FALSE;
                }
                iParamIdx=0;
                if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
                {   assert(NULL);
                    FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
                    SafeArrayDestroy(psa);
                    return FALSE;
                }
                iParamIdx=1;
                if(SafeArrayPutElement(psa,&iParamIdx,&vrtArrayIdx)!=S_OK)
                {   assert(NULL);
                    FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
                    SafeArrayDestroy(psa);
                    return FALSE;
                }

                ////// Invoke DotNET control class method
                variant_t vrtRes;
                bstr_t bstrMethod(lpMethod);
                HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                                  static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                                  NULL,vtObjInst,psa,&vrtRes);
                if(FAILED(hr))
                {   assert(NULL);
                    FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
                    SafeArrayDestroy(psa);
                    return FALSE;
                }
                if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
                {   assert(NULL);
                    FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOINT);
                    SafeArrayDestroy(psa);
                    return FALSE;
                }

                // Assign data
                pData->AddValue(vrtRes.intVal);
            }

            SafeArrayDestroy(psa);
            break;
        }
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::GetDataStrVal(UINT iDataIdx,CData* pData,CData::Type dataType,int iValCount,const char* lpMethod) const
{
    // Create & Assign parameters
    VARIANT vrtDataIdx;
    VariantInit(&vrtDataIdx);
    vrtDataIdx.vt=VT_UINT;

    vrtDataIdx.uintVal=iDataIdx; ////// iDataIdx

    VARIANT vrtArrayIdx;
    VariantInit(&vrtArrayIdx);
    vrtArrayIdx.vt=VT_UINT;

    vrtArrayIdx.uintVal=0; ////// iArrayIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,2);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx=1;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtArrayIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    // Get Integer value according value type
    switch(dataType)
    {   case CData::String: ////// String
        {
            // Check expected value count (==1)
            if(iValCount!=1)
            {   assert(NULL);
                FillDotNetErr(lpMethod,__LINE__,ERR_VALUE_COUNT);
                SafeArrayDestroy(psa);
                return FALSE;
            }

            ////// Invoke DotNET control class method
            variant_t vrtRes;
            bstr_t bstrMethod(lpMethod);
            HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                                static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                                NULL,vtObjInst,psa,&vrtRes);
            SafeArrayDestroy(psa);
            if(FAILED(hr))
            {   assert(NULL);
                FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
                return FALSE;
            }
            switch(vrtRes.vt)
            {   case VT_EMPTY: // Empty string
                {   assert(NULL); // Should be defined
                    FillDotNetErr(lpMethod,__LINE__,ERR_DATA_UNDEFINED);
                    return FALSE;
                }
                case VT_BSTR: break; // Existing string
                default:
                {   assert(NULL);
                    FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOBSTR);
                    return FALSE;
                }
            }

            //
            char* lpData=NULL;
            ConvertBstrToASCII(lpData,vrtRes.bstrVal);
            VariantClear(&vrtRes);
            assert(lpData); // should be defined

            // Assign data
            pData->SetValue(lpData);
            break;
        }
        case CData::StrArray: ////// String array
        {
            // Loop on array lenght
            for(UINT iArrayIdx=0; iArrayIdx<((UINT)iValCount); iArrayIdx++)
            {
                // Fill parameter array
                vrtArrayIdx.uintVal=iArrayIdx; ////// iArrayIdx

                // Delete, Create & Fill parameter array
                SafeArrayDestroy(psa);
                psa=SafeArrayCreateVector(VT_VARIANT,0,2);
                if(!psa)
                {   assert(NULL);
                    FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
                    return FALSE;
                }
                iParamIdx=0;
                if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
                {   assert(NULL);
                    FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
                    SafeArrayDestroy(psa);
                    return FALSE;
                }
                iParamIdx=1;
                if(SafeArrayPutElement(psa,&iParamIdx,&vrtArrayIdx)!=S_OK)
                {   assert(NULL);
                    FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
                    SafeArrayDestroy(psa);
                    return FALSE;
                }

                ////// Invoke DotNET control class method
                variant_t vrtRes;
                bstr_t bstrMethod(lpMethod);
                HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                                    static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                                    NULL,vtObjInst,psa,&vrtRes);
                if(FAILED(hr))
                {   assert(NULL);
                    FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
                    SafeArrayDestroy(psa);
                    return FALSE;
                }
                switch(vrtRes.vt)
                {   case VT_EMPTY: // Empty string
                    {   assert(NULL); // Should be defined
                        FillDotNetErr(lpMethod,__LINE__,ERR_DATA_UNDEFINED);
                        SafeArrayDestroy(psa);
                        return FALSE;
                    }
                    case VT_BSTR: break; // Existing string
                    default:
                    {   assert(NULL);
                        FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOBSTR);
                        SafeArrayDestroy(psa);
                        return FALSE;
                    }
                }
                
                //
                char* lpData=NULL;
                ConvertBstrToASCII(lpData,vrtRes.bstrVal);
                VariantClear(&vrtRes);
                assert(lpData); // should be defined

                // Assign data
                pData->AddValue(lpData);
            }

            SafeArrayDestroy(psa);
            break;
        }
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::SetDataIntVal(UINT iDataIdx,UINT iArrayIdx,int iValue,const char* lpMethod) const
{
    // Create & Assign parameters
    VARIANT vrtDataIdx;
    VariantInit(&vrtDataIdx);
    vrtDataIdx.vt=VT_UINT;

    vrtDataIdx.uintVal=iDataIdx; ////// iDataIdx

    VARIANT vrtData;
    VariantInit(&vrtData);
    vrtData.vt=VT_INT;

    vrtData.intVal=iValue; ////// Integer value

    VARIANT vrtArrayIdx;
    VariantInit(&vrtArrayIdx);
    vrtArrayIdx.vt=VT_UINT;

    vrtArrayIdx.uintVal=0; ////// iArrayIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,3);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx=1;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtData)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx=2;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtArrayIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(lpMethod);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::SetDataStrVal(UINT iDataIdx,UINT iArrayIdx,const char* lpValue,const char* lpMethod) const
{
    // Create & Assign parameters
    VARIANT vrtDataIdx;
    VariantInit(&vrtDataIdx);
    vrtDataIdx.vt=VT_UINT;

    vrtDataIdx.uintVal=iDataIdx; ////// iDataIdx

    VARIANT vrtData;
    VariantInit(&vrtData);
    vrtData.vt=VT_BSTR;

    ConvertAsciiToBSTR(&vrtData.bstrVal,lpValue); ////// String value

    VARIANT vrtArrayIdx;
    VariantInit(&vrtArrayIdx);
    vrtArrayIdx.vt=VT_UINT;

    vrtArrayIdx.uintVal=iArrayIdx; ////// iArrayIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,3);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        SysFreeString(vrtData.bstrVal);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtDataIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SysFreeString(vrtData.bstrVal);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx=1;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtData)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SysFreeString(vrtData.bstrVal);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx=2;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtArrayIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SysFreeString(vrtData.bstrVal);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(lpMethod);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SysFreeString(vrtData.bstrVal);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(lpMethod,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
int CmHpDotNetCtrl::GetIdCount() const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NO_VALUE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETIDCOUNT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NO_VALUE;
    }
    if((vrtRes.vt!=VT_UI4)&&(vrtRes.vt!=VT_UINT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOUINT);
        return NO_VALUE;
    }
    return (int)vrtRes.uintVal;
}
BOOL CmHpDotNetCtrl::GetIdStrVal(int iValCount)
{
    // Create & Assign parameters
    VARIANT vrtArrayIdx;
    VariantInit(&vrtArrayIdx);
    vrtArrayIdx.vt=VT_UINT;

    // Loop on ID array lenght
    for(UINT iArrayIdx=0; iArrayIdx<((UINT)iValCount); iArrayIdx++)
    {
        // Fill parameter array
        vrtArrayIdx.uintVal=iArrayIdx; ////// iArrayIdx

        // Delete, Create & Fill parameter array
        SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
        if(!psa)
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
            return FALSE;
        }
        LONG iParamIdx=0;
        if(SafeArrayPutElement(psa,&iParamIdx,&vrtArrayIdx)!=S_OK)
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
            SafeArrayDestroy(psa);
            return FALSE;
        }

        ////// Invoke DotNET control class method
        variant_t vrtRes;
        bstr_t bstrMethod(MHPDC_METHOD_GETSTRID);
        HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                            static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                            NULL,vtObjInst,psa,&vrtRes);
        SafeArrayDestroy(psa);
        if(FAILED(hr))
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
            return FALSE;
        }
        switch(vrtRes.vt)
        {   case VT_EMPTY: // Empty string
            {   assert(NULL); // Should be defined
                FillDotNetErr(__FUNCTION__,__LINE__,ERR_DATA_UNDEFINED);
                return FALSE;
            }
            case VT_BSTR: break; // Existing string
            default:
            {   assert(NULL);
                FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
                return FALSE;
            }
        }

        //
        char* lpData=NULL;
        ConvertBstrToASCII(lpData,vrtRes.bstrVal);
        VariantClear(&vrtRes);
        assert(lpData); // should be defined

        // Assign data
        pID->AddValue(lpData);
    }
    return TRUE;
}
BOOL CmHpDotNetCtrl::SetIdStrVal(UINT iArrayIdx,const char* lpValue) const
{
    // Create & Assign parameters
    VARIANT vrtValueID;
    VariantInit(&vrtValueID);
    vrtValueID.vt=VT_BSTR;

    ConvertAsciiToBSTR(&vrtValueID.bstrVal,lpValue); ////// ID string value

    VARIANT vrtArrayIdx;
    VariantInit(&vrtArrayIdx);
    vrtArrayIdx.vt=VT_UINT;

    vrtArrayIdx.uintVal=iArrayIdx; ////// iArrayIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,2);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        SysFreeString(vrtValueID.bstrVal);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtValueID)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SysFreeString(vrtValueID.bstrVal);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    iParamIdx=1;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtArrayIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SysFreeString(vrtValueID.bstrVal);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_SETSTRID);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SysFreeString(vrtValueID.bstrVal);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}

////// State
UINT CmHpDotNetCtrl::GetStateCount() const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return 0;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETSTATECOUNT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return 0;
    }
    if((vrtRes.vt!=VT_UI4)&&(vrtRes.vt!=VT_UINT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOUINT);
        return 0;
    }
    return vrtRes.uintVal;
}
CCtrlState* CmHpDotNetCtrl::GetState(UINT iStateIdx)
{
    // Check if 'FillDotNetData' has been called
    if((!pState)||(iStateIdx>=iStateCount))
    {   assert(NULL);
        return NULL;
    }

    return pState[iStateIdx];
}
BOOL CmHpDotNetCtrl::NeedToCheckState() const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_NEEDTOCHECKSTATE);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
const char* CmHpDotNetCtrl::DisplayState()
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NULL;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_DISPLAYSTATE);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NULL;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: return NULL; // Empty string
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            return NULL;
        }
    }

    //
    if(szDisplayState)
    {   delete [] szDisplayState;
        szDisplayState=NULL;
    }
    ConvertBstrToASCII(szDisplayState,vrtRes.bstrVal);
    VariantClear(&vrtRes);

    // Return control state under string shape
    if((szDisplayState)&&(*szDisplayState))
        return szDisplayState;
    return NULL;
}

////// Config
UINT CmHpDotNetCtrl::GetCfgCount() const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return 0;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETCFGCOUNT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return 0;
    }
    if((vrtRes.vt!=VT_UI4)&&(vrtRes.vt!=VT_UINT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOUINT);
        return 0;
    }
    return vrtRes.uintVal;
}
CCtrlCfg* CmHpDotNetCtrl::GetConfig(UINT iCfgIdx)
{
    // Check if 'FillDotNetData' has been called
    if((!pCfg)||(iCfgIdx>=iCfgCount))
    {   assert(NULL);
        return NULL;
    }

    return pCfg[iCfgIdx];
}

////// HTML tags
void CmHpDotNetCtrl::GetHtmlTags(char* lpTags,UINT iMaxSize,UINT iTaskIdx) const
{
    // Create & Fill parameter array
    VARIANT vrtTaskIdx;
    VariantInit(&vrtTaskIdx);
    vrtTaskIdx.vt=VT_UINT;

    vrtTaskIdx.uintVal=iTaskIdx; ////// iTaskIdx

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        *lpTags=0;
        return;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtTaskIdx)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        *lpTags=0;
        return;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_GETHTMLTAGS);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                      static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                      NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        *lpTags=0;
        return;
    }
    switch(vrtRes.vt)
    {   case VT_EMPTY: // Empty string
        {   *lpTags=0;
            break;
        }
        case VT_BSTR: break; // Existing string
        default:
        {   assert(NULL);
            FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBSTR);
            *lpTags=0;
            return;
        }
    }

    //
    ConvertBstrToASCII(lpTags,iMaxSize,vrtRes.bstrVal);
    VariantClear(&vrtRes);
    if(!strncmp(lpTags,BSTR_TOO_LONG,sizeof(BSTR_TOO_LONG)))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_BSTR2LONG);
        *lpTags=0;
    }
}

// --------------------------------
// Capture processus
//
BOOL CmHpDotNetCtrl::SetCaptMsgID(int iMsgID)
{
    // Create & Assign parameters
    VARIANT vrtMsgID;
    VariantInit(&vrtMsgID);
    vrtMsgID.vt=VT_INT;

    vrtMsgID.intVal=iMsgID; ////// iMsgID

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtMsgID)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_SETCAPTEVENT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::CaptureWin32(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
    // Check if 'Message' correspond to a DotNET registered window message
    // -> Call 'CaptureEvent' method

    // Create & Assign parameters
    VARIANT vrtEventID;
    VariantInit(&vrtEventID);
    vrtEventID.vt=VT_UINT;

    vrtEventID.uintVal=Message; ////// Message

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtEventID)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_CAPTUREEVENT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;    // TRUE: DotNET control message
                                    // FALSE: Otherwise
}

// --------------------------------
// Reproduce processus
//

//////
int CmHpDotNetCtrl::GetCtrlLoc(const char* lpMethod) const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return NO_VALUE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(lpMethod);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return NO_VALUE;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        return NO_VALUE;
    }
    return vrtRes.intVal;
}

//////
BOOL CmHpDotNetCtrl::GetCtrlPos(HWND hDlg,LONG &x,LONG &y,LONG &cx,LONG &cy) const
{
    // Get control left position
    x=(LONG)GetCtrlLoc(MHPDC_METHOD_GETCTRLLEFT);
    if(bDotNetCallErr)
        return FALSE;

    // Get control top position
    y=(LONG)GetCtrlLoc(MHPDC_METHOD_GETCTRLTOP);
    if(bDotNetCallErr)
        return FALSE;

    // Get control width
    cx=(LONG)GetCtrlLoc(MHPDC_METHOD_GETCTRLWIDTH);
    if(bDotNetCallErr)
        return FALSE;

    // Get control height
    cy=(LONG)GetCtrlLoc(MHPDC_METHOD_GETCTRLHEIGHT);
    if(bDotNetCallErr)
        return FALSE;

    return TRUE;
}
BOOL CmHpDotNetCtrl::CheckState(HWND hDlg) const
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_CHECKSTATE);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
ImHpCtrlMgmt::ActType CmHpDotNetCtrl::NextAction(HWND hDlg)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return ActError;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_NEXTACTION);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return ActError;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        return ActError;
    }
    return (ImHpCtrlMgmt::ActType)vrtRes.intVal;
}
BOOL CmHpDotNetCtrl::Start(HWND hDlg)
{
    // Create & Assign parameters
    VARIANT vrtForm;
    VariantInit(&vrtForm);
    vrtForm.vt=VT_INT;

    vrtForm.intVal=(INT)hDlg; ////// hDlg

    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,1);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }
    LONG iParamIdx=0;
    if(SafeArrayPutElement(psa,&iParamIdx,&vrtForm)!=S_OK)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_PUT_ELEMENT);
        SafeArrayDestroy(psa);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    VariantClear(&vrtRes);
    bstr_t bstrMethod(MHPDC_METHOD_START);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                    static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                    NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::End(HWND hDlg)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_END);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::MoveTo(HWND hDlg,int &xPos,int &yPos)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod1(MHPDC_METHOD_MOVETOX);
    HRESULT hr=spType->InvokeMember_3(bstrMethod1,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        SafeArrayDestroy(psa);
        return FALSE;
    }
    if(vrtRes.intVal==NO_VALUE)
    {   SafeArrayDestroy(psa);
        return FALSE;
    }

    // Assign new X position
    xPos=vrtRes.intVal;

    ////// Invoke DotNET control class method
    VariantClear(&vrtRes);
    bstr_t bstrMethod2(MHPDC_METHOD_MOVETOY);
    hr=spType->InvokeMember_3(bstrMethod2,
                                static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if((vrtRes.vt!=VT_I4)&&(vrtRes.vt!=VT_INT))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOINT);
        return FALSE;
    }
    if(vrtRes.intVal==NO_VALUE)
        return FALSE;

    // Assign new Y position
    yPos=vrtRes.intVal;

    return TRUE;
}
BOOL CmHpDotNetCtrl::MoveEvent(HWND hDlg)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_MOVEACT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::WaitUntil(HWND hDlg)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_WAITUNTIL);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::LeftPressEvent(HWND hDlg)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_LEFTPRESSACT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::LeftUnpressEvent(HWND hDlg)
{   
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_LEFTUNPRESSACT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::RightPressEvent(HWND hDlg)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_RIGHTPRESSACT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::RightUnpressEvent(HWND hDlg)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_RIGHTUNPRESSACT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}
BOOL CmHpDotNetCtrl::EditEvent(HWND hDlg)
{
    // Create & Fill parameter array
    SAFEARRAY* psa=SafeArrayCreateVector(VT_VARIANT,0,0);
    if(!psa)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_SAFEARRAY_CREATE_VECTOR);
        return FALSE;
    }

    ////// Invoke DotNET control class method
    variant_t vrtRes;
    bstr_t bstrMethod(MHPDC_METHOD_EDITACT);
    HRESULT hr=spType->InvokeMember_3(bstrMethod,
                                        static_cast<BindingFlags>(BindingFlags_InvokeMethod|BindingFlags_Instance|BindingFlags_Public),
                                        NULL,vtObjInst,psa,&vrtRes);
    SafeArrayDestroy(psa);
    if(FAILED(hr))
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_INVOKE_MEMBER_3);
        return FALSE;
    }
    if(vrtRes.vt!=VT_BOOL)
    {   assert(NULL);
        FillDotNetErr(__FUNCTION__,__LINE__,ERR_VARIANT_RES_NOBOOL);
        return FALSE;
    }
    return (BOOL)vrtRes.boolVal;
}

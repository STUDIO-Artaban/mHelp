// mHpCtrlMgmt.cpp
//

#include "mHpCtrlMgmt.h"
#include <stdio.h>

inline char* NewStrData(const char* szVal)
{   size_t nValSize=strlen(szVal)+1;
    char* lpNewVal=new char[nValSize];
    memcpy(lpNewVal,szVal,nValSize);
    return lpNewVal;
}

HWND GetCommonCtrlHandle(HWND hDlg,BOOL bKeepZeroID,const CCtrlID* pID,UINT &iIdIdx)
{   HWND hwndCtrl=GetWindow(hDlg,GW_CHILD);
    const int* iCtrlID=(const int*)pID->GetValue(iIdIdx);
    // Check to remove zero ID(s)
    if(!bKeepZeroID)
    {   while((iCtrlID!=NULL)&&(!*iCtrlID))
            iCtrlID=(const int*)pID->GetValue(++iIdIdx);
    }
    while((hwndCtrl!=NULL)&&(iCtrlID!=NULL))
    {   if(*iCtrlID==(int)GetWindowLongPtr(hwndCtrl,GWLP_ID))
        {   if(pID->GetValue(++iIdIdx)==NULL) // Last ID
                return hwndCtrl;
            HWND hwndPrev=hwndCtrl;
            hwndCtrl=GetCommonCtrlHandle(hwndCtrl,bKeepZeroID,pID,iIdIdx);
            if(hwndCtrl!=NULL)
                return hwndCtrl;
            hwndCtrl=hwndPrev;
            iIdIdx--;
        }
        hwndCtrl=GetWindow(hwndCtrl,GW_HWNDNEXT);
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
CData::CData(const char* szDataName,Type eDataType) : DataType(eDataType), pValue(NULL), iCount(0)
{   strncpy(szName,szDataName,DATA_NAME_SIZE);
}
CData::CData(const CData& CopyData) { *this=CopyData; }
CData::CData() : DataType(Undefined), pValue(NULL), iCount(0)
{   *szName=0;
}
CData::~CData()
{   ClearValue();
}

////// Operators
const CData& CData::operator=(const CData& CopyData)
{
    // Copy szName, DataType & iCount
    strncpy(szName,CopyData.GetName(),DATA_NAME_SIZE);
    DataType=CopyData.GetType();
    iCount=CopyData.GetCount();

    pValue=NULL;
    iCount=0;

    // Copy pValue
    switch(DataType)
    {   case Integer:
        {   if(CopyData.GetValue()!=NULL)
            {   const int* iCopyVal=(const int*)CopyData.GetValue();
                SetValue(*iCopyVal);
            }
            break;
        }
        case IntArray:
        {   if(CopyData.GetValue()!=NULL)
            {   for(UINT i=0; i<CopyData.GetCount(); i++)
                {   const int* iCopyVal=(const int*)CopyData.GetValue(i);
                    AddValue(*iCopyVal);
                }
            }
            break;
        }
        case String:
        {   if(CopyData.GetValue()!=NULL)
            {   const char* lpCopyVal=(const char*)CopyData.GetValue();
                SetValue(lpCopyVal);
            }
            break;
        }
        case StrArray:
        {   if(CopyData.GetValue()!=NULL)
            {   for(UINT i=0; i<CopyData.GetCount(); i++)
                {   const char* lpCopyVal=(const char*)CopyData.GetValue(i);
                    AddValue(lpCopyVal);
                }
            }
            break;
        }
        case Undefined:
        {   break; }
    }

    return *this;
}
bool CData::operator==(const CData& CmpData) const
{
    // Compare szName & DataType
    if((strncmp(szName,CmpData.GetName(),DATA_NAME_SIZE))||
       (DataType!=CmpData.GetType()))
    {   return false;
    }

    // Compare iCount
    if(iCount!=CmpData.GetCount())
    {
        ////// WARNING: Add Win32 exception!
        // -> Operator == only used to compare pID type IntArray (Win32)
        // -> According parent window passed to SetWin32CtrlID parameter (0^1009 intead of 1009)
        if(DataType==IntArray)
        {   UINT iLastValIdx=0;
            const int* iVal;
            do
            {   iVal=(const int*)pValue[iLastValIdx++];
            } while((iCount>iLastValIdx)&&(*iVal==0));
            // Check index
            if(iCount>=iLastValIdx)
            {   UINT iLastCmpIdx=0;
                const int* iCmpVal;
                do
                {   iCmpVal=(const int*)CmpData.GetValue(iLastCmpIdx++);
                } while((iCmpVal!=NULL)&&(*iCmpVal==0));
                // Check index
                if(CmpData.GetCount()>=iLastCmpIdx)
                {   while((iCount!=iLastValIdx)&&(CmpData.GetCount()!=iLastCmpIdx))
                    {   iVal=(const int*)pValue[iLastValIdx++];
                        iCmpVal=(const int*)CmpData.GetValue(iLastCmpIdx++);
                        if(*iVal!=*iCmpVal)
                            return false;
                    }    
                    if((iCount==iLastValIdx)&&(CmpData.GetCount()==iLastCmpIdx)&&(*iVal==*iCmpVal))
                        return true; // Same pValue (after having removed all 0^)
                }
            }
        }
        //////

        return false;
    }

    // Compare pValue
    switch(DataType)
    {   case Integer:
        case IntArray:
        {   if(pValue!=NULL)
            {   for(UINT i=0; i<iCount; i++)
                {   const int* iCmpVal=(const int*)CmpData.GetValue(i);
                    const int* iVal=(const int*)pValue[i];
                    if((iVal!=NULL)&&(iCmpVal!=NULL))
                    {   if(*iVal!=*iCmpVal) // Compare integer value
                            return false;
                    }
                    else if(iVal!=iCmpVal)
                        return false;
                }
            }
            else if(CmpData.GetValue()!=NULL)
                return false;
            break;
        }
        case String:
        case StrArray:
        {   if(pValue!=NULL)
            {   for(UINT i=0; i<iCount; i++)
                {   const char* szCmpVal=(const char*)CmpData.GetValue();
                    const char* szVal=(const char*)pValue[0];
                    if((szVal!=NULL)&&(szCmpVal!=NULL))
                    {   if(strcmp(szVal,szCmpVal)) // Compare string value
                            return false;
                    }
                    else if(szVal!=szCmpVal)
                        return false;
                }
            }
            else if(CmpData.GetValue()!=NULL)
                return false;
            break;
        }
        case Undefined:
        {   break; }
    }

    return true; // Same CData
}

////// Common
const char* CData::GetName() const { return szName; }
CData::Type CData::GetType() const { return DataType; }
UINT CData::GetCount() const { return iCount; }
const void* CData::GetValue(UINT iIdx) const { return (iIdx<iCount)? pValue[iIdx]:NULL; }

void CData::SetValue(int iNewVal,UINT iIdx)
{   switch(DataType)
    {   case Integer:
        {   assert(!iIdx);
            if(iIdx)
                return; // Failed: Not an array

            if(pValue!=NULL)
            {   delete pValue[0];
                int* pNewVal=new int(iNewVal);
                pValue[0]=pNewVal;
            }
            else
            {   // Define one integer pointer
                pValue=new void*[1];
                int* pNewVal=new int(iNewVal);
                pValue[0]=pNewVal;
                iCount=1;
            }
            break;
        }
        case IntArray:
        {
            assert(iIdx<iCount);
            if(iIdx>=iCount)
                return; // Failed: Wrong index

            delete pValue[iIdx];
            int* pNewVal=new int(iNewVal);
            pValue[iIdx]=pNewVal;
            break;
        }
        case String:
        case StrArray:
        case Undefined:
        {   break; } // Failed: Wrong data type
    }
}
void CData::SetValue(const char* szNewVal,UINT iIdx)
{   switch(DataType)
    {   case String:
        {   assert(!iIdx);
            if(iIdx)
                return; // Failed: Not an array

            if(pValue!=NULL)
            {   delete [] pValue[0];
                pValue[0]=NewStrData(szNewVal);
            }
            else
            {   // Define one string pointer
                pValue=new void*[1];
                pValue[0]=NewStrData(szNewVal);
                iCount=1;
            }
            break;
        }
        case StrArray:
        {
            assert(iIdx<iCount);
            if(iIdx>=iCount)
                return; // Failed: Wrong index

            delete [] pValue[iIdx];
            pValue[iIdx]=NewStrData(szNewVal);
            break;
        }
        case Integer:
        case IntArray:
        case Undefined:
        {   break; } // Failed: Wrong data type
    }
}
void CData::ClearValue()
{   if(pValue)
    {   for(UINT i=0; i<iCount; i++)
        {   if(pValue[i])
            {   if((DataType==Integer)||(DataType==IntArray))
                    delete pValue[i];
                else
                    delete [] pValue[i]; // String
            }
        }
        delete [] pValue;
    }
    pValue=NULL;
}

////// Array
void CData::AddValue(int iNewVal)
{
    assert(DataType==IntArray);
    if(DataType!=IntArray)
        return; // Failed: Integer array only

    if(pValue!=NULL)
    {   void** pLastVal=pValue;
        pValue=new void*[iCount+1];
        for(UINT i=0; i<iCount; i++)
            pValue[i]=pLastVal[i];
        delete [] pLastVal;

        int* pNewVal=new int(iNewVal);
        pValue[iCount]=pNewVal;
        iCount++;
    }
    else
    {   pValue=new void*[1];
        int* pNewVal=new int(iNewVal);
        pValue[0]=pNewVal;
        iCount=1;
    }
}
void CData::AddValue(const char* szNewVal)
{
    assert(DataType==StrArray);
    if(DataType!=StrArray)
        return; // Failed: String array only

    if(pValue!=NULL)
    {   void** pLastVal=pValue;
        pValue=new void*[iCount+1];
        for(UINT i=0; i<iCount; i++)
            pValue[i]=pLastVal[i];
        delete [] pLastVal;

        pValue[iCount]=NewStrData(szNewVal);
        iCount++;
    }
    else
    {   pValue=new void*[1];
        pValue[0]=NewStrData(szNewVal);
        iCount=1;
    }
}
void CData::Reverse()
{
    assert((DataType==IntArray)||(DataType==StrArray));
    if((DataType!=IntArray)&&(DataType!=StrArray))
        return; // Failed: Not an array

    for(UINT i=0; i<(iCount/2); i++)
    {   void* pVal=pValue[i];
        pValue[i]=pValue[iCount-i-1];
        pValue[iCount-i-1]=pVal;
    }
}

///////////////////////////////////////////////////////////////////////////////
ImHpCtrlMgmt::ImHpCtrlMgmt(UINT iIdx,CData::Type TypeID) : iIndex(iIdx), PrevAct(ActEnd),
    iCtrlStyleID(NO_VALUE), iCaptMsgID(NO_VALUE), szErrorMsg(NULL), szDisplayState(NULL),
    iActStep(0), hControl(NULL), pName(NULL), pState(NULL), pCfg(NULL)
{   *szClass=0;
    *szDllVersion=0;
    *szStateBound=0;
    pID=new CCtrlID(CTRL_ID_NAME,TypeID);
}
ImHpCtrlMgmt::~ImHpCtrlMgmt()
{   if(szErrorMsg)
        delete [] szErrorMsg;
    if(szDisplayState)
        delete [] szDisplayState;
    if(pID)
        delete pID;
}

//////
UINT ImHpCtrlMgmt::GetIndex() const { return iIndex; }
const char* ImHpCtrlMgmt::GetLastErr() const { return szErrorMsg; }

void CopymHpCtrlMgmt(ImHpCtrlMgmt& Dest,const ImHpCtrlMgmt& Copy,UINT iStateCount,UINT iCfgCount)
{
    // Copy control management data (included protected data)
    strncpy(Dest.szClass,Copy.szClass,CTRL_CLASS_SIZE);

    assert(!Dest.pState);
    if(iStateCount)
    {   Dest.pState=new CCtrlState*[iStateCount];
        for(UINT i=0; i<iStateCount; i++)
        {   Dest.pState[i]=new CCtrlState(*Copy.pState[i]);
        }
    }

    assert(!Dest.pCfg);
    if(iCfgCount)
    {   Dest.pCfg=new CCtrlCfg*[iCfgCount];
        for(UINT i=0; i<iCfgCount; i++)
        {   Dest.pCfg[i]=new CCtrlCfg(*Copy.pCfg[i]);
        }
    }

    *Dest.pID=*Copy.pID;

    strncpy(Dest.szStateBound,Copy.szStateBound,STATE_BOUND_SIZE);

    Dest.iCtrlStyleID=Copy.iCtrlStyleID;
    Dest.iCaptMsgID=Copy.iCaptMsgID;
}

// --------------------------------
// Win32 tools
//
void ImHpCtrlMgmt::SetWin32CtrlID(HWND hDlg)
{
    //////HWND hActive=GetActiveWindow();
    // -> Can be mHelp Server dialog!!!
    HWND hActive=hControl;
    while(GetParent(hActive)!=NULL)
        hActive=GetParent(hActive);
    //////

    HWND hParent=hDlg;
    pID->ClearValue();
    pID->AddValue(GetDlgCtrlID(hControl));
    while((hActive!=hParent)&&(hParent!=NULL))
    {   pID->AddValue(GetDlgCtrlID(hParent));
        hParent=GetParent(hParent);
    }
    pID->Reverse();
}
void ImHpCtrlMgmt::GetWin32CtrlHandle(HWND hDlg)
{   hControl=hDlg;
    bool bNoID=false;
    UINT iIdIdx=0;
    const int* iCtrlID=(const int*)pID->GetValue(iIdIdx);
    while((iCtrlID!=NULL)&&(hControl!=NULL))
    {   if(!*iCtrlID)
            bNoID=true;
        // Get control/parent handle using ID
        hControl=GetDlgItem(hControl,*iCtrlID);
        iCtrlID=(const int*)pID->GetValue(++iIdIdx);
    }
    if((!hControl)&&(bNoID)) // Dialog control (ID == 0)
    {   UINT iIdx=0;
        hControl=GetCommonCtrlHandle(hDlg,TRUE,pID,iIdx);
        if(!hControl)
            hControl=GetCommonCtrlHandle(hDlg,FALSE,pID,iIdx); // Last chance
    }
}
void ImHpCtrlMgmt::ForceWin32ScrollMsg(HWND hDlg,int iScrollPos,UINT iNotifyCode,BOOL bVertical)
{
    // Simulate a window vertical/horizontal scroll message (WM_VSCROLL & WM_HSCROLL)
    LPARAM lParam=(LPARAM)hControl;
    WPARAM wParam=MAKEWPARAM(iNotifyCode,iScrollPos);
	if(bVertical)
        SendMessage(hDlg,WM_VSCROLL,wParam,lParam);
	else
        SendMessage(hDlg,WM_HSCROLL,wParam,lParam);
}
void ImHpCtrlMgmt::ForceWin32NotifyMsg(HWND hDlg,UINT iNotifyCode)
{
    int iCurCtrlID=GetDlgCtrlID(hControl);

    // Simulate a window notify message (WM_NOTIFY)
    NMHDR NotifyStruct;
    NotifyStruct.code=iNotifyCode;
    NotifyStruct.hwndFrom=hControl;
    NotifyStruct.idFrom=(UINT_PTR)iCurCtrlID;

    SendMessage(hDlg,WM_NOTIFY,(WPARAM)iCurCtrlID,(LPARAM)&NotifyStruct);
}

// --------------------------------
// Qt tools
//
BOOL ImHpCtrlMgmt::FillQtData() { return FALSE; }
BOOL ImHpCtrlMgmt::UpdateQtData() { return FALSE; }
void ImHpCtrlMgmt::CloseQtUI() { }

// --------------------------------
// Properties
//
const char* ImHpCtrlMgmt::GetDllVersion() const { return szDllVersion; }

////// ID
CCtrlID* ImHpCtrlMgmt::GetCtrlID() { return pID; }

////// Win32 Class name
const char* ImHpCtrlMgmt::GetCtrlClass() const { return szClass; }
void ImHpCtrlMgmt::SetCtrlClass(const char* lpClass) { strncpy(szClass,lpClass,CTRL_CLASS_SIZE); }

////// Style
int ImHpCtrlMgmt::GetStyle() const { return iCtrlStyleID; }

////// State
const char* ImHpCtrlMgmt::GetStateBound() const { return szStateBound; }

// --------------------------------
// Capture processus
//
int ImHpCtrlMgmt::GetCaptMsgID() const { return iCaptMsgID; }

// --------------------------------
// Reproduce processus
//
BOOL ImHpCtrlMgmt::CheckNextAct(ActType NextAct)
{
    // Check next consistent action
    switch(PrevAct)
    {   case ActLeftPress:
        {   switch(NextAct)
            {   case ActLeftUnpress:
                case ActMove:
                {   break; // Consistent
                }
                default: // Error
                {   return FALSE; }
            }
            break;
        }
        case ActRightPress:
        {   switch(NextAct)
            {   case ActRightUnpress:
                case ActMove:
                {   break; // Consistent
                }
                default: // Error
                {   return FALSE;
                }
            }
            break;
        }
    }

    // Assign action if requires next consistent action
    switch(NextAct)
    {   case ActLeftPress:
        case ActRightPress:
        case ActLeftUnpress:
        case ActRightUnpress:
        {   PrevAct=NextAct;
            break;
        }
        case ActMove:
        case ActWait:
        {   // Do not change previous action
            break;
        }
        default:
        {   // No previous action check required
            PrevAct=ActEnd;
            break;
        }
    }
    return TRUE;
}
ImHpCtrlMgmt::ActType ImHpCtrlMgmt::GetPrevAct() const { return PrevAct; }

// ComCtrl.cpp
//

#include "stdafx.h"
#include <ComCtrl.h>

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif
#include <stdio.h>

// mHpCtrl's include --------------------------------------------------------------------------------------------------**//
#ifndef __MENUCTRL_H
 #include <Menu\MenuCtrl.h>
#endif
#ifndef __COMBOCTRL_H
 #include <ComboBox\CboCtrl.h>
#endif
#ifndef __LISTCTRL_H
 #include <ListBox\ListCtrl.h>
#endif
#ifndef __EDITCTRL_H
 #include <EditBox\EditCtrl.h>
#endif
#ifndef __SLIDERCTRL_H
 #include <Slider\SlidCtrl.h>
#endif
#ifndef __BUTTONCTRL_H
 #include <Button\BtnCtrl.h>
#endif
#ifndef __CHANGEEXECTRL_H
 #include <ChangeExe\ChExCtrl.h>
#endif
#ifndef __LISTVIEWCTRL_H
 #include <ListView\ListView.h>
#endif
#ifndef __RICHEDITCTRL_H
 #include <RichEdit\EditRich.h>
#endif
#ifndef __TREEVIEWCTRL_H
 #include <TreeView\TreeView.h>
#endif
#ifndef __TABCTRL_H
 #include <TabCtrl\TabCtrl.h>
#endif
#ifndef __TOOLBARCTRL_H
 #include <ToolBar\ToolBar.h>
#endif
#ifndef __CTRLMAN_H
 #include "CtrlMan.h"
#endif

//---------------------------------------------------------------------------------------------------------------------**//
std::string StartPostData;

//****************************************************************************************************************GENERAL//
// AddIfChangeExe
//-----------------------------------------------------//
void AddIfChangeExe(CEventListe* EventObject,char *lpAppName)
{	CmHpTask* AppTask;
	if(EventObject->GetEventCount()<=(GetNextKeyPos()-1))
	{	ReadEventKeys(EventObject);
		AppTask=new CChangeExeTask(0,FALSE,NULL,lpAppName,NULL);
		AppTask->WriteTaskKey();
		EventObject->AddEvent(AppTask,NULL,NULL);
	}
}

//-----------------------------------------------------//
// ReadEventKeys
//-----------------------------------------------------//
void ReadEventKeys(CEventListe* EventObject)
{	int index=0,imHpId;
	EventObject->EmptyEvents();
	imHpId=GetmHpIDKey(index);
	while(imHpId!=NO)
	{	// ReadEventKeys ----------------------------------------------------------------------------------------------**//
		ComCtrl_ReadEventKeys(imHpId,
							  index,
							  EventObject,
							  NULL);
		//-------------------------------------------------------------------------------------------------------------**//
		index++;
		imHpId=GetmHpIDKey(index);
	}
}

//-----------------------------------------------------//
// CreateChangeExeObj
//-----------------------------------------------------//
CmHpTask* CreateChangeExeObj(BOOL bLaunch,char* lpMess,char* lpNewApp,char* lpCmd)
{	CmHpTask* cResTask=new CChangeExeTask(0,bLaunch,lpMess,lpNewApp,lpCmd);
	return cResTask;
}

//-----------------------------------------------------//
// GetCtrlmHpID
//-----------------------------------------------------//
int GetCtrlmHpID(HWND hWndActive,HWND hWndCtrl,CEventListe* EvntObj,UINT pCtrlID)
{	char lpmHpClass[MAX_KEY_SIZE]={0};
	if(!GetClassName(hWndCtrl,lpmHpClass,MAX_KEY_SIZE))
        *lpmHpClass=0;
    int nClassSize=(int)strlen(lpmHpClass);

    // mHpCtrl's Class name -------------------------------------------------------------------------------------------**//
	if(Menu_IsClassName(hWndActive,lpmHpClass,nClassSize,pCtrlID))
	{	if(ToolBar_IsItDropDown(EvntObj)) return 18;									// ToolBar - DropDown (Menu)
		else return 1;																	// Menu
	}
	if(Button_IsClassName(hWndCtrl,lpmHpClass,nClassSize)) return 2;					// Button
	if(SingleList_IsClassName(hWndCtrl,lpmHpClass,nClassSize)) return 3;				// SingleList
	if(Combo_IsClassName(hWndCtrl,lpmHpClass,nClassSize))
	{	GetClassName(hWndActive,lpmHpClass,nClassSize);
		if(ToolBar_IsClassName(NULL,lpmHpClass,nClassSize,0)!=0) return 17;				// ToolBar - ComboBox
		else return 4;																	// Combo
	}
	if(Slider_IsClassName(lpmHpClass,nClassSize)) return 5;								// Slider
	if(Edit_IsClassName(lpmHpClass,nClassSize)) return 6;								// Edit
	if(Check_IsClassName(hWndCtrl,lpmHpClass,nClassSize)) return 7;						// Check
	if(Radio_IsClassName(hWndCtrl,lpmHpClass,nClassSize)) return 8;						// Radio
	if(MultiList_IsClassName(hWndCtrl,lpmHpClass,nClassSize)) return 9;					// MultiList
	if(TabCtrl_IsClassName(lpmHpClass,nClassSize)) return 10;							// TabCtrl
	if(SListView_IsClassName(hWndCtrl,lpmHpClass,nClassSize)) return 11;				// SingleListView
	if(MListView_IsClassName(hWndCtrl,lpmHpClass,nClassSize)) return 12;				// MultiListView
	if(TreeView_IsClassName(lpmHpClass,nClassSize)) return 13;							// TreeView
	int iResClss=ToolBar_IsClassName(hWndCtrl,lpmHpClass,nClassSize,pCtrlID);
	switch(iResClss)
	{	case 14:																		// ToolBar - Button
		case 15:																		// ToolBar - Checkbox
		case 16:																		// ToolBar - Radiobox
		case 18:																		// ToolBar - DropDown
		{	return iResClss;}
		default:{	break;}
	}
	//-------------------------------------------------//
	if(RichEdit_IsClassName(lpmHpClass,nClassSize)) return 0;
	//-----------------------------------------------------------------------------------------------------------------**//

	// Contrôles non supportés
	//if(mHpPak_UnEventList->IsItUnEvent(lpmHpClass)) return 0;
	//return NO;
    return 0;
}

//****************************************************************************************************************COMCTRL//
// ComCtrl_AddDotNetCtrlNotify
//-----------------------------------------------------//
BOOL ComCtrl_AddDotNetCtrlNotify(HWND hWndCtrl)
{
    // Get parent control window
    HWND hDlg=hWndCtrl;
    while(GetParent(hDlg)!=NULL)
        hDlg=GetParent(hDlg);

    ////// Add event(s) notification on current DotNET control
    // -> If DotNET control handle
    return GetmHpVarStruct()->CtrlMgmtList->AddDotNetCtrlEvents(hDlg,hWndCtrl);
}

//-----------------------------------------------------//
// ComCtrl_AddPlatformCtrlNotify
//-----------------------------------------------------//
BOOL ComCtrl_AddPlatformCtrlNotify(HWND hWndActive)
{
    ////// Add slot(s) notification on all Qt controls (-> If any Qt control)
    ////// Add event(s) notification on all WPF controls (-> If any WPF control)
    ////// Add action event(s) notification on all Java controls (-> If any Java control)
    return GetmHpVarStruct()->CtrlMgmtList->AddCtrlEvents(hWndActive);
}

//-----------------------------------------------------//
// ComCtrl_IsNoNotifyDotNetCtrl
//-----------------------------------------------------//
/*BOOL ComCtrl_IsNoNotifyDotNetCtrl(HWND hWndCtrl)
{   char lpClassName[MAX_RS_SIZE]={0};
    GetClassName(hWndCtrl,lpClassName,MAX_RS_SIZE);
    if((!strncmp(lpClassName,DOTNET_TASK_BUTTON,sizeof(DOTNET_TASK_BUTTON)-1))||    // DotNet Button
       (!strncmp(lpClassName,DOTNET_TASK_TREEVIEW,sizeof(DOTNET_TASK_TREEVIEW)-1))) // DotNet TreeView
        return TRUE;
    return FALSE;
}

//-----------------------------------------------------//
// ComCtrl_LockNoNotifyDotNetCtrlMsg
//-----------------------------------------------------//
BOOL ComCtrl_LockNoNotifyDotNetCtrlMsg(UINT Message,LPARAM lParam)
{   // Check to lock messages provided through DotNet no notify control management
    switch(Message)
    {   // No notify DotNet control messages (WM_COMMAND)
        case WM_COMMAND:
        {   if(ComCtrl_IsNoNotifyDotNetCtrl((HWND)lParam))
                return TRUE;
            break;
        }
        // No notify DotNet control messages (WM_NOTIFY)
        case WM_NOTIFY:
        {   if(ComCtrl_IsNoNotifyDotNetCtrl(((LPNMHDR)lParam)->hwndFrom))
            return TRUE;
        }
    }
    return FALSE;
}*/

//-----------------------------------------------------//
// ComCtrl_GetmHpID
//-----------------------------------------------------//
void ComCtrl_GetmHpID(HWND hWndActive,HWND hWndCtrl,CEventListe* EvntObj,UINT pCtrlID,int& imHpID,char* lpCtrlName)
{   /*if(mHpPak_IsQtHwnd(hWndCtrl))
    {   mHpPak_GetQtCtrlInfo(hWndCtrl,imHpID,lpCtrlName,MAX_CTRL_NAME);
        return;
    }*/
    imHpID=GetCtrlmHpID(hWndActive,hWndCtrl,EvntObj,pCtrlID);
	/*if(mHpPak_IsDotNetHwnd(hWndActive))
	{	if(imHpID<2) return; // Unknown/Unsupported/Menu control
		int iDotNetmHpID=NO;
		if(mHpPak_GetDotNetCtrlInfo(hWndActive,hWndCtrl,iDotNetmHpID,lpCtrlName,MAX_CTRL_NAME))
		{   switch(imHpID)
			{   case 2: // Button
				{   imHpID=iDotNetmHpID; // Button style: Button, CheckBox or RadioBox
					break;
				}
			}
		}
		//else imHpID=NO; // Not a DotNet control in a DotNet application!
	}*/
}

//-----------------------------------------------------//
// ComCtrl_AddIcon
//-----------------------------------------------------//
void ComCtrl_AddIcon(HIMAGELIST hImgLst)
{   ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_DEFAULT)));
    // mHpCtrl's icon -------------------------------------------------------------------------------------------------**//
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_EXE)));				     // 0
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_MENU)));			     // 1
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_BUTTON)));			     // 2
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_SINGLELIST)));		     // 3
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_COMBO)));			     // 4
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_SLIDER)));			     // 5
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_EDIT)));			     // 6
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_CHECK)));			     // 7
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_RADIO)));			     // 8
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_MULTILIST)));		     // 9
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TABCTRL)));			     // 10
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_LISTVIEW)));		     // 11
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_LISTVIEW)));		     // 12
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TREEVIEW)));		     // 13
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TOOLBAR)));			     // 14
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TOOLBAR)));			     // 15
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TOOLBAR)));			     // 16
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TOOLBAR)));			     // 17
	ImageList_AddIcon(hImgLst,LoadIcon(GetmHpVarStruct()->hmHpCtrlInst,MAKEINTRESOURCE(IDI_ICON_TOOLBAR)));			     // 18

    ////// mHelp Control Management /////////////////////////////////
    UINT iCtrlMgmtIdx=0;
    CCtrlMgmtCell* CurCtrlCell=GetmHpVarStruct()->CtrlMgmtList->GetCtrlMgmtCell(iCtrlMgmtIdx);
    while(CurCtrlCell!=NULL)
    {   // Reset number of loaded icon
        CurCtrlCell->iIconCount=0;

        // Add icon(s) from current control management
        UINT iIconIdx=0;
        HICON hCtrlIcon=NULL;

        ////// Check Qt Platform
        if(CurCtrlCell->GetCtrlPlatformID()==PLATFORM_ID_QT)
            hCtrlIcon=CurCtrlCell->GetmHpQtCtrlMgmt()->GetQtCtrlMgmt()->GetIconList(iIconIdx);
        else
            hCtrlIcon=CurCtrlCell->GetmHpCtrlMgmt()->GetIconList(iIconIdx);

        while(hCtrlIcon!=NULL)
        {
            if(ImageList_AddIcon(hImgLst,hCtrlIcon)==NO)
                break; // Error: Failed to add icon into image list

            // Next icon
            CurCtrlCell->iIconCount++;

            ////// Check Qt Platform
            if(CurCtrlCell->GetCtrlPlatformID()==PLATFORM_ID_QT)
                hCtrlIcon=CurCtrlCell->GetmHpQtCtrlMgmt()->GetQtCtrlMgmt()->GetIconList(++iIconIdx);
            else
                hCtrlIcon=CurCtrlCell->GetmHpCtrlMgmt()->GetIconList(++iIconIdx);
        }

        // Next control management
        CurCtrlCell=GetmHpVarStruct()->CtrlMgmtList->GetCtrlMgmtCell(++iCtrlMgmtIdx);
    }
	//-----------------------------------------------------------------------------------------------------------------**//
}

//-----------------------------------------------------//
// ComCtrl_AddCommonUnEvnt
//-----------------------------------------------------//
void ComCtrl_AddCommonUnEvnt()
{	// CommonUnEvnt ---------------------------------------------------------------------------------------------------**//
	mHpPak_UnEventList->AddUnEvent("msctls_statusbar32");
	mHpPak_UnEventList->AddUnEvent(TOOLTIPS_CLASS_VB);
	mHpPak_UnEventList->AddUnEvent("ComboLBox");
	mHpPak_UnEventList->AddUnEvent(TOOLTIPS_CLASS_A);
	mHpPak_UnEventList->AddUnEvent(TOOLTIPS_CLASS_B);
	mHpPak_UnEventList->AddUnEvent("ComboBoxEx32");
	mHpPak_UnEventList->AddUnEvent("ReBarWindow32");
    // v1.3
	mHpPak_UnEventList->AddUnEvent("#32770");
    mHpPak_UnEventList->AddUnEvent("SysHeader32");
    mHpPak_UnEventList->AddUnEvent("WindowsForms10.tooltips_class32.app",false); // WindowsForms10.tooltips_class32.app.0.2bf8098_r17_ad1
	//-----------------------------------------------------------------------------------------------------------------**//
}

//-----------------------------------------------------//
// ComCtrl_ItemIndexToSel
//-----------------------------------------------------//
int ComCtrl_ItemIndexToSel(HWND hWndActive)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpID())
	{	// DinCtrl's Index To Select ----------------------------------------------------------------------------------**//
		case 9: // MultiList
		{	return MultiList_ItemIndexToSel(hWndActive);
		}
		case 12: // MultiListView
		{	return MListView_ItemIndexToSel(hWndActive);
		}
		//-------------------------------------------------------------------------------------------------------------**//
		default:
		{	break;}
	}
	return NO;
}

//****************************************************************************************************************MHPFILE//
// ComCtrl_ReadEvent
//-----------------------------------------------------//
BOOL ComCtrl_ReadEvent(HWND hWndActive,int pmHpID,UINT iPlatformID,int iClass,int iStyle,int iMsgID,UINT &iCustIdx)
{	switch(pmHpID)
	{	// mHpCtrl's read ---------------------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	return ChangeExe_ReadEvent(hWndActive,StartPostData,iCustIdx);
		}
		case 1: // Menu
		{	return Menu_ReadEvent(hWndActive);
		}
		case 2: // Button
		{	return Button_ReadEvent(hWndActive);
		}
		case 3: // SingleList
		case 9: // MultiList
		{	return List_ReadEvent(hWndActive,pmHpID,StartPostData,iCustIdx);
		}
		case 4: // Combo
		{	return Combo_ReadEvent(hWndActive,StartPostData,iCustIdx);
		}
		case 5: // Slider
		{	return Slider_ReadEvent(hWndActive);
		}
		case 6: // Edit
		{	return Edit_ReadEvent(hWndActive,StartPostData,iCustIdx);
		}
		case 7: // Check
		{	return CheckRadio_ReadEvent(hWndActive,7);
		}
		case 8: // Radio
		{	return CheckRadio_ReadEvent(hWndActive,8);
		}
		case 10: // TabCtrl
		{	return TabCtrl_ReadEvent(hWndActive,StartPostData,iCustIdx);
		}
		case 11: // SingleListView
		case 12: // MultiListView
		{	return ListView_ReadEvent(hWndActive,pmHpID,StartPostData,iCustIdx);
		}
		case 13: // TreeView
		{	return TreeView_ReadEvent(hWndActive,StartPostData,iCustIdx);
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	return ToolBar_ReadEvent(hWndActive,pmHpID,StartPostData,iCustIdx);
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            return CtrlMan_ReadEvent(hWndActive,iPlatformID,iClass,iStyle,iMsgID,StartPostData,iCustIdx);
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:
		{	break;}
	}
	// Error
	return TRUE;
}

//-----------------------------------------------------//
// ComCtrl_WriteTask
//-----------------------------------------------------//
void ComCtrl_WriteTask(HWND hWndActive,int &mHpIndex,char* lpCurNameExe,int CurSize,CEventListe* Evntobj,
                       CWindowListe* ReadWndLst,BOOL bParam,int &iParam,BOOL &bResult)
{	static BOOL bFirstLnk=TRUE;
	if(!bResult) bFirstLnk=TRUE;
	bResult=TRUE;
	switch(Evntobj->GetmHpTask(mHpIndex,FALSE)->GetmHpID())
	{	// mHpCtrl's written mHpFile ----------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_WriteEvent(hWndActive,mHpIndex,Evntobj,bParam,lpCurNameExe,CurSize,iParam,bResult);
			break;
		}
		case 1: // Menu
		{	Menu_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,lpCurNameExe,bResult);
			break;
		}
		case 2: // Button
		{	Button_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,lpCurNameExe,bResult);
			break;
		}
		case 3: // SingleList
		{	SingleList_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 4: // Combo
		{	Combo_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 5: // Slider
		{	Slider_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 6: // Edit
		{	Edit_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 7: // Check
		{	Check_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 8: // Radio
		{	Radio_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bFirstLnk,bResult);
			break;
		}
		case 9: // MultiList
		{	MultiList_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 11: // SingleListView
		{	SListView_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 12: // MultiListView
		{	MListView_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 13: // TreeView
		{	TreeView_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bFirstLnk,bResult);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            CtrlMan_WriteEvent(hWndActive,mHpIndex,Evntobj,ReadWndLst,bParam,lpCurNameExe,iParam,bResult);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:
		{	bResult=FALSE;
			break;
		}
	}
}

//-----------------------------------------------------//
// ComCtrl_WriteParam
//-----------------------------------------------------//
BOOL ComCtrl_WriteParam(HWND hWndActive,int mHpIndex,CEventListe* Evntobj)
{	switch(Evntobj->GetmHpTask(mHpIndex,FALSE)->GetmHpID())
	{	// mHpCtrl's write param --------------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	return ChangeExe_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 1: // Menu
		{	return Menu_WriteParam();
		}
		case 2: // Button
		{	return Button_WriteParam();
		}
		case 3: // SingleList
		{	return SingleList_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 4: // Combo
		{	return Combo_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 5: // Slider
		{	return Slider_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 6: // Edit
		{	return Edit_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 7: // Check
		{	return Check_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 8: // Radio
		{	return Radio_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 9: // MultiList
		{	return MultiList_WriteParam(hWndActive,Evntobj,mHpIndex);
		}			
		case 10: // TabCtrl
		{	return TabCtrl_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 11: // SingleListView
		{	return SListView_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 12: // MultiListView
		{	return MListView_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 13: // TreeView
		{	return TreeView_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	return ToolBar_WriteParam(hWndActive,Evntobj,mHpIndex);
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            return CtrlMan_WriteParam(hWndActive,Evntobj,mHpIndex);
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{	break;}
	}
	return TRUE;
}

//*******************************************************************************************************************KEYS//
// ComCtrl_ReadEventKeys
//-----------------------------------------------------//
void ComCtrl_ReadEventKeys(int pmHpID,int mHpIndex,CEventListe* Evntobj,CTaskListe* Tasklst)
{	switch(pmHpID)
	{	// mHpCtrl's reading Event keys -------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 1: // Menu
		{	Menu_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 2: // Button
		{	Button_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 3: // SingleList
		case 9: // MultiList
		{	List_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 4: // Combo
		{	Combo_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 5: // Slider
		{	Slider_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 6: // Edit
		{	Edit_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 7: // Check
		{	Check_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 8: // Radio
		{	Radio_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 11: // SingleListView
		case 12: // MultiListView
		{	ListView_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 13: // TreeView
		{	TreeView_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_ReadEventKeys(mHpIndex,Evntobj,Tasklst,pmHpID);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            CtrlMan_ReadEventKeys(mHpIndex,Evntobj,Tasklst);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{	break;}
	}
}

//********************************************************************************************************************ADD//
// ComCtrl_AddEvent
//-----------------------------------------------------//
void ComCtrl_AddEvent(HWND hWndActive,char* lpCtrlName,ImHpCtrlMgmt* pCtrlMgmt,UINT iPlatformID,HWND hWndInCtrl,
                      WPARAM wParam,int pmHpID,int &iLastID,int ictrlID,int mHpCur,int mHpCnt)
{	int iLastTab[MAX_PART_TREE]={0};
	int iCtrlTab[MAX_PART_TREE]={0};
	switch(pmHpID)
	{	// mHpCtrl's write event --------------------------------------------------------------------------------------**//
		case 1: // Menu
		{	Menu_AddEvent(hWndActive,ictrlID,mHpCur,mHpCnt);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 2: // Button
		{	Button_AddEvent(hWndActive,lpCtrlName,ictrlID,mHpCur,mHpCnt);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 3: // SingleList
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	SingleList_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,TRUE);}
			else SingleList_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 4: // Combo
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	Combo_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,TRUE);}
			else Combo_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 5: // Slider
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	Slider_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,TRUE);}
			else Slider_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 6: // Edit
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	Edit_AddEvent(hWndActive,lpCtrlName,ictrlID,mHpCur,mHpCnt,TRUE);}
			else Edit_AddEvent(hWndActive,lpCtrlName,ictrlID,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 7: // Check
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	Check_AddEvent(hWndActive,lpCtrlName,ictrlID,mHpCur,mHpCnt,TRUE);}
			else Check_AddEvent(hWndActive,lpCtrlName,ictrlID,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 8: // Radio
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	Radio_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,TRUE);}
			else Radio_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 9: // MultiList
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	MultiList_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,TRUE);}
			else MultiList_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 10: // TabCtrl
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	TabCtrl_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,TRUE);}
			else TabCtrl_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 11: // SingleListView
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	SListView_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,TRUE);}
			else SListView_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 12: // MultiListView
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	MListView_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,TRUE);}
			else MListView_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 13: // TreeView
		{	if((iLastID!=NO)&&(iLastID==ictrlID))
			{	AddmHpCtrlID(hWndActive,iLastTab,MAX_PART_TREE,iLastID);
				AddmHpCtrlID(hWndActive,iCtrlTab,MAX_PART_TREE,ictrlID);
			}
			if((iLastID!=ictrlID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab)))
			{	TreeView_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,wParam,mHpCur,mHpCnt,TRUE);}
			else TreeView_AddEvent(hWndActive,lpCtrlName,hWndInCtrl,wParam,mHpCur,mHpCnt,FALSE);
			iLastID=ictrlID;
			GetmHpVarStruct()->iLastCmdID=NO;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	int iToolID=(int)GetWindowLongPtr(hWndInCtrl,GWLP_ID);
			/*if((iLastID!=NO)&&(iLastID==iToolID))
			{	AddToolCtrlID(hWndActive,hWndInCtrl,iLastTab,MAX_PART_TREE);
				AddToolCtrlID(hWndActive,hWndInCtrl,iCtrlTab,MAX_PART_TREE);
			}
			if((iLastID!=iToolID)||(!mHpPak_CompareTab(iLastTab,iCtrlTab))||(GetmHpVarStruct()->iLastCmdID!=ictrlID))*/
			if((iLastID!=iToolID)||(GetmHpVarStruct()->iLastCmdID!=ictrlID))
			{	ToolBar_AddEvent(hWndActive,hWndInCtrl,ictrlID,mHpCur,mHpCnt,TRUE,pmHpID);}
			else ToolBar_AddEvent(hWndActive,hWndInCtrl,ictrlID,mHpCur,mHpCnt,FALSE,pmHpID);
			iLastID=iToolID;
			GetmHpVarStruct()->iLastCmdID=ictrlID;
            *GetmHpVarStruct()->szLastClass=0;
            GetmHpVarStruct()->iLastStyle=NO;
            GetmHpVarStruct()->pLastID->ClearValue();
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            // Check existing state on managed control
            if(pCtrlMgmt->GetStateCount())
            {   BOOL bAddEvent=TRUE;
                if((!strncmp(GetmHpVarStruct()->szLastClass,pCtrlMgmt->GetCtrlClass(),CTRL_CLASS_SIZE))&&
                   (GetmHpVarStruct()->iLastStyle==pCtrlMgmt->GetStyle())&&
                   (*GetmHpVarStruct()->pLastID==*pCtrlMgmt->GetCtrlID()))
                {   bAddEvent=FALSE; // Same control
                }
                CtrlMan_AddEvent(hWndActive,pCtrlMgmt,iPlatformID,lpCtrlName,ictrlID,mHpCur,mHpCnt,bAddEvent);
            }
            else // Do not replace previous event with this one if control has no any state
                CtrlMan_AddEvent(hWndActive,pCtrlMgmt,iPlatformID,lpCtrlName,ictrlID,mHpCur,mHpCnt,TRUE);

            iLastID=NO;
            strncpy(GetmHpVarStruct()->szLastClass,pCtrlMgmt->GetCtrlClass(),CTRL_CLASS_SIZE);
            GetmHpVarStruct()->iLastStyle=pCtrlMgmt->GetStyle();
            *GetmHpVarStruct()->pLastID=*pCtrlMgmt->GetCtrlID();
			GetmHpVarStruct()->iLastCmdID=NO;
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:
        {   break; }
	}
}

//*************************************************************************************************************PROPERTIES//
// ComCtrl_IsCustom
//-----------------------------------------------------//
void ComCtrl_IsCustom(CEventCell* CurCell,int &pLastID,BOOL &bCustom)
{	switch(CurCell->mHpTask->GetmHpID())
	{	// mHpCtrl's custom? ------------------------------------------------------------------------------------------**//
		case 0:	// ChangeExe
		{	ChangeExe_IsCustom(CurCell,pLastID,bCustom);
			break;
		}
		case 1:	// Menu
		{	Menu_IsCustom(pLastID);
		}
		case 2:	// Button
		{	Button_IsCustom(pLastID);
			break;
		}
		case 3:	// SingleList
		{	SingleList_IsCustom(bCustom);
			break;
		}
		case 4: // Combo
		{	Combo_IsCustom(bCustom);
			break;
		}
		case 5: // Slider
		{	Slider_IsCustom(bCustom);
			break;
		}
		case 6: // Edit
		{	Edit_IsCustom(CurCell,bCustom);
			break;
		}
		case 7: // Check
		{	Check_IsCustom(bCustom);
			break;
		}
		case 8: // Radio
		{	Radio_IsCustom(CurCell,bCustom);
			break;
		}
		case 9:	// MultiList
		{	MultiList_IsCustom(bCustom);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_IsCustom(bCustom);
			break;
		}
		case 11: // SingleListView
		{	SListView_IsCustom(bCustom);
			break;
		}
		case 12: // MultiListView
		{	MListView_IsCustom(bCustom);
			break;
		}
		case 13: // TreeView
		{	TreeView_IsCustom(bCustom);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_IsCustom(CurCell,bCustom,pLastID);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            CtrlMan_IsCustom(bCustom,CurCell);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{	break;}
	}
}

//-----------------------------------------------------//
// ComCtrl_StrState
//-----------------------------------------------------//
void ComCtrl_StrState(CEventCell* CurCell,BOOL bWriteOp,char* lpCtrlState,int StateSize)
{	switch(CurCell->mHpTask->GetmHpID())
	{	// mHpCtrl's state string -------------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
		case 1: // Menu
		{	Menu_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
		case 2: // Bouton
		{	Button_StrState(lpCtrlState,StateSize);
			break;
		}
		case 3: // SingleList
		{	SingleList_StrState(CurCell,lpCtrlState,StateSize);
			break;
		}
		case 4: // Combo
		{	Combo_StrState(CurCell,lpCtrlState,StateSize);
			break;
		}
		case 5: // Slider
		{	Slider_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
		case 6: // Edit
		{	Edit_StrState(CurCell,lpCtrlState,StateSize);
			break;
		}
		case 7: // Check
		{	Check_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
		case 8: // Radio
		{	Radio_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
		case 9: // MultiList
		{	MultiList_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_StrState(CurCell,lpCtrlState,StateSize);
			break;
		}
		case 11: // SingleListView
		{	SListView_StrState(CurCell,lpCtrlState,StateSize);
			break;
		}
		case 12: // MultiListView
		{	MListView_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
		case 13: // TreeView
		{	TreeView_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////
            
            CtrlMan_StrState(CurCell,bWriteOp,lpCtrlState,StateSize);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{	break;}
	}
}

//-----------------------------------------------------//
// ComCtrl_ParamType
//-----------------------------------------------------//
int ComCtrl_ParamType(CEventCell* CurCell)
{	switch(CurCell->mHpTask->GetmHpID())
	{	// mHpCtrl's param type ---------------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	return ChangeExe_ParamType(CurCell);
		}
		case 1: // Menu
		{	return Menu_ParamType();
		}
		case 2: // Button
		{	return Button_ParamType();
		}
		case 3: // SingleList
		{	return SingleList_ParamType();
		}
		case 4: // Combo
		{	return Combo_ParamType();
		}
		case 5: // Slider
		{	return Slider_ParamType();
		}
		case 6: // Edit
		{	return Edit_ParamType();
		}
		case 7: // Check
		{	return Check_ParamType();
		}
		case 8: // Radio
		{	return Radio_ParamType();
		}
		case 9: // MultiList
		{	return MultiList_ParamType();
		}
		case 10: // TabCtrl
		{	return TabCtrl_ParamType();
		}
		case 11: // SingleListView
		{	return SListView_ParamType();
		}
		case 12: // MultiListView
		{	return MListView_ParamType();
		}
		case 13: // TreeView
		{	return TreeView_ParamType();
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	return ToolBar_ParamType(CurCell->mHpTask->GetmHpID());
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////
            
            return CtrlMan_ParamType(CurCell);
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{	break;}
	}
	return NO;
}

//-----------------------------------------------------//
// ComCtrl_DefaultBound
//-----------------------------------------------------//
void ComCtrl_DefaultBound(CEventCell* CurEvnt,char* lpBound,int BoundSize)
{	switch(CurEvnt->mHpTask->GetmHpID())
	{	// mHpCtrl's default limite -----------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_DefaultBound(CurEvnt,lpBound,BoundSize);
			break;
		}
		case 1: // Menu
		{	Menu_DefaultBound();
			break;
		}
		case 2: // Button
		{	Button_DefaultBound();
			break;
		}
		case 3: // SingleList
		{	SingleList_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 4: // Combo
		{	Combo_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 5: // Slider
		{	Slider_DefaultBound(CurEvnt,lpBound,BoundSize);
			break;
		}
		case 6: // Edit
		{	Edit_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 7: // Check
		{	Check_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 8: // Radio
		{	Radio_DefaultBound(CurEvnt,lpBound,BoundSize);
			break;
		}
		case 9: // MultiList
		{	MultiList_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 11: // SingleListView
		{	SListView_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 12: // MultiListView
		{	MListView_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 13: // TreeView
		{	TreeView_DefaultBound(lpBound,BoundSize);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_DefaultBound(CurEvnt,lpBound,BoundSize);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////
            
            CtrlMan_DefaultBound(CurEvnt,lpBound,BoundSize);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{ break;}
	}
}

//-----------------------------------------------------//
// ComCtrl_IsCustomized
//-----------------------------------------------------//
BOOL ComCtrl_IsCustomized(CEventCell* CurCell,int Index,BOOL bcustom)
{	switch(CurCell->mHpTask->GetmHpID())
	{	// mHpCtrl's !custom? -----------------------------------------------------------------------------------------**//
		case 0: // ChangExe
		{	return ChangeExe_IsCustomized(CurCell);
		}
		case 1: // Menu
		{	return Menu_IsCustomized(CurCell,Index,bcustom);
		}
		case 2: // Button
		{	return Button_IsCustomized();
		}
		case 3: // SingleList
		{	return SingleList_IsCustomized();
		}
		case 4: // Combo
		{	return Combo_IsCustomized();
		}
		case 5: // Slider
		{	return Slider_IsCustomized();
		}
		case 6: // Edit
		{	return Edit_IsCustomized(CurCell,bcustom);
		}
		case 7: // Check
		{	return Check_IsCustomized();
		}
		case 8: // Radio
		{	return Radio_IsCustomized(CurCell,Index,bcustom);
		}
		case 9: // MultiList
		{	return MultiList_IsCustomized();
		}
		case 10: // TabCtrl
		{	return TabCtrl_IsCustomized();
		}
		case 11: // SingleListView
		{	return SListView_IsCustomized();
		}
		case 12: // MultiListView
		{	return MListView_IsCustomized();
		}
		case 13: // TreeView
		{	return TreeView_IsCustomized();
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	return ToolBar_IsCustomized(CurCell,Index,bcustom);
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            return CtrlMan_IsCustomized(CurCell);
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{break;}
	}
	return FALSE;
}

//-----------------------------------------------------//
// ComCtrl_IsNoStateMsg
//-----------------------------------------------------//
BOOL ComCtrl_IsNoStateMsg(CEventCell* CurCell)
{	switch(CurCell->mHpTask->GetmHpID())
	{	// mHpCtrl's no state message ---------------------------------------------------------------------------------**//
		case 0: // ChangExe
		{	return ChangeExe_IsNoStateMsg();
		}
		case 1: // Menu
		{	return Menu_IsNoStateMsg();
		}
		case 2: // Button
		{	return Button_IsNoStateMsg();
		}
		case 3: // SingleList
		{	return SingleList_IsNoStateMsg();
		}
		case 4: // Combo
		{	return Combo_IsNoStateMsg(CurCell);
		}
		case 5: // Slider
		{	return Slider_IsNoStateMsg();
		}
		case 6: // Edit
		{	return Edit_IsNoStateMsg();
		}
		case 7: // Check
		{	return Check_IsNoStateMsg();
		}
		case 8: // Radio
		{	return Radio_IsNoStateMsg();
		}
		case 9: // MultiList
		{	return MultiList_IsNoStateMsg();
		}
		case 10: // TabCtrl
		{	return TabCtrl_IsNoStateMsg();
		}
		case 11: // SingleListView
		{	return SListView_IsNoStateMsg();
		}
		case 12: // MultiListView
		{	return MListView_IsNoStateMsg();
		}
		case 13: // TreeView
		{	return TreeView_IsNoStateMsg();
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	return ToolBar_IsNoStateMsg(CurCell);
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            return CtrlMan_IsNoStateMsg(CurCell);
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{break;}
	}
	return FALSE;
}

//-----------------------------------------------------//
// ComCtrl_GetHtmlTag
//-----------------------------------------------------//
void ComCtrl_GetHtmlTag(CEventCell* CurCell,char* lpHtmlTag,UINT iMaxSize,int iCustIndex)
{	*lpHtmlTag=0;
    switch(CurCell->mHpTask->GetmHpID())
	{	// mHpCtrl's HTML tag -----------------------------------------------------------------**//
		case 0: // ChangExe
		{	ChangeExe_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 1: // Menu
		{	Menu_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 2: // Button
		{	//Button_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 3: // SingleList
		{	SingleList_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 4: // Combo
		{	Combo_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 5: // Slider
		{	//Slider_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 6: // Edit
		{	Edit_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 7: // Check
		{	//Check_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 8: // Radio
		{	//Radio_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 9: // MultiList
		{	MultiList_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 10: // TabCtrl
		{	TabCtrl_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 11: // SingleListView
		{	SListView_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 12: // MultiListView
		{	MListView_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 13: // TreeView
		{	TreeView_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            CtrlMan_GetHtmlTag(CurCell,lpHtmlTag,iMaxSize,iCustIndex);
            break;
        }
		//-------------------------------------------------------------------------------------**//
		default: { break; }
	}
}

//***************************************************************************************************************CFGTOOLS//
// ComCtrl_ShowCfgTools
//-----------------------------------------------------//
void ComCtrl_ShowCfgTools(HWND hWndActive)
{	if(GetmHpVarStruct()->EventList->IsCtrlState(GetmHpVarStruct()->iCtrlSel,FALSE))
	{	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
		{	// mHpCtrl's state tools ----------------------------------------------------------------------------------**//
			case 0: // ChangeExe
			{	ChangeExe_ShowCfgTools(hWndActive);
				break;
			}
			case 3: // SingleList
			{	SingleList_ShowCfgTools(hWndActive);
				break;
			}
			case 4: // Combo
			{	Combo_ShowCfgTools(hWndActive);
				break;
			}
			case 5: // Slider
			{	Slider_ShowCfgTools(hWndActive);
				break;
			}
			case 6: // Edit
			{	Edit_ShowCfgTools(hWndActive);
				break;
			}
			case 7: // Check
			{	Check_ShowCfgTools(hWndActive);
				break;
			}
			case 8: // Radio
			{	Radio_ShowCfgTools(hWndActive);
				break;
			}
			case 9: // MultiList
			{	MultiList_ShowCfgTools(hWndActive);
				break;
			}
			case 10: // TabCtrl
			{	TabCtrl_ShowCfgTools(hWndActive);
				break;
			}
			case 11: // SingleListView
			{	SListView_ShowCfgTools(hWndActive);
				break;
			}
			case 12: // MultiListView
			{	MListView_ShowCfgTools(hWndActive);
				break;
			}
			case 13: // TreeView
			{	TreeView_ShowCfgTools(hWndActive);
				break;
			}
			case 14: // ToolBar - Button
			case 15: // ToolBar - Checkbox
			case 16: // ToolBar - Radiobox
			case 17: // ToolBar - Combobox
			case 18: // ToolBar - DropDown
			{	ToolBar_ShowCfgTools(hWndActive);
				break;
			}
            case MHPID_CTRL_MAN:
            {   ////// mHelp Control Management /////////////////////////////////

                CtrlMan_ShowCfgTools(hWndActive);
                break;
            }
			//---------------------------------------------------------------------------------------------------------**//
			default:{ break;}
		}
	}
}

//-----------------------------------------------------//
// ComCtrl_HideCfgTools
//-----------------------------------------------------//
void ComCtrl_HideCfgTools(HWND hWndActive)
{	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	// mHpCtrl's state tools --------------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_HideCfgTools(hWndActive);
			break;
		}
		case 3: // SingleList
		{	SingleList_HideCfgTools(hWndActive);
			break;
		}
		case 4: // Combo
		{	Combo_HideCfgTools(hWndActive);
			break;
		}
		case 5: // Slider
		{	Slider_HideCfgTools(hWndActive);
			break;
		}
		case 6: // Edit
		{	Edit_HideCfgTools(hWndActive);
			break;
		}
		case 7: // Check
		{	Check_HideCfgTools(hWndActive);
			break;
		}
		case 8: // Radio
		{	Radio_HideCfgTools(hWndActive);
			break;
		}
		case 9: // MultiList
		{	MultiList_HideCfgTools(hWndActive);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_HideCfgTools(hWndActive);
			break;
		}
		case 11: // SingleListView
		{	SListView_HideCfgTools(hWndActive);
			break;
		}
		case 12: // MultiListView
		{	MListView_HideCfgTools(hWndActive);
			break;
		}
		case 13: // TreeView
		{	TreeView_HideCfgTools(hWndActive);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_HideCfgTools(hWndActive);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            CtrlMan_HideCfgTools(hWndActive);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{ break;}
	}
}

//-----------------------------------------------------//
// ComCtrl_SaveCfg
//-----------------------------------------------------//
void ComCtrl_SaveCfg(HWND hWndActive,BOOL &bError)
{	bError=FALSE;
	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	// mHpCtrl's state cfg writting -------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_SaveCfg(hWndActive);
			break;
		}
		case 3: // SingleList
		{	SingleList_SaveCfg(hWndActive);
			break;
		}
		case 4: // Combo
		{	Combo_SaveCfg(hWndActive);
			break;
		}
		case 5: // Slider
		{	Slider_SaveCfg(hWndActive);
			break;
		}
		case 6: // Editbox
		{	Edit_SaveCfg(hWndActive);
			break;
		}
		case 7: // Checkbox
		{	Check_SaveCfg(hWndActive);
			break;
		}
		case 8: // Radiobox
		{	Radio_SaveCfg(hWndActive,bError);
			break;
		}
		case 9: // MultiList
		{	MultiList_SaveCfg(hWndActive);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_SaveCfg(hWndActive);
			break;
		}
		case 11: // SingleListView
		{	SListView_SaveCfg(hWndActive);
			break;
		}
		case 12: // MultiListView
		{	MListView_SaveCfg(hWndActive);
			break;
		}
		case 13: // TreeView
		{	TreeView_SaveCfg(hWndActive);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_SaveCfg(hWndActive,bError);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            CtrlMan_SaveCfg(hWndActive);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{ break;}
	}
}

//-----------------------------------------------------//
// ComCtrl_LoadCfg
//-----------------------------------------------------//
void ComCtrl_LoadCfg(HWND hWndActive,HWND hWndTabCtrl,HWND hDecritWnd,HWND hBoundWnd)
{	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	// mHpCtrl's state cfg reading --------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_LoadCfg(hWndActive);
			break;
		}
		case 3: // SingleList
		{	SingleList_LoadCfg(hWndActive);
			break;
		}
		case 4: // Combo
		{	Combo_LoadCfg(hWndActive);
			break;
		}
		case 5: // Slider
		{	Slider_LoadCfg(hWndActive);
			break;
		}
		case 6: // Edit
		{	Edit_LoadCfg(hWndActive);
			break;
		}
		case 7: // Checkbox
		{	Check_LoadCfg(hWndActive);
			break;
		}
		case 8: // RadioBox
		{	Radio_LoadCfg(hWndActive,hWndTabCtrl,hDecritWnd,hBoundWnd);
			break;
		}
		case 9: // MultiList
		{	MultiList_LoadCfg(hWndActive);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_LoadCfg(hWndActive);
			break;
		}
		case 11: // SingleListView
		{	SListView_LoadCfg(hWndActive);
			break;
		}
		case 12: // MultiListView
		{	MListView_LoadCfg(hWndActive);
			break;
		}
		case 13: // TreeView
		{	TreeView_LoadCfg(hWndActive);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_LoadCfg(hWndActive,hWndTabCtrl,hDecritWnd,hBoundWnd);
			break;
		}
		//-------------------------------------------------------------------------------------------------------------**//
		default:{ break;}
	}
}

//-----------------------------------------------------//
// ComCtrl_GetInfosCfg
//-----------------------------------------------------//
void ComCtrl_GetInfosCfg(HWND hWndDlgCfg,HWND hWndApply,HWND hWndIcon,HWND hWndProd,HWND hWndVers,HWND hWndDescrip,
                         HWND hWndFmCtrl,HWND hWndViewNext,HWND hWndNoView,HWND hWndCtrlWait)
{	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	// mHpCtrl's Icon/Name/ProductName/Version/Description---------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl,hWndViewNext,hWndNoView,hWndCtrlWait);
			break;
		}
		case 1: // Menu
		{	Menu_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 2: // Button
		{	Button_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 3: // SingleList
		{	SingleList_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 4: // Combo
		{	Combo_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 5: // Slider
		{	Slider_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 6: // Edit
		{	Edit_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 7: // Check
		{	Check_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 8: // Radio
		{	Radio_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 9: // MultiList
		{	MultiList_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 11: // SingleListView
		{	SListView_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 12: // MultiListView
		{	MListView_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 13: // TreeView
		{	TreeView_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_GetInfosCfg(hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            CtrlMan_GetInfosCfg(hWndDlgCfg,hWndApply,hWndIcon,hWndProd,hWndVers,hWndDescrip,hWndFmCtrl);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{	break;}
	}
}

//-----------------------------------------------------//
// ComCtrl_ProcCfg
//-----------------------------------------------------//
BOOL ComCtrl_ProcCfg(HWND hWndActive,UINT cfgMessage,WPARAM cfgWParam,LPARAM cfgLParam,HWND hWndDecrit,
					 HWND hWndLimite,HWND hWndChange)
{	switch(GetmHpVarStruct()->EventList->GetmHpTask(GetmHpVarStruct()->iCtrlSel,FALSE)->GetmHpID())
	{	// mHpCtrl's dialog process -----------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	return ChangeExe_ProcCfg(cfgMessage,cfgWParam,hWndChange);
		}
		case 3: // SingleList
		{	return SingleList_ProcCfg(cfgMessage,cfgWParam,hWndChange);
		}
		case 4: // Combo
		{	return Combo_ProcCfg(cfgMessage,cfgWParam,hWndChange);
		}
		case 5: // Slider
		{	return Slider_ProcCfg(hWndActive,cfgMessage,cfgWParam,cfgLParam,hWndChange);
		}
		case 6: // Edit
		{	return Edit_ProcCfg(hWndActive,cfgMessage,cfgWParam,hWndChange);
		}
		case 7: // Check
		{	return Check_ProcCfg(hWndActive,cfgMessage,cfgWParam,cfgLParam,hWndChange);
		}
		case 8: // Radio
		{	return Radio_ProcCfg(hWndActive,cfgMessage,cfgWParam,cfgLParam,hWndDecrit,hWndLimite,hWndChange);
		}
		case 9: // MultiList
		{	return MultiList_ProcCfg(hWndActive,cfgMessage,cfgWParam,hWndChange);
		}
		case 10: // TabCtrl
		{	return TabCtrl_ProcCfg(cfgMessage,cfgWParam,hWndChange);
		}
		case 11: // SingleListView
		{	return SListView_ProcCfg(cfgMessage,cfgWParam,hWndChange);
		}
		case 12: // MultiListView
		{	return MListView_ProcCfg(hWndActive,cfgMessage,cfgWParam,hWndChange);
		}
		case 13: // TreeView
		{	return TreeView_ProcCfg(hWndActive,cfgMessage,cfgWParam,hWndChange);
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	return ToolBar_ProcCfg(hWndActive,cfgMessage,cfgWParam,cfgLParam,hWndDecrit,hWndLimite,hWndChange);
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            return CtrlMan_ProcCfg(hWndActive,cfgMessage);
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:{	break;}
	}
	return FALSE;
}

//*******************************************************************************************************************HTML//
// ComCtrl_SetStartPostData
//-----------------------------------------------------//
void ComCtrl_SetStartPostData(const std::string& strPostData)
{   StartPostData=strPostData;
}

//****************************************************************************************************************PROCESS//
// ComCtrl_CaptureMsg
//-----------------------------------------------------//
void ComCtrl_CaptureMsg(HWND hWndActive,UINT pMessage,WPARAM pwParam,LPARAM plParam,BOOL &pUserAction,int pmHpOp,
						HWND hWndTool,char* ExeNameApp,CEventListe* Evntobj,int &pCaptRes,UINT &pmHpLastID,
						int pmHpCur,int pmHpCnt)
{	if(!pUserAction) return;
	pCaptRes=6; // No Process

    ////// mHelp Control Management /////////////////////////////////////////
    ImHpCtrlMgmt* CaptCtrlMgmt=NULL;
    UINT iCaptPlatformID=0;
    GetmHpVarStruct()->CtrlMgmtList->CaptureEvent(hWndActive,pMessage,pwParam,plParam,CaptCtrlMgmt,iCaptPlatformID);
    if(CaptCtrlMgmt!=NULL)
    {   CtrlMan_CaptureMsg(hWndActive,
                           pwParam,
                           plParam,
                           CaptCtrlMgmt,
                           iCaptPlatformID,
                           hWndTool,
                           pmHpOp,
                           Evntobj,
                           ExeNameApp,
                           pmHpLastID,
                           pmHpCur,
                           pmHpCnt,
                           pUserAction,
                           pCaptRes);
        return;
    } /////////////////////////////////////////////////////////////////////////

    int iehpID;
    char lpCaptCtrlName[MAX_CTRL_NAME]={0};
	switch(pMessage)
	{	// mHpCtrl's capture --------------------------------------------------------------------------------------------//
		case WM_HSCROLL:
		case WM_VSCROLL:
		{	if(plParam!=NULL)
			{	ComCtrl_GetmHpID(hWndActive,(HWND)plParam,Evntobj,(UINT)GetDlgCtrlID((HWND)plParam),iehpID,lpCaptCtrlName);
                switch(iehpID)
				{	case 5: // Slider /////////////////////////////////////////////////////////////////////////////////////
					{	Slider_CaptureMsg(hWndActive,
                                          lpCaptCtrlName,
										  plParam,
										  hWndTool,
										  pmHpOp,
										  Evntobj,
										  ExeNameApp,
										  pmHpLastID,
										  pmHpCur,
										  pmHpCnt,
										  pUserAction,
										  pCaptRes,
										  TRUE);
						///////////////////////////////////////////////////////////////////////////////////////////////////
						break;
					}
					case NO:
					{	SendMessage(hWndTool,WM_CLOSE,0,0);
						StopmHelp(hWndActive,TRUE,TRUE);
                        char lpCtrlClass[MAX_RS_SIZE]={0};
						GetClassName((HWND)plParam,lpCtrlClass,MAX_RS_SIZE);
						mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_CREATE_TASK1,
										  lpCtrlClass,MAX_RS_SIZE,IDS_CREATE_TASK2,MB_ICONEXCLAMATION);
						break;
					}
				}
			}
			break;
		}
		case WM_NOTIFY:
		{	if(((LPNMHDR)plParam)->hwndFrom!=NULL)
			{	ComCtrl_GetmHpID(hWndActive,((LPNMHDR)plParam)->hwndFrom,Evntobj,(UINT)((LPNMHDR)plParam)->idFrom,iehpID,lpCaptCtrlName);
                switch(iehpID)
				{	case 18: // ToolBar - DropDown ////////////////////////////////////////////////////////////////////////
					{	ToolBar_CaptureMsg(hWndActive,
										   ((LPNMHDR)plParam)->hwndFrom,
										   plParam,
										   pwParam,
										   hWndTool,
										   pmHpOp,
										   Evntobj,
										   ExeNameApp,
										   pmHpLastID,
										   pmHpCur,
										   pmHpCnt,
										   iehpID,
										   pUserAction,
										   pCaptRes);
						///////////////////////////////////////////////////////////////////////////////////////////////////
						break;
					}
					case 13: // TreeView //////////////////////////////////////////////////////////////////////////////////
					{	TreeView_CaptureMsg(hWndActive,
                                            lpCaptCtrlName,
										    pwParam,
										    plParam,
										    hWndTool,
										    pmHpOp,
										    Evntobj,
										    ExeNameApp,
										    pmHpLastID,
										    pmHpCur,
										    pmHpCnt,
											pUserAction,
										    pCaptRes);
						///////////////////////////////////////////////////////////////////////////////////////////////////
						break;
					}
					case 11: // SingleListView ////////////////////////////////////////////////////////////////////////////
					case 12: // MultiListView
					{	ListView_CaptureMsg(hWndActive,
                                            lpCaptCtrlName,
                                            pwParam,
										    plParam,
										    hWndTool,
										    pmHpOp,
										    Evntobj,
										    ExeNameApp,
										    pmHpLastID,
										    pmHpCur,
										    pmHpCnt,
											iehpID,
										    pUserAction,
										    pCaptRes);
						///////////////////////////////////////////////////////////////////////////////////////////////////
						break;
					}
					case 10: // TabCtrl ///////////////////////////////////////////////////////////////////////////////////
					{   TabCtrl_CaptureMsg(hWndActive,
                                           lpCaptCtrlName,
                                           ((LPNMHDR)plParam)->hwndFrom,
										   pwParam,
										   plParam,
										   hWndTool,
										   pmHpOp,
										   Evntobj,
										   ExeNameApp,
										   pmHpLastID,
										   pmHpCur,
										   pmHpCnt,
										   pUserAction,
										   pCaptRes);
						///////////////////////////////////////////////////////////////////////////////////////////////////
						break;
					}
					case 5: // Slider /////////////////////////////////////////////////////////////////////////////////////
					{	Slider_CaptureMsg(hWndActive,
                                          lpCaptCtrlName,
										  plParam,
										  hWndTool,
										  pmHpOp,
										  Evntobj,
										  ExeNameApp,
										  pmHpLastID,
										  pmHpCur,
										  pmHpCnt,
										  pUserAction,
										  pCaptRes,
										  FALSE);
						///////////////////////////////////////////////////////////////////////////////////////////////////
						break;
					}
					case NO:
					{	SendMessage(hWndTool,WM_CLOSE,0,0);
						StopmHelp(hWndActive,TRUE,TRUE);
                        char lpCtrlClass[MAX_RS_SIZE]={0};
						GetClassName(((LPNMHDR)plParam)->hwndFrom,lpCtrlClass,MAX_RS_SIZE);
						mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_CREATE_TASK1,
										  lpCtrlClass,MAX_RS_SIZE,IDS_CREATE_TASK2,MB_ICONEXCLAMATION);
						break;
					}
				}
			}
			break;
		}
		case WM_COMMAND:
		{	ComCtrl_GetmHpID(hWndActive,(HWND)plParam,Evntobj,(UINT)((int)LOWORD(pwParam)),iehpID,lpCaptCtrlName);
			switch(iehpID)
			{	case 6: // Edit ///////////////////////////////////////////////////////////////////////////////////////
				{	Edit_CaptureMsg(hWndActive,
									lpCaptCtrlName,
                                    pwParam,
									hWndTool,
									pmHpOp,
									Evntobj,
									ExeNameApp,
									pmHpLastID,
									pmHpCur,
									pmHpCnt,
									pUserAction,
									pCaptRes);
					///////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				case 2: // Button /////////////////////////////////////////////////////////////////////////////////////
				case 7: // Check
				case 8: // Radio
                {   Button_CaptureMsg(hWndActive,
                                      lpCaptCtrlName,
                                      (HWND)plParam,
									  pwParam,
									  hWndTool,
									  pmHpOp,
									  Evntobj,
									  ExeNameApp,
									  pmHpLastID,
									  pmHpCur,
									  pmHpCnt,
									  iehpID,
									  pUserAction,
									  pCaptRes);
					///////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				case 1: // Menu ///////////////////////////////////////////////////////////////////////////////////////
				{	Menu_CaptureMsg(hWndActive,
                                    lpCaptCtrlName,
									pwParam,
									hWndTool,
									pmHpOp,
									Evntobj,
									ExeNameApp,
									pmHpLastID,
									pmHpCur,
									pmHpCnt,
									iehpID,
									pUserAction,
									pCaptRes);
					///////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				case 4: // Combobox ///////////////////////////////////////////////////////////////////////////////////
				{	Combo_CaptureMsg(hWndActive,
                                     lpCaptCtrlName,
                                     (HWND)plParam,
									 pwParam,
									 hWndTool,
									 pmHpOp,
									 Evntobj,
									 ExeNameApp,
									 pmHpLastID,
									 pmHpCur,
									 pmHpCnt,
									 pUserAction,
									 pCaptRes);
					///////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				case 3: // SingleList /////////////////////////////////////////////////////////////////////////////////
				case 9: // MultiList
				{	List_CaptureMsg(hWndActive,
									lpCaptCtrlName,
                                    (HWND)plParam,
									pwParam,
									hWndTool,
									pmHpOp,
									Evntobj,
									ExeNameApp,
									pmHpLastID,
									pmHpCur,
									pmHpCnt,
									iehpID,
									pUserAction,
									pCaptRes);
					///////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				case 14: // ToolBar - Button //////////////////////////////////////////////////////////////////////////
				case 15: // ToolBar - Checkbox
				case 16: // ToolBar - Radiobox
				case 17: // ToolBar - Combobox
				case 18: // ToolBar - DropDown (Menu)
				{	ToolBar_CaptureMsg(hWndActive,
									   (HWND)plParam,
									   plParam,
									   pwParam,
									   hWndTool,
									   pmHpOp,
									   Evntobj,
									   ExeNameApp,
									   pmHpLastID,
									   pmHpCur,
									   pmHpCnt,
									   iehpID,
									   pUserAction,
									   pCaptRes);
					///////////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}
				case NO:
				{	SendMessage(hWndTool,WM_CLOSE,0,0);
					StopmHelp(hWndActive,TRUE,TRUE);
                    char lpCtrlClass[MAX_RS_SIZE]={0};
					GetClassName((HWND)plParam,lpCtrlClass,MAX_RS_SIZE);
					mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,IDS_CREATE_TASK1,
										lpCtrlClass,MAX_RS_SIZE,IDS_CREATE_TASK2,MB_ICONEXCLAMATION);
					break;
				}
			}
			break;
		}
		//---------------------------------------------------------------------------------------------------------------//
		default:
		{	break; }
	}
}

//-----------------------------------------------------//
// ComCtrl_mHpProcess
//-----------------------------------------------------//
void ComCtrl_mHpProcess(HWND hWndActive,BOOL &ChangeWnd,int iWndCur,int iWndCnt)
{	switch(GetmHpVarStruct()->CourantTask->GetmHpID())
	{	// mHpCtrl's process ------------------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	ChangeExe_mHpProcess(hWndActive);
			break;
		}
		case 1: // Menu
		{	Menu_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 2: // Bouton
		case 7: // CheckBox
		case 8: // RadioBox
		{	Button_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 3: // SingleList
		{	SingleList_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 9: // MultiList
		{	MultiList_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 4: // ComboBox
		{	Combo_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 5: // Slider
		{	Slider_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 6: // EditBox
		{	Edit_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 10: // TabCtrl
		{	TabCtrl_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 11: // SingleListView
		{	SListView_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 12: // MultiListView
		{	MListView_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 13: // TreeView
		{	TreeView_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	ToolBar_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
			break;
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            CtrlMan_mHpProcess(hWndActive,ChangeWnd,iWndCur,iWndCnt);
            break;
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:
		{	break;}
	}
}

//-----------------------------------------------------//
// ComCtrl_StartProc
//-----------------------------------------------------//
int ComCtrl_StartProc(HWND hWndActive)
{	// WARNING *********************************************************************************//
	// In the StartProc we must not change the mHpStep when the control is not ready ***********//
	//******************************************************************************************//
	GetmHpVarStruct()->iSkipSnd=0;
	GetmHpVarStruct()->AscensSens=0;
	switch(GetmHpVarStruct()->CourantTask->GetmHpID())
	{	// mHpCtrl's start process ------------------------------------------------------------------------------------**//
		case 0: // ChangeExe
		{	if(!ChangeExe_StartProc(hWndActive)) return 0;
			return 2;
		}
		case 1: // Menu
		{	return ((int)Menu_StartProc(hWndActive));
		}
		case 2: // Bouton
		case 7: // CheckBox
		case 8: // RadioBox
		{	return ((int)Button_StartProc(hWndActive));
		}
		case 3: // SingleList
		{	return ((int)SingleList_StartProc(hWndActive));	
		}
		case 4: // Combo
		{	return ((int)Combo_StartProc(hWndActive));
		}
		case 5: // Slider
		{	return ((int)Slider_StartProc(hWndActive));
		}
		case 6: // Edit
		{	return ((int)Edit_StartProc(hWndActive));
		}
		case 9: // Multi
		{	return ((int)MultiList_StartProc(hWndActive));
		}
		case 10: // TabCtrl
		{	return ((int)TabCtrl_StartProc(hWndActive));
		}
		case 11: // SingleListView
		{	return ((int)SListView_StartProc(hWndActive));
		}
		case 12: // MultiListView
		{	return ((int)MListView_StartProc(hWndActive));
		}
		case 13: // TreeView
		{	return ((int)TreeView_StartProc(hWndActive));
		}
		case 14: // ToolBar - Button
		case 15: // ToolBar - Checkbox
		case 16: // ToolBar - Radiobox
		case 17: // ToolBar - Combobox
		case 18: // ToolBar - DropDown
		{	return ((int)ToolBar_StartProc(hWndActive));
		}
        case MHPID_CTRL_MAN:
        {   ////// mHelp Control Management /////////////////////////////////

            return ((int)CtrlMan_StartProc(hWndActive));
        }
		//-------------------------------------------------------------------------------------------------------------**//
		default:
		{	break;}
	}
	return 0;
}

//**************************************************************************************************************TIMERPROC//
// ComCtrl_TimerProc
//-----------------------------------------------------//
void ComCtrl_TimerProc(HWND hWndActive,UINT pTimerID)
{	switch(pTimerID)
	{	// mHpCtrl'sTimer Process -------------------------------------------------------------------------------------**//
		case ID_TIMER_MENU:
		{	KillTimer(hWndActive,ID_TIMER_MENU);
			Menu_TimerProc(hWndActive);				
			break;
		}
		case ID_TIMER_LISTE:
		{	KillTimer(hWndActive,ID_TIMER_LISTE);
			List_TimerProc(hWndActive);
			break;
		}
		case ID_TIMER_EDIT:
		{	KillTimer(hWndActive,ID_TIMER_EDIT);
			Edit_TimerProc(hWndActive);
			break;
		}
		case ID_TIMER_COMBO:
		{	KillTimer(hWndActive,ID_TIMER_COMBO);
			Combo_TimerProc(hWndActive);
			break;
		}
        ////// mHelp Control Management /////////////////////////////////////

		case ID_TIMER_CTRLMAN:
		{	KillTimer(hWndActive,ID_TIMER_CTRLMAN);
			CtrlMan_TimerProc(hWndActive);
			break;
		}
		//-------------------------------------------------------------------------------------------------------------**//
		default:
		{	break;}
	}
}

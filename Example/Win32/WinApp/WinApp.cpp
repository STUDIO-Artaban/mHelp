// WinApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "winapp.h"
#include <stdlib.h>
#include <stdio.h>
#include <windowsx.h>
#include <commctrl.h>

#include <mHelp.h>

//---------------------------------------------//
BOOL BoolTab[24]={FALSE,TRUE,TRUE,TRUE,FALSE,FALSE,TRUE,FALSE,FALSE,FALSE,
					FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,
					FALSE,FALSE,FALSE,FALSE,FALSE};
HINSTANCE hInst;
long SliderPos=3L;
int IndexList=1;
int IndexCbo=2;
BOOL bCheck1=TRUE;
BOOL bCheck2=FALSE;
BOOL bCheck3=TRUE;
BOOL bCheck4=FALSE;
BOOL bExtend=FALSE;
int iCurYScroll=0;
//---------------------------------------------//

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    // TODO: Place code here.
	FARPROC funProp;
	hInst=hInstance;
	funProp=MakeProcInstance((FARPROC)MainDlgProc,hInstance);
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG),NULL,(DLGPROC)funProp); 
	FreeProcInstance(funProp);
	return 0;
}

void ScrollCtrls(HWND hwndDlg)
{
    MoveWindow(GetDlgItem(hwndDlg,IDC_ENGHELP),6,235-iCurYScroll,92,30,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_BUTTON1),6,199-iCurYScroll,92,30,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_STATIC_LIST),6,2-iCurYScroll,323,190,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_LIST1),18,24-iCurYScroll,146,156,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_LIST2),173,24-iCurYScroll,146,156,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDOK),338,8-iCurYScroll,92,23,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDCANCEL),338,40-iCurYScroll,92,23,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_STATIC_RADIO),335,68-iCurYScroll,95,124,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_RADIO1),348,89-iCurYScroll,70,16,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_RADIO2),348,111-iCurYScroll,70,16,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_RADIO3),348,132-iCurYScroll,70,16,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_RADIO4),348,154-iCurYScroll,70,16,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_CHECK1),348,171-iCurYScroll,70,16,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_STATIC_SLIDER),107,196-iCurYScroll,322,73,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_SLIDER1),116,217-iCurYScroll,237,39,TRUE);
    MoveWindow(GetDlgItem(hwndDlg,IDC_EDIT1),361,220-iCurYScroll,55,28,TRUE);
    RedrawWindow(hwndDlg,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
}

BOOL CALLBACK MainDlgProc(HWND hwndMain,UINT message,WPARAM wParam,LPARAM lParam)
{	FARPROC ChildProp;
	static HWND hWndList1=NULL;
	static HWND hWndList2=NULL;
	static HWND hWndSlider=NULL;
	static HWND hWndEdit=NULL;
	switch (message)
    {	case WM_INITDIALOG:
		{	hWndList1=GetDlgItem(hwndMain,IDC_LIST1);
			hWndList2=GetDlgItem(hwndMain,IDC_LIST2);
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel1"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel2"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel3"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel4"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel5"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel6"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel7"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel8"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel9"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel10"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel11"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel12"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel13"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel14"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel15"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel16"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel17"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel18"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel19"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel20"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel21"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel22"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel23"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel24"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel25"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel26"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel27"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel28"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel29"));
			SendMessage(hWndList1,LB_ADDSTRING,0,(LPARAM)("MultiSel30"));
			for(int i=0;i<24;i++) SendMessage(hWndList1,LB_SETSEL,(WPARAM)BoolTab[i],(LPARAM)i);
			SendMessage(hwndMain,WM_COMMAND,(WPARAM)(MAKELONG(IDC_LIST1,LBN_SELCHANGE)),(LPARAM)hWndList1);
			hWndSlider=GetDlgItem(hwndMain,IDC_SLIDER1);
			SendMessage(hWndSlider,TBM_SETRANGE,FALSE,MAKELONG((WORD)1,(WORD)10)); 
			SendMessage(hWndSlider,TBM_SETPAGESIZE,0,1L);
			SendMessage(hWndSlider,TBM_SETPOS,TRUE,SliderPos);
			hWndEdit=GetDlgItem(hwndMain,IDC_EDIT1);
			SetWindowText(hWndEdit,"3");
			SendDlgItemMessage(hwndMain,IDC_RADIO2,BM_SETCHECK,1,0);
            SetScrollRange(hwndMain,SB_VERT,0,300,TRUE);
            ScrollCtrls(hwndMain);
			return TRUE;
		}
        case WM_VSCROLL:
        {   switch(LOWORD(wParam))
            {   case SB_PAGEUP: // Mouse clicked in up zone
                {   if(iCurYScroll>60)
                        iCurYScroll-=60;
                    else
                        iCurYScroll=0;
                    SetScrollPos(hwndMain,SB_VERT,iCurYScroll,TRUE);
                    ScrollCtrls(hwndMain);
                    break;
                }
                case SB_PAGEDOWN: // Mouse clicked in down zone
                {   if(iCurYScroll<240)
                        iCurYScroll+=60;
                    else
                        iCurYScroll=300;
                    SetScrollPos(hwndMain,SB_VERT,iCurYScroll,TRUE);
                    ScrollCtrls(hwndMain);
                    break;
                }
                case SB_LINEUP: // Scroll button up
                {   if(iCurYScroll>30)
                        iCurYScroll-=30;
                    else
                        iCurYScroll=0;
                    SetScrollPos(hwndMain,SB_VERT,iCurYScroll,TRUE);
                    ScrollCtrls(hwndMain);
                    break;
                }
                case SB_LINEDOWN: // Scroll button down
                {   if(iCurYScroll<270)
                        iCurYScroll+=30;
                    else
                        iCurYScroll=300;
                    SetScrollPos(hwndMain,SB_VERT,iCurYScroll,TRUE);
                    ScrollCtrls(hwndMain);
                    break;
                }
                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                {   iCurYScroll=HIWORD(wParam);
                    SetScrollPos(hwndMain,SB_VERT,iCurYScroll,TRUE);
                    ScrollCtrls(hwndMain);
                    break;
                }
            }
            break;
        }
		case WM_ACTIVATE:
		{	if(LOWORD(wParam)==WA_ACTIVE)
			{	mHelpInit(hwndMain,"WinApp",true);
			}
			break;
		}
		case WM_NOTIFY:
		{	switch(((LPNMHDR)lParam)->idFrom)
			{	case IDC_SLIDER1:
				{	switch(((LPNMHDR)lParam)->code)
					{	case NM_RELEASEDCAPTURE:
						{	char lpSliderSel[16]={0};
							SliderPos=(long)SendMessage(hWndSlider,TBM_GETPOS,0,0);
							sprintf(lpSliderSel,"%d",SliderPos);
							SetWindowText(hWndEdit,lpSliderSel);
							break;
						}
					}
					break;
				}
				default:
				{	return (BOOL)DefDlgProc(hwndMain,message,wParam,lParam);}
			}
			break;
		}
		case WM_COMMAND:
		{	switch(LOWORD(wParam))
			{	case IDOK:
				case IDCANCEL:
				{	EndDialog(hwndMain,IDOK);
					return TRUE;
				}
				case IDC_BUTTON1:
				{	ChildProp=MakeProcInstance((FARPROC)ChildDlgProc,hInst);
					DialogBox(hInst,MAKEINTRESOURCE(IDD_CHILDDLG1),hwndMain,(DLGPROC)ChildProp); 
					FreeProcInstance(ChildProp);
                    break;
				}
				case IDC_ENGHELP:
				{	mHelpOpen(hwndMain);
                    break;
				}
				case IDC_LIST1:
				{	switch(HIWORD(wParam))
					{	case LBN_SELCHANGE:
						{	int temp,index;
							char lpListSel[16]={0};
							int ItemsSel[24]={0};
							index=(int)(SendMessage(hWndList1,LB_GETSELITEMS,(WPARAM)24,(LPARAM)(LPINT)ItemsSel));
							SendMessage(hWndList2,LB_RESETCONTENT,0,0);
							for(temp=0;temp<24;temp++) BoolTab[temp]=FALSE;
							temp=0;
							while(temp!=index)
							{	SendMessage(hWndList1,LB_GETTEXT,(WPARAM)ItemsSel[temp],(LPARAM)(LPCTSTR)lpListSel);
								SendMessage(hWndList2,LB_ADDSTRING,0,(LPARAM)lpListSel);
								BoolTab[(ItemsSel[temp])]=TRUE;
								temp++;
							}
							break;
						}
						default: NULL;
					}
					break;
				}
			}
		    break;
	    }
    }
    return FALSE;
}

BOOL CALLBACK ChildDlgProc(HWND hwndDlg,UINT message,WPARAM wParam,LPARAM lParam)
{	static HWND hWndList=NULL;
	static HWND hWndCombo=NULL;
	static char lpEdit[100]={0};
	static RECT WndRect={0,0,0,0};
	static BOOL bPass=FALSE;
	switch (message)
    {	case WM_INITDIALOG:
		{	char lp[100]={0};
			if(!bPass)
			{	bPass=TRUE;
				sprintf(lpEdit,"Edit content...");
			}
			hWndList=GetDlgItem(hwndDlg,IDC_LIST1);
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel1"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel2"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel3"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel4"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel5"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel6"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel7"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel8"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel9"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel10"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel11"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel12"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel13"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel14"));
			SendMessage(hWndList,LB_ADDSTRING,0,(LPARAM)("SingleSel15"));
			SendMessage(hWndList,LB_SETCURSEL,IndexList,0);
			SendMessage(hWndList,LB_GETTEXT,(WPARAM)IndexList,(LPARAM)(LPCTSTR)lp);
			SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT2),lp);
			if(bCheck1) SendDlgItemMessage(hwndDlg,IDC_CHECK1,BM_SETCHECK,1,0);
			if(bCheck2) SendDlgItemMessage(hwndDlg,IDC_CHECK2,BM_SETCHECK,1,0);
			if(bCheck3) SendDlgItemMessage(hwndDlg,IDC_CHECK3,BM_SETCHECK,1,0);
			if(bCheck4) SendDlgItemMessage(hwndDlg,IDC_CHECK4,BM_SETCHECK,1,0);
			hWndCombo=GetDlgItem(hwndDlg,IDC_COMBO1);
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel1"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel2"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel3"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel4"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel5"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel6"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel7"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel8"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel9"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel10"));
			SendMessage(hWndCombo,CB_ADDSTRING,0,(LPARAM)("CboSel11"));
			SendMessage(hWndCombo,CB_SETCURSEL,IndexCbo,0);
			SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT1),lpEdit);
			if(!bExtend)
			{	GetWindowRect(hwndDlg,&WndRect);
				MoveWindow(hwndDlg,(int)WndRect.left,(int)WndRect.top,333,115,FALSE);
				SendDlgItemMessage(hwndDlg,IDC_EXTEND,BM_SETCHECK,0,0);
			}
			else SendDlgItemMessage(hwndDlg,IDC_EXTEND,BM_SETCHECK,1,0);
			return TRUE;
		}
		case WM_COMMAND:
		{	switch(LOWORD(wParam))
			{	case IDOK:
				case IDCANCEL:
				{	GetWindowText(GetDlgItem(hwndDlg,IDC_EDIT1),lpEdit,100);
					EndDialog(hwndDlg,IDOK);
					return TRUE;
				}
				case IDC_LIST1:
				{	switch(HIWORD(wParam))
					{	case LBN_SELCHANGE:
						{	char lpListSel[16]={0};
							IndexList=(int)(SendMessage(hWndList,LB_GETCURSEL,0,0));
							SendMessage(hWndList,LB_GETTEXT,(WPARAM)IndexList,(LPARAM)(LPCTSTR)lpListSel);
							SetWindowText(GetDlgItem(hwndDlg,IDC_EDIT2),lpListSel);
							break;
						}
					}
					break;
				}
				case IDC_CHECK1:
				{	bCheck1=(BOOL)SendDlgItemMessage(hwndDlg,IDC_CHECK1,BM_GETCHECK,0,0);
					break;
				}
				case IDC_CHECK2:
				{	bCheck2=(BOOL)SendDlgItemMessage(hwndDlg,IDC_CHECK2,BM_GETCHECK,0,0);
					break;
				}
				case IDC_CHECK3:
				{	bCheck3=(BOOL)SendDlgItemMessage(hwndDlg,IDC_CHECK3,BM_GETCHECK,0,0);
					break;
				}
				case IDC_CHECK4:
				{	bCheck4=(BOOL)SendDlgItemMessage(hwndDlg,IDC_CHECK4,BM_GETCHECK,0,0);
					break;
				}
				case IDC_COMBO1:
				{	switch(HIWORD(wParam))
					{	case CBN_SELCHANGE:
						{	IndexCbo=(int)(ComboBox_GetCurSel(hWndCombo));
							break;
						}
					}
					break;
				}
				case IDC_EXTEND:
				{	GetWindowRect(hwndDlg,&WndRect);
					if(bExtend)
					{	bExtend=FALSE;
						MoveWindow(hwndDlg,(int)WndRect.left,(int)WndRect.top,333,115,TRUE);
					}
					else
					{	bExtend=TRUE;
						MoveWindow(hwndDlg,(int)WndRect.left,(int)WndRect.top,333,270,TRUE);
					}
					break;
				}
			}
		    break;
	    }
    }
    return FALSE;
}


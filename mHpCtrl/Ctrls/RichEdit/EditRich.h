// EditRich.h
//

#ifndef  __RICHEDITCTRL_H //*********************************************************************************************//
#define  __RICHEDITCTRL_H

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif

#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif

#ifndef  __COMMCTRL_H
 #include <commctrl.h>
#endif

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL RichEdit_ReadEvent(HWND);
//--------------------------------------------------------------------------------------------------------------------ADD//
void RichEdit_AddEvent(HWND,UINT,int,int,BOOL);
//--------------------------------------------------------------------------------------------------------------CLASSNAME//
BOOL RichEdit_IsClassName(char*,int);
//-------------------------------------------------------------------------------------------------------------MHPPROCESS//
void RichEdit_mHpProcess(HWND,BOOL&,int,int);
BOOL RichEdit_StartProc(HWND);

#endif //****************************************************************************************************************//

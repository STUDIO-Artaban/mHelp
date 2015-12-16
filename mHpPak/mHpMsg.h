// mHpMsg.h
//

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif

#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif

//----------------------------------------------------------------------------------------------------------------GENERAL//
int mHpMessage(HWND,HINSTANCE,UINT,char*,int,UINT,UINT);
void SetViewFlag(BOOL);

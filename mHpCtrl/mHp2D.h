// mHp2D.h
//

#ifndef __MHP2D_H //*****************************************************************************************************//
#define __MHP2D_H

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif

#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif

#ifndef  __COMMCTRL_H
 #include <commctrl.h>
#endif

//-----------------------------------------------------------------------------------------------------------------------//
BOOL DoInit(HWND);
BOOL UpdateFrame(HWND);
void EndObjects(HWND);
void SetDDrawObj(HWND,int);
BOOL RestoreBackBuffer(HWND);
BOOL RestoreKeyBackBuffer(HWND);
void UpdateBackBuffer(BOOL);
void RemoveBackBuffer();

#endif //****************************************************************************************************************//
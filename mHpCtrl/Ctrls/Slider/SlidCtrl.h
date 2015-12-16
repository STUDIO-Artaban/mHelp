// SliderCtrl.h
//

#ifndef  __SLIDERCTRL_H //***********************************************************************************************//
#define  __SLIDERCTRL_H

#ifndef  __WINDOWS_H
 #include <windows.h>
#endif

#ifndef  _INC_WINDOWSX
 #include <windowsx.h>
#endif

#ifndef  __COMMCTRL_H
 #include <commctrl.h>
#endif

#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif

//----------------------------------------------------------------------------------------------------------------OBJECTS//
class CSliderTask : public CmHpTask
{	private:
		int iSliderPos;
	public:
		CSliderTask(char*,char*,int,int,int,int*,char*,int);
		CSliderTask();
		BOOL ReadTaskKey(int);
		BOOL WriteTaskKey();
		int GetSliderPos();
		void SetSliderPos(int);
};
//------------------------------------------------------//
class CSliderWrite : public CmHpWrite
{	private:
		int iMinPos;
		int iMaxPos;
	public:
		CSliderWrite(int,int);
		CSliderWrite();
		BOOL ReadEventKey(int);
		BOOL WriteEventKey();
		int GetMinPos();
		int GetMaxPos();
};

//----------------------------------------------------------------------------------------------------------------MHPFILE//
BOOL Slider_ReadEvent(HWND);
void Slider_WriteEvent(HWND,int,CEventListe*,CWindowListe*,BOOL,char*,int&,BOOL&);
BOOL Slider_WriteParam(HWND,CEventListe*,int);

//-------------------------------------------------------------------------------------------------------------------KEYS//
void Slider_ReadEventKeys(int,CEventListe*,CTaskListe*);

//--------------------------------------------------------------------------------------------------------------------ADD//
void Slider_AddEvent(HWND,char*,HWND,int,int,BOOL);

//-------------------------------------------------------------------------------------------------------------PROPERTIES//
BOOL Slider_IsClassName(char*,int);
void Slider_IsCustom(BOOL&);
void Slider_StrState(CEventCell*,BOOL,char*,int);
int Slider_ParamType();
void Slider_DefaultBound(CEventCell*,char*,int);
BOOL Slider_IsCustomized();
BOOL Slider_IsNoStateMsg();
void Slider_GetHtmlTag(CEventCell*,char*,UINT,int);

//---------------------------------------------------------------------------------------------------------------CFGTOOLS//
void Slider_ShowCfgTools(HWND);
void Slider_HideCfgTools(HWND);
void Slider_SaveCfg(HWND);
void Slider_LoadCfg(HWND);
void Slider_GetInfosCfg(HWND,HWND,HWND,HWND,HWND);
BOOL Slider_ProcCfg(HWND,UINT,WPARAM,LPARAM,HWND);

//----------------------------------------------------------------------------------------------------------------PROCESS//
void Slider_CaptureMsg(HWND,char*,LPARAM,HWND,int,CEventListe*,char*,UINT&,int,int,BOOL&,int&,BOOL);
void Slider_mHpProcess(HWND,BOOL&,int,int);
BOOL Slider_StartProc(HWND);

#endif //****************************************************************************************************************//
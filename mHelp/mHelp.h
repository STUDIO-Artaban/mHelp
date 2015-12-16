//---------------------------------------------------------------------------//
// mHelp.h
//
// Copyright(c) 1999-2004, Unknow Software. All rights reserved.
//---------------------------------------------------------------------------//
#define MHELP_API __declspec(dllexport)

#ifndef __MHELP_H
#define __MHELP_H

extern "C" {
    //-----------------------------------------------------------------------//
    // Main
    void MHELP_API mHelpInit(HWND,const char*,bool);
    void MHELP_API mHelpOpen(HWND);

    //-----------------------------------------------------------------------//
    // Extended
    int  MHELP_API mHelpErrorMsg(int,char*,int);
    int  MHELP_API mHelpLaunchFavTask(HWND,char*,int);
    void MHELP_API mHelpCreateCustomTask(HWND,char*,int,int&,int&);
    int  MHELP_API mHelpLnchCustomTask(HWND,int);
    void MHELP_API mHelpGetParamCnt(int,int&,int&);
    void MHELP_API mHelpGetParamType(int,int,int&,int&);
    void MHELP_API mHelpGetRepeatOpt(int,int&,int&);
    int  MHELP_API mHelpSetRepeatOpt(int,int);
    void MHELP_API mHelpGetIntParamVal(int,int,int,int,int&,int&);
    int  MHELP_API mHelpSetIntParamVal(int,int,int,int);
    int  MHELP_API mHelpAddIntParamVal(int,int,int,int);
    int  MHELP_API mHelpRemoveIntParamVal(int,int,int,int);
    int  MHELP_API mHelpGetStrParamVal(int,int,int,int,char*,int);
    int  MHELP_API mHelpSetStrParamVal(int,int,int,char*);
    int  MHELP_API mHelpAddStrParamVal(int,int,int,char*,int);
    int  MHELP_API mHelpRemoveStrParamVal(int,int,int,int);
    int  MHELP_API mHelpDestroyCustomTask(int);

}

#endif

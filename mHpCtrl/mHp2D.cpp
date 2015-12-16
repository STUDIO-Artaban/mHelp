// mHp2D.cpp
//

#include "stdafx.h"
#include "mHp2D.h"

#ifndef  __MHPCOMMON_H
 #include "mHpComm.h"
#endif

#ifndef _D2D1_H_
 #include <d2d1.h>
#endif

#include "resource.h"

#ifndef  __MHPVAR_H
 #include "mHpVar.h"
#endif

//-----------------------------------------------------//
// Macros
#define     GET_KEY_CURSOR_POS_X()          15
#define     GET_KEY_CURSOR_POS_Y()          (GetmHpVarStruct()->yResolution-110)

//-----------------------------------------------------//
// Definitions
#define     SHIFT_KEY_CURSOR_WIDTH          118
#define     SHIFT_KEY_CURSOR_HEIGHT         97
#define     ALTGR_KEY_CURSOR_WIDTH          86
#define     ALTGR_KEY_CURSOR_HEIGHT         97

//-----------------------------------------------------//
// Variables
DWORD Delay=4;										   // Delay between each D2D cursor move
LPCSTR szBitmap=NULL;                                  // Cursor bitmap
int picWidth=0;                                        // Width of the selected cursor
int picHeight=0;                                       // Height of the selected cursor
int CurPosX=NO;                                        // X position of the cursor (== NO means no back buffer)
int CurPosY=NO;                                        // Y position of the cursor (== NO means no back buffer)
int LastXimpl=0;                                       // Last horizontal lag (cursor move)
int LastYimpl=0;                                       // Last vertical lag (cursor move)
float Coeff=1.0f;									   // Coefficient du deplacement lineaire du curseur DDraw
float Const=0.0f;									   // Constante du deplacement lineaire du curseur DDraw
BOOL bNewRequete=FALSE;						           // Keep background for new request
int scrnWidth=0;                                       // Screen width
int scrnHeight=0;                                      // Screen height
BOOL bShiftAltGr=FALSE;                                // TRUE: Shift key; FALSE: AltGr key
ID2D1Factory *pFactory=NULL;                           // Direct2D factory
ID2D1DCRenderTarget *pRenderTarget=NULL;               // Direct2D device context render target
ID2D1Bitmap *pBmpToDraw=NULL;                          // Direct2D bitmap to draw
ID2D1Bitmap *pBmpBackGRD=NULL;                         // Direct2D background bitamp
BYTE *pToDrawBuff=NULL;                                // Buffer of the bitmap to draw
BYTE *pBackBuff=NULL;                                  // Back buffer that the bitmap to draw will overwrite
BYTE *pBackGrdBuff=NULL;                               // Buffer of the entire background screen
BYTE *pKeyBackBuff=NULL;                               // Back buffer of the key cursor (Shift or AltGr)

//-----------------------------------------------------//
// Déclarations
BOOL InitFail(HWND,UINT);
void PreChangeDDCurPos(int,int,int&,int&);
float FonctionX(BOOL);
BOOL GetBackGrdScreen(HWND);
BOOL GetBackGrdBuffer(HWND,int,int);
BOOL SetBackGrdBuffer(HWND);
BOOL CreateBitmapD2D(HWND,int,int);

//-----------------------------------------------------------------------------------------------------------------------//
// InitFail
//-----------------------------------------------------//
BOOL InitFail(HWND hWndActive,UINT StringID)
{	StopmHelp(hWndActive,TRUE,TRUE);
	mHpPak_mHpMessage(hWndActive,GetmHpVarStruct()->hmHpCtrlInst,StringID,NULL,0,0,MB_ICONSTOP);
	return FALSE;
}

/*void PreChangeCurPos(int& Xinc,int& Yinc)
{   static float Tx=0.0f;
    static float Ty=0.0f;
    static float NextPosX=0.0f;
    static float NextPosY=0.0f;
    if(GetmHpVarStruct()->bNewDepl)
    {   int CurEcartX=abs(GetmHpVarStruct()->XnextPos-GetmHpVarStruct()->XposCur);
        int CurEcartY=abs(GetmHpVarStruct()->YnextPos-GetmHpVarStruct()->YposCur);
        int MaxEcart=(CurEcartX>CurEcartY)?CurEcartX:CurEcartY;
        int RefFrameCnt=100/GetmHpVarStruct()->iDynMouseSpeed;
        GetmHpVarStruct()->EcartDepl=(RefFrameCnt*MaxEcart)/100;
        if(!GetmHpVarStruct()->EcartDepl) GetmHpVarStruct()->EcartDepl=1;
        //
        Tx=(GetmHpVarStruct()->XnextPos-GetmHpVarStruct()->XposCur)/GetmHpVarStruct()->EcartDepl;
        Ty=(GetmHpVarStruct()->YnextPos-GetmHpVarStruct()->YposCur)/GetmHpVarStruct()->EcartDepl;
        NextPosX=(float)GetmHpVarStruct()->XposCur;
        NextPosY=(float)GetmHpVarStruct()->YposCur;
        //
        GetmHpVarStruct()->bNewDepl=FALSE;
    }
    NextPosX+=Tx;
    NextPosY+=Ty;
    //
    Xinc=((int)NextPosX)-GetmHpVarStruct()->XposCur;
    Yinc=((int)NextPosY)-GetmHpVarStruct()->YposCur;
}*/

//-----------------------------------------------------//
// FonctionX
//-----------------------------------------------------//
float FonctionX(BOOL xSens)
{	float fResult;
	if(xSens) fResult=Coeff*((float)(GetmHpVarStruct()->XposCur+1));
	else fResult=Coeff*((float)(GetmHpVarStruct()->XposCur));
	fResult+=Const;
	return fResult;
}

//-----------------------------------------------------//
// PreChangeDDCurPos
//-----------------------------------------------------//
void PreChangeDDCurPos(int Xini,int Yini,int& Ximpl,int& Yimpl)
{	int	Ydiff=Yini-GetmHpVarStruct()->YnextPos;
	int	Xdiff=Xini-GetmHpVarStruct()->XnextPos;
	int	Yffid=GetmHpVarStruct()->YnextPos-Yini;
	int	Xffid=GetmHpVarStruct()->XnextPos-Xini;
	if(Ydiff==0)
	{	if(Xdiff>0) Ximpl--;
		else if(Xdiff<0) Ximpl++;
		return;
	}
	if(Xdiff==0)
	{	if(Ydiff>0) Yimpl--;
		else if(Ydiff<0) Yimpl++;
		return;
	}
	if((Ydiff>0) && (Xdiff<0))
	{	if(GetmHpVarStruct()->bNewDepl)
		{	Coeff=(((float)(Yffid))/((float)(Xdiff)));
			Const=(float)Yini;Const++;Const*=NO;
			Const-=(Coeff*((float)Xini));
			GetmHpVarStruct()->bNewDepl=FALSE;
		}
		Yimpl--;Ximpl++;
		if(FonctionX(TRUE)>((float)(-Yini)))
		{	if(Ydiff>Xffid) Ximpl--;
		}
		else if(FonctionX(TRUE)<((float)(-Yini)))
		{	if(Ydiff<Xffid) Yimpl++;
		}
	}
	else
	{	if((Ydiff<0) && (Xdiff<0))
		{	if(GetmHpVarStruct()->bNewDepl)
			{	Coeff=(((float)(Yffid))/((float)(Xdiff)));
				Const=(float)Yini;Const*=NO;
				Const-=(Coeff*((float)Xini));
				GetmHpVarStruct()->bNewDepl=FALSE;
			}
			Yimpl++;Ximpl++;
			if(FonctionX(TRUE)<((float)(-1-Yini)))
			{	if(Yffid>Xffid) Ximpl--;
			}
			else if(FonctionX(TRUE)>((float)(-1-Yini)))
			{	if(Yffid<Xffid) Yimpl--;
			}
		}
		else
		{	if((Ydiff<0) && (Xdiff>0))
			{	if(GetmHpVarStruct()->bNewDepl)
				{	Coeff=(((float)(Yffid))/((float)(Xdiff)));
					Const=(float)Yini;Const*=NO;
					Const-=(Coeff*((float)(Xini+1)));
					GetmHpVarStruct()->bNewDepl=FALSE;
				}
				Yimpl++;Ximpl--;
				if(FonctionX(FALSE)>((float)(-1-Yini)))
				{	if(Yffid<Xdiff) Yimpl--;
				}
				else if(FonctionX(FALSE)<((float)(-1-Yini)))
				{	if(Yffid>Xdiff) Ximpl++;
				}
			}
			else
			{	if((Ydiff>0) && (Xdiff>0))
				{	if(GetmHpVarStruct()->bNewDepl)
					{	Coeff=(((float)(Yffid))/((float)(Xdiff)));
						Const=(float)Yini;Const++;Const*=NO;
						Const-=(Coeff*((float)(Xini+1)));
						GetmHpVarStruct()->bNewDepl=FALSE;
					}
					Yimpl--;Ximpl--;
					if(FonctionX(FALSE)>((float)(-Yini)))
					{	if(Ydiff>Xdiff) Ximpl++;
					}
					else if(FonctionX(FALSE)<((float)(-Yini)))
					{	if(Ydiff<Xdiff) Yimpl++;
					}
				}
			}
		}
	}
	/*static int iDirect=0;
	static int incrE=0;
	static int incrNE=0;
	static int d=0;
	if(GetmHpVarStruct()->bNewDepl)
	{	if(((GetmHpVarStruct()->XnextPos-Xini)>0)&&((Yini-GetmHpVarStruct()->YnextPos)>0)) iDirect=0;
		if(((GetmHpVarStruct()->XnextPos-Xini)>0)&&((GetmHpVarStruct()->YnextPos-Yini)>0)) iDirect=1;
		if(((Xini-GetmHpVarStruct()->XnextPos)>0)&&((GetmHpVarStruct()->YnextPos-Yini)>0)) iDirect=2;
		if(((Xini-GetmHpVarStruct()->XnextPos)>0)&&((Yini-GetmHpVarStruct()->YnextPos)>0)) iDirect=3;
	}
	switch(iDirect)
	{	case 0: // ->D->H
		{	if(GetmHpVarStruct()->bNewDepl)
			{	d=(Yini-GetmHpVarStruct()->YnextPos)*2-(GetmHpVarStruct()->XnextPos-Xini);
				incrE=(Yini-GetmHpVarStruct()->YnextPos)*2;
				incrNE=((Yini-GetmHpVarStruct()->YnextPos)-(GetmHpVarStruct()->XnextPos-Xini))/2;
				GetmHpVarStruct()->bNewDepl=FALSE;
			}
			if(d<=0)
			{	d+=incrE;
				Ximpl++;
			}
			else
			{	d+=incrNE;
				Ximpl++;
				Yimpl--;
			}
			break;
		}
		case 1: // ->D->B
		{	if(GetmHpVarStruct()->bNewDepl)
			{	d=(GetmHpVarStruct()->YnextPos-Yini)*2-(GetmHpVarStruct()->XnextPos-Xini);
				incrE=(GetmHpVarStruct()->YnextPos-Yini)*2;
				incrNE=((GetmHpVarStruct()->YnextPos-Yini)-(GetmHpVarStruct()->XnextPos-Xini))/2;
				GetmHpVarStruct()->bNewDepl=FALSE;
			}
			if(d<=0)
			{	d+=incrE;
				Ximpl++;
			}
			else
			{	d+=incrNE;
				Ximpl++;
				Yimpl++;
			}
			break;
		}
		case 2: // ->G->B
		{	if(GetmHpVarStruct()->bNewDepl)
			{	d=(GetmHpVarStruct()->YnextPos-Yini)*2-(Xini-GetmHpVarStruct()->XnextPos);
				incrE=(GetmHpVarStruct()->YnextPos-Yini)*2;
				incrNE=((GetmHpVarStruct()->YnextPos-Yini)-(Xini-GetmHpVarStruct()->XnextPos))/2;
				GetmHpVarStruct()->bNewDepl=FALSE;
			}
			if(d<=0)
			{	d+=incrE;
				Ximpl--;
			}
			else
			{	d+=incrNE;
				Ximpl--;
				Yimpl++;
			}
			break;
		}
		case 3: // ->G->H
		{	if(GetmHpVarStruct()->bNewDepl)
			{	d=(Yini-GetmHpVarStruct()->YnextPos)*2-(Xini-GetmHpVarStruct()->XnextPos);
				incrE=(Yini-GetmHpVarStruct()->YnextPos)*2;
				incrNE=((Yini-GetmHpVarStruct()->YnextPos)-(Xini-GetmHpVarStruct()->XnextPos))/2;
				GetmHpVarStruct()->bNewDepl=FALSE;
			}
			if(d<=0)
			{	d+=incrE;
				Ximpl--;
			}
			else
			{	d+=incrNE;
				Ximpl--;
				Yimpl--;
			}
			break;
		}
	}*/
}

//-----------------------------------------------------//
// GetBackGrdScreen
//-----------------------------------------------------//
BOOL GetBackGrdScreen(HWND hWndActive)
{   HDC hdcScreen=CreateDC("DISPLAY",NULL,NULL,NULL);
    if(!hdcScreen)
        return InitFail(hWndActive,IDS_D2D_CREATE_DC);
    HDC hdcCompatible=CreateCompatibleDC(hdcScreen);
    if(!hdcCompatible)
        return InitFail(hWndActive,IDS_D2D_COMPATIBLE_DC);
    HBITMAP hbmpScreen=CreateCompatibleBitmap(hdcScreen,scrnWidth,scrnHeight);
    if(!hbmpScreen)
        return InitFail(hWndActive,IDS_D2D_COMPATIBLE_BMP);
    SelectObject(hdcCompatible,hbmpScreen);
    if(!BitBlt(hdcCompatible,0,0,scrnWidth,scrnHeight,hdcScreen,0,0,SRCCOPY))
        return InitFail(hWndActive,IDS_D2D_BITBLT);
    BITMAPINFO bmi;
    memset(&bmi,0,sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	if(!GetDIBits(hdcCompatible,hbmpScreen,0,0,NULL,&bmi,DIB_RGB_COLORS))
        return InitFail(hWndActive,IDS_D2D_SCREEN_BUFFER);
	bmi.bmiHeader.biCompression=BI_RGB;
    if(!GetDIBits(hdcCompatible,hbmpScreen,0,scrnHeight,pBackGrdBuff,&bmi,DIB_RGB_COLORS))
        return InitFail(hWndActive,IDS_D2D_SCREEN_BUFFER);
    //
    DeleteObject(hbmpScreen);
    DeleteDC(hdcCompatible);
    DeleteDC(hdcScreen);
    return TRUE;
}

//-----------------------------------------------------//
// GetBackGrdBuffer
//-----------------------------------------------------//
BOOL GetBackGrdBuffer(HWND hWndActive,int xPos,int yPos)
{   int bbIdx=0;
    // Back buffer screen is verticaly reversed
	for(int iHeight=(scrnHeight-yPos-1); iHeight>=(scrnHeight-picHeight-yPos); iHeight--)
    {   for(int iWidth=xPos; iWidth<(xPos+picWidth); iWidth++)
        {   int i=(iHeight*scrnWidth*4)+(iWidth*4);
            pBackBuff[bbIdx++]=pBackGrdBuff[i++];
            pBackBuff[bbIdx++]=pBackGrdBuff[i++];
            pBackBuff[bbIdx++]=pBackGrdBuff[i];
            pBackBuff[bbIdx++]=0xFF;  // Alpha = 255
        }
    }
    HRESULT hr=S_OK;
    if((!pBmpBackGRD)||(pBmpBackGRD->GetSize().width!=picWidth)||(pBmpBackGRD->GetSize().height!=picHeight))
    {   if(pBmpBackGRD)
            pBmpBackGRD->Release();
        D2D1_SIZE_U size=D2D1::SizeU(picWidth,picHeight);
        D2D1_PIXEL_FORMAT pixFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED);
        D2D1_BITMAP_PROPERTIES bmpProperties=D2D1::BitmapProperties(pixFormat,0.0f,0.0f);
        hr=pRenderTarget->CreateBitmap(size,bmpProperties,&pBmpBackGRD);
    }
    if(SUCCEEDED(hr))
    {   D2D_RECT_U bbRect=D2D1::RectU(0,0,picWidth,picHeight);
        hr=pBmpBackGRD->CopyFromMemory(&bbRect,pBackBuff,picWidth*3+picWidth);
        if(FAILED(hr))
            return InitFail(hWndActive,IDS_D2D_COPY_BBUFFER);
    }
    else
        return InitFail(hWndActive,IDS_D2D_CREATE_BBUFFER);
    return TRUE;
}

//-----------------------------------------------------//
// SetBackGrdBuffer
//-----------------------------------------------------//
BOOL SetBackGrdBuffer(HWND hWndActive)
{   //////////////////////////////////////////////////////////////////////////// Direct2D
    if((pRenderTarget)&&(CurPosX!=NO)&&(CurPosY!=NO))
    {   if(GetBackGrdBuffer(hWndActive,CurPosX-LastXimpl,CurPosY-LastYimpl))
        {   pRenderTarget->BeginDraw();
            pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
            D2D1_RECT_F backRect=D2D1::RectF((float)(CurPosX-LastXimpl),
                                             (float)(CurPosY-LastYimpl),
                                             (float)(CurPosX-LastXimpl+picWidth),
                                             (float)(CurPosY-LastYimpl+picHeight));
            pRenderTarget->DrawBitmap(pBmpBackGRD,&backRect,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
            HRESULT hr=pRenderTarget->EndDraw();
            if((FAILED(hr))||(hr==D2DERR_RECREATE_TARGET))
                return InitFail(hWndActive,IDS_D2D_END_DRAW);
        }
        else
            return FALSE;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    return TRUE;
}

//-----------------------------------------------------//
// CreateBitmapD2D
//-----------------------------------------------------//
BOOL CreateBitmapD2D(HWND hWndActive,int bmpWidth,int bmpHeight)
{   static LPCSTR szLastBitmap=NULL;
    if((!pBmpToDraw)||(pBmpToDraw->GetSize().width!=bmpWidth)||(pBmpToDraw->GetSize().height!=bmpHeight))
    {   if(pBmpToDraw)
        {   pBmpToDraw->Release();
            pBmpToDraw=NULL;
        }
        D2D1_SIZE_U size=D2D1::SizeU(bmpWidth,bmpHeight);
        D2D1_PIXEL_FORMAT pixFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED);
        D2D1_BITMAP_PROPERTIES bmpProperties=D2D1::BitmapProperties(pixFormat,0.0f,0.0f);
        HRESULT hr=pRenderTarget->CreateBitmap(size,bmpProperties,&pBmpToDraw);
        if(FAILED(hr))
            return InitFail(hWndActive,IDS_D2D_CREATE_CURSOR);
        szLastBitmap=NULL;
    }
    if(szBitmap!=szLastBitmap)
    {   szLastBitmap=szBitmap;
        HBITMAP hBmp=LoadBitmap(GetmHpVarStruct()->hmHpCtrlInst,szBitmap);
        if(hBmp!=NULL)
        {   long lres=GetBitmapBits(hBmp,bmpWidth*bmpHeight*4,pToDrawBuff);
            DeleteObject(hBmp);
            if(lres)
            {   DWORD* pPixel=(DWORD*)pToDrawBuff;
                for(int i = 0; i<(bmpWidth*bmpHeight); i++)
                {   if(*(pPixel+i)==0x0000FFFF)     // Alpha == 0 + Color key == RGB(0,255,255)
                        *(pPixel+i)=0x00000000;     // Alpha = 0 + Color = RGB(0,0,0)
                    else
                        *(pPixel+i)|=0xFF000000;    // Alpha = 255
                }
                D2D_RECT_U bmpRect=D2D1::RectU(0,0,bmpWidth,bmpHeight);
                HRESULT hr=pBmpToDraw->CopyFromMemory(&bmpRect,pToDrawBuff,bmpWidth*3+bmpWidth);
                if(FAILED(hr))
                    return InitFail(hWndActive,IDS_D2D_COPY_CBUFFER);
                return TRUE;
            }
            return InitFail(hWndActive,IDS_D2D_GET_CBUFFER);
        }
        return InitFail(hWndActive,IDS_D2D_LOAD_CURSOR);
    }
    return TRUE;
}

//******************************************************************************************************************DYN2D//
// DoInit
//-----------------------------------------------------//
BOOL DoInit(HWND hWndActive)
{   if(GetmHpVarStruct()->imHpMode==1) return TRUE;
    if(!pFactory)
    {   HRESULT hr=D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
        if(FAILED(hr))
            return InitFail(hWndActive,IDS_D2D_FACTORY);
        D2D1_RENDER_TARGET_PROPERTIES rtProp=D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE),
                        0.0f,
                        0.0f,
                        D2D1_RENDER_TARGET_USAGE_NONE,
                        D2D1_FEATURE_LEVEL_DEFAULT);
        hr=pFactory->CreateDCRenderTarget(&rtProp,&pRenderTarget);
        if(FAILED(hr))
            return InitFail(hWndActive,IDS_D2D_RENDERTARGET);
        HDC desktopDC=GetDC(NULL);
        scrnWidth=GetDeviceCaps(desktopDC,HORZRES);
        scrnHeight=GetDeviceCaps(desktopDC,VERTRES);
        RECT scrnRect;
        scrnRect.left=0;
        scrnRect.top=0;
        scrnRect.right=(LONG)scrnWidth;
        scrnRect.bottom=(LONG)scrnHeight;
        hr=pRenderTarget->BindDC(desktopDC,&scrnRect);
        if(FAILED(hr))
            return InitFail(hWndActive,IDS_D2D_RT_BINDDC);
        pToDrawBuff=new BYTE[352*97*4];     // Size of the biggest resource bitmap (IDB_SHIFT_PRESS)
        pBackBuff=new BYTE[352*97*4];       // " " "
		pBackGrdBuff=new BYTE[scrnHeight*scrnWidth*4];
    }
    LastYimpl=0;
	LastXimpl=0;
    CurPosX=NO;
    CurPosY=NO;
    bNewRequete=TRUE;
    return TRUE;
}

//-----------------------------------------------------//
// UpdateFrame
//-----------------------------------------------------//
BOOL UpdateFrame(HWND hWndActive)
{	static DWORD LastTickCount=0;
	DWORD ThisTickCount;
    if(GetmHpVarStruct()->imHpMode==1)
	{	GetmHpVarStruct()->XposCur=GetmHpVarStruct()->XnextPos;
		GetmHpVarStruct()->YposCur=GetmHpVarStruct()->YnextPos;
		GetmHpVarStruct()->bRefreshDDraw=FALSE;
		return TRUE;
	}
    if((GetmHpVarStruct()->DDrawBitmap==5)||(GetmHpVarStruct()->DDrawBitmap==6))
    {   CurPosX=GET_KEY_CURSOR_POS_X();
        CurPosY=GET_KEY_CURSOR_POS_Y();
    }
    else
    {   CurPosX=GetmHpVarStruct()->XposCur;
        CurPosY=GetmHpVarStruct()->YposCur;
    }
	switch(GetmHpVarStruct()->DDrawBitmap)
	{	case 0: CreateBitmapD2D(hWndActive,232,28); break;
		case 1: CreateBitmapD2D(hWndActive,106,34); break;
		case 2: CreateBitmapD2D(hWndActive,106,34); break;
		case 3: CreateBitmapD2D(hWndActive,106,34);	break;
		case 4:	CreateBitmapD2D(hWndActive,232,28); break;
		case 5:	CreateBitmapD2D(hWndActive,256,97);	break;
		case 6:	CreateBitmapD2D(hWndActive,352,97);	break;
		case 7:
		case 8:
		{	CreateBitmapD2D(hWndActive,139,51);
            CurPosX=GetmHpVarStruct()->XposCur;
            CurPosY=GetmHpVarStruct()->YposCur-23;
			break;
		}
	}
	// Delay
    ThisTickCount=GetTickCount();
    if((ThisTickCount-LastTickCount)<=Delay) return TRUE;
    //
    D2D1_RECT_F srcRect;
    int Yimpl=0;
    int Ximpl=0;
	switch(GetmHpVarStruct()->DDrawBitmap)
	{	case 0:
		{	// Choix du DDraw curseur
            srcRect=D2D1::RectF((float)(GetmHpVarStruct()->CursorType*33),
                                0.0f,
                                (float)(33+(GetmHpVarStruct()->CursorType*33)+1),
                                (float)(27+1));
            // Futur position par rapport a la vitesse de deplacement
			int Xfutur=GetmHpVarStruct()->XposCur;
			int Yfutur=GetmHpVarStruct()->YposCur;
			int EcartTemp=GetmHpVarStruct()->EcartDepl;
			while(((EcartTemp!=0)&&(Xfutur!=GetmHpVarStruct()->XnextPos)) ||
				  ((EcartTemp!=0)&&(Yfutur!=GetmHpVarStruct()->YnextPos)))
			{	PreChangeDDCurPos(Xfutur,Yfutur,Ximpl,Yimpl);
				Xfutur=GetmHpVarStruct()->XposCur+Ximpl;
				Yfutur=GetmHpVarStruct()->YposCur+Yimpl;
				EcartTemp--;
			}
			break;
		}
		case 1:
		case 2:
		case 3:
		{	// Change d'animation
			GetmHpVarStruct()->iCurAnim++;
			if(GetmHpVarStruct()->iCurAnim>=3) GetmHpVarStruct()->iCurAnim=0;
            srcRect=D2D1::RectF((float)(GetmHpVarStruct()->iCurAnim*35),
                                0.0f,
                                (float)(35+(GetmHpVarStruct()->iCurAnim*35)+1),
                                (float)(33+1));
			break;
		}
		case 4:
		{	// Choix du waiting cursor
            srcRect=D2D1::RectF((float)(GetmHpVarStruct()->CursorType*33),
                                0.0f,
                                (float)(33+(GetmHpVarStruct()->CursorType*33)+1),
                                (float)(27+1));
			break;
		}
		case 5: // Alt Gr
		{	// Change d'animation
            srcRect=D2D1::RectF((float)(GetmHpVarStruct()->iCurAnim*85),
                                0.0f,
                                (float)(85+(GetmHpVarStruct()->iCurAnim*85)+1),
                                (float)(96+1));
			GetmHpVarStruct()->iCurAnim++;
			if((GetmHpVarStruct()->iCurAnim==3)&&(GetmHpVarStruct()->iCntAnim==GetmHpVarStruct()->iMaxAnim))
			{	GetmHpVarStruct()->bLockAnim=FALSE;}
			else if(GetmHpVarStruct()->iCurAnim>=3)
			{	GetmHpVarStruct()->iCurAnim=0;
				GetmHpVarStruct()->iCntAnim++;
			}
			break;
		}
		case 6: // Shift
		{	// Change d'animation
            srcRect=D2D1::RectF((float)(GetmHpVarStruct()->iCurAnim*117),
                                0.0f,
                                (float)(117+(GetmHpVarStruct()->iCurAnim*117)+1),
                                (float)(96+1));
			GetmHpVarStruct()->iCurAnim++;
			if((GetmHpVarStruct()->iCurAnim==3)&&(GetmHpVarStruct()->iCntAnim==GetmHpVarStruct()->iMaxAnim))
			{	GetmHpVarStruct()->bLockAnim=FALSE;}
			else if(GetmHpVarStruct()->iCurAnim>=3)
			{	GetmHpVarStruct()->iCurAnim=0;
				GetmHpVarStruct()->iCntAnim++;
			}
			break;
		}
		case 7:
		{	srcRect=D2D1::RectF(0.0f,0.0f,(float)(69+1),(float)(50+1));
			break;
		}
		case 8:
		{	srcRect=D2D1::RectF(69.0f,0.0f,(float)(138+1),(float)(50+1));
			break;
		}
	}
	// Delay
    ThisTickCount=GetTickCount();
    if((ThisTickCount-LastTickCount)>Delay)
    {   LastTickCount=ThisTickCount;
        if(bNewRequete)
        {   if(GetmHpVarStruct()->bConserveBGD) GetmHpVarStruct()->bConserveBGD=FALSE;
		    else if(!GetBackGrdScreen(hWndActive))  // Get back ground screen
                return FALSE;
        }
        if(!GetBackGrdBuffer(hWndActive,CurPosX-LastXimpl,CurPosY-LastYimpl))
            return FALSE;
        //////////////////////////////////////////////////////////////////////// Direct2D
        pRenderTarget->BeginDraw();
        if(bNewRequete)
            bNewRequete=FALSE;
        else
        {   if(!GetmHpVarStruct()->DDrawBitmap)
                pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
            D2D1_RECT_F backRect=D2D1::RectF((float)(CurPosX-LastXimpl),
                                             (float)(CurPosY-LastYimpl),
                                             (float)(CurPosX-LastXimpl+picWidth),
                                             (float)(CurPosY-LastYimpl+picHeight));
            pRenderTarget->DrawBitmap(pBmpBackGRD,&backRect,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
            if(!GetmHpVarStruct()->DDrawBitmap)
                pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation((float)Ximpl,(float)Yimpl));
        }
        if(!GetmHpVarStruct()->DDrawBitmap)
            pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        D2D1_RECT_F destRect=D2D1::RectF((float)CurPosX,
                                         (float)CurPosY,
                                         (float)(CurPosX+picWidth),
                                         (float)(CurPosY+picHeight));
        pRenderTarget->DrawBitmap(pBmpToDraw,&destRect,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,&srcRect);
        if(!GetmHpVarStruct()->DDrawBitmap)
        {   pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation((float)Ximpl,(float)Yimpl));
            GetmHpVarStruct()->XposCur+=Ximpl;
	        GetmHpVarStruct()->YposCur+=Yimpl;
            if((!Ximpl)&&(!Yimpl)) GetmHpVarStruct()->bMoveDone=TRUE;
            else GetmHpVarStruct()->bMoveDone=FALSE;
            //
            LastXimpl=Ximpl;
            LastYimpl=Yimpl;
            CurPosX=GetmHpVarStruct()->XposCur;
            CurPosY=GetmHpVarStruct()->YposCur;
        }
        HRESULT hr=pRenderTarget->EndDraw();
        if((FAILED(hr))||(hr==D2DERR_RECREATE_TARGET))
            return InitFail(hWndActive,IDS_D2D_END_DRAW);
        /////////////////////////////////////////////////////////////////////////////////
        if(((GetmHpVarStruct()->DDrawBitmap==5)||
            (GetmHpVarStruct()->DDrawBitmap==6))&&
            (!GetmHpVarStruct()->bLockAnim)) // Finish to animate
        {   if(!pKeyBackBuff)
                pKeyBackBuff=new BYTE[352*97*4];
            memcpy(pKeyBackBuff,pBackBuff,352*97*4);
            bShiftAltGr=(GetmHpVarStruct()->DDrawBitmap==6)? TRUE:FALSE;
            //
            CurPosX=GetmHpVarStruct()->XposCur;
            CurPosY=GetmHpVarStruct()->YposCur;
        }
    }
	GetmHpVarStruct()->bRefreshDDraw=FALSE;
    return TRUE;
}

//-----------------------------------------------------//
// EndObjects
//-----------------------------------------------------//
void EndObjects(HWND hWndActive)
{   if(GetmHpVarStruct()->imHpMode==1) return;
    SetBackGrdBuffer(hWndActive); // Restore back buffer if any
    if(pFactory) // Determine if all following varaible are defined or not (see DoInit)
    {   pFactory->Release();
        pFactory=NULL;
    }
    if(pRenderTarget)
    {   pRenderTarget->Release();
        pRenderTarget=NULL;
    }
    if(pBmpToDraw)
    {   pBmpToDraw->Release();
        pBmpToDraw=NULL;
    }
    if(pBmpBackGRD)
    {   pBmpBackGRD->Release();
        pBmpBackGRD=NULL;
    }
    if(pToDrawBuff)
    {   delete [] pToDrawBuff;
        pToDrawBuff=NULL;
    }
    if(pBackBuff)
    {   delete [] pBackBuff;
        pBackBuff=NULL;
    }
    if(pBackGrdBuff)
    {   delete [] pBackGrdBuff;
        pBackGrdBuff=NULL;
    }
    // Clean up the screen on exit
    RedrawWindow(NULL,NULL,NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
}

//-----------------------------------------------------//
// SetDDrawObj
//-----------------------------------------------------//
void SetDDrawObj(HWND hWndActive,int BitmapType)
{	GetmHpVarStruct()->DDrawBitmap=BitmapType;
	Delay=100;
	switch(GetmHpVarStruct()->DDrawBitmap)
	{	case 0:
		{	GetmHpVarStruct()->iCurAnim=0;
			GetmHpVarStruct()->CursorType=0;
            Delay=4;
            szBitmap=MAKEINTRESOURCE(IDB_MOUSE);
			picWidth=34;
            picHeight=28;
            break;
		}
		case 1: 
		{	GetmHpVarStruct()->iCurAnim=0;
			// Initialisation de "XposCur" et "YposCur"
			GetmHpVarStruct()->YposCur-=32;
			GetmHpVarStruct()->XposCur-=6;
            szBitmap=MAKEINTRESOURCE(IDB_CLICK);
			picWidth=36;
            picHeight=34;
            break;
		}
		case 2: 
		{	GetmHpVarStruct()->iCurAnim=0;
			// Initialisation de "XposCur" et "YposCur"
			GetmHpVarStruct()->YposCur-=32;
			GetmHpVarStruct()->XposCur-=6;
            szBitmap=MAKEINTRESOURCE(IDB_PRESS);
			picWidth=36;
            picHeight=34;
            break;
		}
		case 3: 
		{	GetmHpVarStruct()->iCurAnim=0;
			// Initialisation de "XposCur" et "YposCur"
			GetmHpVarStruct()->YposCur-=32;
			GetmHpVarStruct()->XposCur-=6;
            szBitmap=MAKEINTRESOURCE(IDB_UNPRESS);
			picWidth=36;
            picHeight=34;
            break;
		}
		case 4:
		{	GetmHpVarStruct()->iCurAnim=0;
			// Initialisation de "XposCur" et "YposCur"
			GetmHpVarStruct()->XposCur+=5;
            szBitmap=MAKEINTRESOURCE(IDB_MOUSE);
			picWidth=34;
            picHeight=28;
            break;
		}
		case 5:
		{	GetmHpVarStruct()->bLockAnim=TRUE;
			Delay=150;
            szBitmap=MAKEINTRESOURCE(IDB_ALTGR_PRESS);
			picWidth=ALTGR_KEY_CURSOR_WIDTH;
            picHeight=ALTGR_KEY_CURSOR_HEIGHT;
            break;
		}
		case 6:
		{	GetmHpVarStruct()->bLockAnim=TRUE;
			Delay=150;
            szBitmap=MAKEINTRESOURCE(IDB_SHIFT_PRESS);
			picWidth=SHIFT_KEY_CURSOR_WIDTH;
            picHeight=SHIFT_KEY_CURSOR_HEIGHT;
            break;
		}
		case 7:
		case 8:
		{	GetmHpVarStruct()->iCurAnim=0;
			GetmHpVarStruct()->CursorType=0;
			Delay=4;
            szBitmap=MAKEINTRESOURCE(IDB_DBLCLICK);
			picWidth=70;
            picHeight=51;
            break;
		}
	}
}

//-----------------------------------------------------//
// RestoreBackBuffer
//-----------------------------------------------------//
BOOL RestoreBackBuffer(HWND hWndActive)
{   if(GetmHpVarStruct()->imHpMode==1) return TRUE;
    if(!SetBackGrdBuffer(hWndActive))
        return FALSE;
    return TRUE;
}

//-----------------------------------------------------//
// RestoreKeyBackBuffer
//-----------------------------------------------------//
BOOL RestoreKeyBackBuffer(HWND hWndActive)
{   if(GetmHpVarStruct()->imHpMode==1) return TRUE;
    if((pRenderTarget)&&(pKeyBackBuff))
    {   ID2D1Bitmap *pBmpKeyBack=NULL;
        UINT keyWidth=(bShiftAltGr)? SHIFT_KEY_CURSOR_WIDTH:ALTGR_KEY_CURSOR_WIDTH;
        UINT keyHeight=(bShiftAltGr)? SHIFT_KEY_CURSOR_HEIGHT:ALTGR_KEY_CURSOR_HEIGHT;
        //
        D2D1_SIZE_U size=D2D1::SizeU(keyWidth,keyHeight);
        D2D1_PIXEL_FORMAT pixFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED);
        D2D1_BITMAP_PROPERTIES bmpProperties=D2D1::BitmapProperties(pixFormat,0.0f,0.0f);
        HRESULT hr=pRenderTarget->CreateBitmap(size,bmpProperties,&pBmpKeyBack);
        if(SUCCEEDED(hr))
        {   D2D_RECT_U kbRect=D2D1::RectU(0,0,keyWidth,keyHeight);
            hr=pBmpKeyBack->CopyFromMemory(&kbRect,pKeyBackBuff,keyWidth*3+keyWidth);
        }
        else
        {   delete [] pKeyBackBuff;
            pKeyBackBuff=NULL;
            return InitFail(hWndActive,IDS_D2D_CREATE_BBUFFER);
        }
        if(SUCCEEDED(hr))
        {   //////////////////////////////////////////////////////////////////////////// Direct2D
            pRenderTarget->BeginDraw();
            pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
            D2D1_RECT_F keyRect=D2D1::RectF((float)GET_KEY_CURSOR_POS_X(),
                                            (float)GET_KEY_CURSOR_POS_Y(),
                                            (float)(GET_KEY_CURSOR_POS_X()+keyWidth),
                                            (float)(GET_KEY_CURSOR_POS_Y()+keyHeight));
            pRenderTarget->DrawBitmap(pBmpKeyBack,&keyRect,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
            hr=pRenderTarget->EndDraw();
            if((FAILED(hr))||(hr==D2DERR_RECREATE_TARGET))
            {   delete [] pKeyBackBuff;
                pKeyBackBuff=NULL;
                return InitFail(hWndActive,IDS_D2D_END_DRAW);
            }
            /////////////////////////////////////////////////////////////////////////////////////
        }
        else
        {   delete [] pKeyBackBuff;
            pKeyBackBuff=NULL;
            return InitFail(hWndActive,IDS_D2D_COPY_BBUFFER);
        }
        delete [] pKeyBackBuff;
        pKeyBackBuff=NULL;
    }
    return TRUE;
}

//-----------------------------------------------------//
// UpdateBackBuffer
//-----------------------------------------------------//
void UpdateBackBuffer(BOOL bUpdate)
{   if(GetmHpVarStruct()->imHpMode==1) return;
    bNewRequete=bUpdate; // TRUE: GetBackGrdScreen() will be called at next UpdateFrame() call
                         // FALSE: No GetBackGrdScreen() call
}

//-----------------------------------------------------//
// RemoveBackBuffer
//-----------------------------------------------------//
void RemoveBackBuffer()
{   // Do not display the back buffer at next EndObjects() call (see Menu management)
    CurPosX=NO;
    CurPosY=NO;
}

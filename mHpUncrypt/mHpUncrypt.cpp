// DynUncrypt.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <commdlg.h>
#include <mHpPak.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{	// TODO: Place code here.
	OPENFILENAME OpenFile;
	char szFilter[MAX_KEY_SIZE]={0};
	char szFile[MAX_RS_SIZE]={0};
	strcpy(szFilter,"Fichier mHelp (*.mhf)");
	strcpy(&szFilter[strlen(szFilter)+1],"*.mhf");
	memset(&OpenFile,0,sizeof(OPENFILENAME));
	OpenFile.lStructSize=sizeof(OPENFILENAME);
	OpenFile.hwndOwner=NULL;
	OpenFile.lpstrFilter=szFilter;
	OpenFile.nFilterIndex=1;
	OpenFile.nMaxFile=MAX_RS_SIZE;
	OpenFile.lpstrTitle=NULL;
	OpenFile.lpstrFile=szFile;
	OpenFile.Flags=OFN_ENABLESIZING|OFN_EXPLORER|OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
	if(GetOpenFileName(&OpenFile))
	{	if(mHpPak_DecodemHpFile(NULL,szFile))
		{	MessageBox(NULL,"Succès! Le fichier a été décrypté...","mHelp Message",MB_ICONINFORMATION);
		}
		else MessageBox(NULL,"Erreur! Le fichier n'a pas pu être décrypté...","mHelp Message",MB_ICONSTOP);
	}
	return 0;
}




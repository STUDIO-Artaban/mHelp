unit MainUnit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Wintypes, WinProcs;

// mHelp declarations
procedure mHelpInit(hWndMain: HWND; const AppName: PChar; Shared: Bool); cdecl; external 'mHelp.dll';
procedure mHelpOpen(hWndMain: HWND); cdecl; external 'mHelp.dll';

type
  TMainForm = class(TForm)
    btnClose: TButton;
    btnTest: TButton;
    Label1: TLabel;
    btnmHelp: TButton;
    procedure OnMainActivate(Sender: TObject);
    procedure OnClose(Sender: TObject);
    procedure OnTest(Sender: TObject);
    procedure OnMHelp(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  MainForm: TMainForm;
  nCount: Integer = 0;

implementation

{$R *.dfm}

procedure TMainForm.OnMainActivate(Sender: TObject);
begin
  // mHelp - Init
  mHelpInit(self.Handle, 'AppDELPHI', True);
end;

procedure TMainForm.OnClose(Sender: TObject);
begin
  // Close application
  application.Terminate;
end;

procedure TMainForm.OnTest(Sender: TObject);
begin
  // Test
  nCount := nCount + 1;
  label1.Caption := 'Test... ' + IntToStr(nCount);
end;

procedure TMainForm.OnMHelp(Sender: TObject);
begin
  // mHelp - Open
  mHelpOpen(self.Handle);
end;

end.

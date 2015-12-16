library mHpPas;

uses ShareMem, Controls, Windows, Forms, SysUtils, Classes, Math;

{$R *.res}

function GetDelphiCtrlHandle(hWndActive: HWND; const CtrlName: PChar): HWND; stdcall;
var
  i: Integer;
  CurControl: TWinControl;
begin
  Result := 0;
  for i := 0 to Screen.FormCount - 1 do begin
    if Screen.Forms[i].Handle = hWndActive then begin
      CurControl := TWinControl(Screen.ActiveForm.FindComponent(CtrlName));
      if Assigned(CurControl) then
        Result := CurControl.Handle;
    end;
  end;
end;

function GetDelphiCtrlName(hWndActive, hWndCtrl: HWND; CtrlName: PAnsiChar; NameLen: Integer): Integer; stdcall;
var
  i, j: Integer;
  CurControl: TWinControl;
  strName: AnsiString;
begin
  Result := 0;
  for i := 0 to Screen.FormCount - 1 do begin
    if Screen.Forms[i].Handle = hWndActive then begin
      for j := 0 to Screen.Forms[i].ComponentCount - 1 do begin
        CurControl := TWinControl(Screen.Forms[i].Components[j]);
        if (Assigned(CurControl)) and (CurControl.Handle = hWndCtrl) then begin
          strName := AnsiString(CurControl.Name);
          Result := Min(NameLen, Length(strName));
          Move(strName[1], CtrlName^, Result * SizeOf(AnsiChar));
        end;
      end;
    end;
  end;
end;

exports GetDelphiCtrlHandle, GetDelphiCtrlName;

begin
end.

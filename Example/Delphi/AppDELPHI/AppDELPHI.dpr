program AppDELPHI;

uses
  Forms,
  MainUnit in 'MainUnit.pas' {MainForm};

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'AppDELPHI';
  Application.CreateForm(TMainForm, MainForm);
  Application.Run;
end.

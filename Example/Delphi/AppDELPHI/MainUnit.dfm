object MainForm: TMainForm
  Left = 192
  Top = 124
  Width = 517
  Height = 331
  Caption = 'AppDELPHI'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = OnMainActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 136
    Top = 32
    Width = 31
    Height = 13
    Caption = 'Label1'
  end
  object btnClose: TButton
    Left = 408
    Top = 256
    Width = 83
    Height = 25
    Caption = 'Fermer'
    TabOrder = 0
    OnClick = OnClose
  end
  object btnTest: TButton
    Left = 32
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Test'
    TabOrder = 1
    OnClick = OnTest
  end
  object btnmHelp: TButton
    Left = 320
    Top = 256
    Width = 75
    Height = 25
    Caption = 'mHelp ?!'
    TabOrder = 2
    OnClick = OnMHelp
  end
end

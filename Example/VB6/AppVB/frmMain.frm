VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmMain 
   Caption         =   "SampleVB"
   ClientHeight    =   6180
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   7500
   LinkTopic       =   "Form1"
   ScaleHeight     =   6180
   ScaleWidth      =   7500
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "Frame1"
      Height          =   3375
      Left            =   120
      TabIndex        =   2
      Top             =   480
      Width           =   3975
      Begin VB.Frame Frame2 
         Caption         =   "Frame2"
         Height          =   1335
         Left            =   240
         TabIndex        =   7
         Top             =   1800
         Width           =   3495
         Begin VB.OptionButton Option5 
            Caption         =   "Option5"
            Height          =   255
            Left            =   1680
            TabIndex        =   12
            Top             =   600
            Width           =   1575
         End
         Begin VB.OptionButton Option4 
            Caption         =   "Option4"
            Height          =   255
            Left            =   1680
            TabIndex        =   11
            Top             =   360
            Width           =   1335
         End
         Begin VB.OptionButton Option3 
            Caption         =   "Option3"
            Height          =   255
            Left            =   360
            TabIndex        =   10
            Top             =   840
            Width           =   1575
         End
         Begin VB.OptionButton Option2 
            Caption         =   "Option2"
            Height          =   255
            Left            =   360
            TabIndex        =   9
            Top             =   600
            Width           =   1335
         End
         Begin VB.OptionButton Option1 
            Caption         =   "Option1"
            Height          =   255
            Left            =   360
            TabIndex        =   8
            Top             =   360
            Width           =   1335
         End
      End
      Begin VB.ListBox ListSel 
         Height          =   1425
         Left            =   240
         TabIndex        =   6
         Top             =   360
         Width           =   1815
      End
      Begin VB.TextBox TextAdd 
         Height          =   285
         Left            =   2160
         TabIndex        =   5
         Top             =   360
         Width           =   1575
      End
      Begin VB.CommandButton CmdRemove 
         Caption         =   "Remove"
         Height          =   375
         Left            =   2160
         TabIndex        =   4
         Top             =   1320
         Width           =   1575
      End
      Begin VB.CommandButton CmdAdd 
         Caption         =   "Add"
         Height          =   375
         Left            =   2160
         TabIndex        =   3
         Top             =   840
         Width           =   1575
      End
   End
   Begin MSComctlLib.Toolbar tbToolBar 
      Align           =   1  'Align Top
      Height          =   420
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   7500
      _ExtentX        =   13229
      _ExtentY        =   741
      ButtonWidth     =   609
      ButtonHeight    =   582
      Appearance      =   1
      ImageList       =   "imlToolbarIcons"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   17
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Nouveau"
            Object.ToolTipText     =   "Nouveau"
            ImageKey        =   "New"
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Ouvrir"
            Object.ToolTipText     =   "Ouvrir"
            ImageKey        =   "Open"
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Enregistrer"
            Object.ToolTipText     =   "Enregistrer"
            ImageKey        =   "Save"
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Imprimer"
            Object.ToolTipText     =   "Imprimer"
            ImageKey        =   "Print"
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Couper"
            Object.ToolTipText     =   "Couper"
            ImageKey        =   "Cut"
         EndProperty
         BeginProperty Button8 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Copier"
            Object.ToolTipText     =   "Copier"
            ImageKey        =   "Copy"
         EndProperty
         BeginProperty Button9 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Coller"
            Object.ToolTipText     =   "Coller"
            ImageKey        =   "Paste"
         EndProperty
         BeginProperty Button10 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button11 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Gras"
            Object.ToolTipText     =   "Gras"
            ImageKey        =   "Bold"
         EndProperty
         BeginProperty Button12 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Italique"
            Object.ToolTipText     =   "Italique"
            ImageKey        =   "Italic"
         EndProperty
         BeginProperty Button13 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Souligné"
            Object.ToolTipText     =   "Souligné"
            ImageKey        =   "Underline"
         EndProperty
         BeginProperty Button14 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button15 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Aligner à gauche"
            Object.ToolTipText     =   "Aligner à gauche"
            ImageKey        =   "Align Left"
            Style           =   2
         EndProperty
         BeginProperty Button16 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Centrer"
            Object.ToolTipText     =   "Centrer"
            ImageKey        =   "Center"
            Style           =   2
         EndProperty
         BeginProperty Button17 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "Aligner à droite"
            Object.ToolTipText     =   "Aligner à droite"
            ImageKey        =   "Align Right"
            Style           =   2
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.StatusBar sbStatusBar 
      Align           =   2  'Align Bottom
      Height          =   270
      Left            =   0
      TabIndex        =   0
      Top             =   5910
      Width           =   7500
      _ExtentX        =   13229
      _ExtentY        =   476
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   3
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   7567
            Text            =   "État"
            TextSave        =   "État"
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   6
            AutoSize        =   2
            TextSave        =   "19/02/2003"
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   5
            AutoSize        =   2
            TextSave        =   "14:50"
         EndProperty
      EndProperty
   End
   Begin MSComDlg.CommonDialog dlgCommonDialog 
      Left            =   5760
      Top             =   3000
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComctlLib.ImageList imlToolbarIcons 
      Left            =   5640
      Top             =   3600
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   13
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0000
            Key             =   "New"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0112
            Key             =   "Open"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0224
            Key             =   "Save"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0336
            Key             =   "Print"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0448
            Key             =   "Cut"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":055A
            Key             =   "Copy"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":066C
            Key             =   "Paste"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":077E
            Key             =   "Bold"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0890
            Key             =   "Italic"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":09A2
            Key             =   "Underline"
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0AB4
            Key             =   "Align Left"
         EndProperty
         BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0BC6
            Key             =   "Center"
         EndProperty
         BeginProperty ListImage13 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0CD8
            Key             =   "Align Right"
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&Fichier"
      Begin VB.Menu mnuFileNew 
         Caption         =   "&Nouveau"
         Shortcut        =   ^N
      End
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Ouvrir"
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Fermer"
      End
      Begin VB.Menu mnuFileBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "&Enregistrer"
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "Enregistrer &sous..."
      End
      Begin VB.Menu mnuFileSaveAll 
         Caption         =   "Enregistrer &tout"
      End
      Begin VB.Menu mnuFileBar1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileProperties 
         Caption         =   "&Propriétés"
      End
      Begin VB.Menu mnuFileBar2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFilePageSetup 
         Caption         =   "&Mise en page..."
      End
      Begin VB.Menu mnuFilePrintPreview 
         Caption         =   "&Aperçu avant impression"
      End
      Begin VB.Menu mnuFilePrint 
         Caption         =   "&Imprimer..."
      End
      Begin VB.Menu mnuFileBar3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileSend 
         Caption         =   "En&voyer..."
      End
      Begin VB.Menu mnuFileBar4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileMRU 
         Caption         =   ""
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileMRU 
         Caption         =   ""
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileMRU 
         Caption         =   ""
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileBar5 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "&Quitter"
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "&Edition"
      Begin VB.Menu mnuEditUndo 
         Caption         =   "&Annuler"
      End
      Begin VB.Menu mnuEditBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEditCut 
         Caption         =   "&Couper"
         Shortcut        =   ^X
      End
      Begin VB.Menu mnuEditCopy 
         Caption         =   "C&opier"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuEditPaste 
         Caption         =   "Co&ller"
         Shortcut        =   ^V
      End
      Begin VB.Menu mnuEditPasteSpecial 
         Caption         =   "Colla&ge spécial..."
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "Affic&hage"
      Begin VB.Menu mnuViewToolbar 
         Caption         =   "&Barre d'outils"
         Checked         =   -1  'True
      End
      Begin VB.Menu mnuViewStatusBar 
         Caption         =   "B&arre d'état"
         Checked         =   -1  'True
      End
      Begin VB.Menu mnuViewBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewRefresh 
         Caption         =   "Actualise&r"
      End
      Begin VB.Menu mnuViewOptions 
         Caption         =   "&Options..."
      End
      Begin VB.Menu mnuViewWebBrowser 
         Caption         =   "Navigateur &Web"
      End
   End
   Begin VB.Menu mnuMenu 
      Caption         =   "Menu"
      Begin VB.Menu mnuitem1 
         Caption         =   "Menu item1"
      End
      Begin VB.Menu mnuItem2 
         Caption         =   "Menu item2"
      End
      Begin VB.Menu mnuItem3 
         Caption         =   "Menu item3"
      End
      Begin VB.Menu pmnu1 
         Caption         =   "Pop-up menu1"
         Begin VB.Menu mnuItem4 
            Caption         =   "Menu item4"
         End
         Begin VB.Menu mnuItem5 
            Caption         =   "Menu item5"
         End
         Begin VB.Menu pmnu2 
            Caption         =   "Pop-up menu2"
            Begin VB.Menu mnuitem6 
               Caption         =   "Menu item6"
            End
            Begin VB.Menu mnuitem7 
               Caption         =   "Menu item7"
            End
            Begin VB.Menu mnuitem8 
               Caption         =   "Menu item8"
            End
            Begin VB.Menu mnuitem9 
               Caption         =   "Menu item9"
            End
            Begin VB.Menu pmnu3 
               Caption         =   "Pop-up menu3"
               Begin VB.Menu mnuitem10 
                  Caption         =   "Menu item10"
               End
               Begin VB.Menu mnuitem11 
                  Caption         =   "Menu item11"
               End
               Begin VB.Menu mnuitem12 
                  Caption         =   "Menu item12"
               End
               Begin VB.Menu pmnu4 
                  Caption         =   "Pop-up menu4"
                  Begin VB.Menu mnuitem13 
                     Caption         =   "Menu item13"
                  End
                  Begin VB.Menu mnuitem14 
                     Caption         =   "Menu item14"
                  End
                  Begin VB.Menu mnuitem15 
                     Caption         =   "Menu item15"
                  End
                  Begin VB.Menu mnuitem16 
                     Caption         =   "Menu item16"
                  End
                  Begin VB.Menu mnusep1 
                     Caption         =   "-"
                  End
                  Begin VB.Menu mnuitem17 
                     Caption         =   "Menu item17"
                  End
               End
            End
         End
      End
      Begin VB.Menu mnusep3012 
         Caption         =   "-"
      End
      Begin VB.Menu mnuother 
         Caption         =   "Other dialog test"
      End
      Begin VB.Menu mnuLnchFavTask 
         Caption         =   "Launch first ""Appvb.dhf"" fav. task"
      End
   End
   Begin VB.Menu mnudlgtest 
      Caption         =   "Dialog test"
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&?"
      Begin VB.Menu mnuHelpContents 
         Caption         =   "&Sommaire"
      End
      Begin VB.Menu mnuHelpSearchForHelpOn 
         Caption         =   "A&ide sur..."
      End
      Begin VB.Menu mnuDynHelp 
         Caption         =   "DynHelp ?!"
      End
      Begin VB.Menu mnuHelpBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "À &propos de "
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Declare Function OSWinHelp% Lib "user32" Alias "WinHelpA" (ByVal hwnd&, ByVal HelpFile$, ByVal wCommand%, dwData As Any)

Private StockStr() As String

Private Sub RefreshStock()
    Dim i As Integer
    ReDim StockStr(0)
    For i = 0 To ListSel.ListCount - 1
        ReDim Preserve StockStr(UBound(StockStr) + 1)
        StockStr(i) = ListSel.List(i)
    Next
End Sub

Private Function StrNotFound(ByVal NewStr As String) As Boolean
    Dim i As Integer
    Dim bNoExist As Boolean
    bNoExist = True
    For i = 0 To UBound(StockStr)
        If StockStr(i) = NewStr Then
            bNoExist = False
        End If
    Next i
    StrNotFound = bNoExist
End Function

Private Sub CmdAdd_Click()
    If Trim(TextAdd.Text) <> "" And StrNotFound(Trim(TextAdd.Text)) = True Then
        ListSel.AddItem (Trim(TextAdd.Text))
        ReDim Preserve StockStr(UBound(StockStr) + 1)
        StockStr(UBound(StockStr)) = Trim(TextAdd.Text)
    End If
End Sub

Private Sub CmdRemove_Click()
    Dim i As Integer
    Dim iSel As Integer
    iSel = -1
    For i = 0 To ListSel.ListCount - 1
        If ListSel.Selected(i) Then
            iSel = i
        End If
    Next i
    If iSel <> -1 Then
        ListSel.RemoveItem iSel
        Call RefreshStock
    End If
End Sub

Private Sub Form_Activate()
    '******************************************************'
    DynHelp_MainActive Me.hwnd, "AppVB", True
    '******************************************************'
End Sub

Private Sub Form_Load()
    Me.Left = 1000
    Me.Top = 1000
    CmdRemove.Enabled = True
    ReDim StockStr(0)
    Option2.Value = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Dim i As Integer
    'close all sub forms
    For i = Forms.Count - 1 To 1 Step -1
        Unload Forms(i)
    Next
    If Me.WindowState <> vbMinimized Then
        SaveSetting App.Title, "Settings", "MainLeft", Me.Left
        SaveSetting App.Title, "Settings", "MainTop", Me.Top
        SaveSetting App.Title, "Settings", "MainWidth", Me.Width
        SaveSetting App.Title, "Settings", "MainHeight", Me.Height
    End If
End Sub

Private Sub mnuDynHelp_Click()
    '*************************************************'
    DynHelp_Create Me.hwnd
    '*************************************************'
End Sub

Private Sub mnudlgtest_Click()
    Form1.Show vbModal
End Sub

Private Sub mnuitem17_Click()
    MsgBox "Menus and Pop-Up menus test!"
End Sub

Private Sub mnuLnchFavTask_Click()
    Dim iDynError As Integer
    Dim lpErrorMsg As String
    '******************************************************'
    iDynError = DynHelp_LaunchFavTask(Me.hwnd, "Appvb.dhf", 0)
    '******************************************************'
    If iDynError <> 0 Then
        lpErrorMsg = Space(100)
        '******************************************************'
        DynHelp_ErrorMsg iDynError, lpErrorMsg, 100
        '******************************************************'
        MsgBox lpErrorMsg, vbExclamation, "DynHelp Error Message"
    End If
End Sub

Private Sub mnuother_Click()
    Form2.Show vbModal
End Sub

Private Sub tbToolBar_ButtonClick(ByVal Button As MSComCtlLib.Button)
    On Error Resume Next
    Select Case Button.Key
        Case "Nouveau"
            'À faire: Ajouter le code du bouton 'Nouveau'.
            MsgBox "Ajouter le code du bouton 'Nouveau'."
        Case "Ouvrir"
            mnuFileOpen_Click
        Case "Enregistrer"
            mnuFileSave_Click
        Case "Imprimer"
            mnuFilePrint_Click
        Case "Couper"
            mnuEditCut_Click
        Case "Copier"
            mnuEditCopy_Click
        Case "Coller"
            mnuEditPaste_Click
        Case "Gras"
            'À faire: Ajouter le code du bouton 'Gras'.
            MsgBox "Ajouter le code du bouton 'Gras'."
        Case "Italique"
            'À faire: Ajouter le code du bouton 'Italique'.
            MsgBox "Ajouter le code du bouton 'Italique'."
        Case "Souligné"
            'À faire: Ajouter le code du bouton 'Souligné'.
            MsgBox "Ajouter le code du bouton 'Souligné'."
        Case "Aligner à gauche"
            'À faire: Ajouter le code du bouton 'Aligner à gauche'.
            'MsgBox "Ajouter le code du bouton 'Aligner à gauche'."
        Case "Centrer"
            'À faire: Ajouter le code du bouton 'Centrer'.
            'MsgBox "Ajouter le code du bouton 'Centrer'."
        Case "Aligner à droite"
            'À faire: Ajouter le code du bouton 'Aligner à droite'.
            'MsgBox "Ajouter le code du bouton 'Aligner à droite'."
    End Select
End Sub

Private Sub mnuHelpAbout_Click()
    MsgBox "Version " & App.Major & "." & App.Minor & "." & App.Revision
End Sub

Private Sub mnuHelpSearchForHelpOn_Click()
    Dim nRet As Integer


    'S'il n'y a pas de fichier d'aide pour le projet, afficher un message à l'utilisateur
    'afin de définir le fichier d'aide de votre application dans le
    'dialogue des propriétés du projet
    If Len(App.HelpFile) = 0 Then
        MsgBox "Impossible d'afficher le sommaire de l'Aide. Aucune Aide n'est associée à ce projet.", vbInformation, Me.Caption
    Else
        On Error Resume Next
        nRet = OSWinHelp(Me.hwnd, App.HelpFile, 261, 0)
        If Err Then
            MsgBox Err.Description
        End If
    End If

End Sub

Private Sub mnuHelpContents_Click()
    Dim nRet As Integer


    'S'il n'y a pas de fichier d'aide pour le projet, afficher un message à l'utilisateur
    'afin de définir le fichier d'aide de votre application dans le
    'dialogue des propriétés du projet
    If Len(App.HelpFile) = 0 Then
        MsgBox "Impossible d'afficher le sommaire de l'Aide. Aucune Aide n'est associée à ce projet.", vbInformation, Me.Caption
    Else
        On Error Resume Next
        nRet = OSWinHelp(Me.hwnd, App.HelpFile, 3, 0)
        If Err Then
            MsgBox Err.Description
        End If
    End If

End Sub


Private Sub mnuViewWebBrowser_Click()
    'À faire: Ajouter le code de 'mnuViewWebBrowser_Click'.
    MsgBox "Ajouter le code de 'mnuViewWebBrowser_Click'."
End Sub

Private Sub mnuViewOptions_Click()
    'À faire: Ajouter le code de 'mnuViewOptions_Click'.
    MsgBox "Ajouter le code de 'mnuViewOptions_Click'."
End Sub

Private Sub mnuViewRefresh_Click()
    'À faire: Ajouter le code de 'mnuViewRefresh_Click'.
    MsgBox "Ajouter le code de 'mnuViewRefresh_Click'."
End Sub

Private Sub mnuViewStatusBar_Click()
    mnuViewStatusBar.Checked = Not mnuViewStatusBar.Checked
    sbStatusBar.Visible = mnuViewStatusBar.Checked
End Sub

Private Sub mnuViewToolbar_Click()
    mnuViewToolbar.Checked = Not mnuViewToolbar.Checked
    tbToolBar.Visible = mnuViewToolbar.Checked
End Sub

Private Sub mnuEditPasteSpecial_Click()
    'À faire: Ajouter le code de 'mnuEditPasteSpecial_Click'.
    MsgBox "Ajouter le code de 'mnuEditPasteSpecial_Click'."
End Sub

Private Sub mnuEditPaste_Click()
    'À faire: Ajouter le code de 'mnuEditPaste_Click'.
    MsgBox "Ajouter le code de 'mnuEditPaste_Click'."
End Sub

Private Sub mnuEditCopy_Click()
    'À faire: Ajouter le code de 'mnuEditCopy_Click'.
    MsgBox "Ajouter le code de 'mnuEditCopy_Click'."
End Sub

Private Sub mnuEditCut_Click()
    'À faire: Ajouter le code de 'mnuEditCut_Click'.
    MsgBox "Ajouter le code de 'mnuEditCut_Click'."
End Sub

Private Sub mnuEditUndo_Click()
    'À faire: Ajouter le code de 'mnuEditUndo_Click'.
    MsgBox "Ajouter le code de 'mnuEditUndo_Click'."
End Sub

Private Sub mnuFileExit_Click()
    'décharge la feuille
    Unload Me

End Sub

Private Sub mnuFileSend_Click()
    'À faire: Ajouter le code de 'mnuFileSend_Click'.
    MsgBox "Ajouter le code de 'mnuFileSend_Click'."
End Sub

Private Sub mnuFilePrint_Click()
    'À faire: Ajouter le code de 'mnuFilePrint_Click'.
    MsgBox "Ajouter le code de 'mnuFilePrint_Click'."
End Sub

Private Sub mnuFilePrintPreview_Click()
    'À faire: Ajouter le code de 'mnuFilePrintPreview_Click'.
    MsgBox "Ajouter le code de 'mnuFilePrintPreview_Click'."
End Sub

Private Sub mnuFilePageSetup_Click()
    On Error Resume Next
    With dlgCommonDialog
        .DialogTitle = "Mise en page"
        .CancelError = True
        .ShowPrinter
    End With

End Sub

Private Sub mnuFileProperties_Click()
    'À faire: Ajouter le code de 'mnuFileProperties_Click'.
    MsgBox "Ajouter le code de 'mnuFileProperties_Click'."
End Sub

Private Sub mnuFileSaveAll_Click()
    'À faire: Ajouter le code de 'mnuFileSaveAll_Click'.
    MsgBox "Ajouter le code de 'mnuFileSaveAll_Click'."
End Sub

Private Sub mnuFileSaveAs_Click()
    'À faire: Ajouter le code de 'mnuFileSaveAs_Click'.
    MsgBox "Ajouter le code de 'mnuFileSaveAs_Click'."
End Sub

Private Sub mnuFileSave_Click()
    'À faire: Ajouter le code de 'mnuFileSave_Click'.
    MsgBox "Ajouter le code de 'mnuFileSave_Click'."
End Sub

Private Sub mnuFileClose_Click()
    'À faire: Ajouter le code de 'mnuFileClose_Click'.
    MsgBox "Ajouter le code de 'mnuFileClose_Click'."
End Sub

Private Sub mnuFileOpen_Click()
    Dim sFile As String


    With dlgCommonDialog
        .DialogTitle = "Ouvrir"
        .CancelError = False
        'À faire : Définir les indicateurs et les attributs du contrôle des dialogues communs
        .Filter = "Tous les fichiers (*.*)|*.*"
        .ShowOpen
        If Len(.FileName) = 0 Then
            Exit Sub
        End If
        sFile = .FileName
    End With
    'À faire : Ajouter le code de traitement du fichier ouvert

End Sub

Private Sub mnuFileNew_Click()
    'À faire: Ajouter le code de 'mnuFileNew_Click'.
    MsgBox "Ajouter le code de 'mnuFileNew_Click'."
End Sub


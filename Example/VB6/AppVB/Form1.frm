VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Form1"
   ClientHeight    =   5130
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8130
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5130
   ScaleWidth      =   8130
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "Frame1"
      Height          =   4575
      Left            =   120
      TabIndex        =   1
      Top             =   0
      Width           =   7935
      Begin VB.Frame Frame4 
         Caption         =   "Frame4"
         Height          =   1335
         Left            =   240
         TabIndex        =   16
         Top             =   3000
         Width           =   3735
         Begin VB.TextBox Text3 
            Height          =   375
            Left            =   240
            Locked          =   -1  'True
            TabIndex        =   18
            Text            =   "Text3"
            Top             =   720
            Width           =   3255
         End
         Begin VB.ComboBox Combo1 
            Height          =   315
            Left            =   240
            Style           =   2  'Dropdown List
            TabIndex        =   17
            Top             =   360
            Width           =   3255
         End
      End
      Begin VB.ListBox List2 
         Height          =   2010
         Left            =   2160
         TabIndex        =   12
         Top             =   360
         Width           =   1815
      End
      Begin VB.TextBox Text1 
         Height          =   435
         Left            =   240
         TabIndex        =   4
         Text            =   "Text1"
         Top             =   2520
         Width           =   3735
      End
      Begin VB.Frame Frame2 
         Caption         =   "Frame2"
         Height          =   4095
         Left            =   4080
         TabIndex        =   3
         Top             =   240
         Width           =   3615
         Begin VB.TextBox Text2 
            Height          =   375
            Left            =   240
            Locked          =   -1  'True
            TabIndex        =   13
            Text            =   "Text2"
            Top             =   3480
            Width           =   615
         End
         Begin VB.Frame Frame3 
            Caption         =   "Frame3"
            Height          =   3615
            Left            =   960
            TabIndex        =   6
            Top             =   240
            Width           =   2415
            Begin VB.OptionButton Option7 
               Caption         =   "Option7"
               Height          =   255
               Left            =   480
               TabIndex        =   15
               Top             =   2640
               Width           =   1455
            End
            Begin VB.OptionButton Option6 
               Caption         =   "Option6"
               Height          =   255
               Left            =   480
               TabIndex        =   14
               Top             =   2280
               Width           =   1575
            End
            Begin VB.OptionButton Option5 
               Caption         =   "Option5"
               Height          =   255
               Left            =   480
               TabIndex        =   11
               Top             =   1920
               Width           =   1695
            End
            Begin VB.OptionButton Option4 
               Caption         =   "Option4"
               Height          =   255
               Left            =   480
               TabIndex        =   10
               Top             =   1560
               Width           =   1695
            End
            Begin VB.OptionButton Option3 
               Caption         =   "Option3"
               Height          =   255
               Left            =   480
               TabIndex        =   9
               Top             =   1200
               Width           =   1695
            End
            Begin VB.OptionButton Option2 
               Caption         =   "Option2"
               Height          =   255
               Left            =   480
               TabIndex        =   8
               Top             =   840
               Width           =   1695
            End
            Begin VB.OptionButton Option1 
               Caption         =   "Option1"
               Height          =   255
               Left            =   480
               TabIndex        =   7
               Top             =   480
               Width           =   1695
            End
         End
         Begin MSComctlLib.Slider Slider1 
            Height          =   3015
            Left            =   240
            TabIndex        =   5
            Top             =   360
            Width           =   630
            _ExtentX        =   1111
            _ExtentY        =   5318
            _Version        =   393216
            Orientation     =   1
            Max             =   20
         End
      End
      Begin VB.ListBox List1 
         Height          =   2010
         Left            =   240
         MultiSelect     =   1  'Simple
         TabIndex        =   2
         Top             =   360
         Width           =   1815
      End
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Close"
      Height          =   375
      Left            =   6840
      TabIndex        =   0
      Top             =   4680
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private iOption As Integer
Private iComboSel As Integer
Private sText As String
Private iSliderPos As Integer
Private iListSel() As Integer

Private Sub Combo1_Click()
    Text3.Text = Combo1.List(Combo1.ListIndex)
End Sub

Private Sub Command1_Click()
    Dim i As Integer
    Dim iSize As Integer
    sText = Text1.Text
    If Option1.Value = True Then iOption = 1
    If Option2.Value = True Then iOption = 2
    If Option3.Value = True Then iOption = 3
    If Option4.Value = True Then iOption = 4
    If Option5.Value = True Then iOption = 5
    If Option6.Value = True Then iOption = 6
    If Option7.Value = True Then iOption = 7
    iComboSel = Combo1.ListIndex
    iSliderPos = Slider1.Value
    iSize = 0
    For i = 0 To List1.ListCount - 1
        If List1.Selected(i) = True Then
            ReDim Preserve iListSel(iSize)
            iListSel(iSize) = i
            iSize = iSize + 1
        End If
    Next i
    Unload Me
End Sub

Private Sub Form_Load()
    Dim i As Integer
    List1.AddItem "StringSel1"
    List1.AddItem "StringSel2"
    List1.AddItem "StringSel3"
    List1.AddItem "StringSel4"
    List1.AddItem "StringSel5"
    List1.AddItem "StringSel6"
    List1.AddItem "StringSel7"
    List1.AddItem "StringSel8"
    List1.AddItem "StringSel9"
    List1.AddItem "StringSel10"
    List1.AddItem "StringSel11"
    List1.AddItem "StringSel12"
    List1.AddItem "StringSel13"
    List1.AddItem "StringSel14"
    List1.AddItem "StringSel15"
    List1.AddItem "StringSel16"
    List1.AddItem "StringSel17"
    List1.AddItem "StringSel18"
    List1.AddItem "StringSel19"
    List1.AddItem "StringSel20"
    List1.AddItem "StringSel21"
    List1.AddItem "StringSel22"
    List1.AddItem "StringSel23"
    List1.AddItem "StringSel24"
    List1.AddItem "StringSel25"
    List1.AddItem "StringSel26"
    List1.AddItem "StringSel27"
    Combo1.AddItem "ComboSel1"
    Combo1.AddItem "ComboSel2"
    Combo1.AddItem "ComboSel3"
    Combo1.AddItem "ComboSel4"
    Combo1.AddItem "ComboSel5"
    Combo1.AddItem "ComboSel6"
    Combo1.AddItem "ComboSel7"
    Combo1.AddItem "ComboSel8"
    If bPass = False Then
        Combo1.ListIndex = 3
        Option2.Value = True
        Text1.Text = "Edit box"
        Text2.Text = "0"
        Text3.Text = "ComboSel4"
        sText = Text1.Text
        iOption = 2
        iComboSel = 3
        iSliderPos = 0
        ReDim iListSel(0)
        bPass = True
    Else
        Select Case iOption
        Case 1
            Option1.Value = True
        Case 2
            Option2.Value = True
        Case 3
            Option3.Value = True
        Case 4
            Option4.Value = True
        Case 5
            Option5.Value = True
        Case 6
            Option6.Value = True
        Case 7
            Option7.Value = True
        End Select
        Slider1.Value = iSliderPos
        Combo1.ListIndex = iComboSel
        Text1.Text = sText
        Text2.Text = Format(iSliderPos)
        Text3.Text = Combo1.List(iComboSel)
        For i = 0 To UBound(iListSel)
            List1.Selected(iListSel(i)) = True
        Next i
    End If
End Sub

Private Sub List1_Click()
    Dim i As Integer
    List2.Clear
    For i = 0 To List1.ListCount - 1
        If List1.Selected(i) = True Then
            List2.AddItem List1.List(i)
        End If
    Next i
End Sub

Private Sub Slider1_Scroll()
    Text2.Text = Format(Slider1.Value)
End Sub

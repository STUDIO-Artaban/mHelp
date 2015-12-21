VERSION 5.00
Begin VB.Form Form2 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Form2"
   ClientHeight    =   3195
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3960
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3195
   ScaleWidth      =   3960
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2760
      TabIndex        =   2
      Top             =   720
      Width           =   1095
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Ok"
      Height          =   375
      Left            =   2760
      TabIndex        =   1
      Top             =   240
      Width           =   1095
   End
   Begin VB.Frame Frame1 
      Caption         =   "Frame1"
      Height          =   2895
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2535
      Begin VB.CommandButton Command4 
         Caption         =   "Clear"
         Height          =   375
         Left            =   1320
         TabIndex        =   5
         Top             =   2400
         Width           =   975
      End
      Begin VB.CommandButton Command3 
         Caption         =   "Add"
         Height          =   375
         Left            =   240
         TabIndex        =   4
         Top             =   2400
         Width           =   975
      End
      Begin VB.ComboBox Combo1 
         Height          =   1935
         Left            =   240
         Style           =   1  'Simple Combo
         TabIndex        =   3
         Text            =   "Combo1"
         Top             =   360
         Width           =   2055
      End
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command1_Click()
    StockModif Combo1
    Unload Me
End Sub

Private Sub Command2_Click()
    StockModif Combo1
    Unload Me
End Sub

Private Sub Command3_Click()
    Dim bIn As Boolean
    Dim i As Integer
    If Combo1.Text <> "" Then
        bIn = False
        For i = 0 To Combo1.ListCount - 1
            If Combo1.Text = Combo1.List(i) Then
                bIn = True
            End If
        Next i
        If bIn = False Then
            Combo1.AddItem Combo1.Text
        End If
    End If
End Sub

Private Sub Command4_Click()
    Combo1.Clear
End Sub

Private Sub Form_Load()
    Combo1.Text = ""
    ReplaceModif Combo1
End Sub

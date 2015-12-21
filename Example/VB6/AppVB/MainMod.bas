Attribute VB_Name = "MainModule"
Option Explicit

Public fMainForm As frmMain
Public CboSel() As String
Global bPass As Boolean

Public Sub ReplaceModif(Cbo As ComboBox)
    Dim i As Integer
    Cbo.Clear
    For i = 0 To UBound(CboSel)
        If CboSel(i) <> "" Then
            Cbo.AddItem CboSel(i)
        End If
    Next i
End Sub

Public Sub StockModif(Cbo As ComboBox)
    Dim i As Integer
    ReDim CboSel(0)
    For i = 0 To Cbo.ListCount - 1
        ReDim Preserve CboSel(i)
        If Cbo.List(i) <> "" Then
            CboSel(i) = Cbo.List(i)
        End If
    Next i
End Sub

Sub Main()
    Set fMainForm = New frmMain
    ReDim CboSel(0)
    bPass = False
    fMainForm.Show
End Sub


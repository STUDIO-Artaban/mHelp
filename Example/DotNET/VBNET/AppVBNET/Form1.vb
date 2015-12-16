Option Explicit On

Public Class Form1

    Private iCount As Integer = 0

    Private Sub Form1_Activated(sender As System.Object, e As System.EventArgs) Handles MyBase.Activated
        CmHelp.MainActive(Me.Handle, "AppVBNET", True)
    End Sub

    Private Sub bntmHelp_Click(sender As System.Object, e As System.EventArgs) Handles bntmHelp.Click
        CmHelp.Create(Me.Handle)
    End Sub

    Private Sub bntClose_Click(sender As System.Object, e As System.EventArgs) Handles bntClose.Click
        Me.Close()
    End Sub

    Private Sub chckBox_CheckedChanged(sender As System.Object, e As System.EventArgs) Handles chckBox.CheckedChanged
        iCount += 1
        lblTest.Text = "CheckedChanged..." + iCount.ToString()
    End Sub

    Private Sub chckBox_CheckStateChanged(sender As System.Object, e As System.EventArgs) Handles chckBox.CheckStateChanged
        iCount += 1
        lblTest.Text = "CheckStateChanged..." + iCount.ToString()
    End Sub

    Private Sub chckBox3_CheckedChanged(sender As System.Object, e As System.EventArgs) Handles chckBox3.CheckedChanged
        iCount += 1
        lblTest.Text = "CheckedChanged 3..." + iCount.ToString()
    End Sub

    Private Sub chckBox3_CheckStateChanged(sender As System.Object, e As System.EventArgs) Handles chckBox3.CheckStateChanged
        iCount += 1
        lblTest.Text = "CheckStateChanged 3..." + iCount.ToString()
    End Sub

    Private Sub bntTest_Click(sender As System.Object, e As System.EventArgs) Handles bntTest.Click
        iCount += 1
        lblTest.Text = "Test..." + iCount.ToString()

        MsgBox("Test", MsgBoxStyle.OkCancel, "Test")
    End Sub

End Class

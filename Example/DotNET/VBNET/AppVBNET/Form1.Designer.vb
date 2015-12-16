<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.bntTest = New System.Windows.Forms.Button()
        Me.lblTest = New System.Windows.Forms.Label()
        Me.bntClose = New System.Windows.Forms.Button()
        Me.bntmHelp = New System.Windows.Forms.Button()
        Me.chckBox = New System.Windows.Forms.CheckBox()
        Me.chckBox3 = New System.Windows.Forms.CheckBox()
        Me.SuspendLayout()
        '
        'bntTest
        '
        Me.bntTest.Location = New System.Drawing.Point(12, 12)
        Me.bntTest.Name = "bntTest"
        Me.bntTest.Size = New System.Drawing.Size(75, 23)
        Me.bntTest.TabIndex = 0
        Me.bntTest.Text = "Test"
        Me.bntTest.UseVisualStyleBackColor = True
        '
        'lblTest
        '
        Me.lblTest.AutoSize = True
        Me.lblTest.Location = New System.Drawing.Point(109, 17)
        Me.lblTest.Name = "lblTest"
        Me.lblTest.Size = New System.Drawing.Size(39, 13)
        Me.lblTest.TabIndex = 1
        Me.lblTest.Text = "Label1"
        '
        'bntClose
        '
        Me.bntClose.Location = New System.Drawing.Point(223, 250)
        Me.bntClose.Name = "bntClose"
        Me.bntClose.Size = New System.Drawing.Size(75, 23)
        Me.bntClose.TabIndex = 2
        Me.bntClose.Text = "Fermer"
        Me.bntClose.UseVisualStyleBackColor = True
        '
        'bntmHelp
        '
        Me.bntmHelp.Location = New System.Drawing.Point(142, 250)
        Me.bntmHelp.Name = "bntmHelp"
        Me.bntmHelp.Size = New System.Drawing.Size(75, 23)
        Me.bntmHelp.TabIndex = 3
        Me.bntmHelp.Text = "mHelp ?!"
        Me.bntmHelp.UseVisualStyleBackColor = True
        '
        'chckBox
        '
        Me.chckBox.AutoSize = True
        Me.chckBox.Location = New System.Drawing.Point(12, 51)
        Me.chckBox.Name = "chckBox"
        Me.chckBox.Size = New System.Drawing.Size(75, 17)
        Me.chckBox.TabIndex = 4
        Me.chckBox.Text = "CheckBox"
        Me.chckBox.UseVisualStyleBackColor = True
        '
        'chckBox3
        '
        Me.chckBox3.AutoSize = True
        Me.chckBox3.Location = New System.Drawing.Point(12, 74)
        Me.chckBox3.Name = "chckBox3"
        Me.chckBox3.Size = New System.Drawing.Size(84, 17)
        Me.chckBox3.TabIndex = 5
        Me.chckBox3.Text = "CheckBox 3"
        Me.chckBox3.ThreeState = True
        Me.chckBox3.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(310, 285)
        Me.Controls.Add(Me.chckBox3)
        Me.Controls.Add(Me.chckBox)
        Me.Controls.Add(Me.bntmHelp)
        Me.Controls.Add(Me.bntClose)
        Me.Controls.Add(Me.lblTest)
        Me.Controls.Add(Me.bntTest)
        Me.Name = "Form1"
        Me.Text = "AppVBNET"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents bntTest As System.Windows.Forms.Button
    Friend WithEvents lblTest As System.Windows.Forms.Label
    Friend WithEvents bntClose As System.Windows.Forms.Button
    Friend WithEvents bntmHelp As System.Windows.Forms.Button
    Friend WithEvents chckBox As System.Windows.Forms.CheckBox
    Friend WithEvents chckBox3 As System.Windows.Forms.CheckBox

End Class

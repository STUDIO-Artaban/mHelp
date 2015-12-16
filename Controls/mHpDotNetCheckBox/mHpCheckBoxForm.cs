/// <summary>
/// mHpCheckBoxForm.cs - Form definition for mHelp .NET CheckBox control state
/// Date - 01/15/2013
/// 
/// Copyright(c) - Unknown Corporation. All rights reserved.
/// </summary>

using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace mHpDotNetCheckBox
{
    /// <summary>
    /// Class mHpCheckBoxForm - mHelp .NET CheckBox control state Form class
    /// </summary>
    public partial class mHpCheckBoxForm : Form
    {
        [DllImport("User32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool EnableWindow(IntPtr hWnd, bool bEnable);

        private TextBox edtState;
        private Label label1;
        private RadioButton rdoUndef;
        private RadioButton rdoUncheck;
        private RadioButton rdoCheck;

        /// <summary>
        /// hApplyBtn - Handle of the button that save state change
        /// </summary>
        private IntPtr hApplyBtn;

        ////// Enable button to apply change on CheckBox state (when state config has changed)
        private void rdoUncheck_CheckedChanged(object sender, EventArgs e)
        {
            EnableWindow(hApplyBtn, true);
            CheckBoxState = mHpCheckBoxState.UNCHECKED;
            edtState.Text = CheckBoxState.ToString();
        }
        private void rdoCheck_CheckedChanged(object sender, EventArgs e)
        {
            EnableWindow(hApplyBtn, true);
            CheckBoxState = mHpCheckBoxState.CHECKED;
            edtState.Text = CheckBoxState.ToString();
        }
        private void rdoUndef_CheckedChanged(object sender, EventArgs e)
        {
            EnableWindow(hApplyBtn, true);
            CheckBoxState = mHpCheckBoxState.UNDEFINED; // CHANGESTATE
            edtState.Text = CheckBoxState.ToString();
        }

        /// <summary>
        /// CheckBoxState - mHelp CheckBox state
        /// </summary>
        private int CheckBoxState;
        public int State {
            get { return CheckBoxState; }
        }

        /// <summary>
        /// Init - Initialize mHelp .NET CheckBox control state Form 
        /// </summary>
        /// <param name="State">mHelp CheckBox state</param>
        /// <param name="ThreeState">CheckBox style</param>
        public void Init(int State, bool ThreeState)
        {
            // Change indeterminate RadioBox name if a 3 state CheckBox
            if (ThreeState)
                rdoUndef.Text = Properties.Resources.RS_STATE_INDETERMINATE;

            // Set CheckBox state
            CheckBoxState = State;
            switch (State)
            {
                case mHpCheckBoxState.UNCHECKED:
                {
                    rdoUncheck.Checked = true;
                    break;
                }
                case mHpCheckBoxState.CHECKED:
                {
                    rdoCheck.Checked = true;
                    break;
                }
                case mHpCheckBoxState.UNDEFINED: // CHANGESTATE
                {
                    rdoUndef.Checked = true;
                    break;
                }
            }
            edtState.Text = CheckBoxState.ToString();
            EnableWindow(hApplyBtn, false);
        }

        ///////////////////////////////////////////////////

        public mHpCheckBoxForm(Int32 hSaveBtn)
        {
            InitializeComponent();

            // Let mHelp make it visible
            this.Visible = false;

            //
            hApplyBtn = (IntPtr)hSaveBtn;
            rdoCheck.Checked = true; // Default
        }

        private void InitializeComponent()
        {
            this.edtState = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.rdoUndef = new System.Windows.Forms.RadioButton();
            this.rdoUncheck = new System.Windows.Forms.RadioButton();
            this.rdoCheck = new System.Windows.Forms.RadioButton();
            this.SuspendLayout();
            // 
            // edtState
            // 
            this.edtState.Location = new System.Drawing.Point(218, 6);
            this.edtState.Name = "edtState";
            this.edtState.ReadOnly = true;
            this.edtState.Size = new System.Drawing.Size(37, 20);
            this.edtState.TabIndex = 9;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(132, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Valeur de l\'état:";
            // 
            // rdoUndef
            // 
            this.rdoUndef.AutoSize = true;
            this.rdoUndef.Location = new System.Drawing.Point(10, 53);
            this.rdoUndef.Name = "rdoUndef";
            this.rdoUndef.Size = new System.Drawing.Size(91, 17);
            this.rdoUndef.TabIndex = 7;
            this.rdoUndef.Text = "Change d\'état";
            this.rdoUndef.UseVisualStyleBackColor = true;
            // 
            // rdoUncheck
            // 
            this.rdoUncheck.AutoSize = true;
            this.rdoUncheck.Location = new System.Drawing.Point(10, 30);
            this.rdoUncheck.Name = "rdoUncheck";
            this.rdoUncheck.Size = new System.Drawing.Size(73, 17);
            this.rdoUncheck.TabIndex = 6;
            this.rdoUncheck.Text = "Désactivé";
            this.rdoUncheck.UseVisualStyleBackColor = true;
            // 
            // rdoCheck
            // 
            this.rdoCheck.AutoSize = true;
            this.rdoCheck.Checked = true;
            this.rdoCheck.Location = new System.Drawing.Point(10, 7);
            this.rdoCheck.Name = "rdoCheck";
            this.rdoCheck.Size = new System.Drawing.Size(55, 17);
            this.rdoCheck.TabIndex = 5;
            this.rdoCheck.TabStop = true;
            this.rdoCheck.Text = "Activé";
            this.rdoCheck.UseVisualStyleBackColor = true;
            // 
            // mHpCheckBoxForm
            // 
            this.ClientSize = new System.Drawing.Size(265, 123);
            this.Controls.Add(this.edtState);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.rdoUndef);
            this.Controls.Add(this.rdoUncheck);
            this.Controls.Add(this.rdoCheck);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "mHpCheckBoxForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "mHpCheckBoxForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }
    }
}

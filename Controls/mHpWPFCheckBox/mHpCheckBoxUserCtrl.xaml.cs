/// <summary>
/// mHpCheckBoxUserCtrl.cs - User Control definition for mHelp WPF CheckBox control state
/// Date - 02/15/2013
/// 
/// Copyright(c) - Unknown Corporation. All rights reserved.
/// </summary>

using System;
using System.Windows.Controls;
using System.Runtime.InteropServices;

namespace mHpWPFCheckBox
{
    /// <summary>
    /// Class mHpCheckBoxUserCtrl - mHelp WPF CheckBox user control class
    /// </summary>
    public partial class mHpCheckBoxUserCtrl : UserControl
    {
        [DllImport("User32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool EnableWindow(IntPtr hWnd, bool bEnable);

        /// <summary>
        /// hApplyBtn - Handle of the button that save state change
        /// </summary>
        private IntPtr hApplyBtn;

        ////// Enable button to apply change on CheckBox state (when state config has changed)
        private void rdoCheck_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            EnableWindow(hApplyBtn, true);
            CheckBoxState = mHpCheckBoxState.CHECKED;
            edtState.Text = CheckBoxState.ToString();
        }
        private void rdoUncheck_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            EnableWindow(hApplyBtn, true);
            CheckBoxState = mHpCheckBoxState.UNCHECKED;
            edtState.Text = CheckBoxState.ToString();
        }
        private void rdoUndef_Checked(object sender, System.Windows.RoutedEventArgs e)
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
        /// Init - Initialize mHelp WPF CheckBox user control
        /// </summary>
        /// <param name="State">mHelp CheckBox state</param>
        /// <param name="ThreeState">CheckBox style</param>
        public void Init(int State, bool ThreeState)
        {
            // Change indeterminate RadioBox name if a 3 state CheckBox
            if (ThreeState)
                rdoUndef.Content = Properties.Resources.RS_STATE_INDETERMINATE;

            // Set CheckBox state
            CheckBoxState = State;
            switch (State)
            {
                case mHpCheckBoxState.UNCHECKED:
                {
                    rdoUncheck.IsChecked = true;
                    break;
                }
                case mHpCheckBoxState.CHECKED:
                {
                    rdoCheck.IsChecked = true;
                    break;
                }
                case mHpCheckBoxState.UNDEFINED: // CHANGESTATE
                {
                    rdoUndef.IsChecked = true;
                    break;
                }
            }
            edtState.Text = CheckBoxState.ToString();
            EnableWindow(hApplyBtn, false);
        }

        ///////////////////////////////////////////////////

        public mHpCheckBoxUserCtrl(Int32 hSaveBtn)
        {
            InitializeComponent();

            //
            hApplyBtn = (IntPtr)hSaveBtn;
            rdoCheck.IsChecked = true; // Default
        }
    }
}

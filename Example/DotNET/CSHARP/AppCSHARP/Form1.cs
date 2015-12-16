using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using mHelp;

namespace AppCSHARP
{
    public partial class Form1 : Form
    {
        int iClickCount;

        public Form1()
        {
            InitializeComponent();
            iClickCount = 0;
        }

        private void Form1_Activated(object sender, EventArgs e)
        {
            CmHelp.MainActive(this.Handle, "AppCSHARP", true);
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btnmHelp_Click(object sender, EventArgs e)
        {
            CmHelp.Create(this.Handle);
        }

        private void btnTest_Click(object sender, EventArgs e)
        {
            iClickCount++;
            label.Text = "Test..." + iClickCount.ToString();

            MessageBox.Show("Test", "Message");
        }

        private void chckTest_CheckedChanged(object sender, EventArgs e)
        {
            iClickCount++;
            label.Text = "CheckedChanged..." + iClickCount.ToString();
        }

        private void chckTest_CheckStateChanged(object sender, EventArgs e)
        {
            iClickCount++;
            label.Text = "CheckStateChanged..." + iClickCount.ToString();
        }

        private void chck3Test_CheckStateChanged(object sender, EventArgs e)
        {
            iClickCount++;
            label.Text = "CheckStateChanged..." + iClickCount.ToString();
        }

        private void chck3Test_CheckedChanged(object sender, EventArgs e)
        {
            iClickCount++;
            label.Text = "CheckedChanged..." + iClickCount.ToString();
        }
    }
}

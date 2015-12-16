namespace AppCSHARP
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.chck3Test = new System.Windows.Forms.CheckBox();
            this.chckTest = new System.Windows.Forms.CheckBox();
            this.btnmHelp = new System.Windows.Forms.Button();
            this.label = new System.Windows.Forms.Label();
            this.btnTest = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // chck3Test
            // 
            this.chck3Test.AutoSize = true;
            this.chck3Test.Location = new System.Drawing.Point(12, 76);
            this.chck3Test.Name = "chck3Test";
            this.chck3Test.Size = new System.Drawing.Size(83, 17);
            this.chck3Test.TabIndex = 11;
            this.chck3Test.Text = "checkBox 3";
            this.chck3Test.ThreeState = true;
            this.chck3Test.UseVisualStyleBackColor = true;
            this.chck3Test.CheckedChanged += new System.EventHandler(this.chck3Test_CheckedChanged);
            this.chck3Test.CheckStateChanged += new System.EventHandler(this.chck3Test_CheckStateChanged);
            // 
            // chckTest
            // 
            this.chckTest.AutoSize = true;
            this.chckTest.Location = new System.Drawing.Point(12, 53);
            this.chckTest.Name = "chckTest";
            this.chckTest.Size = new System.Drawing.Size(74, 17);
            this.chckTest.TabIndex = 10;
            this.chckTest.Text = "checkBox";
            this.chckTest.UseVisualStyleBackColor = true;
            this.chckTest.CheckedChanged += new System.EventHandler(this.chckTest_CheckedChanged);
            this.chckTest.CheckStateChanged += new System.EventHandler(this.chckTest_CheckStateChanged);
            // 
            // btnmHelp
            // 
            this.btnmHelp.Location = new System.Drawing.Point(116, 227);
            this.btnmHelp.Name = "btnmHelp";
            this.btnmHelp.Size = new System.Drawing.Size(75, 23);
            this.btnmHelp.TabIndex = 9;
            this.btnmHelp.Text = "mHelp ?!";
            this.btnmHelp.UseVisualStyleBackColor = true;
            this.btnmHelp.Click += new System.EventHandler(this.btnmHelp_Click);
            // 
            // label
            // 
            this.label.AutoSize = true;
            this.label.Location = new System.Drawing.Point(103, 17);
            this.label.Name = "label";
            this.label.Size = new System.Drawing.Size(35, 13);
            this.label.TabIndex = 8;
            this.label.Text = "label1";
            // 
            // btnTest
            // 
            this.btnTest.Location = new System.Drawing.Point(12, 12);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(75, 23);
            this.btnTest.TabIndex = 7;
            this.btnTest.Text = "Test";
            this.btnTest.UseVisualStyleBackColor = true;
            this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(197, 227);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 6;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.chck3Test);
            this.Controls.Add(this.chckTest);
            this.Controls.Add(this.btnmHelp);
            this.Controls.Add(this.label);
            this.Controls.Add(this.btnTest);
            this.Controls.Add(this.btnClose);
            this.Name = "Form1";
            this.Text = "AppCSHARP";
            this.Activated += new System.EventHandler(this.Form1_Activated);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox chck3Test;
        private System.Windows.Forms.CheckBox chckTest;
        private System.Windows.Forms.Button btnmHelp;
        private System.Windows.Forms.Label label;
        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.Button btnClose;
    }
}


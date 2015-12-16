using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Windows.Interop;
using mHelp;

namespace AppWPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        int iCount;

        public MainWindow()
        {
            InitializeComponent();

            iCount = 0;
            btnTest.Click += new RoutedEventHandler(btnTest_Click1);
        }

        private void Window_Activated(object sender, EventArgs e)
        {
            IntPtr hwndMain = new WindowInteropHelper(this).Handle;
            CmHelp.MainActive(hwndMain, "AppWPF", true);
        }

        private void btnmHelp_Click(object sender, RoutedEventArgs e)
        {
            IntPtr hwndMain = new WindowInteropHelper(this).Handle;
            CmHelp.Create(hwndMain);
        }
       
        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void btnTest_Click1(object sender, RoutedEventArgs e)
        {
            iCount++;
            lblTest.Content = "...Test1 " + iCount.ToString();
        }

        private void btnTest_Click(object sender, RoutedEventArgs e)
        {
            iCount++;
            lblTest.Content = "...Test " + iCount.ToString();
        }

        private void checkBox1_Checked(object sender, RoutedEventArgs e)
        {
            iCount++;
            label1.Content = "...Checked " + iCount.ToString();
        }

        private void checkBox1_Unchecked(object sender, RoutedEventArgs e)
        {
            iCount++;
            label1.Content = "...Unchecked " + iCount.ToString();
        }

        private void checkBox1_Click(object sender, RoutedEventArgs e)
        {
            iCount++;
            label1.Content = "...Click " + iCount.ToString();
        }

        private void checkBox3_Checked(object sender, RoutedEventArgs e)
        {
            iCount++;
            label1.Content = "...Checked-3 " + iCount.ToString();
        }

        private void checkBox3_Unchecked(object sender, RoutedEventArgs e)
        {
            iCount++;
            label1.Content = "...Unchecked-3 " + iCount.ToString();
        }

        private void checkBox3_Indeterminate(object sender, RoutedEventArgs e)
        {
            iCount++;
            label1.Content = "...Indeterminate-3 " + iCount.ToString();
        }

        private void checkBox3_Click(object sender, RoutedEventArgs e)
        {
            iCount++;
            label1.Content = "...Click-3 " + iCount.ToString();
        }
    }
}

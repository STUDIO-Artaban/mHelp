/// <summary>
/// mHpWPFCheckBox.cs - Interface for mHelp WPF CheckBox Control Management
/// mHelp version - 1.3
/// Framework .NET version - 3.5
/// Platform - WPF
/// Date - 02/14/2013
/// 
/// Copyright(c) - Unknown Corporation. All rights reserved.
/// </summary>

using mHpWPFCtrl;

using System;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Reflection;

namespace mHpWPFCheckBox
{
    /// <summary>
    /// mHpCheckBoxState - mHelp CheckBox state definitions
    /// </summary>
    static class mHpCheckBoxState
    {
        public const int UNCHECKED = 0;             // Unchecked
        public const int CHECKED = 1;               // Checked
        public const int UNDEFINED = 2;             // Indeterminate
        public const int CHANGESTATE = UNDEFINED;   // Change state
    }

    /// <summary>
    /// Class CmHpWPFCheckBox - CmHpWPFCtrl abstract class implementation for CheckBox control
    /// </summary>
    public class CmHpWPFCheckBox : CmHpWPFCtrl<CheckBox>
    {
        private const string CHECKBOX_STATE_NAME = "CheckState";

        /// <summary>
        /// CheckBox style definition(s)
        /// </summary>
        private const int STYLE_CHECKBOX_COMMON = 0;
        private const int STYLE_CHECKBOX_3STATE = 1;

        /// <summary>
        /// CheckBox event definition(s)
        /// </summary>
        private const string EVENT_CHECKBOX_CHECKED = "mHpWPFCheckBox_Checked";
        private const string EVENT_CHECKBOX_UNCHECKED = "mHpWPFCheckBox_Unchecked";
        private const string EVENT_CHECKBOX_INDETERMINATE = "mHpWPFCheckBox_Indeterminate";

        // ************************************************
        // Properties

        // ------------------------------------------------
        // Object
        //

        //// IsCtrlObject
        public override bool IsCtrlObject(string ObjectName)
        {
            // Compare CheckBox object type name
            CheckBox CtrlObj = new CheckBox();
            if (ObjectName == CtrlObj.GetType().Name)
                return true;

            return false;
        }

        //// GetCtrlObject
        public override string GetCtrlObject(uint iObjIdx)
        {
            // Check right CheckBox object type index (== 0)
            if (iObjIdx < 1)
            {
                CheckBox CtrlObj = new CheckBox();
                return CtrlObj.GetType().Name;
            }

            return null;
        }

        // ------------------------------------------------
        // Name
        //

        //// GetCtrlName
        public override string GetCtrlName(int iNameIdx = mHpCtrlConst.NO_VALUE)
        {
            // Return the name of the control...
            if (iNameIdx <= mHpCtrlConst.NO_VALUE) // ...according control style
                return Properties.Resources.RS_CHECKBOX_NAME;

            // ...according index parameter
            if (iNameIdx < 1) // One control name is available for CheckBox
                return Properties.Resources.RS_CHECKBOX_NAME; // CheckBox

            return null;
        }

        // ------------------------------------------------
        // Info
        //

        //// GetCtrlVers
        public override string GetCtrlVers()
        {
            // Return WPF CheckBox version (.NET framework version)
            return Properties.Resources.RS_CHECKBOX_VERS;
        }

        //// GetCtrlDesc
        public override string GetCtrlDesc()
        {
            // Return WPF CheckBox version (.NET framework version)
            return Properties.Resources.RS_CHECKBOX_DESC;
        }

        ////// GetIconList
        public override IntPtr GetIconList(uint iIconIdx)
        {
            // Return icon according specific index
            if (iIconIdx < 1)
                return Properties.Resources.mHpCheckBox.Handle;   // CheckBox icon

            // No more icon
            return IntPtr.Zero;
        }

        ////// GetIconIdx
        public override int GetIconIdx()
        {
            // Return icon index according CheckBox style (see 'GetIconList' method)
            if ((CtrlStyle == STYLE_CHECKBOX_COMMON) || (CtrlStyle == STYLE_CHECKBOX_3STATE))
                return 0;   // CheckBox icon index

            // Wrong CheckBox style
            return mHpCtrlConst.NO_VALUE;
        }

        // ------------------------------------------------
        // User Interface (UI)
        //

        /// <summary>
        /// CheckBoxCfgCtrl - mHelp WPF CheckBox state user control
        /// CheckBoxCfgForm - Window containning the mHelp WPF CheckBox state user control
        /// </summary>
        private mHpCheckBoxUserCtrl CheckBoxCfgCtrl = null;
        private System.Windows.Window CheckBoxCfgForm = null;

        ////// CreateUI
        public override IntPtr CreateUI(Int32 hSaveBtn)
        {
            // Create new mHelp .NET CheckBox control state Form instance
            CheckBoxCfgCtrl = new mHpCheckBoxUserCtrl(hSaveBtn);
            CheckBoxCfgForm = new System.Windows.Window();

            WindowInteropHelper win32Owner = new WindowInteropHelper(CheckBoxCfgForm);
            win32Owner.Owner = (IntPtr)hSaveBtn; // Same thing if it was the config dialog handle

            CheckBoxCfgForm.Width = 278;
            CheckBoxCfgForm.Height = 133;
            CheckBoxCfgForm.WindowStyle = System.Windows.WindowStyle.None;
            CheckBoxCfgForm.WindowStartupLocation = System.Windows.WindowStartupLocation.CenterScreen;
            CheckBoxCfgForm.ShowInTaskbar = false;
            CheckBoxCfgForm.AllowsTransparency = true;

            Canvas CfgCanvas = new Canvas();
            CfgCanvas.Width = 278;
            CfgCanvas.Height = 133;

            Grid CfgGrid = new Grid();
            CfgGrid.Children.Add(CheckBoxCfgCtrl);

            CfgCanvas.Children.Add(CfgGrid);
            CheckBoxCfgForm.Content = CfgCanvas;

            // Initialize CheckBox state user control
            CheckBoxCfgCtrl.Init((int)CtrlState[0].Value[0],
                                 (CtrlStyle == STYLE_CHECKBOX_3STATE) ? true : false);

            CheckBoxCfgForm.Show();
            return win32Owner.Handle;
        }

        ////// SaveUI
        public override void SaveUI()
        {
            // Assign new configured CheckBox state
            CtrlState[0].Value[0] = CheckBoxCfgCtrl.State;
        }

        ////// CloseUI
        public override void CloseUI()
        {
            // Close mHelp .NET CheckBox control state Form
            if (CheckBoxCfgForm != null)
                CheckBoxCfgForm.Close();

            CheckBoxCfgCtrl = null;
            CheckBoxCfgForm = null;
        }

        // ------------------------------------------------
        // Style
        //

        ////// SetStyle
        public override bool SetStyle(int iStyle)
        {
            // Check valid style
            if ((iStyle != STYLE_CHECKBOX_COMMON) && (iStyle != STYLE_CHECKBOX_3STATE))
                return false; // Wrong CheckBox style

            // Assign control style
            CtrlStyle = iStyle;

            // Define state & config class instance according control style
            if (CtrlState == null)
            {
                // Define CheckBox state
                CtrlState = new mHpCtrlData[1];
                CtrlState[0] = new mHpCtrlData(CHECKBOX_STATE_NAME, mHpCtrlData.ValueType.Integer);
                CtrlState[0].Value = new Object[1]; // Always == 1 (mHpCtrlData.ValueType.Integer)
            }

            return true;
        }

        // ------------------------------------------------
        // State
        //

        ////// GetStateCount
        public override uint GetStateCount()
        {
            // Return CheckBox state count (== 1 for CheckBox state)
            return 1;
        }
        public override uint GetStateCount(uint iStateIdx)
        {
            // Check valid CheckBox state index (== 0 index for CheckBox state)
            if (iStateIdx != 0)
                return 0;

            // Check valid control state
            if (CtrlState[iStateIdx].Value == null)
                return 0;

            // CheckBox config integer value type count
            return (uint)CtrlState[iStateIdx].Value.Length; // Always == 1 (mHpCtrlData.ValueType.Integer)
        }
        
        ////// GetStateName
        public override string GetStateName(uint iStateIdx)
        {
            // Check valid CheckBox state index (== 0)
            if (iStateIdx != 0)
                return null; // Wrong CheckBox state index

            // Return CheckBox state name
            return CtrlState[iStateIdx].Name; // 'CheckState'
        }

        ////// GetStateType
        public override int GetStateType(uint iStateIdx)
        {
            // Check valid CheckBox state index (== 0)
            if (iStateIdx != 0)
                return mHpCtrlConst.NO_VALUE; // Wrong CheckBox state index

            // Return CheckBox state value type (Integer for CheckBox state)
            return (int)CtrlState[iStateIdx].Type;
        }

        ////// GetIntState
        public override int GetIntState(uint iStateIdx, uint iArrayIdx = 0)
        {
            // Check valid CheckBox state index without no array (== 0)
            if ((iStateIdx != 0) || (iArrayIdx != 0))
                return mHpCtrlConst.NO_VALUE; // Wrong CheckBox state index

            // Return CheckBox state value
            return (int)CtrlState[iStateIdx].Value[iArrayIdx]; // CheckBox state
        }

        ////// GetStrState
        public override string GetStrState(uint iStateIdx, uint iArrayIdx = 0)
        {
            // No String CheckBox state value type
            return null;
        }

        ////// NeedToCheckState
        public override bool NeedToCheckState()
        {
            return false;   // No message could be displayd if process failed to set state
                            // -> State of CheckBox is fixed and can always be set successfully
        }

        ////// DisplayState
        public override string DisplayState()
        {
            // Display CheckBox state
            switch ((int)CtrlState[0].Value[0])
            {
                case mHpCheckBoxState.UNCHECKED: return Properties.Resources.RS_STATE_UNCHECKED; // Unchecked
                case mHpCheckBoxState.CHECKED: return Properties.Resources.RS_STATE_CHECKED; // Checked
                case mHpCheckBoxState.UNDEFINED: // CHANGESTATE
                {
                    // Display state according CheckBox style
                    if (CtrlStyle == STYLE_CHECKBOX_COMMON)
                        return Properties.Resources.RS_STATE_CHANGECHECK; // Change state
                    else
                        return Properties.Resources.RS_STATE_INDETERMINATE; // Indeterminate
                }
            }

            // Unexpected CheckBox state
            return null;
        }

        // ------------------------------------------------
        // Config (No config for CheckBox control)
        //
        public override uint GetCfgCount() { return 0; }
        public override uint GetCfgCount(uint iCfgIdx) { return 0; }
        public override int GetCfgType(uint iCfgIdx) { return mHpCtrlConst.NO_VALUE; }
        public override string GetCfgName(uint iCfgIdx) { return null; }
        public override int GetIntCfg(uint iCfgIdx, uint iArrayIdx = 0) { return mHpCtrlConst.NO_VALUE; }
        public override string GetStrCfg(uint iCfgIdx, uint iArrayIdx = 0) { return null; }

        // ------------------------------------------------
        // HTML tags
        //

        //// GetHtmlTags
        public override string GetHtmlTags(uint iTaskIdx)
        {
            // Use HTML radio input type to set CheckBox state
            HtmlTags = "\t<input type=\"radio\" name=\"" + iTaskIdx.ToString();
            HtmlTags += "-" + CHECKBOX_STATE_NAME + "\" value=\"1\"";
            if ((int)CtrlState[0].Value[0] == mHpCheckBoxState.CHECKED)
                HtmlTags += " checked"; // Select current state
            HtmlTags += "/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">";
            HtmlTags += Properties.Resources.RS_STATE_CHECKED + "</font><br>\r\n";
            HtmlTags += "\t<input type=\"radio\" name=\"" + iTaskIdx.ToString();
            HtmlTags += "-" + CHECKBOX_STATE_NAME + "\" value=\"0\"";
            if ((int)CtrlState[0].Value[0] == mHpCheckBoxState.UNCHECKED)
                HtmlTags += " checked"; // Select current state
            HtmlTags += "/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">";
            HtmlTags += Properties.Resources.RS_STATE_UNCHECKED + "</font><br>\r\n";
            HtmlTags += "\t<input type=\"radio\" name=\"" + iTaskIdx.ToString();
            HtmlTags += "-" + CHECKBOX_STATE_NAME + "\" value=\"2\"";
            if ((int)CtrlState[0].Value[0] == mHpCheckBoxState.UNDEFINED)
                HtmlTags += " checked"; // Select current state
            HtmlTags += "/><font style=\"font-family: Verdana,Lucida; font-size: 10pt\">";

            // Check CheckBox style to display correct label
            if (CtrlStyle == STYLE_CHECKBOX_3STATE)
                HtmlTags += Properties.Resources.RS_STATE_INDETERMINATE; // For 3 state style
            else
                HtmlTags += Properties.Resources.RS_STATE_CHANGECHECK; // For common style
            HtmlTags += "</font><br>\r\n";

            // Will return following for a 3 state CheckBox style with an unchecked current state:

            //  <input type="radio" name="0-CheckState" value="1"/>Activé<br>
            //  <input type="radio" name="0-CheckState" value="0" checked/>Désactivé<br>
            //  <input type="radio" name="0-CheckState" value="2"/>Indéterminé<br>

            return HtmlTags;
        }

        // ------------------------------------------------
        // Capture processus
        //

        private void OnmHpCheckBoxEvent(object senderCtrl, string RegCtrlEvent)
        {
            // Get CheckBox that send this event
            CheckBox CurChk = senderCtrl as CheckBox;

            ////// WARNING: Always check triggered user event
            // -> Using code below
            if (!CurChk.Focus())
                return;

            // Set CheckBox control identifier
            // -> Using common control identifier (CheckBox.Name)
            SetControl(CurChk as Control);

            // Set current control style
            if (CurChk.IsThreeState)
                SetStyle(STYLE_CHECKBOX_3STATE);
            else
                SetStyle(STYLE_CHECKBOX_COMMON);

            // Set CheckBox control config (call 'SetStyle' first)
            // -> No config available for CheckBox control

            // Set CheckBox state (call 'SetStyle' first)
            if (CurChk.IsChecked == true)
                CtrlState[0].Value[0] = mHpCheckBoxState.CHECKED; // Checked
            else if (CurChk.IsChecked == false)
                CtrlState[0].Value[0] = mHpCheckBoxState.UNCHECKED; // Unchecked
            else
                CtrlState[0].Value[0] = mHpCheckBoxState.UNDEFINED; // Indeterminate

            // Set current control object name
            CtrlObjName = CurChk.GetType().Name; // One CheckBox object name available (CheckBox)

            // Set current state bound
            StateBound = "[0;2]";   // 0: Uncheck
            // 1: Check
            // 2: Indeterminate/Change

            ////// WARNING: Always call function below for all control event occurence
            // -> Send window message in order to inform mHelp a CheckBox 'Checked' event has occurred
            SendmHpEvent(senderCtrl, RegCtrlEvent);
        }

        private const string METHOD_CHECKBOX_CHECKED = "OnmHpCheckBoxChecked";
        private void OnmHpCheckBoxChecked(object sender, System.Windows.RoutedEventArgs e)
        {
            OnmHpCheckBoxEvent(sender, EVENT_CHECKBOX_CHECKED);
        }

        private const string METHOD_CHECKBOX_UNCHECKED = "OnmHpCheckBoxUnchecked";
        private void OnmHpCheckBoxUnchecked(object sender, System.Windows.RoutedEventArgs e)
        {
            OnmHpCheckBoxEvent(sender, EVENT_CHECKBOX_UNCHECKED);
        }

        private const string METHOD_CHECKBOX_INDETERMINATE = "OnmHpCheckBoxIndeterminate";
        private void OnmHpCheckBoxIndeterminate(object sender, System.Windows.RoutedEventArgs e)
        {
            OnmHpCheckBoxEvent(sender, EVENT_CHECKBOX_INDETERMINATE);
        }

        ////// AddmHpEvent
        protected override bool AddmHpEvent(Visual Ctrl, string RegCtrlEvent)
        {
            CheckBox ChkCtrl = Ctrl as CheckBox;
            System.Windows.UIElement CtrlElement = ChkCtrl as System.Windows.UIElement;

            // Get the EventHandlersStore instance which holds event handlers for the specified element.
            BindingFlags allBindingFlags = BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance;
            System.Reflection.PropertyInfo evtHdlrStoreProp = typeof(System.Windows.UIElement).GetProperty("EventHandlersStore", allBindingFlags);
            if (evtHdlrStoreProp == null)
                return false;
            Object evtHdlrStoreObj = evtHdlrStoreProp.GetValue(CtrlElement, null);
            if (evtHdlrStoreObj == null)
                return false;

            // Invoke the GetRoutedEventHandlers method on the EventHandlersStore instance
            System.Reflection.MethodInfo rtdEvtHdlrMth = evtHdlrStoreObj.GetType().GetMethod("GetRoutedEventHandlers", allBindingFlags);
            if (rtdEvtHdlrMth == null)
                return false;

            // Swith on event to add
            switch (RegCtrlEvent)
            {
                case EVENT_CHECKBOX_CHECKED: ////// 'Checked' event
                {
                    System.Windows.RoutedEventHandlerInfo[] rtdEvtHdlrInfoTab = (System.Windows.RoutedEventHandlerInfo[])rtdEvtHdlrMth.Invoke(evtHdlrStoreObj, new object[] { System.Windows.Controls.Primitives.ToggleButton.CheckedEvent });
                    if (rtdEvtHdlrInfoTab == null)
                        return false;

                    // Check existing CheckBox 'Checked' routed event handler
                    if (rtdEvtHdlrInfoTab.Length > 0)
                    {
                        ////// WARNING: This method can be called several times for the same control
                        // -> Check if mHelp routed event handler has already been added
                        bool bEvtmHpAdded = false;
                        foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                        {
                            if (evtHdlrInfo.Handler.Method.Name == METHOD_CHECKBOX_CHECKED)
                            {
                                // mHelp routed event handler has already been added
                                bEvtmHpAdded = true;
                                break;
                            }
                        }

                        // Add if needed mHelp routed event handler at the top of the event list...
                        if (!bEvtmHpAdded)
                            ChkCtrl.Checked += new System.Windows.RoutedEventHandler(OnmHpCheckBoxChecked);

                        // Remove all previous events
                        foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                        {
                            if (evtHdlrInfo.Handler.Method.Name != METHOD_CHECKBOX_CHECKED)
                                ChkCtrl.Checked -= (System.Windows.RoutedEventHandler)evtHdlrInfo.Handler;

                        }  // ...after having removed all previous event(s)

                        // Restore all previous event(s)
                        foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                        {
                            if (evtHdlrInfo.Handler.Method.Name != METHOD_CHECKBOX_CHECKED)
                                ChkCtrl.Checked += (System.Windows.RoutedEventHandler)evtHdlrInfo.Handler;
                        }
                    }
                    else
                        ChkCtrl.Checked += new System.Windows.RoutedEventHandler(OnmHpCheckBoxChecked);

                    break;
                }
                case EVENT_CHECKBOX_UNCHECKED: ////// 'Unchecked' event
                {
                    System.Windows.RoutedEventHandlerInfo[] rtdEvtHdlrInfoTab = (System.Windows.RoutedEventHandlerInfo[])rtdEvtHdlrMth.Invoke(evtHdlrStoreObj, new object[] { System.Windows.Controls.Primitives.ToggleButton.UncheckedEvent });
                    if (rtdEvtHdlrInfoTab == null)
                        return false;

                    // Check existing CheckBox 'Unchecked' routed event handler
                    if (rtdEvtHdlrInfoTab.Length > 0)
                    {
                        ////// WARNING: This method can be called several times for the same control
                        // -> Check if mHelp routed event handler has already been added
                        bool bEvtmHpAdded = false;
                        foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                        {
                            if (evtHdlrInfo.Handler.Method.Name == METHOD_CHECKBOX_UNCHECKED)
                            {
                                // mHelp routed event handler has already been added
                                bEvtmHpAdded = true;
                                break;
                            }
                        }

                        // Add if needed mHelp routed event handler at the top of the event list...
                        if (!bEvtmHpAdded)
                            ChkCtrl.Unchecked += new System.Windows.RoutedEventHandler(OnmHpCheckBoxUnchecked);

                        // Remove all previous events
                        foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                        {
                            if (evtHdlrInfo.Handler.Method.Name != METHOD_CHECKBOX_UNCHECKED)
                                ChkCtrl.Unchecked -= (System.Windows.RoutedEventHandler)evtHdlrInfo.Handler;

                        }  // ...after having removed all previous event(s)

                        // Restore all previous event(s)
                        foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                        {
                            if (evtHdlrInfo.Handler.Method.Name != METHOD_CHECKBOX_UNCHECKED)
                                ChkCtrl.Unchecked += (System.Windows.RoutedEventHandler)evtHdlrInfo.Handler;
                        }
                    }
                    else
                        ChkCtrl.Unchecked += new System.Windows.RoutedEventHandler(OnmHpCheckBoxUnchecked);

                    break;
                }
                case EVENT_CHECKBOX_INDETERMINATE: ////// 'Indeterminate' event
                {
                    // Check 3 state CheckBox style
                    if (ChkCtrl.IsThreeState)
                    {
                        System.Windows.RoutedEventHandlerInfo[] rtdEvtHdlrInfoTab = (System.Windows.RoutedEventHandlerInfo[])rtdEvtHdlrMth.Invoke(evtHdlrStoreObj, new object[] { System.Windows.Controls.Primitives.ToggleButton.IndeterminateEvent });
                        if (rtdEvtHdlrInfoTab == null)
                            return false;

                        // Check existing CheckBox 'Indeterminate' routed event handler
                        if (rtdEvtHdlrInfoTab.Length > 0)
                        {
                            ////// WARNING: This method can be called several times for the same control
                            // -> Check if mHelp routed event handler has already been added
                            bool bEvtmHpAdded = false;
                            foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                            {
                                if (evtHdlrInfo.Handler.Method.Name == METHOD_CHECKBOX_INDETERMINATE)
                                {
                                    // mHelp routed event handler has already been added
                                    bEvtmHpAdded = true;
                                    break;
                                }
                            }

                            // Add if needed mHelp routed event handler at the top of the event list...
                            if (!bEvtmHpAdded)
                                ChkCtrl.Indeterminate += new System.Windows.RoutedEventHandler(OnmHpCheckBoxIndeterminate);

                            // Remove all previous events
                            foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                            {
                                if (evtHdlrInfo.Handler.Method.Name != METHOD_CHECKBOX_INDETERMINATE)
                                    ChkCtrl.Indeterminate -= (System.Windows.RoutedEventHandler)evtHdlrInfo.Handler;

                            }  // ...after having removed all previous event(s)

                            // Restore all previous event(s)
                            foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                            {
                                if (evtHdlrInfo.Handler.Method.Name != METHOD_CHECKBOX_INDETERMINATE)
                                    ChkCtrl.Indeterminate += (System.Windows.RoutedEventHandler)evtHdlrInfo.Handler;
                            }
                        }
                        else
                            ChkCtrl.Indeterminate += new System.Windows.RoutedEventHandler(OnmHpCheckBoxIndeterminate);
                    }
                    break;
                }
            }

            return true;
        }

        // --------------------------------
        // Reproduce processus
        //

        ////// GetCtrlLeft
        public override int GetCtrlLeft()
        {
            // Get visual CheckBox
            Visual VisualChk = CtrlObj as Visual;

            // Get CheckBox coordinate in the screen
            System.Windows.Point ChkPos = VisualChk.PointToScreen(new System.Windows.Point(0, 0));

            return (int)ChkPos.X;
        }

        ////// GetCtrlTop
        public override int GetCtrlTop()
        {
            // Get visual CheckBox
            Visual VisualChk = CtrlObj as Visual;

            // Get CheckBox coordinate in the screen
            System.Windows.Point ChkPos = VisualChk.PointToScreen(new System.Windows.Point(0, 0));

            return (int)ChkPos.Y;
        }

        ////// GetCtrlWidth
        public override int GetCtrlWidth()
        {
            // Get CheckBox control
            CheckBox ChkCtrl = CtrlObj as CheckBox;

            return (int)ChkCtrl.ActualWidth;
        }

        ////// GetCtrlHeight
        public override int GetCtrlHeight()
        {
            // Get CheckBox control
            CheckBox ChkCtrl = CtrlObj as CheckBox;

            return (int)ChkCtrl.ActualHeight;
        }

        ////// CheckState
        public override bool CheckState()
        {
            // Can always set CheckBox state
            return true;
        }

        ////// NextAction
        public override int NextAction()
        {
            // Check existing valid state
            if ((CtrlState != null) &&
                (CtrlState[0] != null) &&
                (CtrlState[0].Value != null) &&
                (CtrlState[0].Value[0] != null) &&
                (CtrlState[0].Value[0].GetType() == typeof(int)))
            {
                // Next action step
                ActStep++;

                // Switch on event to reproduce
                if ((GetCtrlEvent() == EVENT_CHECKBOX_CHECKED) ||
                    (GetCtrlEvent() == EVENT_CHECKBOX_UNCHECKED) ||
                    (GetCtrlEvent() == EVENT_CHECKBOX_INDETERMINATE))
                {
                    // Switch on current action step
                    switch (ActStep)
                    {
                        case 1: { return (int)mHpCtrlConst.ActType.ActMove; } // Move mHelp cursor over the control
                        case 2:
                        {
                            // Get CheckBox control
                            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

                            // Check CheckBox state to set
                            switch((int)CtrlState[0].Value[0])
                            {
                                case mHpCheckBoxState.UNCHECKED: ////// Uncheck
                                {
                                    // Check if already unchecked
                                    if (ChckBoxCtrl.IsChecked == false)
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                                case mHpCheckBoxState.CHECKED: ////// Check
                                {
                                    // Check if already checked
                                    if (ChckBoxCtrl.IsChecked == true)
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                                case mHpCheckBoxState.UNDEFINED: ////// Indeterminate/Change (CHANGESTATE)
                                {
                                    // Check 3 state CheckBox style
                                    if (CtrlStyle == STYLE_CHECKBOX_3STATE)
                                    {
                                        // Check if already indeterminate
                                        if ((ChckBoxCtrl.IsChecked != true) && (ChckBoxCtrl.IsChecked != false))
                                            return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    }
                                    //else // Common CheckBox style
                                    //        -> Always change CheckBox state (whatever its state)
                                    break;
                                }
                            }

                            // Press left mHelp cursor button
                            return (int)mHpCtrlConst.ActType.ActLeftPress;
                        }
                        case 3: { return (int)mHpCtrlConst.ActType.ActLeftUnpress; } // Unpress left mHelp cursor button
                        case 4:
                        {
                            // Get CheckBox control
                            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

                            // Check new state (Only for 3 state CheckBox)
                            switch ((int)CtrlState[0].Value[0])
                            {
                                case mHpCheckBoxState.UNCHECKED: ////// Uncheck
                                {
                                    // Check if already unchecked
                                    if (ChckBoxCtrl.IsChecked == false)
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                                case mHpCheckBoxState.CHECKED: ////// Check
                                {
                                    // Check if already checked
                                    if (ChckBoxCtrl.IsChecked == true)
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                                case mHpCheckBoxState.UNDEFINED: ////// Indeterminate/Change (CHANGESTATE)
                                {
                                    // Check 3 state CheckBox style
                                    if (ChckBoxCtrl.IsThreeState)
                                    {
                                        // Check if already indeterminate
                                        if ((ChckBoxCtrl.IsChecked != true) && (ChckBoxCtrl.IsChecked != false))
                                            return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    }
                                    else
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                            }

                            // Wait a little (avoid double click)
                            return (int)mHpCtrlConst.ActType.ActWait;
                        }
                        case 5: { return (int)mHpCtrlConst.ActType.ActEnd; } // Finished
                    }
                }
            }

            // Error
            return (int)mHpCtrlConst.ActType.ActError;
        }

        ////// Start
        public override bool Start(Int32 hForm)
        {
            // Get control object (using standard control identifier)
            if (!GetControl((IntPtr)hForm))
                return false; // Failed to get CheckBox object in the Form
            //else
            // -> Control object found (stored in 'CtrlObj')

            // Get CheckBox control
            CheckBox ChkCtrl = CtrlObj as CheckBox;

            // Check CheckBox status...
            if ((!ChkCtrl.IsVisible) ||   // ...visible
                (!ChkCtrl.IsEnabled))     // ...enabled
                return false;

            // Check CheckBox style
            if ((CtrlStyle != STYLE_CHECKBOX_COMMON) && (CtrlStyle != STYLE_CHECKBOX_3STATE))
                return false;

            // Reset action step
            ActStep = 0;

            // CheckBox control is ready
            return true;
        }

        ////// MoveTo
        public override int MoveToX()
        {
            // Check valid action step
            if (ActStep != 1)
                return mHpCtrlConst.NO_VALUE;

            // Get visual and CheckBox object
            Visual VisualChk = CtrlObj as Visual;

            // Get CheckBox coordinate in the screen
            System.Windows.Point ChkPos = VisualChk.PointToScreen(new System.Windows.Point(0, 0));

            // Return new mHelp cursor horizontal position
            return (int)(ChkPos.X + 8);
        }
        public override int MoveToY()
        {
            // Check valid action step
            if (ActStep != 1)
                return mHpCtrlConst.NO_VALUE;

            // Get visual and CheckBox object
            Visual VisualChk = CtrlObj as Visual;
            CheckBox ChkCtrl = CtrlObj as CheckBox;

            // Get CheckBox coordinate in the screen
            System.Windows.Point ChkPos = VisualChk.PointToScreen(new System.Windows.Point(0, 0));

            // Return new mHelp cursor vertical position
            return (int)((ChkCtrl.ActualHeight / 2) + ChkPos.Y - 4);
        }

        ////// LeftPressAct
        public override bool LeftPressAct()
        {
            // Get CheckBox control
            CheckBox ChkCtrl = CtrlObj as CheckBox;

            // Set focus on CheckBox
            ChkCtrl.Focus();

            return true;
        }

        ////// LeftUnpressAct
        public override bool LeftUnpressAct()
        {
            // Get CheckBox control
            CheckBox ChkCtrl = CtrlObj as CheckBox;

            // Change CheckBox state by launching the 'Click' event programmatically
            ChkCtrl.RaiseEvent(new System.Windows.RoutedEventArgs(CheckBox.ClickEvent, ChkCtrl));

            return true;
        }

        ////// WaitUntil
        public override bool WaitUntil()
        {
            // Wait a little to avoid displaying mHelp cursor double click
            System.Threading.Thread.Sleep(500);

            // New click action to set the right state of the CheckBox 3 states
            ActStep = 1;

            return true;
        }

        //////
        public override bool MoveAct() { return true; }
        public override bool End() { return true; }
        public override bool RightPressAct() { return true; }
        public override bool RightUnpressAct() { return true; }
        public override bool EditAct() { return true; }

        ///////////////////////////////////////////////////

        public CmHpWPFCheckBox()
        {
            // Set WPF Button DLL version
            DllVersion = Assembly.GetExecutingAssembly().GetName().Version.ToString();

            ////// WARNING: Always define event(s) in constructor
            // -> Define & Assign control event(s) to be registered

            CtrlEvent = new string[3]; // Three CheckBox events are available

            CtrlEvent[0] = EVENT_CHECKBOX_CHECKED; // 'Checked' event
            CtrlEvent[1] = EVENT_CHECKBOX_UNCHECKED; // 'Unchecked' event
            CtrlEvent[2] = EVENT_CHECKBOX_INDETERMINATE; // 'Indeterminate' event
        }
    }
}

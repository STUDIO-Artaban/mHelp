/// <summary>
/// mHpDotNetCheckBox.cs - Interface for mHelp .NET CheckBox Control Management
/// mHelp version - 1.3
/// Platform - .NET (C#, VBNET, C++/CLI)
/// Framework .NET version - 3.5
/// Date - 01/15/2013
/// 
/// Copyright(c) - Unknown Corporation. All rights reserved.
/// </summary>

using mHpDotNetCtrl;

using System;
using System.Drawing;
using System.Reflection;
using System.ComponentModel;
using System.Windows.Forms;
using System.Threading;

namespace mHpDotNetCheckBox
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
    /// Class CmHpDotNetCheckBox - CmHpDotNetCtrl abstract class implementation for CheckBox control
    /// </summary>
    public class CmHpDotNetCheckBox : CmHpDotNetCtrl<CheckBox>
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
        private const string EVENT_CHECKBOX_CHECKSTATECHANGED = "mHpCheckBoxEvent_CheckStateChanged";

        // ************************************************
        // Properties

        // ------------------------------------------------
        // Object
        //

        //// IsCtrlObject
        public override bool IsCtrlObject(string ObjectName)
        {
            // Compare CheckBox object type name
            CheckBox ChkBoxObj = new CheckBox();
            if (ObjectName == ChkBoxObj.GetType().Name)
                return true;

            return false;
        }

        //// GetCtrlObject
        public override string GetCtrlObject(uint iObjIdx)
        {
            // Check right CheckBox object type index (== 0)
            if (iObjIdx < 1)
            {
                CheckBox ChkBoxObj = new CheckBox();
                return ChkBoxObj.GetType().Name;
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
            // Return CheckBox version (.NET framework version)
            return Properties.Resources.RS_CHECKBOX_VERS;
        }

        //// GetCtrlDesc
        public override string GetCtrlDesc()
        {
            // Return CheckBox version (.NET framework version)
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
            if ((CtrlStyle == STYLE_CHECKBOX_COMMON) ||
                (CtrlStyle == STYLE_CHECKBOX_3STATE))
                return 0;   // CheckBox icon index (same icon)

            // Wrong CheckBox style
            return mHpCtrlConst.NO_VALUE;
        }

        // ------------------------------------------------
        // User Interface (UI)
        //

        /// <summary>
        /// CheckBoxCfgForm - mHelp .NET CheckBox control state Form
        /// </summary>
        private mHpCheckBoxForm CheckBoxCfgForm = null;

        ////// CreateUI
        public override IntPtr CreateUI(Int32 hSaveBtn)
        {
            // Create new mHelp .NET CheckBox control state Form instance
            CheckBoxCfgForm = new mHpCheckBoxForm(hSaveBtn);
            CheckBoxCfgForm.Init((int)CtrlState[0].Value[0],
                                 (CtrlStyle == STYLE_CHECKBOX_3STATE)? true:false);

            return CheckBoxCfgForm.Handle;
        }

        ////// SaveUI
        public override void SaveUI()
        {
            // Assign new configured CheckBox state
            CtrlState[0].Value[0] = CheckBoxCfgForm.State;
        }

        ////// CloseUI
        public override void CloseUI()
        {
            // Close mHelp .NET CheckBox control state Form
            if (CheckBoxCfgForm != null)
                CheckBoxCfgForm.Close();

            CheckBoxCfgForm = null;
        }

        // ------------------------------------------------
        // Style
        //

        ////// SetStyle
        public override bool SetStyle(int iStyle)
        {
            // Check valid style
            if ((iStyle != STYLE_CHECKBOX_COMMON) &&
                (iStyle != STYLE_CHECKBOX_3STATE))
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

        private const string METHOD_CHECKBOX_CHECKSTATECHANGED = "OnmHpCheckBox_CheckStateChanged";
        private void OnmHpCheckBox_CheckStateChanged(object sender, EventArgs e)
        {
            // Get CheckBox that send this event
            CheckBox CurChckBox = sender as CheckBox;

            ////// WARNING: Always check triggered user event
            // -> Using code below
            if (!CurChckBox.Focused)
                return;

            // Set CheckBox control identifier
            // -> Using common control identifier (CheckBox.Name)
            SetControl(CurChckBox as Control);

            // Set current control style
            if (!CurChckBox.ThreeState)
                SetStyle(STYLE_CHECKBOX_COMMON);
            else
                SetStyle(STYLE_CHECKBOX_3STATE);

            // Set CheckBox control config (call 'SetStyle' first)
            // -> No config available for CheckBox control

            // Set CheckBox state (call 'SetStyle' first)
            switch (CurChckBox.CheckState)
            {
                case System.Windows.Forms.CheckState.Unchecked:
                {
                    CtrlState[0].Value[0] = mHpCheckBoxState.UNCHECKED; // Unchecked
                    break;
                }
                case System.Windows.Forms.CheckState.Checked:
                {
                    CtrlState[0].Value[0] = mHpCheckBoxState.CHECKED; // Checked
                    break;
                }
                case System.Windows.Forms.CheckState.Indeterminate:
                {
                    CtrlState[0].Value[0] = mHpCheckBoxState.UNDEFINED; // Indeterminate (CHANGESTATE)
                    break;
                }
            }

            // Set current control object name
            CtrlObjName = CurChckBox.GetType().Name; // One CheckBox object name available (CheckBox)

            // Set current state bound
            StateBound = "[0;2]";   // 0: Uncheck
                                    // 1: Check
                                    // 2: Indeterminate/Change

            ////// WARNING: Always call function below for all control event occurence
            // -> Send window message in order to inform mHelp a CheckBox 'CheckStateChanged' event has occurred
            SendmHpEvent(sender, EVENT_CHECKBOX_CHECKSTATECHANGED);
        }

        ////// AddmHpEvent
        protected override bool AddmHpEvent(Control Ctrl, string RegCtrlEvent)
        {
            CheckBox ChckBoxCtrl = Ctrl as CheckBox;

            // Add mHelp CheckBox 'CheckedChanged' event handler in under to catch it in the capture processus
            BindingFlags allBindingFlags = BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance;
            FieldInfo EvtField = typeof(CheckBox).GetField("EVENT_CHECKSTATECHANGED", allBindingFlags);
            if (EvtField == null)
                return false;
            Object objFieldEvt = EvtField.GetValue(ChckBoxCtrl);
            if (objFieldEvt == null)
                return false;
            PropertyInfo EvtProp = ChckBoxCtrl.GetType().GetProperty("Events", allBindingFlags);
            if (EvtProp == null)
                return false;
            EventHandlerList EvtHandlerList = (EventHandlerList)EvtProp.GetValue(ChckBoxCtrl, null);
            if (EvtHandlerList == null)
                return false;

            // Check existing CheckBox 'CheckedChanged' event handler
            if ((EvtHandlerList[objFieldEvt] != null) && (EvtHandlerList[objFieldEvt].GetInvocationList().GetLength(0) > 0))
            {
                ////// WARNING: This method can be called several times for the same control
                // -> Check if mHelp event handler has already been added
                bool bEvtmHpAdded = false;
                foreach (Delegate CurDelegate in EvtHandlerList[objFieldEvt].GetInvocationList())
                {
                    if (CurDelegate.Method.Name == METHOD_CHECKBOX_CHECKSTATECHANGED)
                    {
                        // mHelp event handler has already been added
                        bEvtmHpAdded = true;
                        break;
                    }
                }

                // Stock & remove all previous events
                Delegate[] aPrevDelegate = new Delegate[EvtHandlerList[objFieldEvt].GetInvocationList().GetLength(0)];

                // Add if needed mHelp event handler at the top of the event list...
                if (!bEvtmHpAdded)
                    ChckBoxCtrl.Click += new EventHandler(OnmHpCheckBox_CheckStateChanged);

                int aIdx = 0;
                foreach (Delegate CurDelegate in EvtHandlerList[objFieldEvt].GetInvocationList())
                {
                    // Do not remove mHelp event handler (already existing or added above)
                    // -> 'Click' event handler should not be a null pointer
                    if (CurDelegate.Method.Name != METHOD_CHECKBOX_CHECKSTATECHANGED)
                    {
                        aPrevDelegate[aIdx++] = (Delegate)CurDelegate.Clone();
                        EvtHandlerList.RemoveHandler(objFieldEvt, CurDelegate);
                    }

                } // ...after having removed all previous event(s)

                // Restore all previous event(s)
                foreach (Delegate CurDelegate in aPrevDelegate)
                    EvtHandlerList.AddHandler(objFieldEvt, CurDelegate);
            }
            else // Add event (first event cos no other 'CheckedChanged' event handler defined)
                ChckBoxCtrl.CheckedChanged += new EventHandler(OnmHpCheckBox_CheckStateChanged);

            return true;
        }

        // --------------------------------
        // Reproduce processus
        //

        ////// GetCtrlLeft
        public override int GetCtrlLeft()
        {
            // Get CheckBox control
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            // Get CheckBox coordinate in the screen
            Point ChkBoxPos = ChckBoxCtrl.PointToScreen(Point.Empty);

            return ChkBoxPos.X;
        }

        ////// GetCtrlTop
        public override int GetCtrlTop()
        {
            // Get CheckBox control
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            // Get CheckBox coordinate in the screen
            Point ChkBoxPos = ChckBoxCtrl.PointToScreen(Point.Empty);

            return ChkBoxPos.Y;
        }

        ////// GetCtrlWidth
        public override int GetCtrlWidth()
        {
            // Get CheckBox control
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            return ChckBoxCtrl.Width;
        }

        ////// GetCtrlHeight
        public override int GetCtrlHeight()
        {
            // Get CheckBox control
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            return ChckBoxCtrl.Height;
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
                if (GetCtrlEvent() == EVENT_CHECKBOX_CHECKSTATECHANGED)
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
                                    if (ChckBoxCtrl.CheckState == System.Windows.Forms.CheckState.Unchecked)
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                                case mHpCheckBoxState.CHECKED: ////// Check
                                {
                                    // Check if already checked
                                    if (ChckBoxCtrl.CheckState == System.Windows.Forms.CheckState.Checked)
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                                case mHpCheckBoxState.UNDEFINED: ////// Indeterminate/Change (CHANGESTATE)
                                {
                                    // Check 3 state CheckBox style
                                    if (CtrlStyle == STYLE_CHECKBOX_3STATE)
                                    {
                                        // Check if already indeterminate
                                        if (ChckBoxCtrl.CheckState == System.Windows.Forms.CheckState.Indeterminate)
                                            return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    }
                                    //else // Common CheckBox style
                                    // -> Always change CheckBox state (whatever its state)
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
                                    if (ChckBoxCtrl.CheckState == System.Windows.Forms.CheckState.Unchecked)
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                                case mHpCheckBoxState.CHECKED: ////// Check
                                {
                                    // Check if already checked
                                    if (ChckBoxCtrl.CheckState == System.Windows.Forms.CheckState.Checked)
                                        return (int)mHpCtrlConst.ActType.ActEnd; // Finished
                                    break;
                                }
                                case mHpCheckBoxState.UNDEFINED: ////// Indeterminate/Change (CHANGESTATE)
                                {
                                    // Check CheckBox style
                                    if (ChckBoxCtrl.ThreeState)
                                    {
                                        // Check if already indeterminate
                                        if (ChckBoxCtrl.CheckState == System.Windows.Forms.CheckState.Indeterminate)
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
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            // Check CheckBox status...
            if ((!ChckBoxCtrl.Visible) ||   // ...visible
                (!ChckBoxCtrl.Enabled))     // ...enabled
                return false;

            // Check CheckBox style
            if (((CtrlStyle == STYLE_CHECKBOX_3STATE) && (!ChckBoxCtrl.ThreeState)) ||
                ((CtrlStyle == STYLE_CHECKBOX_COMMON) && (ChckBoxCtrl.ThreeState)))
                return false; // Wrong style

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

            // Get CheckBox control
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            // Get CheckBox coordinate in the screen
            Point ChckBoxPos = ChckBoxCtrl.PointToScreen(Point.Empty);

            // Return new mHelp cursor horizontal position
            return (ChckBoxPos.X + 8);
        }
        public override int MoveToY()
        {
            // Check valid action step
            if (ActStep != 1)
                return mHpCtrlConst.NO_VALUE;

            // Get CheckBox control
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            // Get CheckBox coordinate in the screen
            Point ChckBoxPos = ChckBoxCtrl.PointToScreen(Point.Empty);

            // Return new mHelp cursor vertical position
            return ((ChckBoxCtrl.Height / 2) + ChckBoxPos.Y - 4);
        }

        ////// LeftPressAct
        public override bool LeftPressAct()
        {
            // Get CheckBox control
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            // Set focus on CheckBox
            ChckBoxCtrl.Focus();

            return true;
        }
        
        ////// LeftUnpressAct
        public override bool LeftUnpressAct()
        {
            // Get CheckBox control
            CheckBox ChckBoxCtrl = CtrlObj as CheckBox;

            // Launch event(s) according CheckBox style and state
            if (ChckBoxCtrl.ThreeState) // 3 state CheckBox style
            {
                switch (ChckBoxCtrl.CheckState)
                {   case System.Windows.Forms.CheckState.Checked:
                    {
                        // Next CheckBox state: Checked -> Indeterminate
                        ChckBoxCtrl.CheckState = System.Windows.Forms.CheckState.Indeterminate;
                        break;
                    }
                    case System.Windows.Forms.CheckState.Unchecked:
                    {
                        // Next CheckBox state: Unchecked -> Checked
                        ChckBoxCtrl.CheckState = System.Windows.Forms.CheckState.Checked;
                        break;
                    }
                    case System.Windows.Forms.CheckState.Indeterminate:
                    {
                        // Next CheckBox state: Indeterminate -> Unchecked
                        ChckBoxCtrl.CheckState = System.Windows.Forms.CheckState.Unchecked;
                        break;
                    }
                }
            }
            else // Common CheckBox style
            {
                // Change CheckBox state
                ChckBoxCtrl.Checked = !ChckBoxCtrl.Checked;
            }

            return true;
        }

        ////// WaitUntil
        public override bool WaitUntil()
        {
            // Wait a little to avoid displaying mHelp cursor double click
            Thread.Sleep(500);

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

        public CmHpDotNetCheckBox()
        { 
            // Set DotNET CheckBox DLL version
            DllVersion = Assembly.GetExecutingAssembly().GetName().Version.ToString();

            ////// WARNING: Always define event(s) in constructor
            // -> Define & Assign control event(s) to be registered
            CtrlEvent = new string[1];
            CtrlEvent[0] = EVENT_CHECKBOX_CHECKSTATECHANGED; // 'CheckStateChanged' event
        }
    }
}

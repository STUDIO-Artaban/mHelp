/// <summary>
/// mHpWPFButton.cs - Interface for mHelp WPF Button Control Management
/// mHelp version - 1.3
/// Framework .NET version - 3.5
/// Platform - WPF
/// Date - 02/01/2013
/// 
/// Copyright(c) - Unknown Corporation. All rights reserved.
/// </summary>

using mHpWPFCtrl;

using System;
using System.Windows.Media;
using System.Windows.Controls;
using System.Reflection;

namespace mHpWPFButton
{
    /// <summary>
    /// Class CmHpWPFButton - CmHpWPFCtrl abstract class implementation for Button control
    /// </summary>
    public class CmHpWPFButton : CmHpWPFCtrl<Button>
    {
        /// <summary>
        /// Button style definition(s)
        /// </summary>
        private const int STYLE_BUTTON = 0;

        /// <summary>
        /// Button event definition(s)
        /// </summary>
        private const string EVENT_BUTTON_CLICK = "mHpWPFButton_Click";

        // ************************************************
        // Properties

        // ------------------------------------------------
        // Object
        //

        //// IsCtrlObject
        public override bool IsCtrlObject(string ObjectName)
        {
            // Compare Button object type name
            Button BtnObj = new Button();
            if (ObjectName == BtnObj.GetType().Name)
                return true;

            return false;
        }

        //// GetCtrlObject
        public override string GetCtrlObject(uint iObjIdx)
        {
            // Check right Button object type index (== 0)
            if (iObjIdx < 1)
            {
                Button BtnObj = new Button();
                return BtnObj.GetType().Name;
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
                return Properties.Resources.RS_BUTTON_NAME;

            // ...according index parameter
            if (iNameIdx < 1) // One control name is available for Button
                return Properties.Resources.RS_BUTTON_NAME; // Button

            return null;
        }

        // ------------------------------------------------
        // Info
        //

        //// GetCtrlVers
        public override string GetCtrlVers()
        {
            // Return WPF Button version (.NET framework version)
            return Properties.Resources.RS_BUTTON_VERS;
        }

        //// GetCtrlDesc
        public override string GetCtrlDesc()
        {
            // Return WPF Button version (.NET framework version)
            return Properties.Resources.RS_BUTTON_DESC;
        }

        ////// GetIconList
        public override IntPtr GetIconList(uint iIconIdx)
        {
            // Return icon according specific index
            if (iIconIdx < 1)
                return Properties.Resources.mHpButton.Handle;   // Button icon

            // No more icon
            return IntPtr.Zero;
        }

        ////// GetIconIdx
        public override int GetIconIdx()
        {
            // Return icon index according Button style (see 'GetIconList' method)
            if (CtrlStyle == STYLE_BUTTON)
                return 0;   // Button icon index

            // Wrong Button style
            return mHpCtrlConst.NO_VALUE;
        }

        // ------------------------------------------------
        // User Interface (UI)
        //

        ////// No UI for Button control (no state to configure)
        public override IntPtr CreateUI(Int32 hSaveBtn) { return IntPtr.Zero; }
        public override void SaveUI() { }
        public override void CloseUI() { }

        // ------------------------------------------------
        // Style
        //

        ////// SetStyle
        public override bool SetStyle(int iStyle)
        {
            // Check valid style
            if (iStyle != STYLE_BUTTON)
                return false; // Wrong Button style

            // Assign control style
            CtrlStyle = iStyle;

            // Define state & config class instance according control style
            if (CtrlCfg == null)
            {
                // Define Button config (Button label)
                CtrlCfg = new mHpCtrlData[1];
                CtrlCfg[0] = new mHpCtrlData("BtnLabel", mHpCtrlData.ValueType.String);
                CtrlCfg[0].Value = new Object[1]; // Always == 1 (mHpCtrlData.ValueType.String)
            }

            return true;
        }

        // ------------------------------------------------
        // State (No state for Button control)
        //
        public override uint GetStateCount() { return 0; }
        public override uint GetStateCount(uint iStateIdx) { return 0; }
        public override string GetStateName(uint iStateIdx) { return null; }
        public override int GetStateType(uint iStateIdx) { return mHpCtrlConst.NO_VALUE; }
        public override int GetIntState(uint iStateIdx, uint iArrayIdx = 0) { return mHpCtrlConst.NO_VALUE; }
        public override string GetStrState(uint iStateIdx, uint iArrayIdx = 0) { return null; }
        public override bool NeedToCheckState() { return false; }

        ////// DisplayState
        public override string DisplayState()
        {
            // No Button state is available
            // -> But display Button label
            return Properties.Resources.RS_NO_STATE + " (" + (string)CtrlCfg[0].Value[0] + ")";
        }

        // ------------------------------------------------
        // Config
        //

        ////// GetCfgCount
        public override uint GetCfgCount()
        {
            // Return Button config count (== 1 for Button label)
            return 1;
        }
        public override uint GetCfgCount(uint iCfgIdx)
        {
            // Check valid Button config index (== 0 index for Button label)
            if (iCfgIdx != 0)
                return 0;

            // Check valid control config
            if (CtrlCfg[iCfgIdx].Value == null)
                return 0;

            // Button config string value type count
            return (uint)CtrlCfg[iCfgIdx].Value.Length; // Always == 1 (mHpCtrlData.ValueType.String)
        }

        ////// GetCfgType
        public override int GetCfgType(uint iCfgIdx)
        {
            // Check Button config index (== 0)
            if (iCfgIdx != 0)
                return mHpCtrlConst.NO_VALUE; // Wrong Button config index

            // Return Button config value type (String for Button label)
            return (int)CtrlCfg[iCfgIdx].Type;
        }

        ////// GetCfgName
        public override string GetCfgName(uint iCfgIdx)
        {
            // Check Button config index (== 0)
            if (iCfgIdx != 0)
                return null; // Wrong Button config index

            // Return Button config name
            return CtrlCfg[iCfgIdx].Name; // 'BtnLabel'
        }

        ////// GetIntCfg
        public override int GetIntCfg(uint iCfgIdx, uint iArrayIdx = 0)
        {
            // No Integer Button config value type
            return mHpCtrlConst.NO_VALUE;
        }

        ////// GetStrCfg
        public override string GetStrCfg(uint iCfgIdx, uint iArrayIdx = 0)
        {
            // Check Button config index without no array (== 0)
            if ((iCfgIdx != 0) || (iArrayIdx != 0))
                return null; // Wrong Button config index

            // Return Button config value
            return (string)CtrlCfg[iCfgIdx].Value[iArrayIdx]; // Button label
        }

        // ------------------------------------------------
        // HTML tags
        //

        //// GetHtmlTags
        public override string GetHtmlTags(uint iTaskIdx) { return null; }  // Return null
                                                                            // -> No state for Button control

        // ------------------------------------------------
        // Capture processus
        //

        private const string METHOD_BUTTON_CLICK = "OnmHpButtonClick";
        private void OnmHpButtonClick(object sender, System.Windows.RoutedEventArgs e)
        {
            // Get Button that send this event
            Button CurBtn = sender as Button;

            ////// WARNING: Always check triggered user event
            // -> Using code below
            if (!CurBtn.Focus())
                return;

            // Set Button control identifier
            // -> Using common control identifier (Button.Name)
            SetControl(CurBtn as Control);

            // Set current control style
            SetStyle(STYLE_BUTTON); // One Button style available

            // Set Button control config (call 'SetStyle' first)
            CtrlCfg[0].Value[0] = (string)CurBtn.Content; // Button label

            // Set Button state (call 'SetStyle' first)
            // -> No state available for Button control

            // Set current control object name
            CtrlObjName = CurBtn.GetType().Name; // One Button object name available (Button)

            // Set current state bound
            StateBound = null; // No state available for Button control

            ////// WARNING: Always call function below for all control event occurence
            // -> Send window message in order to inform mHelp a Button 'Click' event has occurred
            SendmHpEvent(sender, EVENT_BUTTON_CLICK);
        }

        ////// AddmHpEvent
        protected override bool AddmHpEvent(Visual Ctrl, string RegCtrlEvent)
        {
            Button BtnCtrl = Ctrl as Button;
            System.Windows.UIElement CtrlElement = BtnCtrl as System.Windows.UIElement;

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
            System.Windows.RoutedEventHandlerInfo[] rtdEvtHdlrInfoTab = (System.Windows.RoutedEventHandlerInfo[])rtdEvtHdlrMth.Invoke(evtHdlrStoreObj, new object[] { System.Windows.Controls.Primitives.ButtonBase.ClickEvent });
            if (rtdEvtHdlrInfoTab == null)
                return false;

            // Check existing Button 'Click' routed event handler
            if (rtdEvtHdlrInfoTab.Length > 0)
            {
                ////// WARNING: This method can be called several times for the same control
                // -> Check if mHelp routed event handler has already been added
                bool bEvtmHpAdded = false;
                foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                {
                    if (evtHdlrInfo.Handler.Method.Name == METHOD_BUTTON_CLICK)
                    {
                        // mHelp routed event handler has already been added
                        bEvtmHpAdded = true;
                        break;
                    }
                }

                // Add if needed mHelp routed event handler at the top of the event list...
                if (!bEvtmHpAdded)
                    BtnCtrl.Click += new System.Windows.RoutedEventHandler(OnmHpButtonClick);

                // Remove all previous events
                foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                {
                    if (evtHdlrInfo.Handler.Method.Name != METHOD_BUTTON_CLICK)
                        BtnCtrl.Click -= (System.Windows.RoutedEventHandler)evtHdlrInfo.Handler;

                }  // ...after having removed all previous event(s)

                // Restore all previous event(s)
                foreach (System.Windows.RoutedEventHandlerInfo evtHdlrInfo in rtdEvtHdlrInfoTab)
                {
                    if (evtHdlrInfo.Handler.Method.Name != METHOD_BUTTON_CLICK)
                        BtnCtrl.Click += (System.Windows.RoutedEventHandler)evtHdlrInfo.Handler;
                }
            }
            else
                BtnCtrl.Click += new System.Windows.RoutedEventHandler(OnmHpButtonClick);

            return true;
        }

        // --------------------------------
        // Reproduce processus
        //

        ////// GetCtrlLeft
        public override int GetCtrlLeft()
        {
            // Get visual Button
            Visual VisualBtn = CtrlObj as Visual;

            // Get Button coordinate in the screen
            System.Windows.Point BtnPos = VisualBtn.PointToScreen(new System.Windows.Point(0, 0));

            return (int)BtnPos.X;
        }

        ////// GetCtrlTop
        public override int GetCtrlTop()
        {
            // Get visual Button
            Visual VisualBtn = CtrlObj as Visual;

            // Get Button coordinate in the screen
            System.Windows.Point BtnPos = VisualBtn.PointToScreen(new System.Windows.Point(0, 0));

            return (int)BtnPos.Y;
        }

        ////// GetCtrlWidth
        public override int GetCtrlWidth()
        {
            // Get Button control
            Button BtnCtrl = CtrlObj as Button;

            return (int)BtnCtrl.ActualWidth;
        }

        ////// GetCtrlHeight
        public override int GetCtrlHeight()
        {
            // Get Button control
            Button BtnCtrl = CtrlObj as Button;

            return (int)BtnCtrl.ActualHeight;
        }

        ////// CheckState
        public override bool CheckState()
        {
            return true; // Can always set state
                         // -> Because of Button has no state!
        }

        ////// NextAction
        public override int NextAction()
        {
            // Next action step
            ActStep++;

            // Switch on event to reproduce
            if (GetCtrlEvent() == EVENT_BUTTON_CLICK)
            {
                // Switch on current action step
                switch (ActStep)
                {
                    case 1: { return (int)mHpCtrlConst.ActType.ActMove; } // Move mHelp cursor over the control
                    case 2: { return (int)mHpCtrlConst.ActType.ActLeftPress; } // Press left mHelp cursor button
                    case 3: { return (int)mHpCtrlConst.ActType.ActLeftUnpress; } // Unpress left mHelp cursor button
                    case 4: { return (int)mHpCtrlConst.ActType.ActEnd; } // Finished
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
                return false; // Failed to get Button object in the Form
            //else
            // -> Control object found (stored in 'CtrlObj')

            // Get Button control
            Button BtnCtrl = CtrlObj as Button;

            // Check Button status...
            if ((!BtnCtrl.IsVisible) ||   // ...visible
                (!BtnCtrl.IsEnabled))     // ...enabled
                return false;

            // Check Button style
            if (CtrlStyle != STYLE_BUTTON)
                return false;

            // Reset action step
            ActStep = 0;

            // Button control is ready
            return true;
        }

        ////// MoveTo
        public override int MoveToX()
        {
            // Check valid action step
            if (ActStep != 1)
                return mHpCtrlConst.NO_VALUE;

            // Get visual and Button object
            Visual VisualBtn = CtrlObj as Visual;
            Button BtnCtrl = CtrlObj as Button;

            // Get Button coordinate in the screen
            System.Windows.Point BtnPos = VisualBtn.PointToScreen(new System.Windows.Point(0, 0));

            // Return new mHelp cursor horizontal position
            return (int)((BtnPos.X + BtnCtrl.ActualWidth) - 9);
        }
        public override int MoveToY()
        {
            // Check valid action step
            if (ActStep != 1)
                return mHpCtrlConst.NO_VALUE;

            // Get visual and Button object
            Visual VisualBtn = CtrlObj as Visual;
            Button BtnCtrl = CtrlObj as Button;

            // Get Button coordinate in the screen
            System.Windows.Point BtnPos = VisualBtn.PointToScreen(new System.Windows.Point(0, 0));

            // Return new mHelp cursor vertical position
            return (int)((BtnPos.Y + BtnCtrl.ActualHeight) - 10);
        }

        ////// LeftPressAct
        public override bool LeftPressAct()
        {
            // Get Button control
            Button BtnCtrl = CtrlObj as Button;

            // Set focus on Button
            BtnCtrl.Focus();

            return true;
        }

        ////// LeftUnpressAct
        public override bool LeftUnpressAct()
        {
            // Get Button control
            Button BtnCtrl = CtrlObj as Button;

            // Launch the 'Click' event programmatically
            BtnCtrl.RaiseEvent(new System.Windows.RoutedEventArgs(Button.ClickEvent, BtnCtrl));

            return true;
        }

        //////
        public override bool MoveAct() { return true; }
        public override bool End() { return true; }
        public override bool WaitUntil() { return true; }
        public override bool RightPressAct() { return true; }
        public override bool RightUnpressAct() { return true; }
        public override bool EditAct() { return true; }

        ///////////////////////////////////////////////////

        public CmHpWPFButton()
        {
            // Set WPF Button DLL version
            DllVersion = Assembly.GetExecutingAssembly().GetName().Version.ToString();

            ////// WARNING: Always define event(s) in constructor
            // -> Define & Assign control event(s) to be registered
            CtrlEvent = new string[1]; // One Button event is available ('Click' event)
            CtrlEvent[0] = EVENT_BUTTON_CLICK;
        }
    }
}

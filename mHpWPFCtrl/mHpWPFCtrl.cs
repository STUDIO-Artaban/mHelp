/// <summary>
/// mHpWPFCtrl.cs - Interface for mHelp WPF Controls Management
/// mHelp version - 1.3
/// Framework .NET version - 3.5
/// Platform - WPF
/// Date - 02/01/2013
/// 
/// Copyright(c) - Unknown Corporation. All rights reserved.
/// </summary>

using System;
using System.Collections;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace mHpWPFCtrl
{
    /// <summary>
    /// Constants class mHpCtrlConst
    /// </summary>
    public static class mHpCtrlConst
    {
        // No value
        public const int NO_VALUE = (-1);

        /// <summary>
        /// RegisterWindowMessage - Invoke function to register a new window message
        /// </summary>
        [DllImport("User32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern uint RegisterWindowMessage(string mHpCtrlEventMsg);

        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, IntPtr lParam);

        /// <summary>
        /// ActType - Action type (for reproduce processus)
        /// </summary>
        public enum ActType : int
        {
            ActError = 0,       // Error (display 'LastErrMsg' if any and stop process)
            ActMove,            // Move mouse (move to 'MoveToX' & 'MoveToY' definitions and call 'MoveAct' when positionned)
            ActWait,            // Wait (call 'WaitUntil')
            ActLeftPress,       // Left mouse button press (call 'LeftPressAct')
            ActLeftUnpress,     // Left mouse button unpress (call 'LeftUnpressAct')
            ActRightPress,      // Right mouse button press (call 'RightPressAct')
            ActRightUnpress,    // Right mouse button unpress (call 'RightUnpressAct')
            ActLeftDblClick,    // Left mouse button double click (call 'LeftPressAct', 'LeftUnpressAct', 'LeftPressAct' and 'LeftUnpressAct' successively)
            ActRightDblClick,   // Right mouse button double click (call 'RightPressAct', 'RightUnpressAct', 'RightPressAct' and 'RightUnpressAct' successively)
            ActEdit,            // Edit text (call 'EditAct')
            ActEnd              // End (call 'End' - Finish process)

            ////// After all action mHelp processus call 'NextAction' until 'ActEnd' or 'ActError' are returned
        };
    }

    /// <summary>
    /// Data class mHpCtrlData
    /// </summary>
    public class mHpCtrlData
    {
        /// <summary>
        /// ValueType - Data value type definitions
        /// </summary>
        public enum ValueType : int
        {
            Undefined = 0,  // Undefined type (no value)
            Integer,        // Integer type
            String,         // String type
            IntArray,       // Integer array type
            StrArray        // String array type
        };

        public Object[] Value;      // Data value
        private ValueType DataType; // Data value type
        private string DataName;    // Data name

        ///////////////////////////////////////////////////

        /// <summary>
        /// mHpCtrlData - Constructor
        /// </summary>
        /// <param name="strName">Data name</param>
        public mHpCtrlData(string strName, ValueType valType)
        {
            // Assign data name and type
            DataName = strName;
            DataType = valType;
        }

        /// <summary>
        /// Accessors
        /// </summary>
        public string Name
        {
            get { return DataName; }
        }
        public ValueType Type
        {
            get { return DataType; }
        }
    }

    /// <summary>
    /// Class CmHpWPFCtrl - Class to derive and implement in order to manage WPF control
    /// </summary>
    public abstract class CmHpWPFCtrl<WPFCtrl>
    {
        // ************************************************
        // Properties

        /// <summary>
        /// EventRegTab - Event window message registered hashtable
        /// </summary>
        private Hashtable EventRegTab;

        // Control style identifier
        protected int CtrlStyle;

        // Control event identifier
        private int CtrlEventId;
        protected string[] CtrlEvent;

        protected mHpCtrlData CtrlId;       // Control identifier (Control.Name)
        protected mHpCtrlData[] CtrlState;  // Control state
        protected mHpCtrlData[] CtrlCfg;    // Control config

        protected string HtmlTags;  // HTML tags
        protected Object CtrlObj;   // Control object (reproduce processus)

        // Action step identifier
        protected int ActStep;

        /// <summary>
        /// LastError - String containing the last error message
        /// DllVersion - DLL version (e.g. 1.0.10.52)
        /// CtrlObjName - Control object name
        /// StateBound - Control state bound
        /// </summary>
        protected string LastErrMsg;
        protected string DllVersion;
        protected string CtrlObjName;
        protected string StateBound;

        ////// Implemented methods (Reserved)

        /// <summary>
        /// ClearData - Empty all control data
        /// </summary>
        public void ClearData()
        {
            // Clear properties, state, config & ID
            CtrlStyle = mHpCtrlConst.NO_VALUE;
            CtrlEventId = mHpCtrlConst.NO_VALUE;
            CtrlObjName = null;
            CtrlObj = null;

            // State
            CtrlState = null;

            // Config
            CtrlCfg = null;

            // ID
            if (CtrlId != null)
                CtrlId.Value = null;
        }

        /// <summary>
        /// GetLastErr - Return the last error message
        /// </summary>
        /// <returns>Last error message</returns>
        public string GetLastErr() { return LastErrMsg; }

        /// <summary>
        /// GetDllVersion - Return WPF control DLL version
        /// </summary>
        /// <returns>DLL version</returns>
        public string GetDllVersion() { return DllVersion; }

        /// <summary>
        /// GetCtrlObject - Return control object name
        /// </summary>
        /// <returns>Control object name</returns>
        public string GetCtrlObject() { return CtrlObjName; }

        /// <summary>
        /// GetCtrlStyle - Return control style
        /// </summary>
        /// <returns>Control style</returns>
        public int GetCtrlStyle() { return CtrlStyle; }

        /// <summary>
        /// GetStateBound - Return state bound
        /// </summary>
        /// <returns>State bound</returns>
        public string GetStateBound() { return StateBound; }

        /// <summary>
        /// GetCaptEvent - Return control event identifier
        /// </summary>
        /// <returns>Control event identifier</returns>
        public int GetCaptEvent() { return CtrlEventId; }

        ////// Implemented methods (Reserved)

        /// <summary>
        /// SetCtrlObject - Set control object name
        /// </summary>
        /// <param name="ObjCtrlName">Control object name</param>
        public void SetCtrlObject(string ObjCtrlName)
        {
            // Set control object name
            CtrlObjName = ObjCtrlName;
        }

        /// <summary>
        /// SetStateBound - Set control state bound
        /// </summary>
        /// <param name="BoundState">Control state bound</param>
        public void SetStateBound(string BoundState)
        {
            // Set control state bound
            if (BoundState == "")
                StateBound = null;
            else
                StateBound = BoundState;
        }

        // ------------------------------------------------
        // ID
        //

        ////// Implemented methods (Reserved)

        /// <summary>
        /// GetIdCount - Return ID value count (string array length)
        /// </summary>
        /// <returns>ID value count</returns>
        public uint GetIdCount()
        {
            // Check valid control identifier
            if ((CtrlId == null) ||
                (CtrlId.Type != mHpCtrlData.ValueType.StrArray) ||
                (CtrlId.Value == null))
            {
                // Invalid/Empty control identifier
                return 0;
            }

            // Return ID value count (string value count)
            return (uint)CtrlId.Value.Length; // Should be >= 1
        }

        /// <summary>
        /// GetStrId - Return indexed ID string value
        /// </summary>
        /// <param name="iArrayIdx">Index of ID string array value</param>
        /// <returns>String ID</returns>
        public string GetStrId(uint iArrayIdx = 0)
        {
            // Check valid control identifier
            if ((CtrlId == null) ||
                (CtrlId.Type != mHpCtrlData.ValueType.StrArray) ||
                (CtrlId.Value == null))
            {
                // Invalid control identifier
                return null;
            }

            // Check valid index of ID string array value
            if (iArrayIdx >= (uint)CtrlId.Value.Length)
            {
                // Invalid index of ID string array
                return null;
            }

            // Return indexed ID string value
            return (string)CtrlId.Value[iArrayIdx];
        }

        /// <summary>
        /// SetStrId - Update ID string value
        /// </summary>
        /// <param name="iArrayIdx">Index of ID string array value</param>
        /// <returns>TRUE: ID string value has been updated successfully; FALSE: Otherwise</returns>
        public bool SetStrId(string strValue, uint iArrayIdx = 0)
        {
            // Check valid control identifier
            if ((CtrlId == null) || (CtrlId.Type != mHpCtrlData.ValueType.StrArray))
            {
                // Invalid control identifier
                return false;
            }

            // Realloc ID string array
            Array.Resize(ref CtrlId.Value, ((int)iArrayIdx) + 1);
            CtrlId.Value[iArrayIdx] = strValue;

            return true;
        }

        // ------------------------------------------------
        // Object
        //

        /// <summary>
        /// IsCtrlObject - Compare name of managed control object type
        /// </summary>
        /// <param name="ObjectName">Control object type name to compare</param>
        /// <returns>TRUE: Same object type name; FALSE: Otherwise</returns>
        public abstract bool IsCtrlObject(string ObjectName);

        /// <summary>
        /// GetCtrlObject - Return name of control object type
        /// </summary>
        /// <param name="iObjIdx">Index of control object type name to manage</param>
        /// <returns>Indexed control object type name</returns>
        public abstract string GetCtrlObject(uint iObjIdx);

        // ------------------------------------------------
        // Name
        //

        /// <summary>
        /// GetCtrlName - Return control name
        /// </summary>
        /// <param name="iNameIdx">Index of control name(s)</param>
        /// <returns>Indexed control name</returns>
        public abstract string GetCtrlName(int iNameIdx = mHpCtrlConst.NO_VALUE);

        // ------------------------------------------------
        // Info
        //

        /// <summary>
        /// GetCtrlVers - Return control version
        /// </summary>
        /// <returns></returns>
        public abstract string GetCtrlVers();

        /// <summary>
        /// GetCtrlDesc - Return control description
        /// </summary>
        /// <returns></returns>
        public abstract string GetCtrlDesc();

        /// <summary>
        /// GetIconList - Return control icon
        /// </summary>
        /// <param name="iIconIdx">Index of control icon</param>
        /// <returns>Indexed control icon</returns>
        public abstract IntPtr GetIconList(uint iIconIdx);

        /// <summary>
        /// GetIconIdx - Return control icon
        /// </summary>
        /// <returns></returns>
        public abstract int GetIconIdx();

        // ------------------------------------------------
        // User Interface (UI)
        //

        /// <summary>
        /// CreateUI - Create and return handle on control User Interface (UI)
        /// </summary>
        /// <param name="hSaveBtn">Handle of config dialog save button</param>
        /// <returns>Handle of UI to set control state</returns>
        public abstract IntPtr CreateUI(Int32 hSaveBtn);

        ////// Implemented methods (Reserved)

        /// <summary>
        /// MoveUI - Position mHelp WPF control state config window
        /// </summary>
        /// <param name="hCfgWindow">Config window handle</param>
        /// <param name="hUI">Control state config Form handle</param>
        /// <param name="xPos">Horizontal position</param>
        /// <param name="yPos">Vertical position</param>
        /// <returns>TRUE: Control User Interface has been moved successfully; FALSE: Otherwise</returns>
        public bool MoveUI(Int32 hUI, int xPos, int yPos) { return true; }

        //////

        /// <summary>
        /// SaveUI - Apply state config defined in control User Interface
        /// </summary>
        public abstract void SaveUI();

        /// <summary>
        /// CloseUI - Close control state config User Interface
        /// </summary>
        public abstract void CloseUI();

        // ------------------------------------------------
        // Style
        //

        /// <summary>
        /// SetStyle - Assign control style (provided from mHelp file)
        /// </summary>
        /// <param name="iStyle">Control style</param>
        /// <returns>TRUE: Valid control style; FALSE: Otherwise</returns>
        public abstract bool SetStyle(int iStyle);

        // ------------------------------------------------
        // State
        //

        /// <summary>
        /// GetStateCount - Return state count
        /// </summary>
        /// <returns>State count</returns>
        public abstract uint GetStateCount();

        /// <summary>
        /// GetStateCount - Return state value count for array value type
        /// </summary>
        /// <param name="iStateIdx">State index</param>
        /// <returns>State array value count</returns>
        public abstract uint GetStateCount(uint iStateIdx);

        /// <summary>
        /// GetStateType - Return indexed state value type
        /// </summary>
        /// <param name="iStateIdx">State index</param>
        /// <returns>State value type</returns>
        public abstract int GetStateType(uint iStateIdx);

        /// <summary>
        /// GetStateName - Return indexed state name
        /// </summary>
        /// <param name="iStateIdx">State index</param>
        /// <returns>State name</returns>
        public abstract string GetStateName(uint iStateIdx);

        /// <summary>
        /// GetIntState - Return indexed integer value state
        /// </summary>
        /// <param name="iStateIdx">Index of integer state</param>
        /// <param name="iArrayIdx">Index of integer array state</param>
        /// <returns>Integer state</returns>
        public abstract int GetIntState(uint iStateIdx, uint iArrayIdx = 0);

        /// <summary>
        /// GetStrState - Return indexed string value state
        /// </summary>
        /// <param name="iStateIdx">Index of string state</param>
        /// <param name="iArrayIdx">Index of string array state</param>
        /// <returns>String state</returns>
        public abstract string GetStrState(uint iStateIdx, uint iArrayIdx = 0);

        ////// Implemented methods (Reserved)

        /// <summary>
        /// SetIntState - Update indexed integer value state
        /// </summary>
        /// <param name="iStateIdx">Index of integer state</param>
        /// <param name="iArrayIdx">Index of integer array state</param>
        /// <returns></returns>
        public bool SetIntState(uint iStateIdx, int iValue, uint iArrayIdx = 0)
        {
            // Check defined state index
            if ((CtrlState != null) && (CtrlState[iStateIdx] != null))
            {
                switch (CtrlState[iStateIdx].Type)
                {
                    case mHpCtrlData.ValueType.Integer:
                    {
                        // Check defined state array index and size (== 1)
                        if ((CtrlState[iStateIdx].Value != null) &&
                            (CtrlState[iStateIdx].Value.Length == 1))
                        {
                            CtrlState[iStateIdx].Value[0] = iValue;
                            return true;
                        }
                        break;
                    }
                    case mHpCtrlData.ValueType.IntArray:
                    {
                        // Check defined state array index
                        if (CtrlState[iStateIdx].Value != null)
                        {
                            // Check to realloc integer array
                            Array.Resize(ref CtrlState[iStateIdx].Value, ((int)iArrayIdx) + 1);
                            CtrlState[iStateIdx].Value[iArrayIdx] = iValue;
                            return true;
                        }
                        break;
                    }
                }
            }

            // Wrong 'CtrlState' definition
            return false;
        }

        /// <summary>
        /// SetStrState - Update indexed string value state
        /// </summary>
        /// <param name="iStateIdx">Index of string state</param>
        /// <param name="iArrayIdx">Index of string array state</param>
        /// <returns></returns>
        public bool SetStrState(uint iStateIdx, string strValue, uint iArrayIdx = 0)
        {
            // Check defined state index
            if ((CtrlState != null) && (CtrlState[iStateIdx] != null))
            {
                switch (CtrlState[iStateIdx].Type)
                {
                    case mHpCtrlData.ValueType.String:
                    {
                        // Check defined state array index and size (== 1)
                        if ((CtrlState[iStateIdx].Value != null) &&
                            (CtrlState[iStateIdx].Value.Length == 1))
                        {
                            CtrlState[iStateIdx].Value[0] = strValue;
                            return true;
                        }
                        break;
                    }
                    case mHpCtrlData.ValueType.IntArray:
                    {
                        // Check defined state array index
                        if (CtrlState[iStateIdx].Value != null)
                        {
                            // Check to realloc string array
                            Array.Resize(ref CtrlState[iStateIdx].Value, ((int)iArrayIdx) + 1);
                            CtrlState[iStateIdx].Value[iArrayIdx] = strValue;
                            return true;
                        }
                        break;
                    }
                }
            }

            // Wrong 'CtrlState' definition
            return false;
        }

        //////

        /// <summary>
        /// NeedToCheckState - Define if state control can always be set
        /// </summary>
        /// <returns>TRUE: State control can not be set; FALSE: Can systematically be set</returns>
        public abstract bool NeedToCheckState();

        /// <summary>
        /// DisplayState - Return state control under string shape
        /// </summary>
        /// <returns>String representing control state</returns>
        public abstract string DisplayState();

        // ------------------------------------------------
        // Config
        //

        /// <summary>
        /// GetCfgCount - Return config count
        /// </summary>
        /// <returns>Config count</returns>
        public abstract uint GetCfgCount();

        /// <summary>
        /// GetCfgCount - Return config value count for array value type
        /// </summary>
        /// <param name="iCfgIdx">Config index</param>
        /// <returns>Config array value count</returns>
        public abstract uint GetCfgCount(uint iCfgIdx);

        /// <summary>
        /// GetCfgType - Return indexed config value type
        /// </summary>
        /// <param name="iCfgIdx">Config index</param>
        /// <returns>Config value type</returns>
        public abstract int GetCfgType(uint iCfgIdx);

        /// <summary>
        /// GetCfgName - Return indexed config name
        /// </summary>
        /// <param name="iCfgIdx">Config index</param>
        /// <returns>Config name</returns>
        public abstract string GetCfgName(uint iCfgIdx);

        /// <summary>
        /// GetIntCfg - Return indexed integer value config
        /// </summary>
        /// <param name="iCfgIdx">Index of integer config</param>
        /// <param name="iArrayIdx">Index of integer array config</param>
        /// <returns>Integer config</returns>
        public abstract int GetIntCfg(uint iCfgIdx, uint iArrayIdx = 0);

        /// <summary>
        /// GetStrCfg - Return indexed string value config
        /// </summary>
        /// <param name="iCfgIdx">Index of string config</param>
        /// <param name="iArrayIdx">Index of string array config</param>
        /// <returns>String config</returns>
        public abstract string GetStrCfg(uint iCfgIdx, uint iArrayIdx = 0);

        ////// Implemented methods (Reserved)

        /// <summary>
        /// SetIntCfg - Update indexed integer value config
        /// </summary>
        /// <param name="iCfgIdx">Index of integer config</param>
        /// <param name="iArrayIdx">Index of integer array config</param>
        /// <returns></returns>
        public bool SetIntCfg(uint iCfgIdx, int iValue, uint iArrayIdx = 0)
        {
            // Check defined config index
            if ((CtrlCfg != null) && (CtrlCfg[iCfgIdx] != null))
            {
                switch (CtrlCfg[iCfgIdx].Type)
                {
                    case mHpCtrlData.ValueType.Integer:
                    {
                        // Check defined config array index and size (== 1)
                        if ((CtrlCfg[iCfgIdx].Value != null) &&
                            (CtrlCfg[iCfgIdx].Value.Length == 1))
                        {
                            CtrlCfg[iCfgIdx].Value[0] = iValue;
                            return true;
                        }
                        break;
                    }
                    case mHpCtrlData.ValueType.IntArray:
                    {
                        // Check defined config array index
                        if (CtrlCfg[iCfgIdx].Value != null)
                        {
                            // Check to realloc integer array
                            Array.Resize(ref CtrlCfg[iCfgIdx].Value, ((int)iArrayIdx) + 1);
                            CtrlCfg[iCfgIdx].Value[iArrayIdx] = iValue;
                            return true;
                        }
                        break;
                    }
                }
            }

            // Wrong 'CtrlCfg' definition
            return false;
        }

        /// <summary>
        /// SetStrCfg - Update indexed string value config
        /// </summary>
        /// <param name="iCfgIdx">Index of string config</param>
        /// <param name="iArrayIdx">Index of string array config</param>
        /// <returns></returns>
        public bool SetStrCfg(uint iCfgIdx, string strValue, uint iArrayIdx = 0)
        {
            // Check defined config index
            if ((CtrlCfg != null) && (CtrlCfg[iCfgIdx] != null))
            {
                switch (CtrlCfg[iCfgIdx].Type)
                {
                    case mHpCtrlData.ValueType.String:
                    {
                        // Check defined config array index and size (== 1)
                        if ((CtrlCfg[iCfgIdx].Value != null) &&
                            (CtrlCfg[iCfgIdx].Value.Length == 1))
                        {
                            CtrlCfg[iCfgIdx].Value[0] = strValue;
                            return true;
                        }
                        break;
                    }
                    case mHpCtrlData.ValueType.StrArray:
                    {
                        // Check defined config array index
                        if (CtrlCfg[iCfgIdx].Value != null)
                        {
                            // Check to realloc integer array
                            Array.Resize(ref CtrlCfg[iCfgIdx].Value, ((int)iArrayIdx) + 1);
                            CtrlCfg[iCfgIdx].Value[iArrayIdx] = strValue;
                            return true;
                        }
                        break;
                    }
                }
            }

            // Wrong 'CtrlCfg' definition
            return false;
        }

        // ------------------------------------------------
        // HTML tags
        //

        /// <summary>
        /// GetHtmlTags - Return HTML tags associated with the control state configurable (if any)
        /// </summary>
        /// <param name="iTaskIdx">Index of the task</param>
        /// <returns>HTML tags or null if no control state</returns>
        public abstract string GetHtmlTags(uint iTaskIdx);

        // ------------------------------------------------
        // Capture processus
        //

        /// <summary>
        /// AddmHpEvent - Add mHelp event handler on control
        /// </summary>
        /// <param name="Ctrl">Control to add event handler</param>
        /// <param name="RegCtrlEvent">Registered control event (to identify which event)</param>
        protected abstract bool AddmHpEvent(Visual Ctrl, string RegCtrlEvent);

        ////// Implemented methods (Reserved)

        /// <summary>
        /// SetCaptEvent - Set control event identifier (provided from mHelp file)
        /// </summary>
        /// <param name="iEventId">Control event identifier</param>
        /// <returns>TRUE: Valid control event identifier; FALSE: Otherwise</returns>
        public bool SetCaptEvent(int iEventId)
        {
            CtrlEventId = mHpCtrlConst.NO_VALUE;

            // Check valid control event(s)
            if (CtrlEvent == null)
                return false; // Invalid control event(s) definition

            // Check valid event identifier (index of control event array)
            if ((iEventId < 0) || (iEventId >= CtrlEvent.Length))
                return false; // Invalid event identifier

            // Assign event identifier
            CtrlEventId = iEventId;

            return true;
        }

        /// <summary>
        /// SetControl - Set standard WPF control identifier (set 'CtrlId')
        /// </summary>
        /// <param name="Ctrl">Control object that emitted an event</param>
        protected void SetControl(Control Ctrl)
        {
            // Set standard WPF control identifier (using 'Control.Name')
            CtrlId.Value = null;
            CtrlId.Value = new Object[1];
            CtrlId.Value[0] = Ctrl.Name;

            /*
            Visual ParentCtrl = Ctrl as Visual;
            while (VisualTreeHelper.GetParent(ParentCtrl) != null)
            {
                ParentCtrl = (Visual)VisualTreeHelper.GetParent(ParentCtrl);
                if ((VisualTreeHelper.GetParent(ParentCtrl) != null) && // Avoid to add Window name
                    (ParentCtrl is Control)) // Parent object should be a Control
                {
                    Control ParentName = ParentCtrl as Control;

                    Array.Resize(ref CtrlId.Value, CtrlId.Value.Length + 1);
                    CtrlId.Value[CtrlId.Value.Length - 1] = ParentName.Name;
                }
            }
            Array.Reverse(CtrlId.Value);
            */
        }

        //private bool AddCtrlEvent(Control ParentCtrl, string RegEvent)
        private bool AddCtrlEvent(Visual ParentCtrl, string RegEvent)
        {
            // Check if control match with expected WPF control object
            if (ParentCtrl.GetType() == typeof(WPFCtrl))
            {
                ////// Add mHelp event handler
                if (!AddmHpEvent(ParentCtrl, RegEvent))
                    return false;
            }

            int nChildCount = VisualTreeHelper.GetChildrenCount(ParentCtrl);

            // Loop on all control defined in the window
            for (int i = 0; i <= nChildCount - 1; i++)
            {
                Visual ChildCtrl = (Visual)VisualTreeHelper.GetChild(ParentCtrl, i);

                // Check if control match with expected WPF control object
                if (ChildCtrl.GetType() == typeof(WPFCtrl))
                {
                    ////// Add mHelp event handler
                    if (!AddmHpEvent(ChildCtrl, RegEvent))
                        return false;
                }

                // Check if child control has children
                if (VisualTreeHelper.GetChildrenCount(ChildCtrl) > 0)
                    AddCtrlEvent(ChildCtrl, RegEvent); // Recursive
            }

            // No error
            return true;
        }

        /// <summary>
        /// AddmHpEvents - Add to control event(s) to be reproduced by mHelp
        /// </summary>
        /// <param name="hWindow">Window handle (containing control)</param>
        /// <returns>TRUE: Event(s) added successfully; FALSE: Otherwise</returns>
        public bool AddmHpEvents(Int32 hWindow, Int32 hControl)
        {
            // Get window from handle
            HwndSource ParentCtrl = HwndSource.FromHwnd((IntPtr)hWindow);
            if (ParentCtrl == null)
                return true; // Not a WPF window

            // Check valid control event(s)
            if (CtrlEvent == null)
                return false; // Invalid control event(s) definition

            // Loop on all event to add
            foreach (string CurRegEvent in CtrlEvent)
            {
                // Register current event
                if (!RegistermHpEvent(CurRegEvent))
                    return false; // Failed to register event

                // Add mHelp event handler on control to be able to catch it
                if (!AddCtrlEvent(ParentCtrl.RootVisual, CurRegEvent))
                    return false; // Error: Control found but failed to add mHelp event handler

                // Next event to add
            }

            // - Event(s) on control added
            // - Not an expected WPF control object (not managed with this DLL)
            // - Control not found (control handle did not match with any WPF control)
            return true;
        }

        /// <summary>
        /// RegistermHpEvent - Register a new window message to inform mHelp a WPF control event has occurred
        /// </summary>
        /// <param name="mHpCtrlEvent">Message to be registered</param>
        /// <returns>TRUE: New window message registered successfully; FALSE: Otherwise</returns>
        private bool RegistermHpEvent(string CtrlRegEvent)
        {
            // Check event already registered
            if (EventRegTab.Contains(CtrlRegEvent))
                return true; // Already registered

            // Register new window message for current event
            uint iEventMsgReg = mHpCtrlConst.RegisterWindowMessage(CtrlRegEvent);
            if (iEventMsgReg == 0)
                return false;

            // Store new window message identifier (using 'CtrlRegEvent' as index key)
            EventRegTab[CtrlRegEvent] = iEventMsgReg;

            return true;
        }

        /// <summary>
        /// SendmHpEvent - Send window message associated with registered control event in order to inform
        ///                mHelp a managed WPF control event has occurred
        /// </summary>
        /// <param name="CtrlSender">Control object on which an event occurred</param>
        /// <param name="RegCtrlEvent">Registered control event</param>
        /// <returns>TRUE: mHelp has been informed; FALSE: Otherwise</returns>
        protected bool SendmHpEvent(Object CtrlSender, string RegCtrlEvent)
        {
            // Check valid control event(s)
            if (CtrlEvent == null)
                return false; // Invalid control event(s) definition

            // Set control event identifier
            CtrlEventId = Array.FindIndex<string>(CtrlEvent, s => s.Equals(RegCtrlEvent));
            if (CtrlEventId == mHpCtrlConst.NO_VALUE)
                return false; // Failed to set control event identifier

            // Get window message registered for this event
            if (!EventRegTab.Contains(RegCtrlEvent))
                return false; // Unexpected registered event

            uint imHpWPFMsg = (uint)EventRegTab[RegCtrlEvent];

            // Get parent window
            System.Windows.Window ParentWnd = System.Windows.Window.GetWindow((Visual)CtrlSender);
            if (ParentWnd == null)
                return false;

            // Send message to inform mHelp a WPF control event has occured
            WindowInteropHelper ParentHandle = new WindowInteropHelper(ParentWnd);
            mHpCtrlConst.SendMessage(ParentHandle.Handle, imHpWPFMsg, IntPtr.Zero, IntPtr.Zero);

            return true;
        }

        /// <summary>
        /// CaptureEvent - Check if register window message is associated with current control
        /// </summary>
        /// <param name="iRegEvent">Registered window message</param>
        /// <returns>TRUE: Current managed control has emitted this event; FALSE: Otherwise</returns>
        public bool CaptureEvent(uint imHpWPFMsg)
        {
            // Check if message match with a registered event (window message)
            return EventRegTab.ContainsValue(imHpWPFMsg);
        }

        // --------------------------------
        // Reproduce processus
        //

        /*
        private Control GetChildControl(Visual ParentCtrl, ref uint iIdIdx)
        {
            // Check valid standard control identifier
            if ((CtrlId.Value[iIdIdx] == null) ||
                (CtrlId.Value[iIdIdx].GetType() != typeof(string)))
                return null; // Wrong standard control identifier

            // Check if parent control is a Control object
            if (ParentCtrl is Control)
            {
                // Check if control name match with current control identifier
                Control ChildCtrl = ParentCtrl as Control;
                if (ChildCtrl.Name == (string)CtrlId.Value[iIdIdx])
                {
                    iIdIdx++; // Next child control ID

                    // Check not a parent control but control
                    if (CtrlId.Value.Length == iIdIdx)
                    {
                        // Check if control match with expected WPF control object type
                        if (ChildCtrl.GetType() != typeof(WPFCtrl))
                            return null; // Not the expected control object type found

                        // Return control found
                        return ChildCtrl;
                    }

                    // Check parent control contains child control
                    if (VisualTreeHelper.GetChildrenCount(ParentCtrl) == 0)
                        return null; // Failed to find control
                }
            }

            int nChildCount = VisualTreeHelper.GetChildrenCount(ParentCtrl);

            // Loop on all control defined in the parent control
            for (int i = 0; i <= nChildCount - 1; i++)
            {
                // Check if child control is a Control object
                Visual ChildVisual = (Visual)VisualTreeHelper.GetChild(ParentCtrl, i);
                if (ChildVisual is Control)
                {
                    // Check if control name match with current control identifier
                    Control ChildCtrl = ChildVisual as Control;
                    if (ChildCtrl.Name == (string)CtrlId.Value[iIdIdx])
                    {
                        iIdIdx++; // Next child control ID

                        // Check not a parent control but control
                        if (CtrlId.Value.Length == iIdIdx)
                        {
                            // Check if control match with expected WPF control object type
                            if (ChildCtrl.GetType() != typeof(WPFCtrl))
                                return null; // Not the expected control object type found

                            // Return control found
                            return ChildCtrl;
                        }

                        // Check parent control contains child control
                        if (VisualTreeHelper.GetChildrenCount(ParentCtrl) == 0)
                            return null; // Failed to find control
                    }
                }
            }

            // Failed to find control
            return null;
        }
        */

        ////// Implemented methods (Reserved)

        /// <summary>
        /// GetControl - Get standard WPF control object using parent window handle and standard control
        ///              identifier stored in 'CtrlId' (set 'CtrlObj')
        /// </summary>
        /// <param name="hWindow">Parent window handle</param>
        /// <returns>TRUE: 'CtrlObj' control object set successfully; FALSE: Otherwise</returns>
        protected bool GetControl(IntPtr hWindow)
        {
            // Get Form from handle
            HwndSource ParentCtrl = HwndSource.FromHwnd((IntPtr)hWindow);
            if (ParentCtrl == null)
                return false; // Not a WPF window

            // Check valid parent object and control identifier
            if ((ParentCtrl != null) &&
                (CtrlId != null) &&
                (CtrlId.Type == mHpCtrlData.ValueType.StrArray) &&
                (CtrlId.Value != null) &&
                (CtrlId.Value[0] != null) &&
                (CtrlId.Value[0].GetType() == typeof(string)))
            {
                CtrlObj = null;

                // Set control object (or null if failed)
                Control WindowCtrl = ParentCtrl.RootVisual as Control;
                Control ChildCtrl = (Control)WindowCtrl.FindName((string)CtrlId.Value[0]);
                if (ChildCtrl != null)
                {
                    // Check if control match with expected WPF control object type
                    if (ChildCtrl.GetType() != typeof(WPFCtrl))
                        return false; // Not the expected control object type

                    CtrlObj = (Object)ChildCtrl;
                    return true; // Control object found
                }

                /*
                uint iIdIdx = 0;
                CtrlObj = (Object)GetChildControl(ParentCtrl.RootVisual, ref iIdIdx);
                if (CtrlObj != null)
                    return true; // Control object found
                */
            }

            // Failed to find control object
            return false;
        }

        /// <summary>
        /// GetCtrlEvent - Return control event to be reproduced
        /// </summary>
        /// <returns>Registered control event</returns>
        protected string GetCtrlEvent()
        {
            // Return control event to be reproduced (according 'CtrlEvent' definition)

            return CtrlEvent[CtrlEventId];  // -> This method should not be called if 'SetCaptEvent' failed
            //    but if succeed 'CtrlEventId' should be valid
        }

        //////

        /// <summary>
        /// GetCtrlLeft - Return the left coordinate of the control in the screen
        /// GetCtrlTop - Return top coordinate of the control in the screen
        /// GetCtrlWidth - Return control width
        /// GetCtrlHeight - Return control height
        /// </summary>
        /// <returns>Coordinate of the control</returns>
        public abstract int GetCtrlLeft();
        public abstract int GetCtrlTop();
        public abstract int GetCtrlWidth();
        public abstract int GetCtrlHeight();

        /// <summary>
        /// CheckState - Check if state can be set
        /// </summary>
        /// <returns>TRUE: State control can be set; FALSE: Otherwise</returns>
        public abstract bool CheckState();

        /// <summary>
        /// NextAction - Return next action type for reproduce processus
        /// </summary>
        /// <returns>Next action type</returns>
        public abstract int NextAction();

        /// <summary>
        /// Start - Launched at start of reproduce processus
        /// </summary>
        /// <param name="hWindow">Parent window handle</param>
        /// <returns>TRUE: Ready to continue processus; FALSE: Otherwise</returns>
        public abstract bool Start(Int32 hWindow);

        /// <summary>
        /// End - Launched at end of reproduce processus
        /// </summary>
        /// <returns>TRUE: Ended processus successfully; FALSE: Otherwise</returns>
        public abstract bool End();

        /// <summary>
        /// MoveToX/MoveToY - Move mHelp cursor to horizontal/vertical screen position
        /// </summary>
        /// <returns>Horizontal/Vertical screen position to move to (NO_VALUE if error)</returns>
        public abstract int MoveToX();
        public abstract int MoveToY();

        /// <summary>
        /// MoveAct - Define behaviour when mHelp cursor is in position (after 'ActMove' action)
        /// </summary>
        /// <returns>TRUE: Ready to continue; FALSE: Otherwise</returns>
        public abstract bool MoveAct();

        /// <summary>
        /// WaitUntil - Define behaviour to mark a delay (after 'ActWait' action)
        /// </summary>
        /// <returns>TRUE: Ready to continue; FALSE: Otherwise</returns>
        public abstract bool WaitUntil();

        /// <summary>
        /// LeftPressAct - Define behaviour after a mHelp cursor left button press (after 'ActLeftPress' action)
        /// </summary>
        /// <returns>TRUE: Ready to continue; FALSE: Otherwise</returns>
        public abstract bool LeftPressAct();

        /// <summary>
        /// LeftUnpressAct - Define behaviour after a mHelp cursor left button unpress (after 'ActLeftUnpress' action)
        /// </summary>
        /// <returns>TRUE: Ready to continue; FALSE: Otherwise</returns>
        public abstract bool LeftUnpressAct();

        /// <summary>
        /// RightPressAct - Define behaviour after a mHelp cursor right button press (after 'ActRightPress' action)
        /// </summary>
        /// <returns>TRUE: Ready to continue; FALSE: Otherwise</returns>
        public abstract bool RightPressAct();

        /// <summary>
        /// RightUnpressAct - Define behaviour after a mHelp cursor right button unpress (after 'ActRightUnpress' action)
        /// </summary>
        /// <returns>TRUE: Ready to continue; FALSE: Otherwise</returns>
        public abstract bool RightUnpressAct();

        /// <summary>
        /// EditAct - Define behaviour to edit a control (after 'ActEdit' action)
        /// </summary>
        /// <returns>TRUE: Ready to continue; FALSE: Otherwise</returns>
        public abstract bool EditAct();

        ///////////////////////////////////////////////////

        /// <summary>
        /// CmHpWPFCtrl - Constructor
        /// </summary>
        public CmHpWPFCtrl()
        {
            // Initialization
            CtrlStyle = mHpCtrlConst.NO_VALUE;

            CtrlEventId = mHpCtrlConst.NO_VALUE;
            CtrlEvent = null;

            CtrlId = new mHpCtrlData("CtrlID", mHpCtrlData.ValueType.StrArray);
            CtrlId.Value = null;

            CtrlCfg = null;
            CtrlState = null;

            HtmlTags = null;
            CtrlObj = null;
            ActStep = 0;

            //////
            EventRegTab = new Hashtable();

            LastErrMsg = null;
            DllVersion = null;
            CtrlObjName = null;
            StateBound = null;
        }
    }
}

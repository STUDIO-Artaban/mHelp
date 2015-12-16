using System.Runtime.InteropServices;

namespace mHelp
{
    static class CmHelp
    {
        //-------------------------------------------------------------------------//
        // Main
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpInit", CharSet = CharSet.Ansi)]
        public static extern int MainActive(System.IntPtr hWnd, string ExeName, bool SharedApp);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpOpen", CharSet = CharSet.Ansi)]
        public static extern void Create(System.IntPtr hWnd);

        //-------------------------------------------------------------------------//
        // Extended
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpErrorMsg", CharSet = CharSet.Ansi)]
        public static extern int ErrorMsg(int ErrorID, ref string ReceiveBuffer, int MaxBuffer);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpLaunchFavTask", CharSet = CharSet.Ansi)]
        public static extern int LaunchFavTask(System.IntPtr hWnd, string DynFilePath, int FavIndex);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpCreateCustomTask", CharSet = CharSet.Ansi)]
        public static extern void CreateCustomTask(System.IntPtr hWnd, string DynFilePath, int FavIndex, ref int CustomID, ref int DynError);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpLnchCustomTask", CharSet = CharSet.Ansi)]
        public static extern int LnchCustomTask(System.IntPtr hWnd, int CustomID);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpGetParamCnt", CharSet = CharSet.Ansi)]
        public static extern void GetParamCnt(int CustomID, ref int CntCustom, ref int DynError);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpGetParamType", CharSet = CharSet.Ansi)]
        public static extern void GetParamType(int CustomID, int CustomIndex, ref int TypeCustom, ref int DynError);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpGetRepeatOpt", CharSet = CharSet.Ansi)]
        public static extern void GetRepeatOpt(int CustomID, ref int RepeatOpt, ref int DynError);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpSetRepeatOpt", CharSet = CharSet.Ansi)]
        public static extern int SetRepeatOpt(int CustomID, int NewRepeatOpt);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpGetIntParamVal", CharSet = CharSet.Ansi)]
        public static extern void GetIntParamVal(int CustomID, int CustomIndex, int RepeatIndex, int SelIndex, ref int IntVal, ref int DynError);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpSetIntParamVal", CharSet = CharSet.Ansi)]
        public static extern int SetIntParamVal(int CustomID, int CustomIndex, int RepeatIndex, int NewVal);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpAddIntParamVal", CharSet = CharSet.Ansi)]
        public static extern int AddIntParamVal(int CustomID, int CustomIndex, int RepeatIndex, int AddVal);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpRemoveIntParamVal", CharSet = CharSet.Ansi)]
        public static extern int RemoveIntParamVal(int CustomID, int CustomIndex, int RepeatIndex, int SelIndex);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpGetStrParamVal", CharSet = CharSet.Ansi)]
        public static extern int GetStrParamVal(int CustomID, int CustomIndex, int RepeatIndex, int SelIndex, ref string StrVal, int MaxStrSize);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpSetStrParamVal", CharSet = CharSet.Ansi)]
        public static extern int SetStrParamVal(int CustomID, int CustomIndex, int RepeatIndex, string StrVal);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpAddStrParamVal", CharSet = CharSet.Ansi)]
        public static extern int AddStrParamVal(int CustomID, int CustomIndex, int RepeatIndex, string AddVal, int MaxAddSize);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpRemoveStrParamVal", CharSet = CharSet.Ansi)]
        public static extern int RemoveStrParamVal(int CustomID, int CustomIndex, int RepeatIndex, int SelIndex);
        [DllImport("mHelp.dll", CallingConvention = CallingConvention.StdCall, EntryPoint = "mHelpDestroyCustomTask", CharSet = CharSet.Ansi)]
        public static extern int DestroyCustomTask(int CustomID);

    }
}

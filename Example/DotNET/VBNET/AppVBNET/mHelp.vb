Option Explicit On

Module mHelp

    Public Class CmHelp

        ' ---------------------------------------------------------------------
        ' Main
        Public Declare Sub MainActive Lib "mHelp" Alias "mHelpInit" (ByVal hWnd As System.IntPtr, ByVal ExeName As String, ByVal SharedApp As Boolean)
        Public Declare Sub Create Lib "mHelp" Alias "mHelpOpen" (ByVal hWnd As System.IntPtr)

        ' ---------------------------------------------------------------------
        ' Extended
        Public Declare Function ErrorMsg Lib "mHelp" Alias "mHelpErrorMsg" (ByVal ErrorID As Integer, ByRef ReceiveBuffer As String, ByVal MaxBuffer As Integer) As Integer
        Public Declare Function LaunchFavTask Lib "mHelp" Alias "mHelpLaunchFavTask" (ByVal hWnd As System.IntPtr, ByVal DynFilePath As String, ByVal FavIndex As Integer) As Integer
        Public Declare Sub CreateCustomTask Lib "mHelp" Alias "mHelpCreateCustomTask" (ByVal hWnd As System.IntPtr, ByVal DynFilePath As String, ByVal FavIndex As Integer, ByRef CustomID As Integer, ByRef DynError As Integer)
        Public Declare Function LnchCustomTask Lib "mHelp" Alias "mHelpLnchCustomTask" (ByVal hWnd As System.IntPtr, ByVal CustomID As Integer) As Integer
        Public Declare Sub GetParamCnt Lib "mHelp" Alias "mHelpGetParamCnt" (ByVal CustomID As Integer, ByRef CntCustom As Integer, ByRef DynError As Integer)
        Public Declare Sub GetParamType Lib "mHelp" Alias "mHelpGetParamType" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByRef TypeCustom As Integer, ByRef DynError As Integer)
        Public Declare Sub GetRepeatOpt Lib "mHelp" Alias "mHelpGetRepeatOpt" (ByVal CustomID As Integer, ByRef RepeatOpt As Integer, ByRef DynError As Integer)
        Public Declare Function SetRepeatOpt Lib "mHelp" Alias "mHelpSetRepeatOpt" (ByVal CustomID As Integer, ByVal NewRepeatOpt As Integer) As Integer
        Public Declare Sub GetIntParamVal Lib "mHelp" Alias "mHelpGetIntParamVal" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer, ByRef IntVal As Integer, ByRef DynError As Integer)
        Public Declare Function SetIntParamVal Lib "mHelp" Alias "mHelpSetIntParamVal" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal NewVal As Integer) As Integer
        Public Declare Function AddIntParamVal Lib "mHelp" Alias "mHelpAddIntParamVal" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal AddVal As Integer) As Integer
        Public Declare Function RemoveIntParamVal Lib "mHelp" Alias "mHelpRemoveIntParamVal" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer) As Integer
        Public Declare Function GetStrParamVal Lib "mHelp" Alias "mHelpGetStrParamVal" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer, ByRef StrVal As String, ByVal MaxStrSize As Integer) As Integer
        Public Declare Function SetStrParamVal Lib "mHelp" Alias "mHelpSetStrParamVal" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal StrVal As String) As Integer
        Public Declare Function AddStrParamVal Lib "mHelp" Alias "mHelpAddStrParamVal" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal AddVal As String, ByVal MaxAddSize As Integer) As Integer
        Public Declare Function RemoveStrParamVal Lib "mHelp" Alias "mHelpRemoveStrParamVal" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer) As Integer
        Public Declare Function DestroyCustomTask Lib "mHelp" Alias "mHelpDestroyCustomTask" (ByVal CustomID As Integer) As Integer

    End Class

End Module

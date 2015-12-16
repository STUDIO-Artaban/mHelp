Attribute VB_Name = "mHelp"
Option Explicit

' -------------------------------------------------------------------------------------------------
' Main
Public Declare Sub mHelpInit Lib "mHelp" (ByVal hWnd As Long, ByVal ExeName As String, ByVal SharedApp As Boolean)
Public Declare Sub mHelpOpen Lib "mHelp" (ByVal hWnd As Long)

' -------------------------------------------------------------------------------------------------
' Extended
Public Declare Function mHelpErrorMsg Lib "mHelp" (ByVal ErrorID As Integer, ByRef ReceiveBuffer As String, ByVal MaxBuffer As Integer) As Integer
Public Declare Function mHelpLaunchFavTask Lib "mHelp" (ByVal hWnd As Long, ByVal DynFilePath As String, ByVal FavIndex As Integer) As Integer
Public Declare Sub mHelpCreateCustomTask Lib "mHelp" (ByVal hWnd As Long, ByVal DynFilePath As String, ByVal FavIndex As Integer, ByRef CustomID As Integer, ByRef DynError As Integer)
Public Declare Function mHelpLnchCustomTask Lib "mHelp" (ByVal hWnd As Long, ByVal CustomID As Integer) As Integer
Public Declare Sub mHelpGetParamCnt Lib "mHelp" (ByVal CustomID As Integer, ByRef CntCustom As Integer, ByRef DynError As Integer)
Public Declare Sub mHelpGetParamType Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByRef TypeCustom As Integer, ByRef DynError As Integer)
Public Declare Sub mHelpGetRepeatOpt Lib "mHelp" (ByVal CustomID As Integer, ByRef RepeatOpt As Integer, ByRef DynError As Integer)
Public Declare Function mHelpSetRepeatOpt Lib "mHelp" (ByVal CustomID As Integer, ByVal NewRepeatOpt As Integer) As Integer
Public Declare Sub mHelpGetIntParamVal Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer, ByRef IntVal As Integer, ByRef DynError As Integer)
Public Declare Function mHelpSetIntParamVal Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal NewVal As Integer) As Integer
Public Declare Function mHelpAddIntParamVal Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal AddVal As Integer) As Integer
Public Declare Function mHelpRemoveIntParamVal Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer) As Integer
Public Declare Function mHelpGetStrParamVal Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer, ByRef StrVal As String, ByVal MaxStrSize As Integer) As Integer
Public Declare Function mHelpSetStrParamVal Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal StrVal As String) As Integer
Public Declare Function mHelpAddStrParamVal Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal AddVal As String, ByVal MaxAddSize As Integer) As Integer
Public Declare Function mHelpRemoveStrParamVal Lib "mHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer) As Integer
Public Declare Function mHelpDestroyCustomTask Lib "mHelp" (ByVal CustomID As Integer) As Integer

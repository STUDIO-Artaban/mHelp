Attribute VB_Name = "DynHelp"
Option Explicit

'----------------------------------------------------------------------------------------------------------'
Public Declare Sub DynHelp_MainActive Lib "DynHelp" (ByVal hwnd As Long, ByVal ExeName As String, ByVal SharedApp As Boolean)
Public Declare Sub DynHelp_Create Lib "DynHelp" (ByVal hwnd As Long)
Public Declare Function DynHelp_AddUnSupport Lib "DynHelp" (ByVal ClassName As String) As Integer
'----------------------------------------------------------------------------------------------------------'
Public Declare Function DynHelp_ErrorMsg Lib "DynHelp" (ByVal ErrorID As Integer, ByRef ReceiveBuffer As String, ByVal MaxBuffer As Integer) As Integer
Public Declare Function DynHelp_LaunchFavTask Lib "DynHelp" (ByVal hwnd As Long, ByVal DynFilePath As String, ByVal FavIndex As Integer) As Integer
Public Declare Sub DynHelp_CreateCustomTask Lib "DynHelp" (ByVal hwnd As Long, ByVal DynFilePath As String, ByVal FavIndex As Integer, ByRef CustomID As Integer, ByRef DynError As Integer)
Public Declare Function DynHelp_LnchCustomTask Lib "DynHelp" (ByVal hwnd As Long, ByVal CustomID As Integer) As Integer
Public Declare Sub DynHelp_GetParamCnt Lib "DynHelp" (ByVal CustomID As Integer, ByRef CntCustom As Integer, ByRef DynError As Integer)
Public Declare Sub DynHelp_GetParamType Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByRef TypeCustom As Integer, ByRef DynError As Integer)
Public Declare Sub DynHelp_GetRepeatOpt Lib "DynHelp" (ByVal CustomID As Integer, ByRef RepeatOpt As Integer, ByRef DynError As Integer)
Public Declare Function DynHelp_SetRepeatOpt Lib "DynHelp" (ByVal CustomID As Integer, ByVal NewRepeatOpt As Integer) As Integer
Public Declare Sub DynHelp_GetIntParamVal Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer, ByRef IntVal As Integer, ByRef DynError As Integer)
Public Declare Function DynHelp_SetIntParamVal Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal NewVal As Integer) As Integer
Public Declare Function DynHelp_AddIntParamVal Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal AddVal As Integer) As Integer
Public Declare Function DynHelp_RemoveIntParamVal Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer) As Integer
Public Declare Function DynHelp_GetStrParamVal Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer, ByRef StrVal As String, ByVal MaxStrSize As Integer) As Integer
Public Declare Function DynHelp_SetStrParamVal Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal StrVal As String) As Integer
Public Declare Function DynHelp_AddStrParamVal Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal AddVal As String, ByVal MaxAddSize As Integer) As Integer
Public Declare Function DynHelp_RemoveStrParamVal Lib "DynHelp" (ByVal CustomID As Integer, ByVal CustomIndex As Integer, ByVal RepeatIndex As Integer, ByVal SelIndex As Integer) As Integer
Public Declare Function DynHelp_DestroyCustomTask Lib "DynHelp" (ByVal CustomID As Integer) As Integer


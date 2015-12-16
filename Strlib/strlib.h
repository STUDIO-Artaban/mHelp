// strlib.h: Declaration of the String Library (SL).
//
//////////////////////////////////////////////////////////////////////

/***
* PURPOSE:
*     This file defines values, macros, and functions for string tools.
*     It also defines the CStrlib class (CString class like without any
*     reference count).
*
* CONTENT:
*     // MACROS
*     isDigit:       Return if it's a digit character
*     isAlpha:       Return if it's an alphabetic character
*     isUpper:       Return if it's an uppercase alphabetic character
*     isLower:       Return if it's an lowercase alphabetic character
*
*     // FUNCTIONS
*     slLength:      Return the length of a string
*     slCopy:        Copy a string
*     slConcat:      Append a string
*     slCompare:     Compare two strings
*     slLTrim:       Remove leading space characters
*     slRTrim:       Remove trailing space characters
*     slTrim:        Remove leading and trailing space characters
*     slUpper:       Convert a string to uppercase
*     slLower:       Convert a string to lowercase
*     slMiddle:      Extract a located substring
*     slLeft:        Extract the left part of a string
*     slRight:       Extract the right part of a string
*     slReverse:     Reverse a string
*     slFind:        Find substring in a string
*     slRevFind:     Find substring starting from the end of a string
*     slRemove:      Delete located substring
*     slInsert:      Insert a substring
*     slToNumber:    Convert a string to number
*     slFormat:      Write formatted data string
*     slToken:       Find the next token in a string
*
*     // CLASS
*     CStrlib:       CString class like that implements all function above
*
*********************************************************************/

#ifndef _STR_LIB
#define _STR_LIB

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
#define  isDigit(c)              (bool)((c > 47) && (c < 58))
#define  isLower(c)              (bool)((c > 96) && (c < 123))
#define  isUpper(c)              (bool)((c > 64) && (c < 91))
#define  isAlpha(c)              (bool)(isUpper(c) || isLower(c))

//////////////////////////////////////////////////////////////////////
#define  NOT_FOUND               (-1)

/***
* FUNCTION:       slLength
* DESCRIPTION:    Return the length of a string
* RETURN:         'Len' string length (unsigned int)
* PARAMETER:
*     Len         I        String to return its length (null terminated string)
* REMARKS:
******************************************************************/
inline unsigned int slLength(const char* Len);

/***
* FUNCTION:       slCopy
* DESCRIPTION:    Copy a string
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        O        Destination string (null terminated string)
*     Source      I        String to copy (null terminated string)
* REMARKS:        Max 'Dest' string length must be greater than 'Source' string size.
******************************************************************/
inline const char* slCopy(char* Dest, const char* Source);

/***
* FUNCTION:       slConcat
* DESCRIPTION:    Append a string
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        I/O      Destination string (null terminated string)
*     Concat      I        String to append (null terminated string)
* REMARKS:        Max 'Dest' string length must be greater than 'Concat' string size with
*                 its own size.
******************************************************************/
inline const char* slConcat(char* Dest, const char* Concat);

/***
* FUNCTION:       slCompare
* DESCRIPTION:    Compare two strings
* RETURN:         bool     true: If 'Comp1' is equal to 'Comp2'
* PARAMETER:
*     Comp1       I        String to compare to 'Comp2' (null terminated string)
*     Comp2       I        String to compare to 'Comp1' (null terminated string)
*     CompCnt     I        Optional number of character to compare (default: All)
* REMARKS:        If 'CompCnt' is defined, it must be different of zero otherwise this function
*                 always returns false. 'CompCnt' can include in the comparison, the null terminated
*                 string.
******************************************************************/
inline bool slCompare(const char* Comp1, const char* Comp2);
inline bool slCompare(const char* Comp1, const char* Comp2, unsigned int CompCnt);

/***
* FUNCTION:       slLTrim
* DESCRIPTION:    Remove leading space characters
* RETURN:         'Trim' string result (const char*)
* PARAMETER:
*     Trim        I/O      String to remove its started space characters (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slLTrim(char* Trim);

/***
* FUNCTION:       slRTrim
* DESCRIPTION:    Remove trailing space characters
* RETURN:         'Trim' string result (const char*)
* PARAMETER:
*     Trim        I/O      String to remove its ended space characters (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slRTrim(char* Trim);

/***
* FUNCTION:       slTrim
* DESCRIPTION:    Remove leading and trailing space characters
* RETURN:         'Trim' string result (const char*)
* PARAMETER:
*     Trim        I/O      String to remove its started and ended space
*                          characters (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slTrim(char* Trim);

/***
* FUNCTION:       slUpper
* DESCRIPTION:    Convert a string to uppercase
* RETURN:         'Upper' string result (const char*)
* PARAMETER:
*     Dest        I/O      String to make characters upper (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slUpper(char* Upper);

/***
* FUNCTION:       slLower
* DESCRIPTION:    Convert a string to lowercase
* RETURN:         'Lower' string result (const char*)
* PARAMETER:
*     Dest        I/O      String to make characters lower (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slLower(char* Lower);

/***
* FUNCTION:       slMiddle
* DESCRIPTION:    Extract a located substring
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        O        Destination string (null terminated string)
*     Source      I        String from where to copy a substring (null terminated string)
*     Start       I        Substring position (zero based)
*     Count       I        Substring length
* REMARKS:        Max 'Dest' string length must be greater than the substring size returned.
*                 If 'Start' is less than zero, the process starts at the begining. If 'Count'
*                 is less than zero, a copy of all of the rest of 'Source' string after 'Start'
*                 position will be returned.
******************************************************************/
inline const char* slMiddle(char* Dest, const char* Source, int Start, int Count = -1);

/***
* FUNCTION:       slLeft
* DESCRIPTION:    Extract the left part of a string
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        O        Destination string (null terminated string)
*     Source      I        String from where to copy the left part substring (null terminated string)
*     Count       I        Substring length
* REMARKS:        Max 'Dest' string length must be greater than the substring size returned.
*                 If 'Count' is less than zero, a copy of the 'Source' string will be returned.
******************************************************************/
inline const char* slLeft(char* Dest, const char* Source, int Count);

/***
* FUNCTION:       slRight
* DESCRIPTION:    Extract the right part of a string
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        O        Destination string (null terminated string)
*     Source      I        String from where to copy the right part substring (null terminated string)
*     Count       I        Substring length
* REMARKS:        Max 'Dest' string length must be greater than the substring size returned.
*                 If 'Count' is less than zero, a copy of the 'Source' string will be returned.
******************************************************************/
inline const char* slRight(char* Dest, const char* Source, int Count);

/***
* FUNCTION:       slReverse
* DESCRIPTION:    Reverse a string
* RETURN:         'Rev' string result (const char*)
* PARAMETER:
*     Rev         I/O      String to reverse (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slReverse(char* Rev);

/***
* FUNCTION:       slFind
* DESCRIPTION:    Find substring in a string
* RETURN:         int      Index position of the first occurence of 'Search' in 'Source',
*                          or -1 if 'Search' string was not found
* PARAMETER:
*     Source      I        String from where to search a substring (null terminated string)
*     Search      I        String to find (null terminated string)
*     Start       I        Optional zero based searching start position (default: 0)
* REMARKS:        'Search' string must be different of "" otherwise this function always
*                 returns 0, or always return -1 if 'Source' string is empty. If 'Start'
*                 position sets the started search at the end of the 'Source' string, this
*                 function will return -1. If this parameter is less than zero, the process
*                 starts at the begining of the 'Source' string.
******************************************************************/
inline int slFind(const char* Source, const char* Search, int Start = 0);

/***
* FUNCTION:       slRevFind
* DESCRIPTION:    Find substring starting from the end of a string
* RETURN:         int      Index position of the first reverse occurence of 'Search' in
*                          'Source', or -1 if 'Search' string was not found
* PARAMETER:
*     Source      I        String from where to search a substring (null terminated string)
*     Search      I        String to find (null terminated string)
* REMARKS:        'Search' string must be different of "" otherwise this function always
*                 returns -1, like if 'Source' string is empty.
******************************************************************/
inline int slRevFind(const char* Source, const char* Search);

/***
* FUNCTION:       slRemove
* DESCRIPTION:    Delete a located substring
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        I/O      String from where to delete a substring (null terminated string)
*     Position    I        Index position of the substring to delete (zero based index)
*     DelCnt      I        Substring length to delete
* REMARKS:        If 'DelCnt' is equal or less than zero, no substring will be removed as if
*                 'Position' locates a position after the null terminated string.
******************************************************************/
inline const char* slRemove(char* Dest, int Position, int DelCnt);

/***
* FUNCTION:       slInsert
* DESCRIPTION:    Insert a substring
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        I/O      String from where to insert a substring (null terminated string)
*     Position    I        Index position of the substring to insert (zero based index)
*     Insert      I        Substring to insert (null terminated string)
* REMARKS:        Max 'Dest' string length must be greater than 'Insert' string size with its
*                 own size. If 'Position' locates a position after the null terminated string
*                 the 'Insert' substring is added at the end of the 'Dest' string.
******************************************************************/
inline const char* slInsert(char* Dest, int Position, const char* Insert);

/***
* FUNCTION:       slToNumber
* DESCRIPTION:    Convert a string to number
* RETURN:         T        Number containned in the 'Source' string
* PARAMETER:
*     Source      I        String to be converted to a number (null terminated string)
* REMARKS:        Started space or zero characters are ignored. The conversion stops when the null
*                 terminated string or a none digit character is found. If it's a signed number, the
*                 type name T must be signed too, as if it's a real number, T must be a float type
*                 like, otherwise the returned number will be wrong. The same problem will occur if
*                 the type name T length cannot contain the converted number.
******************************************************************/
template<typename T> inline T slToNumber(const char* Source);

/***
* FUNCTION:       slFormat
* DESCRIPTION:    Write formatted data string
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        I/O      Destination string (null terminated string)
*     Format      I        String format (null terminated string), with:
*                          %s    :Null terminated string argument
*                          %c    :Character argument
*                          %i    :Signed integer argument (short, int, long, ...)
*                          %I    :Unsigned integer argument (BYTE, UINT, ULONG, ...)
*                          %nnI  :Idem %I with width specification (n: 0-9)
*     ...         I        Arguments list (see type above)
* REMARKS:        Max 'Dest' string length must be greater than 'Format' string size with
*                 its  arguments.
******************************************************************/
inline const char* slFormat(char* Dest, const char* Format, ...);

/***
* FUNCTION:       slToken
* DESCRIPTION:    Find the next token in a string
* RETURN:         Next token (char*)
* PARAMETER:
*     Token       I/O      String from where to find the next token (null terminated string)
*     Delimiter   I        Delimiter character
* REMARKS:
******************************************************************/
inline char* slToken(char* Token, char Delimiter);

//////////////////////////////////////////////////////////////////////
#if defined(__cplusplus)

/***
* CLASS:                      CStrlib
* DESCRIPTION:                CString class like (without any reference count)
* CONSTRUCTORS:
*     CStrlib()               Empty string constructor (default allocation size: CSTRLIB_MEM_SCALE)
*     CStrlib(UINT)           Empty string constructor with a specific started allocation size
*     CStrlib(CStrlib)        Initialized CStrlib string cast constructor
*     CStrlib(LPCSTR)         Initialized string constructor
*     CStrlib(LPCSTR,int)     Initialized string constructor with its knowledge length
* TYPES CAST:
*     LPCSTR                  Constant string type cast
* OPERATORS:
*     =                       Copy a CStrlib object or a constant string (see 'Copy' method)
*     + & +=                  Concat a CStrlib object or a constant string (see 'Concat' method)
*     == & !=                 Compare with a CStrlib object or a constant string
* METHODS:
*     GetString:              Return CStrlib string indexed pointer
*     Copy:                   Copy a constant string to CStrlib string using its knowledge length (instead of '=' operator)
*     Concat:                 Concat a constant string to CStrlib string using its knowledge length (instead of '+=' operator)
*     GetAt:                  Return CStrlib string indexed character (return NULL for invalid index)
*     SetAt:                  Set CStrlib string indexed character
*     GetLength:              Return CStrlib string length
*     Compare:                Compare CStrlib string
*     IsEmpty:                Check if CStrlib string is empty
*     Empty:                  Empty CStrlib string
*     MakeLTrim:              Remove CStrlib leading space characters
*     MakeRTrim:              Remove CStrlib trailing space characters
*     MakeTrim:               Remove CStrlib leading and trailing space characters
*     MakeUpper:              Convert CStrlib string to uppercase
*     MakeLower:              Convert CStrlib string to lowercase
*     Middle:                 Get or assign to CStrlib string a located substring of it
*     Left:                   Get or assign to CStrlib string a left part of it
*     Right:                  Get or assign to CStrlib string a right part of it
*     Reverse:                Reverse CStrlib string
*     Find:                   Find substring in CStrlib string
*     RevFind:                Find substring starting from the end of CStrlib string
*     Remove:                 Delete a located substring from CStrlib string
*     Insert:                 Insert substring into CStrlib string
*     ToNumber:               Convert CStrlib string to number
*     Format:                 Write formatted data string in CStrlib string
* REMARKS:  The contructor max length parameter is used to initialise the started CStrlib string
*           max length, which can grow up according to string manipulations. Calling the 'Empty'
*           method permits to restore the started CStrlib string max length.
******************************************************************/

class CStrlib
{
public:
   /**************************************************************
    * Constructors/Destructor
    **************************************************************/
   CStrlib();
   CStrlib(unsigned int nAllocation);
   CStrlib(const CStrlib& stbVal);
   CStrlib(const char* lpszVal);
   CStrlib(const char* lpszVal, int nLength);

   virtual ~CStrlib();

   /**************************************************************
    * Types cast
    **************************************************************/
   inline operator const char*() const;
   
   /**************************************************************
    * Operators
    **************************************************************/
   inline const CStrlib& operator=(const CStrlib& stbVal);
   inline const CStrlib& operator=(const char* lpszVal);
   inline const CStrlib& operator=(const int iVal);

   inline const CStrlib& operator+(const CStrlib& stbVal);
   inline const CStrlib& operator+(const char* lpszVal);

   inline const CStrlib& operator+=(const CStrlib& stbVal);
   inline const CStrlib& operator+=(const char* lpszVal);

   inline bool operator==(const CStrlib& stbVal) const;
   inline bool operator==(const char* lpszVal) const;

   inline bool operator!=(const CStrlib& stbVal) const;
   inline bool operator!=(const char* lpszVal) const;


   /**************************************************************
    * Methods
    **************************************************************/
   inline const char* GetString(int Index = 0) const;
   inline const CStrlib& Copy(const char* lpszVal, int nLength);
   inline const CStrlib& Concat(const char* lpszVal, int nLength);
   inline char GetAt(int nPos) const;
   inline bool SetAt(int nPos, char cVal);
   inline int GetLength() const;
   inline bool Compare(const char* lpszComp, unsigned int CompCnt) const;
   inline bool IsEmpty() const;
   inline void Empty();
   inline const CStrlib& MakeLTrim();
   inline const CStrlib& MakeRTrim();
   inline const CStrlib& MakeTrim();
   inline const CStrlib& MakeUpper();
   inline const CStrlib& MakeLower();

   inline const char* Middle(char* lpszMid, int Start, int Count = -1) const;
   inline const CStrlib& Middle(int Start, int Count = -1);

   inline const char* Left(char* lpszLeft, int Count) const;
   inline const CStrlib& Left(int Count);

   inline const char* Right(char* lpszRight, int Count) const;
   inline const CStrlib& Right(int Count);

   inline const CStrlib& Reverse();
   inline int Find(const char* Search, int Start = 0) const;
   inline int RevFind(const char* Search) const;
   inline const CStrlib& Remove(int Position, int DelCnt);
   inline const CStrlib& Insert(int Position, const char* Insert, int InsertLen = 0);
   //template<typename T> inline T ToNumber(int Start = 0) const;
   template<typename T> inline T ToNumber(T Default, int Start = 0) const;
   inline const CStrlib& Format(const char* Format, ...);

   ////////////////////////////////////////////////////////////////

private:
   char* m_lpszStrLib;  // String
   int   m_nLength;     // String length
   int   m_nAllocated;  // Allocated memory

   /**** New string allocation (if needed) ****/
   inline void NewMemAlloc(int nNewAlloc, bool bConcatOp);

   /**** Conversion from T integer to string ****/
   template<typename T> inline int IntToStr(int Idx, T Num, char Width = -1);

};

#endif // __cplusplus
#endif // _STR_LIB

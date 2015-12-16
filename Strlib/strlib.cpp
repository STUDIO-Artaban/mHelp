// strlib.cpp: Implementation of the String Library (SL).
//
//////////////////////////////////////////////////////////////////////

#include "strlib.h"
#include <assert.h>
#include <memory.h>
#include <stdarg.h>

#define  POINT_CHAR              '.'
#define  COMMA_CHAR              ','
#define  ZERO_CHAR               '0'
#define  ZERO_STRING             "0"
#define  DIGIT_MASK              0x0F
#define  DIGIT_CODE              0x30

#define  ARG_FLAG                '%'
#define  SPACE_CHAR              ' '
#define  UPLOW_MASK              0x20

#define  NULL                    0

/**** Conversion from T integer to string ****/
template<typename T> inline int IntToStr(char* Dest, int Idx, T Num, char Width = -1)
{
   long nDiv = 1000000000;

   while((nDiv != 1) && ((Num / nDiv) == 0))
      nDiv /= 10;

   // Conversion loop
   do
   {
      Dest[Idx] = (char)((Num / nDiv) + DIGIT_CODE); // Digit character code
      Num -= (T)((Dest[Idx++] & DIGIT_MASK) * nDiv);
   }
   while((--Width) && ((nDiv /= 10) != 0));

   return Idx;
}

//////////////////////////////////////////////////////////////////////

/***
* FUNCTION:       slLength
* DESCRIPTION:    Return the length of a string
* RETURN:         'Len' string length (unsigned int)
* PARAMETER:
*     Len         I        String to return its length (null terminated string)
* REMARKS:
******************************************************************/
inline unsigned int slLength(const char* Len)
{
   unsigned int iLen = 0;

   // Entire string loop
   for ( ; (*Len); Len++, iLen++);

   return iLen;
}

/***
* FUNCTION:       slCopy
* DESCRIPTION:    Copy a string
* RETURN:         'Dest' string result (const char*)
* PARAMETER:
*     Dest        O        Destination string (null terminated string)
*     Source      I        String to copy (null terminated string)
* REMARKS:        Max 'Dest' string length must be greater than 'Source' string size.
******************************************************************/
inline const char* slCopy(char* Dest, const char* Source)
{
   char* lpWalk = Dest;

   // Copy the entire source string
   while (*(lpWalk++) = *(Source++));

   return Dest;
}

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
inline const char* slConcat(char* Dest, const char* Concat)
{
   if (*Concat)
   {
      char* lpWalk = Dest;

      // Entire string loop
      for ( ; (*lpWalk); lpWalk++);

      // Copy the entire string to append
      while (*(lpWalk++) = *(Concat++));
   }

   return Dest;
}

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
inline bool slCompare(const char* Comp1, const char* Comp2)
{
   // Loop until difference or null terminated string is found
   for ( ; ((*Comp1) && (*Comp2) && (*Comp1 == *Comp2)); Comp1++, Comp2++);

   return ((!*Comp1) && (!*Comp2));
}
//////
inline bool slCompare(const char* Comp1, const char* Comp2, unsigned int CompCnt)
{
   // Loop until 'CompCnt' is reached or difference/null terminated string is found
   for ( ; ((--CompCnt) && (*Comp1) && (*Comp2) && (*Comp1 == *Comp2)); Comp1++, Comp2++);

   return ((!CompCnt) && (*Comp1 == *Comp2));
}

/***
* FUNCTION:       slLTrim
* DESCRIPTION:    Remove leading space characters
* RETURN:         'Trim' string result (const char*)
* PARAMETER:
*     Trim        I/O      String to remove its started space characters (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slLTrim(char* Trim)
{
   const char* lpWalk = Trim;
   char* lpStore = Trim;

   // Loop on started space characters
   for ( ; ((*lpWalk) && (*lpWalk == SPACE_CHAR)); lpWalk++);

   // Copy the string so found
   while (*(lpStore++) = *(lpWalk++));

   return Trim;
}

/***
* FUNCTION:       slRTrim
* DESCRIPTION:    Remove trailing space characters
* RETURN:         'Trim' string result (const char*)
* PARAMETER:
*     Trim        I/O      String to remove its ended space characters (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slRTrim(char* Trim)
{
   char* lpWalk = Trim;

   // Entire string loop
   for ( ; (*lpWalk); lpWalk++)
   {
      const char* lpRest = lpWalk;

      // Loop on space characters remaining
      for ( ; ((*lpRest) && (*lpRest == SPACE_CHAR)); lpRest++);

      if (!*lpRest)
         *lpWalk = 0;
   }

   return Trim;
}

/***
* FUNCTION:       slTrim
* DESCRIPTION:    Remove leading and trailing space characters
* RETURN:         'Trim' string result (const char*)
* PARAMETER:
*     Trim        I/O      String to remove its started and ended space
*                          characters (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slTrim(char* Trim)
{
   char* lpWalk = Trim;
   char* lpStore = Trim;

   // Loop on started space characters
   for ( ; ((*lpWalk) && (*lpWalk == SPACE_CHAR)); lpWalk++);

   // Loop on the rest of the string
   for ( ; (*lpWalk); lpWalk++, lpStore++)
   {
      const char* lpRest = lpWalk;

      // Loop on space characters remaining
      for ( ; ((*lpRest) && (*lpRest == SPACE_CHAR)); lpRest++);

      if (!*lpRest)
         break;
      else
         *lpStore = *lpWalk;
   }

   *lpStore = 0;
   return Trim;
}

/***
* FUNCTION:       slUpper
* DESCRIPTION:    Convert a string to uppercase
* RETURN:         'Upper' string result (const char*)
* PARAMETER:
*     Dest        I/O      String to make characters upper (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slUpper(char* Upper)
{
   char* lpWalk = Upper;

   // Entire string loop
   for ( ; (*lpWalk); lpWalk++)
   {
      if (isLower(*lpWalk))
         *lpWalk = *lpWalk - UPLOW_MASK; // Make it uppercase
   }

   return Upper;
}

/***
* FUNCTION:       slLower
* DESCRIPTION:    Convert a string to lowercase
* RETURN:         'Lower' string result (const char*)
* PARAMETER:
*     Dest        I/O      String to make characters lower (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slLower(char* Lower)
{
   char* lpWalk = Lower;

   // Entire string loop
   for ( ; (*lpWalk); lpWalk++)
   {
      if (isUpper(*lpWalk))
         *lpWalk = *lpWalk + UPLOW_MASK; // Make it lowercase
   }

   return Lower;
}

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
inline const char* slMiddle(char* Dest, const char* Source, int Start, int Count)
{
   const char* lpWalk = Source;
   char* lpStore = Dest;

   // Loop to set started position
   for ( ; ((*lpWalk) && (Start > 0)); lpWalk++, Start--);

   // Copy the substring such located
   for ( ; ((*lpWalk) && (Count != 0)); lpWalk++, Count--, lpStore++)
      *lpStore = *lpWalk;

   *lpStore = 0;
   return Dest;
}

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
inline const char* slLeft(char* Dest, const char* Source, int Count)
{
   char* lpStore = Dest;

   // Copy the left part of the source string
   for ( ; ((*Source) && (Count != 0)); Source++, Count--, lpStore++)
      *lpStore = *Source;

   *lpStore = 0;
   return Dest;
}

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
inline const char* slRight(char* Dest, const char* Source, int Count)
{
   const char* lpWalk = Source;
   char* lpStore = Dest;

   // Entire string loop
   for ( ; (*lpWalk); lpWalk++);

   // Go back to the appropriate right part location
   for ( ; ((lpWalk != Source) && (Count != 0)); lpWalk--, Count--);

   // Copy the right part so located
   while (*(lpStore++) = *(lpWalk++));

   return Dest;
}

/***
* FUNCTION:       slReverse
* DESCRIPTION:    Reverse a string
* RETURN:         'Rev' string result (const char*)
* PARAMETER:
*     Rev         I/O      String to reverse (null terminated string)
* REMARKS:
******************************************************************/
inline const char* slReverse(char* Rev)
{
   char* lpWalk1 = Rev;
   char* lpWalk2 = Rev;

   // Position at the last character
   for ( ; (*lpWalk1); lpWalk1++);
   lpWalk1--;

   // Loop to reverse characters until the middle string was found
   for ( ; ((lpWalk1 != lpWalk2) && ((lpWalk1 + 1) != lpWalk2)); lpWalk1--, lpWalk2++)
   {
      char cLag = *lpWalk2;
      *lpWalk2 = *lpWalk1;
      *lpWalk1 = cLag;
   }

   return Rev;
}

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
inline int slFind(const char* Source, const char* Search, int Start)
{
   int iPos = 0;

   // Loop to set start position
   for ( ; ((*Source) && (Start > 0)); Source++, Start--, iPos++);

   // Rest string loop
   for ( ; (*Source); Source++, iPos++)
   {
      const char* lpWalk = Source;
      const char* lpFind = Search;

      // Search string loop
      for ( ; (*lpWalk) && (*lpFind) && (*lpWalk == *lpFind); lpWalk++, lpFind++);

      if (!*lpFind)
         return iPos;
   }

   return NOT_FOUND;
}

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
inline int slRevFind(const char* Source, const char* Search)
{
   // Check search string
   if (!*Search)
      return NOT_FOUND;

   int iLast = NOT_FOUND;

   // Entire source string loop
   for (int iPos = 0; (*Source); Source++, iPos++)
   {
      const char* lpWalk = Source;
      const char* lpFind = Search;

      // Search string loop
      for ( ; (*lpWalk) && (*lpFind) && (*lpWalk == *lpFind); lpWalk++, lpFind++);

      if (!*lpFind)
         iLast = iPos; // Store the last position of the searched string
   }

   return iLast;
}

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
inline const char* slRemove(char* Dest, int Position, int DelCnt)
{
   if ((Position > -1) && (DelCnt > 0))
   {
      const char* lpWalk = Dest;

      // Set position to the substring to remove
      for (int i = Position; ((*lpWalk) && (i)); lpWalk++, i--);

      // Set position to the rest of the string after the incoming deletion
      for ( ; ((*lpWalk) && (DelCnt)); lpWalk++, DelCnt--);

      // Delete substring by adding the rest of the string so found
      while (Dest[Position++] = *(lpWalk++));
   }

   return Dest;
}

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
inline const char* slInsert(char* Dest, int Position, const char* Insert)
{
   if (Position > -1)
   {
      for ( ; (*Insert); Insert++, Position++)
      {
         char* lpWalk = Dest;

         // Set position to the right insertion location
         for (int i = Position; ((*lpWalk) && (i)); lpWalk++, i--);

         char c2,c1 = *lpWalk;

         // Insert the next one substring's character
         *lpWalk = *Insert;

         if (c1)
         {
            // Shift string loop
            do
            {
               c2 = *(++lpWalk);
               *lpWalk = c1;
               c1 = c2;
            }
            while(c2);
         }

         *(++lpWalk) = 0;
      }
   }

   return Dest;
}

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
template<typename T> inline T slToNumber(const char* Source)
{
   T Num = 0;
   float fDiv = 1.0f;
   bool bComma = false;
   bool bSigned = false;

   // Search the first none zero digit or the sign character if any
   for ( ; ((*Source) && ((*Source == SPACE_CHAR) || (*Source == ZERO_CHAR))); Source++);

   // Check signed number
   if (*Source == '-')
   {
      Source++;
      bSigned = true;
   }

   // Conversion loop
   for ( ; (*Source) && (isDigit(*Source) || (*Source == POINT_CHAR) || (*Source == COMMA_CHAR)); Source++)
   {
      // Check comma
      if ((*Source == POINT_CHAR) || (*Source == COMMA_CHAR))
      {
         if (bComma)
            break; // Comma already found

         bComma = true;
         fDiv = 10.0f;
      }
      else
      if (!bComma)
         Num = (*Source & DIGIT_MASK) + (Num * 10); // Character to digit
      else
      {
         Num += (T)((*Source & DIGIT_MASK) / fDiv); // Character to decimal digit
         fDiv *= 10.0f;
      }
   }

   return bSigned? (Num *= -1):Num;
}

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
inline const char* slFormat(char* Dest, const char* Format, ...)
{
   va_list argList;
   int i = 0;

   // Set arguments list
   va_start(argList, Format);

   for ( ; (*Format); Format++)
   {
      // Check argument flag
      if (*Format == ARG_FLAG)
      {
         Format++;

         // Check argument ID
         switch(*Format)
         {
            case 's': // String
            {
               const char* lpArg = va_arg(argList, const char*);
               Dest[i] = 0;
               i += slLength(lpArg);
               slConcat(Dest, lpArg);
               break;
            }
            case 'c': // Character
            {
               Dest[i++] = va_arg(argList, char);
               break;
            }
            case 'i': // Integer (signed)
            {
               long lArg = va_arg(argList, long);
               // Check signed number
               if (lArg < 0)
               {
                  Dest[i++] = '-';
                  lArg *= -1;
               }
               i = IntToStr<long>(Dest, i, lArg);
               break;
            }
            case 'I': // Integer (unsigned)
            {
               unsigned long ulArg = va_arg(argList, unsigned long);
               i = IntToStr<unsigned long>(Dest, i, ulArg);
               break;
            }
            default: // Default
            {
               // Check width specification
               if ((isDigit(*Format)) && (isDigit(*(Format+1))) && (*(Format+2) == 'I'))
               {
                  unsigned long ulArg = va_arg(argList, unsigned long);
                  char cWidth = ((*(Format+1)) & DIGIT_MASK) + (((*Format) & DIGIT_MASK) * 10);
                  int nDelta = IntToStr<unsigned long>(Dest, i, ulArg, cWidth);

                  Dest[nDelta] = 0;

                  // Insert zeros digit if needed
                  for ( ; (cWidth > (nDelta - i)); nDelta++)
                     slInsert(Dest, i, ZERO_STRING);

                  i = nDelta;
                  Format += 2; // + Width specification length (2 digits)
               }
               else // Invalid argument ID
               {
                  Dest[i++] = ARG_FLAG;
                  Dest[i++] = *Format;
               }
               break;
            }
         }
      }
      else
         Dest[i++] = *Format; // Write Format's character
   }

   // Reset arguments list
   va_end(argList);

   Dest[i] = 0;
   return Dest;
}

/***
* FUNCTION:       slToken
* DESCRIPTION:    Find the next token in a string
* RETURN:         Next token (char*)
* PARAMETER:
*     Token       I/O      String from where to find the next token (null terminated string)
*     Delimiter   I        Delimiter character
* REMARKS:
******************************************************************/
inline char* slToken(char* Token, char Delimiter)
{
   static char* s_Token = 0;
   static char* s_NextToken = 0;

   char* lpWalk = s_NextToken = (Token)? Token:s_Token;

   if (lpWalk)
   {
      // Find the next token
      for ( ; ((*lpWalk) && (*lpWalk != Delimiter)); lpWalk++);

      // Check it
      if (*lpWalk)
      {
         s_Token = lpWalk + 1; // Next token
         *lpWalk = 0;
      }
      else
         s_Token = 0; // No more token
   }

   return s_NextToken;
}

//////////////////////////////////////////////////////////////////////
#if defined(__cplusplus)

#define  CSTRLIB_MEM_SCALE       32 // Multiple of 2
#define  GetMemToAlloc(n)        (unsigned int)((((unsigned int)(n / CSTRLIB_MEM_SCALE)) * CSTRLIB_MEM_SCALE) + \
                                                (((n % CSTRLIB_MEM_SCALE))? CSTRLIB_MEM_SCALE:0))

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

/**************************************************************
* Constructors/Destructor
**************************************************************/
CStrlib::CStrlib():m_nLength(0)
{  
    m_lpszStrLib = new char[m_nAllocated = CSTRLIB_MEM_SCALE];
    *m_lpszStrLib = 0;
}
CStrlib::CStrlib(unsigned int nAllocation):m_nLength(0)
{  
    m_lpszStrLib = new char[m_nAllocated = GetMemToAlloc(nAllocation)];
    *m_lpszStrLib = 0;
}
CStrlib::CStrlib(const CStrlib& stbVal)
{
    m_lpszStrLib = new char[m_nAllocated = GetMemToAlloc((m_nLength = stbVal.m_nLength) + 1)];
    memcpy(m_lpszStrLib, stbVal.m_lpszStrLib, stbVal.m_nLength + 1);
}
CStrlib::CStrlib(const char* lpszVal)
{
    m_lpszStrLib = new char[m_nAllocated = GetMemToAlloc((m_nLength = slLength(lpszVal)) + 1)];
    memcpy(m_lpszStrLib, lpszVal, m_nLength + 1);
}
CStrlib::CStrlib(const char* lpszVal, int nLength)
{
    m_lpszStrLib = new char[m_nAllocated = GetMemToAlloc((m_nLength = nLength) + 1)];
    memcpy(m_lpszStrLib, lpszVal, m_nLength + 1);
}

CStrlib::~CStrlib() { delete [] m_lpszStrLib; }

/**************************************************************
* Types cast
**************************************************************/
inline CStrlib::operator const char*() const { return m_lpszStrLib; }
   
/**************************************************************
* Operators
**************************************************************/
inline const CStrlib& CStrlib::operator=(const CStrlib& stbVal)
{
    NewMemAlloc(stbVal.m_nLength, false);
    memcpy(m_lpszStrLib, stbVal.m_lpszStrLib, m_nLength + 1);
    return *this;
}
inline const CStrlib& CStrlib::operator=(const char* lpszVal)
{
    NewMemAlloc(slLength(lpszVal), false);
    memcpy(m_lpszStrLib, lpszVal, m_nLength + 1);
    return *this;
}
inline const CStrlib& CStrlib::operator=(const int iVal)
{
    assert(iVal == NULL); // Only available for hash table behavior (CStrlib = NULL)
                        // Otherwise use 'IntToStr' method to convert 'int' into 'CStrlib'
    if (!iVal)
        Empty();
    return *this;
}
/////
inline const CStrlib& CStrlib::operator+(const CStrlib& stbVal)
{
    NewMemAlloc(m_nLength + stbVal.m_nLength, true);
    slConcat(m_lpszStrLib, stbVal.m_lpszStrLib);
    return *this;
}
inline const CStrlib& CStrlib::operator+(const char* lpszVal)
{
    NewMemAlloc(m_nLength + slLength(lpszVal), true);
    slConcat(m_lpszStrLib, lpszVal);
    return *this;
}
/////
inline const CStrlib& CStrlib::operator+=(const CStrlib& stbVal)
{
    NewMemAlloc(m_nLength + stbVal.m_nLength, true);
    slConcat(m_lpszStrLib, stbVal.m_lpszStrLib);
    return *this;
}
inline const CStrlib& CStrlib::operator+=(const char* lpszVal)
{
    NewMemAlloc(m_nLength + slLength(lpszVal), true);
    slConcat(m_lpszStrLib, lpszVal);
    return *this;
}
/////
inline bool CStrlib::operator==(const CStrlib& stbVal) const { return (!memcmp(m_lpszStrLib, stbVal.m_lpszStrLib, m_nLength + 1)); }
inline bool CStrlib::operator==(const char* lpszVal) const { return (!memcmp(m_lpszStrLib, lpszVal, m_nLength + 1)); }
/////
inline bool CStrlib::operator!=(const CStrlib& stbVal) const { return (memcmp(m_lpszStrLib, stbVal.m_lpszStrLib, m_nLength + 1))? true:false; }
inline bool CStrlib::operator!=(const char* lpszVal) const { return (memcmp(m_lpszStrLib, lpszVal, m_nLength + 1))? true:false; }

/**************************************************************
* Methods
**************************************************************/
inline const char* CStrlib::GetString(int Index) const { return ((Index > -1) && (m_nLength > Index))? &m_lpszStrLib[Index]:""; }
inline const CStrlib& CStrlib::Copy(const char* lpszVal, int nLength)
{
    NewMemAlloc(nLength, false);
    memcpy(m_lpszStrLib, lpszVal, m_nLength + 1);
    return *this;
}
inline const CStrlib& CStrlib::Concat(const char* lpszVal, int nLength)
{
    NewMemAlloc(m_nLength + nLength, true);
    slConcat(m_lpszStrLib, lpszVal);
    return *this;
}
inline char CStrlib::GetAt(int nPos) const { return ((nPos > -1) && (m_nLength > nPos))? m_lpszStrLib[nPos]:0; }
inline bool CStrlib::SetAt(int nPos, char cVal)
{  
    if ((nPos > -1) && (m_nLength > nPos))
    {
        if (!(m_lpszStrLib[nPos] = cVal))
        m_nLength = nPos + 1;
        return true;
    }
    return false;
}
inline int CStrlib::GetLength() const { return m_nLength; }
inline bool CStrlib::Compare(const char* lpszComp, unsigned int CompCnt) const { return (!memcmp(m_lpszStrLib, lpszComp, CompCnt)); }
inline bool CStrlib::IsEmpty() const { return (!m_nLength); }
inline void CStrlib::Empty() { *m_lpszStrLib = m_nLength = 0; }
inline const CStrlib& CStrlib::MakeLTrim() { slLTrim(m_lpszStrLib); return *this; }
inline const CStrlib& CStrlib::MakeRTrim() { slRTrim(m_lpszStrLib); return *this; }
inline const CStrlib& CStrlib::MakeTrim() { slTrim(m_lpszStrLib); return *this; }
inline const CStrlib& CStrlib::MakeUpper() { slUpper(m_lpszStrLib); return *this; }
inline const CStrlib& CStrlib::MakeLower() { slLower(m_lpszStrLib); return *this; }
/////
inline const char* CStrlib::Middle(char* lpszMid, int Start, int Count) const { return slMiddle(lpszMid, m_lpszStrLib, Start, Count); }
inline const CStrlib& CStrlib::Middle(int Start, int Count)
{
    const char* lpWalk = m_lpszStrLib;

    // Loop to set started position
    for ( ; ((*lpWalk) && (Start > 0)); lpWalk++, Start--);

    // Copy the substring such located
    for (m_nLength = 0; ((*lpWalk) && (Count != 0)); lpWalk++, Count--, m_nLength++)
        m_lpszStrLib[m_nLength] = *lpWalk;

    m_lpszStrLib[m_nLength] = 0;
    return *this;
}
/////
inline const char* CStrlib::Left(char* lpszLeft, int Count) const { return slLeft(lpszLeft, m_lpszStrLib, Count); }
inline const CStrlib& CStrlib::Left(int Count)
{
    const char* lpWalk = m_lpszStrLib;

    // Set length depending on the parameter and CStrlib string
    for (m_nLength = 0; ((*lpWalk) && (Count != 0)); lpWalk++, Count--, m_nLength++);

    m_lpszStrLib[m_nLength] = 0;
    return *this;
}
/////
inline const char* CStrlib::Right(char* lpszRight, int Count) const { return slRight(lpszRight, m_lpszStrLib, Count); }
inline const CStrlib& CStrlib::Right(int Count)
{
    const char* lpWalk = m_lpszStrLib;

    // Entire string loop
    for ( ; (*lpWalk); lpWalk++);

    // Go back to the appropriate right part location
    for ( ; ((lpWalk != m_lpszStrLib) && (Count != 0)); lpWalk--, Count--);

    // Copy the right part so located
    for (m_nLength = 0; (m_lpszStrLib[m_nLength] = *lpWalk); lpWalk++, m_nLength++);

    return *this;
}
/////
inline const CStrlib& CStrlib::Reverse() { slReverse(m_lpszStrLib); return *this; }
inline int CStrlib::Find(const char* Search, int Start) const { return slFind(m_lpszStrLib, Search, Start); }
inline int CStrlib::RevFind(const char* Search) const { return slRevFind(m_lpszStrLib, Search); }
inline const CStrlib& CStrlib::Remove(int Position, int DelCnt)
{  
    m_nLength = slLength(slRemove(m_lpszStrLib, Position, DelCnt));
    return *this;
}
inline const CStrlib& CStrlib::Insert(int Position, const char* Insert, int InsertLen)
{
    if ((Position > -1) && (*Insert))
    {
        NewMemAlloc(m_nLength + (InsertLen? InsertLen:slLength(Insert)), true);
        slInsert(m_lpszStrLib, Position, Insert);
    }
    return *this;
}
//template<typename T> inline T CStrlib::ToNumber(int Start = 0) const { return ((Start > -1) && (m_nLength > Start))? slToNumber<T>(&m_lpszStrLib[Start]):(T)0;}
template<typename T> inline T CStrlib::ToNumber(T Default, int Start) const { return ((Start > -1) && (m_nLength > Start))? slToNumber<T>(&m_lpszStrLib[Start]):Default; }
inline const CStrlib& CStrlib::Format(const char* Format, ...)
{
    va_list argList;
    int i = 0;

    // Set arguments list
    va_start(argList, Format);

    for ( ; (*Format); Format++)
    {
        // Check argument flag
        if (*Format == ARG_FLAG)
        {
        Format++;

        // Check argument ID
        switch(*Format)
        {
            case 's': // String
            {
                const char* lpArg = va_arg(argList, const char*);
                m_lpszStrLib[i] = 0;
                int nLen = slLength(lpArg);
                i += nLen;
                NewMemAlloc(m_nLength + nLen, true);
                slConcat(m_lpszStrLib, lpArg);
                break;
            }
            case 'c': // Character
            {
                m_lpszStrLib[i] = 0;
                NewMemAlloc(m_nLength + 1, true);
                m_lpszStrLib[i++] = va_arg(argList, char);
                break;
            }
            case 'i': // Integer (signed)
            {
                long lArg = va_arg(argList, long);
                // Check signed number
                if (lArg < 0)
                {
                    m_lpszStrLib[i] = 0;
                    NewMemAlloc(m_nLength + 1, true);
                    m_lpszStrLib[i++] = '-';
                    lArg *= -1;
                }
                i = IntToStr(i, lArg);
                break;
            }
            case 'I': // Integer (unsigned)
            {
                unsigned long ulArg = va_arg(argList, unsigned long);
                i = IntToStr(i, ulArg);
                break;
            }
            default: // Default
            {
                // Check width specification
                if ((isDigit(*Format)) && (isDigit(*(Format+1))) && (*(Format+2) == 'I'))
                {
                    unsigned long ulArg = va_arg(argList, unsigned long);
                    char cWidth = ((*(Format+1)) & DIGIT_MASK) + (((*Format) & DIGIT_MASK) * 10);
                    int nDelta = IntToStr(i, ulArg, cWidth);

                    m_lpszStrLib[nDelta] = 0;

                    // Insert zeros digit if needed
                    for ( ; (cWidth > (nDelta - i)); nDelta++)
                    Insert(i, ZERO_STRING);

                    i = nDelta;
                    Format += 2; // + Width specification length (2 digits)
                }
                else // Invalid argument ID
                {
                    m_lpszStrLib[i] = 0;
                    NewMemAlloc(m_nLength + 2, true);
                    m_lpszStrLib[i++] = ARG_FLAG;
                    m_lpszStrLib[i++] = *Format;
                }
                break;
            }
        }
        }
        else
        {
        m_lpszStrLib[i] = 0;
        NewMemAlloc(m_nLength + 1, true);
        m_lpszStrLib[i++] = *Format; // Write format's character
        }
    }

    // Reset arguments list
    va_end(argList);

    m_lpszStrLib[i] = 0;
    return *this;
}

////////////////////////////////////////////////////////////////

/**** New string allocation (if needed) ****/
inline void CStrlib::NewMemAlloc(int nNewAlloc, bool bConcatOp)
{
    m_nLength = nNewAlloc++;

    if (m_nAllocated < nNewAlloc)
    {
        if (bConcatOp)
        {
            char* lpszStore = new char[m_nAllocated];
            memcpy(lpszStore, m_lpszStrLib, m_nAllocated);

            int nLastAlloc = m_nAllocated;
            m_nAllocated = GetMemToAlloc(nNewAlloc);

            delete m_lpszStrLib;
            m_lpszStrLib = new char[m_nAllocated]; // New allocation

            memcpy(m_lpszStrLib, lpszStore, nLastAlloc);
            delete lpszStore;
        }
        else
        {
            delete m_lpszStrLib;
            m_lpszStrLib = new char[m_nAllocated = GetMemToAlloc(nNewAlloc)]; // New allocation
        }
    }
}

/**** Conversion from T integer to string ****/
template<typename T> inline int CStrlib::IntToStr(int Idx, T Num, char Width)
{
    long nDiv = 1000000000;

    while((nDiv != 1) && ((Num / nDiv) == 0))
        nDiv /= 10;

    // Conversion loop
    do
    {
        m_lpszStrLib[Idx] = 0;
        NewMemAlloc(m_nLength + 1, true);
        m_lpszStrLib[Idx] = (char)((Num / nDiv) + DIGIT_CODE); // Digit character code
        Num -= (m_lpszStrLib[Idx++] & DIGIT_MASK) * nDiv;
    }
    while((--Width) && ((nDiv /= 10) != 0));

    return Idx;
}

#endif // __cplusplus


#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iomanip> // required for the hex (BYTE *) to string convertion

//  TO LPCTSTR
//  ==========

// Convert a DWORD to an LPCTSTR
LPCTSTR convertToLPCTSTR(DWORD valueToConvert);

// Convert a DWORD to an LPCSTR
LPCSTR convertToLPCSTR(DWORD valueToConvert);

//  TO DWORD
//  ========

// Convert a STRING to DWORD
DWORD convertToDWORD(std::string valueToConvert);

//  TO STRING
//  =========

// Convert DWORD to string
std::string convertToString(DWORD valueToConvert);
// Convert TCHAR to string - this is a char *
//std::string convertToString(TCHAR  valueToConvert[]);
// Convert char * to string
std::string convertToString(char * valueToConvert);
// Convert LPVOID to string
std::string convertToString(LPVOID valueToConvert);
// Convert HANDLE to string
std::string convertToString(HANDLE hwnd);
// BYTE* to string
std::string convertToString(BYTE *data);

//  TO TCHAR
//  ========

// Convert string to TCHAR
TCHAR * convertToTCHAR(std::string valueToConvert);

//  TO INT - works better for process IDs, better than converting to DWORD!
//  ======

// Convert string to INT
int convertToINT(std::string valueToConvert);
// Convert DWORD to INT
unsigned int convertToINT(DWORD valueToConvert);

//  TO HEX
//  ======

// Convert DEC to HEX
std::string convertToHEX(DWORD valueToConvert);
// Convert INT to HEX
std::string convertToHEX(int valueToConvert);

// BIG ENDIAN TO LITTLE ENDIAN
// ===========================
std::string convertToHEXLittleEndian(std::string valueToConvert);

/*
* this function concatenates 2 strings of type LPSTR (char *)
* use c_str() to convert the returned value back into a char *, if need be
*/
std::string concatStrings(char * pFirstString, char * pSecondString);
std::string concatStrings(std::string sFirstString, char * pSecondString);

std::string concatStrings(char * pFirstString, std::string sSecondString, char * pThirdString);
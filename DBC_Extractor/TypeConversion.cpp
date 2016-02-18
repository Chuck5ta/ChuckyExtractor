/*
	These functions convert from one type to another and from one base to another
	e,g, DWORD to LPCSTR, DWORD to string, DEC to HEX

	Author: Chuck E 
    (only in that I collated all of these functions into this file, not that I created them from scratch)
	Date: 22nd of July, 2014

    15 Feb 2016 - added a function to concatenate to LPTSTR strings (char *)
    it returns an std::string, which can be turned back into a char * with the use of c_str()

    I probably should change the name of this file to TypeManipulation
 */

#include "TypeConversion.h"

/*
	Convert DWORD to LPCTSTR
	- can then be used in MessageBox()

	DISABLED BECAUSE WE CAN CONVERT THE DWORD TO A STRING< THEN USE THAT
 */
LPCTSTR convertToLPCTSTR(DWORD valueToConvert)
{
	char szTest[10];
	sprintf_s(szTest, "%d", valueToConvert);

	return szTest;
} 

LPCSTR convertToLPCSTR(DWORD valueToConvert)
{
	char szTest[10];
	sprintf_s(szTest, "%d", valueToConvert);

	return szTest;
} 

/*
	Convert STRING to DWORD
	- does NOT work for process IDs!!!!!
 */
DWORD convertToDWORD(std::string valueToConvert)
{
    DWORD doubleWord;

    std::stringstream dwordStream;

    dwordStream << valueToConvert.substr(0,4);
    dwordStream >> std::hex >> doubleWord;

	return doubleWord;
}


/*
	Convert STRING to INT
	- works very well for process IDs
 */
int convertToINT(std::string valueToConvert)
{
	int result = atoi(valueToConvert.c_str());

	return result;
}


/*
	Convert DWORD to INT
 */
unsigned int convertToINT(DWORD valueToConvert)
{
	const char *dw = "0xFDE390";

	std::istringstream is( valueToConvert );

	unsigned int x;

	is >> std::hex >> x;

	return x;
}


/*
	Convert DWORD to string
 */
std::string convertToString(DWORD valueToConvert)
{
	std::ostringstream stream;
	stream << valueToConvert;
	std::string str = stream.str();
	return str;
}

/*
    Convert char * to string
*/
std::string convertToString(char * valueToConvert)
{
    std::ostringstream stream;
    stream << valueToConvert;
    std::string str = stream.str();
    return str;
}

std::string convertToString(HANDLE hwnd)
{
	std::stringstream streamData;
	std::string str;
	streamData << hwnd;
	streamData >> str;
	return str;
}

// BYTE* to string
std::string convertToString(BYTE *data)
{
	DWORD result = (DWORD)data;

    return convertToString(result);
}

/*
	Convert string to TCHAR array
 */
TCHAR * convertToTCHAR(std::string valueToConvert)
{
	TCHAR *param=new TCHAR[valueToConvert.size()+1];
	param[valueToConvert.size()]=0;
	//As much as we'd love to, we can't use memcpy() because
	//sizeof(TCHAR)==sizeof(char) may not be true:
	std::copy(valueToConvert.begin(),valueToConvert.end(),param);
	return param;
}

/*
	Convert DEC (DWORD) to HEX
 */
std::string convertToHEX(DWORD valueToConvert)
{
	std::stringstream ss;
	ss << std::hex << valueToConvert; // int decimal_value
	std::string res ( ss.str() );
	
	return res;
}

/*
	Convert DEC (INT) to HEX
 */
std::string convertToHEX(int valueToConvert)
{
	std::stringstream ss;
	ss << std::hex << valueToConvert; // int decimal_value
	std::string res ( ss.str() );
	
	return res;
}


// BIG ENDIAN TO LITTLE ENDIAN
// ===========================
/*
	Convert HEX (Big Endian to Little Endian)
 */

std::string convertToHEXLittleEndian(std::string valueToConvert)
{
	std::string result = "";

	// work backwards through the char grouping them into 2 and storing them from the start of a new string
	int length = valueToConvert.length();
	 
	for (int x=0; x<length; x += 2)
	{
		if ((length%2)==0) // there is an even number of characters in the address
		{
			result = valueToConvert.substr(x,1) + valueToConvert.substr(x+1,1) + result;
		}
		else // odd length value - there is an odd number of characters in the address
			if (x < length-1)
				result = valueToConvert.substr(x,1) + valueToConvert.substr(x+1,1) + result;
			else
				result = '0' + valueToConvert.substr(x,1) + result;
	}
	 
	return result;
}




std::string convertToHEXLittleEndian_OLD(std::string valueToConvert)
{
	std::string result = "";

	// work backwards through the char grouping them into 2 and storing them from the start of a new string
	int length = valueToConvert.length();
	 
	for (int x=length; x>0; x -= 2)
	{
		if ((length%2)==0) // there is an even number of characters in the address
		{
			result += valueToConvert.substr(x-2,1) + valueToConvert.substr(x-1,1);
		}
		else // odd length value - there is an odd number of characters in the address
			if (x > 1)
				result += valueToConvert.substr(x-2,1) + valueToConvert.substr(x-1,1);
			else
				result += valueToConvert.substr(x-1,1) + '0';
	}
	 
	return result;
}

/*
 * this function concatenates 2 strings of type LPSTR (char *)
 */
std::string concatStrings(char * pFirstString, char * pSecondString)
{
    char str[80];
    strcpy(str, pFirstString);
    strcat(str, pSecondString);

    return convertToString(str);
}

/*
* this function concatenates 2 strings of type LPSTR (char *)
*/
std::string concatStrings(std::string sFirstString, char * pSecondString)
{
    char str[80];
    strcpy(str, sFirstString.c_str());
    strcat(str, pSecondString);

    return convertToString(str);
}

/*
* this function concatenates 3 strings of type LPSTR (char *)
*/
std::string concatStrings(char * pFirstString, std::string sSecondString, char * pThirdString)
{
    char str[80];
    strcpy(str, pFirstString);
    strcat(str, sSecondString.c_str());

    strcpy(str, sSecondString.c_str());
    strcat(str, pThirdString);

    return convertToString(str);
}
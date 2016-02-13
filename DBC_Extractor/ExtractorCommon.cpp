

#include "ExtractorCommon.h"



/**
*  This function searches for and opens the WoW exe file, using all known variations on its spelling
*
*  @RETURN pFile the pointer to the file, so that it can be worked on
*/
FILE* openWoWExe()
{
    FILE *pFile;
    const char* ExeFileName[] = { "WoW.exe", "Wow.exe", "wow.exe" };
    int iExeSpelling = 3; ///> WoW.exe (Classic, CATA), Wow.exe (TBC, MoP, WoD), wow.exe (WOTLK)

    /// loop through all possible file names
    for (int iFileCount = 0; iFileCount < iExeSpelling; iFileCount++)
    {
#ifdef WIN32
        if (fopen_s(&pFile, ExeFileName[iFileCount], "rb") == 0)
            return pFile; ///< successfully located the WoW executable
#else     
        if ((pFile = fopen(ExeFileName[iFileCount], "rb")))
            return pFile; ///< successfully located the WoW executable
#endif
    }

    return 0; ///< failed to locate WoW executable
}


/**
*  This function loads up a binary file (WoW executable), then searches for a specified 
*  group of hex values in order to locate and return the Build # of the file
* 
*  @PARAM sFilename is the filename of the WoW executable to be loaded
*  @RETURN iBuild the build number of the WoW executable, or 0 if failed
*/
int getBuildNumber()
{
    int iBuild = -1; ///< build version # of the WoW executable (returned value)

    bool bBuildFound = false;

    /// hex values of the text/bytes we need to search for:
    /// WoW [Rel
    int iHexValue_W = 0x57;
    int iHexValue_o = 0x6F;
    int iHexValue_space = 0x20;
    int iHexValue_OpeningBracket = 0x5B; // [ 
    int iHexValue_R = 0x52;
    int iHexValue_e = 0x65;
    int iHexValue_l = 0x6C;
    
    /// buffers used for working on the file's bytes
    unsigned char byteSearchBuffer[1]; ///< used for reading in a single character, ready to be 
                                       ///< tested for the required text we are searching for: "WoW [Rel"
    unsigned char jumpBytesBuffer[128]; ///< used for skipping past the bytes from the file's start
                                        ///< to the base # area, before we start searching for the base #, for faster processing
    unsigned char jumpBytesBuffer2[12]; ///< used for skipping past the bytes between the text being
                                        ///< searched for and the Base #, so that we can then get at the Base #
    unsigned char buildNumber[6]; ///< stored here prior to conversion to an integer

    FILE *pFile;
    if (!(pFile = openWoWExe()))
        return 0; ///> faled to locate exe file
    
    /// jump over as much of the file as possible, before we start searching for the base #
    for (int i = 0; i < 3300; i++)
        fread(jumpBytesBuffer, sizeof(jumpBytesBuffer), 1, pFile);

    /// Search for the build #
    while (!bBuildFound && fread(byteSearchBuffer, 1, 1, pFile))
    {
        /// find W
        if (byteSearchBuffer[0] == 0x57)
        {
            /// is the next byte an o
            fread(byteSearchBuffer, 1, 1, pFile);
            if (byteSearchBuffer[0] == iHexValue_o)
            {
                /// is the next byte a W
                fread(byteSearchBuffer, 1, 1, pFile);
                if (byteSearchBuffer[0] == iHexValue_W)
                {
                    /// is the next byte a space
                    fread(byteSearchBuffer, 1, 1, pFile);
                    if (byteSearchBuffer[0] == iHexValue_space)
                    {
                        /// is the next byte an open square bracket
                        fread(byteSearchBuffer, 1, 1, pFile);
                        if (byteSearchBuffer[0] == iHexValue_OpeningBracket)
                        {
                            /// is the next byte an R
                            fread(byteSearchBuffer, 1, 1, pFile);
                            if (byteSearchBuffer[0] == iHexValue_R)
                            {
                                /// is the next byte an e
                                fread(byteSearchBuffer, 1, 1, pFile);
                                if (byteSearchBuffer[0] == iHexValue_e)
                                {
                                    /// is the next byte an l
                                    fread(byteSearchBuffer, 1, 1, pFile);
                                    if (byteSearchBuffer[0] == iHexValue_l)
                                        bBuildFound = true; ///< we are at the Build # area
                                }
                            }
                        }
                    }
                }
            }
        }
    }    

    if (!bBuildFound)
    {
        /// close the file
        fclose(pFile); ///< housekeping
        return 0; ///< we reached the end of the file without locating the build #, exit funcion
    }

    /// grab data leading up to the build #
    fread(jumpBytesBuffer2, sizeof(jumpBytesBuffer2), 1, pFile);

    /// grab the bytes containing the number
    fread(buildNumber, sizeof(buildNumber), 1, pFile);

    /// place the build number into a string (easy conversion to int)
    std::stringstream ss;
    std::string sbuildNumber;
    ss << buildNumber[0] << buildNumber[1] << buildNumber[2] << buildNumber[3] << buildNumber[4];
    ss >> sbuildNumber;

    fclose(pFile); ///< housekeping

    /// convert build number into an int
    iBuild = atoi(sbuildNumber.c_str());

    return iBuild; ///< build # found
}
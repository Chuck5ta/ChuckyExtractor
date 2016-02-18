

#include "ExtractorCommon.h"



/**
*  This function searches for and opens the WoW exe file, using all known variations on its spelling
*
*  @RETURN pFile the pointer to the file, so that it can be worked on
*/
FILE* openWoWExe()
{
    FILE *pFile;
    const char* ExeFileName[] = { "WoW.exe", "Wow.exe", "wow.exe", "Wow-64.exe" }; 
    int iExeSpelling = 4; ///> WoW.exe (Classic, CATA), Wow.exe (TBC, MoP, WoD), wow.exe (WOTLK), Wow-64.exe (MoP and WoD)

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
    /// ] Build
    int iHexValue_ClosingBracket = 0x5D; // ]
    int iHexValue_space          = 0x20;
    int iHexValue_B              = 0x42;
    int iHexValue_u              = 0x75;
    int iHexValue_i              = 0x69;
    int iHexValue_l              = 0x6C;
    int iHexValue_d              = 0x64;
    
    /// buffers used for working on the file's bytes
    unsigned char byteSearchBuffer[1]; ///< used for reading in a single character, ready to be 
                                       ///< tested for the required text we are searching for: "] Build"
    unsigned char jumpBytesBuffer[256]; ///< used for skipping past the bytes from the file's start
                                        ///< to the base # area, before we start searching for the base #, for faster processing
    unsigned char buildNumber[6]; ///< stored here prior to conversion to an integer

    FILE *pFile;
    if (!(pFile = openWoWExe()))
        return 0; ///> faled to locate exe file
    
    /// jump over as much of the file as possible, before we start searching for the base #
    for (int i = 0; i < 1650; i++)
        fread(jumpBytesBuffer, sizeof(jumpBytesBuffer), 1, pFile);

    /// Search for the build #
    while (!bBuildFound && fread(byteSearchBuffer, 1, 1, pFile))
    {
        /// find ]
        if (byteSearchBuffer[0] == iHexValue_ClosingBracket)
        {
            /// is the next byte a space
            fread(byteSearchBuffer, 1, 1, pFile);
            if (byteSearchBuffer[0] == iHexValue_space)
            {
                /// is the next byte a B
                fread(byteSearchBuffer, 1, 1, pFile);
                if (byteSearchBuffer[0] == iHexValue_B)
                {
                    /// is the next byte a u
                    fread(byteSearchBuffer, 1, 1, pFile);
                    if (byteSearchBuffer[0] == iHexValue_u)
                    {
                        /// is the next byte an i
                        fread(byteSearchBuffer, 1, 1, pFile);
                        if (byteSearchBuffer[0] == iHexValue_i)
                        {
                            /// is the next byte an l
                            fread(byteSearchBuffer, 1, 1, pFile);
                            if (byteSearchBuffer[0] == iHexValue_l)
                            {
                                /// is the next byte a d
                                fread(byteSearchBuffer, 1, 1, pFile);
                                if (byteSearchBuffer[0] == iHexValue_d)
                                {
                                    /// is the next byte a space
                                    fread(byteSearchBuffer, 1, 1, pFile);
                                    if (byteSearchBuffer[0] == iHexValue_space)
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

/**
*  This function looks up the Core Version based in the found build Number
*
*  @RETURN iCoreNumber the build number of the WoW executable, or -1 if failed
*/
int getCoreNumber()
{
    return getCoreNumberFromBuild(getBuildNumber());
}

/**
*  This function looks up the Core Version based in the found build Number
*
*  @PARAM iBuildNumber is the build number of the WoW executable
*  @RETURN iCoreNumber the build number of the WoW executable, or -1 if failed
*/
int getCoreNumberFromBuild(int iBuildNumber)
{
    switch (iBuildNumber)
    {
    case 5875:  //CLASSIC
    case 6005:  //CLASSIC
    case 6141:  //CLASSIC
        return CLIENT_CLASSIC;
        break;
    case 8606:  //TBC
        return CLIENT_TBC;
        break;
    case 12340: //WOTLK
        return CLIENT_WOTLK;
        break;
    case 15595: //CATA
        return CLIENT_CATA;
        break;
    case 18414: //MOP
        return CLIENT_MOP;
        break;
    case 20726: //WOD
        return CLIENT_WOD;
        break;
    case 20740: //LEGION ALPHA
        return CLIENT_LEGION;
        break;

    default:
        return -1;
        break;
    }
}

/**
 * This function retrieves the locale of the WoW client 
 * e.g. enGB, enRU, enCN, enUS, etc
 *
 * @RETURN sLocale the locale of the game client
 */
std::string getLocale()
{
    std::string sLocale = "";
    /// open the Config.wtf file
    std::ifstream configFile("WTF/Config.wtf");
    std::string line;
    while (std::getline(configFile, line))
    {
        // look for the locale - search for "locale"
        int iLocaleFound = line.find("locale");
        if (iLocaleFound != std::string::npos)
        {
            /// locale line found
            /// grab the locale itself
            sLocale = line.substr(iLocaleFound + 8, 4);
            std::cout << "Locale found: " << sLocale << std::endl << std::endl;
            break;
        }
    }

    configFile.close();

    return sLocale;
}



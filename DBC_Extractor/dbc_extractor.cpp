/* *********************************************
 * DBC Extractor
 *
 * Author: Chuck E
 * Date: 11 Feb 2016
 *
 */

#include <iostream>
#include <StormLib.h>
#include "ExtractorCommon.h"


int main(int argc, char* arg[])
{
    // identify build of exe
    
    int iWoWExeBuildNumber = 0;

    if ((iWoWExeBuildNumber = getBuildNumber()) != NULL)
    {
        // output the build number of the WoW exe
        std::cout << "Exe build number: " << iWoWExeBuildNumber << std::endl << std::endl;

        // process MPQ file

        HANDLE hMPQArchiveFile;
        if (!SFileOpenArchive("dbc.MPQ", 0, 0, &hMPQArchiveFile))
            std::cout << "Balls, cannot locate the MPQ file!!!" << std::endl << std::endl; 
        else
            std::cout << "Yay, located the the MPQ file!!!" << std::endl << std::endl;

    }
    else // failed to locate the WoW executable
    {
        std::cout << "WoW exe file could not be found!!!" << std::endl << std::endl;
    }

    int input = 0;
    std::cin >> input;

    return 0;
}
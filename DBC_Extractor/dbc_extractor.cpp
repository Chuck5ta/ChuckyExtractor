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
    // identify the build of the exe
    
    int iWoWExeBuildNumber = 0;

    HANDLE hMPQArchiveFile;
    HANDLE hDBCFile;

    if ((iWoWExeBuildNumber = getBuildNumber()) != NULL)
    {
        // output the build number of the WoW exe
        std::cout << "Exe build number: " << iWoWExeBuildNumber << std::endl << std::endl;

        // process MPQ file

        if (!SFileOpenArchive("dbc.MPQ", 0, 0, &hMPQArchiveFile))
            std::cout << "Balls, cannot locate the MPQ file!!!" << std::endl << std::endl; 
        else
        {
            std::cout << "Yay, located the the MPQ file!!!" << std::endl << std::endl;

            // go through the archive and list each file
            SFILE_FIND_DATA pFile;
            if ((hDBCFile = SFileFindFirstFile(hMPQArchiveFile, "*.dbc", &pFile, 0)) != NULL)
            {
                std::cout << "File: " << pFile.cFileName << std::endl;
                std::cout << "=============" << std::endl;

                // grab the rest of the dbc files
                while (SFileFindNextFile(hDBCFile, &pFile) != NULL)
                {
                    std::cout << "File: " << pFile.cFileName << std::endl;
                
                    // write dbc file to disk

                }
            }
            else
            {
                std::cout << "PROBLEM! Archive is empty!!! " << std::endl;
            }
        }
    }
    else // failed to locate the WoW executable
    {
        std::cout << "WoW exe file could not be found!!!" << std::endl << std::endl;
    }

    int input = 0;
    std::cin >> input;

    // Cleanup and exit
    if (hMPQArchiveFile != NULL)
        CloseHandle(hMPQArchiveFile);

    return 0;
}
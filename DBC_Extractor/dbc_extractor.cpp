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
    HANDLE hDBCFirstFile;
    HANDLE hDBCNextFile;
    HANDLE hDiskFile;
    SFILE_FIND_DATA pFile;

    if ((iWoWExeBuildNumber = getBuildNumber()) != NULL)
    {
        // output the build number of the WoW exe
        std::cout << "Exe build number: " << iWoWExeBuildNumber << std::endl << std::endl;

        // process the dbc MPQ file

        if (!SFileOpenArchive("dbc.MPQ", 0, 0, &hMPQArchiveFile))
            std::cout << "Balls, cannot locate the MPQ file!!!" << std::endl << std::endl; 
        else
        {
            std::cout << "Yay, located the MPQ file!" << std::endl << std::endl;

            // go through the archive and list each file
            if ((hDBCFirstFile = SFileFindFirstFile(hMPQArchiveFile, "*.dbc", &pFile, 0)) != NULL)
            {
                std::cout << "File: " << pFile.szPlainName << std::endl;
                std::cout << "=============" << std::endl;
                // open the file
                if (SFileOpenFileEx(hMPQArchiveFile, pFile.cFileName, 0, &hDBCNextFile))
                {
                    std::cout << "Successfully openned the first file:  " << pFile.szPlainName << std::endl;
                    // create the file to be written to disk
                    if (hDiskFile = CreateFile(pFile.szPlainName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
                    {
                        std::cout << "successfully created disk file handle:  " << pFile.szPlainName << std::endl;
                        // write dbc file to disk

                        char  szBuffer[0x10000];

                        DWORD dwBytes = 1;
                                                
                        while (dwBytes > 0)
                        {
                            SFileReadFile(hDBCNextFile, szBuffer, sizeof(szBuffer), &dwBytes, NULL);
                            std::cout << "byte read from file: " << dwBytes << std::endl;
                            if(dwBytes > 0)
                                if (WriteFile(hDiskFile, szBuffer, dwBytes, &dwBytes, NULL))
                                    std::cout << "File extraction successful, file created:  " << pFile.cFileName << std::endl;
                                else
                                    std::cout << "Failed to write file to disk:  " << pFile.cFileName << std::endl;
                        }

                        // grab the rest of the dbc files
                        // ==============================

                        while (SFileFindNextFile(hDBCFirstFile, &pFile))
                        {
                            std::cout << "File: " << pFile.cFileName << std::endl;
                            // open the file
                            if (SFileOpenFileEx(hMPQArchiveFile, pFile.cFileName, 0, &hDBCNextFile))
                            {
                                std::cout << "Successfully openned file:  " << pFile.cFileName << std::endl;
                                if (hDiskFile = CreateFile(pFile.szPlainName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
                                {
                                    std::cout << "successfully created disk file handle:  " << pFile.cFileName << std::endl;
                                    // write dbc file to disk
                                    char  szBuffer[0x10000];

                                    DWORD dwBytes = 1;

                                    while (dwBytes > 0)
                                    {
                                        SFileReadFile(hDBCNextFile, szBuffer, sizeof(szBuffer), &dwBytes, NULL);
                                        if (dwBytes > 0)
                                        {
                                            if(WriteFile(hDiskFile, szBuffer, dwBytes, &dwBytes, NULL))
                                                std::cout << "File extraction successful, file created:  " << pFile.cFileName << std::endl;
                                            else
                                                std::cout << "Failed to write file to disk:  " << pFile.cFileName << std::endl;
                                        }
                                    }
                                }
                                else
                                    std::cout << "Failed to create disk file HANDLE:  " << pFile.cFileName << std::endl;
                            }
                            else
                                std::cout << "Failed to open file:  " << pFile.cFileName << std::endl;
                        }
                    }
                    else
                        std::cout << "Failed to create disk file HANDLE:  " << pFile.cFileName << std::endl;
                }
                else
                    std::cout << "Failed to open file:  " << pFile.cFileName << std::endl;
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

    std::cout << "ALL DONE!!!" << std::endl << std::endl;

    int input = 0;
    std::cin >> input;

    // Cleanup and exit
    if (hMPQArchiveFile != NULL)
        CloseHandle(hMPQArchiveFile);
    if (hDBCFirstFile != NULL)
        CloseHandle(hDBCFirstFile);
    if (hDBCNextFile != NULL)
        CloseHandle(hDBCNextFile);
 //   if (hDiskFile != NULL)
 //       SFileCloseFile(hDiskFile);
    
    return 0;
}
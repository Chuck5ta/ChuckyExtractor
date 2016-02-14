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


/*
 * This function extracts the DBC files from the dbc.MPQ archive and writes them to the disk as individual files.
 */
void processTheDBCMPQFile()
{
    HANDLE hMPQArchiveFile;
    HANDLE hDBCFirstFile;
    HANDLE hDBCNextFile;
    HANDLE hDiskFile;
    SFILE_FIND_DATA pFile;

    // open the MPQ archive, so that we can extract the dbc files
    if (!SFileOpenArchive("dbc.MPQ", 0, 0, &hMPQArchiveFile))
        std::cout << "Balls, cannot locate the dbc.MPQ file!!!" << std::endl << std::endl;
    else
    {
        std::cout << "Yay, located the dbc.MPQ file!" << std::endl << std::endl;

        // go through the archive and extract each file
        // ============================================

        // we need to deal with the first file first, then we can process each of the remaining files
        if ((hDBCFirstFile = SFileFindFirstFile(hMPQArchiveFile, "*.dbc", &pFile, 0)) != NULL)
        {
            std::cout << "File: " << pFile.szPlainName << std::endl;
            std::cout << "=============" << std::endl;
            // open the file in the MPQ archive
            if (SFileOpenFileEx(hMPQArchiveFile, pFile.cFileName, 0, &hDBCNextFile))
            {
                std::cout << "Successfully openned the first file:  " << pFile.szPlainName << std::endl;
                // create the file to be written to disk
                if (hDiskFile = CreateFile(pFile.szPlainName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
                {
                    std::cout << "Successfully created disk file handle:  " << pFile.szPlainName << std::endl;
                    // write dbc file to disk

                    char  szBuffer[0x10000];
                    DWORD dwBytes = 1;

                    // extract the file and write it to disk
                    while (dwBytes > 0)
                    {
                        // extract the MPQ archive file's contents
                        SFileReadFile(hDBCNextFile, szBuffer, sizeof(szBuffer), &dwBytes, NULL);
                        if (dwBytes > 0)
                            // write the contents to the new file on disk
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
                        // open the file in the MPQ archive
                        if (SFileOpenFileEx(hMPQArchiveFile, pFile.cFileName, 0, &hDBCNextFile))
                        {
                            std::cout << "Successfully openned file:  " << pFile.cFileName << std::endl;
                            // create the file on disk, so that we can write to it
                            if (hDiskFile = CreateFile(pFile.szPlainName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
                            {
                                std::cout << "Successfully created disk file handle:  " << pFile.cFileName << std::endl;

                                char  szBuffer[0x10000];
                                DWORD dwBytes = 1;

                                // extract the file and write it to disk
                                while (dwBytes > 0)
                                {
                                    // extract the MPQ archive file's contents
                                    SFileReadFile(hDBCNextFile, szBuffer, sizeof(szBuffer), &dwBytes, NULL);
                                    if (dwBytes > 0)
                                    {
                                        // write the contents to the new file on disk
                                        if (WriteFile(hDiskFile, szBuffer, dwBytes, &dwBytes, NULL))
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
                            std::cout << "Failed to open archive file:  " << pFile.cFileName << std::endl;
                    }
                }
                else
                    std::cout << "Failed to create disk file HANDLE:  " << pFile.cFileName << std::endl;
            }
            else
                std::cout << "Failed to open archived file:  " << pFile.cFileName << std::endl;
        }
        else
            std::cout << "PROBLEM! The dbc.MPQ archive is empty!!! " << std::endl;
    }

    // Cleanup and exit
    if (hMPQArchiveFile != NULL)
        CloseHandle(hMPQArchiveFile);
    if (hDBCFirstFile != NULL)
        CloseHandle(hDBCFirstFile);
    if (hDBCNextFile != NULL)
        CloseHandle(hDBCNextFile);
}

int main(int argc, char* arg[])
{
    // identify the build of the exe
    
    int iWoWExeBuildNumber = 0;
    
    if ((iWoWExeBuildNumber = getBuildNumber()) != NULL)
    {
        // output the build number of the WoW exe
        std::cout << "Exe build number: " << iWoWExeBuildNumber << std::endl << std::endl;

        // Grab the DBC files
        processTheDBCMPQFile();

    }
    else // failed to locate the WoW executable
        std::cout << "WoW exe file could not be found!!!" << std::endl << std::endl;

    std::cout << std::endl << "ALL DONE!!!" << std::endl << std::endl;

    int input = 0;
    std::cin >> input;
    
    return 0;
}
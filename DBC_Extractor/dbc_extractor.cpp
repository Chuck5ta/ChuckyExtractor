/* *********************************************
 * DBC Extractor
 *
 * Author: Chuck E
 * Date: 11 Feb 2016
 *
 */

#include "dbc_extractor.h"
 
/*
 * this function extracts a single DBC file from the dbc.MPQ archive and writes it to the disk
 *
 * @PARAM HANDLE hCurrentDBCFile;    // handle to the archived dbc file currently being processed
 * @PARAM HANDLE hDiskFile;          // handle to the dbc file that will be created on disk
 * @PARAM SFILE_FIND_DATA pFile;     // a pointer to the current dbc file in the dbc.MPQ archive
 *
 */
void extractDBCFile(HANDLE hCurrentDBCFile, HANDLE hDiskFile, SFILE_FIND_DATA pFile)
{
    char  szBuffer[0x10000];
    DWORD dwBytes = 1;

    // extract the file and write it to the disk
    while (dwBytes > 0)
    {
        // extract the dbc file's contents from the MPQ archive
        SFileReadFile(hCurrentDBCFile, szBuffer, sizeof(szBuffer), &dwBytes, NULL);
        if (dwBytes > 0)
            // write the contents to the new file on disk
            if (WriteFile(hDiskFile, szBuffer, dwBytes, &dwBytes, NULL))
                std::cout << "File extraction successful, creating file:  " << pFile.cFileName << std::endl;
            else
                std::cout << "Failed to write the file to the disk:  " << pFile.cFileName << std::endl;
    }
}


/*
 * This function extracts the DBC files from the dbc.MPQ archive and writes them to the disk as individual files.
 */
void processTheDBCMPQFile()
{
    HANDLE hMPQArchiveFile;    // handle to the dbc.MPQ archive
    HANDLE hDBCFirstFile;      // handle to the first file in the dbc.MPQ archive
    HANDLE hDBCNextFile;       // handle to each of the archived files after the first file
    HANDLE hDiskFile;          // handle to the dbc file that will be created on disk
    SFILE_FIND_DATA pFile;     // a pointer to the current dbc file in the dbc.MPQ archive

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

            // open the dbc file in the MPQ archive
            if (SFileOpenFileEx(hMPQArchiveFile, pFile.cFileName, 0, &hDBCNextFile))
            {
                std::cout << "Successfully openned the first file:  " << pFile.szPlainName << std::endl;

                //  create the file on disk, so that we can write to it
                if (hDiskFile = CreateFile(pFile.szPlainName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
                {
                    std::cout << "Successfully created the disk file's handle:  " << pFile.szPlainName << std::endl;

                    // extract and write dbc file to disk
                    extractDBCFile(hDBCNextFile, hDiskFile, pFile);

                    // grab the rest of the dbc files
                    // ==============================

                    while (SFileFindNextFile(hDBCFirstFile, &pFile))
                    {
                        std::cout << "File: " << pFile.cFileName << std::endl;

                        // open the dbc file in the MPQ archive
                        if (SFileOpenFileEx(hMPQArchiveFile, pFile.cFileName, 0, &hDBCNextFile))
                        {
                            std::cout << "Successfully openned the file:  " << pFile.cFileName << std::endl;
                            // create the file on disk, so that we can write to it
                            if (hDiskFile = CreateFile(pFile.szPlainName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
                            {
                                std::cout << "Successfully created the disk file's handle:  " << pFile.cFileName << std::endl;

                                // extract and write dbc file to disk
                                extractDBCFile(hDBCNextFile, hDiskFile, pFile);
                            }
                            else
                                std::cout << "Failed to create the disk file HANDLE:  " << pFile.cFileName << std::endl;
                        }
                        else
                            std::cout << "Failed to open the archive file:  " << pFile.cFileName << std::endl;
                    }
                }
                else
                    std::cout << "Failed to create the disk file HANDLE:  " << pFile.cFileName << std::endl;
            }
            else
                std::cout << "Failed to open the archived file:  " << pFile.cFileName << std::endl;
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
    if (hDiskFile != NULL)
        CloseHandle(hDiskFile);
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
        std::cout << "The WoW exe file could not be found!!!" << std::endl << std::endl;

    std::cout << std::endl << "ALL DONE!!!" << std::endl << std::endl;

    int input = 0;
    std::cin >> input;
    
    return 0;
}
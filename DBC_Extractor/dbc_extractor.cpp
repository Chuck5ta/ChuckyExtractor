/* *********************************************
 * DBC Extractor
 *
 * Author: Chuck E
 * Date: 11 Feb 2016
 *
 */

#include "dbc_extractor.h"
#include "TypeConversion.h"
 
/**
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


/**
 * This function extracts the DBC files from the dbc.MPQ archive and writes them to the disk as individual files.
 */
void processTheDBCMPQFile(std::string sLocale)
{
    HANDLE hMPQArchiveFile;    // handle to the dbc.MPQ archive
    HANDLE hDBCFirstFile;      // handle to the first file in the dbc.MPQ archive
    HANDLE hDBCNextFile;       // handle to each of the archived files after the first file
    HANDLE hDiskFile;          // handle to the dbc file that will be created on disk
    SFILE_FIND_DATA pFile;     // a pointer to the current dbc file in the dbc.MPQ archive

    // create the folder for the DBC files
    mkdir("./dbc");

    // open the MPQ archive, so that we can extract the dbc files
    if (!SFileOpenArchive("locale-enGB.MPQ", 0, 0, &hMPQArchiveFile))
        std::cout << "Balls, cannot locate the locale-enGB.MPQ file!!!" << std::endl << std::endl;
    else
    {
        std::cout << "Yay, located the MPQ file!" << std::endl << std::endl;

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

                if (hDiskFile = CreateFile(concatStrings(pDBCfolder, sLocale, pFile.szPlainName).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
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
                            if (hDiskFile = CreateFile(concatStrings(pDBCfolder, sLocale, pFile.szPlainName).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
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
                    // Cleanup and exit
                    if (hDBCNextFile != NULL)
                        CloseHandle(hDBCNextFile);
                    if (hDiskFile != NULL)
                        CloseHandle(hDiskFile);
                }
                else
                {
                    std::cout << "Failed to create the disk file HANDLE:  " << pFile.cFileName << std::endl;
                }
            }
            else
            {
                std::cout << "Failed to open the archived file:  " << pFile.cFileName << std::endl;
            }
            if (hDBCFirstFile != NULL)
                CloseHandle(hDBCFirstFile);
        }
        else
        {
            std::cout << "There were no files of the type found in the archive!!! " << std::endl;
        }
        // Cleanup and exit
        if (hMPQArchiveFile != NULL)
            CloseHandle(hMPQArchiveFile);
    }

}


/** 
 * This function
 *
 * @PARAM sFileType is the file to search for (dbc, adt, wmo, etc.)
 *
 */
void processMPQFile(std::string sFileType, std::string sDestinationFolder, std::string sLocale, std::string sMPQFileName)
{
    HANDLE hMPQArchiveFile;    // handle to the dbc.MPQ archive
    HANDLE hDBCFirstFile;      // handle to the first file in the dbc.MPQ archive
    HANDLE hDBCNextFile;       // handle to each of the archived files after the first file
    HANDLE hDiskFile;          // handle to the dbc file that will be created on disk
    SFILE_FIND_DATA pFile;     // a pointer to the current dbc file in the dbc.MPQ archive

    // open the MPQ archive, so that we can extract the dbc files
    if (!SFileOpenArchive(sMPQFileName.c_str(), 0, 0, &hMPQArchiveFile))
        std::cout << "Balls, cannot locate the " << sMPQFileName << " file!!!" << std::endl << std::endl;
    else
    {
        std::cout << "Yay, located the MPQ file!" << std::endl << std::endl;

        // go through the archive and extract each file
        // ============================================

        // we need to deal with the first file, then we can process each of the remaining files
        if ((hDBCFirstFile = SFileFindFirstFile(hMPQArchiveFile, sFileType.c_str(), &pFile, 0)) != NULL)
        {
            std::cout << "File: " << pFile.szPlainName << std::endl;
            std::cout << "=============" << std::endl;

            // open the dbc file in the MPQ archive
            if (SFileOpenFileEx(hMPQArchiveFile, pFile.cFileName, 0, &hDBCNextFile))
            {
                std::cout << "Successfully openned the first file:  " << pFile.szPlainName << std::endl;

                if (hDiskFile = CreateFile(concatStrings(sDestinationFolder, pFile.szPlainName).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
                {
                    std::cout << "Successfully created the disk file's handle:  " << pFile.szPlainName << std::endl;

                    // extract and write dbc file to disk
                    extractDBCFile(hDBCNextFile, hDiskFile, pFile);

                    if (hDiskFile != NULL)
                        CloseHandle(hDiskFile);

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
                            if (hDiskFile = CreateFile(concatStrings(pDBCfolder, pFile.szPlainName).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))
                            {
                                std::cout << "Successfully created the disk file's handle:  " << pFile.cFileName << std::endl;

                                // extract and write dbc file to disk
                                extractDBCFile(hDBCNextFile, hDiskFile, pFile);
                            }
                            else
                                std::cout << "Failed to create the disk file HANDLE:  " << pFile.cFileName << std::endl;

                            if (hDiskFile != NULL)
                                CloseHandle(hDiskFile);
                        }
                        else
                            std::cout << "Failed to open the archive file:  " << pFile.cFileName << std::endl;
                    }
                    // Cleanup and exit
                    if (hDBCNextFile != NULL)
                        CloseHandle(hDBCNextFile);
                }
                else
                {
                    std::cout << "Failed to create the disk file HANDLE:  " << pFile.cFileName << std::endl;
                }
            }
            else
            {
                std::cout << "Failed to open the archived file:  " << pFile.cFileName << std::endl;
            }
            if (hDBCFirstFile != NULL)
                CloseHandle(hDBCFirstFile);
        }
        else
        {
            std::cout << "There were no files of the type found in the archive!!! " << std::endl;
        }
        // Cleanup and exit
        if (hMPQArchiveFile != NULL)
            CloseHandle(hMPQArchiveFile);
    }
}

/**
 * This function starts off the processing of all of the MPQ files that hold DBC (.dbc), MAPS (.adt), VMAPS (.wom), and MMPAS files
 *
 * @PARAM iCore the game version of the exe (Classic, TBC, WOTLK, etc)
 * @PARAM sLocale the locale of the client (enUS, enFR, enGB, etc.)
 */
void processTheMPQFiles(int iCore, std::string sLocale)
{
    // process the DBC files
    mkdir("./dbc");

    if (iCore == CLIENT_CLASSIC) // Classic
    {
        processMPQFile("*.dbc", pDBCfolder, sLocale, "./Data/dbc.MPQ");
        // apply patches
        processMPQFile("*.dbc", pDBCfolder, sLocale, "./Data/patch.MPQ");
        processMPQFile("*.dbc", pDBCfolder, sLocale, "./Data/patch-2.MPQ");
    }

    if (iCore >= CLIENT_TBC) // locale in use for folder and file name
    {
        processMPQFile("*.dbc", pDBCfolder, sLocale, "./Data/" + sLocale + "/locale-" + sLocale + ".MPQ");
    }

    std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;

    if (iCore >= CLIENT_TBC && iCore <= CLIENT_WOTLK)
    {
        // apply patches
        processMPQFile("*.dbc", pDBCfolder, sLocale, "./Data/" + sLocale + "/patch-" + sLocale + ".MPQ");
        processMPQFile("*.dbc", pDBCfolder, sLocale, "./Data/" + sLocale + "/patch-" + sLocale + "-2.MPQ");
    }

    std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;

    if (iCore == CLIENT_WOTLK)
    {
        processMPQFile("*.dbc", pDBCfolder, sLocale, "./Data/" + sLocale + "/patch-" + sLocale + "-3.MPQ");
    }

    // process MAP files
    // process VMAP files
    // process MMAP files
}


int main(int argc, char* arg[])
{
    // identify the build of the exe

    int iWoWExeBuildNumber = 0;
    std::string sLocale;

    if ((iWoWExeBuildNumber = getBuildNumber()) != NULL)
    {
        // output the build number of the WoW exe
        std::cout << "Exe build number: " << iWoWExeBuildNumber << std::endl << std::endl << std::endl << std::endl;

        sLocale = getLocale();

        // output the build number of the WoW exe
        std::cout << "Returned locale: " << sLocale << std::endl << std::endl;
        int iCore = getCoreNumber();
        std::cout << "Returned core: " << iCore << std::endl << std::endl;

        // make sure we have a valid locale if the core is not Classic
        // this is required for accessing the locale folder and certain files.
        if (iCore != CLIENT_CLASSIC && sLocale == "")
        {
            std::cout << "ERROR: Unable to identify your client's locale! " << std::endl;
            std::cout << "You must have a Config.wtf file in the WTF folder with the line:" << std::endl;
            std::cout << "SET locale <your locale>" << std::endl << std::endl;
            std::cout << "<your locale> will be in quotes and be something like enCN, enUS, enRU, etc." << std::endl << std::endl;

            // an error occured, exit the program
            return 0;
        }
        
        // we can now deal with all the required MPQ files for DBC, MAPS, VMAPS, and MMAPS
        processTheMPQFiles(iCore, sLocale);

    }
    else // failed to locate the WoW executable
        std::cout << "The WoW exe file could not be found!!!" << std::endl << std::endl;

    std::cout << std::endl << "ALL DONE!!!" << std::endl << std::endl;

    int input = 0;
    std::cin >> input;

    return 0;
}

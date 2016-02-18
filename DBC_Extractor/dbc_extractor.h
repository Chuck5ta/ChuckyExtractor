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
#include <direct.h>


char * pDBCfolder = "./dbc/";
char * pMAPSfolder = "./maps/";
char * pVMAPSfolder = "./vmaps/";
char * pMMAPSfolder = "./mmaps/";

// array of folder paths

// Classic



/*
 * this function extracts a single DBC file from the dbc.MPQ archive and writes it to the disk
 *
 * @PARAM HANDLE hCurrentDBCFile;    // handle to the archived dbc file currently being processed
 * @PARAM HANDLE hDiskFile;          // handle to the dbc file that will be created on disk
 * @PARAM SFILE_FIND_DATA pFile;     // a pointer to the current dbc file in the dbc.MPQ archive
 *
 */
void extractDBCFile(HANDLE hCurrentDBCFile, HANDLE hDiskFile, SFILE_FIND_DATA pFile);

void addPatchFiles(int iCore, HANDLE hMPQArchiveFile, std::string sLocale);

/*
 * This function extracts the DBC files from the dbc.MPQ archive and writes them to the disk as individual files.
 */
void processTheDBCMPQFile();

void processMPQFile(int iCore, std::string sFileType, std::string sDestinationFolder, std::string sLocale, std::string sMPQFileName);

void processMPQFiles(int core, std::string sLocale);





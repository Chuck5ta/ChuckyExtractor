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
 * this function extracts a single DBC file from the dbc.MPQ archive and writes it to the disk
 *
 * @PARAM HANDLE hCurrentDBCFile;    // handle to the archived dbc file currently being processed
 * @PARAM HANDLE hDiskFile;          // handle to the dbc file that will be created on disk
 * @PARAM SFILE_FIND_DATA pFile;     // a pointer to the current dbc file in the dbc.MPQ archive
 *
 */
void extractDBCFile(HANDLE hCurrentDBCFile, HANDLE hDiskFile, SFILE_FIND_DATA pFile);

/*
 * This function extracts the DBC files from the dbc.MPQ archive and writes them to the disk as individual files.
 */
void processTheDBCMPQFile();





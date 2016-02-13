/* *********************************************
 * DBC Extractor
 *
 * Author: Chuck E
 * Date: 11 Feb 2016
 *
 */

#include <iostream>
#include <StormLib.h>

int main(int argc, char* arg[])
{
    HANDLE hMpqFile;
    if (!SFileOpenArchive("dbc.MPQ", 0, 0, &hMpqFile))
        std::cout << "Balls, cannot locate the MPQ file!!!" << std::endl << std::endl; 
    else
        std::cout << "Yay, located the the MPQ file!!!" << std::endl << std::endl;

    std::cin;

    return 0;
}

#include <stdio.h>
#include <sstream>
#include <fstream>  // getline
#include <iostream> // cout

/// Enumerated Core Numbers
enum CoreNumber
{
    CLIENT_CLASSIC = 0,
    CLIENT_TBC = 1,
    CLIENT_WOTLK = 2,
    CLIENT_CATA = 3,
    CLIENT_MOP = 4,
    CLIENT_WOD = 5,
    CLIENT_LEGION = 6
};

FILE* openWoWExe();
int getBuildNumber();
int getCoreNumber();
int getCoreNumberFromBuild(int iBuildNumber);
std::string getLocale();




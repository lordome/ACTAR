#ifndef cUtilsFunctions_hpp
#define cUtilsFunctions_hpp

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "TString.h"

void getInputMap(std::string &inputFileName, std::map<std::string, double> &parMap, TString &dataFileName)
{

    std::ifstream inFile(inputFileName);

    if (!inFile)
    {
        std::cout << "\nError opening file.\n";
        return;
    }

    std::string Var_Name, dataFileString;
    double Var_Value;

    inFile >> Var_Name >> dataFileName;
    
    while (inFile >> Var_Name >> Var_Value)
    {
        parMap.insert(make_pair(Var_Name, Var_Value));
        std::cout << Var_Name << "  " << Var_Value << std::endl;
    }

    inFile.close();

    return;
}

#endif
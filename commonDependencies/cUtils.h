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

    std::string varName, dataFileString;
    double varValue;

    inFile >> varName >> dataFileName;
    
    while (inFile >> varName >> varValue)
    {
        parMap.insert(make_pair(varName, varValue));
        std::cout << varName << "  " << varValue << std::endl;
    }

    inFile.close();

    return;
}

#endif
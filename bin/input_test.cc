#include <iostream>
#include <fstream>
#include <map>
using namespace std;

int read_file(string input_file = "")
{

    ifstream inFile(input_file);

    string Var_Name;
    double Var_Value;
    map<string, int> mymap;

    string inp_n;
    string data_file_name;

    if (!inFile)
    {
        std::cout << "\nError opening file.\n";
        return 13;
    }

    inFile >> inp_n >> data_file_name;

    while (inFile >> Var_Name >> Var_Value)
    {
        std::cout << "reading" << Var_Name << "  " << Var_Value << std::endl;
        mymap.insert(make_pair(Var_Name, Var_Value));
    }

    for (auto it = mymap.cbegin(); it != mymap.cend(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }
    std::cout << inp_n << " " << data_file_name << std::endl;

    inFile.close();
    return 0;
}
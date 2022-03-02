#include <string>
#include <vector>

#ifndef cUserInput_h
#define cUserInput_h

/**
This class is used to store the instructions given by the user
*/

class cUserInput {
  private:
    cUserInput() {}
    virtual ~cUserInput() {}

    std::vector<std::string> userInst;

  public:
    static void setUserInput(int argc, char const *argv[]);
    static cUserInput* getInstance();

    static std::string getArgs(std::string);
    static bool isSet(std::string);
};

#endif

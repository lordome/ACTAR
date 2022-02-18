#include "cUserInput.h"
#include <algorithm>

using namespace std;

void cUserInput::setUserInput(int argc, char const *argv[]) {
  cUserInput *p = getInstance();

  for (int i = 0; i < argc; i++) {
    p->userInst.push_back(argv[i]);
  }
}

cUserInput* cUserInput::getInstance() {
  static cUserInput cui;

  return &cui;
}

string cUserInput::getArgs(string label) {
  cUserInput *p = getInstance();

  auto ien = p->userInst.begin();
  auto ist = p->userInst.end();

  for (; ien != ist; ien++) {
    if (*ien == label) return *(ien + 1);;
  }

  return "";
}

bool cUserInput::isSet(string label) {
  cUserInput *p = getInstance();
  
  auto ien = p->userInst.begin();
  auto ist = p->userInst.end();

  if (find(ien, ist, label) != ist) return true;
  else return false;
}

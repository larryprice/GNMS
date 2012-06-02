#include "CApp.hpp"

int main(int argc, char** argv) {
  CApp theApp;
  int retCode = 0;

  retCode = theApp.onExecute();

  return retCode;
}

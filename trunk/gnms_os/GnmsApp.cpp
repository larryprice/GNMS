#include "GnmsApp.hpp"

GnmsApp::GnmsApp() : CEntity::CEntity() {
  //  theTile = NULL;
  theExe = "";
}

GnmsApp::GnmsApp(float x, float y, string name) : CEntity::CEntity(x, y) {
  theExe = "";
  theName = name;
}

void GnmsApp::setExe(std::string exeName) {
  theExe = exeName;

  return;
}

std::string GnmsApp::getExe() {
  return theExe;
}

void GnmsApp::setName(std::string name) {
  theName = name;
  return;
}

std::string GnmsApp::getName() {
  return theName;
}

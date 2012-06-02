#include "TouchApp.hpp"

TouchApp* TouchApp::sApp = new TouchApp();

TouchApp::TouchApp() {
  tuioClient = new TuioClient(3333);
  tuioClient->addTuioListener(this);
  tuioClient->connect();

  if (tuioClient->isConnected() == false) {
    cerr << "TUIO failed to connect" << endl;
  }
  gnmsCursors.clear();
  mru = -1;
}

TouchApp::~TouchApp() {
  tuioClient->disconnect();
  delete tuioClient;
}

GnmsCursor* TouchApp::getMostRecentCursor() {
  if (mru >= 0) {
    return gnmsCursors[mru];
  } else {
    return NULL;
  }
}

void TouchApp::ageMostRecentCursor() {
  aged = true;
}

bool TouchApp::getAge() {
  return aged;
}

void TouchApp::addTuioObject(TuioObject *tobj) {
  // empty
}

void TouchApp::updateTuioObject(TuioObject *tobj) {
  // empty
}

void TouchApp::removeTuioObject(TuioObject *tobj) {
  // empty
}

void TouchApp::addTuioCursor(TuioCursor *tcur) {
  // only track the first cursor on the screen
  mru = tcur->getCursorID();
  aged = false;
  gnmsCursors[tcur->getCursorID()] = 
    new GnmsCursor(tcur->getX()*SCREEN_WIDTH, tcur->getY()*SCREEN_HEIGHT);
}

void TouchApp::updateTuioCursor(TuioCursor *tcur) {
  // watch the update to see if it's up/down
  mru = tcur->getCursorID();
  gnmsCursors[tcur->getCursorID()]->
    addCoordinate(tcur->getX()*SCREEN_WIDTH, tcur->getY()*SCREEN_HEIGHT);
}

void TouchApp::removeTuioCursor(TuioCursor *tcur) {
  // Tell the cursor it's been stopped
  // and add it's final coordinates
  mru = tcur->getCursorID();
  if (gnmsCursors[tcur->getCursorID()] != NULL) {
    gnmsCursors[tcur->getCursorID()]->
      stopCursor(tcur->getX()*SCREEN_WIDTH, tcur->getY()*SCREEN_HEIGHT);
  }
}

void TouchApp::refresh(TuioTime frameTime) {
  // empty
}

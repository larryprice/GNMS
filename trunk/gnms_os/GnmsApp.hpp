#ifndef _GNMS_APP_HPP_
#define _GNMS_APP_HPP_

/* this class is for GNMS Apps;
   it includes a Surface consisting of the tile image
   and a link to the executable for the app
*/

#include <string>
#include <SDL/SDL.h>
#include "CEntity.hpp"

class GnmsApp : public CEntity {
public:
  GnmsApp();
  GnmsApp(float x, float y, string name="");

  void setExe(std::string );
  std::string getExe();

  void setName(std::string );
  std::string getName();

private:
  std::string  theExe;
  std::string  theName;
};

#endif

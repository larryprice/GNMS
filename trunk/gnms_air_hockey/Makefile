HOCKEY_LIBS=-lSDL_image -lSDL_mixer -lTUIO -lpthread
HOCKEY_FLAGS=`sdl-config --cflags --libs`
DEBUG_FLAGS="-ggdb"
APP_NAME="bin/gnms_air_hockey"

default : hockey
	@:

debug : 
	@mkdir -p bin/
	@g++ -o $(APP_NAME) $(DEBUG_FLAGS) ./*.cpp $(HOCKEY_FLAGS) $(HOCKEY_LIBS)

hockey : hockey.o
	@mkdir -p bin/
	@g++ -o $(APP_NAME) ./*.o $(HOCKEY_FLAGS) $(HOCKEY_LIBS) 

hockey.o : hockey.cpp CApp.o
	@g++ -c hockey.cpp

CApp.o : CApp.cpp CApp.hpp Define.hpp CSurface.o CPlayer.o \
		CEvent.o CAnimate.o CEntity.o CArea.o CCamera.o \
	     	CPuck.o GnmsCursor.o
	@g++ -c CApp.cpp -DHOCKEY_DIR=\"$(CURDIR)\"

CSurface.o : CSurface.cpp CSurface.hpp
	@g++ -c CSurface.cpp

CEvent.o : CEvent.cpp CEvent.hpp
	@g++ -c CEvent.cpp

CAnimate.o : CAnimate.cpp CAnimate.hpp
	@g++ -c CAnimate.cpp

CEntity.o : CEntity.cpp CEntityCol.cpp CAnimate.o CSurface.o \
                  CArea.o CCamera.o CFPS.o
	@g++ -c CEntity.cpp
	@g++ -c CEntityCol.cpp

CTile.o : CTile.cpp CTile.hpp Define.hpp
	@g++ -c CTile.cpp

CArea.o : CArea.cpp CArea.hpp Define.hpp CMap.o
	@g++ -c CArea.cpp -DHOCKEY_DIR=\"$(CURDIR)\"

CMap.o : CMap.cpp CMap.hpp Define.hpp CTile.o CSurface.o
	@g++ -c CMap.cpp

CCamera.o : CCamera.cpp CCamera.hpp Define.hpp
	@g++ -c CCamera.cpp

CFPS.o : CFPS.cpp CFPS.hpp
	@g++ -c CFPS.cpp

CPlayer.o : CPlayer.cpp CPlayer.hpp CEntity.o
	@g++ -c CPlayer.cpp

CPuck.o : CPuck.cpp CPuck.hpp CPlayer.o
	@g++ -c CPuck.cpp

GnmsCursor.o : GnmsCursor.cpp GnmsCursor.hpp
	@g++ -c GnmsCursor.cpp

clean : 
	@rm -f ./*.o
	@rm -f $(APP_NAME)


GNMS_LIBS=-lSDL_image -lSDL_mixer -lTUIO -lpthread
GNMS_FLAGS=`sdl-config --cflags --libs`
DEBUG_FLAGS="-ggdb"
APP_DIR="bin/gnms_paint"

default : gnms_paint
	@:

debug : 
	mkdir -p $(APP_DIR)
	@g++ -o $(APP_DIR) $(DEBUG_FLAGS) ./*.cpp $(GNMS_FLAGS) $(GNMS_LIBS)

gnms_paint : gnms_paint.o
	@mkdir -p bin/
	@g++ -o $(APP_DIR) ./*.o $(GNMS_FLAGS) $(GNMS_LIBS)

gnms_paint.o : gnms_paint.cpp CApp.o
	@g++ -c gnms_paint.cpp

CApp.o : CApp.cpp CApp.hpp Define.hpp CSurface.o CPlayer.o \
		CEvent.o CAnimate.o CEntity.o CArea.o \
		CCamera.o GnmsCursor.o
	@g++ -c CApp.cpp -DPAINT_DIR=\"`pwd`\"

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
	@g++ -c CArea.cpp -DPAINT_DIR=\"`pwd`\"

CMap.o : CMap.cpp CMap.hpp Define.hpp CTile.o CSurface.o
	@g++ -c CMap.cpp -DPAINT_DIR=\"`pwd`\"

CCamera.o : CCamera.cpp CCamera.hpp Define.hpp
	@g++ -c CCamera.cpp

CFPS.o : CFPS.cpp CFPS.hpp
	@g++ -c CFPS.cpp

CPlayer.o : CPlayer.cpp CPlayer.hpp CEntity.o
	@g++ -c CPlayer.cpp

GnmsCursor.o : GnmsCursor.cpp GnmsCursor.hpp
	@g++ -c GnmsCursor.cpp

clean : 
	@rm -f ./*.o
	@rm -f $(APP_DIR)


GNMS_LIBS=-lSDL_image -lTUIO -lpthread
GNMS_FLAGS=`sdl-config --cflags --libs`
DEBUG_FLAGS="-ggdb"

default : gnms_os
	@:

debug : 
	@g++ -o gnms_os $(DEBUG_FLAGS) ./*.cpp $(GNMS_FLAGS) $(GNMS_LIBS) -DOS_DIR=\"`pwd`\"

gnms_os : gnms_os.o
	@g++ -o gnms_os ./*.o $(GNMS_FLAGS) $(GNMS_LIBS)

gnms_os.o : gnms_os.cpp CApp.o
	@g++ -c gnms_os.cpp

CApp.o : CApp.cpp CApp.hpp Define.hpp CSurface.o CPlayer.o \
		CEvent.o CAnimate.o CEntity.o CArea.o \
		CCamera.o GnmsApp.o GnmsCursor.o
	@g++ -c CApp.cpp -DOS_DIR=\"`pwd`\"

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
	@g++ -c CArea.cpp -DOS_DIR=\"`pwd`\"

CMap.o : CMap.cpp CMap.hpp Define.hpp CTile.o CSurface.o
	@g++ -c CMap.cpp

CCamera.o : CCamera.cpp CCamera.hpp Define.hpp
	@g++ -c CCamera.cpp

CFPS.o : CFPS.cpp CFPS.hpp
	@g++ -c CFPS.cpp

CPlayer.o : CPlayer.cpp CPlayer.hpp CEntity.o
	@g++ -c CPlayer.cpp

GnmsApp.o : GnmsApp.cpp GnmsApp.hpp CEntity.o
	@g++ -c GnmsApp.cpp

GnmsCursor.o : GnmsCursor.cpp GnmsCursor.hpp
	@g++ -c GnmsCursor.cpp

clean : 
	@rm -f ./*.o
	@rm -f ./gnms_os


GNMS_LIBS=-lSDL_image -lSDL_mixer -lTUIO
GNMS_FLAGS=`sdl-config --cflags --libs`
DEBUG_FLAGS="-ggdb"
APP_DIR="bin/gnms_matching"

default : gnms_matching
	@:

debug :
	@g++ -o matching $(DEBUG_FLAGS) ./*.cpp $(GNMS_FLAGS) $(GNMS_LIBS) -DMATCH_DIR=\"`pwd`\"

gnms_matching : gnms_matching.o
	@mkdir -p bin/
	@g++ -o $(APP_DIR) ./*.o $(GNMS_FLAGS) $(GNMS_LIBS)

gnms_matching.o : gnms_matching.cpp CApp.o
	@g++ -c gnms_matching.cpp

CApp.o : CApp.cpp CApp.hpp Define.hpp CSurface.o \
		CEvent.o CAnimate.o CEntity.o \
		GnmsCursor.o
	@g++ -c CApp.cpp -DMATCH_DIR=\"`pwd`\"

CSurface.o : CSurface.cpp CSurface.hpp
	@g++ -c CSurface.cpp

CEvent.o : CEvent.cpp CEvent.hpp
	@g++ -c CEvent.cpp

CAnimate.o : CAnimate.cpp CAnimate.hpp
	@g++ -c CAnimate.cpp

CEntity.o : CEntity.cpp CEntityCol.cpp CAnimate.o CSurface.o 
	@g++ -c CEntity.cpp
	@g++ -c CEntityCol.cpp

GnmsCursor.o : GnmsCursor.cpp GnmsCursor.hpp
	@g++ -c GnmsCursor.cpp

clean : 
	@rm -f ./*.o
	@rm -f $(APP_DIR)


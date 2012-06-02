HOCKEY_LIBS=-lSDL_image -lSDL_mixer -lTUIO -lpthread
HOCKEY_FLAGS=`sdl-config --cflags --libs`
DEBUG_FLAGS="-ggdb"
TTT_APP="bin/gnms_tic_tac_toe"

default : ttt
	@:

debug : 
	@mkdir -p bin/
	@g++ -o $(TTT_APP) $(DEBUG_FLAGS) ./*.cpp $(HOCKEY_FLAGS) $(HOCKEY_LIBS) -DTTT_DIR=\"`pwd`\"

ttt : gnms_tic_tac_toe.o
	@mkdir -p bin/
	@g++ -o $(TTT_APP) ./*.o $(HOCKEY_FLAGS) $(HOCKEY_LIBS)

gnms_tic_tac_toe.o : gnms_tic_tac_toe.cpp CApp.o
	@g++ -c gnms_tic_tac_toe.cpp

CApp.o : CApp.cpp CApp.hpp Define.hpp CSurface.o CEvent.o	\
	CApp_OnCleanup.cpp CApp_OnEvent.cpp CApp_OnInit.cpp	\
	CApp_OnRender.cpp CApp_OnLoop.cpp GnmsCursor.o
	@g++ -c CApp.cpp CApp_OnCleanup.cpp CApp_OnEvent.cpp	\
	CApp_OnInit.cpp CApp_OnRender.cpp CApp_OnLoop.cpp -DTTT_DIR=\"`pwd`\"

CSurface.o : CSurface.cpp CSurface.hpp
	@g++ -c CSurface.cpp

CEvent.o : CEvent.cpp CEvent.hpp
	@g++ -c CEvent.cpp

GnmsCursor.o : GnmsCursor.cpp GnmsCursor.hpp
	@g++ -c GnmsCursor.cpp

clean : 
	@rm -f ./*.o
	@rm -f $(TTT_APP)
ROULETTE_LIBS=-lSDL_image -lSDL_mixer -lTUIO
SDL_FLAGS=`sdl-config --cflags --libs`
DEBUG_FLAGS="-ggdb"
APP_LOCATION="bin/gnms_roulette"

default : roulette

debug : 
	@g++ -o roulette $(DEBUG_FLAGS) ./*.cpp $(SDL_FLAGS) $(ROULETTE_LIBS) -DROULETTE_DIR=\"`pwd`\"

roulette : main.o
	@mkdir -p bin/
	@g++ -o $(APP_LOCATION) ./*.o $(SDL_FLAGS) $(ROULETTE_LIBS)

main.o : main.cpp roulette.o
	@g++ -c main.cpp -DROULETTE_DIR=\"`pwd`\"

roulette.o : roulette.cpp roulette.hpp TouchApp.o GnmsCursor.o
	@g++ -c roulette.cpp -DROULETTE_DIR=\"`pwd`\"

TouchApp.o : TouchApp.cpp TouchApp.hpp roulette.hpp GnmsCursor.o
	@g++ -c TouchApp.cpp

GnmsCursor.o : GnmsCursor.cpp GnmsCursor.hpp
	@g++ -c GnmsCursor.cpp

clean :
	@rm ./*.o
	@rm -rf $(APP_LOCATION)

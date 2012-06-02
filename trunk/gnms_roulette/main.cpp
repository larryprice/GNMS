

#include "roulette.hpp"

int main() {
  int playAgain = 1;
  
  Uint32 waitTime = 1000.0f/30;
  Uint32 frameStartTime = 0;
  Sint32 delayTime;
  Uint32 idleTime = 0;

  Mix_Chunk* welcome   = NULL;
  Mix_Chunk* spinNoise = NULL;

  bool running = true;

  if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096)) {
    cerr << "Unable to open audio" << endl;
    return false;
  }

  string temp;
  temp      = string(ROULETTE_DIR)+
    string("/sounds/welcome.wav");
  welcome   = Mix_LoadWAV(temp.c_str());
  temp      = string(ROULETTE_DIR)+
    string("/sounds/roulette_spin.wav");
  spinNoise = Mix_LoadWAV(temp.c_str());

  Mix_PlayChannel(-1, welcome, 0);

  while (playAgain == 1 && running == true) {

    SDL_Surface *screen = startupInit();

    int goodSpin = 0;
    int last_spin = -1;
    int spinningWheel = 0;

    int userInput[3] = {1,0,0};
    float dragDistance = 0;
    int startPress[2] = {0,0};

    int betSelection = -1;
    int numSelection = -1;
    int padNumber = 0;
    int playerCash = 2000;

    displayDigits(playerCash, screen, 0);

    int exit = 0;

    while (userExit(userInput) && playerCash > 0 && running == true) {

      delayTime = waitTime - (SDL_GetTicks() - frameStartTime);

      if (isRouletteWheel(userInput) == 1) {
	if (padNumber == 0) {
	  displayBetMoney(screen);
	}
	else if (betSelection == -1) {
	  displayChooseNumber(screen);
	}
	else if (numSelection != 11) {
	  displayPressEnter(screen);
	}
	else if (padNumber > playerCash) {
	  displayDontHaveCash(screen);
	}
	else {
	  clearText(screen);
	  spinningWheel = 1;
	  goodSpin = 1;
	  copyCoordinates(startPress, userInput);
	}
      }
      else if (spinningWheel && !isRouletteWheel(userInput) 
	       && userInput[0] ==3) {
	goodSpin = 0;
      }
      else if (spinningWheel && userInput[0] == 2 &&
	       (!goodSpin ||
		!calculateDistance(startPress[0], startPress[1],
				   userInput[1],userInput[2]))) {
	userInput[0] = 3;
	spinningWheel = 0;
      }
      else if (spinningWheel && goodSpin && isRouletteWheel(userInput) == 2) {
	Mix_PlayChannel(-1, spinNoise, 0);
	last_spin = spinRouletteWheel(calculateDistance(startPress[0], startPress[1], userInput[1], userInput[2]), screen, last_spin);
	playerCash = playerCash + (isWinningBet(last_spin, betSelection) * padNumber);
	clearMoneyBar(screen);
	clearRouletteWheel(screen);
	displayDigits(playerCash, screen, 0);

	spinningWheel = 0;
      }
      if (!spinningWheel) {
	betSelection = displayBetTable(userInput, screen,betSelection);
				
	numSelection = numberPadDown(userInput, screen, numSelection, padNumber);
	padNumber = numberPadUp(userInput, screen, numSelection, padNumber);
	userInput[0] = 3;
      }

      if (getUserInput(userInput) == false) {
	running  = false;
      }
      if (running == false) {
	break;
      }
      if(delayTime > 0) {
	SDL_Delay((Uint32)delayTime);
      }
      frameStartTime = SDL_GetTicks();
    }

    playAgain = -1;

    if (playerCash <= 0) {
      displayPlayAgainSplash(screen);
      while (playAgain == -1) {
	delayTime = waitTime - (SDL_GetTicks() - frameStartTime);

	getUserInput(userInput);
	playAgain = playAgainYesNo(userInput[1],userInput[2]);
	
	if(delayTime > 0) {
	  SDL_Delay((Uint32)delayTime);
	}
	frameStartTime = SDL_GetTicks();
      }
    }

    if (playAgain < 1) {
      displayCloseSplash(screen);
    } 

  }

  Mix_FreeChunk(welcome);
  Mix_FreeChunk(spinNoise);

  SDL_Quit();

  return 0;
}

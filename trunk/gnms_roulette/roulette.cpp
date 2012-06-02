#include "roulette.hpp"

int playAgainYesNo(int x, int y) {
  if (x >= 268 && x <= 430 && y >= 636 && y <= 715) {
    return 1;
  } else if (x >= 855 && x <= 993 && y >= 639 && y <= 711) {
    return 0;
  }
  return -1;
}

/* 
   MAPPING OF THE CATEGORY LOOKUP
   [0] First 12, [1] Second 12, [2] Third 12, 
   [3] First 18, [4], Second 18, [5] Even,
   [6] Odd, [7] Black, [8] Red, [9] First Column,
   [10] Second Column, [11], Third Column

  */
int isWinningBet(int winning_spin_num, int bet_selection) {
  int table_lookup[ROULETTE_WHEEL_NUM_OPTIONS] = 
    {37, 27, 10, 25, 29, 12, 8, 19, 31, 18, 6,
     21, 33, 16, 4, 23, 35, 14, 2, 0, 28, 9, 26,
     30, 11, 7, 20, 32, 17, 5, 22, 34, 15, 3, 24, 36, 13, 1};
  int categorie_lookup[NUM_BET_CATEGORIES][NUM_BET_CATEGORIE_MAX + 1];

  int black[NUM_BET_CATEGORIE_MAX] =
    {2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35};
  int red[NUM_BET_CATEGORIE_MAX] =
    {1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36};
	

  int betCtr;
  winning_spin_num = table_lookup[winning_spin_num % ROULETTE_WHEEL_NUM_OPTIONS];

  categorie_lookup[0][NUM_BET_CATEGORIE_MAX] = 2;
  categorie_lookup[1][NUM_BET_CATEGORIE_MAX] = 2;
  categorie_lookup[2][NUM_BET_CATEGORIE_MAX] = 2;
  categorie_lookup[3][NUM_BET_CATEGORIE_MAX] = 1;
  categorie_lookup[4][NUM_BET_CATEGORIE_MAX] = 1;
  categorie_lookup[5][NUM_BET_CATEGORIE_MAX] = 1;
  categorie_lookup[6][NUM_BET_CATEGORIE_MAX] = 1;
  categorie_lookup[7][NUM_BET_CATEGORIE_MAX] = 1;
  categorie_lookup[8][NUM_BET_CATEGORIE_MAX] = 1;
  categorie_lookup[9][NUM_BET_CATEGORIE_MAX] = 2;
  categorie_lookup[10][NUM_BET_CATEGORIE_MAX] = 2;
  categorie_lookup[11][NUM_BET_CATEGORIE_MAX] = 2;
	
  for (betCtr=1; betCtr<=NUM_BET_CATEGORIE_MAX; betCtr++) {
    if (betCtr <= 12) {
      categorie_lookup[0][betCtr-1] = betCtr;
      categorie_lookup[1][betCtr-1] = betCtr+12;
      categorie_lookup[2][betCtr-1] = betCtr+24;

      categorie_lookup[9][betCtr-1] = (betCtr*3)-2;
      categorie_lookup[10][betCtr-1] = (betCtr*3)-1;	
      categorie_lookup[11][betCtr-1] = betCtr*3;
    } else {
      categorie_lookup[0][betCtr-1] = -1;
      categorie_lookup[1][betCtr-1] = -1;
      categorie_lookup[2][betCtr-1] = -1;

      categorie_lookup[9][betCtr-1] = -1;
      categorie_lookup[10][betCtr-1] = -1;		
      categorie_lookup[11][betCtr-1] = -1;
    }

    categorie_lookup[3][betCtr-1] = betCtr;
    categorie_lookup[4][betCtr-1] = betCtr+NUM_BET_CATEGORIE_MAX;

    categorie_lookup[5][betCtr-1] = betCtr*2;
    categorie_lookup[6][betCtr-1] = (betCtr*2)-1;

    categorie_lookup[7][betCtr-1] = black[betCtr-1];
    categorie_lookup[8][betCtr-1] = red[betCtr-1];

  }

  if (bet_selection >= ROULETTE_WHEEL_NUM_OPTIONS) {
    for (betCtr = 0; betCtr < NUM_BET_CATEGORIE_MAX; betCtr++) {
      int selection = categorie_lookup
	[bet_selection - ROULETTE_WHEEL_NUM_OPTIONS][betCtr];
      if (winning_spin_num == selection) {
	return categorie_lookup
	  [bet_selection-ROULETTE_WHEEL_NUM_OPTIONS][NUM_BET_CATEGORIE_MAX];
      }
    }
  }
  else if (bet_selection == winning_spin_num) {
    return 35;
  }
  return -1;
}

void displayDigits(int number, SDL_Surface* screen, int padORmoney) {
  int digitArray[MAX_DIGITS] = {-1, -1, -1, -1, -1, -1};
  int digitCtr = MAX_DIGITS;

  while (number > 0) {
    digitCtr--;
    digitArray[digitCtr] = number % 10;
    number = number / 10;
  }

  loadDigits(digitArray, screen, padORmoney);
}

void loadDigits(int digitArray[], SDL_Surface *screen, int padORmoney) {
  SDL_Surface *digit = NULL;
  int offset = 0;
  int x;
  int y;

  if (padORmoney == 1) {
    x = NUMBER_PAD_DIGIT_X;
    y = NUMBER_PAD_DIGIT_Y;
  } else {
    x = DIGIT_MONEY_X;
    y = DIGIT_MONEY_Y;
  }

  for (int digitCtr = 0; digitCtr < MAX_DIGITS; digitCtr++) {
    if (digitArray[digitCtr] != -1) {
      stringstream temp;
      if (padORmoney == 1) {
	temp << NUMBERS_DIR << digitArray[digitCtr] << ".png"; 
      } else {
	temp << NUMBERS_DIR << digitArray[digitCtr] << ".jpg"; 
      }
      digit = loadImage(temp.str());
      applyVisual(x + (NUMBER_PAD_DIGIT_WIDTH * offset), y,digit,screen);
      SDL_FreeSurface(digit);
      offset = offset + 1;
    }
  }
		
  SDL_Flip(screen);	
}

int userExit(int userInput[]) {
  if (userInput[0] == 1) {
    int x = userInput[1];
    int y = userInput[2];
    if ((x > EXIT_X && x < EXIT_X+EXIT_MID*2) &&
	(y > EXIT_Y && y < EXIT_Y+EXIT_MID*2)) {
      return 0;
    }
  }

  return 1;
}

int padSelect(int x, int y) {
  x = x - NUMBER_PAD_X + 30;
  y = y - NUMBER_PAD_Y - 40;
  int numSelection = 0;

  for (int y_coord = 100; y_coord <= 211; y_coord+= 37) {
    for (int x_coord = 15; x_coord <= 175; x_coord+=80) {
      numSelection = numSelection + 1;
      if (x >= x_coord && x <= x_coord + 64 && y >= y_coord && 
	  y <= y_coord + 30) {
	if (numSelection == 10) {
	  return 0;
	}
	else if (numSelection > 10) {
	  return numSelection-1;
	}
	return numSelection;
      }
    }		
  }
	
  return -1;
}

int numberPadDown(int userInput[], SDL_Surface *screen, int old_numSelection, int padNumber) {
  if (userInput[0] == 1) {
    int padSelection = padSelect(userInput[1], userInput[2]);

    if (padSelection != -1) {
      loadNumberPad(padSelection, screen);
      displayDigits(padNumber, screen, 1);
    }
    return padSelection;
  }
  return old_numSelection;
}

void loadNumberPad(int numSelection, SDL_Surface* screen) {
  char numberPad_num[23] = NUMBER_PAD_DIR;
  char numberPad_num_temp[2];
		
  sprintf(numberPad_num_temp, "%d", numSelection);
  strcat(numberPad_num, numberPad_num_temp);
  strcat(numberPad_num, PNG);

  SDL_Surface *numberPad = loadImage(numberPad_num);
  applyVisual(NUMBER_PAD_X, NUMBER_PAD_Y,numberPad,screen);
  SDL_FreeSurface(numberPad);
  SDL_Flip(screen);	
}

int numberPadUp(int userInput[], SDL_Surface *screen, int numSelection, int old_number) {
  int new_number = old_number;

  if (userInput[0] == 2 && numSelection != -1) {
    clearNumberPad(screen);
    int padSelection = padSelect(userInput[1], userInput[2]);
    if (padSelection == numSelection) {
      new_number = updatePadNum(numSelection, old_number);
    }
    displayDigits(new_number, screen, 1);
  }

  return new_number;
}

int updatePadNum(int numSelection, int padNum) {
  int new_padNum = padNum;

  if (numSelection <= 9) {
    new_padNum = (padNum * 10) + numSelection;
    if (new_padNum > MAX_MONEY) {
      return padNum;
    }
  } else if (numSelection == 10) {
    return 0;
  }
  return new_padNum;
}

void loadRouletteWheel(int wheelCtr, SDL_Surface* screen) {
  stringstream temp;
  temp << ROULETTE_WHEEL_DIR << wheelCtr % ROULETTE_WHEEL_NUM_OPTIONS << JPG;

  SDL_Surface* roulette_wheel = loadImage(temp.str());
  applyVisual(ROULETTE_WHEEL_X, ROULETTE_WHEEL_Y,roulette_wheel,screen);
  SDL_FreeSurface(roulette_wheel);

  SDL_Surface* image = loadImage("images/exit.png");
  applyVisual(EXIT_X, EXIT_Y, image, screen);
  SDL_FreeSurface(image);

  SDL_Flip(screen);
}

int spinRouletteWheel(float dragDistance, SDL_Surface *screen, int wheelCtr) {
  int cycleCtr = 0;
  float cycleMax = .1 * dragDistance;
  float SDL_DelayTimeMS = 1 / dragDistance;
	
  while (SDL_DelayTimeMS < .9) {
    for (cycleCtr = 0; cycleCtr < cycleMax; cycleCtr ++) {
      wheelCtr = wheelCtr + 1;
      loadRouletteWheel(wheelCtr, screen);
      SDL_Delay(SDL_DelayTimeMS * 1500);
    }

    cycleMax = cycleMax / 1.5;
    SDL_DelayTimeMS = SDL_DelayTimeMS * 2;
  }

  for (cycleCtr = 0; cycleCtr < 7; cycleCtr ++) {	
    clearRouletteWheel(screen);
    SDL_Delay(150);
    loadRouletteWheel(wheelCtr, screen);
    SDL_Delay(150);	
  }

  return wheelCtr;
}


void loadBetTable(int betSelection, SDL_Surface* screen) {
  char bet_table_num[22] = BET_TABLE_DIR;
  char bet_table_num_temp[2];
		
  sprintf(bet_table_num_temp, "%d", betSelection);
  strcat(bet_table_num, bet_table_num_temp);
  strcat(bet_table_num, PNG);

  SDL_Surface *bet_table = loadImage(bet_table_num);
  applyVisual(BET_TABLE_X, BET_TABLE_Y,bet_table,screen);
  SDL_FreeSurface(bet_table);
  SDL_Flip(screen);
}

int displayBetTable(int userInput[], SDL_Surface *screen, int old_betSelection) {

  int betSelection = old_betSelection;

  if (userInput[0] == 1) {
    betSelection = betSelect(userInput[1],userInput[2],betSelection);
    if (betSelection != old_betSelection) {
      loadBetTable(betSelection, screen);
    }
  }

  return betSelection;
		
}

int betSelect(int x, int y, int old_betSelection) {
  int betSelection = 0;

  x = x - BET_TABLE_X + 30;
  y = y - BET_TABLE_Y - 40;

  for (int y_coord = 45; y_coord <= 489; y_coord+=37) {
    for (int x_coord = 69; x_coord <= 195; x_coord += 63) {
      betSelection = betSelection + 1;
      if (x >= x_coord && x <= x_coord + 60 
	  && y >= y_coord && y <= y_coord + 32) {
	if (betSelection > 36) {
	  return betSelection + 10;
	}
	return betSelection;
      }
    }
  }

  if (x >= 69 && x <= 158 && y >= 6 && y <= 38) {
    return 0;
  } else if (x >= 162 && x <= 255 && y >= 6 && y <= 38) {
    return 37;
  }
  else if (x >= 4 && x <= 32) {
    if (y >= 44 && y <= 112) {
      return 41;
    } else if (y >= 415 && y <= 483) {
      return 42;
    } else if (y >= 115 && y <= 187) {
      return 43;
    } else if (y >= 339 && y <= 411) {
      return 44;
    } else if (y >= 191 && y <= 262) {
      return 45;
    } else if (y >= 264 && y <= 335) {
      return 46;
    }
  } else if (x >= 36 && x <= 64) {
    if (y >= 44 && y <= 187) {
      return 38;
    }
    else if (y >= 191 && y <= 335) {
      return 39;
    }
    else if (y >= 339 && y <= 484) {
      return 40;
    }
  }
  return old_betSelection;
}

float calculateDistance(int start_x, int start_y, int end_x, int end_y) {
  end_x = end_x - start_x;
  end_y = end_y - start_y;
	
  end_x *= end_x;
  end_y *= end_y;
	
  return sqrt(end_x + end_y);
}

int isRouletteWheel(int userInput[]) {

  int x = userInput[1] - (ROULETTE_WHEEL_MID + ROULETTE_WHEEL_X);
  int y = userInput[2] - (ROULETTE_WHEEL_MID + ROULETTE_WHEEL_Y);

  x *= x;
  y *= y;

  if (sqrt(x + y) <= ROULETTE_WHEEL_MID) {
    return userInput[0];
  }
  return 0;
}

bool getUserInput(int userInput[]) {
  SDL_Event event;

  // get most recent cursor
  GnmsCursor* cur = TouchApp::sApp->getMostRecentCursor();
  if (cur != NULL) {
    // if cursor is live, new
    float x,y;
    cur->getCurrentCoordinates(x, y);
    if (cur->isLive() == true && TouchApp::sApp->getAge() == false) {
      userInput[0] = 1;
      userInput[1] = x;
      userInput[2] = y;
      TouchApp::sApp->ageMostRecentCursor();
    } else if (cur->isLive() == false && cur->isDead() == false) {
      userInput[0] = 2;
      userInput[1] = x;
      userInput[2] = y;
      cur->killCursor();
    } else if (cur->isLive() == true && TouchApp::sApp->getAge() == true) {
      userInput[0] = 3;
      userInput[1] = x;
      userInput[2] = y;
    }
  }

  while(SDL_PollEvent(&event))  {
    if (event.button.button == SDL_BUTTON_LEFT) {
      if (event.type == SDL_MOUSEBUTTONDOWN) {
	userInput[0] = 1;
	userInput[1] = event.button.x;
	userInput[2] = event.button.y;
      }
      else if (event.type == SDL_MOUSEBUTTONUP) {
	userInput[0] = 2;
	userInput[1] = event.button.x;
	userInput[2] = event.button.y;
      }
      else if (event.type == SDL_MOUSEMOTION) {
	userInput[0] = 3;
	userInput[1] = event.motion.x;
	userInput[2] = event.motion.y;
      }
    } else if (event.type == SDL_QUIT) {
      return false;
    } else if (event.type == SDL_KEYUP) {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
	return false;
      } else if (event.key.keysym.sym == SDLK_c) {
	if (SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE) {
	  SDL_ShowCursor(SDL_ENABLE);
	} else {
	  SDL_ShowCursor(SDL_DISABLE);
	}
      }
    }
  }

  return true;
}

SDL_Surface* loadImage(std::string filename) {

  string temp;
  temp = string(ROULETTE_DIR) + "/" + filename;

  SDL_Surface* loadedImage = IMG_Load(temp.c_str());
  if (loadedImage == NULL) {
    return NULL;
  }

  SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

  SDL_FreeSurface(loadedImage);

  return optimizedImage;
}

void applyVisual(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
  SDL_Rect offset;

  offset.x = x;
  offset.y = y;

  SDL_BlitSurface(source, NULL, destination, &offset);
}

void clearNumberPad(SDL_Surface *screen) {
  SDL_Surface* image = loadImage("images/numberPad/12.png");
  applyVisual(NUMBER_PAD_X, NUMBER_PAD_Y, image, screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void clearRouletteWheel(SDL_Surface *screen) {
  SDL_Surface* image = loadImage("images/rouletteWheel_compressed/none.jpg");
  applyVisual(ROULETTE_WHEEL_X, ROULETTE_WHEEL_Y,image,screen);
  SDL_FreeSurface(image);

  SDL_Surface* eimage = loadImage("images/exit.png");
  applyVisual(EXIT_X, EXIT_Y, eimage, screen);
  SDL_FreeSurface(eimage);

  SDL_Flip(screen);
}

void clearBetTable(SDL_Surface *screen) {
  SDL_Surface* image = loadImage("images/betTable/none.png");
  applyVisual(BET_TABLE_X, BET_TABLE_Y,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void clearMoneyBar(SDL_Surface *screen) {
  SDL_Surface* image = loadImage("images/numbers/money.jpg");
  applyVisual(PLAYER_MONEY_X, PLAYER_MONEY_Y,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void copyCoordinates(int copyTo[], int copyFrom[]) {
  copyTo[0] = copyFrom[1];
  copyTo[1] = copyFrom[2];
}

SDL_Surface* startupInit() {
  #if ROULETTE_NO_FULLSCREEN == 0
  SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
                  SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  #else
  SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
		  SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF);
  #endif

  #if ROULETTE_NO_CURSOR == 1
  SDL_ShowCursor(SDL_DISABLE);
  #endif

  displayOpenSplash(screen);

  SDL_FillRect(screen, NULL, 0x125717);

  clearRouletteWheel(screen);
  clearNumberPad(screen);
  clearBetTable(screen);
  clearMoneyBar(screen);

  SDL_Surface* image = loadImage("images/exit.png");
  applyVisual(EXIT_X, EXIT_Y, image, screen);
  SDL_FreeSurface(image);

  return screen;
}

void displayBetMoney(SDL_Surface* screen) {
  SDL_Surface* image = loadImage("images/text/betMoney.png");
  applyVisual(TEXT_X, TEXT_Y,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void displayPressEnter(SDL_Surface* screen) {
  SDL_Surface* image = loadImage("images/text/pressEnter.png");
  applyVisual(TEXT_X, TEXT_Y,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void displayDontHaveCash(SDL_Surface* screen) {
  SDL_Surface* image = loadImage("images/text/dontHaveCash.png");
  applyVisual(TEXT_X, TEXT_Y,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void displayChooseNumber(SDL_Surface* screen) {
  SDL_Surface* image = loadImage("images/text/chooseNumber.png");
  applyVisual(TEXT_X, TEXT_Y,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void clearText(SDL_Surface* screen) {
  SDL_Surface* image = loadImage("images/text/clearText.png");
  applyVisual(TEXT_X, TEXT_Y,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void displayOpenSplash(SDL_Surface* screen) {
  SDL_Surface* image = loadImage("images/splash/open.png");
  applyVisual(0, 0,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
  SDL_Delay(2000);
}

void displayCloseSplash(SDL_Surface* screen) {
  SDL_Surface* image = loadImage("images/splash/close.png");
  applyVisual(0, 0,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

void displayPlayAgainSplash(SDL_Surface* screen) {
  SDL_Surface* image = loadImage("images/splash/playAgain.png");
  applyVisual(0, 0,image,screen);
  SDL_FreeSurface(image);
  SDL_Flip(screen);
}

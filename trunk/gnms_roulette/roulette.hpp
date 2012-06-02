#ifndef _ROULETTE_HPP_
#define _ROULETTE_HPP_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "TouchApp.hpp"
#include "GnmsCursor.hpp"

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define SCREEN_BPP 32

#define BET_TABLE_DIR "images/betTable/"
#define BET_TABLE_X 959
#define BET_TABLE_Y 415
#define NUM_BET_CATEGORIES 12
#define NUM_BET_CATEGORIE_MAX 18

#define ROULETTE_WHEEL_DIR "images/rouletteWheel_compressed/"
#define ROULETTE_WHEEL_X 61
#define ROULETTE_WHEEL_Y 100
#define ROULETTE_WHEEL_MID 416
#define ROULETTE_WHEEL_NUM_OPTIONS 38

#define ROULETTE_NO_FULLSCREEN 0
#define ROULETTE_NO_CURSOR 1

#define NUMBERS_DIR "images/numbers/"

#define NUMBER_PAD_DIR "images/numberPad/"
#define NUMBER_PAD_X 964
#define NUMBER_PAD_Y 145
#define NUMBER_PAD_DIGIT_X NUMBER_PAD_X + 35
#define NUMBER_PAD_DIGIT_Y NUMBER_PAD_Y + 30
#define NUMBER_PAD_DIGIT_WIDTH 30 

#define PLAYER_MONEY_X 964
#define PLAYER_MONEY_Y 30
#define DIGIT_MONEY_X PLAYER_MONEY_X + 60
#define DIGIT_MONEY_Y PLAYER_MONEY_Y

#define EXIT_X 100
#define EXIT_Y 830
#define EXIT_MID 37

#define TEXT_X 177
#define TEXT_Y 30

#define PNG ".png"
#define JPG ".jpg"

#define MAX_MONEY 999999
#define MAX_DIGITS 6

SDL_Surface* loadImage(std::string filename);
SDL_Surface* startupInit();
void applyVisual(int x, int y, SDL_Surface* source, SDL_Surface* destination);

int isRouletteWheel(int[]);
int spinRouletteWheel(float, SDL_Surface*, int);

int displayBetTable(int[],SDL_Surface*, int);
void displayDigits(int, SDL_Surface*, int);
void loadDigits(int[], SDL_Surface*, int);

int numberPadDown(int[], SDL_Surface*, int, int);
int numberPadUp(int[], SDL_Surface*, int, int);

void copyCoordinates(int[], int[]);
float calculateDistance(int,int,int,int);
bool getUserInput(int[]);
int userExit(int[]);
int playAgainYesNo(int, int);

int padSelect(int, int);
int betSelect(int,int,int);
int updatePadNum(int,int);
int isWinningBet(int,int);
void updatePlayerCash(int);

void clearNumberPad(SDL_Surface*);
void clearRouletteWheel(SDL_Surface*);
void clearBetTable(SDL_Surface*);
void clearMoneyBar(SDL_Surface*);

void loadNumberPad(int, SDL_Surface*);
void loadRouletteWheel(int, SDL_Surface*);
void loadBetTable(int, SDL_Surface*);

void displayBetMoney(SDL_Surface*);
void displayPressEnter(SDL_Surface*);
void displayDontHaveCash(SDL_Surface*);
void displayChooseNumber(SDL_Surface*);
void clearText(SDL_Surface*);

void displayOpenSplash(SDL_Surface*);
void displayPlayAgainSplash(SDL_Surface*);
void displayCloseSplash(SDL_Surface*);

#endif

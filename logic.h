#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

typedef struct {
    int x;
    int y;
} Shaymin;

typedef struct {
    int x;
    int y;
    int alive;
} Flowers;

typedef struct {
    int x;
    int y;
    int direction;
    int badB;
} Bees;
typedef struct {
    // Store whether or not the game is over in this member:
    int gameLost;
    int gameWon;
    int gameSuperWin;
    Shaymin shaymin;
    Bees bees[2];
    int nbees;
    Flowers flowers[10];
    int nflowers;
    int score;

    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */

} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/
// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif

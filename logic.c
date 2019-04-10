#include "logic.h"
#include "stdio.h"

int shaymin_width = 15;
int flower_width = 9;
int bee_width = 12;

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    appState->shaymin.x = 0;
    appState->shaymin.y = 145; //shaymin starts at bottom left corner
    appState->gameWon = 0;
    appState->gameLost = 0;
    appState->gameSuperWin = 0;
    appState->score = 0;
    appState->nflowers = 10;
    for (int i = 0; i < appState->nflowers; i++) {
        appState->flowers[i].x = randint(0, 240 - flower_width); //boundary points
        appState->flowers[i].y = randint(0, 160 - flower_width);
        appState->flowers[i].alive = 1; //all begin alive
    }
    appState->nbees = 2;
    int row = 30;
    int col = 30;
    for (int i = 0; i < appState->nbees; i++) {
        appState->bees[i].y = row;
        appState->bees[i].x = col;
        appState->bees[i].direction = randint(0,2); //0 or 1
        if(appState->bees[i].direction) { //if direction is 1
            appState->bees[i].direction = 1;
            appState->bees[i].badB = 1; //it's a bad bee
        } else { //direction is 0
            appState->bees[i].direction = -1;
            appState->bees[i].badB = 0;
        }
        col += 2*col;
        row += 2*row;
    }
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.


static int isFlowerCollision(Shaymin * sh, Flowers * fl) {
    //Shaymin struct is just a coord {x,y}
     Shaymin s_upperleft = {sh->x, sh->y};
     Shaymin s_lowerright = {sh->x + 14, sh->y + 14}; //Shaymin total width/height is 15
     Shaymin fl_upperleft = {fl->x, fl->y};
     Shaymin fl_lowerright = {fl->x + 8, fl->y + 8}; //get lower right corner - flower width/height is 9

     if (s_upperleft.y > fl_lowerright.y || fl_upperleft.y > s_lowerright.y) { //y grows greater as you move down rows
         // shaymin is completely below or above the flower
         return 0;
     }
     if (s_upperleft.x > fl_lowerright.x || s_lowerright.x < fl_upperleft.x) {
         //shaymin is to the right of the flower or shaymin is to the left
         return 0;
     }
     return 1;
}

static int isBeeCollision(Shaymin * sh, Bees * be) {
    //same method as isFlowerCollision
     Shaymin s_upperleft = {sh->x, sh->y};
     Shaymin s_lowerright = {sh->x + 14, sh->y + 14};
     Shaymin b_upperleft = {be->x, be->y};
     Shaymin b_lowerright = {be->x + 11, be->y + 11};

     if (s_upperleft.y > b_lowerright.y || b_upperleft.y > s_lowerright.y) {
         return 0;
     }
     if (s_upperleft.x > b_lowerright.x || s_lowerright.x < b_upperleft.x) {
         return 0;
     }
     return 1;
}

static void moveBee(AppState* nextAppState, AppState* currentAppState) {
    for (int i = 0; i < currentAppState->nbees; i++) {
        int curr_x = currentAppState->bees[i].x;
        int direction = currentAppState->bees[i].direction; //can move right (-1) or left (1)
        nextAppState->bees[i].x = curr_x + direction; //try to continue in current direction

        if (curr_x >= 240 - (bee_width)) { //hits the leftmost boundary
            nextAppState->bees[i].direction = -1*direction; //change direction, but x is still at the leftmost boundary
            nextAppState->bees[i].x = curr_x + nextAppState->bees[i].direction; //change y coord or it will be stuck hitting the wall
        }
        if (curr_x <= 0) { //hits the right most boundary
            nextAppState->bees[i].direction = -1*direction;
            nextAppState->bees[i].x = curr_x + nextAppState->bees[i].direction;
        }
    }
}

AppState processAppState(AppState *currentAppState, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

     AppState nextAppState = *currentAppState;
     int x = currentAppState->shaymin.x;
     int y = currentAppState->shaymin.y;
     if (currentAppState->score == currentAppState->nflowers) {
         nextAppState.gameWon = 1; //ate all the flowers
     }

     if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
        if (x > 0) { //do not move left if at leftmost boundary
            nextAppState.shaymin.x = x - 1;
        }
     }
     if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
         if (x < 240 - (shaymin_width)) { //do not move right if at rightmost boundary
            nextAppState.shaymin.x = x + 1;
        }
     }
     if (KEY_DOWN(BUTTON_DOWN, keysPressedNow)) {
        if (y < 160 - (shaymin_width)) { //do not move down if at bottom
            nextAppState.shaymin.y = y + 1;
        }
     }
     if (KEY_DOWN(BUTTON_UP, keysPressedNow)) {
        if (y > 0) { //do not move up if at top
            nextAppState.shaymin.y = y - 1;
        }
     }
     for (int i = 0; i < currentAppState->nflowers; i++) {
         if (currentAppState->flowers[i].alive && isFlowerCollision(&nextAppState.shaymin, &currentAppState->flowers[i])) {
            currentAppState->flowers[i].alive = 0; //the flower has been eaten
            nextAppState.flowers[i].alive = 0; //flower is gone for next state
            nextAppState.score++;
         }
     }

    for (int i = 0; i < currentAppState->nbees; i++) {
         if(isBeeCollision(&nextAppState.shaymin, &currentAppState->bees[i]) && currentAppState->bees[i].badB) {
            nextAppState.gameLost = 1; //if shaymin collides with a bad bee, the game is over
        } else if (isBeeCollision(&nextAppState.shaymin, &currentAppState->bees[i])) {
            nextAppState.gameSuperWin = 1; //else the bee is friendly
        }
        moveBee(&nextAppState, currentAppState);
     }
    return nextAppState;
}

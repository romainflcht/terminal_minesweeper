#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"
#include "utils.h"

// __ DEFINITIONS ______________________________________________________________
#define BOARD_W        30
#define BOARD_H        10
#define BOMB_RATIO     10
#define CHEAT_ENABLE    0

// __ TYPE DEFINITIONS _________________________________________________________
typedef struct 
{
    int value;
    int is_discovered; 
    int is_flagged; 
} BOX_t;

// __ FUNCTIONS ________________________________________________________________
// * Function that set every box to not discovered and set there values to 0. 
// * return: the game board newly created.  
BOX_t** init_board();

// * Function that show the game board passed in argument. 
// * @param: *board: the game board. 
void show_board(BOX_t **board);

// * Function that place bomb randomly on the game board.
// * @param: *board: the game board. 
void place_bomb(BOX_t **board);

// * Function that return 1 if the coordinates are in the game board, 0 otherwise. 
// * @param: x: x coordinate of the box. 
// * @param: y: y coordinate of the box. 
// * @ret: 1 if the coordinate is valid, 0 otherwise.  
int check_coordinate(int x, int y);

// * Function that update box's around the box passed in argument. 
// * @param: x: the x coordinate of the box that need to update around boxs. 
// * @param: y: the y coordinate of the box that need to update around boxs. 
// * @param: @param: *board: the game board. 
void update_box_value(int x, int y, BOX_t **board);

// * Function that discover the box passed in argument and return 1 if it was successful
// * and -1 if the box was a bomb. 
// * @param: x: the x coordinate of the box to discover. 
// * @param: y: the y coordinate of the box to discover. 
// * @param: @param: *board: the game board. 
// * @ret: 1 if the move has been played and 0/-1 in case of error. 
int discover(int x, int y, BOX_t **board);

// * Function that flag the box passed in argument and return 1 if it was successful
// * and -1 if not. 
// * @param: x: the x coordinate of the box to discover. 
// * @param: y: the y coordinate of the box to discover. 
// * @param: @param: *board: the game board. 
int flag(int x, int y, BOX_t **board);

// * Function that handle the user input and play a move according to the input. 
// * @param: *user_input: pointer to the input string set by the player. 
// * @param: @param: *board: the game board. 
int play_a_move(char *user_input, BOX_t **board);

// * Function that return 1 if every bomb is flagged, 0 otherwise. 
// * @param: @param: *board: the game board. 
int check_win(BOX_t **board);

#endif
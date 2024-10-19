#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdio.h>

// __ FUNCTIONS ________________________________________________________________
// * Clear the terminal screen. 
void clear_screen(void);

// * Show the help text on top of the input field. 
void show_help(void);

// * Show the end screen. 
// * @param: as_won: Show the win screen if the player won and show the 
// *                 lose screen otherwise. 
void end_screen(int as_won);

#endif
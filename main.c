#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define BOARD_W        30
#define BOARD_H        10
#define BOMB_RATIO     10
#define CHEAT_ENABLE    0
const char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";


typedef struct {
    int8_t value;
    uint8_t is_discovered; 
    uint8_t is_flagged; 
} Box;


void init_ramdom_seed(void);
void init_board(Box *board[BOARD_H][BOARD_W]);
void show_board(Box *board[BOARD_H][BOARD_W]);
void clear_screen(void);
void show_help(void);
uint8_t end_screen(uint8_t as_won);
void place_bomb(Box *board[BOARD_H][BOARD_W]);
uint8_t randint(uint8_t min, uint8_t max);
void update_box_value(uint8_t x, uint8_t y, Box *board[BOARD_H][BOARD_W]);
uint8_t check_coordinate(uint8_t x, uint8_t y);
int8_t discover(uint8_t x, uint8_t y, Box *board[BOARD_H][BOARD_W]);
int8_t flag(uint8_t x, uint8_t y, Box *board[BOARD_H][BOARD_W]);
int8_t play_a_move(char *user_input, Box *board[BOARD_H][BOARD_W]);
uint8_t check_win(Box *board[BOARD_H][BOARD_W]);


int main(void) {
    clear_screen();
    init_ramdom_seed();


    Box *board[BOARD_H][BOARD_W];
    init_board(board);

    int8_t as_won = 0; 
    uint8_t display_help = 0; 

    while (!as_won) {
        // Show the game board. 
        show_board(board);

        if (display_help) {
            show_help();
            display_help = 0; 
        }
        

        // Init user_input string, add 2 for the \0 and for int greater than 10. 
        char user_input[3 + 2]; 
        printf("Write an action, type <H> to get help or <Q> to quit : ");
        fgets(user_input, sizeof(user_input), stdin);
        int8_t player_state = play_a_move(user_input, board);

        clear_screen();

        if (check_win(board)) {
            as_won = 1; 
        }

        else if (player_state == -1) {
            as_won = -1;
        }
        
        else if (player_state == -2) {
            show_help();
        }
        else if (player_state == 0) {
            printf("Your move isn't correct. Please try again...\n\n");
        }
    }

    return end_screen(as_won);
}


void init_ramdom_seed(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((unsigned int)ts.tv_nsec);
}


void init_board(Box *board[BOARD_H][BOARD_W]) {
    /*
    Function that set every box to not discovered and set there values to 0. 
    @param: *board: the game board. 
    */

    // Loop through each box. 
    for (uint8_t h_index = 0; h_index < BOARD_H; h_index++) {
        for (uint8_t w_index = 0; w_index < BOARD_W; w_index++) {

            // Allocate memory to a Box structure, set it to 0 and add it to the board. 
            Box *current_box = malloc(sizeof(Box));
            current_box->value = 0;
            current_box->is_discovered = 0;
            current_box->is_flagged = 0;
            board[h_index][w_index] = current_box;
            
        }
    }

    // Place bombs. 
    place_bomb(board);
}

void show_board(Box *board[BOARD_H][BOARD_W]) {
    /*
    Function that show the game board passed in argument. 
    @param: *board: the game board. 
    */


    // Draw the index line. 
    printf("\n  ");
    for (uint8_t index = 0; index < BOARD_W; index++) {
        if (index < 10) {
            printf("  %d", index);
            continue;
        }

        printf(" %d", index);
        
    }

    // Draw the board contour. 
    printf("\n  o");
    for (uint8_t index = 0; index < BOARD_W * 3; index++) {
        printf("-");
        
    }
    printf("o\n");


    // Loop through each box. 
    for (uint8_t h_index = 0; h_index < BOARD_H; h_index++) {

        printf("%c |", alphabet[h_index]);
        for (uint8_t w_index = 0; w_index < BOARD_W; w_index++) {
            
            // Get the box that we want to show. 
            Box *current_box = board[h_index][w_index];

            // Cheat. 
            if (current_box->value == -1 && CHEAT_ENABLE && !current_box->is_discovered && !current_box->is_flagged) {
                printf(" * ");
                continue;
            }

            // If the box is flagged, show the flag. 
            if (current_box->is_flagged) {
                printf(" F ");
                continue;
            }
            // If the box is discovered, show the value, else show blank. 
            else if (current_box->is_discovered) {
                if (current_box->value == 0) {
                    printf("   ");
                    continue;
                }

                else if (current_box->value == -1) {
                    printf(" X ");
                    continue;
                }

                printf(" %d ", current_box->value);
                continue;
            }

            printf(" # ");
        }

        printf("|\n");
    }

    // Draw the last line. 
    printf("  o");
    for (uint8_t index = 0; index < BOARD_W * 3; index++) {
        printf("-");
    }
    printf("o\n");
}


void clear_screen(void) {
    /*
    Function that clear the screen by printing \n 255 times. 
    */

    for (uint8_t index = 0; index < 255; index++) { printf("\n"); }
}


void show_help(void) {
    /*
    Function that print to the screen the help menu. 
    */

    printf(". HELP : \n");
    printf("\nTo discover a box : \n");
    printf("\t - Just put the box coordinate in the input field.\n");
    printf("\t   Example : \"e5\", \"q23\"...\n");
    printf("\nTo set a flag on a box : \n");
    printf("\t - You need to input the letter 'F' and then the coordinate of the box that you want to flag.\n");
    printf("\t   Example : \"Fg2\", \"Fa38\"...\n");
    printf("\n\n");
}


uint8_t end_screen(uint8_t as_won) {
    /*
    Function that print the end screen depending on if the player won or lose. 
    @param: as_won: integer that tell us if the player won or not. 
    */

    if (as_won == 1) {
        // Win screen. 

        printf("                                   .''.       \n"); 
        printf("       .''.      .        *''*    :_\\/_:     . \n");
        printf("      :_\\/_:   _\\(/_  .:.*_\\/_*   : /\\ :  .'.:.'.\n");
        printf("  .''.: /\\ :   ./)\\   ':'* /\\ * :  '..'.  -=:o:=-\n");
        printf(" :_\\/_:'.:::.    ' *''*    * '.\\'/.' _\\(/_'.':'.'\n");
        printf(" : /\\ : :::::     *_\\/_*     -= o =-  /)\\    '  *\n");
        printf("  '..'  ':::'     * /\\ *     .'/.\\'.   '\n");
        printf("      *            *..*         :\n");
        printf("        *\n");
        printf("        *\n\n");

        printf("Well done ! you won. \nGoodbye :)\n\n");

        return EXIT_SUCCESS; 
    }

    // Lose screen. 
    printf("                             ____\n");
    printf("                     __,-~~/~    `---.\n");
    printf("                   _/_,---(      ,    )\n");
    printf("               __ /        <    /   )  \\___\n");
    printf("- ------===;;;'====------------------===;;;===----- -  -\n");
    printf("                  \\/  ~\"~\"~\"~\"~\"~\\~\"~)~\"/\n");
    printf("                  (_ (   \\  (     >    \\)\n");
    printf("                   \\_( _ <         >_>'\n");
    printf("                      ~ `-i' ::>|--\"\n");
    printf("                          I;|.|.|\n");
    printf("                         <|i::|i|`.\n");
    printf("                        (` ^'\"`-' \")\n\n");

    printf("Too bad... You lose.\nGoodbye :)\n\n");
    return EXIT_SUCCESS; 
}


void place_bomb(Box *board[BOARD_H][BOARD_W]) {
    /*
    Function that place bomb randomly on the game board.
    @param: *board: the game board. 
    */

    // Calculate how much bomb we want based on a poucentage. 
    uint8_t nb_bomb = ((BOARD_H * BOARD_W) * BOMB_RATIO) / 100;

    while (nb_bomb > 1) {
        // Genetate random w and y coordinates. 
        uint8_t rand_y = randint(0, BOARD_H - 1);
        uint8_t rand_x = randint(0, BOARD_W - 1);

        // Get the box to update. 
        Box *current_box = board[rand_y][rand_x];

        // If the box is already a bomb, retry. 
        if (current_box->value == -1) {
            continue;
        }

        // Else set it to a bomb. 
        current_box->value = -1;

        // Update every boxs around the bomb. 
        update_box_value(rand_x, rand_y, board);
        nb_bomb--;
    }

}

uint8_t randint(uint8_t min, uint8_t max) {
    /*
    Function that generate a random number between an interval. 
    @param: min: the min interval. 
    @param: max: the max interval. 
    */
    
    int range = max - min + 1;
    int result = (rand() % range) + min;
    return result;
}


uint8_t check_coordinate(uint8_t x, uint8_t y) {
    /*
    Function that return 1 if the coordinates are in the game board, 0 otherwise. 
    @param: x: x coordinate of the box. 
    @param: y: y coordinate of the box. 
    */
    if ((x >= 0 && x <= BOARD_W - 1) && (y >= 0 && y <= BOARD_H - 1)) {
        return 1; 
    }

    return 0;
}


void update_box_value(uint8_t x, uint8_t y, Box *board[BOARD_H][BOARD_W]) {
    /*
    Function that update box's around the box passed in argument. 
    @param: x: the x coordinate of the box that need to update around boxs. 
    @param: y: the y coordinate of the box that need to update around boxs. 
    @param: @param: *board: the game board. 
    */
    
    if (!check_coordinate(x, y)) {
        return; 
    }

    for (int8_t y_index = -1; y_index <= 1; y_index++) {
        for (int8_t x_index = -1; x_index <= 1; x_index++) {

            int8_t new_x = x + x_index;
            int8_t new_y = y + y_index;
            
            // If the coordinate of the box to update aren't in the game board, don't update. 
            if (!check_coordinate(new_x, new_y) || (new_x == x && new_y == y)) {
                continue; 
            }

            // Don't update the box if it is a bomb. 
            else if (board[new_y][new_x]->value == -1) {
                continue;
            }
            
            // Update the box. 
            board[new_y][new_x]->value++;
        }
    }
}


int8_t discover(uint8_t x, uint8_t y, Box *board[BOARD_H][BOARD_W]) {
    /*
    Function that discover the box passed in argument and return 1 if it was successful
    and -1 if the box was a bomb. 
    @param: x: the x coordinate of the box to discover. 
    @param: y: the y coordinate of the box to discover. 
    @param: @param: *board: the game board. 
    */

    // Check if the coordinates are correct. 
    if (!check_coordinate(x, y)) {
        return 0;
    }

    // Get the box to discover. 
    Box *current_box = board[y][x];

    // If the box is already discover or is flagged, don't discover it. 
    if (current_box->is_discovered) {
        return 0;
    }
    else if (current_box->is_flagged) {
        return 0;
    }
    

    if (current_box->value == 0) {
        // If the box value is 0, we need to discover every boxs around it. 
        current_box->is_discovered = 1;
        // First three top boxs. 
        discover(x + 1, y + 1, board);
        discover(x, y + 1, board);
        discover(x - 1, y + 1, board);

        // Two around boxs. 
        discover(x + 1, y, board);
        discover(x - 1, y, board);

        // First three bottom boxs. 
        discover(x + 1, y - 1, board);
        discover(x, y - 1, board);
        discover(x - 1, y - 1, board);
        return 1; 
    }

    else if (current_box->value == -1) {
        // Played on a bomb, need to and the game. 
        current_box->is_discovered = 1; 
        return -1; 
    } 


    // Played on a non zero value, discover the box. 
    current_box->is_discovered = 1; 
    return 1; 
}

int8_t flag(uint8_t x, uint8_t y, Box *board[BOARD_H][BOARD_W]) {
    /*
    Function that flag the box passed in argument and return 1 if it was successful
    and -1 if not. 
    @param: x: the x coordinate of the box to discover. 
    @param: y: the y coordinate of the box to discover. 
    @param: @param: *board: the game board. 
    */

    // Check if the coordinates are correct. 
    if (!check_coordinate(x, y)) {
        return 0;
    }

    // Get the box to discover. 
    Box *current_box = board[y][x];

    // If the box is already discovered, don't flag it. 
    if (current_box->is_discovered) {
        return 0; 
    }

    // If the box is already flagged, remove the flag. 
    if (current_box->is_flagged) {
        current_box->is_flagged = 0; 
        return 1;
    }

    // Else flag the box. 
    current_box->is_flagged = 1;
    return 1; 
}


int8_t play_a_move(char *user_input, Box *board[BOARD_H][BOARD_W]) {
    /*
    Function that handle the user input and play a move according to the input. 
    @param: *user_input: pointer to the input string set by the player. 
    @param: @param: *board: the game board. 
    */

    // If the input is too short, abort. 
    if (strlen(user_input) < 2) {
        return 0; 
    }

    // Check if the user want to show the help menu. 
    if (*user_input == 'H') {
        return -2; 
    }

    // Check if the user want to quit the program. 
    else if (*user_input == 'Q') {
        clear_screen();
        printf("Goodbye :)\n"); 
        exit(EXIT_SUCCESS);
    }

    // Init variable that handle coordinate int convertion and action between discover and flag.  
    uint8_t is_flag_action = 0; 
    uint8_t index_offset = 0; 
    
    // Set the action and index offset. 
    if (*user_input == 'F') {
        is_flag_action = 1; 
        index_offset = 1; 
    }

    // Convert the y char that is a letter to an integer. 
    uint8_t y = user_input[0 + index_offset] - 97;
    

    // Convert the x string coordinate to integer. 
    char *p_user_input_begin; 
    uint8_t x = strtol(user_input + 1 + index_offset, &p_user_input_begin, 10);

    // Do the action. 
    if (is_flag_action == 1) {
        return flag(x, y, board);
    }

    return discover(x, y, board);
}


uint8_t check_win(Box *board[BOARD_H][BOARD_W]) {
    /*
    Function that return 1 if every bomb is flagged, 0 otherwise. 
    */

    // Loop through each boxs. 
    for (uint8_t h_index = 0; h_index < BOARD_H; h_index++) {
        for (uint8_t w_index = 0; w_index < BOARD_W; w_index++) {
            
            // Get the curren box. 
            Box *current_box = board[h_index][w_index]; 

            // Return 0 if the box is a bomb and is not flagged. 
            if (current_box->value == -1 && !current_box->is_flagged) {
                return 0; 
            }
        }
    }

    // Every bomb are flagged. 
    return 1; 
}

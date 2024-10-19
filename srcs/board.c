#include "board.h"


// * Function that set every box to not discovered and set there values to 0. 
// * return: the game board newly created.  
BOX_t** init_board()
{
    int         h_i; 
    int         w_i; 
    BOX_t**     board; 
    BOX_t*      row; 

    board = malloc(sizeof(BOX_t*) * BOARD_H); 
    if (!board)
        return NULL; 

    // Loop through each box. 
    for (h_i = 0; h_i < BOARD_H; h_i++) 
    {
        row = malloc(sizeof(BOX_t) * BOARD_W); 
        if (!row)
            // TODO: free board. 
            return NULL; 

        board[h_i] = row; 

        for (w_i = 0; w_i < BOARD_W; w_i++) 
        {
            board[h_i][w_i].is_discovered = 0; 
            board[h_i][w_i].is_flagged = 0; 
            board[h_i][w_i].value = 0; 
        }
    }

    // Place bombs. 
    place_bomb(board);
    return board; 
}


// * Function that show the game board passed in argument. 
// * @param: *board: the game board. 
void show_board(BOX_t **board) 
{
    int     i;  
    int     h_i; 
    int     w_i;
    BOX_t   *current_box; 
    
    // Draw the index line. 
    printf("\n  ");

    for (i = 0; i < BOARD_W; i++) 
    {
        if (i < 10) 
        {
            printf("  %d", i);
            continue;
        }

        printf(" %d", i); 
    }

    // Draw the board contour. 
    printf("\n  o");
    for (i = 0; i < BOARD_W * 3; i++) 
        printf("-"); 
    printf("o\n");

    // Loop through each box. 
    for (h_i = 0; h_i < BOARD_H; h_i++) 
    { 
        printf("%c |", 'a' + h_i);

        for (w_i = 0; w_i < BOARD_W; w_i++) 
        {
            
            // Get the box that we want to show. 
            current_box = &board[h_i][w_i];

            // Cheat. 
            if (current_box->value == -1 && CHEAT_ENABLE && !current_box->is_discovered && !current_box->is_flagged) 
            {
                printf(" * ");
                continue;
            }

            // If the box is flagged, show the flag. 
            if (current_box->is_flagged) 
            {
                printf(" F ");
                continue;
            }
            // If the box is discovered, show the value, else show blank. 
            else if (current_box->is_discovered) 
            {
                if (current_box->value == 0) 
                {
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

    for (i = 0; i < BOARD_W * 3; i++)
        printf("-");

    printf("o\n");
}


// * Function that place bomb randomly on the game board.
// * @param: *board: the game board. 
void place_bomb(BOX_t **board) 
{
    int nb_bomb;
    int rand_x;
    int rand_y;

    // Calculate how much bomb we want based on a poucentage. 
    nb_bomb = ((BOARD_H * BOARD_W) * BOMB_RATIO) / 100;

    while (nb_bomb > 1) {
        // Genetate random w and y coordinates. 
        rand_x = randint(0, BOARD_W - 1);
        rand_y = randint(0, BOARD_H - 1);

        // If the box is already a bomb, retry. 
        if (board[rand_y][rand_x].value == -1)
            continue;

        // Else set it to a bomb. 
        board[rand_y][rand_x].value = -1;

        // Update every boxs around the bomb. 
        update_box_value(rand_x, rand_y, board);
        nb_bomb--;
    }
}


// * Function that return 1 if the coordinates are in the game board, 0 otherwise. 
// * @param: x: x coordinate of the box. 
// * @param: y: y coordinate of the box. 
// * @ret: 1 if the coordinate is valid, 0 otherwise.  
int check_coordinate(int x, int y) {
    if ((x >= 0 && x <= BOARD_W - 1) && (y >= 0 && y <= BOARD_H - 1))
        return 1; 

    return 0;
}


// * Function that update box's around the box passed in argument. 
// * @param: x: the x coordinate of the box that need to update around boxs. 
// * @param: y: the y coordinate of the box that need to update around boxs. 
// * @param: @param: *board: the game board. 
void update_box_value(int x, int y, BOX_t **board) 
{

    int y_i; 
    int x_i; 
    int new_x; 
    int new_y; 

    if (!check_coordinate(x, y))
        return;

    for (y_i = -1; y_i <= 1; y_i++) {
        for (x_i = -1; x_i <= 1; x_i++) {

            new_x = x + x_i;
            new_y = y + y_i;
            
            // If the coordinate of the box to update aren't in the game board, don't update. 
            if (!check_coordinate(new_x, new_y) || (new_x == x && new_y == y))
                continue; 

            // Don't update the box if it is a bomb. 
            else if (board[new_y][new_x].value == -1) 
                continue;

            // Update the box. 
            board[new_y][new_x].value++;
        }
    }
}



// * Function that discover the box passed in argument and return 1 if it was successful
// * and -1 if the box was a bomb. 
// * @param: x: the x coordinate of the box to discover. 
// * @param: y: the y coordinate of the box to discover. 
// * @param: @param: *board: the game board. 
// * @ret: 1 if the move has been played and 0/-1 in case of error. 
int discover(int x, int y, BOX_t **board) 
{

    // Check if the coordinates are correct. 
    if (!check_coordinate(x, y))
        return 0;

    // If the box is already discover or is flagged, don't discover it. 
    if (board[y][x].is_discovered)
        return 0;

    else if (board[y][x].is_flagged) 
        return 0;


    if (board[y][x].value == 0) 
    {
        // If the box value is 0, we need to discover every boxs around it. 
        board[y][x].is_discovered = 1;
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

    else if (board[y][x].value == -1) 
    {
        // Played on a bomb, need to end the game. 
        board[y][x].is_discovered = 1; 
        return -1; 
    } 

    // Played on a non zero value, discover the box. 
    board[y][x].is_discovered = 1; 
    return 1; 
}



// * Function that flag the box passed in argument and return 1 if it was successful
// * and -1 if not. 
// * @param: x: the x coordinate of the box to discover. 
// * @param: y: the y coordinate of the box to discover. 
// * @param: @param: *board: the game board. 
int flag(int x, int y, BOX_t **board) 
{
    // Check if the coordinates are correct. 
    if (!check_coordinate(x, y)) 
        return 0;

    // If the box is already discovered, don't flag it. 
    if (board[y][x].is_discovered)
        return 0; 

    // If the box is already flagged, remove the flag. 
    if (board[y][x].is_flagged) 
    {
        board[y][x].is_flagged = 0; 
        return 1;
    }

    // Else flag the box. 
    board[y][x].is_flagged = 1;
    return 1; 
}



// * Function that handle the user input and play a move according to the input. 
// * @param: *user_input: pointer to the input string set by the player. 
// * @param: @param: *board: the game board. 
int play_a_move(char *user_input, BOX_t **board) 
{
    char *p_user_input_begin; 
    int is_flag_action; 
    int index_offset; 
    int x; 
    int y; 

    // If the input is too short, abort. 
    if (strlen(user_input) < 2)
        return 0; 

    // Check if the user want to show the help menu. 
    if (*user_input == 'H')
        return -2; 

    // Check if the user want to quit the program. 
    else if (*user_input == 'Q') 
    {
        clear_screen();
        printf("Goodbye :)\n"); 
        exit(0);
    }

    // Init variable that handle coordinate int convertion and action between discover and flag.  
    is_flag_action = 0; 
    index_offset = 0; 
    
    // Set the action and index offset. 
    if (*user_input == 'F') 
    {
        is_flag_action = 1; 
        index_offset = 1; 
    }

    // Convert the y char that is a letter to an integer. 
    y = user_input[0 + index_offset] - 97;

    // Convert the x string coordinate to integer.  
    x = strtol(user_input + 1 + index_offset, &p_user_input_begin, 10);

    // Do the action. 
    if (is_flag_action == 1) 
        return flag(x, y, board);

    return discover(x, y, board);
}



// * Function that return 1 if every bomb is flagged, 0 otherwise. 
// * @param: @param: *board: the game board. 
int check_win(BOX_t **board) 
{
    int h_i; 
    int w_i; 
    // Loop through each boxs. 
    for (h_i = 0; h_i < BOARD_H; h_i++) 
    {
        for (w_i = 0; w_i < BOARD_W; w_i++) 
        {
            // Return 0 if the box is a bomb and is not flagged. 
            if (board[h_i][w_i].value == -1 && !board[h_i][w_i].is_flagged)
                return 0; 
        }
    }

    // Every bomb are flagged. 
    return 1; 
}

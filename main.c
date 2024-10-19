#include <stdio.h>

#include "screen.h"
#include "utils.h"
#include "board.h"


int main(void) 
{
    int as_won; 
    int display_help; 
    int player_state; 
    BOX_t **board;

    // __ INITIALISATION _______________________________________________________
    clear_screen();
    init_ramdom_seed();

    board = create_board();

    as_won = 0; 
    display_help = 0; 

    // __ MAIN LOOP ____________________________________________________________
    while (!as_won) 
    {
       show_board(board);

        if (display_help) 
        {
            show_help();
            display_help = 0; 
        }
        

        // Init user_input string, add 2 for the \0 and for int greater than 10. 
        char user_input[3 + 2]; 

        printf("Write an action, type <H> to get help or <Q> to quit : ");
        fgets(user_input, sizeof(user_input), stdin);
        player_state = play_a_move(user_input, board);

        clear_screen();

        if (check_win(board))
            as_won = 1; 

        // The user input cannot be parsed. 
        else if (player_state == 0)
            printf("Your move isn't correct. Please try again...\n\n");

        // User played on a bomb. 
        else if (player_state == -1)
            as_won = -1;
        
        // User asked for the help menu. 
        else if (player_state == -2)
            show_help();
    }

    // Show the end screen according if the player won or lose. 
    end_screen(as_won);
    free_board(board); 
    return 0; 
}

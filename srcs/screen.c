#include "screen.h"


// * Clear the terminal screen. 
void clear_screen(void) 
{
    printf("\x1b[2J\x1b[H");
}


// * Show the help text on top of the input field. 
void show_help(void) 
{
    printf(". HELP : \n");
    printf("\nTo discover a box : \n");
    printf("\t - Just put the box coordinate in the input field.\n");
    printf("\t   Example : \"e5\", \"q23\"...\n");
    printf("\nTo set a flag on a box : \n");
    printf("\t - You need to input the letter 'F' and then the coordinate of the box that you want to flag.\n");
    printf("\t   Example : \"Fg2\", \"Fa38\"...\n");
    printf("\n\n");
}


// * Show the end screen. 
// * @param: as_won: Show the win screen if the player won and show the 
// *                 lose screen otherwise. 
void end_screen(int as_won) 
{
    if (as_won) 
    {
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

        return; 
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
    return; 
}

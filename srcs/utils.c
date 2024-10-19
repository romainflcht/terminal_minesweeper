#include "utils.h"


// * Initilialize the random seed based on current time. 
void init_ramdom_seed(void) 
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((unsigned int)ts.tv_nsec);
}


// * Function that generate a random number between an interval. 
// * @param: min: the min interval. 
// * @param: max: the max interval. 
int randint(int min, int max) 
{
    int range; 
    range = max - min + 1;
    
    return (rand() % range) + min;
}

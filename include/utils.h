#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <time.h>

// __ FUNCTIONS ________________________________________________________________
// * Initilialize the random seed based on current time. 
void init_ramdom_seed(void);

// * Function that generate a random number between an interval. 
// * @param: min: the min interval. 
// * @param: max: the max interval. 
int randint(int min, int max);

#endif
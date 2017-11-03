/*
 * common.h
 *
 *  Created on: Nov 28, 2011
 *      Author: 8w4
 */

#ifndef COMMON_H_
#define COMMON_H_
#include "BacNetEnumToString.h"

extern int verbosity_level;


/** Display problems...*/
#define VERBOSITY_LEVEL_1 1
#define verbosity_level_1(x) (x) >= (VERBOSITY_LEVEL_1) ? true : false

/** Display some information...*/
#define VERBOSITY_LEVEL_2 2
#define verbosity_level_2(x) (x) >= (VERBOSITY_LEVEL_2) ? true : false

/** Display all information...*/
#define VERBOSITY_LEVEL_3 3
#define verbosity_level_3(x) (x) >= (VERBOSITY_LEVEL_3) ? true : false



#endif /* COMMON_H_ */

/* 
 * test driver to debug nvram-faker code without
 * having to LD_PRELOAD a lib.
 */
#include <stdio.h>
#include <stdlib.h>

#ifndef DEBUG
#define DEBUG
#endif

#ifndef NVRAM_EXE
#define NVRAM_EXE
#endif

//#ifndef INI_FILE_PATH
#define INI_FILE_PATH "./nvram.ini"
//#endif

#include "nvram-faker-internal.h"

extern void initialize_ini(void);
extern void end(void);

int main(void)
{
    DEBUG_PRINTF("calling initalize_ini().\n");
    initialize_ini();
    DEBUG_PRINTF("calling end().\n");
    end();
    DEBUG_PRINTF("Done.\n");
    return 0;
}
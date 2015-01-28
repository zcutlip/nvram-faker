#ifndef __nvram_faker_internal_h
#define __nvram_faker_internal_h

#ifndef INI_MAX_LINE
#define INI_MAX_LINE 1024
#endif /* INI_MAX_LINE */

#ifndef INI_USE_STACK
#define INI_USE_STACK 0
#endif /* INI_USE_STACK */

#ifdef DEBUG
#define DEBUG_PRINTF(format,...) fprintf(stderr,format,## __VA_ARGS__)
#else
#define DEBUG_PRINTF(format,...)
#endif

#define LOG_PRINTF(format,...) fprintf(stderr,format, ## __VA_ARGS__)

#endif /* __nvram_faker_internal_h */
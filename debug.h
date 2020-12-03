#include <stdio.h>

//#define DEBUG

void debug(char *format, ...);

extern FILE *output_file;
extern int debugging;
extern int debugs;

#ifdef DEBUG
    #define PRINTF(...) do{debug(__VA_ARGS__);}while(0)
#else
    #define PRINTF(...) (0)
#endif

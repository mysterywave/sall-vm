#include <stdarg.h>

#include "debug.h"
#include "vm.h"

int debugs = 0;

void debug(char *format, ...) {
    if(debugging) {
        if(debugs > 50) {
            fprintf(output_file, "Debug file getting too large. Exiting.\n");
            cleanup(-1);
        }
        va_list list;
        va_start(list, format);
        vfprintf(output_file, format, list);
        va_end(list);
    }
}

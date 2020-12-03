#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "vm.h"
#include "debug.h"

unsigned int fileLength;

uint8_t *readFile(const char* filename) {
    FILE * file = fopen(filename, "rb");
    if(file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    rewind(file);
    uint8_t *in = (uint8_t *)malloc(fileLength);
    int bytes_read = fread(in, sizeof(uint8_t), fileLength, file);
    fclose(file);
    return in;
}

int main(int argc, char *argv[]) {
    if(argc == 2) {
        const char *filename = argv[1];
        unsigned char *file = readFile(filename);
        if(file) {
            output_file = fopen("vmlog.txt", "w");
            if(output_file == NULL) {
                printf("Can't open log file.\n");
                exit(0);
            }
            PRINTF("======== File Contents ========");
            for(int i = 0; i < fileLength; i++) {
                if(i % 16 == 0) {
                    PRINTF("\n%04X\t", i);
                }
                PRINTF("%.2X ", file[i]);
            }
            PRINTF("\n");
            PRINTF("\n");
            ram = calloc(1, 0x10000);
            memcpy(ram, file, fileLength);
            start();
        } else {
            char c[50];
            strcpy(c, "vm: ");
            perror(strcat(c, filename));
        }
    } else {
        char c[50];
        strcpy(c, "Usage: ");
        strcat(c, argv[0]);
        strcat(c, " FILE");
        printf("%s\n", c);
        return 1;
    }
    return 0;
}

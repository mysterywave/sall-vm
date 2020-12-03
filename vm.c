#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "vm.h"
#include "instructions.h"

#include "debug.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 180

uint8_t *ram;
uint16_t reg[256] = {
    0,      // ip
    0,      // sp
    0,      // oo
    0xFFFF, // r1
    0xFFFF, // r2
    0xFFFF, // r3
    0xFFFF, // r4
    0xFFFF  // r5
};

void cleanup() {
    fputs("\e[?25h", stdout);
    fflush(stdout);
    if(ram) {
        free(ram);
    }
    exit(0);
}

void start() {
    signal(SIGINT, cleanup);
    #ifndef _WIN32
        signal(SIGQUIT, cleanup);
    #endif
    signal(SIGABRT, cleanup);
    //fputs("\ec", stdout);
    //fputs("\e[?25l", stdout);
    fflush(stdout);
    
    int c = 0;
    while(c < 500) {
        PRINTF("\n");
        PRINTF("\x1B[33;1m{0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X}\x1B[0m\n", reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
        for(int j = 0; j < 5; j++) {
            for(int i = j * 16; i < j * 16 + 16; i++) {
                PRINTF("%.2X   ", ram[i]);
            }
            PRINTF("\n");
            if(reg[0] >= j * 16 && reg[0] < j * 16 + 16) {
                if(reg[3] >= j * 16 && reg[3] < j * 16 + 16) {
                    if(reg[0] == reg[3]) {
                        PRINTF("%*s^ ip, r1\n", (reg[0] - j * 16) * 5, "");
                    } else if(reg[0] < reg[3]) {
                        PRINTF("%*s^ ip", (reg[0] - j * 16) * 5, "");
                        PRINTF("%*s^ r1\n", (reg[3] - j * 16) * 5 - (reg[0] - j * 16) * 5 - 4, "");
                    } else {
                        PRINTF("%*s^ r1", (reg[3] - j * 16) * 5, "");
                        PRINTF("%*s^ ip\n", (reg[0] - j * 16) * 5 - (reg[3] - j * 16) * 5 - 4, "");
                    }
                } else {
                    PRINTF("%*s^ ip\n", (reg[0] - j * 16) * 5, "");
                }
            } else {
                if(reg[3] >= j * 16 && reg[3] < j * 16 + 16) {
                    PRINTF("%*s^ r1\n", (reg[3] - j * 16) * 5, "");
                } else {
                    PRINTF("\n");
                }
            }
        }
        PRINTF("\x1B[34;1m");
        instructions[ram[reg[0]]]();
        PRINTF("\x1B[0m");
        c++;
    }
}

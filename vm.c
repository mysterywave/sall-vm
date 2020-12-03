#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <ncurses.h>

#include "vm.h"
#include "instructions.h"

#include "debug.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 180

FILE *output_file;
int debugging = 0;

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

void start_curses() {
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    keypad(stdscr, TRUE);
    timeout(0);
}

void cleanup(int signal) {
    fprintf(output_file, "\x1B[0m\n");
    if(signal == SIGSEGV) {
        fprintf(output_file, "Segmentation fault.\n");
    }
    endwin();
    fflush(stdout);
    if(ram) {
        free(ram);
    }
    fclose(output_file);
    exit(0);
}

void interrupt(uint8_t port, uint16_t value) {
    debugging = 1;
    PRINTF("\x1B[33;1m{0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X}\x1B[0m\n", reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
    ram[reg[1]++] = reg[0] >> 8;
    ram[reg[1]++] = reg[0] & 0xFF;
    ram[reg[1]++] = reg[2] >> 8;
    ram[reg[1]++] = reg[2] & 0xFF;
    ram[reg[1]++] = reg[3] >> 8;
    ram[reg[1]++] = reg[3] & 0xFF;
    ram[reg[1]++] = reg[4] >> 8;
    ram[reg[1]++] = reg[4] & 0xFF;
    ram[reg[1]++] = reg[5] >> 8;
    ram[reg[1]++] = reg[5] & 0xFF;
    ram[reg[1]++] = reg[6] >> 8;
    ram[reg[1]++] = reg[6] & 0xFF;
    ram[reg[1]++] = reg[7] >> 8;
    ram[reg[1]++] = reg[7] & 0xFF;
    reg[0] = (ram[0x8] << 8) | ram[0x9]; // ip
    reg[2] = 0x00;  // oo
    reg[3] = port;  // r1
    reg[4] = value; // r2
    reg[5] = 0x00;  // r3
    reg[6] = 0x00;  // r4
    reg[7] = 0x00;  // r5
}

void start() {
    signal(SIGINT, cleanup);
    #ifndef _WIN32
        signal(SIGQUIT, cleanup);
    #endif
    signal(SIGABRT, cleanup);
    signal(SIGSEGV, cleanup);
    //fputs("\ec", stdout);
    //fputs("\e[?25l", stdout);
    fflush(stdout);
    
    start_curses();
    int c = 0;
    while(getch() != ERR) {}
    while(1) {
        PRINTF("\x1B[33;1m{0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X, 0x%.4X}\x1B[0m\n", reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
        for(int j = 0; j < 10; j++) {
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
                    if(reg[1] >= j * 16 && reg[1] < j * 16 + 16) {
                        PRINTF("%*s^ sp\n", (reg[1] - j * 16) * 5, "");
                    } else {
                        PRINTF("\n");
                    }
                }
            }
        }
        PRINTF("\x1B[34;1m");
        instructions[ram[reg[0]]]();
        PRINTF("\x1B[0m\n");
        char ch = getch();
        if(ch != ERR) {
            interrupt(1, ch);
            #ifndef DEBUG
                fflush(output_file);
            #endif
        }
        c++;
        #ifdef DEBUG
            fflush(output_file);
        #endif
        if(debugging) {
            debugs++;
        }
    }
}

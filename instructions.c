#include "instructions.h"
#include "vm.h"

#include <stdio.h>
#include <ncurses.h>

#define arg(n) ram[reg[0] + n + 1]
#define combine(a, b) (a << 8) | b
#define ip reg[0]

#include "debug.h"

char *register_name(int i) {
    switch(i) {
        case 0: return "%ip";
        case 1: return "%sp";
        case 2: return "%oo";
        case 3: return "%r1";
        case 4: return "%r2";
        case 5: return "%r3";
        case 6: return "%r4";
        case 7: return "%r5";
    }
    return "unknown";
}

void end() {
    PRINTF("end\n");
    cleanup(-1);
}

uint8_t screenOutputBuffer[3];
int screenOutputId = 0;
int displayMode = 0;

// register
#define INPUTLEN_0 1
#define INPUT_0(O) (reg[arg(O)])
#define INPUT_STR_0(O) (register_name(arg(O)))
#define STR_0 "%s"

// pointer register
#define INPUTLEN_1 1
#define INPUT_1(O) (combine(ram[reg[arg(O)]],ram[reg[arg(O)]+1]))
#define INPUT_STR_1(O) (register_name(arg(O)))
#define STR_1 "[%s]"

// constant pointer
#define INPUTLEN_2 2
#define INPUT_2(O) (combine(ram[combine(arg(O),arg(O+1))],ram[combine(arg(O),arg(O+1))+1]))
#define INPUT_STR_2(O) (combine(arg(O),arg(O+1)))
#define STR_2 "[%.4X]"

// constant
#define INPUTLEN_3 2
#define INPUT_3(O) (combine(arg(O),arg(O+1)))
#define INPUT_STR_3(O) (combine(arg(O),arg(O+1)))
#define STR_3 "%.4X"

// register
#define INPUT8LEN_0 1
#define INPUT8_0(O) (reg[arg(O)] & 0xFF)
#define INPUT8_STR_0(O) (register_name(arg(O)))
#define STR8_0 "%s"

// pointer register
#define INPUT8LEN_1 1
#define INPUT8_1(O) (ram[reg[arg(O)]])
#define INPUT8_STR_1(O) (register_name(arg(O)))
#define STR8_1 "[%s]"

// constant pointer
#define INPUT8LEN_2 2
#define INPUT8_2(O) (ram[combine(arg(O),arg(O+1))])
#define INPUT8_STR_2(O) (combine(arg(O),arg(O+1)))
#define STR8_2 "[%.4X]"

// constant
#define INPUT8LEN_3 1
#define INPUT8_3(O) arg(O)
#define INPUT8_STR_3(O) arg(O)
#define STR8_3 "%.2X"

// output to register
#define OUTPUT_0(X,Y) reg[arg(X)] = Y

// output to pointer register
#define OUTPUT_1(X,Y) uint16_t val = Y;ram[reg[arg(X)]] = (val >> 8);ram[reg[arg(X)]+1] = (val & 0xFF)

// output to constant pointer
#define OUTPUT_2(X,Y) uint16_t val = Y;ram[combine(arg(X),arg(X+1))] = (val >> 8);ram[combine(arg(X),arg(X+1))+1] = (val & 0xFF)

// output to register
#define MOV8_OUTPUT_0(X,Y) reg[arg(X)] = (Y)

// output to pointer register
#define MOV8_OUTPUT_1(X,Y) ram[reg[arg(X)]] = (Y)

// output to constant pointer
#define MOV8_OUTPUT_2(X,Y) ram[combine(arg(X),arg(X+1))] = (Y)

// N: function name
// T: function id
// X: C operator
// Y: argument 1 type
// Z: argument 2 type
#define OPERATOR(N,T,X,Y,Z) \
void N##T() {\
    PRINTF(#N " " STR_##Y " " STR_##Z "\n", INPUT_STR_##Y(0), INPUT_STR_##Z(INPUTLEN_##Y));\
    reg[2] = INPUT_##Y(0) X INPUT_##Z(INPUTLEN_##Y);\
    reg[0] += 1 + INPUTLEN_##Y + INPUTLEN_##Z;\
}
#define MOV(T,Y,Z) \
void mov##T() {\
    PRINTF("mov " STR_##Y " " STR_##Z "\n", INPUT_STR_##Y(0), INPUT_STR_##Z(INPUTLEN_##Y));\
    int inc = !(Z == 0 && arg(INPUTLEN_##Y) == 0);\
    OUTPUT_##Z(INPUTLEN_##Y,INPUT_##Y(0));\
    if(inc) reg[0] += 1 + INPUTLEN_##Y + INPUTLEN_##Z;\
}
#define MOV8(T,Y,Z) \
void mov8##T() {\
    PRINTF("mov8 " STR8_##Y " " STR_##Z "\n", INPUT8_STR_##Y(0), INPUT_STR_##Z(INPUT8LEN_##Y));\
    int inc = !(Z == 0 && arg(INPUTLEN_##Y) == 0);\
    MOV8_OUTPUT_##Z(INPUT8LEN_##Y,INPUT8_##Y(0));\
    if(inc) reg[0] += 1 + INPUT8LEN_##Y + INPUTLEN_##Z;\
}
#define OUT(T,Y,Z) \
void out##T() {\
    PRINTF("out " STR8_##Y " " STR8_##Z "\n", INPUT8_STR_##Y(0), INPUT8_STR_##Z(INPUT8LEN_##Y));\
    addch(INPUT8_##Y(0));\
    reg[0] += 1 + INPUT8LEN_##Y + INPUT8LEN_##Z;\
}
#define OPERATORS(X,Y) \
OPERATOR(X, 0, Y, 0, 0)\
OPERATOR(X, 1, Y, 1, 0)\
OPERATOR(X, 2, Y, 2, 0)\
OPERATOR(X, 3, Y, 3, 0)\
OPERATOR(X, 4, Y, 0, 1)\
OPERATOR(X, 5, Y, 1, 1)\
OPERATOR(X, 6, Y, 2, 1)\
OPERATOR(X, 7, Y, 3, 1)\
OPERATOR(X, 8, Y, 0, 2)\
OPERATOR(X, 9, Y, 1, 2)\
OPERATOR(X, A, Y, 2, 2)\
OPERATOR(X, B, Y, 3, 2)\
OPERATOR(X, C, Y, 0, 3)\
OPERATOR(X, D, Y, 1, 3)\
OPERATOR(X, E, Y, 2, 3)\
OPERATOR(X, F, Y, 3, 3)
#define CMP(T,Y,Z) \
void cmp##T() {\
    PRINTF("cmp " STR_##Y " " STR_##Z "\n", INPUT_STR_##Y(0), INPUT_STR_##Z(INPUTLEN_##Y));\
    reg[2] = ((INPUT_##Y(0) > INPUT_##Z(INPUTLEN_##Y)) << 2) | ((INPUT_##Y(0) == INPUT_##Z(INPUTLEN_##Y)) << 1) | (INPUT_##Y(0) < INPUT_##Z(INPUTLEN_##Y));\
    reg[0] += 1 + INPUTLEN_##Y + INPUTLEN_##Z;\
}
#define IF(T,Y,Z) \
void if##T() {\
    PRINTF("if " STR_##Y " " STR_##Z "\n", INPUT_STR_##Y(0), INPUT_STR_##Z(INPUTLEN_##Y));\
    if(INPUT_##Y(0) == 0) {\
        PRINTF("false, jumping...\n");\
        reg[0] = INPUT_##Z(INPUTLEN_##Y);\
    } else {\
        PRINTF("true, continuing...\n");\
        reg[0] += 1 + INPUTLEN_##Y + INPUTLEN_##Z;\
    }\
    \
}
#define PUSH8(T,Y) \
void push8##T() {\
    PRINTF("push8 " STR8_##Y "\n", INPUT8_STR_##Y(0));\
    ram[reg[1]++] = INPUT8_##Y(0);\
    reg[0] += 1 + INPUT8LEN_##Y;\
}
#define POP8(T,Y) \
void pop8##T() {\
    PRINTF("pop8 " STR_##Y "\n", INPUT_STR_##Y(0));\
    reg[1]--;\
    int inc = arg(0) != 0;\
    MOV8_OUTPUT_##Y(0, ram[reg[1]]);\
    if(inc) {reg[0] += 1 + INPUTLEN_##Y;}\
}
#define PEEK8(T,Y) \
void peek8##T() {\
    PRINTF("peek8 " STR_##Y "\n", INPUT_STR_##Y(0));\
    reg[2] = ram[(((int)reg[1]) + ((int)INPUT_##Y(0))) & 0xFFFF];\
    reg[0] += 1 + INPUTLEN_##Y;\
}
#define PUSH(T,Y) \
void push##T() {\
    PRINTF("push " STR_##Y "\n", INPUT_STR_##Y(0));\
    uint16_t i = INPUT_##Y(0);\
    ram[reg[1]++] = i >> 8;\
    ram[reg[1]++] = i & 0xFF;\
    reg[0] += 1 + INPUTLEN_##Y;\
}
#define POP(T,Y) \
void pop##T() {\
    PRINTF("pop " STR_##Y "\n", INPUT_STR_##Y(0));\
    reg[1] -= 2;\
    int inc = arg(0) != 0;\
    OUTPUT_##Y(0, combine(ram[reg[1]], ram[reg[1] + 1]));\
    if(inc) {reg[0] += 1 + INPUTLEN_##Y;}\
}
#define PEEK(T,Y) \
void peek##T() {\
    PRINTF("peek " STR_##Y "\n", INPUT_STR_##Y(0));\
    reg[2] = combine(ram[(((int)reg[1]) + ((int)INPUT_##Y(0))) & 0xFFFF], ram[(((int)reg[1]) + ((int)INPUT_##Y(0)) + 1) & 0xFFFF]);\
    reg[0] += 1 + INPUTLEN_##Y;\
}

PUSH8(0, 0)
PUSH8(1, 1)
PUSH8(2, 2)
PUSH8(3, 3)
POP8(4, 0)
POP8(5, 1)
POP8(6, 2)
PEEK8(7, 3)

PUSH(8, 0)
PUSH(9, 1)
PUSH(A, 2)
PUSH(B, 3)
POP(C, 0)
POP(D, 1)
POP(E, 2)
PEEK(F, 3)

MOV8(0, 0, 0)
MOV8(1, 1, 0)
MOV8(2, 2, 0)
MOV8(3, 3, 0)
MOV8(4, 0, 1)
MOV8(5, 1, 1)
MOV8(6, 2, 1)
MOV8(7, 3, 1)
MOV8(8, 0, 2)
MOV8(9, 1, 2)
MOV8(A, 2, 2)
MOV8(B, 3, 2)

MOV(0, 0, 0)
MOV(1, 1, 0)
MOV(2, 2, 0)
MOV(3, 3, 0)
MOV(4, 0, 1)
MOV(5, 1, 1)
MOV(6, 2, 1)
MOV(7, 3, 1)
MOV(8, 0, 2)
MOV(9, 1, 2)
MOV(A, 2, 2)
MOV(B, 3, 2)

OUT(0, 0, 0)
OUT(1, 1, 0)
OUT(2, 2, 0)
OUT(3, 3, 0)
OUT(4, 0, 1)
OUT(5, 1, 1)
OUT(6, 2, 1)
OUT(7, 3, 1)
OUT(8, 0, 2)
OUT(9, 1, 2)
OUT(A, 2, 2)
OUT(B, 3, 2)
OUT(C, 0, 3)
OUT(D, 1, 3)
OUT(E, 2, 3)
OUT(F, 3, 3)

OPERATORS(add, +)
OPERATORS(sub, -)
OPERATORS(mul, *)
OPERATORS(div, /)
OPERATORS(mod, %)
OPERATORS(and, &)
OPERATORS(or, |)
OPERATORS(xor, ^)
OPERATORS(lsh, <<)
OPERATORS(rsh, >>)

CMP(0, 0, 0)
CMP(1, 1, 0)
CMP(2, 2, 0)
CMP(3, 3, 0)
CMP(4, 0, 1)
CMP(5, 1, 1)
CMP(6, 2, 1)
CMP(7, 3, 1)
CMP(8, 0, 2)
CMP(9, 1, 2)
CMP(A, 2, 2)
CMP(B, 3, 2)
CMP(C, 0, 3)
CMP(D, 1, 3)
CMP(E, 2, 3)
CMP(F, 3, 3)

IF(0, 0, 0)
IF(1, 1, 0)
IF(2, 2, 0)
IF(3, 3, 0)
IF(4, 0, 1)
IF(5, 1, 1)
IF(6, 2, 1)
IF(7, 3, 1)
IF(8, 0, 2)
IF(9, 1, 2)
IF(A, 2, 2)
IF(B, 3, 2)
IF(C, 0, 3)
IF(D, 1, 3)
IF(E, 2, 3)
IF(F, 3, 3)

/*#include "instructions/mov8.c"
#include "instructions/mov.c"
#include "instructions/out.c"
#include "instructions/add.c"
#include "instructions/sub.c"
#include "instructions/mul.c"
#include "instructions/div.c"
#include "instructions/mod.c"
#include "instructions/and.c"
#include "instructions/or.c"
#include "instructions/xor.c"
#include "instructions/lsh.c"
#include "instructions/rsh.c"
#include "instructions/less.c"
#include "instructions/more.c"
#include "instructions/if.c"*/

#include "instruction_table.h"

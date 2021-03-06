void (*instructions[256])() = {
    push80,// 0x00
    push81,// 0x01
    push82,// 0x02
    push83,// 0x03
    pop84, // 0x04
    pop85, // 0x05
    pop86, // 0x06
    peek87,// 0x07
    push8, // 0x08
    push9, // 0x09
    pushA, // 0x0A
    pushB, // 0x0B
    popC,  // 0x0C
    popD,  // 0x0D
    popE,  // 0x0E
    peekF, // 0x0F
    mov80, // 0x10
    mov81, // 0x11
    mov82, // 0x12
    mov83, // 0x13
    mov84, // 0x14
    mov85, // 0x15
    mov86, // 0x16
    mov87, // 0x17
    mov88, // 0x18
    mov89, // 0x19
    mov8A, // 0x1A
    mov8B, // 0x1B
    end,   // 0x1C
    end,   // 0x1D
    end,   // 0x1E
    end,   // 0x1F
    mov0,  // 0x20
    mov1,  // 0x21
    mov2,  // 0x22
    mov3,  // 0x23
    mov4,  // 0x24
    mov5,  // 0x25
    mov6,  // 0x26
    mov7,  // 0x27
    mov8,  // 0x28
    mov9,  // 0x29
    movA,  // 0x2A
    movB,  // 0x2B
    end,   // 0x2C
    end,   // 0x2D
    end,   // 0x2E
    end,   // 0x2F
    out0,  // 0x30
    out1,  // 0x31
    out2,  // 0x32
    out3,  // 0x33
    out4,  // 0x34
    out5,  // 0x35
    out6,  // 0x36
    out7,  // 0x37
    out8,  // 0x38
    out9,  // 0x39
    outA,  // 0x3A
    outB,  // 0x3B
    outC,  // 0x3C
    outD,  // 0x3D
    outE,  // 0x3E
    outF,  // 0x3F
    add0,  // 0x40
    add1,  // 0x41
    add2,  // 0x42
    add3,  // 0x43
    add4,  // 0x44
    add5,  // 0x45
    add6,  // 0x46
    add7,  // 0x47
    add8,  // 0x48
    add9,  // 0x49
    addA,  // 0x4A
    addB,  // 0x4B
    addC,  // 0x4C
    addD,  // 0x4D
    addE,  // 0x4E
    end,   // 0x4F
    sub0,  // 0x50
    sub1,  // 0x51
    sub2,  // 0x52
    sub3,  // 0x53
    sub4,  // 0x54
    sub5,  // 0x55
    sub6,  // 0x56
    sub7,  // 0x57
    sub8,  // 0x58
    sub9,  // 0x59
    subA,  // 0x5A
    subB,  // 0x5B
    subC,  // 0x5C
    subD,  // 0x5D
    subE,  // 0x5E
    end,   // 0x5F
    mul0,  // 0x60
    mul1,  // 0x61
    mul2,  // 0x62
    mul3,  // 0x63
    mul4,  // 0x64
    mul5,  // 0x65
    mul6,  // 0x66
    mul7,  // 0x67
    mul8,  // 0x68
    mul9,  // 0x69
    mulA,  // 0x6A
    mulB,  // 0x6B
    mulC,  // 0x6C
    mulD,  // 0x6D
    mulE,  // 0x6E
    end,   // 0x6F
    div0,  // 0x70
    div1,  // 0x71
    div2,  // 0x72
    div3,  // 0x73
    div4,  // 0x74
    div5,  // 0x75
    div6,  // 0x76
    div7,  // 0x77
    div8,  // 0x78
    div9,  // 0x79
    divA,  // 0x7A
    divB,  // 0x7B
    divC,  // 0x7C
    divD,  // 0x7D
    divE,  // 0x7E
    end,   // 0x7F
    mod0,  // 0x80
    mod1,  // 0x81
    mod2,  // 0x82
    mod3,  // 0x83
    mod4,  // 0x84
    mod5,  // 0x85
    mod6,  // 0x86
    mod7,  // 0x87
    mod8,  // 0x88
    mod9,  // 0x89
    modA,  // 0x8A
    modB,  // 0x8B
    modC,  // 0x8C
    modD,  // 0x8D
    modE,  // 0x8E
    end,   // 0x8F
    and0,  // 0x90
    and1,  // 0x91
    and2,  // 0x92
    and3,  // 0x93
    and4,  // 0x94
    and5,  // 0x95
    and6,  // 0x96
    and7,  // 0x97
    and8,  // 0x98
    and9,  // 0x99
    andA,  // 0x9A
    andB,  // 0x9B
    andC,  // 0x9C
    andD,  // 0x9D
    andE,  // 0x9E
    end,   // 0x9F
    or0,   // 0xA0
    or1,   // 0xA1
    or2,   // 0xA2
    or3,   // 0xA3
    or4,   // 0xA4
    or5,   // 0xA5
    or6,   // 0xA6
    or7,   // 0xA7
    or8,   // 0xA8
    or9,   // 0xA9
    orA,   // 0xAA
    orB,   // 0xAB
    orC,   // 0xAC
    orD,   // 0xAD
    orE,   // 0xAE
    end,   // 0xAF
    xor0,  // 0xB0
    xor1,  // 0xB1
    xor2,  // 0xB2
    xor3,  // 0xB3
    xor4,  // 0xB4
    xor5,  // 0xB5
    xor6,  // 0xB6
    xor7,  // 0xB7
    xor8,  // 0xB8
    xor9,  // 0xB9
    xorA,  // 0xBA
    xorB,  // 0xBB
    xorC,  // 0xBC
    xorD,  // 0xBD
    xorE,  // 0xBE
    end,   // 0xBF
    lsh0,  // 0xC0
    lsh1,  // 0xC1
    lsh2,  // 0xC2
    lsh3,  // 0xC3
    lsh4,  // 0xC4
    lsh5,  // 0xC5
    lsh6,  // 0xC6
    lsh7,  // 0xC7
    lsh8,  // 0xC8
    lsh9,  // 0xC9
    lshA,  // 0xCA
    lshB,  // 0xCB
    lshC,  // 0xCC
    lshD,  // 0xCD
    lshE,  // 0xCE
    end,   // 0xCF
    rsh0,  // 0xD0
    rsh1,  // 0xD1
    rsh2,  // 0xD2
    rsh3,  // 0xD3
    rsh4,  // 0xD4
    rsh5,  // 0xD5
    rsh6,  // 0xD6
    rsh7,  // 0xD7
    rsh8,  // 0xD8
    rsh9,  // 0xD9
    rshA,  // 0xDA
    rshB,  // 0xDB
    rshC,  // 0xDC
    rshD,  // 0xDD
    rshE,  // 0xDE
    end,   // 0xDF
    cmp0,  // 0xE0
    cmp1,  // 0xE1
    cmp2,  // 0xE2
    cmp3,  // 0xE3
    cmp4,  // 0xE4
    cmp5,  // 0xE5
    cmp6,  // 0xE6
    cmp7,  // 0xE7
    cmp8,  // 0xE8
    cmp9,  // 0xE9
    cmpA,  // 0xEA
    cmpB,  // 0xEB
    cmpC,  // 0xEC
    cmpD,  // 0xED
    cmpE,  // 0xEE
    end,   // 0xEF
    if0,   // 0xF0
    if1,   // 0xF1
    if2,   // 0xF2
    if3,   // 0xF3
    if4,   // 0xF4
    if5,   // 0xF5
    if6,   // 0xF6
    if7,   // 0xF7
    if8,   // 0xF8
    if9,   // 0xF9
    ifA,   // 0xFA
    ifB,   // 0xFB
    ifC,   // 0xFC
    ifD,   // 0xFD
    ifE,   // 0xFE
    end    // 0xFF
};

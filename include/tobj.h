/* Estructura de objeto (TObj): generada de ghidra/scripts/DefineObj.java. Tamano 0xC0. */
#ifndef TOBJ_H
#define TOBJ_H

typedef struct { unsigned short frac; short whole; } FixParts;
typedef union { int raw; FixParts p; } Fix16;

typedef struct TObj {
    unsigned char active;   /* +0x00 */
    unsigned char visible;   /* +0x01 */
    unsigned char type;   /* +0x02 */
    unsigned char subtype;   /* +0x03 */
    unsigned char b04;   /* +0x04 */
    unsigned char step;   /* +0x05 */
    unsigned char state;   /* +0x06 */
    unsigned char substep;   /* +0x07 */
    short w08;   /* +0x08 */
    unsigned char b0a;   /* +0x0a */
    unsigned char b0b;   /* +0x0b */
    unsigned char b0c;   /* +0x0c */
    unsigned char b0d;   /* +0x0d */
    unsigned char _pad0e[0x1];
    unsigned char b0f;   /* +0x0f */
    Fix16 a;   /* +0x10 */
    Fix16 y;   /* +0x14 */
    Fix16 b;   /* +0x18 */
    unsigned char category;   /* +0x1c */
    unsigned char b1d;   /* +0x1d */
    short w1e;   /* +0x1e */
    short timer;   /* +0x20 */
    short w22;   /* +0x22 */
    void * anim;   /* +0x24 */
    void * movetab;   /* +0x28 */
    unsigned short animTimer;   /* +0x2c */
    unsigned short animFrame;   /* +0x2e */
    int d30;   /* +0x30 */
    int d34;   /* +0x34 */
    int d38;   /* +0x38 */
    int d3c;   /* +0x3c */
    Fix16 * h;   /* +0x40 */
    Fix16 * d;   /* +0x44 */
    short w48;   /* +0x48 */
    short w4a;   /* +0x4a */
    short w4c;   /* +0x4c */
    short w4e;   /* +0x4e */
    short w50;   /* +0x50 */
    short w52;   /* +0x52 */
    unsigned char _pad54[0x2];
    short w56;   /* +0x56 */
    unsigned short w58;   /* +0x58 */
    unsigned char _pad5a[0x2];
    short w5c;   /* +0x5c */
    unsigned char _pad5e[0x2];
    int d60;   /* +0x60 */
    int d64;   /* +0x64 */
    unsigned char b68;   /* +0x68 */
    unsigned char b69;   /* +0x69 */
    unsigned char b6a;   /* +0x6a */
    unsigned char b6b;   /* +0x6b */
    short box0;   /* +0x6c */
    short box1;   /* +0x6e */
    short box2;   /* +0x70 */
    short box3;   /* +0x72 */
    short w74;   /* +0x74 */
    short w76;   /* +0x76 */
    short w78;   /* +0x78 */
    short w7a;   /* +0x7a */
    short velX;   /* +0x7c */
    short velY;   /* +0x7e */
    short velH;   /* +0x80 */
    short velV;   /* +0x82 */
    int d84;   /* +0x84 */
    int d88;   /* +0x88 */
    int d8c;   /* +0x8c */
    int d90;   /* +0x90 */
    int d94;   /* +0x94 */
    short w98;   /* +0x98 */
    short w9a;   /* +0x9a */
    unsigned char b9c;   /* +0x9c */
    unsigned char b9d;   /* +0x9d */
    unsigned char b9e;   /* +0x9e */
    unsigned char b9f;   /* +0x9f */
    int da0;   /* +0xa0 */
    unsigned char ba4;   /* +0xa4 */
    unsigned char ba5;   /* +0xa5 */
    unsigned char ba6;   /* +0xa6 */
    unsigned char ba7;   /* +0xa7 */
    short wa8;   /* +0xa8 */
    short waa;   /* +0xaa */
    short wac;   /* +0xac */
    short wae;   /* +0xae */
    short wb0;   /* +0xb0 */
    short wb2;   /* +0xb2 */
    short wb4;   /* +0xb4 */
    short wb6;   /* +0xb6 */
    short wb8;   /* +0xb8 */
    short wba;   /* +0xba */
    short wbc;   /* +0xbc */
    unsigned char bbe;   /* +0xbe */
    unsigned char bbf;   /* +0xbf */
} TObj;

#endif

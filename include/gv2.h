/* GV2 (0x8009D2A0, size 0x40): small game variables at 0x8009D2A0 (button/state bytes around 0x8009D2B0). Fields are named by their address until their meaning is known. Plain C types, self-contained. */
#ifndef _TOMBA_GV2_H
#define _TOMBA_GV2_H
typedef struct GV2Block {
    unsigned char _pad00[0x6];   /* 0x00 */
    unsigned short sa6;   /* 0x06 0x8009D2A6 */
    unsigned char _pad08[0x6];   /* 0x08 */
    unsigned char bae;   /* 0x0e 0x8009D2AE */
    unsigned char baf;   /* 0x0f 0x8009D2AF */
    unsigned char bb0;   /* 0x10 0x8009D2B0 */
    unsigned char bb1;   /* 0x11 0x8009D2B1 */
    unsigned char bb2;   /* 0x12 0x8009D2B2 */
    unsigned char bb3;   /* 0x13 0x8009D2B3 */
    unsigned char _pad14[0xc];   /* 0x14 */
    unsigned short sc0;   /* 0x20 0x8009D2C0 */
    unsigned char bc2;   /* 0x22 0x8009D2C2 */
    unsigned char bc3;   /* 0x23 0x8009D2C3 */
    unsigned char _pad24[0x16];   /* 0x24 */
    unsigned short sda;   /* 0x3a 0x8009D2DA */
    unsigned char _pad3c[0x4];   /* 0x3c */
} GV2Block;
extern GV2Block GV2;
#endif

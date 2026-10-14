/* GVARS (0x8009C930..0x8009C95F): the block of small global game variables right before GAME (0x8009C960): flags/counters
   (bytes at +0x08..+0x13, halfwords at +0x14/+0x16) and object pointers (+0x18..+0x24). Fields are named by their address
   (b3f = 0x8009C93F ...) until their meaning is known. Plain C types, self-contained. */
#ifndef _TOMBA_GVARS_H
#define _TOMBA_GVARS_H
typedef struct GVars {
    unsigned char b30;          /* 0x00 0x8009C930 */
    unsigned char _pad31[3];    /* 0x01 */
    int i34;                    /* 0x04 */
    unsigned char b38;          /* 0x08 */
    unsigned char b39;          /* 0x09 */
    unsigned char b3a;          /* 0x0A */
    unsigned char b3b;          /* 0x0B */
    unsigned char b3c;          /* 0x0C */
    unsigned char b3d;          /* 0x0D */
    unsigned char b3e;          /* 0x0E */
    unsigned char b3f;          /* 0x0F */
    unsigned char b40;          /* 0x10 */
    unsigned char b41;          /* 0x11 */
    unsigned char b42;          /* 0x12 */
    unsigned char b43;          /* 0x13 */
    short s44;                  /* 0x14 */
    short s46;                  /* 0x16 */
    void *p48;                  /* 0x18 */
    void *p4c;                  /* 0x1C */
    void *p50;                  /* 0x20 */
    void *p54;                  /* 0x24 */
    unsigned char b58;          /* 0x28 */
    unsigned char b59;          /* 0x29 */
    unsigned char _pad5a[2];    /* 0x2A */
    int i5c;                    /* 0x2C */
} GVars;
extern GVars GVARS;
#endif

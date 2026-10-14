/* GV6 (0x8009D600, size 0xB0): game variables at 0x8009D600 (s70/s74 = D_8009D670/74, the pad button halfwords). Fields are named by their address until their meaning is known. Plain C types, self-contained. */
#ifndef _TOMBA_GV6_H
#define _TOMBA_GV6_H
typedef struct GV6Block {
    short s00;   /* 0x00 0x8009D600 */
    unsigned char _pad02[0xe];   /* 0x02 */
    unsigned short s10;   /* 0x10 0x8009D610 */
    unsigned char _pad12[0x2];   /* 0x12 */
    unsigned char b14;   /* 0x14 0x8009D614 */
    unsigned char _pad15[0x5b];   /* 0x15 */
    unsigned short s70;   /* 0x70 0x8009D670 */
    unsigned char _pad72[0x2];   /* 0x72 */
    unsigned short s74;   /* 0x74 0x8009D674 */
    unsigned char _pad76[0x1e];   /* 0x76 */
    int i94;   /* 0x94 0x8009D694 */
    unsigned char _pad98[0x4];   /* 0x98 */
    int i9c;   /* 0x9c 0x8009D69C */
    int ia0;   /* 0xa0 0x8009D6A0 */
    unsigned char _pada4[0xc];   /* 0xa4 */
} GV6Block;
extern GV6Block GV6;
#endif

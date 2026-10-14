/* PLX (0x800A60F8, size 0x80): global variables. Fields are named by their address (b=u8, s=s16/u16, i=int, p=pointer + low 16 bits of the
   address) until their meaning is known; types are the ones the sources declared. Plain C types, self-contained. */
#ifndef _TOMBA_PLX_H
#define _TOMBA_PLX_H
typedef struct PLXBlock {
    unsigned char b60f8;   /* 0x0 0x800A60F8 */
    unsigned char _pad1[0x3];   /* 0x1 */
    unsigned short s60fc;   /* 0x4 0x800A60FC */
    unsigned char _pad6[0x1];   /* 0x6 */
    unsigned char b60ff;   /* 0x7 0x800A60FF */
    unsigned char b6100;   /* 0x8 0x800A6100 */
    unsigned char _pad9[0x1];   /* 0x9 */
    unsigned char b6102;   /* 0xa 0x800A6102 */
    unsigned char _padb[0x1];   /* 0xb */
    unsigned char b6104;   /* 0xc 0x800A6104 */
    unsigned char _padd[0x3];   /* 0xd */
    short s6108;   /* 0x10 0x800A6108 */
    short s610a;   /* 0x12 0x800A610A */
    unsigned char _pad14[0xc];   /* 0x14 */
    short s6118;   /* 0x20 0x800A6118 */
    unsigned char b611a;   /* 0x22 0x800A611A */
    unsigned char b611b;   /* 0x23 0x800A611B */
    void * p611c;   /* 0x24 0x800A611C */
    unsigned char _pad28[0x58];   /* 0x28 */
} PLXBlock;
extern PLXBlock PLX;
#endif

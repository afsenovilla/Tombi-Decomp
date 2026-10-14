/* SPAD (0x1F800000, size 0x400): global variables. Fields are named by their address (b=u8, s=s16/u16, i=int, p=pointer + low 16 bits of the
   address) until their meaning is known; types are the ones the sources declared. Plain C types, self-contained. */
#ifndef _TOMBA_SPAD_H
#define _TOMBA_SPAD_H
typedef struct SPADBlock {
    unsigned char b0;   /* 0x0 0x1F800000 */
    unsigned char _pad1[0x5f];   /* 0x1 */
    short s60;   /* 0x60 0x1F800060 */
    short s62;   /* 0x62 0x1F800062 */
    short s64;   /* 0x64 0x1F800064 */
    unsigned char _pad66[0xe];   /* 0x66 */
    int i74;   /* 0x74 0x1F800074 */
    int i78;   /* 0x78 0x1F800078 */
    unsigned char _pad7c[0x10];   /* 0x7c */
    int i8c;   /* 0x8c 0x1F80008C */
    unsigned char _pad90[0x30];   /* 0x90 */
    unsigned char bc0;   /* 0xc0 0x1F8000C0 */
    unsigned char _padc1[0x13];   /* 0xc1 */
    int id4;   /* 0xd4 0x1F8000D4 */
    int id8;   /* 0xd8 0x1F8000D8 */
    int idc;   /* 0xdc 0x1F8000DC */
    int ie0;   /* 0xe0 0x1F8000E0 */
    unsigned char _pade4[0x2];   /* 0xe4 */
    short se6;   /* 0xe6 0x1F8000E6 */
    unsigned char _pade8[0x2];   /* 0xe8 */
    short sea;   /* 0xea 0x1F8000EA */
    int iec;   /* 0xec 0x1F8000EC */
    int if0;   /* 0xf0 0x1F8000F0 */
    int if4;   /* 0xf4 0x1F8000F4 */
    unsigned char _padf8[0x20];   /* 0xf8 */
    unsigned char b118;   /* 0x118 0x1F800118 */
    unsigned char _pad119[0x4b];   /* 0x119 */
    void * p164;   /* 0x164 0x1F800164 */
    int i168;   /* 0x168 0x1F800168 */
    int i16c;   /* 0x16c 0x1F80016C */
    int i170;   /* 0x170 0x1F800170 */
    int i174;   /* 0x174 0x1F800174 */
    unsigned char _pad178[0x2];   /* 0x178 */
    unsigned short s17a;   /* 0x17a 0x1F80017A */
    unsigned char _pad17c[0x2];   /* 0x17c */
    unsigned short s17e;   /* 0x17e 0x1F80017E */
    unsigned char _pad180[0x6];   /* 0x180 */
    unsigned short s186;   /* 0x186 0x1F800186 */
    unsigned char _pad188[0x4];   /* 0x188 */
    int i18c;   /* 0x18c 0x1F80018C */
    int i190;   /* 0x190 0x1F800190 */
    unsigned char _pad194[0x4];   /* 0x194 */
    int i198;   /* 0x198 0x1F800198 */
    short s19c;   /* 0x19c 0x1F80019C */
    short s19e;   /* 0x19e 0x1F80019E */
    unsigned char b1a0;   /* 0x1a0 0x1F8001A0 */
    unsigned char _pad1a1[0x3];   /* 0x1a1 */
    unsigned char b1a4;   /* 0x1a4 0x1F8001A4 */
    unsigned char b1a5;   /* 0x1a5 0x1F8001A5 */
    unsigned char _pad1a6[0x1c];   /* 0x1a6 */
    unsigned char b1c2;   /* 0x1c2 0x1F8001C2 */
    unsigned char _pad1c3[0x1];   /* 0x1c3 */
    unsigned char b1c4;   /* 0x1c4 0x1F8001C4 */
    unsigned char b1c5;   /* 0x1c5 0x1F8001C5 */
    short s1c6;   /* 0x1c6 0x1F8001C6 */
    unsigned short s1c8;   /* 0x1c8 0x1F8001C8 */
    unsigned char _pad1ca[0x2];   /* 0x1ca */
    unsigned char b1cc;   /* 0x1cc 0x1F8001CC */
    unsigned char b1cd;   /* 0x1cd 0x1F8001CD */
    unsigned char _pad1ce[0x1];   /* 0x1ce */
    unsigned char b1cf;   /* 0x1cf 0x1F8001CF */
    unsigned char b1d0;   /* 0x1d0 0x1F8001D0 */
    unsigned char b1d1;   /* 0x1d1 0x1F8001D1 */
    unsigned char b1d2;   /* 0x1d2 0x1F8001D2 */
    unsigned char b1d3;   /* 0x1d3 0x1F8001D3 */
    unsigned char _pad1d4[0xc];   /* 0x1d4 */
    int i1e0;   /* 0x1e0 0x1F8001E0 */
    unsigned char _pad1e4[0x4];   /* 0x1e4 */
    short s1e8;   /* 0x1e8 0x1F8001E8 */
    unsigned char _pad1ea[0xa];   /* 0x1ea */
    short s1f4;   /* 0x1f4 0x1F8001F4 */
    short s1f6;   /* 0x1f6 0x1F8001F6 */
    unsigned short s1f8;   /* 0x1f8 0x1F8001F8 */
    unsigned char _pad1fa[0x2];   /* 0x1fa */
    unsigned short s1fc;   /* 0x1fc 0x1F8001FC */
    unsigned short s1fe;   /* 0x1fe 0x1F8001FE */
    unsigned char _pad200[0x1c];   /* 0x200 */
    void * p21c;   /* 0x21c 0x1F80021C */
    unsigned char _pad220[0x4];   /* 0x220 */
    void * p224;   /* 0x224 0x1F800224 */
    void * p228;   /* 0x228 0x1F800228 */
    unsigned char _pad22c[0xc];   /* 0x22c */
    short s238;   /* 0x238 0x1F800238 */
    unsigned char _pad23a[0xc];   /* 0x23a */
    short s246;   /* 0x246 0x1F800246 */
    unsigned short s248;   /* 0x248 0x1F800248 */
    unsigned char _pad24a[0x2];   /* 0x24a */
    short s24c;   /* 0x24c 0x1F80024C */
    unsigned char _pad24e[0x2];   /* 0x24e */
    unsigned short s250;   /* 0x250 0x1F800250 */
    unsigned char _pad252[0x2];   /* 0x252 */
    unsigned short s254;   /* 0x254 0x1F800254 */
    short s256;   /* 0x256 0x1F800256 */
    unsigned char _pad258[0x8];   /* 0x258 */
    void * p260;   /* 0x260 0x1F800260 */
    unsigned char _pad264[0x4];   /* 0x264 */
    void * p268;   /* 0x268 0x1F800268 */
    void * p26c;   /* 0x26c 0x1F80026C */
    unsigned char _pad270[0x8];   /* 0x270 */
    void * p278;   /* 0x278 0x1F800278 */
    unsigned char _pad27c[0x2];   /* 0x27c */
    short s27e;   /* 0x27e 0x1F80027E */
    short s280;   /* 0x280 0x1F800280 */
    unsigned short s282;   /* 0x282 0x1F800282 */
    short s284;   /* 0x284 0x1F800284 */
    short s286;   /* 0x286 0x1F800286 */
    unsigned char _pad288[0x2c];   /* 0x288 */
    int i2b4;   /* 0x2b4 0x1F8002B4 */
    unsigned char _pad2b8[0x18];   /* 0x2b8 */
    int i2d0;   /* 0x2d0 0x1F8002D0 */
    int i2d4;   /* 0x2d4 0x1F8002D4 */
    int i2d8;   /* 0x2d8 0x1F8002D8 */
    int i2dc;   /* 0x2dc 0x1F8002DC */
    int i2e0;   /* 0x2e0 0x1F8002E0 */
    int i2e4;   /* 0x2e4 0x1F8002E4 */
    int i2e8;   /* 0x2e8 0x1F8002E8 */
    unsigned char _pad2ec[0x4];   /* 0x2ec */
    int i2f0;   /* 0x2f0 0x1F8002F0 */
    unsigned char _pad2f4[0x4];   /* 0x2f4 */
    int i2f8;   /* 0x2f8 0x1F8002F8 */
    unsigned char _pad2fc[0x4];   /* 0x2fc */
    int i300;   /* 0x300 0x1F800300 */
    int i304;   /* 0x304 0x1F800304 */
    void * p308;   /* 0x308 0x1F800308 */
    void * p30c;   /* 0x30c 0x1F80030C */
    int i310;   /* 0x310 0x1F800310 */
    int i314;   /* 0x314 0x1F800314 */
    int i318;   /* 0x318 0x1F800318 */
    void * p31c;   /* 0x31c 0x1F80031C */
    unsigned char _pad320[0x14];   /* 0x320 */
    int i334;   /* 0x334 0x1F800334 */
    void * p338;   /* 0x338 0x1F800338 */
    void * p33c;   /* 0x33c 0x1F80033C */
    unsigned char _pad340[0x1c];   /* 0x340 */
    int i35c;   /* 0x35c 0x1F80035C */
    unsigned char _pad360[0x10];   /* 0x360 */
    int i370;   /* 0x370 0x1F800370 */
    int i374;   /* 0x374 0x1F800374 */
    int i378;   /* 0x378 0x1F800378 */
    unsigned char _pad37c[0x3c];   /* 0x37c */
    unsigned short s3b8;   /* 0x3b8 0x1F8003B8 */
    unsigned short s3ba;   /* 0x3ba 0x1F8003BA */
    short s3bc;   /* 0x3bc 0x1F8003BC */
    unsigned char _pad3be[0x2];   /* 0x3be */
    void * p3c0;   /* 0x3c0 0x1F8003C0 */
    unsigned short s3c4;   /* 0x3c4 0x1F8003C4 */
    unsigned short s3c6;   /* 0x3c6 0x1F8003C6 */
    unsigned short s3c8;   /* 0x3c8 0x1F8003C8 */
    unsigned char _pad3ca[0x4];   /* 0x3ca */
    unsigned char b3ce;   /* 0x3ce 0x1F8003CE */
    unsigned char _pad3cf[0x1];   /* 0x3cf */
    unsigned char b3d0;   /* 0x3d0 0x1F8003D0 */
    unsigned char b3d1;   /* 0x3d1 0x1F8003D1 */
    unsigned char _pad3d2[0x2e];   /* 0x3d2 */
} SPADBlock;
extern SPADBlock SPAD;
#endif

/* Task / game task (the fixed one at 0x801FD800 is TASK_TABLE[0]; TASK_C = the current one) as seen from sources
   that include tobj.h (plain C types). Same layout and US names as `Task` in game.h; the halfwords at +0x48..+0x4E are
   signed here because the sources that use them were matched with TObj's `short w48..w4e`.
   +0x4A = 1 gameplay, +0x4C = mode (0 debug select, 1/2/4/5/6 area modes, 7 transition), +0x4E = step. */
#ifndef _TOMBA_GAMETASK_H
#define _TOMBA_GAMETASK_H
typedef struct GameTask {
    short status; /* +0x0 */
    short sleepTimer; /* +0x2 */
    int task_id; /* +0x4 */
    int task_sp; /* +0x8 */
    int task_func; /* +0xC */
    int task_gp; /* +0x10 */
    volatile unsigned short unk14; /* +0x14 */
    unsigned char unk16; /* +0x16 */
    unsigned char unk17; /* +0x17 */
    unsigned char unk18; /* +0x18 */
    unsigned char unk19; /* +0x19 */
    unsigned char unk1a; /* +0x1A */
    unsigned char unk1b; /* +0x1B */
    unsigned char unk1c; /* +0x1C */
    unsigned char unk1d; /* +0x1D */
    unsigned char unk1e; /* +0x1E */
    unsigned char unk1f; /* +0x1F */
    unsigned char unk20; /* +0x20 */
    unsigned char unk21; /* +0x21 */
    unsigned char unk22; /* +0x22 */
    unsigned char unk23; /* +0x23 */
    unsigned char unk24; /* +0x24 */
    unsigned char unk25; /* +0x25 */
    unsigned char unk26; /* +0x26 */
    unsigned char unk27; /* +0x27 */
    unsigned char unk28; /* +0x28 */
    unsigned char unk29; /* +0x29 */
    unsigned char unk2a; /* +0x2A */
    unsigned char unk2b; /* +0x2B */
    unsigned char unk2c; /* +0x2C */
    unsigned char unk2d; /* +0x2D */
    unsigned char unk2e; /* +0x2E */
    unsigned char unk2f; /* +0x2F */
    unsigned char unk30; /* +0x30 */
    unsigned char unk31; /* +0x31 */
    unsigned char unk32; /* +0x32 */
    unsigned char unk33; /* +0x33 */
    unsigned char unk34; /* +0x34 */
    unsigned char unk35; /* +0x35 */
    unsigned char unk36; /* +0x36 */
    unsigned char unk37; /* +0x37 */
    unsigned char unk38; /* +0x38 */
    unsigned char unk39; /* +0x39 */
    unsigned char unk3a; /* +0x3A */
    unsigned char unk3b; /* +0x3B */
    unsigned char unk3c; /* +0x3C */
    unsigned char unk3d; /* +0x3D */
    unsigned char unk3e; /* +0x3E */
    unsigned char unk3f; /* +0x3F */
    unsigned char unk40; /* +0x40 */
    unsigned char unk41; /* +0x41 */
    unsigned char unk42; /* +0x42 */
    unsigned char unk43; /* +0x43 */
    unsigned char unk44; /* +0x44 */
    unsigned char unk45; /* +0x45 */
    unsigned char unk46; /* +0x46 */
    unsigned char unk47; /* +0x47 */
    short state0; /* +0x48 */
    short state1; /* +0x4A */
    short mode; /* +0x4C */
    short step;  /* +0x4E, u16 / volatile u16 in game.h */
    unsigned char unk50; /* +0x50 */
    unsigned char unk51; /* +0x51 */
    unsigned char unk52; /* +0x52 */
    unsigned char unk53; /* +0x53 */
    unsigned char unk54; /* +0x54 */
    unsigned char unk55; /* +0x55 */
    unsigned char unk56; /* +0x56 */
    unsigned char unk57; /* +0x57 */
    unsigned short timer; /* +0x58 */
    unsigned short unk5A; /* +0x5A */
    unsigned short countdown; /* +0x5C */
    unsigned short unk5E; /* +0x5E */
    short unk60; /* +0x60 */
    short unk62; /* +0x62 */
    unsigned short unk64; /* +0x64 */
    unsigned char unk66; /* +0x66 */
    unsigned char unk67; /* +0x67 */
    unsigned char loadGameSelected; /* +0x68 */
    unsigned char titleScreenSelectedOption; /* +0x69 */
    unsigned char unk6A; /* +0x6A */
    unsigned char unk6B; /* +0x6B */
    unsigned char unk6C; /* +0x6C */
    unsigned char unk6D; /* +0x6D */
    unsigned char unk6E; /* +0x6E */
    unsigned char unk6F; /* +0x6F */
} GameTask;
extern GameTask *TASK_C;
#endif

// VERSION SLPM_80165 x00 800FF700 480
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerBittenJumpType0E (src/playerBittenJumpType0E.c): the bitten object always goes to step 1,
// the jump also moves by +0x7C and runs two more per-frame helpers, fixed button bit 0x80, and +0x8C is cleared
// instead of calling playerSetFallLean.
extern void FUN_8001e560(int, int), FUN_800ee680(unsigned char *), FUN_8001fec0(unsigned char *), playerSetFallLean(unsigned char *), FUN_80040278(unsigned char *, int, int);
extern int FUN_8003facc(unsigned char *);
extern void func_8010A570(unsigned char *), func_8010A91C(unsigned char *), func_8002047C(unsigned char *), func_800205A8(unsigned char *);
extern unsigned char *BITTEN_OBJECT;
extern short GVARS_PLAYER_DRIFT_H, GVARS_PLAYER_DRIFT_Y;
extern int GAME_AREA00_EVENT_CONTROL;
extern unsigned short PAD_PRESSED;

void func_800FF700(unsigned char *o)
{
    unsigned char *q;
    switch (o[7]) {
    case 0:
        q = BITTEN_OBJECT;
        *(short *)(o + 0x7c) = 0;
        q[5] = 1;
        FUN_8001e560(0x22, 0x23);
        o[7] = o[7] + 1;
    case 1:
        **(int **)(o + 0x40) += GVARS_PLAYER_DRIFT_H << 8;
        *(int *)(o + 0x14) += GVARS_PLAYER_DRIFT_Y << 8;
        func_8010A570(o);
        **(int **)(o + 0x40) += *(short *)(o + 0x7c) << 8;
        func_8010A91C(o);
        func_8002047C(o);
        func_800205A8(o);
        if (*(short *)(o + 0x7e) > 0) {
            *(int *)(o + 0x84) = 0;
            o[0x9c] = 2;
            *(short *)(o + 0x7e) = 0;
            o[0xac] = 1;
            if ((GAME_AREA00_EVENT_CONTROL & 0x40) != 0 && (*(volatile unsigned short *)&PAD_PRESSED & 0x80) != 0)
                o[0xa7] = 1;
            *(int *)(o + 0x8c) = 0;
            o[5] = 2;
            o[6] = 3;
            o[7] = 0;
        }
        FUN_80040278(o, (*(short **)(o + 0x40))[1], (short)(*(unsigned short *)(o + 0x16) + 0x10));
        if (FUN_8003facc(o) != 0) {
            *(int *)(o + 0x84) = 0;
            o[0x9c] = 2;
            o[0xac] = 1;
            *(short *)(o + 0x7e) = 0;
            if ((GAME_AREA00_EVENT_CONTROL & 0x40) != 0 && (*(volatile unsigned short *)&PAD_PRESSED & 0x80) != 0)
                o[0xa7] = 1;
            *(int *)(o + 0x8c) = 0;
            o[5] = 2;
            o[6] = 3;
            o[7] = 0;
        }
    }
}
#endif

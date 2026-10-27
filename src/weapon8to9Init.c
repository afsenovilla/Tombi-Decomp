// FUNC 80035198 348 MAIN0
// MATCHING 80035198 348
extern int FUN_8005efe4(void);
extern unsigned short FUN_8005e420(int, int);
extern short *WEAPON_STATE;
extern short WEAPON_BEND_PARAMS[];
extern unsigned short *PLAYER_H, *PLAYER_D;
extern unsigned short PLAYER_Y[];

static __inline__ short getm(void)
{
    if (FUN_8005efe4() == 1) return 0x80;
    if (FUN_8005efe4() == 2) return 0x80;
    return 0x20;
}

void weapon8to9Init(unsigned char *o)
{
    short *p;
    WEAPON_STATE = (short *)(o + 0xac);
    if (o[6] == 0) {
        if (FUN_8005efe4() != 1)
            FUN_8005efe4();
        p = WEAPON_STATE;
        *(short *)(o + 0xb0) = 0;
        p[4] = getm();
        WEAPON_STATE[3] = FUN_8005e420(0x80, 0x1ef);
        WEAPON_STATE[5] = FUN_8005e420(0x80, 0x1f0);
        WEAPON_BEND_PARAMS[0] = 0xf;
        WEAPON_BEND_PARAMS[1] = 8;
        WEAPON_BEND_PARAMS[2] = 0x5a;
        *(signed char *)(o + 0xf) = -9;
        o[0xa] = 0;
        (*(unsigned short **)(o + 0x40))[1] = PLAYER_H[1];
        *(unsigned short *)(o + 0x16) = PLAYER_Y[0] - 0x10;
        (*(unsigned short **)(o + 0x44))[1] = PLAYER_D[1];
        *(short *)(o + 0x80) = 0;
        *(short *)(o + 0x82) = 0;
        *(short *)(o + 0x2e) = 0;
        o[1] = 1;
        o[6] = 0;
    }
}

// FUNC 80137f54 180 X000
// MATCHING 80137f54 180
// probably: unlayered variant 0 (D_8007C1C8[0], also entries 0x30..0x36): FUN_801361e4 spawns the three scene parts (types 0x17/0x18), FUN_801363e4 runs the message-box 7 scene once the player passes x 0xD8
extern short *DAT_800a6078;
extern void FUN_801363e4(unsigned char *);
extern void FUN_801361e4(unsigned char *);
extern void FUN_80018980(void);

static __inline__ int chk(void)
{
    return DAT_800a6078[1] > 0xd8;
}

void FUN_80137f54(unsigned char *o)
{
    switch (o[4]) {
    case 0:
        FUN_801361e4(o);
        break;
    case 1:
        if (chk())
            FUN_801363e4(o);
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        FUN_80018980();
        break;
    }
}

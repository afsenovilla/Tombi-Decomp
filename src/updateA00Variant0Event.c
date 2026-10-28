// FUNC 80137f54 180 X000
// MATCHING 80137f54 180
// probably: unlayered variant 0 (D_8007C1C8[0], also entries 0x30..0x36): a00Variant0EventSpawn spawns the three scene parts (types 0x17/0x18), a00Variant0EventScene runs the message-box 7 scene once the player passes x 0xD8
extern short *PLAYER_H;
extern void a00Variant0EventScene(unsigned char *);
extern void a00Variant0EventSpawn(unsigned char *);
extern void FUN_80018980(void);

static __inline__ int chk(void)
{
    return PLAYER_H[1] > 0xd8;
}

void updateA00Variant0Event(unsigned char *o)
{
    switch (o[4]) {
    case 0:
        a00Variant0EventSpawn(o);
        break;
    case 1:
        if (chk())
            a00Variant0EventScene(o);
        break;
    case 2:
        o[4] = 3;
        break;
    case 3:
        FUN_80018980();
        break;
    }
}

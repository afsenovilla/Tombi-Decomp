// FUNC 800fb370 164 X000
typedef struct P { char pad[0x2c]; unsigned short w2c; unsigned short w2e; } P;
extern P *DAT_8009c330;
extern void FUN_800efc04();
extern void FUN_8001fe94();

void FUN_800fb370(char *o)
{
    unsigned u;
    if (*(short *)(o + 0xb4) > 0x28) {
        u = (*(int *)(o + 0x84) >> 3) & 0xf;
        DAT_8009c330->w2c = 0x31;
        FUN_800efc04();
    } else {
        DAT_8009c330->w2c = 0x31;
        FUN_800efc04(o);
        u = 0xf;
    }
    FUN_8001fe94(o, u);
    DAT_8009c330->w2e = DAT_8009c330->w2c;
}

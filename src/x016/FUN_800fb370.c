// FUNC 800fb370 164 X016
// MATCHING 800fb370 164
typedef struct P { char pad[0x2c]; unsigned short w2c; unsigned short w2e; } P;
extern P *DAT_8009c330;
extern void FUN_800efc04();
extern void FUN_8001fe94();
void FUN_800fb370(char *o)
{
    if (*(short *)(o + 0xb4) > 0x28) {
        unsigned u = (*(int *)(o + 0x84) >> 3) & 0xf;
        DAT_8009c330->w2c = 0x31;
        FUN_800efc04(o);
        FUN_8001fe94(o, u);
    } else {
        DAT_8009c330->w2c = 0x31;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0xf);
    }
    DAT_8009c330->w2e = DAT_8009c330->w2c;
}

// FUNC 800eea7c 104 X014
// MATCHING 800eea7c 104
typedef struct D { char pad[0x2c]; unsigned short a; unsigned short b; } D;
extern D *DAT_8009c330;
extern void FUN_800efc04(int);
extern void FUN_8001fe94(int, int);
void FUN_800eea7c(int o, short a, short c)
{
    DAT_8009c330->a = a;
    FUN_800efc04(o);
    FUN_8001fe94(o, c);
    DAT_8009c330->b = DAT_8009c330->a;
}

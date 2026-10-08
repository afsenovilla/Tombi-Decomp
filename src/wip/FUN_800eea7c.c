// FUNC 800eea7c 104 X000
typedef struct D { char pad[0x2c]; unsigned short a; unsigned short b; } D;
extern D *DAT_80096330;
extern void FUN_800efc04(void);
extern void FUN_8001fe94(int, int);
void FUN_800eea7c(int o, short a, short c)
{
    DAT_80096330->a = a;
    FUN_800efc04();
    FUN_8001fe94(o, c);
    DAT_80096330->b = DAT_80096330->a;
}

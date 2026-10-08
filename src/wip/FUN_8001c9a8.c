// FUNC 8001c9a8 88 MAIN0
typedef struct S { char pad[0x4c]; unsigned short a; unsigned short b; } S;
extern S *DAT_1f8001d4;
extern unsigned short DAT_1f8003b8;
extern unsigned short DAT_1f8003ba;
extern char DAT_800a45d8;
extern char DAT_800a45d9;
extern short DAT_800a45ea;
extern short DAT_800a45ee;

void FUN_8001c9a8(short x)
{
    S *s = DAT_1f8001d4;
    DAT_1f8003b8 = s->a;
    DAT_1f8003ba = s->b;
    DAT_800a45d8 = 0;
    DAT_800a45d9 = 0;
    DAT_800a45ea = x;
    DAT_800a45ee = 0;
    s->a = 3;
    s->b = 0;
}

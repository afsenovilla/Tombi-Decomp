// FUNC 8001dad4 100 MAIN0
extern char DAT_800b3e28[], DAT_800c3e28[], DAT_800d3e28[], DAT_800d5e28[];

typedef struct S {
    void *p0, *p4;
    int i8;
    void *pc, *p10;
    int i14;
    short s18, s1a;
    char pad[4];
    short s20, s22;
    char pad2[0x30 - 0x24];
    short s30, s32;
    int i34;
} S;

void FUN_8001dad4(S *o, unsigned short a, short b, short c, short d)
{
    o->p0 = DAT_800b3e28;
    o->p4 = DAT_800c3e28;
    o->pc = DAT_800d3e28;
    o->p10 = DAT_800d5e28;
    o->i8 = 0;
    o->i14 = 0;
    o->s18 = a;
    o->s1a = b;
    o->s20 = c;
    o->i34 = 0;
    o->s30 = 0x10;
    o->s32 = 0xe0;
    o->s22 = d;
}

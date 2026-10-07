// FUNC 800188e0 84 MAIN0
typedef struct S {
    int a, b, c, d;
    char pad[0x1c - 0x10];
    char e;
    char pad2[0x9c - 0x1d];
    char f, g, h, i;
} S;
extern unsigned short DAT_1f800238;
extern int *DAT_1f800208;

void FUN_800188e0(S *o)
{
    o->a = 0; o->b = 0; o->c = 0; o->d = 0;
    o->e = 0; o->f = 0; o->g = 0; o->h = 0; o->i = 0;
    DAT_1f800238 = DAT_1f800238 + 1;
    DAT_1f800208 = DAT_1f800208 - 1;
    *DAT_1f800208 = (int)o;
}

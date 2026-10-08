// FUNC 800587a8 388 MAIN0
// MATCHING 800587a8 388
typedef struct { char p[0x24]; unsigned short **tbl; char q[0x30 - 0x28]; unsigned short *dot; } O;
extern unsigned char D_8009C980;
void FUN_800594e4(O *o, short x, short y, int k);
void AddDrawMode(int, int);
void func_800587A8(O *o, short x, short y)
{
    unsigned char *g = &D_8009C980;
    unsigned short *t;
    if (*g - 1 >= 10) {
        t = o->tbl[(*g - 1) / 10];
        FUN_800594e4(o, x - 5, y, *t);
        FUN_800594e4(o, x + 5, y, *o->tbl[(*g - 1) % 10]);
    } else {
        t = o->tbl[*g - 1];
        FUN_800594e4(o, x, y, *t);
    }
    FUN_800594e4(o, x, y - 4, *o->dot);
    AddDrawMode(0x15, 1);
}

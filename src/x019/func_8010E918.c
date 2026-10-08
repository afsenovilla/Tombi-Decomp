// FUNC 8010e918 100 X019
// MATCHING 8010e918 100
extern unsigned char D_8009D2B3, D_8009C990, D_8009CF06, D_8009D006;
typedef struct { char p[0xc1]; unsigned char c1; } O;
int func_8010E918(O *o)
{
    int b = o->c1;
    int u = D_8009D2B3 + b * 4;
    if ((D_8009C990 & 3) != 0)
        u = (unsigned char)b << 2 | 3;
    if (D_8009CF06 != 0)
        u = 8;
    if (D_8009D006 != 0)
        u = 8;
    return u;
}

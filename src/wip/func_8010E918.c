// FUNC 8010e918 100 X000
// best: score 12 (game recomputes sll in the |3 branch and loads D_8009C990 before the sll)
extern unsigned char D_8009D2B3, D_8009C990, D_8009CF06, D_8009D006;
typedef struct { char p[0xc1]; unsigned char c1; } O;
int func_8010E918(O *o)
{
    int c = o->c1;
    int r = D_8009D2B3 + c * 4;
    if (D_8009C990 & 3) r = c * 4 + 3;
    if (D_8009CF06) r = 8;
    if (D_8009D006) r = 8;
    return r;
}

/* score 34: game has a real loop (constant 1 hoisted to s4, 'addiu v0,s1,1' stolen into both beqz n==0 delay slots
   = jumps to the loop continue point) but keeps the 'lhu e->w0' flag read at the loop top; real for/while loops
   CSE that read into the bottom exit test (and grow the frame to 0x38). This goto version gets everything else. */
// FUNC 8012d3b8 524 X010
#include "TOBJ.H"
typedef struct {
    short w0;
    unsigned short w2, w4, w6, w8, wa, wc, we, w10, w12, w14, w16, w18, w1a;
} E;
extern unsigned char D_8009CDAB, D_8009CFCC;
extern E *D_8012F52C;
extern TObj *FUN_800184d8(void);
extern TObj *FUN_800183b8(void);

void func_8012D3B8(TObj *o)
{
    E *t, *e;
    TObj *n;
    short i;
    unsigned char one;

    if (D_8009CDAB == 0 || D_8009CFCC != 0) {
        o->b04 = 3;
        return;
    }
    t = D_8012F52C;
    e = t;
    if (t->w0 == 0xff)
        return;
    i = 0;
    one = 1;
loop:
    {
        if (e->w0 & 0x80) {
            n = FUN_800184d8();
            if (n != 0) {
                n->type = 0x1a;
                n->active = one;
                n->b0a = 0x10;
                n->animFrame = e->w2;
                n->subtype = e->w18;
                n->b0c = e->w1a;
                n->a.p.whole = e->w12;
                n->y.p.whole = e->w14;
                n->b.p.whole = e->w16;
            }
        } else {
            n = FUN_800183b8();
            if (n != 0) {
                n->active = one;
                n->type = 0x32;
                n->b0a = 0;
                n->animFrame = e->w2;
                n->subtype = e->w18;
                n->b0c = e->w1a;
                n->a.p.whole = e->w12;
                n->y.p.whole = e->w14;
                n->b.p.whole = e->w16;
                n->wb4 = e->w4;
                n->wb8 = e->w10;
                n->wba = e->we;
                n->w74 = e->w6;
                n->w76 = e->w8;
                n->w78 = e->wa;
                n->b68 = 0;
                n->d90 = (int)o;
                n->w7a = e->wc;
            }
        }
        i++;
        e = &t[i];
        if (e->w0 != 0xff)
            goto loop;
    }
}

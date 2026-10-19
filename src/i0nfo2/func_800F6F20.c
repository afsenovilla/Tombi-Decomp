// FUNC 800f6f20 1760 I0NFO2
// MATCHING 800f6f20 1760
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
typedef struct {
    short type, w2, frame, w6, w8, wA, wC, wE, w10, w12, x, y, sub, bc;
} SpawnA;
typedef struct {
    short type, w2, w4, w6, x, y, sub, bc;
} SpawnB;
extern TObj *func_800183B8();
extern TObj *func_80018448();
extern unsigned char D_8009CDD2;
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CDD6;
extern unsigned char D_8009CDD9;
extern unsigned char D_8009CE4B;
extern unsigned char D_8009CEF0;
extern unsigned char D_8009CEF4;
extern unsigned short D_8009C982;
extern TObj *D_8009F2D8[];
extern TObj *D_8009F328;
extern TObj D_800A6038;
extern SpawnA *D_800FFD7C[];
extern SpawnB D_800FF06C[];
#define G8(n) (*((unsigned char *)&GAME + (n)))

#define PI_PARENT asm("s3")
#define PI_TBL asm("s6")
#define PI_P 
#define PI_FF asm("s2")
#define PI_D asm("s5")
#define PI_TWO 
#define PI_LP asm("s4")
#define PI_I1 asm("s1")
void func_800F6F20(TObj *parent0)
{
    register TObj *parent PI_PARENT = parent0;
    register SpawnA *tbl PI_TBL;
    register SpawnA *p PI_P;
    register int ff PI_FF;
    register int d PI_D;
    register int two PI_TWO;
    register TObj **lp PI_LP;
    SpawnB *e;
    TObj *n;
    TObj *o;
    short i;
    register int i1 PI_I1;
    register int j asm("a0");
    short t;
    short s;

    { register unsigned char *g asm("a0"); g = &G8(0x58D); tbl = D_800FFD7C[*g]; }
    i1 = 0;
    p = tbl;
    if (p->type != 0xFF) {
    ff = 0xFF;
    d = 0xD;
    lp = D_8009F2D8;
    do {
        n = func_800183B8();
        if (n != 0) {
            n->active = 1;
            t = p->w2;
            if (t == ff) {
                n->type = 0x30;
                n->b0a = d;
                n->animFrame = p->frame;
                n->subtype = p->sub;
                n->b0c = p->bc;
                n->_pad0e[0] = p->type;
                n->a.p.whole = p->x;
                n->y.p.whole = p->y;
                n->b.p.whole = 0;
                n->wb4 = p->w6;
                n->wb6 = p->w2;
            } else if (t & 0x80) {
                n->type = 0x2E;
                n->b0a = d;
                n->animFrame = p->frame;
                n->subtype = p->sub;
                n->b0c = p->bc;
                n->_pad0e[0] = p->type;
                n->a.p.whole = p->x;
                n->y.p.whole = p->y;
                n->b.p.whole = 0;
                n->wb4 = p->w6;
                n->wb6 = p->w2 & 0x7F;
            } else if (t & 0x40) {
                n->type = 0x2D;
                n->b0a = d;
                n->animFrame = p->frame;
                n->subtype = p->sub;
                n->b0c = p->bc;
                n->_pad0e[0] = p->type & 0x3F;
                n->a.p.whole = p->x;
                n->y.p.whole = p->y;
                n->b.p.whole = 0;
                n->wb4 = p->w6;
                n->wb6 = p->w2 & 0x3F;
            } else {
                goto plain;
            }
            n->wb8 = p->w12;
            n->wba = p->w10;
            n->w74 = p->w8;
            n->w76 = p->wA;
            n->w78 = p->wC;
            n->w7a = p->wE;
            lp[(short)i1] = n;
            n->d90 = (int)parent;
            goto next;
        plain:
            n->type = 0x30;
            n->b0a = d;
            n->animFrame = p->frame;
            n->subtype = s = p->sub;
            if (G8(0x963) & 0x10) {
                n->subtype = s + 3;
            }
            n->b0c = p->bc;
            n->_pad0e[0] = p->type;
            n->a.p.whole = p->x;
            n->y.p.whole = p->y;
            n->b.p.whole = 0;
            n->wb4 = p->w6;
            n->wb6 = p->w2;
            n->wb8 = p->w12;
            n->wba = p->w10;
            n->w74 = p->w8;
            n->w76 = p->wA;
            n->w78 = p->wC;
            n->w7a = p->wE;
            lp[(short)i1] = n;
            n->d90 = (int)parent;
            if ((unsigned short)n->wba == 0x15) {
                if (G8(0x4E1) != 1) {
                    n->b04 = 2;
                }
                if (G8(0x67E) != 0) {
                    { register int c asm("v0"); c = 2; n->b04 = c; }
                }
            }
            if ((unsigned short)n->wba == 0x16 && D_8009CDD6 != ff) {
                n->b04 = 2;
            }
            if ((D_8009CDD2 == 0 || D_8009CDD2 == ff) && (D_8009CDD5 == 0 || D_8009CDD5 == ff) && D_8009CEF0 == 2) {
                switch ((short)i1) {
                case 3:
                    n->h->p.whole = 0x116;
                    break;
                case 4:
                    n->h->p.whole = 0x142;
                    break;
                }
            }
            if ((short)i1 == 0xB) {
                if (D_8009CDD9 != 0 && D_8009CDD9 != ff && D_8009CDD5 == ff) {
                    n->h->p.whole = 0x1B;
                    n->y.p.whole = 0x1B6;
                    n->animFrame = 3;
                } else {
                    if (D_8009CEF4 == 1 && D_8009C982 == 2) {
                        D_800A6038.h->p.whole = n->h->p.whole + 0x10;
                        D_800A6038.y.p.whole = n->y.p.whole;
                    }
                    if (D_8009CDD5 >= 3) {
                        n->b04 = 2;
                    }
                }
            }
            if ((short)i1 == 0xC && D_8009CE4B == ff) {
                n->b04 = 2;
            }
        }
    next:
        j = i1 + 1;
        i1 = j;
        { register int m asm("v0"); m = (short)j * 28; p = (SpawnA *)((char *)tbl + m); }
    } while (p->type != ff);
    }
    for (i = 0; (e = &D_800FF06C[i])->type != 0xFF; i++) {
        o = func_80018448();
        if (o != 0) {
            o->active = 1;
            o->type = 0x1D;
            o->b0a = 0xD;
            o->animFrame = 0;
            o->subtype = e->sub;
            o->b0c = e->bc;
            o->_pad0e[0] = e->type;
            o->a.p.whole = e->x;
            o->y.p.whole = e->y;
            o->b.p.whole = 0;
            o->wb4 = e->w6;
            o->wb6 = e->w2;
        }
    }
    if (D_8009CDD5 == 0xFF) {
        n = func_800183B8();
        if (n != 0) {
            n->active = 1;
            n->type = 0x30;
            n->b0a = 0xD;
            n->animFrame = 3;
            n->subtype = 5;
            n->b0c = 0;
            n->_pad0e[0] = 0;
            n->a.p.whole = 0x12C;
            n->y.p.whole = 0x7C;
            n->b.p.whole = 0;
            n->wb4 = 3;
            n->wb6 = 2;
            n->wb8 = 0;
            n->wba = 0x14;
            n->w74 = 0;
            n->w76 = 0;
            n->w78 = 0;
            n->w7a = 0;
            D_8009F328 = n;
            n->d90 = (int)parent;
        }
    }
}

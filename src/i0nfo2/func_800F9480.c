// FUNC 800f9480 540 I0NFO2
// MATCHING 800f9480 540
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void func_800EEC3C(TObj *o, int i, short n);
extern void func_8005A9A4(int e, int a);
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern TObj *D_8009F308;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern short D_800A604E;
extern void *D_800A6078;

void func_800F9480(TObj *o)
{
    TObj *n = D_8009F308;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        o->state++;
    case 1:
        if (D_800A604E >= 0xC6) {
            n->animFrame = 3;
        } else if (S16(D_800A6078, 2) >= n->h->p.whole) {
            n->animFrame = 0;
        } else {
            n->animFrame = 2;
        }
        func_800EEC3C(n, 8, 0);
        n->step = 2;
        n->state = 0;
        D_800A603D = 4;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 2:
        p = *(Pos6 *)&n->a;
        S32(n, 0x90) = func_8002DCC8(2, 0x1F, &p);
        o->state++;
    case 3:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            func_8005A9A4(0x2B, 0);
            n->animFrame = 3;
            func_800EEC3C(n, 0, 0);
            o->w08 = 0xC8;
            o->state = 8;
        }
        break;
    case 8:
        if (--o->w08 <= 0) {
            o->state = 9;
        }
        break;
    case 9:
        n->b68 = 0;
        n->b6a = 0;
        n->step = 0;
        n->state = 0;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}

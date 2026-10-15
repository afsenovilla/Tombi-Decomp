// FUNC 800f8f30 680 I0NFO2
// MATCHING 800f8f30 680
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void func_800EEBE8(TObj *o, unsigned short i, short n);
extern void func_800EEC3C(TObj *o, int i, short n);
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CEF0[];
extern TObj *D_8009F2E4;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern short D_800A604E;

static inline void startWalk(TObj *n)
{
    n->step = 2;
    n->state = 0;
    n->animFrame = n->b69 - 1;
    func_800EEC3C(n, 8, 0);
}

static inline void stopWalk(TObj *n)
{
    n->b68 = 0;
    n->b6a = 0;
    n->step = 0;
    n->state = 0;
    n->animFrame = 1;
    func_800EEC3C(n, 0, 0);
}

void func_800F8F30(TObj *o)
{
    TObj *n = D_8009F2E4;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        startWalk(n);
        D_800A603D = 4;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        p = *(Pos6 *)&n->a;
        if (D_8009CDD5 == 0xFF) {
            S32(n, 0x90) = func_8002DCC8(2, 0x48, &p);
        } else {
            S32(n, 0x90) = func_8002DCC8(2, 0x36, &p);
        }
        o->state++;
        break;
    case 2:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            if (D_800A604E > n->y.p.whole) {
                func_800EEBE8(n, 6, 0);
                o->state++;
            } else if (D_8009CEF0[0] == 1) {
                func_800EEBE8(n, 6, 0);
                if (D_8009CDD5 == 0 || D_8009CDD5 == 0xFF) {
                    o->state++;
                } else {
                    func_800EEBE8(n, 1, 0);
                    o->state = 9;
                }
            } else {
                func_800EEBE8(n, 1, 0);
                o->state = 9;
            }
        }
        break;
    case 3:
        n->h->raw += 0xFFFF0000;
        if (n->h->p.whole < 0x10E) {
            D_8009CEF0[0] = 2;
            n->h->p.whole = 0x10E;
            func_800EEBE8(n, 1, 0);
            n->animFrame = 1;
            o->state = 9;
        }
        break;
    case 9:
        stopWalk(n);
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

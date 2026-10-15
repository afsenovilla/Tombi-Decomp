// FUNC 800ed100 464 I0NFO
// MATCHING 800ed100 464
#include "TOBJ.H"
extern int func_8001FE0C(int a, int r);
extern int func_8001FE3C(int a, int r);
extern void func_8001FEC0(TObj *o);
extern void func_800EC88C(TObj *o, int x, int y, int z);
extern unsigned char D_8009C942;

void func_800ED100(TObj *o)
{
    short a;
    switch (o->state) {
    case 0:
        o->velX = 0x100;
        o->state++;
    case 1:
        if (D_8009C942 == 0) {
            func_8001FEC0(o);
            if (o->b69 != o->animFrame + 1) {
                a = o->animFrame << 6;
                o->h->raw += (short)func_8001FE3C(a, o->velX) << 8;
                o->y.raw += (short)func_8001FE0C(a, o->velX) << 8;
                func_800EC88C(o, o->h->p.whole, o->y.p.whole, o->animFrame * 2);
            } else {
                o->b69 = 0;
            }
            switch (o->animFrame & 3) {
            case 0:
                if (o->d84 < o->h->p.whole) {
                    o->h->p.whole = o->d84;
                    o->b6a = 0; o->step = 0; o->state = 0;
                }
                break;
            case 1:
                if (o->y.p.whole < o->d88) {
                    o->y.p.whole = o->d88;
                    o->b6a = 0; o->step = 0; o->state = 0;
                }
                break;
            case 2:
                if (o->h->p.whole < o->d84) {
                    o->h->p.whole = o->d84;
                    o->b6a = 0; o->step = 0; o->state = 0;
                }
                break;
            case 3:
                if (o->d88 < o->y.p.whole) {
                    o->y.p.whole = o->d88;
                done:
                    o->b6a = 0;
                    o->step = 0;
                    o->state = 0;
                }
                break;
            }
        }
        break;
    }
}

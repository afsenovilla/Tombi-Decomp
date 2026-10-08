// FUNC 8002eb34 680 MAIN0
// MATCHING 8002eb34 680
#include "TOBJ.H"

extern TObj *ObjAlloc();
extern int Rand(void);
extern int ObjCullRegister(TObj *o);
extern int AnimAdvance(TObj *o);
extern void ObjFree(TObj *o);
extern void FUN_8002eddc(TObj *o);
extern short D_8007A5F0[];
extern short D_8007A3F0[];

void func_8002EB34(TObj *o)
{
    TObj *n;
    short r;
    short s;
    short b;
    short i;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if ((*(unsigned short *)0x1F8001F8 + *(int *)0x1F800198) & 1) {
                if (--o->timer == -1) {
                    o->b04 = 3;
                    break;
                }
                n = ObjAlloc();
                if (n == 0) break;
                r = Rand();
                b = r & 1;
                s = b;
                switch (o->b0c) {
                case 0:
                    s = (b << 11) + 0x800;
                    n->wac = 0;
                    n->b0a = 0;
                    break;
                case 1:
                    s = (b * 3 << 10) + 0xc00;
                    n->wac = Rand() & 1;
                    if (n->wac) goto one;
                    n->b0a = 0;
                    break;
                case 2:
                    s = (b << 12) + 0x1000;
                    n->wac = 1;
                one:
                    n->b0a = 1;
                    n->d64 = 0x1800;
                    break;
                }
                n->h->raw = o->h->raw;
                n->y.raw = o->y.raw;
                n->d->raw = o->d->raw;
                i = r & 0xf0;
                n->h->raw += (D_8007A5F0[i] * s) >> 4;
                n->y.raw += (D_8007A3F0[i] * s) >> 4;
                n->b04 = 1;
                FUN_8002eddc(n);
            }
            break;
        case 1:
            o->b04 = 1;
            FUN_8002eddc(o);
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->wac == 1) o->d64 += 0x200;
        if (AnimAdvance(o)) o->b04 = 3;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

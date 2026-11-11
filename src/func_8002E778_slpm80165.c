// VERSION SLPM_80165 slpm80165 8002E778 616
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of updatePoolType32Object (src/updatePoolType32Object.c): the pieces only get the +0xAC kind (no
// +0x0A / +0x64 spin).
#include "TOBJ.H"

extern TObj *ObjAlloc();
extern int Rand(void);
extern int ObjCullRegister(TObj *o);
extern int AnimAdvance(TObj *o);
extern void ObjFree(TObj *o);
extern void initPoolType32Object(TObj *o);
extern short COS_TABLE[];
extern short NEG_SIN_TABLE[];

void func_8002E778(TObj *o)
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
                    break;
                case 1:
                    s = (b * 3 << 10) + 0xc00;
                    n->wac = Rand() & 1;
                    break;
                case 2:
                    s = (b << 12) + 0x1000;
                    n->wac = 1;
                    break;
                }
                n->h->raw = o->h->raw;
                n->y.raw = o->y.raw;
                n->d->raw = o->d->raw;
                i = r & 0xf0;
                n->h->raw += (COS_TABLE[i] * s) >> 4;
                n->y.raw += (NEG_SIN_TABLE[i] * s) >> 4;
                n->b04 = 1;
                initPoolType32Object(n);
            }
            break;
        case 1:
            o->b04 = 1;
            initPoolType32Object(o);
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (AnimAdvance(o)) o->b04 = 3;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif

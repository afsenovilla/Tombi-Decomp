// FUNC 801193e4 764 X001
// MATCHING 801193e4 764
#include "TOBJ.H"

extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern TObj *ObjAlloc(void);
extern void ObjFree(TObj *);
extern int ObjCullRegister(TObj *);
extern int Rand(void);
extern void func_801196E0(TObj *);

void func_801193E4(TObj *o)
{
    TObj *e;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if (!((D_1F8001F8 + D_1F800198) & 7)) break;
            if (--o->timer == -1) {
                o->b04 = 3;
                break;
            }
            e = ObjAlloc();
            if (e) {
                e->wac = o->timer & 1;
                e->subtype = 1;
                e->timer = 0x3c;
                e->b0c = (*(unsigned char *)&D_1F8001F8 + D_1F800198) & 3;
                e->h->raw = o->h->raw + 0x100000;
                e->y.raw = o->y.raw - 0x180000;
                e->d->raw = o->d->raw;
                func_801196E0(e);
            }
            break;
        case 1:
            o->b04 = 1;
            func_801196E0(o);
            o->animFrame = (D_1F8001F8 + D_1F800198) & 1;
            o->a.p.whole += (Rand() & 3) << 3;
            o->y.p.whole += (Rand() & 3) * 12;
            switch (o->b0c) {
            case 0:
                o->velH = 0x180;
                o->velX = -2;
                break;
            case 1:
                o->velH = 0x100;
                o->velX = -2;
                break;
            case 2:
                o->velH = 0x80;
                o->velX = -1;
                break;
            }
            o->velV = -0x180;
            o->velY = 0x10;
            break;
        }
        break;
    case 1:
        if (!ObjCullRegister(o)) {
            o->b04 = 3;
            break;
        }
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        if (o->animFrame & 1) o->h->raw += o->velH << 8;
        else o->h->raw -= o->velH << 8;
        o->velH += o->velX;
        if (o->animFrame & 1) o->d8c = (o->d8c + 4) & 0xff;
        else o->d8c = (o->d8c - 4) & 0xff;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

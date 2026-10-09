// FUNC 80118da8 1084 X001
// MATCHING 80118da8 1084
#include "TOBJ.H"

extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern int D_1F8002D4[];
extern void *D_8013E5B0[];
extern TObj *ObjAlloc(void);
extern int FUN_8001f9e0(void);
extern int ObjCullRegister(TObj *);
extern void FUN_800187e4(TObj *);

void func_80118DA8(TObj *o)
{
    TObj *e;
    int t;

    t = o->b04;
    switch (t) {
    case 0:
        t = o->subtype;
        switch (t) {
        case 0:
            if ((D_1F8001F8 + D_1F800198) & 1) break;
            if (--o->timer == -1) goto done;
            e = ObjAlloc();
            if (e == 0) break;
            e->active = 1;
            e->type = 0x1f;
            e->subtype = 1;
            e->h->raw = o->h->raw;
            e->h->p.whole += o->timer - 0x10;
            e->y.raw = o->y.raw;
            e->d->raw = o->d->raw;
            e->b0c = FUN_8001f9e0() & 3;
            e->wac = o->timer / 4;
            e->timer = (FUN_8001f9e0() & 0x1f) + 0x30;
            e->animFrame = FUN_8001f9e0() & 1;
            break;
        done:
            o->subtype = 1;
            o->b0c = 0;
            o->wac = 0;
            o->timer = 0x5a;
            o->animFrame = 0;
            break;
        case 1:
            o->w1e = 8;
            o->b0a = 2;
            o->b0d = 0;
            *(signed char *)&o->b0f = -10;
            o->anim = D_8013E5B0[o->wac];
            o->d3c = D_1F8002D4[0];
            o->step = 0;
            o->b04++;
            switch (o->b0c) {
            case 0:
                o->velH = 0x200;
                o->velX = -4;
                break;
            case 1:
                o->velH = 0x180;
                o->velX = -2;
                break;
            case 2:
                o->velH = 0x100;
                o->velX = -2;
                break;
            case 3:
                o->velH = 0x80;
                o->velX = -1;
                break;
            }
            o->velV = 0;
            o->velY = 0x60;
            break;
        }
        break;
    case 1:
        o->y.raw += o->velV << 8;
        switch (o->wac) {
        case 0:
            if (o->velV < 0x200) o->velV += o->velY;
            break;
        case 1:
            if (o->velV < 0x180) o->velV += o->velY;
            break;
        case 2:
            if (o->velV < 0x100) o->velV += o->velY;
            break;
        }
        if (o->animFrame & 1) o->h->raw += o->velH << 8;
        else o->h->raw -= o->velH << 8;
        o->velH += o->velX;
        switch (o->step) {
        case 0:
            if (ObjCullRegister(o)) o->step++;
            break;
        case 1:
            if (o->timer < 0x1e) {
                if ((D_1F8001F8 + D_1F800198) & 1) ObjCullRegister(o);
            } else {
                if (!ObjCullRegister(o)) o->b04 = 3;
            }
            if (--o->timer == -1) o->b04 = 3;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}

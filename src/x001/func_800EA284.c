// FUNC 800ea284 704 X001
// MATCHING 800ea284 704
#include "TOBJ.H"

typedef struct { short x, y; } V2;
extern unsigned short D_8009C960;
extern void *D_8013B0EC[];
extern void *D_80134CF4[];
extern V2 D_80114A68[];
void AnimLoadDuration(TObj *o);
int ObjCullRegister(TObj *o);
void FUN_80020490(TObj *o);
short FUN_800411cc(TObj *o, int x, int y);
void ObjFree(TObj *o);

void func_800EA284(TObj *o)
{
    short *v = &o->wb4;
    char pad[8];
    switch (o->b04) {
    case 0:
        if (D_8009C960 == 0) o->anim = D_8013B0EC[o->subtype];
        else o->anim = D_80134CF4[o->subtype];
        v[1] = -0x400;
        AnimLoadDuration(o);
        o->b04++;
        break;
    case 1:
        if (ObjCullRegister(o) == 0) FUN_80020490(o);
        switch (o->step) {
        case 0:
            o->wb4 = D_80114A68[o->wb8].x;
            o->wb6 = D_80114A68[*(short *)((char *)o + 0xb8)].y;
            o->step++;
        case 1:
            o->h->raw += v[0] << 8;
            o->y.raw += v[1] << 8;
            v[1] += 0x20;
            if (o->animFrame & 1) o->d8c = (o->d8c - 4) & 0xfff;
            else o->d8c = (o->d8c + 8) & 0xfff;
            if (v[1] > 0) o->step++;
            break;
        case 2:
            o->h->raw += o->wb4 << 8;
            o->y.raw += o->wb6 << 8;
            o->wb6 += 0x20;
            if (o->animFrame & 1) o->d8c = (o->d8c - 4) & 0xfff;
            else o->d8c = (o->d8c + 8) & 0xfff;
            if (FUN_800411cc(o, o->h->p.whole, o->y.p.whole)) {
                o->b04 = 2;
                o->step = 0;
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

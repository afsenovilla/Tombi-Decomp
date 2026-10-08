// FUNC 8002e83c 624 MAIN0
/* score 94 (short r,k): game stores anim/d3c via a0 (move a0,sX before a cross-jumped tail sw 0x24(a0); jal AnimLoadDuration; sw 0x3c(a0)) with the D_ loads duplicated in each path; also c-path y mult scheduled early */
#include "TOBJ.H"
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern int D_1F8002D8;
extern void *D_80012204;
extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern TObj *ObjAlloc();
extern int Rand(void);
extern void ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void ObjFree(TObj *);
void func_8002E83C(TObj *o)
{
    TObj *c, *p;
    short r, k;
    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if (((D_1F8001F8 + D_1F800198) & 1) == 0) break;
            if (--o->timer == -1) {
                o->b04 = 3;
                break;
            }
            c = ObjAlloc();
            if (c == 0) break;
            r = Rand();
            k = ((r & 1) << 11) + 0x800;
            c->h->raw = o->h->raw;
            c->y.raw = o->y.raw;
            c->d->raw = o->d->raw;
            r &= 0xf0;
            c->h->raw += (D_8007A5F0[r] * k) >> 4;
            c->type = 0x31;
            c->animFrame = 1;
            c->b04 = 1;
            c->active = 1;
            c->b0a = 0; c->b0b = 0; c->b0f = 0;
            c->w1e = 0x14;
            c->b0d = 1;
            c->y.raw += (D_8007A3F0[r] * k) >> 4;
            c->w08 = ((c->b0c + 0x1fc) << 6) | 0x17;
            p = c;
            goto common;
        case 1:
            o->type = 0x31;
            o->animFrame = 1;
            o->b04 = 1;
            o->active = 1;
            o->b0a = 0;
            o->b0b = 0;
            o->b0f = 0;
            o->w1e = 0x14;
            o->b0d = 1;
            o->w08 = ((o->b0c + 0x1fc) << 6) | 0x17;
            p = o;
        common:
            p->anim = D_80012204;
            p->d3c = D_1F8002D8;
            AnimLoadDuration(p);
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

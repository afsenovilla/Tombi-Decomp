// FUNC 8012f9a0 548 X003
/* score 6: case 0 only - game loads o->subtype after the animFrame/w1e stores and reuses v1 for the constant 1
   (ours hoists the lbu and puts 1 in a0). Tried: statement hill-climb, raw stores, if/switch forms, empty-loop barrier. */
#include "TOBJ.H"

extern int D_1F8002DC[];
extern void *D_8013A698[];
extern signed char D_80135EA0[], D_80135EB0[];
extern short D_8007A5F0[], D_8007A3F0[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_8012F9A0(TObj *o)
{
    TObj *p;
    signed char *t;

    switch (o->b04) {
    case 0:
        o->box1 = 0x10;
        o->box3 = 0x10;
        o->animFrame = 1;
        o->box0 = 8;
        o->w1e = 1;
        o->box2 = 8;
        o->b04++;
        o->d3c = D_1F8002DC[0];
        *(signed char *)&o->b0f = -10;
        o->b0d = 0x80;
        o->b0a = 0;
        o->d30 = 0;
        o->d34 = 0;
        if (o->subtype == 0) {
            o->velH = 0x80;
            o->wac = 0x19;
        } else {
            o->active = 1;
            o->velH = 0x480;
            o->wac = 0x12;
        }
        o->anim = D_8013A698[o->wac];
        AnimLoadDuration(o);
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->animFrame) t = D_80135EB0 + ((o->d8c >> 4) & 0xe);
        else t = D_80135EA0 + ((o->d8c >> 4) & 0xe);
        {
            int i, x, y, tx, ty;
            i = o->d8c & 0xf0;
            x = o->velH * D_8007A5F0[i];
            y = o->velH * D_8007A3F0[i];
            tx = t[0] << 16;
            ty = t[1] << 16;
            o->d30 += x >> 4;
            o->d34 += y >> 4;
            o->a.raw = p->a.raw + o->d30 + tx;
            o->y.raw = p->y.raw + o->d34 + ty;
        }
        if (AnimAdvance(o)) o->b04++;
        ObjCullRegister(o);
        break;
    case 2:
    case 3:
        FUN_80018790(o);
        break;
    }
}

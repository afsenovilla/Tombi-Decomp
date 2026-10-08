// FUNC 8002be80 472 MAIN0
// MATCHING 8002be80 472
#include "TOBJ.H"
extern void *D_80012034[];
extern void *D_80012268;
extern unsigned char D_80079D3C[];
extern unsigned short D_800A6066;
extern Fix16 *D_800A6078;
extern unsigned short D_800A604E[];
extern int DAT_1f8002d8[];
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern void ObjListPush_1F800220(TObj *);
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);

void func_8002BE80(TObj *o)
{
    unsigned char s = o->b04;
    int d;
    switch (s) {
    case 0:
        o->w1e = 0x15;
        if (o->subtype == 0 || o->subtype == 3) {
            o->w1e = 0x14;
        }
        o->b0d = 0;
        *(signed char *)&o->b0f = -30;
        o->b0a = 0;
        o->animFrame = 0;
        o->anim = D_80012034[o->subtype];
        o->d3c = DAT_1f8002d8[0];
        o->b04++;
        AnimLoadDuration(o);
        if (o->subtype == 2) {
            o->b0d = 0x80;
            o->w1e = 0x13;
            o->anim = D_80012268;
            playSFX(D_80079D3C[o->subtype]);
        }
        break;
    case 1:
        if (o->subtype == 2) {
            o->visible = 1;
            ObjListPush_1F800220(o);
            if (D_800A6066 & 1) d = -14; else d = 14;
            o->h->p.whole = D_800A6078->p.whole + d;
            o->y.p.whole = D_800A604E[0] - 14;
            if (AnimAdvance(o)) {
                o->b04++;
            }
        } else if (ObjCullRegister(o)) {
            if (AnimAdvance(o)) {
                o->b04++;
            }
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

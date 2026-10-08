// FUNC 8002e4b4 528 MAIN0
// MATCHING 8002e4b4 528
#include "TOBJ.H"
extern int GetGraphType(void);
extern short GetClut(int x, int y);
extern char D_80012528[];
extern int D_1F8002D8[];
extern short D_1F80016A[];
extern unsigned short D_1F80016E[];
extern unsigned short D_1F800172[];
extern void AnimLoadDuration(TObj *o);
extern void ObjListPush_1F800220(TObj *o);
extern short MulNegSin(short a, short b);
extern void ObjFree(TObj *o);

void func_8002E4B4(TObj *o)
{
    short v;
    switch (o->b04) {
    case 0:
        o->b04++;
        if (GetGraphType() == 1 || GetGraphType() == 2) v = 0xa5;
        else v = 0x35;
        o->w1e = v;
        *(signed char *)&o->b0f = -9;
        o->b0a = 14;
        o->b0d = 0x81;
        o->w08 = GetClut(0x160, o->b0c + 0x1f1);
        o->animFrame = 0;
        o->anim = D_80012528;
        o->d3c = D_1F8002D8[0];
        o->d30 = D_1F80016A[0] << 16;
        o->y.p.whole = D_1F80016E[0];
        o->d->p.whole = D_1F800172[0];
        o->velV = -0xc00;
        o->timer = 1;
        o->wb4 = 0x80;
        o->wb6 = 0;
        o->wb8 = 100;
        AnimLoadDuration(o);
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800220(o);
        if (--o->timer == 0) {
            o->timer = 1;
            if (--*(unsigned short *)&o->wb4 == 0) o->b04++;
        }
        o->wb6 = (o->wb6 + 8) & 0xff;
        o->wb8 += 2;
        o->h->raw = o->d30 + (MulNegSin(o->wb6, 0x10) << 16);
        o->y.raw += o->velV << 4;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

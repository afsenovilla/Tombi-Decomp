// FUNC 80119eec 724 X001
// MATCHING 80119eec 724
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void *D_8013E514;
extern void *D_8013E51C;
extern void *D_8013E524;
extern void *D_8013E528[];
extern void *D_8013E530;
extern void *D_8013E538;
extern unsigned char D_8009CE58;
extern short D_8007A3F0[];
extern char D_80077CDC[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern int Rand(void);
extern void FUN_8001faf4(TObj *);
extern void FUN_800187e4(TObj *);

void func_80119EEC(TObj *o)
{
    int v;
    int d;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 11;
        o->d3c = D_1F8002D4[0];
        switch (o->b0c) {
        case 0:
            o->b0d = 0;
            o->anim = D_8013E514;
            break;
        case 1:
            o->b0d = 0;
            o->anim = D_8013E51C;
            break;
        case 2:
            o->b0d = 0;
            o->anim = D_8013E524;
            break;
        case 3:
            o->b0d = 0;
            o->anim = D_8013E528[o->subtype];
            AnimLoadDuration(o);
            break;
        case 4:
            o->b0d = 0x80;
            o->anim = D_8013E530;
            break;
        case 5:
            if (D_8009CE58 == 0) {
                o->h->p.whole = 0x6b8;
                o->b0d = 0x80;
                o->y.p.whole = -0x28;
                o->anim = D_8013E538;
                AnimLoadDuration(o);
            } else {
                o->b04 = 3;
            }
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->b0c) {
        case 3:
            switch (o->state) {
            case 0:
                o->state++;
                o->animFrame = Rand() & 1;
                o->movetab = D_80077CDC;
                o->velV = 0x80;
                o->d88 = Rand() & 0xf0;
                o->timer = 4;
                break;
            case 1:
                FUN_8001faf4(o);
                v = D_8007A3F0[*(unsigned char *)&o->d88] * o->velV;
                d = (o->d88 + 2) & 0xff;
                o->d88 = d;
                o->y.raw += v >> 4;
                if (d == 0 && --o->timer == 0) {
                    o->timer = 4;
                    o->animFrame = 1 - o->animFrame;
                }
                break;
            }
            AnimAdvance(o);
            break;
        case 0:
        case 1:
        case 2:
        case 4:
            break;
        case 5:
            if (D_8009CE58 == 0xff) {
                o->b04 = 3;
                break;
            }
            AnimAdvance(o);
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

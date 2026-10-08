// FUNC 80117ddc 1320 X006
// MATCHING 80117ddc 1320
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_80122DF4[], *D_80122E04[], *D_80122DC4[];
extern unsigned short D_8009CFE2;
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

#define S(x) ((short)(x))

void func_80117DDC(TObj *o)
{
    short t;
    short d;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->b0a = 7;
        o->b0d = 0;
        o->w1e = 7;
        o->b0f = 0;
        o->d3c = D_1F8002D4[0];
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
        case 3:
            o->timer = 0x32;
            break;
        case 4:
            o->timer = 0x78;
            break;
        case 5:
            o->anim = D_80122DF4[o->b0c];
            AnimLoadDuration(o);
            o->timer = 0x78;
            break;
        case 6:
            o->anim = D_80122E04[o->b0c];
            AnimLoadDuration(o);
            o->timer = 0x78;
            break;
        }
        break;
    case 1:
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
        case 3:
            if (--o->timer == 0) o->b04++;
            o->anim = D_80122DC4[o->b0c];
            AnimLoadDuration(o);
            break;
        case 4:
            t = D_8009CFE2;
            switch (o->b0c) {
            case 0:
                d = S(t / 3600) % 10;
                break;
            case 1:
                d = S(S(S(t / 60) % 60) / 10) % 10;
                break;
            case 2:
                d = S(S(t / 60) % 60) % 10;
                break;
            case 3:
                d = S(S(S(t % 60) * 100 / 60) / 10) % 10;
                break;
            case 4:
                d = S(S(t % 60) * 100 / 60) % 10;
                break;
            case 10:
            case 11:
                d = o->b0c;
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                break;
            }
            o->anim = D_80122DC4[d];
            if (--o->timer == 0) o->b04++;
            AnimLoadDuration(o);
            break;
        case 5:
        case 6:
            if (--o->timer == 0) o->b04++;
            break;
        }
        AnimAdvance(o);
        o->visible = 1;
        ObjListPush_1F800220(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}

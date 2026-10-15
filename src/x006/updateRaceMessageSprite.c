// FUNC 80117ddc 1320 X006
// MATCHING 80117ddc 1320
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

extern void *D_80122DF4[], *D_80122E04[];
extern char D_80122ED8[];
extern char D_80122EE0[];
extern char D_80122EE8[];
extern char D_80122EF0[];
extern char D_80122EF8[];
extern char D_80122F00[];
extern char D_80122F08[];
extern char D_80122F10[];
extern char D_80122F18[];
extern char D_80122F20[];
extern char D_80122F28[];
extern char D_80122F30[];
static void *D_80122DC4[12] = {
    D_80122ED8, D_80122EE0, D_80122EE8, D_80122EF0,
    D_80122EF8, D_80122F00, D_80122F08, D_80122F10,
    D_80122F18, D_80122F20, D_80122F28, D_80122F30,
};
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

#define S(x) ((short)(x))

void updateRaceMessageSprite(TObj *o)
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
        o->d3c = ((int*)&SPAD.i2d4)[0];
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
            t = (*(unsigned short*)&GCD.scfe2);
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

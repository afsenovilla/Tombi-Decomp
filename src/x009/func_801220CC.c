// FUNC 801220cc 992 X009
// MATCHING 801220cc 992
#include "TOBJ.H"
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001fb20(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern void playSFX(int);
extern int Rand(void);
extern void *D_8012E9F8[];
extern void *D_8012EA00[];
extern void *D_8012EA40[];
extern unsigned char D_8012B2CC[];
extern unsigned char D_8009D2B1;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern char D_80077CF4[];

static __inline__ short ground(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 16)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_801220CC(TObj *o)
{
    short *p = &o->wb4;
    int t;

    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        if (*p != 3) {
            o->b6a = 1;
            o->state++;
            *p = 3;
            o->wac = 0x12;
            o->anim = D_8012EA40[0];
            AnimLoadDuration(o);
        } else {
            o->state = 2;
        }
        break;
    case 2:
        ObjSetFacingToPlayer(o);
        o->wac = 2;
        o->state++;
        o->anim = D_8012EA00[0];
        AnimLoadDuration(o);
        break;
    case 1:
    case 3:
        if (AnimAdvance(o)) {
            o->state++;
        }
        break;
    case 4:
        if (o->subtype & 0x80) {
            *p = 0;
            o->wac = 15;
            o->b6a = 0;
        } else {
            switch (D_8009D2B1) {
            case 0:
                *p = 0;
                o->wac = 15;
                o->b6a = 0;
                break;
            case 1:
                *p = 1;
                if (o->visible) {
                    playSFX(0x95);
                }
                o->wac = 14;
                o->b6a = 0;
                break;
            case 2:
                *p = 2;
                if (o->visible) {
                    playSFX(0x96);
                }
                o->wac = 13;
                o->b6a = 0;
                break;
            }
        }
        ObjSetFacingToPlayer(o);
        o->timer = 0x78;
        o->state++;
        o->anim = D_8012E9F8[o->wac];
        AnimLoadDuration(o);
        break;
    case 5:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->state++;
        }
        if (*p == 1) {
            if (o->visible) {
                t = D_1F8001F8 + D_1F800198;
                if (!(t & 0x3f)) {
                    if (Rand() & 1) {
                        playSFX(0x95);
                    }
                } else if (!(t & 0x1f)) {
                    playSFX(0x95);
                }
            }
        } else if (*p == 2) {
            if (o->visible) {
                t = D_1F8001F8 + D_1F800198;
                if (!(t & 0x7f)) {
                    if (Rand() & 1) {
                        playSFX(0x96);
                    }
                } else if (!(t & 0x3f)) {
                    playSFX(0x96);
                }
            }
        }
        break;
    case 6:
        ObjSetFacingToPlayer(o);
        if (o->subtype & 0x80) {
            if (D_8012B2CC[Rand() & 7]) {
                o->timer = 0x78;
            } else {
                o->timer = 0x3c;
            }
        } else {
            o->timer = 0xb4;
        }
        {
            unsigned char c = *(unsigned char *)p;
            o->state = 0;
            o->step = c + 4;
        }
        break;
    }
    if (o->visible && !ground(o)) {
        FUN_8001fb20(o);
    }
}

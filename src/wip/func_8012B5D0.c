// FUNC 8012b5d0 1220 X000
/* score 148: InRange inline returning short fixes most; left: case 2 game computes dx (h - D_1F80016A) fully before dy with an unfilled load slot (ours interleaves), and InRange result lands in a0 + copy instead of v0. */
#include "TOBJ.H"
extern char D_80077CF4[], D_80077CDC[];
extern unsigned char D_801390EC[], D_801390F4[];
extern unsigned short *D_8013A160[];
extern unsigned short *D_8013A21C, *D_8013A220, *D_8013A268;
extern unsigned char D_80138FD8[];
extern unsigned short D_1F80016A, D_1F80016E, D_1F800172;
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001fb20(TObj *);
extern void FUN_8001fab4(TObj *);
extern int FUN_801274cc(TObj *);
extern int FUN_801275e4(TObj *);
extern int FUN_80127720(TObj *);
extern void ObjSetFacingToPlayer(TObj *);

static __inline__ void SetBox(TObj *o)
{
    unsigned char *p;
    p = &D_80138FD8[((unsigned short *)o->anim)[1] * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    o->box3 = *p;
    o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
}

static __inline__ short InRange(TObj *o)
{
    return (unsigned short)(o->d->p.whole - D_1F800172 + 0x2d) < 0x5b
        && (unsigned short)(o->y.p.whole - D_1F80016E + 0x46) < 0x6f
        && (unsigned short)(o->h->p.whole - D_1F80016A + 0x80) < 0x101;
}

void func_8012B5D0(TObj *o)
{
    short *q = &o->wb4;
    int r;
    unsigned short dx;
    unsigned short dy;
    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b69 = 0;
        o->state++;
        o->wac = D_801390EC[o->subtype];
        o->anim = D_8013A160[o->wac];
        SetBox(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        if (FUN_801274cc(o)) {
            int t = (unsigned short)q[1];
            o->timer = 0;
            o->d8c = t;
            o->state++;
        }
        break;
    case 2:
        AnimAdvanceWithBox(o);
        if (o->b0c == 1)
            break;
        dx = o->h->p.whole - D_1F80016A;
        if ((unsigned short)(o->d->p.whole - D_1F800172 + 0x2d) >= 0x5b
            || (unsigned short)(dx + 0xa0) >= 0x141) {
            o->timer = 0;
        } else if (o->timer++ > 0x3c) {
            o->state++;
        }
        break;
    case 3:
        o->timer = 0x78;
        o->state++;
        o->wac = D_801390F4[o->subtype];
        o->anim = D_8013A160[o->wac];
        SetBox(o);
        break;
    case 4:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (o->subtype) {
                r = FUN_80127720(o);
                if ((unsigned char)r == 0xff) {
                    o->state = 2;
                    o->wac = D_801390EC[o->subtype];
                    o->anim = D_8013A160[o->wac];
                    SetBox(o);
                } else {
                    o->state = r;
                    o->substep = 0;
                    o->step++;
                }
            } else {
                o->timer = 0x78;
                o->state++;
                ObjSetFacingToPlayer(o);
                *(char **)((char *)o + 0x28) = D_80077CDC;
                *(short *)((char *)o + 0xac) = 0x2f;
                o->anim = D_8013A21C;
                SetBox(o);
            }
        }
        break;
    case 5:
        AnimAdvanceWithBox(o);
        FUN_8001fab4(o);
        if (FUN_801275e4(o))
            o->timer = 0;
        FUN_801274cc(o);
        if (--o->timer == -1) {
            *(short *)((char *)o + 0x20) = 0x3c;
            *(short *)((char *)o + 0xac) = 0x30;
            o->state++;
            o->anim = D_8013A220;
            SetBox(o);
        }
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            if (InRange(o)) {
                o->state = 2;
                o->wac = D_801390EC[o->subtype];
                o->anim = D_8013A160[o->wac];
                SetBox(o);
            } else {
                *(short *)((char *)o + 0xac) = 0x42;
                o->state++;
                o->anim = D_8013A268;
                SetBox(o);
            }
        }
        break;
    case 7:
        if (AnimAdvanceWithBox(o)) {
            o->step++;
            ObjSetFacingToPlayer(o);
            o->state = 0;
            o->substep = 0;
            r = FUN_80127720(o);
            if ((unsigned char)r != 0xff)
                o->state = r;
        }
        break;
    }
}

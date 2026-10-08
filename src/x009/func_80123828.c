// FUNC 80123828 1216 X009
// MATCHING 80123828 1216
#include "TOBJ.H"
extern char D_80077D24[];
extern void *D_8012EA44[0];
extern void *D_8012EA24[0];
extern unsigned char D_800A603E, D_800A60E4;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001faf4(TObj *);
extern short FUN_80040278(TObj *, short, short);
extern short FUN_8004065c(TObj *, short, short, short);
extern int FUN_8001f9e0(void);
extern void FUN_8001e4f0(int);

static __inline__ short land(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_80123828(TObj *o)
{
    short *e = &o->wb4;
    unsigned char m;
    short d;
    short r;

    switch (o->state) {
    case 0:
        o->b9c = 0;
        o->velV = 0;
        if (*e == 2) {
            o->state = 4;
            o->timer = 5;
            o->velV = -0x200;
            o->b9c = 1;
            o->movetab = D_80077D24;
            o->b69 = 0;
            o->wac = 0x13;
            o->anim = D_8012EA44[0];
            FUN_8001fe6c(o);
            if (o->visible != 0) FUN_8001e4f0(0x96);
            break;
        }
        o->timer = 5;
        o->box2 = 4;
        o->box3 = 0x14;
    next:
        o->state++;
        o->wac = 0xb;
        o->anim = D_8012EA24[0];
        FUN_8001fe6c(o);
    fall:
        r = land(o);
        goto test;
    case 1:
        if (--o->timer != 0) goto fall;
        D_800A603E = 2;
        D_800A60E4 = 3;
        goto next;
    case 2:
        r = land(o);
    test:
        if (!r) {
            o->velV += 0x40;
            if (o->velV > 0x600) o->velV = 0x600;
            o->y.raw += o->velV << 8;
        } else {
            o->b9c = 0;
            o->velV = 0x80;
        }
        break;
    case 3:
        break;
    case 4:
        FUN_8001fec0(o);
        if (--o->timer == 0) {
            D_800A603E = 2;
            D_800A60E4 = 3;
            o->state++;
        }
    case 5:
        o->velV += 0x20;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (land(o)) {
                o->b9c = 0;
                o->velV = 0;
                o->state++;
            }
        }
        break;
    case 6:
        FUN_8001fec0(o);
        m = o->b9d;
        if ((m & 2) && o->animFrame == (m & 1)) {
            d = 1;
        } else {
            short dx;
            if (o->animFrame & 1) dx = -0x10;
            else dx = 0x10;
            d = FUN_8004065c(o, o->h->p.whole + dx, o->y.p.whole, o->animFrame) != 0;
        }
        if (d) o->animFrame = (o->animFrame + 1) & 1;
        if (land(o)) {
            o->b9c = 0;
            o->velV = 0;
            FUN_8001faf4(o);
            o->y.p.whole += 2;
        } else {
            o->velV += 0x40;
            if (o->velV > 0x600) o->velV = 0x600;
            o->y.raw += o->velV << 8;
        }
        if (o->visible != 0) {
            int t = D_1F8001F8 + D_1F800198;
            if ((t & 0x7f) == 0) {
                if (FUN_8001f9e0() & 1) FUN_8001e4f0(0x96);
            } else if ((t & 0x3f) == 0) {
                FUN_8001e4f0(0x96);
            }
        }
        break;
    case 7:
        break;
    }
}

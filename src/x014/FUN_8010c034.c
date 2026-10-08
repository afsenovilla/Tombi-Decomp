// FUNC 8010c034 1916 X014
// MATCHING 8010c034 1916
#include "TOBJ.H"
typedef struct {
    char pad0[2];
    short w02;
    unsigned char b4;
    char pad5;
    unsigned char b6;
    unsigned char b7;
    unsigned char b8;
    unsigned char b9;
    char pada[2];
    short w0c;
    short w0e;
    short w10;
    short w12;
    char pad14[0x14];
    unsigned short w28;
    unsigned short w2a;
} G330;
extern G330 *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned char DAT_8009d2b0;
extern int DAT_8009c984;
extern unsigned short DAT_8009d670;
extern void FUN_80010d68();
extern void FUN_80010dac();
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001e560(int, int);
extern void FUN_8010bd08(TObj *);
extern void FUN_8010e328(TObj *, int);

#define SETPOS(o) \
    o->h->p.whole = DAT_8009f0ec->h->p.whole + o->wb8; \
    if (DAT_8009f0ec->type == 0x24) o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba + 0x10; \
    else o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba;

static __inline__ short chk(TObj *o)
{
    short r = 0;

    if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
        DAT_8009d2b0 = 0;
        FUN_8010bd08(o);
        if (DAT_8009c984 & 0x40) {
            if (*(volatile unsigned short *)&DAT_8009d670 & *(unsigned short *)0x1F8003C4) {
                o->ba7 = 1;
            }
        }
        r++;
        o->b9c = 1;
        o->step = 2;
        o->state = 0;
    }
    return r;
}

static __inline__ void turn(TObj *o)
{
    G330 *p;
    short v;
    o->anim = (void *)FUN_80010dac;
    FUN_8001fe94(o, 0);
    DAT_8009c330->b4 = 1;
    DAT_8009f0ec->b69 = 4;
    FUN_8001e560(0x1e, 8);
    p = DAT_8009c330;
    v = (DAT_8009f0ec->box1 << 8) / 0x18;
    if (o->animFrame & 1)
        v = -v;
    p->w10 = v;
    o->d88 = 0;
    o->state++;
}

void FUN_8010c034(TObj *o)
{
    short tbl[12] = { 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1 };
    short y;
    unsigned short u;

    DAT_8009c330->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        o->wb6 = -0x420;
        DAT_8009c330->w0e = 0;
        DAT_8009c330->w02 = 0x10;
        o->wb2 = DAT_8009f0ec->box1;
        o->animFrame &= 1;
        DAT_8009c330->b6 = DAT_8009f0ec->active;
        DAT_8009c330->b9 = 0;
        DAT_8009c330->w0c = 0;
        DAT_8009c330->w28 = 0xffff;
        DAT_8009c330->w2a = 0xffff;
        o->h->p.whole = o->wb8 + (DAT_8009f0ec->h->p.whole + DAT_8009f0ec->d30);
        y = DAT_8009f0ec->y.p.whole;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->ba4 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->y.p.whole = y + o->wba;
        o->d8c = 0;
        o->velX = o->wb8;
        DAT_8009c330->w10 = DAT_8009f0ec->h->p.whole + o->wb8;
        DAT_8009c330->w12 = DAT_8009f0ec->y.p.whole + o->wba;
        o->anim = (void *)FUN_80010d68;
        FUN_8001fe94(o, 0);
        o->timer = 10;
        o->state++;
    case 1:
        SETPOS(o);
        if (--o->timer == 0) o->state++;
        goto tail;
    case 2:
        o->h->p.whole = DAT_8009f0ec->h->p.whole + o->wb8;
        switch (DAT_8009f0ec->type) {
        case 0x24:
            o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba + 0x10;
            break;
        case 0x1d:
            o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba;
            if (DAT_8009f0ec->step == 3) {
                DAT_8009f0ec->b6a = 0;
                DAT_8009c330->b4 = 0;
                o->timer = 6;
                o->b9c = 2;
                o->velH = 0;
                o->velV = 0;
                o->velX = 0;
                o->velY = 0;
                {
                    Fix16 *p = o->h; int t = o->animFrame & 1; int h = p->p.whole;
                    if (t) t = h + 0x10; else t = h - 0x10;
                    p->p.whole = t;
                }
                o->step = 2;
                o->state = 3;
            }
            break;
        case 0x21:
            o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba;
            if (DAT_8009f0ec->w98 == 0 || *(unsigned short *)&DAT_8009f0ec->b04 == 0x502) {
                FUN_8010bd08(o);
                o->b9c = 2;
                o->step = 2;
                o->state = 3;
            }
            break;
        default:
            o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba;
            break;
        }
        FUN_8001fec0(o);
        if (o->animFrame & 1) u = *(unsigned short *)0x1F8001FC & 0x80;
        else u = *(unsigned short *)0x1F8001FC & 0x20;
        if (u) {
            turn(o);
        }
    tail:
        if (!chk(o)) FUN_8010e328(o, 2);
        break;
    case 3:
        FUN_8001fec0(o);
        SETPOS(o);
        o->wb8 += (DAT_8009c330->w10 * tbl[o->d88 % 12]) >> 8;
        if (++o->d88 < 12) return;
        o->animFrame ^= 1;
        if (o->animFrame & 1) {
            o->wb8 = o->velX + o->wb2;
            SETPOS(o);
        } else {
            o->wb8 = o->velX - o->wb2;
            SETPOS(o);
        }
        o->anim = (void *)FUN_80010d68;
        FUN_8001fe94(o, 0);
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        DAT_8009c330->b4 = 0;
        o->state = 0;
        break;
    }
}

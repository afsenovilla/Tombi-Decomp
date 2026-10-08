// FUNC 8012ba94 2100 X000
// MATCHING 8012ba94 2100
#include "TOBJ.H"
extern char D_80077CDC[];
extern unsigned char D_801390EC[];
extern unsigned short *D_8013A160[];
extern unsigned short *D_8013A1D8[];
extern unsigned short *D_8013A1D4[];
extern unsigned short *D_8013A1DC[];
extern unsigned short *D_8013A1BC[];
extern unsigned char D_80138FD8[];
extern unsigned short D_1F800282;
extern int AnimAdvanceWithBox(TObj *);
extern void FUN_8001faf4(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern TObj *FUN_80018448(void);

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

static __inline__ void Spawn(TObj *o)
{
    TObj *n = FUN_80018448();
    if (n) {
        n->active = 1;
        n->type = 0x10;
        n->subtype = 1;
        n->a.p.whole = o->a.p.whole;
        n->y.p.whole = o->y.p.whole + 0x10;
        n->b.p.whole = o->b.p.whole;
    }
}

#define FALL() \
    FUN_8001faf4(o); \
    o->velV += 0x40; \
    if (o->velV > 0x500) \
        o->velV = 0x500; \
    o->y.raw += o->velV << 8;

void FUN_8012ba94(TObj *o)
{
    TObj *p;
    unsigned char k;

    switch (o->state) {
    case 0:
        o->active = 5;
        o->movetab = D_80077CDC;
        o->timer = 0;
        o->w22 = 0;
        o->b69 = 0;
        o->state++;
        k = D_801390EC[o->subtype];
        o->wac = k;
        o->anim = D_8013A160[k];
        SetBox(o);
        break;
    case 1:
        AnimAdvanceWithBox(o);
        p = (TObj *)o->d94;
        o->h->p.whole = (short)(p->h->p.whole - 0xe) - (o->w22 << 1);
        o->y.p.whole = p->y.p.whole - 0x17;
        o->d8c = -(p->d8c >> 4) & 0xff;
        if (o->timer == 0) {
            if (o->d8c != 0)
                o->timer = 1;
        } else if (o->d8c == 0) {
            o->timer = 0;
            if (++o->w22 == 3)
                o->state++;
        }
        break;
    case 2:
        p = (TObj *)o->d94;
        o->h->p.whole = (short)(p->h->p.whole - 0xe) - (o->w22 << 1);
        o->y.p.whole = p->y.p.whole - 0x17;
        o->d8c = -(p->d8c >> 4) & 0xff;
        if (o->d8c >= 4) {
            o->velV = 0;
            o->state++;
        }
        break;
    case 3:
        FALL();
        o->d8c += 5;
        if (o->d8c >= 0x33) {
            o->b9c = 2;
            o->d8c = 0xc0;
            o->b69 = 0;
            o->wac = 0x1e;
            o->state++;
            o->anim = D_8013A1D8[0];
            SetBox(o);
        }
        break;
    case 4:
        FALL();
        o->d8c += 5;
        if (o->d8c >= 0x100)
            o->d8c = 0x100;
        if (o->b69 == 1 || (o->b69 & 8) || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            if (((D_1F800282 >> 5) & 0xf) == 0xf)
                o->w98 = 0;
            if (!(o->b69 & 8))
                Spawn(o);
            o->active = 1;
            o->timer = 2;
            o->b69 = 0;
            o->b9c = 0;
            o->wac = 0x1d;
            o->state++;
            o->anim = D_8013A1D4[0];
            SetBox(o);
        }
        break;
    case 5:
        if (--o->timer == -1) {
            o->b9c = 1;
            o->velV = -0x300;
            o->timer = 0;
            o->b69 = 0;
            o->d8c = 0;
            o->wac = 0x1e;
            o->state++;
            o->anim = D_8013A1D8[0];
            SetBox(o);
        }
        break;
    case 6:
        FALL();
        if (o->velV > 0)
            o->b9c = 2;
        if (o->b69 == 1 || (o->b69 & 8) || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            if (((D_1F800282 >> 5) & 0xf) == 0xf)
                o->w98 = 0;
            if (!(o->b69 & 8))
                Spawn(o);
            o->timer = 2;
            o->b69 = 0;
            o->b9c = 0;
            o->wac = 0x1d;
            o->state++;
            o->anim = D_8013A1D4[0];
            SetBox(o);
        }
        break;
    case 7:
        if (--o->timer == -1) {
            o->b9c = 1;
            o->velV = -0x200;
            o->timer = 0;
            o->d8c = 0;
            o->wac = 0x1e;
            o->state++;
            o->anim = D_8013A1D8[0];
            SetBox(o);
        }
        break;
    case 8:
        FALL();
        if (o->velV > 0)
            o->b9c = 2;
        if (o->timer == 0) {
            o->d8c += 2;
            if (o->d8c >= 0x16) {
                o->d8c = 0xe0;
                o->timer = 1;
                o->wac = 0x1f;
                o->anim = D_8013A1DC[0];
                SetBox(o);
            }
        } else {
            o->d8c += 4;
            if (o->d8c >= 0x100)
                o->d8c = 0x100;
        }
        if (o->b69 == 1 || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
            if (((D_1F800282 >> 5) & 0xf) == 0xf)
                o->w98 = 0;
            Spawn(o);
            o->timer = 0x14;
            o->b69 = 0;
            o->b9c = 0;
            o->d8c = 0;
            o->wac = 0x17;
            o->state++;
            o->anim = D_8013A1BC[0];
            SetBox(o);
        }
        break;
    case 9:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->step = 1;
            o->state = 1;
            o->substep = 2;
            o->b69 = 0;
            o->b68 = 0;
        }
        break;
    }
}

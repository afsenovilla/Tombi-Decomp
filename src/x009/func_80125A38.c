// FUNC 80125a38 1684 X009
// MATCHING 80125a38 1684
#include "TOBJ.H"

typedef struct { short x, y; } P2;
extern void *D_8012EE80[], *D_8012EE84[], *D_8012EEB0[], *D_8012EEB4[], *D_8012EEB8[], *D_8012EEBC[];
extern void *D_8012EEC0[];
extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern unsigned short *D_800A6078;
extern unsigned short D_800A604E;
extern unsigned char D_800A603E;
extern short D_800A6066;
extern unsigned char D_8009C970;
extern unsigned short D_1F80016A, D_1F80016E;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern int FUN_8002078c(P2, P2);
extern int func_8004065C(TObj *, int, int, int);
extern short TileCollideAt(TObj *, int, int);
extern TObj *FUN_800183b8(void);

static __inline__ int Blocked(TObj *o)
{
    short d;
    short af = o->animFrame;
    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) return 1;
    if (af) d = -0x10;
    else d = 0x10;
    return (short)func_8004065C(o, (short)(o->h->p.whole + d), (short)(o->y.p.whole + 0x30), af);
}

void func_80125A38(TObj *o)
{
    TObj *p;
    TObj *n;
    short s;
    short c;
    P2 a, b;

    switch (o->state) {
    case 0:
        p = *(TObj **)&o->wa8;
        if (*(unsigned short *)&p->b04 == 0x301 || p->b04 == 2) {
            o->step = 0;
            o->state = 0;
            break;
        }
        o->state++;
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        b.x = D_1F80016A;
        b.y = D_1F80016E - 0x40;
        o->d88 = FUN_8002078c(a, b);
        if ((unsigned)(o->d88 - 0x41) < 0x80) o->animFrame = 1;
        else o->animFrame = 0;
        o->velH = 0x180;
        o->timer = 0xb4;
        o->wac = 0;
        o->anim = D_8012EE84[0];
        AnimLoadDuration(o);
        break;
    case 1:
        AnimAdvance(o);
        o->h->raw += (D_8007A5F0[*(unsigned char *)&o->d88] * o->velH) >> 4;
        s = Blocked(o);
        o->y.raw += (D_8007A3F0[*(unsigned char *)&o->d88] * o->velH) >> 4;
        c = TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x20));
        if (s || c) {
            o->state = 10;
            o->animFrame = 1 - o->animFrame;
            break;
        }
        if (--o->timer == -1) {
            o->state = 10;
            break;
        }
        if ((((unsigned short)o->h->p.whole - D_800A6078[1] + 8) & 0xffff) > 0x10) break;
        o->wac = 0;
        o->state++;
        o->anim = D_8012EEB0[0];
        AnimLoadDuration(o);
        n = FUN_800183b8();
        if (n) {
            n->active = 2;
            n->type = 0x21;
            n->subtype = o->subtype;
            n->b0c = 1;
            n->b0a = 0;
            n->b0f = o->b0f;
            n->b04 = o->b04;
            n->step = o->step;
            n->state = o->state;
            n->w1e = o->w1e;
            n->b0d = 1;
            n->w08 = o->w08;
            n->d3c = o->d3c;
            n->wac = 0;
            n->anim = D_8012EEC0[o->wac];
            AnimLoadDuration(n);
            n->animFrame = o->animFrame;
            n->h->raw = o->h->raw;
            n->y.raw = o->y.raw;
            n->d->raw = o->d->raw;
            n->box0 = 0x10;
            n->box1 = 0x20;
            n->box2 = 0;
            n->box3 = 0x20;
            n->d90 = (int)o;
        }
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box2 = 0x10;
        o->box3 = 0x30;
        o->b6a = 1;
        break;
    case 2:
        AnimAdvance(o);
        if (o->b6a == 2) {
            o->state++;
            break;
        }
        o->y.p.whole += 2;
        if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x20)) || o->b69) {
            o->state = 10;
            o->b6a = 0;
            o->b69 = 0;
        }
        break;
    case 3:
        o->wac = 1;
        o->state++;
        o->anim = D_8012EEB4[0];
        AnimLoadDuration(o);
        playSFX(0x9b);
        break;
    case 4:
        if (AnimAdvance(o)) o->state++;
        break;
    case 5:
        o->wac = 2;
        o->state++;
        o->anim = D_8012EEB8[0];
        AnimLoadDuration(o);
        break;
    case 6:
        if (AnimAdvance(o)) {
            o->timer = 0x20;
            o->state++;
        }
        break;
    case 7:
        AnimAdvance(o);
        o->y.p.whole -= 2;
        { unsigned short *k = &D_800A604E; *k -= 2; }
        if (--o->timer == -1) o->state++;
        break;
    case 8:
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box3 = 0x20;
        o->box2 = 0x10;
        o->wac = 3;
        o->state++;
        o->anim = D_8012EEBC[0];
        AnimLoadDuration(o);
        o->timer = 0x10;
        break;
    case 9:
        if (--o->timer == -1) {
            { unsigned char *c = &D_800A603E; *c = *c + 1; }
            D_800A6066 = o->animFrame;
            o->b6a = 0;
        }
        if (AnimAdvance(o)) {
            o->wac = 0;
            o->state++;
            o->anim = D_8012EE80[0];
            AnimLoadDuration(o);
        }
        break;
    case 10:
        o->active = 1;
        o->step = 0;
        o->state = 0;
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box2 = 0x10;
        o->box3 = 0x20;
        break;
    }
    if (D_8009C970 == 0 && o->b6a == 2) {
        { unsigned char *c = &D_800A603E; *c = *c + 1; }
        D_800A6066 = o->animFrame;
        o->b6a = 0;
    }
}

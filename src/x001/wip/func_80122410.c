// FUNC 80122410 2220 X001
/* score 33: whole function incl. csv pieces 801228BC/80122990/80122A18 (ends at 80122CBC). Remaining: in case 4 the orbit pointer g and the %12 magic constant swap a1/a2, and at the end the la of the anim pointer is scheduled before the AnimJump arg moves (the empty do/while(0) after the final orbit, debt, fixed the y-store placement). Tried: block-local/function g, explicit orbit code, macro orbit, index temps. */
#include "TOBJ.H"
typedef struct {
    unsigned char b0, b1;
    short w02;
    unsigned char b4, b5, b6, b7, b8, b9;
    char pa[0xc - 0xa];
    short w0c, w0e, w10, w12;
    char p14[0x28 - 0x14];
    unsigned short w28, w2a;
    char p2c[0x2e - 0x2c];
    unsigned short w2e;
} P;
typedef struct L { unsigned char active; char p[0x94 - 1]; struct L *next; } L;
extern P *D_8009C330;
extern TObj *D_8009F0EC;
extern unsigned short D_8009D670;
extern unsigned short D_1F8001FC, D_1F8001F8;
extern void FUN_80010d68();
extern void FUN_80010dac();
extern void SfxPlay3(int, int);
extern void SfxPlay2(int, int);
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int rcos(int);
extern int rsin(int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern TObj *func_8004BBC0(TObj *, int);
extern void FUN_800ee428(TObj *);
extern void func_8010E328(TObj *, int);
extern void func_801222C4(TObj *);

static __inline__ TObj *orbit2(TObj *o, int a)
{
    int c, s;
    TObj *g;
    a = (a + 0x400) & 0xfff;
    c = (rcos(a) * o->wba) >> 12;
    s = rsin(a) * o->wba;
    g = D_8009F0EC;
    o->h->p.whole = o->wb8 + (g->h->p.whole + c) + g->d30;
    o->y.p.whole = g->y.p.whole + (s >> 12);
    return g;
}
#define orbit(o) orbit2(o, D_8009F0EC->d8c)

void func_80122410(TObj *o)
{
    short t[12] = { 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1 };
    P *p;
    TObj *g;
    TObj *r;
    unsigned short *k;

    D_8009C330->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        p = D_8009C330;
        g = D_8009F0EC;
        o->wb6 = -0x420;
        p->w0e = 0;
        p->w02 = 0x10;
        o->wb2 = g->box1;
        o->animFrame &= 1;
        p->b6 = g->active;
        o->ba4 = 0;
        D_8009C330->b9 = 0;
        {
        P *q = D_8009C330;
        q->w2e = 0xffff;
        q->w28 = 0xffff;
        q->w2a = 0xffff;
        q->w0c = 0;
        }
        ((unsigned char *)&o->wa8)[1] = 1;
        g = D_8009F0EC;
        o->h->p.whole = o->wb8 + (g->h->p.whole + g->d30);
        o->y.p.whole = g->y.p.whole + o->wba;
        o->velH = g->h->p.whole - o->h->p.whole;
        o->velV = g->y.p.whole - o->y.p.whole;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velX = o->wb8;
        {
        P *r2 = D_8009C330;
        r2->w10 = g->h->p.whole + o->wb8;
        r2->w12 = g->y.p.whole + o->wba;
        }
        o->anim = (void *)FUN_80010d68;
        AnimJump(o, 0);
        o->timer = 10;
        o->state++;
    case 2:
        orbit(o);
        if (--o->timer == 0) o->state++;
        func_801222C4(o);
        break;
    case 3:
        AnimAdvance(o);
        k = &D_8009D670;
        if (!(*k & 0x50)) {
            switch (*(unsigned short *)o->anim) {
            case 0x75:
            case 0x149:
            case 0x14a:
            case 0x14b:
                break;
            default:
                o->anim = (void *)FUN_80010d68;
                AnimJump(o, 0);
                {
                    P *q = D_8009C330;
                    q->w2e = 0xffff;
                    q->w28 = 0xffff;
                    q->w2a = 0xffff;
                }
                break;
            }
            o->velY = 0;
            if (o->animFrame & 1) {
                if (D_1F8001FC & 0x80) {
                    o->anim = (void *)FUN_80010dac;
                    AnimJump(o, 0);
                    D_8009C330->b4 = 1;
                    D_8009F0EC->b69 = 4;
                    SfxPlay2(0x1e, 8);
                    D_8009C330->w10 = -((D_8009F0EC->box1 << 8) / 24);
                    o->state++;
                }
            } else {
                if (D_1F8001FC & 0x20) {
                    o->anim = (void *)FUN_80010dac;
                    AnimJump(o, 0);
                    D_8009C330->b4 = 1;
                    D_8009F0EC->b69 = 4;
                    SfxPlay2(0x1e, 8);
                    D_8009C330->w10 = (D_8009F0EC->box1 << 8) / 24;
                    o->state++;
                }
            }
        } else {
            if (!(D_1F8001F8 & 0xf)) SfxPlay2(0x1d, 0);
            if (*k & 0x10) {
                PlayerSetAnimIfChanged(o, 0x25);
                if (D_8009F0EC->y.p.whole - D_8009F0EC->d38 - 0x10 < o->y.p.whole) o->y.raw += -0x18000;
            } else {
                PlayerSetAnimIfChanged(o, 0x4a);
                o->y.raw += 0x28000;
            }
        }
        o->b9e = 0;
        r = func_8004BBC0(o, 0);
        if (r == 0) {
            L *q, *n;
            ((unsigned char *)&o->wa8)[1] = 0;
            n = ((L *)D_8009F0EC)->next;
            if (n) {
                q = n;
                goto chk;
            loop:
                q->active = 3;
                q = q->next;
            chk:
                if (q->next) goto loop;
                q->active = 1;
            }
            D_8009F0EC = r;
            o->b9c = 2;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->velY = 0;
            FUN_800ee428(o);
            D_8009C330->b4 = 0;
            o->step = 2;
            o->state = 3;
            break;
        }
        D_8009F0EC = r;
        orbit(o);
        o->d88 = 0;
        func_8010E328(o, 2);
        func_801222C4(o);
        break;
    case 4:
        AnimAdvance(o);
        g = orbit(o);
        o->wb8 += (D_8009C330->w10 * t[o->d88 % 12]) >> 8;
        if (++o->d88 >= 12) {
            o->animFrame ^= 1;
            if (o->animFrame & 1) o->wb8 = o->velX + o->wb2;
            else o->wb8 = o->velX - o->wb2;
            orbit2(o, g->d8c);
            do { } while (0);
            o->anim = (void *)FUN_80010d68;
            AnimJump(o, 0);
            o->state = 1;
            o->velY = 0;
            o->d84 = 0;
            o->d88 = 0;
            D_8009C330->b4 = 0;
        }
        break;
    }
}

// FUNC 8011beac 2220 X010
// MATCHING 8011beac 2220
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
    char pad2c[2];
    unsigned short w2e;
} G330;
extern G330 *D_8009C330;
extern TObj *D_8009F0EC;
extern unsigned short D_8009D670;
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8001F8;
extern void FUN_80010d68();
extern void FUN_80010dac();
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void SfxPlay3(int, int);
extern int rcos(int);
extern int rsin(int);
extern TObj *func_8004BBC0(TObj *, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_800ee428(TObj *);
extern void func_8010E328(TObj *, int);
extern void func_8011BD5C(TObj *);

#define ORBIT(o) { \
    int a = (D_8009F0EC->d8c + 0x400) & 0xfff; \
    int x = rcos(a) * o->wba >> 12; \
    int y = rsin(a) * o->wba; \
    o->h->p.whole = o->wb8 + (D_8009F0EC->h->p.whole + x) + D_8009F0EC->d30; \
    o->y.p.whole = D_8009F0EC->y.p.whole + (y >> 12); \
}

void func_8011BEAC(TObj *o)
{
    short tbl[12] = { 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1 };
    unsigned short *pad;
    TObj *e;

    D_8009C330->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        o->wb6 = -0x420;
        D_8009C330->w0e = 0;
        D_8009C330->w02 = 0x10;
        o->wb2 = D_8009F0EC->box1;
        o->animFrame &= 1;
        D_8009C330->b6 = D_8009F0EC->active;
        D_8009C330->b9 = 0;
        D_8009C330->w2e = 0xffff;
        D_8009C330->w28 = 0xffff;
        D_8009C330->w2a = 0xffff;
        D_8009C330->w0c = 0;
        ((unsigned char *)&o->wa8)[1] = 1;
        o->h->p.whole = o->wb8 + (D_8009F0EC->h->p.whole + D_8009F0EC->d30);
        o->y.p.whole = D_8009F0EC->y.p.whole + o->wba;
        o->velH = D_8009F0EC->h->p.whole - o->h->p.whole;
        o->velV = D_8009F0EC->y.p.whole - o->y.p.whole;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velX = o->wb8;
        D_8009C330->w10 = D_8009F0EC->h->p.whole + o->wb8;
        D_8009C330->w12 = D_8009F0EC->y.p.whole + o->wba;
        o->anim = (void *)FUN_80010d68;
        AnimJump(o, 0);
        o->timer = 10;
        o->state++;
    case 2:
        ORBIT(o);
        if (--o->timer == 0) o->state++;
        func_8011BD5C(o);
        break;
    case 3:
        AnimAdvance(o);
        pad = &D_8009D670;
        if (!(*pad & 0x50)) {
            switch (*(unsigned short *)o->anim) {
            case 0x75:
            case 0x149:
            case 0x14a:
            case 0x14b:
                break;
            default:
                o->anim = (void *)FUN_80010d68;
                AnimJump(o, 0);
                D_8009C330->w2e = 0xffff;
                D_8009C330->w28 = 0xffff;
                D_8009C330->w2a = 0xffff;
                break;
            }
            o->velY = 0;
            if (o->animFrame & 1) {
                if (D_1F8001FC & 0x80) {
                    o->timer = 0;
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
                    o->timer = 0;
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
            if ((D_1F8001F8 & 0xf) == 0) SfxPlay2(0x1d, 0);
            if (*pad & 0x10) {
                PlayerSetAnimIfChanged(o, 0x25);
                if (D_8009F0EC->y.p.whole - D_8009F0EC->d38 - 0x10 < o->y.p.whole)
                    o->y.raw += -0x18000;
            } else {
                PlayerSetAnimIfChanged(o, 0x4a);
                o->y.raw += 0x28000;
            }
        }
        o->b9e = 0;
        e = func_8004BBC0(o, 0);
        if (e == 0) {
            TObj *q;
            ((unsigned char *)&o->wa8)[1] = 0;
            if (D_8009F0EC->d94) {
                q = (TObj *)D_8009F0EC->d94;
                goto test;
            body:
                q->active = 3;
                q = (TObj *)q->d94;
            test:
                if (q->d94) goto body;
                q->active = 1;
            }
            D_8009F0EC = e;
            o->timer = 0;
            D_8009C330->b4 = 0;
            o->b9c = 2;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->velY = 0;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
            break;
        }
        D_8009F0EC = e;
        ORBIT(o);
        o->d88 = 0;
        func_8010E328(o, 2);
        func_8011BD5C(o);
        break;
    case 4:
        AnimAdvance(o);
        ORBIT(o);
        o->wb8 += (D_8009C330->w10 * tbl[o->d88 % 12]) >> 8;
        if (++o->d88 < 12) break;
        o->animFrame ^= 1;
        if (o->animFrame & 1) {
            o->wb8 = o->velX + o->wb2;
            ORBIT(o);
        } else {
            o->wb8 = o->velX - o->wb2;
            ORBIT(o);
        }
        o->anim = (void *)FUN_80010d68;
        AnimJump(o, 0);
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        D_8009C330->b4 = 0;
        o->state = 1;
        break;
    }
}

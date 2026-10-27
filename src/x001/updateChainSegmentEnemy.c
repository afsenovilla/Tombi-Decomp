// FUNC 8012f448 1312 X001
// MATCHING 8012f448 1312
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { unsigned short b0, b1, b2, b3, w1e, wac; int idx; void **anims; } E20;
typedef struct { Fix16 a, y, b; } FV;
extern E20 D_8013C928[];
extern int SPR_DATA[];
static char D_8013C94A[50] = {
    0, 0, -0x24, -0x1b, 0x13, -0x80, 1, 1, 0, 0x10, 0, 0, 1, 1, 0, 0x10,
    0, 0, 1, 1, 0, 0x11, 0, 0, 1, 1, 0, 0x12, 0, 0, 1, 1,
    0, 0x13, 0, 0, 1, 1, 0, 0x14, 0, 0, 1, 1, 0, 0x15, 0, 0,
    -0x7d,
};
extern char MOVE_TABLE_2[];
extern void AnimLoadDuration(TObj *);
extern int FUN_800202b4(TObj *);
extern int rcos(int);
extern int rsin(int);
extern void spawnItemNotification(int, int);
extern void applyItemEffect(char *, FV *, int, int, int);
extern void applyAnimVelocityX(TObj *, int);
extern void freeObjectLayer2(TObj *);

void updateChainSegmentEnemy(TObj *o)
{
    TObj *p;
    FV v;
    int a, s, t;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->b6b = o->animFrame;
        o->animFrame = 0;
        o->b0d = 0;
        o->box0 = D_8013C928[o->subtype].b0;
        o->box1 = D_8013C928[o->subtype].b1;
        o->box2 = D_8013C928[o->subtype].b2;
        o->box3 = D_8013C928[o->subtype].b3;
        o->w1e = D_8013C928[o->subtype].w1e;
        o->d3c = SPR_DATA[D_8013C928[o->subtype].idx];
        o->wac = D_8013C928[o->subtype].wac;
        o->anim = D_8013C928[o->subtype].anims[o->wac];
        AnimLoadDuration(o);
        if (o->d94)
            o->active = 3;
        else
            o->active = 1;
        o->b0a = 2;
        o->b68 = 0;
        o->d8c = 0;
        if (o->d90) {
            p = (TObj *)o->d90;
            while (p->d90)
                p = (TObj *)p->d90;
            *(TObj **)&o->wa8 = p;
            o->velX = o->h->p.whole - p->h->p.whole;
            o->velY = o->y.p.whole - p->y.p.whole;
        } else {
            *(TObj **)&o->wa8 = 0;
        }
        break;
    case 1:
        if (GVARS.b42) {
            FUN_800202b4(o);
            break;
        }
        if (FUN_800202b4(o) == 0) break;
        o->b0f = PLAYER.obj.b0f + 1;
        p = *(TObj **)&o->wa8;
        if (p == 0) break;
        if (o->b68) {
            p->b68 = 1;
            o->b68 = 0;
        }
        a = (p->d8c + 0x400) & 0xfff;
        s = (rcos(a) * o->velY) >> 12;
        t = rsin(a) * o->velY;
        o->h->p.whole = s + (p->h->p.whole + o->velX);
        o->y.p.whole = p->y.p.whole + (t >> 12);
        break;
    case 2:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            p = (TObj *)o->d90;
            p->active = 1;
            p->d94 = 0;
            o->step = 1;
            o->state = 0;
            if (o->subtype == 0)
                spawnItemNotification(0, 6);
            break;
        case 1:
            if (o->state == 1) {
                o->state++;
                if (o->b6b) {
                    v.a.p.whole = o->h->p.whole;
                    v.y.p.whole = o->y.p.whole;
                    v.b.p.whole = o->d->p.whole;
                    applyItemEffect(D_8013C94A + o->b6b * 6, &v, 0, 0, 1);
                }
            }
            break;
        case 2:
            switch (o->state) {
            case 0:
                o->velV = -0x400;
                o->movetab = MOVE_TABLE_2;
                o->animFrame &= 1;
                o->state++;
                break;
            case 1:
                applyAnimVelocityX(o, (unsigned short)(1 - o->animFrame));
                o->velV += 0x40;
                if (o->velV > 0x400)
                    o->velV = 0x400;
                o->y.raw += o->velV << 8;
                break;
            }
            if (o->animFrame & 1)
                o->d8c = (o->d8c + 0x14) & 0xff;
            else
                o->d8c = (o->d8c - 0x14) & 0xff;
            if (!o->visible)
                o->b04 = 3;
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}

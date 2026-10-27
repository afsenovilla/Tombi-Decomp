// FUNC 8012ae1c 1972 X000
// MATCHING 8012ae1c 1972
#include "TOBJ.H"
typedef struct P { unsigned char b0; unsigned char b1; unsigned short w2; short w4; } P;
extern char MOVE_TABLE_2[];
extern char MOVE_TABLE_1[];
extern char MOVE_TABLE_0[];
extern unsigned short *D_8013A204[];
extern unsigned short *D_8013A1E4[];
extern unsigned short *D_8013A1E8[];
extern unsigned short *D_8013A208[];
extern unsigned short *D_8013A20C[];
extern unsigned short *D_8013A210[];
extern unsigned char KOMA_PIG_ANIM_BOXES[];
extern void FUN_8001e4f0(int);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern short probeWallAt(TObj *, short, short, short);
extern int AnimAdvanceWithBox(TObj *);
extern int enemyProbeGround(TObj *);
extern void cloneItemPickupObject(TObj *);

#define SETBOX(o, A) \
    { \
        unsigned short *aa = A; unsigned char *b; \
        o->anim = aa; \
        b = KOMA_PIG_ANIM_BOXES + aa[1] * 4; \
        o->box0 = *b++; \
        o->box1 = *b++; \
        o->box2 = b[0]; \
        o->box3 = b[1]; \
    }
#define ANIMT(o) o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff

static __inline__ void move(TObj *o)
{
    P *p = (P *)&o->wb4;
    if (o->movetab != 0) {
        applyAnimVelocityX(o, 1 - o->animFrame);
    }
    if (o->b9d != 0) {
        if (o->animFrame != (o->b9d & 1)) {
            o->movetab = 0;
        }
        o->b9d = 0;
    }
    if (o->animFrame == 1) p->w4 = 0x10;
    else p->w4 = -0x10;
    if (probeWallAt(o, o->h->p.whole + p->w4, o->y.p.whole, 1 - o->animFrame)) {
        o->movetab = 0;
    }
}

static __inline__ void fin(TObj *t)
{
    t->y.p.whole += 2;
    enemyProbeGround(t);
}

static __inline__ void fin2(TObj *t)
{
    ANIMT(t);
    cloneItemPickupObject(t);
}

void komaPigHit(TObj *o)
{
    P *p = (P *)&o->wb4;
    unsigned char t;
    unsigned short *a;

    switch (o->state) {
    case 0:
        o->category |= 0x80;
        FUN_8001e4f0(0xf);
        o->velV = -0x400;
        o->movetab = MOVE_TABLE_2;
        o->b9c = 1;
        o->wac = 0x29;
        o->b69 = 0;
        o->d8c = 0;
        o->animFrame = (1 - o->w7a) & 1;
        o->state++;
        SETBOX(o, D_8013A204[0]);
        ANIMT(o);
    case 1:
        move(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        return;
    case 2:
        move(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (enemyProbeGround(o)) {
            if (o->movetab == 0) {
                { unsigned short u = p->w2; o->state = 4; o->timer = 0x168; o->wac = 0x22; o->d8c = u; SETBOX(o, D_8013A1E8[0]); fin2(o); }
            } else {
                o->b9c = 1;
                o->active = 3;
                o->velV = -0x280;
                o->d8c = 0;
                o->movetab = MOVE_TABLE_1;
                o->state++;
                p->b1 = 1;
                o->b69 = 0;
                o->wac = 0x21;
                SETBOX(o, D_8013A1E4[0]);
                ANIMT(o);
            }
        }
        return;
    case 3:
        {
            int x;
            if (o->animFrame != 0) x = o->d8c - 20;
            else x = o->d8c + 20;
            o->d8c = x & 0xff;
        }
        move(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (enemyProbeGround(o)) {
                if (o->movetab == 0) {
                    { unsigned short u = p->w2; o->state = 4; o->timer = 0x168; o->wac = 0x22; o->d8c = u; SETBOX(o, D_8013A1E8[0]); fin2(o); }
                } else if (t = p->b1, p->b1 = t + 0xff, t != 0) {
                    o->velV = ~(o->velV - 0x80) + 1;
                    if (o->velV < 0) o->b9c = 1;
                    o->movetab = MOVE_TABLE_0;
                } else {
                    o->d8c = p->w2;
                    o->timer = 0x168;
                    o->wac = 0x22;
                    o->state++;
                    SETBOX(o, D_8013A1E8[0]);
                    fin2(o);
                }
            }
        }
        return;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x2a;
            o->state++;
            a = D_8013A208[0];
            goto common;
        }
        break;
    case 5:
        if (AnimAdvanceWithBox(o)) {
            o->timer = 0x78;
            o->wac = 0x2b;
            o->state++;
            a = D_8013A20C[0];
            goto common;
        }
        break;
    case 6:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0x2c;
            o->state++;
            a = D_8013A210[0];
        common:
            SETBOX(o, a);
            ANIMT(o);
        }
        break;
    case 7:
        AnimAdvanceWithBox(o);
        if (--o->timer == -1) {
            *(signed char *)&o->b0f = -9;
            o->active = 1;
            o->b04 = 1;
            o->category &= 0x7f;
            if (o->subtype == 8) o->step = 3;
            else o->step = 1;
            o->state = 1;
            o->substep = 2;
            o->b69 = 0;
            o->b68 = 0;
            o->b9c = 0;
        }
        break;
    default:
        return;
    }
    fin(o);
}

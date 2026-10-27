// FUNC 8012247c 1496 X004
// MATCHING 8012247c 1496
#include "spad.h"
#include "player.h"
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern char MOVE_TABLE_1[];
extern B D_80130FD4[];
extern A *D_80133B5C[];
extern unsigned char D_800A60E4[];
extern short D_1F80027E;
extern unsigned short D_1F800282;
extern void spawnItemNotification(int, int);
extern void applyFrameVelocityY(TObj *);
extern void applyFrameVelocityX(TObj *);
extern TObj *FUN_80018448(void);
extern short probeWallAt(TObj *, short, short, short);
extern int komaPigCheckLanded(TObj *);

#define SETANIM(k) \
    o->anim = D_80133B5C[k]; \
    { unsigned char *p; { B *t = D_80130FD4; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define COMMON() \
    applyFrameVelocityY(o); \
    probeWallAt(o, o->h->p.whole + 8, o->y.p.whole + 8, 0); \
    probeWallAt(o, o->h->p.whole - 8, o->y.p.whole + 8, 1); \
    komaPigCheckLanded(o);

void komaPigGrabbed(TObj *o)
{
    TObj *n;
    short v;
    int d;

    switch (o->state) {
    case 0:
        o->b9c = 0;
        spawnItemNotification(0, 6);
        o->movetab = MOVE_TABLE_1;
        *(signed char *)&o->b0f = -7;
        o->b68 = 0;
        o->d8c = 0;
        o->velV = 0;
        o->state++;
    case 1:
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        probeWallAt(o, o->h->p.whole + 8, o->y.p.whole + 8, 0);
        probeWallAt(o, o->h->p.whole - 8, o->y.p.whole + 8, 1);
        if (komaPigCheckLanded(o)) o->velV = 0;
        break;
    case 2:
        PLAYER.obj.state = 2;
        o->timer = 4;
        o->wac = 0x23;
        o->state++;
        SETANIM(0x23);
        COMMON();
        break;
    case 3:
        if (--o->timer == -1) {
            o->timer = 8;
            o->wac = 0x24;
            o->state++;
            SETANIM(0x24);
            n = FUN_80018448();
            if (n) {
                n->active = 1;
                n->type = 0x10;
                n->subtype = 0;
                d = -0x10;
                if (o->animFrame & 1) d = 0x10;
                n->a.p.whole = o->a.p.whole + d;
                n->y.p.whole = o->y.p.whole;
                n->b.p.whole = o->b.p.whole;
                n->animFrame = o->animFrame & 1;
            }
        }
        COMMON();
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x25;
            SETANIM(0x25);
            o->state++;
            o->timer = 0xe;
        }
        COMMON();
        break;
    case 5:
        applyFrameVelocityX(o);
        o->y.p.whole += 3;
        probeWallAt(o, o->h->p.whole + 8, o->y.p.whole + 8, 0);
        probeWallAt(o, o->h->p.whole - 8, o->y.p.whole + 8, 1);
        if (komaPigCheckLanded(o)) {
            v = D_1F80027E;
            if (v < 0) v = -v;
            if (v > 8) v = 8;
            if (D_1F80027E < 0) v = -v;
            o->wb2 = v;
            o->d8c = -v & 0xff;
            o->wb6 = (-v << 2) & 0xff;
            o->wae = (*(unsigned short*)&SPAD.s284);
            o->b9c = 0;
            if ((short)((D_1F800282 >> 5) & 0xf) >= 0xd) o->w98 = 0;
        }
        if (--o->timer == -1) {
            D_800A60E4[0] = 3;
            o->wac = 0x23;
            SETANIM(0x23);
            o->state++;
        }
        break;
    case 6:
        o->wac = 0x23;
        SETANIM(0x23);
        COMMON();
        break;
    case 7:
        o->wac = 0x26;
        SETANIM(0x26);
        o->state++;
        break;
    case 8:
        break;
    }
}

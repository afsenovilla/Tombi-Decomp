// FUNC 8010c034 1916 X003
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
extern G330 *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern unsigned char GAME_PLAYER_MODE;
extern int GAME_AREA00_EVENT_CONTROL;
extern unsigned short PAD_PRESSED;
extern void FUN_80010d68();
extern void FUN_80010dac();
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001e560(int, int);
extern void playerReleaseClingObject(TObj *);
extern void playerTryAttack(TObj *, int);

#define SETPOS(o) \
    o->h->p.whole = GRABBED_OBJECT->h->p.whole + o->wb8; \
    if (GRABBED_OBJECT->type == 0x24) o->y.p.whole = GRABBED_OBJECT->y.p.whole + o->wba + 0x10; \
    else o->y.p.whole = GRABBED_OBJECT->y.p.whole + o->wba;

static __inline__ short chk(TObj *o)
{
    short r = 0;

    if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
        GAME_PLAYER_MODE = 0;
        playerReleaseClingObject(o);
        if (GAME_AREA00_EVENT_CONTROL & 0x40) {
            if (*(volatile unsigned short *)&PAD_PRESSED & *(unsigned short *)0x1F8003C4) {
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
    PLAYER_EXT->b4 = 1;
    GRABBED_OBJECT->b69 = 4;
    FUN_8001e560(0x1e, 8);
    p = PLAYER_EXT;
    v = (GRABBED_OBJECT->box1 << 8) / 0x18;
    if (o->animFrame & 1)
        v = -v;
    p->w10 = v;
    o->d88 = 0;
    o->state++;
}

void playerStepClingToObject(TObj *o)
{
    short tbl[12] = { 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1 };
    short y;
    unsigned short u;

    PLAYER_EXT->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        o->wb6 = -0x420;
        PLAYER_EXT->w0e = 0;
        PLAYER_EXT->w02 = 0x10;
        o->wb2 = GRABBED_OBJECT->box1;
        o->animFrame &= 1;
        PLAYER_EXT->b6 = GRABBED_OBJECT->active;
        PLAYER_EXT->b9 = 0;
        PLAYER_EXT->w0c = 0;
        PLAYER_EXT->w28 = 0xffff;
        PLAYER_EXT->w2a = 0xffff;
        o->h->p.whole = o->wb8 + (GRABBED_OBJECT->h->p.whole + GRABBED_OBJECT->d30);
        y = GRABBED_OBJECT->y.p.whole;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->ba4 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->y.p.whole = y + o->wba;
        o->d8c = 0;
        o->velX = o->wb8;
        PLAYER_EXT->w10 = GRABBED_OBJECT->h->p.whole + o->wb8;
        PLAYER_EXT->w12 = GRABBED_OBJECT->y.p.whole + o->wba;
        o->anim = (void *)FUN_80010d68;
        FUN_8001fe94(o, 0);
        o->timer = 10;
        o->state++;
    case 1:
        SETPOS(o);
        if (--o->timer == 0) o->state++;
        goto tail;
    case 2:
        o->h->p.whole = GRABBED_OBJECT->h->p.whole + o->wb8;
        switch (GRABBED_OBJECT->type) {
        case 0x24:
            o->y.p.whole = GRABBED_OBJECT->y.p.whole + o->wba + 0x10;
            break;
        case 0x1d:
            o->y.p.whole = GRABBED_OBJECT->y.p.whole + o->wba;
            if (GRABBED_OBJECT->step == 3) {
                GRABBED_OBJECT->b6a = 0;
                PLAYER_EXT->b4 = 0;
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
            o->y.p.whole = GRABBED_OBJECT->y.p.whole + o->wba;
            if (GRABBED_OBJECT->w98 == 0 || *(unsigned short *)&GRABBED_OBJECT->b04 == 0x502) {
                playerReleaseClingObject(o);
                o->b9c = 2;
                o->step = 2;
                o->state = 3;
            }
            break;
        default:
            o->y.p.whole = GRABBED_OBJECT->y.p.whole + o->wba;
            break;
        }
        FUN_8001fec0(o);
        if (o->animFrame & 1) u = *(unsigned short *)0x1F8001FC & 0x80;
        else u = *(unsigned short *)0x1F8001FC & 0x20;
        if (u) {
            turn(o);
        }
    tail:
        if (!chk(o)) playerTryAttack(o, 2);
        break;
    case 3:
        FUN_8001fec0(o);
        SETPOS(o);
        o->wb8 += (PLAYER_EXT->w10 * tbl[o->d88 % 12]) >> 8;
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
        PLAYER_EXT->b4 = 0;
        o->state = 0;
        break;
    }
}

// FUNC 800f5078 5740 X008
// MATCHING 800f5078 5740
#include "gv2.h"
#include "TOBJ.H"
#include "raw7.h"
typedef struct { unsigned char b0; unsigned char b1; short w2; char p4[8]; short wc; short we; char p10[0x28 - 0x10]; unsigned short w28, w2a; } PL;

extern TObj *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned short D_1f8001fc, D_1f8003c6;
extern unsigned char PLAYER_SWING_ANIM_PAIRS[][2];
extern unsigned char PLAYER_SPIN_JUMP_ANIMS[];
extern char D_80011108[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001e5f4(int, int);
extern void startPadVibration(int, int, int, int);
extern void FUN_8001e560(int, int);
extern void playerSpinPlaceAndAccelerate(TObj *, int, short);
extern void playerSpinAimDir8A(TObj *);
extern void playerSpinAimDir8B(TObj *);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = PLAYER_EXT;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
    }
}

static __inline__ void rot(TObj *o)
{
    PL *p = (PL *)PLAYER_EXT;
    o->wb6 += p->we;
    if ((unsigned short)o->wb6 < 0x800) U8(p, 8) = 1;
    if ((unsigned)((unsigned short)o->wb6 - 0x800) < 0x800) (*(unsigned char*)&PLAYER_EXT->w08) = 0;
    if ((unsigned short)(o->wb6 + 0x7ff) < 0x800) (*(unsigned char*)&PLAYER_EXT->w08) = 0;
    if ((unsigned short)(o->wb6 + 0xfff) < 0x800) (*(unsigned char*)&PLAYER_EXT->w08) = 1;
}

#define SETTBL(o) \
    { \
        unsigned char *b = PLAYER_SWING_ANIM_PAIRS[o->wb2]; \
        PL *q = (PL *)PLAYER_EXT; \
        q->we = b[0]; \
        q->w2 = b[1]; \
    }

#define SND(k) \
    startPadVibration(0, 0, 0xff, 2); \
    FUN_8001e560(3, k); \
    PLAYER_EXT->b0a = GRABBED_OBJECT->b0c;

#define INC1(o, n) \
    { \
        int w = o->wb2; \
        int x; \
        if (w >= n) x = w; \
        else x = w + 1; \
        o->wb2 = x; \
    }
#define INC2(o, n) \
    { \
        int w = o->wb2; \
        short x; \
        if (w < n) x = w + 1; \
        else x = w; \
        o->wb2 = x; \
    }
#define DEC(o) \
    { \
        int w = o->wb2; \
        if (w > 0) w--; \
        o->wb2 = w; \
    }

#define QBLK(o) \
    { \
        TObj *q = GRABBED_OBJECT; \
        switch (q->type) { \
        case 14: \
            { int t = (o->animFrame & 1) * 6 - 4; o->d30 = q->d30 + t; } \
            o->d34 = q->d34 + 6; \
            break; \
        case 66: \
            o->d30 = q->h->p.whole; \
            o->d34 = q->y.p.whole; \
            break; \
        } \
    }

static __inline__ void swing(TObj *o, short i, short a)
{
    playerSpinPlaceAndAccelerate(o, i, a);
    o->anim = D_80011108;
    if (o->animFrame & 1) {
        FUN_8001fe94(o, PLAYER_SPIN_JUMP_ANIMS[i]);
        o->d8c = 0x100 - (i << 2);
    } else {
        FUN_8001fe94(o, PLAYER_SPIN_JUMP_ANIMS[i]);
        o->d8c = i << 2;
    }
}

void playerStepSpinOnObject(TObj *o)
{
    PL *p;
    short a;
    short i;
    short f;

    PLAYER_EXT->b0a = 0xff;
    switch (o->state) {
    case 0:
        o->wb6 = 0;
        {
            signed char c = (*(signed char*)&PLAYER_EXT->step);
            (*(unsigned char*)&o->wac) = 2;
            o->wb2 = c + 1;
        }
        PLAYER_EXT->step = 0;
        PLAYER_EXT->b0a = GRABBED_OBJECT->b0c;
        {
            PL *q = (PL *)PLAYER_EXT;
            q->w2 = 0;
            q->we = 0;
        }
        o->ba4 = 0;
        (*(unsigned char*)&PLAYER_EXT->w08) = 0;
        (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
        {
            PL *q = (PL *)PLAYER_EXT;
            q->wc = 0;
            q->w28 = 0xffff;
            q->w2a = 0xffff;
        }
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        FUN_8001e5f4(4, 0x7f);
        SETTBL(o);
        o->state = 1;
    case 1:
        rot(o);
        if (o->wb6 == 0) {
            if (o->animFrame & 1) {
                if (((PL *)PLAYER_EXT)->we > 0) {
                    PLAYER_EXT->b0a = GRABBED_OBJECT->b0c;
                    if (PAD_PRESSED[0] & 0x80) INC1(o, 9);
                    if (PAD_PRESSED[0] & 0x20) {
                        o->animFrame = 0;
                        GRABBED_OBJECT->state = 0;
                    }
                    SETTBL(o);
                }
            } else {
                if (((PL *)PLAYER_EXT)->we > 0) {
                    PLAYER_EXT->b0a = GRABBED_OBJECT->b0c;
                    if (PAD_PRESSED[0] & 0x20) INC1(o, 9);
                    if (PAD_PRESSED[0] & 0x80) {
                        o->animFrame = 1;
                        GRABBED_OBJECT->state = 0;
                    }
                    SETTBL(o);
                }
            }
        }
        setanim(o, 6);
        FUN_8001fec0(o);
        PLAYER_EXT->visible = 0x20;
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        QBLK(o);
        playerSpinPlaceAndAccelerate(o, i, a);
        if (D_1f8001fc & D_1f8003c6) {
            GV2.bb0 = 0;
            o->b9c = 1;
            (*(unsigned char*)&o->wac) = 0;
            playerSpinAimDir8A(o);
            o->h->p.whole += (o->animFrame & 1) ? 16 : -16;
            (*(signed char*)&o->b0f) = -8;
            o->wb0 = 0;
            PLAYER_EXT->visible = 0;
            o->timer = 6;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb0 = 0;
            o->b9e = 0;
            o->step = 9;
            o->state = 0;
            break;
        }
        if (o->wb2 >= 2) o->state = 2;
        break;
    case 2:
        rot(o);
        if (o->wb6 == 0) {
            if (o->animFrame & 1) {
                if (((PL *)PLAYER_EXT)->we > 0) {
                    SND(0);
                    if (PAD_PRESSED[0] & 0x80) INC2(o, 9);
                    if (PAD_PRESSED[0] & 0x20) DEC(o);
                    SETTBL(o);
                } else {
                    SND(0);
                    if (PAD_PRESSED[0] & 0x80) INC2(o, 3);
                    if (PAD_PRESSED[0] & 0x20) DEC(o);
                    SETTBL(o);
                    ((PL *)PLAYER_EXT)->we = -((PL *)PLAYER_EXT)->we;
                }
            } else {
                if (((PL *)PLAYER_EXT)->we > 0) {
                    SND(0);
                    if (PAD_PRESSED[0] & 0x20) INC1(o, 9);
                    if (PAD_PRESSED[0] & 0x80) DEC(o);
                    SETTBL(o);
                } else {
                    SND(0);
                    if (PAD_PRESSED[0] & 0x20) INC1(o, 3);
                    if (PAD_PRESSED[0] & 0x80) DEC(o);
                    SETTBL(o);
                    ((PL *)PLAYER_EXT)->we = -((PL *)PLAYER_EXT)->we;
                }
            }
        }
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        PLAYER_EXT->animFrame = 0xffff;
        o->anim = D_80011108;
        FUN_8001fe94(o, PLAYER_SPIN_JUMP_ANIMS[i]);
        PLAYER_EXT->visible = 0x20;
        QBLK(o);
        playerSpinPlaceAndAccelerate(o, i, a);
        if (D_1f8001fc & D_1f8003c6) {
            GV2.bb0 = 0;
            playerSpinAimDir8B(o);
            if (o->wb6 > o->d88) (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 1;
            else (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
            o->state = 3;
            break;
        }
        if (o->wb2 >= 4) o->state = 4;
        if (o->wb2 < 2) o->state = 1;
        break;
    case 3:
        rot(o);
        if (o->wb6 == 0) {
            if (o->animFrame & 1) {
                if (((PL *)PLAYER_EXT)->we > 0) {
                    SND(2);
                    (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
                    SETTBL(o);
                } else {
                    SND(2);
                }
            } else {
                if (((PL *)PLAYER_EXT)->we > 0) {
                    SND(2);
                    (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
                    SETTBL(o);
                } else {
                    SND(2);
                }
            }
        }
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        PLAYER_EXT->animFrame = 0xffff;
        o->anim = D_80011108;
        FUN_8001fe94(o, PLAYER_SPIN_JUMP_ANIMS[i]);
        QBLK(o);
        playerSpinPlaceAndAccelerate(o, i, a);
        if ((*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) != 0) break;
        if (o->wb6 > o->d88) {
            (*(unsigned char*)&o->wac) = 0;
            (*(signed char*)&o->b0f) = -8;
            o->wb0 = 0;
            PLAYER_EXT->visible = 0;
            o->timer = 6;
            o->b9e = 4;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb0 = 0;
            o->step = 9;
            o->state = 0;
        }
        break;
    case 4:
        {
            TObj *q = PLAYER_EXT;
            U8(q, 9) = 0;
            q->animFrame = 0xffff;
            PLAYER_EXT->b0a = 0;
        }
        f = 0;
        rot(o);
        if (o->wb6 < -0xffe) {
            SETTBL(o);
            f = 1;
        }
        if (o->wb6 > 0xffe) {
            SETTBL(o);
            f = 1;
        }
        if (f) {
            o->wb6 = 0;
            if (o->animFrame & 1) {
                SND(2);
                if (PAD_PRESSED[0] & 0x80) INC2(o, 9);
                if (PAD_PRESSED[0] & 0x20) DEC(o);
            } else {
                SND(2);
                if (PAD_PRESSED[0] & 0x20) INC1(o, 9);
                if (PAD_PRESSED[0] & 0x80) DEC(o);
            }
            if ((D_1f8001fc & D_1f8003c6) && o->wb2 < 4) o->wb2 = 4;
            SETTBL(o);
        }
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        QBLK(o);
        swing(o, i, a);
        if (D_1f8001fc & D_1f8003c6) {
            playerSpinAimDir8A(o);
            if (o->wb6 > o->d88) (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 1;
            else (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
            o->state = 5;
            break;
        }
        if (o->wb2 < 4) o->state = 2;
        break;
    case 5:
        p = (PL *)PLAYER_EXT;
        ((TObj *)p)->animFrame = 0xffff;
        f = 0;
        rot(o);
        if (o->wb6 < -0xfff) {
            SETTBL(o);
            f = 1;
        }
        if (o->wb6 > 0xfff) {
            SETTBL(o);
            f = 1;
        }
        if (f) {
            o->wb6 = 0;
            if (((PL *)PLAYER_EXT)->we > 0) {
                SND(2);
                (*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) = 0;
                SETTBL(o);
            } else {
                SND(2);
            }
        }
        a = (short)o->wb6 >> 4;
        i = ((unsigned short)((short)o->wb6 >> 4) >> 2) & 0x3f;
        QBLK(o);
        swing(o, i, a);
        if ((*(unsigned char*)((char*)&PLAYER_EXT->w08 + 1)) != 0) break;
        if (o->wb6 > o->d88) {
            o->wb6 = o->d88;
            playerSpinPlaceAndAccelerate(o, i, o->wb6 >> 4);
            {
                PL *q = (PL *)PLAYER_EXT;
                U8(q, 8) = 0;
                q->we = 0;
                ((PL *)PLAYER_EXT)->w2 = 0;
            }
            (*(unsigned char*)&o->wac) = 0;
            (*(signed char*)&o->b0f) = -8;
            o->wb0 = 0;
            PLAYER_EXT->visible = 0;
            o->timer = 6;
            o->b9e = 4;
            o->d30 = 0;
            o->d34 = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb0 = 0;
            o->step = 9;
            o->state = 0;
        }
        break;
    }
}

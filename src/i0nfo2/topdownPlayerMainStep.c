// FUNC 800eaea8 1056 I0NFO2
// MATCHING 800eaea8 1056
#include "TOBJ.H"
extern void func_8001FEC0(TObj *o);
extern void topdownPlayerStatusAnim(TObj *o);
extern void topdownPlayerSetMoveAnim(TObj *o);
extern void topdownPlayerWalkStep(TObj *o);
extern void topdownPlayerItemUseStep(TObj *o);
typedef struct { char p[0x2C]; unsigned short w2c; } P330;
extern P330 *D_8009C330;
extern unsigned char D_8009C93A;
extern unsigned char D_8009C975;
extern unsigned char TALK_POSE;
extern unsigned char D_8009D2B0[];
extern volatile unsigned short PAD_PRESSED[];
extern void topdownCheckTileTrigger(TObj *o, short x, short y);
extern short (*D_800FEC9C[])[4];
#include "gamestate.h"
extern TObj PLAYER;

extern void topdownPlayerReadDpadDir(TObj *o);

void topdownPlayerMainStep(TObj *o)
{
    short i;
    short *e;
    short (*tb)[4];
    short *py;
    short px;
    unsigned short d;
    short l;
    int k = 0;
    *((unsigned char *)o + 0xE2) = 0;
    switch (o->step) {
    case 0:
        D_8009D2B0[0] = 1;
        topdownPlayerReadDpadDir(o);
        if (D_8009C93A != 0) {
            *(unsigned char *)&o->wac = 0;
            switch (o->state) {
            case 0:
                o->state++;
            case 1:
                if (TALK_POSE != 0) {
                    topdownPlayerStatusAnim(o);
                } else {
                    topdownPlayerSetMoveAnim(o);
                    func_8001FEC0(o);
                }
                if (!(o->animFrame & 8)) {
                    o->step = 1;
                    o->state = 0;
                }
                break;
            }
            if (*(unsigned char *)&o->wac >= 2) {
                *(unsigned char *)&o->wac = 0;
                o->step = 2;
                o->state = 0;
                break;
            }
            if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C4) {
                *(unsigned char *)&o->wac = 1;
                o->animFrame |= 8;
                break;
            }
        }
        break;
    case 1:
        D_8009D2B0[0] = 1;
        topdownPlayerReadDpadDir(o);
        topdownPlayerWalkStep(o);
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->animFrame |= 8;
            topdownPlayerSetMoveAnim(o);
            o->state++;
            break;
        case 1:
            if (TALK_POSE != 0) {
                topdownPlayerStatusAnim(o);
            } else {
                topdownPlayerSetMoveAnim(o);
                func_8001FEC0(o);
            }
            break;
        }
        break;
    case 0x30:
        topdownPlayerItemUseStep(o);
        break;
    case 0x63:
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            D_8009C330->w2c = o->animFrame |= 8;
            topdownPlayerSetMoveAnim(o);
            o->state++;
        case 1:
            if (TALK_POSE != 0) {
                topdownPlayerStatusAnim(o);
            } else {
                topdownPlayerSetMoveAnim(o);
            }
            break;
        }
        if (D_8009C975 == 0) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            D_8009C93A = 1;
        }
        break;
    }
    topdownCheckTileTrigger(o, o->h->p.whole, o->y.p.whole);
    if (o->b68 != 0) {
        switch (GAME.selectedArea) {
        case 5: k = 0; break;
        case 0xB: k = 1; break;
        case 0x10: k = 2; break;
        case 0x11: k = 3; break;
        case 0x13: k = 4; break;
        }
        i = 0;
        e = D_800FEC9C[k][0];
        if (e[0] != 0xFF) {
            py = &PLAYER.y.p.whole;
            px = PLAYER.h->p.whole;
            tb = (void *)e;
        loop:
            d = px - e[0];
            l = e[2] - e[0];
            if ((unsigned short)d <= (short)l
                && (d = *py - e[1], l = e[3] - e[1], (unsigned short)d <= (short)l)) {
                goto found;
            }
            i++;
            e = tb[i];
            if (e[0] == 0xFF) return;
            goto loop;
        }
        return;
    found:
        o->b6b = i + 1;
        return;
    } else {
        o->b6b = 0;
    }
}

// FUNC 800ea644 1344 I0NFO3
// MATCHING 800ea644 1344
#include "TOBJ.H"
extern void func_8001FEC0(TObj *o);
extern void tunnelPlayerStatusAnim(TObj *o);
extern void tunnelPlayerSetMoveAnim(TObj *o);
extern void tunnelPlayerWalkStep(TObj *o);
extern void tunnelPlayerItemUseStep(TObj *o);
typedef struct { char p[0x2C]; unsigned short w2c; } P330;
extern P330 *PLAYER_EXT;
extern unsigned char D_8009C93A;
extern unsigned char D_8009C975;
extern unsigned char TALK_POSE;
extern unsigned char D_8009D2B0[];
extern volatile unsigned short PAD_PRESSED[];
extern void tunnelCheckTileTrigger(TObj *o, short x, short y);
extern short D_800EECC8[][4];
extern TObj PLAYER;

#define setDir(o) { \
    D_8009D2B0[0] = 1; \
    o->animFrame &= 7; \
    if (PAD_PRESSED[0] & 0x10) { \
        if (PAD_PRESSED[0] & 0x20) o->animFrame = 1; \
        else if (PAD_PRESSED[0] & 0x80) o->animFrame = 3; \
        else o->animFrame = 2; \
    } else if (PAD_PRESSED[0] & 0x40) { \
        if (PAD_PRESSED[0] & 0x20) o->animFrame = 7; \
        else if (PAD_PRESSED[0] & 0x80) o->animFrame = 5; \
        else o->animFrame = 6; \
    } else if (PAD_PRESSED[0] & 0x80) { \
        o->animFrame = 4; \
    } else if (PAD_PRESSED[0] & 0x20) { \
        o->animFrame = 0; \
    } else { \
        o->animFrame |= 8; \
    } \
}

void tunnelPlayerMainStep(TObj *o)
{
    short i;
short *e; int d; unsigned short w;
    *((unsigned char *)o + 0xE2) = 0;
    switch (o->step) {
    case 0:
        setDir(o);
        if (D_8009C93A != 0) {
            *(unsigned char *)&o->wac = 0;
            switch (o->state) {
            case 0:
                o->state++;
            case 1:
                if (TALK_POSE != 0) {
                    tunnelPlayerStatusAnim(o);
                } else {
                    tunnelPlayerSetMoveAnim(o);
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
        setDir(o);
        tunnelPlayerWalkStep(o);
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->animFrame |= 8;
            tunnelPlayerSetMoveAnim(o);
            o->state++;
            break;
        case 1:
            if (TALK_POSE != 0) {
                tunnelPlayerStatusAnim(o);
            } else {
                tunnelPlayerSetMoveAnim(o);
                func_8001FEC0(o);
            }
            break;
        }
        break;
    case 0x30:
        tunnelPlayerItemUseStep(o);
        break;
    case 0x63:
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            PLAYER_EXT->w2c = o->animFrame |= 8;
            tunnelPlayerSetMoveAnim(o);
            o->state++;
        case 1:
            if (TALK_POSE != 0) {
                tunnelPlayerStatusAnim(o);
            } else {
                tunnelPlayerSetMoveAnim(o);
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
    tunnelCheckTileTrigger(o, o->h->p.whole, o->y.p.whole);
    if (o->b68 != 0) {
        i = 0;
        e = D_800EECC8[0];
        if (e[0] != 0xFF) do {
            if ((d = (unsigned short)PLAYER.h->p.whole - (unsigned short)e[0], w = e[2] - e[0], (unsigned short)d <= (short)w)
                && (d = (unsigned short)PLAYER.y.p.whole - (unsigned short)e[1], w = e[3] - e[1], (unsigned short)d <= (short)w)) {
                o->b6b = i + 1;
                return;
            }
            e = D_800EECC8[++i];
        } while (e[0] != 0xFF);
    } else {
        o->b6b = 0;
    }
}

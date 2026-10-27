// FUNC 800e9870 1128 I0NFO
// MATCHING 800e9870 1128
#include "TOBJ.H"
extern void func_8001FEC0(TObj *o);
extern void overheadPlayerStatusAnim(TObj *o);
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void overheadPlayerWalkStep(TObj *o);
extern void overheadPlayerItemUseStep(TObj *o);
typedef struct { char p[0x2C]; unsigned short w2c; } P330;
extern P330 *PLAYER_EXT;
extern unsigned char D_8009C93A;
extern unsigned char GAME_FADE_SCREEN_CONTROL;
extern unsigned char TALK_POSE;
extern unsigned char GAME_PLAYER_MODE[];
extern volatile unsigned short PAD_PRESSED[];

static __inline__ void setDir(TObj *o)
{
    GAME_PLAYER_MODE[0] = 1;
    o->animFrame &= 7;
    if (PAD_PRESSED[0] & 0x10) {
        if (PAD_PRESSED[0] & 0x20) o->animFrame = 1;
        else if (PAD_PRESSED[0] & 0x80) o->animFrame = 3;
        else o->animFrame = 2;
    } else if (PAD_PRESSED[0] & 0x40) {
        if (PAD_PRESSED[0] & 0x20) o->animFrame = 7;
        else if (PAD_PRESSED[0] & 0x80) o->animFrame = 5;
        else o->animFrame = 6;
    } else if (PAD_PRESSED[0] & 0x80) {
        o->animFrame = 4;
    } else if (PAD_PRESSED[0] & 0x20) {
        o->animFrame = 0;
    } else {
        o->animFrame |= 8;
    }
}

void overheadPlayerMainStep(TObj *o)
{
    *((unsigned char *)o + 0xE2) = 0;
    switch (o->step) {
    case 0:
        setDir(o);
        if (D_8009C93A != 0) {
            switch (o->state) {
            case 0:
                o->state++;
            case 1:
                if (TALK_POSE != 0) {
                    overheadPlayerStatusAnim(o);
                } else {
                    overheadPlayerSetMoveAnim(o);
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
                return;
            }
            if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C4) {
                *(unsigned char *)&o->wac = 1;
                o->animFrame |= 8;
                return;
            }
            *(unsigned char *)&o->wac = 0;
        }
        break;
    case 1:
        setDir(o);
        overheadPlayerWalkStep(o);
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->state++;
            break;
        case 1:
            if (TALK_POSE != 0) {
                overheadPlayerStatusAnim(o);
            } else {
                overheadPlayerSetMoveAnim(o);
                func_8001FEC0(o);
            }
            break;
        }
        break;
    case 0x30:
        overheadPlayerItemUseStep(o);
        break;
    case 0x63:
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            PLAYER_EXT->w2c = o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->state++;
        case 1:
            if (TALK_POSE != 0) {
                overheadPlayerStatusAnim(o);
            } else {
                overheadPlayerSetMoveAnim(o);
            }
            break;
        }
        if (GAME_FADE_SCREEN_CONTROL == 0) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            D_8009C93A = 1;
        }
        break;
    }
}

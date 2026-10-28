// FUNC 800f80d8 1992 I0NFO2
// MATCHING 800f80d8 1992
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char c[12]; } B12;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned char D_800A60E4;
extern TObj *D_8009F2E0;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDD3;
extern unsigned char D_8009CEEB;
extern unsigned char D_8009D00C[];
extern unsigned char D_8009D0DB;
extern unsigned char D_8009D2C3;
extern void topdownActorSetAnim(TObj *o, int a, int b);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void addItemToInventory(int, int, int);
extern void removeItemFromInventory(int, int);
extern void AnimAdvance(TObj *o);

#define VX(v) (*(short *)&(v).c[2])
#define VY(v) (*(short *)&(v).c[6])

void npcSet07SceneStep2(TObj *o)
{
    B12 v;
    TObj *t = D_8009F2E0;
    unsigned char k;

    switch (o->state) {
    case 0:
        t->step = 2;
        t->state = 0;
        PLAYER_SUBSTATE = 2;
        PLAYER_STEP = 0;
        D_8009C93F = 1;
        D_8009C93E = 1;
        D_8009C942 = 1;
        o->w08 = 0x3C;
        o->state++;
        if (D_8009D00C[0] >= 2) {
            v = *(B12 *)&t->a;
            VX(v) += 0x10;
            t->animFrame = t->b69 - 1;
            t->d90 = (int)showMessageBoxDirect(2, 0x30, &v);
            topdownActorSetAnim(t, 0xB, 0);
            o->state = 0xF;
        } else if (D_8009CDD3 != 0) {
            t->animFrame = t->b69 - 1;
            o->state = 5;
        } else if (D_8009D2C3 & 0x10) {
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            S16(t, 0x2E) = 3;
            t->d90 = (int)showMessageBoxDirect(2, 0x2C, &v);
            topdownActorSetAnim(t, 0xB, 0);
            o->state = 4;
        } else {
            topdownActorSetAnim(t, 0x10, 0);
        }
        break;
    case 1:
        AnimAdvance(o);
        if (--o->w08 <= 0) {
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            t->d90 = (int)showMessageBoxDirect(2, 0x2A, &v);
            topdownActorSetAnim(t, 0xB, 0);
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            S16(t, 0x2E) = 3;
            t->d90 = (int)showMessageBoxDirect(2, 0x2B, &v);
            topdownActorSetAnim(t, 0xB, 0);
            o->state++;
        }
        break;
    case 3:
        AnimAdvance(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            t->d90 = (int)showMessageBoxDirect(2, 0x2C, &v);
            S16(t, 0x2E) = 3;
            topdownActorSetAnim(t, 0xB, 0);
            o->state++;
        }
        break;
    case 4:
        AnimAdvance(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            setEventStarted(0x2F, 0, 0);
            o->w08 = 0x14A;
            o->state = 0xA;
        }
        break;
    case 5:
        v = *(B12 *)&t->a;
        VX(v) -= 0x50;
        VY(v) += 0xA0;
        t->d90 = (int)showMessageBoxDirect(2, 0x2D, &v);
        k = t->b69;
        t->animFrame = k - 1;
        topdownActorSetAnim(t, k + 7, 0);
        o->state++;
        break;
    case 6:
        AnimAdvance(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            switch (D_8009D00C[0]) {
            case 0:
                if (D_8009D0DB >= 10) {
                    if (D_8009D0DB >= 15) {
                        removeItemFromInventory(0x37, 15);
                        t->d90 = (int)showMessageBoxDirect(2, 0x2F, &v);
                        o->state = 8;
                    } else {
                        removeItemFromInventory(0x37, 10);
                        t->d90 = (int)showMessageBoxDirect(2, 0x2E, &v);
                        o->state = 7;
                    }
                } else {
                    o->state = 9;
                }
                break;
            case 1:
                if (D_8009D0DB >= 5) {
                    removeItemFromInventory(0x37, 5);
                    t->d90 = (int)showMessageBoxDirect(2, 0x2F, &v);
                    o->state = 8;
                } else {
                    o->state = 9;
                }
                break;
            case 2:
            case 3:
                o->state = 9;
                break;
            }
        }
        break;
    case 7:
        AnimAdvance(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            addItemToInventory(0x1F, 1, 1);
            addItemToInventory(0x1F, 1, 1);
            D_8009D00C[0] |= 1;
            if (D_8009CDD3 != 0xFF) {
                setEventComplete(0x2F, 0);
                o->w08 = 0x14A;
                o->state = 0xA;
            } else {
                o->state = 9;
            }
        }
        break;
    case 8:
        AnimAdvance(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            addItemToInventory(0x5E, 1, 1);
            D_8009D00C[0] |= 2;
            if (D_8009CDD3 != 0xFF) {
                setEventComplete(0x2F, 0);
                o->w08 = 0x14A;
                o->state = 0xA;
            } else {
                o->state = 9;
            }
        }
        break;
    case 9:
        t->b68 = 0;
        topdownActorSetAnim(t, 3, 0);
        t->step = 0;
        t->state = 0;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_8009CEEB = 0;
        D_800A60E4 = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->step = 0;
        o->state = 0;
        break;
    case 10:
        if (--o->w08 == -1) {
            o->state = 9;
        }
        break;
    case 15:
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            o->state = 9;
        }
        break;
    }
}

// FUNC 800fa84c 1820 I0NFO2
// MATCHING 800fa84c 1820
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char c[12]; } B12;
extern TObj D_800A6038;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern B12 D_800A6048;
extern short D_800A604E;
extern short D_800A6066;
extern Fix16 *D_800A6078;
extern TObj *D_800A60C8;
extern TObj *D_8009F2D8;
extern TObj *D_8009F2E0;
extern TObj *D_8009F2E4;
extern unsigned char D_8009C93A;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDD6;
extern unsigned char D_8009CEF4;
extern unsigned char D_8009CF00;
extern unsigned char D_8009CF01;
extern unsigned char D_8009CF02[];
extern unsigned char D_8009CF04;
extern unsigned char D_8009CFC9;
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventComplete(int, int);
extern void removeItemFromInventory(int, int);
extern void AnimAdvance(TObj *o);

void npcSet0CMessageScene(TObj *o)
{
    B12 v;
    TObj *a = D_8009F2D8;
    TObj *b = D_8009F2E0;
    TObj *c = D_8009F2E4;

    switch (o->state) {
    case 0:
        if (D_8009C93A != 0) {
            if (D_8009CDD6 == 0xFF) {
                o->state = 3;
            } else {
                D_800A603D = 4;
                D_800A6066 = 6;
                D_800A603E = 0;
                D_8009C93F = 1;
                D_8009C942 = 1;
                o->w08 = 1;
                o->state++;
            }
        }
        break;
    case 1:
        if (--o->w08 <= 0) {
            a->animFrame = 1;
            v = D_800A6048;
            D_800A60C8 = showMessageBoxDirect(2, 0, &v);
            o->state++;
        }
        break;
    case 2:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            setEventComplete(0x32, 0);
            o->w08 = 2;
            a->b68 = 0;
            a->b6a = 0;
            a->step = 0;
            a->state = 0;
            b->b68 = 0;
            b->b6a = 0;
            b->step = 0;
            b->state = 0;
            c->b68 = 0;
            c->b6a = 0;
            c->step = 0;
            c->state = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            o->state++;
        }
        break;
    case 3:
        if (a->b68 != 0) {
            v = D_800A6048;
            D_800A603D = 4;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A603E = 0;
            if (D_8009CF00 != 0 && D_8009CF04 != 0) {
                D_800A60C8 = showMessageBoxDirect(2, 2, &v);
                o->state = 10;
            } else {
                D_800A60C8 = showMessageBoxDirect(2, 1, &v);
                o->state = 6;
            }
        } else if (b->b68 != 0) {
            v = D_800A6048;
            D_800A603D = 4;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A603E = 0;
            if (D_8009CF00 != 0 && D_8009CF04 != 0) {
                D_800A60C8 = showMessageBoxDirect(0, 0xCD, &v);
            } else if (D_8009CF01 != 0) {
                D_800A60C8 = showMessageBoxDirect(0, 0xCA, &v);
                removeItemFromInventory(0x40, 1);
                if (D_8009CF00 == 0) {
                    D_8009CF00++;
                }
            } else {
                D_800A60C8 = showMessageBoxDirect(0, 0xC9, &v);
            }
            o->state = 4;
        } else if (c->b68 != 0) {
            v = D_800A6048;
            D_800A603D = 4;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A603E = 0;
            if (D_8009CF00 != 0 && D_8009CF04 != 0) {
                D_800A60C8 = showMessageBoxDirect(0, 0xCD, &v);
            } else if (D_8009CF02[0] != 0) {
                D_800A60C8 = showMessageBoxDirect(0, 0xCC, &v);
                if (D_8009CF04 == 0) {
                    D_8009CF04++;
                }
            } else {
                D_800A60C8 = showMessageBoxDirect(0, 0xCB, &v);
                D_8009CF02[0]++;
            }
            o->state = 5;
        }
        break;
    case 4:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            b->b68 = 0;
            o->w08 = 2;
            o->state = 9;
            b->b68 = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            o->w08 = 2;
            o->state = 3;
        }
        break;
    case 5:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            o->w08 = 2;
            c->b68 = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            o->w08 = 2;
            o->state = 3;
        }
        break;
    case 6:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            a->b68 = 0;
            o->state = 9;
        }
        break;
    case 9:
        a->b68 = 0;
        b->b68 = 0;
        c->b68 = 0;
        D_800A603C = 1;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A603D = 0;
        o->state = 3;
        break;
    case 10:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            S16(PTR(a, 0x40), 2) = S16(D_800A6078, 2);
            a->y.p.whole = D_800A604E - 4;
            o->w08 = 0x3C;
            o->state++;
        }
        break;
    case 11:
        if (--o->w08 <= 0) {
            D_8009CEF4 = 0;
            D_8009CFC9 = 1;
            o->state++;
        }
        break;
    case 12:
        AnimAdvance(&D_800A6038);
        D_800A6038.y.p.whole += 2;
        S16(PTR(a, 0x40), 2) = S16(D_800A6078, 2);
        a->y.p.whole = D_800A604E - 4;
        break;
    }
}

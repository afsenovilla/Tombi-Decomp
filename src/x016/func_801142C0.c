// FUNC 801142c0 924 X016
// MATCHING 801142c0 924
#include "TOBJ.H"

typedef struct { short s[6]; } P6;
extern unsigned short D_8009C960;
extern int D_8009C960_w;
extern unsigned char D_8009CDB1;
extern unsigned char *D_8009C330;
extern unsigned char D_800A603C[];
extern unsigned char D_800A60A1;
extern short D_800A6066, D_800A60B4;
extern void *D_8009F2D8[];
int FUN_8002dcf0(int a, int b, P6 *p, int c);
void TileCollideAt(TObj *o, short x, short y);
void AnimAdvance(TObj *o);
void func_8004D620(int a, int b);

void func_801142C0(TObj *o)
{
    P6 pos;
    unsigned short *p;
    switch (o->state) {
    case 0:
        o->timer = 60;
        o->state++;
    case 1:
        if (--o->timer > 0) break;
        o->timer = 0;
        pos = *(P6 *)&o->a;
        p = &D_8009C960;
        if (*p == 2) pos.s[1] -= 0x50;
        pos.s[3] -= 0x20;
        o->b68 = 0;
        o->b6a = 1;
        if (*p == 2) {
            switch (D_8009CDB1) {
            case 1: o->d90 = FUN_8002dcf0(2, 0, &pos, 0x78); break;
            case 2: o->d90 = FUN_8002dcf0(2, 1, &pos, 0x78); break;
            case 3: o->d90 = FUN_8002dcf0(2, 1, &pos, 0x78); break;
            case 4: o->d90 = FUN_8002dcf0(2, 2, &pos, 0x78); break;
            }
        } else {
            switch (D_8009CDB1) {
            case 1: o->d90 = FUN_8002dcf0(2, 0, &pos, 0x78); break;
            case 2: o->d90 = FUN_8002dcf0(2, 1, &pos, 0x78); break;
            case 3: o->d90 = FUN_8002dcf0(2, 1, &pos, 0x78); break;
            case 4: o->d90 = FUN_8002dcf0(2, 2, &pos, 0x78); break;
            }
        }
        D_800A603C[0] = 6;
        D_800A603C[1] = 5;
        D_800A603C[2] = 0;
        D_8009C330[9] = 10;
        if (D_8009C960_w == 0x10002) {
            D_800A6066 = 1;
            D_800A60B4 = 0xc0;
        } else {
            D_800A6066 = 0;
            D_800A60B4 = -0xc0;
        }
        D_800A60A1 = 0;
        o->state++;
        break;
    case 2:
        if (D_8009C960 == 1) o->h->p.whole += 4;
        else if (D_8009C960 == 2) o->h->p.whole -= 4;
        o->y.raw += 0x80000;
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + o->box3 - o->box2);
        AnimAdvance(o);
        if (D_800A60A1) {
            if (D_8009C960 == 2) {
                if (D_8009CDB1 == 4) func_8004D620(8, 3);
            } else if (D_8009CDB1 != 0xff) {
                func_8004D620(D_8009CDB1 + 7, 2);
            }
            o->b6a = 0;
            D_8009F2D8[o->b0c] = 0;
            D_800A603C[0] = 1;
            D_800A603C[1] = 0;
            D_800A603C[2] = 0;
            o->b04 = 3;
        }
        break;
    }
}

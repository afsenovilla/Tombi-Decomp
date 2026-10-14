// FUNC 8013aab8 1112 X001
// MATCHING 8013aab8 1112
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    short w08;
    char p0a[0x1c - 0xa];
    TObj *p1c;
    char p20[0x24 - 0x20];
    short w24;
} O;
extern unsigned char PLAYER, D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A6066;
extern unsigned char D_800A60A1;
extern unsigned short D_800A604E;
extern unsigned char D_800A60D6, D_800A60E2, D_800A60DA, D_800A60F8;
extern TObj *D_800A60C8;
extern TObj *showMessageBox(int, int, int, int);
extern void FUN_80026c50(int, int, int);
extern void setEventComplete(int, int);

void raceResult(O *o)
{
    TObj *p = o->p1c;
    TObj *t;

    switch (o->step) {
    case 0:
        ((unsigned char*)&GCD.bcedf)[0] = 0;
        o->step++;
        break;
    case 1:
        if (D_800A603C[0] != 1) break;
        if (!D_800A60A1) break;
        if ((unsigned short)(D_800A604E + 0x6d6) >= 6) break;
        if (((unsigned char*)&GCD.bcedf)[0] == 0) {
            PLAYER = 1;
            D_800A6066 = 1;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            D_800A60D6 = 0;
            D_800A60E2 = 0;
            D_800A60DA = 0;
            D_800A60F8 = 0;
            if (p->step != 5) {
                p->step = 5;
                p->state = 0;
            }
            ((unsigned char*)&GCD.bcedf)[0] = 1;
            o->step++;
        } else if (((unsigned char*)&GCD.bcedf)[0] == 0xfe) {
            PLAYER = 1;
            D_800A6066 = 1;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            D_800A60D6 = 0;
            D_800A60E2 = 0;
            D_800A60DA = 0;
            D_800A60F8 = 0;
            if (p->step != 5) {
                p->step = 5;
                p->state = 0;
            }
            o->step++;
        }
        break;
    case 2:
        switch (((unsigned char*)&GCD.bcedf)[0]) {
        case 0xfe:
            D_800A60C8 = showMessageBox(2, 0x1c, 0x80, 0x6c);
            o->step++;
            break;
        case 0xff:
            D_800A60C8 = showMessageBox(2, 0x1a, 0x80, 0x6c);
            o->step++;
            break;
        }
        break;
    case 3:
        switch (((unsigned char*)&GCD.bcedf)[0]) {
        case 0xfe:
            t = D_800A60C8;
            if (t->b04 != 2) break;
            t->b04 = 3;
            D_800A60C8 = showMessageBox(2, 0x1d, 0x80, 0x6c);
            o->step++;
            break;
        case 0xff:
            t = D_800A60C8;
            if (t->b04 != 2) break;
            t->b04 = 3;
            D_800A60C8 = showMessageBox(2, 0x1b, 0x80, 0x6c);
            o->step++;
            break;
        }
        break;
    case 4: {
        TObj *q = D_800A60C8;
        if (q->b04 != 2) break;
        q->b04 = 3;
        p->subtype = 0;
        switch (((unsigned char*)&GCD.bcedf)[0]) {
        case 0xfe:
            p->step = 1;
            p->state = 0;
            o->w08 = 4;
            o->step = 6;
            break;
        case 0xff:
            p->animFrame = 0;
            p->step = 3;
            p->state = 0;
            FUN_80026c50(0x66, 1, 1);
            setEventComplete(0xb5, 0);
            o->w08 = 0xb4;
            o->step = 5;
            break;
        }
        break;
    }
    case 5:
        if (--o->w08 > 0) break;
        o->w08 = 0x78;
        o->step++;
        break;
    case 6:
        if (--o->w08 > 0) break;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        D_800A60F8 = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->w24 = 0;
        switch (((unsigned char*)&GCD.bcedf)[0]) {
        case 0xfe:
            GCD.bcede = 0;
            ((unsigned char*)&GCD.bcedf)[0] = 0;
            GCD.bce59 = 2;
            o->step = 0;
            o->state = 0;
            break;
        case 0xff:
            ((unsigned char*)&GCD.bcedf)[0] = 0;
            o->b04 = 3;
            break;
        }
        break;
    }
}

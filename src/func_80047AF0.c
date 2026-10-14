// FUNC 80047af0 900 MAIN0
// MATCHING 80047af0 900
#include "spad.h"
#include "gvars.h"
#include "gamestate.h"
typedef struct {
    unsigned char b0; char p1[3]; unsigned char b4, b5, b6;
    char p7[0x16 - 7]; short s16;
    char p18[0x9e - 0x18]; unsigned char b9e;
} TO;
typedef struct { char p[0x4c]; unsigned short s4c; } C;
extern unsigned short D_8009C960_h;
extern short *D_800A605C;
extern unsigned short D_80115320[];
extern unsigned short D_800A60A4, D_800A60A6, D_800A60A8, D_800A60AA;
extern TO PLAYER;
extern signed char D_8009D2B0;
extern C *TASK_C;
extern short D_800A4582;
extern void func_8011E514(void), FUN_800490ac(void), func_80048E68(void), func_800491F0(void);
extern void func_80046D58(TO *), FUN_8004432c(TO *, unsigned char *), func_80047484(TO *), func_80126770(TO *);
extern void func_80045F00(void), FUN_80045d98(void), func_80046610(void), func_80049524(void);

static __inline__ void collide(TO *p)
{
    unsigned char **q;
    unsigned char *e;
    if (GVARS.b38 == 0 && D_8009D2B0 != 3) {
        q = (*(unsigned char ***)&SPAD.p228);
        for (SPAD.s19e = SPAD.s248; SPAD.s19e != 0;) {
            e = *q;
            SPAD.s19e--;
            q++;
            if (*e & 3) FUN_8004432c(p, e);
        }
    }
}

void func_80047AF0(void)
{
    unsigned short *t;
    TO *p;

    if (SPAD.b1a5 != 0) return;
    if ((*(int*)&GAME.selectedArea) == 6) {
        func_8011E514();
        return;
    }
    t = &D_80115320[D_800A605C[1] * 4];
    D_800A60A4 = *t++;
    D_800A60A6 = *t++;
    D_800A60A8 = *t++;
    D_800A60AA = *t++;
    FUN_800490ac();
    func_80048E68();
    func_800491F0();
    p = &PLAYER;
    switch (p->b0) {
    case 0: case 5:
    default:
        goto dflt;
    case 1: case 2: case 3: case 7:
        switch (p->b9e) {
        case 0: case 5: case 6:
            func_80046D58(p);
        }
        collide(p);
        if (GVARS.b42 | GVARS.b3f) goto tail;
        func_80047484(p);
        break;
    case 4: case 6:
        collide(p);
        if (GVARS.b42 | GVARS.b3f) goto tail;
        break;
    }
    if (SPAD.b1a4 == 0 && TASK_C->s4c == 1 && D_8009D2B0 != 3 && D_800A4582 + 0xa0 < p->s16) {
        p->b0 = 5;
        p->b4 = 2;
        p->b5 = 3;
        p->b6 = 0;
    }
dflt:
    if (!(GVARS.b42 | GVARS.b3f) && D_8009C960_h == 0 && GAME.selectedSection == 2)
        func_80126770(p);
tail:
    func_80045F00();
    FUN_80045d98();
    func_80046610();
    func_80049524();
}

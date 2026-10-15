// FUNC 800e9c7c 520 I0NFO2
// MATCHING 800e9c7c 520
#include "TOBJ.H"
extern int rand(void);
extern void func_8001FE6C(TObj *o);
extern int func_8001FEC0(TObj *o);
extern unsigned char D_8009D00F;
extern TObj *D_8009C330;
extern void *D_800106F0[];
extern void *D_80010700[];
extern void *D_8001070C[];

void func_800E9C7C(TObj *arg)
{
    register TObj *o asm("$17") = arg;
    short d;
    switch (o->animFrame & 7) {
    case 2:
        d = 0;
        break;
    case 6:
        d = 2;
        break;
    case 0: case 1: case 3: case 4: case 5: case 7:
        d = 1;
        break;
    }
    switch (D_8009D00F) {
    case 1:
    case 5:
        if ((rand() & 0x3F) == 0) {
            D_8009C330->animTimer = 0x5A;
            if (D_8009C330->animFrame != 0x5A) {
                o->anim = D_800106F0[d];
                func_8001FE6C(o);
                D_8009C330->animFrame = D_8009C330->animTimer;
            }
        }
        if (func_8001FEC0(o)) {
            D_8009C330->animFrame = 0xFF;
        }
        break;
    case 2:
        D_8009C330->animTimer = 0x5B;
        if (D_8009C330->animFrame != 0x5B) {
            o->anim = D_8001070C[d];
            func_8001FE6C(o);
            D_8009C330->animFrame = D_8009C330->animTimer;
        }
        func_8001FEC0(o);
        break;
    case 3: {
        TObj *p = D_8009C330;
        p->animTimer = 0x5B;
        if (p->animFrame != 0x5B) {
            o->anim = D_8001070C[d];
            p->animTimer = p->animFrame;
        }
        func_8001FEC0(o);
        break;
    }
    case 4:
        D_8009C330->animTimer = 0x5C;
        if (D_8009C330->animFrame != 0x5C) {
            o->anim = D_80010700[d];
            func_8001FE6C(o);
            D_8009C330->animFrame = D_8009C330->animTimer;
        }
        func_8001FEC0(o);
        break;
    }
}

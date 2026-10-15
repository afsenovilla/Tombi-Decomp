// FUNC 800eaaa0 448 I0NFO2
// MATCHING 800eaaa0 448
#include "TOBJ.H"
extern void func_8001E4F0(int a);
extern void func_8001F110(int a);
extern void func_8001FEC0(TObj *o);
extern void func_800E9E84(TObj *o);
extern TObj *D_8009C330;
extern int D_8009C960[];
extern unsigned char D_8009C975[];
extern int D_8009CD94[];
extern unsigned char D_8009D2B0[];

void func_800EAAA0(TObj *o)
{
    TObj *p;
    switch (o->state) {
    case 0:
        o->active = 4;
        D_8009C330->timer = 0;
        D_8009C330->animFrame = 0xFF;
        func_800E9E84(o);
        o->timer = 0x14;
        o->d8c = 0;
        *(short *)((char *)o + 0xE0) = 0;
        o->w22 = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        D_8009D2B0[0] = 3;
        o->d8c = 0;
        if (D_8009C960[0] != D_8009CD94[0]) {
            func_8001F110(1);
        }
        o->state++;
        break;
    case 1:
        if (o->substep != 0 && !(*(unsigned short *)0x1F8001F8 & 3)) {
            func_8001E4F0(0x16);
        }
        func_8001FEC0(o);
        o->y.raw += o->velY << 8;
        o->velY -= 8;
        if (--o->timer <= 0) {
            D_8009C975[0] = 3;
            o->state++;
        }
        break;
    case 2:
        func_8001FEC0(o);
        o->y.raw += o->velY << 8;
        o->velY -= 8;
        if (D_8009C975[0] == 1) {
            p = *(TObj **)0x1F8001D4;
            p->w4c = 7;
            p->w4e = 0;
        }
        break;
    }
}

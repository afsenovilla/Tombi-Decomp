// FUNC 80119a6c 872 X010
// MATCHING 80119a6c 872
#include "TOBJ.H"

extern unsigned char D_8009D2C3[];
extern void FUN_80020078(TObj *, int);
extern void FUN_80018838(TObj *);
extern void func_801198F8(TObj *);
extern void func_80119648(TObj *);

#define SET(a, b, c, k, v) r[0] = a; r[1] = b; o->d38 = c; o->subtype = k; o->d34 = 0; o->d30 = (o->d38 >> 8) & 0xfff; o->d8c = (v - o->d30) & 0xfff; break;

void func_80119A6C(TObj *o)
{
    int *r = (int *)&o->wb4;

    switch (o->b04) {
    case 0:
        o->box0 = 0x76;
        o->box1 = 0xec;
        o->box2 = 4;
        o->box3 = 8;
        o->b0a = 0x11;
        o->d84 = 0;
        o->d88 = 0;
        o->w7a = 0;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->animFrame = 2;
        o->b69 = 0;
        o->b04++;
        if (D_8009D2C3[0] & 0x40) o->b0c = 1;
        else o->b0c = 0;
        switch (o->subtype) {
        case 3: case 4: case 6: case 11:
            r[0] = 0x40000; r[1] = -0x40000; o->d38 = 0; o->d34 = 0; o->subtype = 0; o->d30 = (o->d38 >> 8) & 0xfff; o->d8c = (0x1000 - o->d30) & 0xfff; break;
        case 0: case 13: case 15:
            SET(0x80000, -0x40000, 0x80000, 1, 0x1800)
        case 1: case 7:
            SET(0xc0000, -0x40000, 0xc0000, 2, 0x1800)
        case 2:
            SET(0x80000, 0x40000, 0x40000, 3, 0x1400)
        case 12:
            SET(0x40000, 0, 0, 3, 0x1400)
        case 5:
            SET(0, -0x40000, 0, 3, 0x1400)
        case 9:
            SET(0xc0000, 0x80000, 0x80000, 3, 0x1400)
        case 8:
            SET(0x40000, 0, 0x40000, 3, 0x1400)
        case 14:
            SET(0, -0x40000, -0x40000, 3, 0x1400)
        case 10:
            SET(0xc0000, 0x80000, 0xc0000, 3, 0x1400)
        }
        break;
    case 1:
        FUN_80020078(o, 0x88);
        if (D_8009D2C3[0] & 0x40) {
            func_801198F8(o);
            break;
        }
        switch (o->step) {
        case 0:
            if (o->b69) {
                if (o->b9d) {
                    o->velH = 0x1200;
                    o->d34 = o->d38 + 0x40000;
                    if (r[0] < o->d34) o->d34 = r[0];
                } else {
                    o->velH = -0x1200;
                    o->d34 = o->d38 - 0x40000;
                    if (o->d34 < r[1]) o->d34 = r[1];
                }
                o->step++;
            }
            break;
        case 1:
            if (o->velH == 0) {
                o->step = 0;
                o->b69 = 0;
            }
            break;
        }
        if (o->velH) func_80119648(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}

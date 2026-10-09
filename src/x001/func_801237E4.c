// FUNC 801237e4 844 X001
// MATCHING 801237e4 844
#include "TOBJ.H"

extern TObj *D_8009C330;
extern char D_80077CF4[];
extern unsigned char D_8009D2B0[];
extern int D_8009C984;
extern unsigned short D_8009D670;
extern unsigned short D_1F8001F8, D_1F8001FC, D_1F8003C4, D_1F8003C6;
extern void func_801072FC(TObj *);
extern void FUN_801073c0(TObj *);
extern void FUN_80110760(TObj *);
extern void FUN_800ee9cc(TObj *);
extern void FUN_800f00ac(TObj *);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_800eae0c(int, int, int, int);
extern void func_8010E97C(TObj *);
extern void FUN_8001fc14(TObj *, int, int);
extern void ObjApplyVelocity(TObj *);
extern void FUN_800ee4e0(TObj *, short);

void func_801237E4(TObj *o)
{
    switch (o->state) {
    case 0:
        func_801072FC(o);
    case 1:
        FUN_801073c0(o);
        FUN_80110760(o);
        FUN_800ee9cc(o);
        if (o->bbe == 0) {
            if (*(unsigned char *)&D_8009C330->w08 == o->animFrame) {
                o->b9c = 1;
                o->d88 = -0x200;
                o->step = 0x1e;
                o->ba4 = 0;
                o->state = 2;
            } else {
                *(unsigned char *)&D_8009C330->w08 = 0;
                o->b9c = 0;
                o->d88 = 0;
                o->ba4 = 0;
                o->step = 1;
                o->state = 0;
            }
        } else if (o->ba6 >= 2) {
            o->b9c = 1;
            o->d88 = -0x100;
            o->ba4 = 0;
            if (o->ba6 & 6) {
                o->wb2 = 0;
                o->velH = 0;
                o->velV = 0;
                o->velX = 0;
                o->velY = 0;
            }
            o->step = 0x1e;
            o->state = 2;
        }
        FUN_800f00ac(o);
        if (D_1F8001FC & D_1F8003C6) {
            D_8009D2B0[0] = 0;
            if (o->wb2 > 0x200) {
                o->wb2 = 0x200;
            }
            if (o->wb2 < -0x200) {
                o->wb2 = -0x200;
            }
            o->ba4 = 0;
            o->b9c = 1;
            if ((D_8009C984 & 0x40) && (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4)) {
                o->ba7 = 1;
            }
            o->step = 2;
            o->state = 0;
        }
        if (D_8009C330->animTimer < 4) {
            o->d8c = (-o->wb0 << 2) & 0xff;
        }
        if (o->ba4) {
            if ((D_1F8001F8 & 3) == 0) {
                FUN_80025f40(0, 0, 0xff, 2);
            }
            if ((D_1F8001F8 & 7) == 0) {
                FUN_800eae0c(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
            }
        }
        break;
    case 2:
        func_8010E97C(o);
        FUN_8001fc14(o, o->wb6, o->wb2);
        ObjApplyVelocity(o);
        if (o->wb2 == 0) {
            *(unsigned char *)&D_8009C330->w08 = 0;
            o->b9c = 0;
            o->d88 = 0;
            o->ba4 = 0;
            o->step = 0;
            o->state = 0;
        }
        if (o->bbe) {
            o->movetab = D_80077CF4;
            o->step = 0x1e;
            *(unsigned char *)&o->wac = 0;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->velX = 0;
            o->velY = 0;
            o->b6b = 0;
            o->state = 1;
        }
        FUN_800f00ac(o);
        break;
    }
    FUN_800ee4e0(o, 1);
}

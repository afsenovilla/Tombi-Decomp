// FUNC 80130990 1196 X003
// MATCHING 80130990 1196
#include "TOBJ.H"
extern TObj D_800A6038;
extern unsigned char D_8009D2C3, D_1F8001A4, D_8009C93A, D_8009C93F;
extern unsigned short D_8009C962;
extern short D_8009C944, D_8009C946;
extern unsigned short D_80135EC0[], D_80135EC8[];
extern int FUN_8001f9e0(void);
extern void FUN_80018980(TObj *);

#define W1C(o) (*(short *)((char *)(o) + 0x1c))
#define U8(o, n) (*(unsigned char *)((char *)(o) + (n)))

void func_80130990(TObj *o)
{
    TObj *pl = &D_800A6038;
    short v;

    switch (o->b04) {
    case 0:
        if (D_8009D2C3 & 2)
            o->b04 = 2;
        else if (D_8009C962 < 2)
            o->b04++;
        else
            o->b04 = 3;
    clr:
        D_8009C944 = 0;
        break;
    case 1:
        if (D_1F8001A4) goto clr;
        switch (o->step) {
        case 0:
            if (D_8009C93A) {
                o->step++;
                D_8009C944 = 0x100;
            }
            break;
        case 1:
            if (D_800A6038.y.p.whole >= -0x153) goto clr;
            if (U8(&D_800A6038, 0xa2) | U8(&D_800A6038, 0xa3)) goto clr;
            if (D_8009C93F) goto clr;
            if (D_800A6038.step == 0x34) goto clr;
            switch (D_800A6038.b9c) {
            case 0:
                D_8009C944 = 0x100;
                break;
            case 1:
                D_8009C944 += 8;
                if (D_8009C944 > 0x200) D_8009C944 = 0x200;
                break;
            case 2:
                D_8009C944 += 0x10;
                if (D_8009C944 > 0x300) D_8009C944 = 0x300;
                break;
            }
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            if (D_8009C962 < 4) {
                o->b04 = 3;
            } else {
                o->w08 = 0;
                o->step++;
            }
            break;
        case 1:
            o->w08 = 0;
            W1C(o) = D_80135EC0[FUN_8001f9e0() & 3];
            o->w1e = D_80135EC8[FUN_8001f9e0() & 3];
            o->step++;
            break;
        case 2:
            o->w08 += W1C(o);
            if (o->w08 < -0x300) o->w08 = -0x300;
            if ((unsigned short)--o->w1e == 0xffff) {
                o->w1e = 0x32;
                o->w08 = 0;
                o->step++;
            }
            break;
        case 3:
            if ((unsigned short)--o->w1e == 0xffff) o->step = 1;
            break;
        }
        D_8009C946 = 0;
        if (pl->b69 == 1) break;
        if (U8(pl, 0xc9))
            v = o->w08 >> 1;
        else
            v = o->w08;
        if ((unsigned short)(pl->a.p.whole - 0x514) < 0xb4) {
            if (pl->b9c && pl->velY > 0x200) pl->velY = 0x200;
            D_8009C946 = v;
        }
        if ((unsigned short)(pl->a.p.whole - 0x1012) < 0x226) {
            if (pl->b9c && pl->velY > 0x200) pl->velY = 0x200;
            D_8009C946 = v;
        }
        if ((unsigned short)(pl->a.p.whole - 0x13a3) < 0x85) {
            if (pl->b9c && pl->velY > 0x200) pl->velY = 0x200;
            D_8009C946 = v;
        }
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}

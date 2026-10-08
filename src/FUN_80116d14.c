// FUNC 80116d14 2192 X000
// MATCHING 80116d14 2192
#include "TOBJ.H"
typedef void (*Fn)(TObj *);
typedef struct { short s0, s2; } S2;
extern unsigned char DAT_8009cda2[];
extern unsigned char DAT_8009c966, DAT_8009cda6, DAT_8009c93a, DAT_8009cdc0, DAT_8009ce0b, DAT_800a60da;
extern unsigned char DAT_8009cfed, DAT_8009c938, DAT_8009cda8, DAT_8009c93f, DAT_8009c975, DAT_8009c93c;
extern unsigned char DAT_8009d13f, DAT_8009c93e, DAT_8009c942;
extern signed char DAT_8009d2b0;
extern unsigned short DAT_8009c962;
extern unsigned short DAT_8009c982[];
extern unsigned char DAT_801384c0[];
extern Fn DAT_8013841c[];
extern short DAT_1f80016a, DAT_1f80016e, DAT_1f800172;
extern short DAT_1f8001c6;
extern unsigned char DAT_1f8001cc, DAT_1f8001cd;
extern int DAT_800a609c;
extern S2 *DAT_800a6078;
extern char DAT_8001d6a4[];
extern unsigned int FUN_80028420(TObj *);
extern unsigned int FUN_80028b40(TObj *);
extern void FUN_80027c74(TObj *);
extern void FUN_800277f8(TObj *);
extern void FUN_80029078(TObj *);
extern void FUN_8005a9a4(int, int);
extern void FUN_8002715c(int);
extern int FUN_800270a0(TObj *, int, int);
extern void FUN_800216c4(void);
extern void FUN_80029cb4(TObj *);
extern void FUN_8002a4d0(TObj *);
extern void FUN_8001f110(int);
extern void FUN_8001f4bc(void);
extern void FUN_800171b8(int, char *);
extern void FUN_8001eb64(void);

void FUN_80116d14(TObj *o)
{
    unsigned short f, t;

    switch (o->step) {
    case 0:
        f = o->animFrame;
        o->animFrame = 0x4b0;
        t = o->animTimer;
        o->visible = 0;
        ((unsigned char *)o)[0x70] = 0;
        ((unsigned char *)o)[0x71] = 0;
        ((unsigned char *)o)[0x72] = 0;
        ((unsigned char *)o)[0x73] = 0;
        ((unsigned short *)&o->h)[1] = f;
        ((unsigned short *)&o->h)[0] = t;
        o->step++;
    case 1:
        if (DAT_8009cda2[0] != 0) {
            if (FUN_80028420(o) & FUN_80028b40(o)) DAT_8009cda2[0] = 0;
            break;
        }
        o->subtype = 1;
        o->step++;
        if (DAT_8009c966 == 3 || DAT_8009cda6 == 0xff) {
            o->subtype = 0;
            o->animFrame = ((unsigned short *)&o->h)[1];
            break;
        }
        if (DAT_8009c966 == 1) DAT_8009c966 = 2;
        break;
    case 2:
        if (DAT_8009c93a != 0) o->step++;
        FUN_80027c74(o);
        *(unsigned char *)&o->d3c = 0;
        FUN_800277f8(o);
        FUN_80029078(o);
        break;
    case 3:
        {
            unsigned char v = DAT_801384c0[DAT_8009c962];
            o->state = 0;
            o->step = v;
        }
        break;
    case 4:
        DAT_8013841c[o->subtype](o);
        switch (DAT_8009c982[0]) {
        case 0:
        case 1:
            if (DAT_8009ce0b != 0 && DAT_800a60da == 2 && (unsigned)(*(unsigned short *)&DAT_1f80016a - 0x400) < 0x18) {
                FUN_8002715c(0x40c);
                if (DAT_1f800172 >= 0x2e) {
                    if (!FUN_800270a0(o, 2, 2)) break;
                    DAT_800a609c = 1;
                    o->step = 8;
                    break;
                }
            }
            if (DAT_1f80016a >= 0x4a2 && DAT_1f80016e >= -0x3c) {
                if (!FUN_800270a0(o, 2, 0)) break;
                DAT_800a609c = 1;
                goto s8;
            }
            if (o->subtype == 1) break;
            if (DAT_1f80016a < 0x4ed) break;
            if (DAT_1f80016e >= -0x95) break;
            DAT_8009c962 = 1;
            FUN_800216c4();
            o->state = 0;
            o->step++;
            break;
        case 2:
            if (DAT_8009cdc0 == 1) FUN_8005a9a4(0x1c, 0);
            DAT_8009c982[0] = 0;
            break;
        default:
            DAT_8009c982[0] = 0;
            break;
        }
        break;
    case 5:
        DAT_8009c982[0] = 0;
        DAT_8013841c[o->subtype](o);
        if (DAT_1f80016a < 0x4ec) {
            o->state = 0;
            o->step--;
            DAT_8009c962 = 0;
            FUN_800216c4();
            break;
        }
        if (DAT_1f80016a >= 0xa46 && DAT_1f80016e >= -0x122) {
            if (FUN_800270a0(o, 2, 0)) o->step = 8;
            break;
        }
        if (DAT_1f80016a >= 0x9dd && DAT_1f80016e < -0x18f) {
            DAT_8009c962 = 2;
            o->step++;
            break;
        }
        {
        int k = DAT_8009cfed;
        if (k != 1) break;
        if (DAT_1f80016a < 0x938) break;
        if (DAT_1f80016e < -0x64) break;
        if (FUN_800270a0(o, 2, 1)) {
            DAT_800a609c = k;
            o->step = 8;
        }
        }
        break;
    case 6:
        FUN_80029cb4(o);
        if (DAT_1f80016e < -0x2ed) {
            DAT_8009c982[0] = 2;
            o->step++;
            break;
        }
        if (DAT_1f80016a >= 0x9dc) break;
        DAT_8009c962 = 1;
        FUN_800216c4();
        o->step--;
        break;
    case 7:
        FUN_80029cb4(o);
        if (DAT_1f80016e < -0x559 && DAT_1f800172 == 0x1c2 && DAT_1f80016a < 0x88c) {
            if (!FUN_800270a0(o, 2, 1)) break;
            o->step = 8;
            o->state = 0;
            break;
        }
        if (DAT_1f80016e < -0x2ed) break;
        DAT_8009c982[0] = 0;
        o->step--;
        break;
    case 8:
        FUN_8002a4d0(o);
        break;
    case 9:
        DAT_8013841c[o->subtype](o);
        if (DAT_8009c938 != 0) break;
        {
        short k = DAT_1f80016a;
        unsigned short u = k;
        if (k < 0x78) {
            if (FUN_800270a0(o, 2, 0)) o->step = 8;
            break;
        }
        if ((unsigned short)(u - 0xf0) < 0x3c) {
            if (DAT_1f80016e < -0x40) break;
            if (DAT_8009cda8 != 0) break;
            DAT_8009c93f = 1;
            FUN_8001f110(1);
            DAT_8009c975 = 3;
            DAT_8009c93c = 0;
            o->step = 0xb;
            break;
        }
        if (k < 400) break;
        if (FUN_800270a0(o, 2, 1)) o->step = 8;
        }
        break;
    case 10:
        DAT_8013841c[o->subtype](o);
        if (DAT_1f80016a < 0x29 && DAT_1f80016e >= -0x126) {
            if (!FUN_800270a0(o, 2, 0)) break;
        s8:
            o->step = 8;
            o->state = 0;
            break;
        }
        if (DAT_8009d13f != 0) {
            if (DAT_1f80016a < 0x374) break;
            if (DAT_1f80016e < -0x167) break;
            if (FUN_800270a0(o, 2, 1)) o->step = 8;
            break;
        }
        if (DAT_8009d2b0 == 3) break;
        if (DAT_1f80016a < 0x373) break;
        DAT_800a6078->s2 = 0x373;
        break;
    case 11:
        if (DAT_8009c975 != 1) break;
        DAT_8009cda8 = 0xff;
        DAT_1f8001c6 = 2;
        FUN_8001f4bc();
        o->step++;
        DAT_1f8001cc = 1;
        DAT_1f8001cd = 4;
        FUN_800171b8(1, DAT_8001d6a4);
        break;
    case 12:
        if (DAT_1f8001cc != 0) break;
        DAT_1f8001c6 = 0;
        DAT_8009c975 = 4;
        FUN_8001eb64();
        o->step++;
        break;
    case 13:
        if (DAT_8009c975 != 0) break;
        DAT_8009c93f = 0;
        DAT_8009c93e = 0;
        DAT_8009c942 = 0;
        o->step = 9;
        break;
    }
}

// FUNC 80115fb4 640 X013
// MATCHING 80115fb4 640
#include "TOBJ.H"
typedef void (*Fn)(TObj *);

extern Fn D_80079A98[];
extern unsigned char D_8009CDA2[];
extern unsigned char D_8009C93A, D_800A60DA, D_8009C93C, D_8009C975;
extern unsigned char D_800A603C, D_800A6039, D_800A603D, D_800A603E;
extern unsigned short D_8009C982;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern short D_800A60B6;
extern short D_1F800172;
extern int D_1F8000F0;
extern TObj *D_1F8001D4;
extern unsigned int FUN_80028420(TObj *);
extern unsigned int FUN_80028b40(TObj *);
extern void FUN_80027c74(TObj *);
extern void func_8002795C(TObj *);
extern void FUN_800277f8(TObj *);
extern void func_80029078(TObj *);
extern int FUN_8002715c(int);

void func_80115FB4(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CDA2[0] != 0) {
            if (FUN_80028420(o) & FUN_80028b40(o)) D_8009CDA2[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    case 1:
        if (D_8009C93A != 0) o->step++;
        FUN_80027c74(o);
        func_8002795C(o);
        FUN_800277f8(o);
        func_80029078(o);
        D_8009C982 = 0;
        break;
    case 2:
        D_80079A98[o->subtype](o);
        if (D_800A60DA == 2) {
            FUN_8002715c(0xa0);
            if (D_1F800172 >= 0x50) {
                D_800A603C = 5;
                D_800A6039 = 0;
                D_800A603D = 0x41;
                D_800A603E = 0;
                o->step++;
            }
        }
        break;
    case 3:
        o->w48 = 0x2d;
        o->step++;
        D_800A60B6 = 0;
        break;
    case 4:
        if (--o->w48 == -1) {
            D_8009CD94 = 1;
            D_8009CD96 = 4;
            D_8009CDA0 = 4;
            D_8009C93C = 0;
            D_8009C975 = 3;
            o->step++;
        }
    case 5:
        {
            short *k = &D_800A60B6;
            *k += 0x20;
            if (*k > 0x500) *k = 0x500;
            { int *g = &D_1F8000F0; *g += *k << 8; }
        }
        if (D_8009C975 == 1) {
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
        }
        break;
    }
}

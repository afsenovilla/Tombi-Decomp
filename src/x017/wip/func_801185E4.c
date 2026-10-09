// FUNC 801185e4 656 X017
/* score ~5 (word diffs): case 3 tail only. The game leaves a nop in the bgtz delay slot and stores D_8009D00F first, then *s, 93A, 939, F2, state; ours puts the D_8009D00F store in the delay slot and the scalar F2 store before 939. Tried: scalar/[0] per global, all orders of the tail block, a volatile D_8009D00F name. k = 4 / k = 2 vars stop fold from reassociating the -4/-2 into the Rand() term. */
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned short D_8009C982;
extern unsigned char D_800A60E0, D_8009CFD6, D_8009D007, D_8009D00F, D_8009C939, D_8009C93F, D_8009C942, D_8009C93A;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern short D_1F8000EE, D_1F8000F2;
extern unsigned char D_800A603CA[], D_800A603DA[], D_800A603EA[], D_8009D00FA[], D_8009C939A[];
extern short D_1F8000EEA[], D_1F8000F2A[];
extern unsigned char D_8009C93AA[];
extern void FUN_8001e5f4(int, int);
extern unsigned int FUN_8001f9e0(void);
extern void FUN_800eea7c(TObj *, int, int);
extern void FUN_800eeb5c(TObj *, int);

void func_801185E4(TObj *o)
{
    short *s;
    int k;

    switch (o->state) {
    case 0:
        if (D_8009C982 == 0) {
            D_800A60E0 = 8;
            break;
        }
        if (D_800A6038.d->p.whole >= 100)
            break;
        o->state = 2;
        if (D_8009CFD6 == 0)
            break;
        if (D_8009D007 != 0)
            break;
        D_800A603CA[0] = 5;
        D_800A603DA[0] = 100;
        D_800A603EA[0] = 0;
        D_8009D00FA[0] = 2;
        o->w08 = 30;
        o->a.raw = D_1F8000EEA[0];
        o->y.raw = D_1F8000F2A[0];
        D_8009C939A[0] = 1;
        FUN_8001e5f4(0x28, 0x7f);
        o->state = 3;
        D_8009D007 = 1;
        D_8009C93F = 1;
        D_8009C942 = 1;
        break;
    case 1:
    case 2:
        D_800A60E0 = 8;
        break;
    case 3:
        s = &D_1F8000EE;
        k = 4;
        *s = o->a.raw - k + (int)(FUN_8001f9e0() & 7);
        k = 2;
        D_1F8000F2A[0] = o->y.raw - k + (int)(FUN_8001f9e0() & 3);
        if (--o->w08 > 0)
            break;
        D_8009D00FA[0] = 0;
        *s = o->a.raw;
        D_1F8000F2 = o->y.raw;
        D_8009C93AA[0] = 1;
        o->state = 4;
        D_8009C939A[0] = 0;
        o->w08 = 60;
        break;
    case 4:
        if (--o->w08 > 0)
            break;
        FUN_800eea7c(&D_800A6038, 0x24, 0);
        o->w08 = 60;
        o->state++;
        break;
    case 5:
        if (--o->w08 > 0)
            break;
        FUN_800eeb5c(&D_800A6038, 0x2b);
        o->state++;
        break;
    case 6:
        D_800A6038.d->p.whole += 3;
        break;
    }
}

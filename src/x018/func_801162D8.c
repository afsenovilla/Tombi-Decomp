// FUNC 801162d8 676 X018
// MATCHING 801162d8 676
/* csv entry 801162b8 (708 B) starts with 8 data words (jump-table/pointer data); code starts at 801162d8.
   debt: volatile read of pl->a in the last range test stops gcc threading the first test's failure jump past it. */
#include "TOBJ.H"

typedef void (*Fn)(TObj *);
extern TObj D_800A6038;
extern Fn D_80079AD0[];
extern unsigned char D_8009C93A, D_8009CDED, D_800A60E0;
extern unsigned short D_8009C962;
extern short D_800A604A, D_800A604E;
extern void FUN_80027810(TObj *);
extern void FUN_800270a0(TObj *, int, int);
extern void FUN_8002715c(int);
extern void FUN_8002a4d0(TObj *);

void func_801162D8(TObj *o)
{
    TObj *pl = &D_800A6038;

    switch (o->step) {
    case 0:
        if (D_8009C93A) o->step++;
        *(short *)((char *)o->d38 + 2) = 0;
        FUN_80027810(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (D_8009C962) {
        case 1:
            if (D_800A604A < 0x14 && D_800A604E >= -0x3b) {
                if (D_8009CDED == 0) goto set3;
                FUN_800270a0(o, 1, 0);
            } else if (pl->a.p.whole >= 0x12d && pl->y.p.whole >= -0x3b) {
                FUN_800270a0(o, 1, 2);
                break;
            }
            if (D_8009CDED != 0) {
                if ((unsigned short)(pl->a.p.whole - 0x70) < 0x60 && pl->b.p.whole > 0) {
                    FUN_8002715c(0xa0);
                    if (pl->b.p.whole >= 0x5b) FUN_800270a0(o, 1, 1);
                }
            } else {
            set3:
                D_800A60E0 = 3;
            }
            if ((unsigned short)(*(volatile short *)&pl->a.p.whole - 0x70) < 0x60 && pl->b.p.whole < 0) {
                FUN_8002715c(0xa0);
                if (pl->b.p.whole < -0x5a) FUN_800270a0(o, 1, 3);
            }
            break;
        case 2:
            if (D_800A604A < 0x14 && D_800A604E >= -0x3b) FUN_800270a0(o, 1, 0);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}

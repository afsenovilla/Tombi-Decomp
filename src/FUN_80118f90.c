// FUNC 80118f90 544 X000
// MATCHING 80118f90 544
#include "TOBJ.H"
extern int FUN_8005efe4(void);
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80118e70(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_800187e4(TObj *);
extern int DAT_1f8002d4[];
extern void *DAT_8013b108[];

void FUN_80118f90(TObj *o)
{
    short q;

    switch (o->b04) {
    case 0:
        o->wb4 = 1;
        o->wb6 = 0x80;
        o->wb8 = 0;
        o->wba = 0;
        o->w1e = (FUN_8005efe4() == 1 || FUN_8005efe4() == 2) ? 0x8a : 0x2a;
        o->w08 = FUN_8005e420(0xe0, 0x1e0);
        o->b0d = 0x80;
        o->d3c = DAT_1f8002d4[0];
        o->anim = DAT_8013b108[o->subtype];
        FUN_8001fe6c(o);
        o->timer = 0x38;
        o->w22 = 0;
        o->b04++;
        break;
    case 1:
        if (FUN_800202b4(o) == 0) {
            o->b04 = 3;
            break;
        }
        if (--o->timer == 0) {
            o->timer++;
            if (*(unsigned short *)&o->wb6 != 0) o->wb6--;
            o->w22 += 2;
            q = o->w22 / 10;
            if (q > 0) {
                o->w22 %= 10;
                o->wb8++;
                o->wba++;
            }
        }
        if (o->subtype == 1 && o->b0c == 1) FUN_80118e70(o);
        if (FUN_8001fec0(o)) o->b04++;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}

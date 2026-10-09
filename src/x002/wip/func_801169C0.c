// FUNC 801169c0 2528 X002
/* in progress: whole function incl. csv piece 80116D50 */
#include "TOBJ.H"
typedef struct {
    unsigned short w1e;
    short idx;
    void **tab;
} Ent;
extern Ent D_8011C6D8[];
extern int D_1F8002C8[];
extern unsigned char D_8009CE41;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CFDB, D_8009CFDC, D_8009CFDD, D_8009CFDE, D_8009CFDF, D_8009CFE0, D_8009CFE1;
extern unsigned short D_1F800176, D_1F800186;
extern short FUN_8005e420(int, int);
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

static __inline__ void sa(TObj *o, int n)
{
    if (o->subtype == 1) {
        o->w1e = 2;
        o->anim = D_8011C6D8[1].tab[n];
    } else {
        o->anim = D_8011C6D8[o->subtype].tab[n];
    }
    FUN_8001fe94(o, 0);
}

#define PICK(c) if (c) sa(o, 0); else sa(o, 2); break

void func_801169C0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->animFrame = 1;
        o->b0a = 7;
        o->b0d = 1;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->w08 = FUN_8005e420(0x80, 0x1ff);
        o->w1e = D_8011C6D8[o->subtype].w1e;
        o->d3c = D_1F8002C8[D_8011C6D8[o->subtype].idx];
        if (o->subtype == 1) {
            if (D_8009CE41 == 0) {
                o->w1e = 5;
                o->anim = D_8011C6D8[9].tab[0];
                FUN_8001fe94(o, 0);
            } else if (D_8009CE41 == 0xff) {
                sa(o, 2);
            } else {
                sa(o, 0);
            }
        } else if (D_8009CE41 == 0) {
            switch (o->subtype) {
            case 2: PICK(D_8009D2C3 & 2);
            case 3: PICK(D_8009D2C3 & 0x10);
            case 4: PICK(D_8009D2C3 & 0x40);
            case 5: PICK(D_8009D2C3 & 4);
            case 6: PICK(D_8009D2C3 & 1);
            case 7: PICK(D_8009D2C3 & 0x20);
            case 8: PICK(D_8009D2C3 & 8);
            }
        } else {
            switch (o->subtype) {
            case 2: PICK(D_8009CFDB);
            case 3: PICK(D_8009CFDC);
            case 4: PICK(D_8009CFDD);
            case 5: PICK(D_8009CFDE);
            case 6: PICK(D_8009CFDF);
            case 7: PICK(D_8009CFE0);
            case 8: PICK(D_8009CFE1);
            }
        }
        o->b04++;
        break;
    case 1:
        o->a.p.whole = o->d30 - D_1F800176;
        o->visible = 1;
        o->y.p.whole = o->d34 - D_1F800186;
        FUN_80018ca4(o);
        FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}

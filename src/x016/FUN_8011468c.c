// FUNC 8011468c 516 X016
// MATCHING 8011468c 516
#include "TOBJ.H"
typedef struct {
    unsigned short b0, b1, b2, b3, w1e;
    short idx;
    void **tab;
} Ent5de4;
extern Ent5de4 DAT_80115de4[];
extern int D_1f8002c8[];
extern void FUN_8001fe94(TObj *, int);
extern void FUN_800202b4(TObj *);
extern void FUN_80113f38(TObj *);
extern void FUN_801142c0(TObj *);
extern void FUN_80018790(TObj *);

void FUN_8011468c(TObj *o)
{
    char pad[16];

    switch (o->b04) {
    case 0:
        o->box0 = DAT_80115de4[o->subtype].b0;
        o->box1 = DAT_80115de4[o->subtype].b1;
        o->box2 = DAT_80115de4[o->subtype].b2;
        o->box3 = DAT_80115de4[o->subtype].b3;
        o->w1e = DAT_80115de4[o->subtype].w1e;
        o->d3c = D_1f8002c8[DAT_80115de4[o->subtype].idx];
        o->anim = DAT_80115de4[o->subtype].tab[o->w74];
        FUN_8001fe94(o, o->w76);
        o->b0a = 2;
        o->d8c = 0;
        o->b0d = 0;
        switch (o->subtype) {
        case 9:
            o->b0d = 1;
            o->w08 = 0x7fc8;
            o->b0a = 0;
            break;
        case 10:
            o->b0d = 1;
            o->w08 = 0x7809;
            break;
        }
        o->category |= 0x80;
        o->b04++;
        break;
    case 1:
        FUN_800202b4(o);
        FUN_80113f38(o);
        break;
    case 2:
        FUN_800202b4(o);
        if (o->step == 0)
            FUN_801142c0(o);
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

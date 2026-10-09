// FUNC 80126b68 432 X003
// MATCHING 80126b68 432
#include "TOBJ.H"
typedef struct B { unsigned char c[4]; } B;
extern B D_80135CB0[];
extern void *D_80139500[];
extern unsigned short D_80135D2C[];
extern unsigned char D_80135D5B[];
extern unsigned char D_80135D63[];
extern void ObjSetFacingToPlayer(TObj *);
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern void FUN_8001fe6c(TObj *);

void func_80126B68(TObj *o)
{
    switch (o->substep) {
    case 0:
        o->d8c = 0;
        ObjSetFacingToPlayer(o);
        o->timer = D_80135D2C[Rand() & 7];
        o->substep++;
        o->wac = D_80135D5B[o->wb6];
        o->anim = D_80139500[o->wac];
        { unsigned char *p = D_80135CB0[o->wac].c;
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1]; }
        FUN_8001fe6c(o);
        break;
    case 1:
        ObjSetFacingToPlayer(o);
        AnimAdvance(o);
        if (--o->timer == -1 || o->wb6 == 5) {
            o->substep++;
            o->wac = D_80135D63[o->wb6];
            o->wb6 = 0;
            o->anim = D_80139500[o->wac];
            { unsigned char *p = D_80135CB0[o->wac].c;
            o->box0 = *p++;
            o->box1 = *p++;
            o->box2 = *p;
            o->box3 = p[1]; }
            FUN_8001fe6c(o);
        }
        break;
    case 2:
        if (AnimAdvance(o)) {
            o->state = 1;
            o->substep = 0;
        }
        break;
    }
}

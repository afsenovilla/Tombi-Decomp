// FUNC 8011fb10 620 X001
// MATCHING 8011fb10 620
#include "TOBJ.H"
typedef struct {
    unsigned short b0, b1, b2, b3, w1e;
    short idx;
    void **tab;
} Ent;
extern Ent D_8013C730[];
extern int D_1F8002C8[];
extern unsigned char D_8009CEF7;
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018838(TObj *);
extern void func_8011F2B0(TObj *);
extern void func_8011F6C4(TObj *);

void func_8011FB10(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->box0 = D_8013C730[o->subtype].b0;
        o->box1 = D_8013C730[o->subtype].b1;
        o->box2 = D_8013C730[o->subtype].b2;
        o->box3 = D_8013C730[o->subtype].b3;
        o->w1e = D_8013C730[o->subtype].w1e;
        o->d3c = D_1F8002C8[D_8013C730[o->subtype].idx];
        o->anim = *D_8013C730[o->subtype].tab;
        o->d8c = 0;
        o->b0d = 0;
        o->category |= 0x80;
        o->b0a = 2;
        FUN_8001fe6c(o);
        o->b04 = 1;
        break;
    case 1:
        FUN_800202b4(o);
        if (o->visible) {
            switch (o->step) {
            case 0:
                break;
            case 1:
                func_8011F2B0(o);
                break;
            case 2:
                func_8011F6C4(o);
                break;
            }
        }
        break;
    case 2:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            o->step = 1;
            break;
        case 1:
            FUN_8001fec0(o);
            break;
        case 2:
            o->step = 5;
            o->state = 0;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        D_8009CEF7 = 0;
        FUN_80018838(o);
        break;
    }
}

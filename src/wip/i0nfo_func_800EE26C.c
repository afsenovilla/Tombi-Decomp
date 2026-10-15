// FUNC 800ee26c 596 I0NFO
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE6C(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800EE0F0(TObj *o);
extern unsigned char D_8009D064[];
extern signed char D_8009D2B0;
extern unsigned short D_800A604E;
extern void *D_800A6078;
extern unsigned char D_800A60A3;
typedef struct { short on; unsigned short x, y; short w, h, pad; } Area93C;
extern Area93C D_800EE93C[];
extern void *D_800F53D0;

void func_800EE26C(TObj *o)
{
    Area93C *a;
    unsigned char s;
    func_800202B4(o);
    o->visible = 0;
    o->b6b = 0;
    a = &D_800EE93C[o->b0c];
    if (a->on != 0) {
        if ((unsigned short)(U16(D_800A6078, 2) - a->x) < a->w
            && (unsigned short)(D_800A604E - a->y) < a->h) {
            o->b6b = 1;
            o->visible = 1;
        }
    } else {
        o->b6b = o->visible = D_800A60A3 != 0;
    }
    switch (o->step) {
    case 0:
        func_800EE0F0(o);
        break;
    case 1:
        s = o->state;
        switch (s) {
        case 0:
            o->state = s + 1;
        case 1:
            o->anim = D_800F53D0;
            func_8001FE6C(o);
            o->b68 = 0;
            o->state++;
        case 2:
            ((TObj *)PTR(o, 0x94))->visible = o->visible;
            if (o->b68 != 0) {
                D_8009D064[o->b0c] = 2;
            }
            o->b68 = 0;
            break;
        }
        break;
    case 2:
        if (o->state == 0) {
            o->anim = D_800F53D0;
            func_8001FE6C(o);
            o->state++;
        }
        break;
    }
    if (D_8009D2B0 == 3) {
        if (D_800A60A3 != 1 && S16(D_800A6078, 2) >= 0x29) {
            o->b6b = 0x80;
            o->visible = 1;
        } else {
            o->b6b = 0;
            o->visible = 0;
        }
    }
}

// VERSION SLPS_01144 info3 800EEE10 740
// JP version of PAL i0nfo3 800ed7a8 (tunnelActorTalkStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern void func_80020E0C(TObj *o, int n);
extern void func_80020E38(TObj *o);
extern void func_80027D58(TObj *o);
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void func_8002EDB0(int a, int b);
extern void func_8001C88C(int e, int a, int b);
extern void func_8001C988(int a, int b);
extern short func_800EEB3C(TObj *o);
extern unsigned char D_8009E4EA[];
extern unsigned char D_8009E94C[];
extern unsigned char D_8009E9DE;
extern unsigned char D_8009EB94[];
extern unsigned char D_800A7B7C[];
extern unsigned char D_800A7B7D[];
extern unsigned char D_800A7B7E[];
extern unsigned short D_800A7BA6[];
extern unsigned char D_800A7C24[];
extern void **D_800F0338[][3];

void func_800EEE10(TObj *o)
{
    short f;
    TObj *p;
    switch (o->state) {
    case 0:
        D_800A7B7C[0] = 1;
        D_800A7B7D[0] = 2;
        D_800A7B7E[0] = 0;
        D_800A7C24[0] = 3;
        D_800A7BA6[0] |= 8;
        o->wb0 = o->animFrame;
        f = o->b69 + 3;
        o->animFrame = o->b69 - 1;
        o->anim = D_800F0338[o->subtype][0][f];
        func_80020E0C(o, 0);
        S16(o, 0xD2) = f;
        o->b68 = 2;
        o->state++;
        if (func_800EEB3C(o) == 0) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
        }
        break;
    case 1:
        func_80020E38(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            goto done;
        }
        break;
    case 2:
        func_80020E38(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            if (D_8009E94C[o->wac] == 0) {
                func_8001C88C(o->wac, 0, 0);
                o->timer = 0xC8;
            } else {
                o->timer = 2;
            }
            o->state++;
        }
        break;
    case 3:
        if (--o->timer <= 0) {
        done:
            D_8009E4EA[0] = 0;
            D_800A7C24[0] = 0;
            D_800A7B7C[0] = 1;
            D_800A7B7D[0] = 0;
            D_800A7B7E[0] = 0;
            o->b68 = 0;
            o->b6a = 0;
            o->step = 0;
            o->state = 0;
            o->animFrame = o->wb0;
        }
        break;
    case 4:
        func_80020E38(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            if (D_8009E9DE != 0xFF) {
                func_8001C988(0x92, 0);
                o->timer = 0xC8;
            } else {
                o->timer = 2;
            }
            o->state = 3;
        }
        break;
    case 5:
        func_80020E38(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->timer = 4;
            if (D_8009EB94[0] == 0) {
                D_8009EB94[0] = 1;
                func_80027D58(p);
                func_8002EDB0(0x15, 3);
                o->timer = 0x3C;
            }
            o->state = 3;
        }
        break;
    }
}
#endif

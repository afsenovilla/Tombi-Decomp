// VERSION PCPX_96084 tomba 8003F5AC 400
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x000 8011F608 (updateA00L4Type17.c): halfword store at +0x0A, one-argument cull call.
#include "TOBJ.H"
extern void func_80018550(TObj *);
extern void func_8001A8C8_pcpx96084(TObj *);
extern unsigned char D_800F4FDD;
extern unsigned char D_800F4FE7[];
extern short *D_800F501C[];

void func_8003F5AC_pcpx96084(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (o->subtype == 0) {
            o->box0 = 0x58;
            o->box1 = 0x60;
            o->box2 = 4;
            o->box3 = 8;
            *(short *)&o->b0a = 0x11;
            o->b0f = D_800F4FE7[0] + 1;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->d->p.whole = D_800F501C[0][1];
            o->b04++;
        }
        break;
    case 1:
        func_80018550(o);
        switch (o->step) {
        case 0:
            if (D_800F4FDD == 5) {
                o->timer = 0x80;
                o->step++;
            }
            break;
        case 1:
            o->d8c += 0x20;
            o->y.p.whole += 2;
            if (--o->timer == 0) {
                o->b04 = 2;
                o->step = 0;
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_8001A8C8_pcpx96084(o);
        break;
    }
}
#endif

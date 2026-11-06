// VERSION SLPS_01144 x04 8011A268 1108
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern unsigned char D_800A609D;

void func_8011A268_slps01144(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->subtype == 0x17) {
            if (D_800A609D == 6 || D_800A609D == 0xd) {
                o->d38 = 0;
                o->step++;
            }
        } else if (D_800A609D == 9 || D_800A609D == 10) {
            o->d38 = 0;
            o->step++;
        }
        break;
    case 1:
        switch (o->b0c) {
        case 0:
            o->d->p.whole--;
            if (++o->d38 >= 0x1e) {
                o->timer = 0x1e;
                o->step++;
            }
            break;
        case 1:
            o->d->p.whole--;
            if (++o->d38 >= 0x14) {
                o->timer = 0x28;
                o->step++;
            }
            break;
        case 2:
            o->d->p.whole--;
            if (++o->d38 >= 0xa) {
                o->timer = 0x32;
                o->step++;
            }
            break;
        case 6:
            o->d->p.whole++;
            if (++o->d38 >= 0xa) {
                o->timer = 0x32;
                o->step++;
            }
            break;
        case 5:
            o->d->p.whole++;
            if (++o->d38 >= 0x14) {
                o->timer = 0x28;
                o->step++;
            }
            break;
        case 4:
            o->d->p.whole++;
            if (++o->d38 >= 0x1e) {
                o->timer = 0x1e;
                o->step++;
            }
            break;
        case 7:
            break;
        }
        break;
    case 2:
        if (--o->timer == -1) {
            o->d38 = 0;
            o->step++;
        }
        break;
    case 3:
        switch (o->b0c) {
        case 2:
            o->d->p.whole++;
            if (++o->d38 >= 0xa)
                o->step++;
            break;
        case 1:
            o->d->p.whole++;
            if (++o->d38 >= 0x14)
                o->step++;
            break;
        case 0:
            o->d->p.whole++;
            if (++o->d38 >= 0x1e)
                o->step++;
            break;
        case 4:
            o->d->p.whole--;
            if (++o->d38 >= 0x1e)
                o->step++;
            break;
        case 5:
            o->d->p.whole--;
            if (++o->d38 >= 0x14)
                o->step++;
            break;
        case 6:
            o->d->p.whole--;
            if (++o->d38 >= 0xa)
                o->step++;
            break;
        case 7:
            break;
        }
        break;
    case 4:
        o->timer = 0x78;
        o->step++;
        break;
    case 5:
        if (--o->timer == -1)
            o->step = 0;
        break;
    }
}
#endif

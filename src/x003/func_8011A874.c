// FUNC 8011a874 1376 X003
// MATCHING 8011a874 1376
/* includes the csv piece 8011ADC0 (the shared epilogue) */
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_800A60E3;
extern unsigned char D_8009CDCD, D_8009CEF0, D_8009D2C3, D_8009CE4E, D_8009D0D9, D_8009CDDB;
extern unsigned char D_8009C93F[], D_8009C942[], D_8009C93E[];
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern int func_8011A6C4(TObj *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);

#define DOOR(o) \
    switch (o->state) { \
    case 0: \
        o->state = ((TObj *)o->d90)->state; \
        o->d30 = 0; \
        if (o->state == 3) { \
            o->d30 = 0; \
            o->h->p.whole += 0x20; \
        } \
        break; \
    case 1: \
        o->h->raw += 0x18000; \
        o->d30 += 0x18000; \
        if ((o->d30 >> 16) >= 0x20) o->state++; \
        break; \
    case 2: \
    case 3: \
        o->state = ((TObj *)o->d90)->state; \
        break; \
    case 4: \
        o->h->raw -= 0x18000; \
        o->d30 -= 0x18000; \
        if ((o->d30 >> 16) <= 0) o->state = 0; \
        break; \
    }

void func_8011A874(TObj *o)
{
    unsigned char s = o->b04;
    unsigned char t;
    int r;

    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            r = func_8011A6C4(o);
            if (r == 1) {
                if (D_800A60E3 == r) {
                    if (D_8009CDCD == 0xff) break;
                    o->substep = 4;
                } else {
                    if (D_8009CDCD != 0) break;
                    o->substep = 1;
                }
                o->wac = 0x29;
            }
            break;
        case 1:
            DOOR(o);
            break;
        case 2:
            t = o->state;
            switch (t) {
            case 0:
                if (D_8009CEF0) o->state = 2;
                else o->state = t + 1;
                break;
            case 1:
                if (D_800A6038.h->p.whole < 0x25) D_800A6038.h->p.whole = 0x25;
                o->d88 = 0;
                break;
            case 2:
                o->d88 = 0x400;
                break;
            }
            break;
        case 3:
            switch (o->state) {
            case 0:
                o->state = ((TObj *)o->d90)->state;
                break;
            case 1:
                o->d88 = 0;
                break;
            case 2:
                o->d88 = 0xc00;
                break;
            }
            break;
        case 4:
            r = func_8011A6C4(o);
            if (D_8009D2C3 & 2) {
                if (r != 2) break;
                if (D_8009CE4E == 0xff) break;
                o->substep = 4;
            } else {
                if (r != 1) break;
                if (D_8009CE4E != 0) break;
                o->substep = 1;
            }
            o->wac = 0xaa;
            break;
        case 5:
            DOOR(o);
            break;
        case 8:
            r = func_8011A6C4(o);
            if (D_8009D0D9) {
                if (r != 2) break;
                if (D_8009CDDB == 0xff) break;
                o->substep = 4;
            } else {
                if (r != 1) break;
                if (D_8009CDDB != 0) break;
                o->substep = 1;
            }
            o->wac = 0x37;
            break;
        case 9:
            DOOR(o);
            break;
        }
        switch (o->substep) {
        case 0:
            break;
        case 1:
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_8009C93E[0] = 1;
            o->timer = 0x12c;
            o->substep++;
            FUN_8005a8a8(o->wac, 0, 0);
            break;
        case 4:
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_8009C93E[0] = 1;
            o->timer = 0x12c;
            o->substep++;
            FUN_8005a9a4(o->wac, 0);
            break;
        case 2:
        case 5:
            if (--o->timer == -1) o->substep++;
            break;
        case 3:
        case 6:
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_8009C93E[0] = 0;
            o->substep = 0;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

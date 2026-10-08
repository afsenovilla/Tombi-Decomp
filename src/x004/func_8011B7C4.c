// FUNC 8011b7c4 1868 X004
// MATCHING 8011b7c4 1868
#include "TOBJ.H"

extern unsigned short D_1F8001C8;
extern short D_1F800172;
extern unsigned char D_8009D0DA, D_8009D0D9;
extern unsigned char D_8009CDD7, D_8009CDCD, D_8009CDCE, D_8009CDDB;
extern unsigned char D_800A60E3;
extern unsigned short D_8009C962;
extern unsigned char D_8009C93F[], D_8009C942[], D_8009C93E[];
extern void ObjListPush_1F800224(TObj *);
extern void ObjFreeDup(TObj *);
extern void playSFX(int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern int func_8011AFA8(TObj *);
extern int func_8011B218(TObj *);
extern void func_8011B138(TObj *);
extern void func_8011B438(TObj *);

void func_8011B7C4(TObj *o)
{
    int r;
    short *p;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d8c = 0;
        switch (o->animFrame) {
        case 0:
            o->d88 = 0;
            break;
        case 1:
            o->d88 = 0x400;
            break;
        case 2:
            o->d88 = 0x800;
            break;
        }
        if (o->subtype == 0x16 || o->subtype == 0x17)
            *(signed char *)&o->b0f = -2;
        else
            o->b0f = 4;
        o->b6b = 0;
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800224(o);
        switch (o->subtype) {
        case 4:
            if (D_1F8001C8 & 1) break;
            r = func_8011AFA8(o);
            if (D_8009D0DA) {
                if (r == 2 && D_1F800172 == 0x384) {
                    if (D_8009CDD7 == 0xff) {
                        o->b6b = 1;
                    } else {
                        o->substep = 4;
                        o->wac = 0x33;
                    }
                }
            } else {
                if (r == 1 && D_1F800172 == 0x384) {
                    o->state = 0;
                    if (D_8009CDD7 == 0) {
                        o->substep = 1;
                        o->wac = 0x33;
                    }
                }
            }
            if (r == 2 && o->b6b == 1) playSFX(0x7e);
            break;
        case 2:
        case 6:
        case 14:
        case 16:
        case 18:
            if (func_8011AFA8(o) == 2) playSFX(0x7e);
            break;
        case 20:
            if (D_8009C962 == 2 && func_8011AFA8(o) == 2) playSFX(0x7e);
            break;
        case 3:
        case 5:
        case 7:
        case 15:
        case 17:
        case 19:
        case 21:
            func_8011B138(o);
            break;
        case 0:
            if (D_1F8001C8 & 1) break;
            p = &D_1F800172;
            if (*p >= 0x384) break;
            r = func_8011B218(o);
            if (r == 1 && *p == 0x32a) {
                if (D_800A60E3 == 1) {
                    if (D_8009CDCD == 0xff) break;
                    o->substep = 4;
                } else {
                    o->state = 0;
                    if (D_8009CDCD != 0) break;
                    o->substep = 1;
                }
                o->wac = 0x29;
                break;
            }
            if (r == 2) playSFX(0x80);
            break;
        case 10:
            if (!(D_1F8001C8 & 1)) break;
            r = func_8011B218(o);
            if (r == 1 && D_1F800172 == 0x546) {
                if (D_800A60E3 == 2) {
                    if (D_8009CDCE == 0xff) break;
                    o->substep = 4;
                } else {
                    o->state = 0;
                    if (D_8009CDCE != 0) break;
                    o->substep = 1;
                }
                o->wac = 0x2a;
                break;
            }
            if (r == 2) playSFX(0x81);
            break;
        case 8:
            if (D_1F8001C8 & 1) break;
            r = func_8011B218(o);
            if (D_8009D0D9) {
                if (r == 2 && D_1F800172 == 0x1c2) {
                    if (D_8009CDDB == 0xff) {
                        o->b6b = 1;
                    } else {
                        o->substep = 4;
                        o->wac = 0x37;
                    }
                }
            } else {
                if (r == 1 && D_1F800172 == 0x1c2) {
                    o->state = 0;
                    if (D_8009CDDB == 0) {
                        o->substep = 1;
                        o->wac = 0x37;
                    }
                }
            }
            if (r == 2 && o->b6b == 1) playSFX(0x7f);
            break;
        case 12:
            if (D_1F8001C8 & 1) break;
            if (func_8011B218(o) == 2) playSFX(0x7f);
            break;
        case 1:
        case 9:
        case 11:
        case 13:
            switch (o->state) {
            case 0:
                o->state = ((TObj *)o->d90)->state;
                o->d30 = 0;
                if (o->state == 3) {
                    o->d30 = 0;
                    o->h->p.whole += 0x20;
                }
                break;
            case 1:
                o->h->raw += 0x18000;
                o->d30 += 0x18000;
                if ((o->d30 >> 16) >= 0x20) o->state++;
                break;
            case 2:
                break;
            case 3:
                o->h->raw += -0x18000;
                o->d30 += -0x18000;
                if ((o->d30 >> 16) < -0x1f) o->state++;
                break;
            case 4:
                o->state = ((TObj *)o->d90)->state;
                break;
            }
            break;
        case 22:
        case 23:
            func_8011B438(o);
            break;
        }
        switch (o->substep) {
        case 0:
            break;
        case 1:
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_8009C93E[0] = 1;
            o->timer = 300;
            o->substep++;
            FUN_8005a8a8(o->wac, 0, 0);
            break;
        case 3:
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_8009C93E[0] = 0;
            o->substep = 0;
            break;
        case 4:
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_8009C93E[0] = 1;
            o->timer = 300;
            o->substep++;
            FUN_8005a9a4(o->wac, 0);
            break;
        case 2:
        case 5:
            if (--o->timer == -1) o->substep++;
            break;
        case 6:
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_8009C93E[0] = 0;
            o->substep = 0;
            o->b6b = 1;
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

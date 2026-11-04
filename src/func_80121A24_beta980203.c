// VERSION BETA_980203 x00 80121A24 364
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/a00SwingGateMechanism.c (func_8011D190) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { short p; unsigned short whole; } HW;
typedef struct { char p0[6]; unsigned char st; char p1[0x16 - 7]; unsigned short w16; char p2[0x40 - 0x18]; HW *h; char p3[0x88 - 0x44]; int d88; } O;
extern unsigned short PLAYER_H_WHOLE, PLAYER_Y_WHOLE;
extern short PLAYER_D_WHOLE;
extern unsigned char D60da, GVARS_PLAYER_LANDED;
extern signed char GAME_PLAYER_MODE;

void func_80121A24_beta980203(O *o)
{
    int t;
    unsigned char c;
    switch (o->st) {
    case 0:
        if ((unsigned short)(o->h->whole - PLAYER_H_WHOLE + 0x20) > 0x40) break;
        if ((unsigned short)(o->w16 - PLAYER_Y_WHOLE + 0x30) > 0x60) break;
        if (D60da == 1) break;
        if (D60da == 2) {
            c = o->st;
            o->d88 = 0;
            goto inc2;
        }
        if (GVARS_PLAYER_LANDED != 0) break;
        if (GAME_PLAYER_MODE >= 3) break;
        if (PLAYER_D_WHOLE <= 0) break;
        o->st = 3;
        o->d88 = 0xa00;
        break;
    case 1:
        t = (o->d88 - 0x20) & 0xfff;
        o->d88 = t;
        if (t > 0xa00) break;
        goto inc;
    case 2:
        break;
    case 3:
        t = (o->d88 + 0x20) & 0xfff;
        o->d88 = t;
        if (t != 0) break;
    inc:
        c = o->st;
    inc2:
        o->st = c + 1;
        break;
    case 4:
        if (GVARS_PLAYER_LANDED != 0)
            o->st = 0;
        break;
    }
}
#endif

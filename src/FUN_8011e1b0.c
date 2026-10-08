// FUNC 8011e1b0 348 X000
// MATCHING 8011e1b0 348
typedef struct { char p0[6]; unsigned char st; char p1[0x16 - 7]; unsigned short w16; char p2[0x88 - 0x18]; int d88; } O;
extern unsigned short G16a, G16e;
extern short G172;
extern unsigned char D60da, C36c6;
extern signed char D2d50;

void FUN_8011e1b0(O *o)
{
    int t;
    unsigned char c;
    switch (o->st) {
    case 0:
        if ((unsigned short)(G16a - 0x400) >= 0x18) break;
        if ((unsigned short)(o->w16 - G16e + 0x30) > 0x60) break;
        if (D60da == 1) break;
        if (D60da == 2) {
            c = o->st;
            o->d88 = 0;
            goto inc2;
        }
        if (C36c6 != 0) break;
        if (D2d50 >= 3) break;
        if (G172 <= 0) break;
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
        if (C36c6 != 0)
            o->st = 0;
        break;
    }
}

// FUNC 8011e1b0 348 X000
// MATCHING 8011e1b0 348
// probably: updateAreaMechanisms entry: swings +0x88 open (to 0xA00) when the player is within 0x18 x / 0x60 y and the event bytes allow, closes again (states 1/3)
typedef struct { char p0[6]; unsigned char st; char p1[0x16 - 7]; unsigned short w16; char p2[0x88 - 0x18]; int d88; } O;
extern unsigned short PLAYER_H_WHOLE, PLAYER_Y_WHOLE;
extern short PLAYER_D_WHOLE;
extern unsigned char D60da, GVARS_PLAYER_LANDED;
extern signed char GAME_PLAYER_MODE;

void FUN_8011e1b0(O *o)
{
    int t;
    unsigned char c;
    switch (o->st) {
    case 0:
        if ((unsigned short)(PLAYER_H_WHOLE - 0x400) >= 0x18) break;
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

// FUNC 80111dc8 400 X017
// MATCHING 80111dc8 400
// FLAGS -O2 -G0 -fno-cse-skip-blocks
/* Debt: -fno-cse-skip-blocks (precedent: playerUpdateSpeedFromPad). The velX update is written in both arms of
   `if (r == 0)`: at sched2 each copy ends its basic block, so `sh velX` is scheduled right after the
   addu (negu/lh follow); jump2 then cross-jumps the two copies into one tail. With the default flags
   cse follows the `if (velX < 0)` skip and turns the then-arm's lhu into an andi of the compare's lh,
   so the tails differ and are not merged. The sum into a function-scope `t` (global pseudo, also used
   in the tail) keeps `addu v1,v0,v1` (velX first, result in s's register). */
#include "TOBJ.H"
extern short DAT_8013c984[];
extern short DAT_8007a1f0[], COS_TABLE[];

void treasureChestTossMove(TObj *o)
{
    int s;
    char pad[4];
    int r;
    int a;
    unsigned int v;
    int t;

    if (o->b69) {
        o->b69 = 0;
        o->w78 = ((o->d38 + 0x800) & 0xfff) >> 4;
        if (o->w78 > 0x80) o->w78 = 0x80;
        o->velY = 0x100;
        r = DAT_8013c984[(unsigned)(o->w78 - 0x40) >> 3 & 0xf];
        s = r;
        if (r == 0) {
            s = -4;
            if (o->velX < 0) s = 4;
            t = (unsigned short)o->velX + s;
            o->velX = t;
        } else {
            t = (unsigned short)o->velX + s;
            o->velX = t;
        }
        a = -t;
        o->velV = ((short)a * DAT_8007a1f0[o->w78]) >> 12;
        o->velH = ((short)a * COS_TABLE[o->w78]) >> 12;
        o->h->raw += o->velH << 8;
        o->y.raw = o->y.raw + (o->velV << 8);
    } else {
        o->velY += 0x20;
        if (o->velY > 0x380) o->velY = 0x380;
        o->y.raw = o->y.raw + (o->velY << 8);
    }
    a = o->d8c;
    t = (unsigned short)o->w78;
    v = (unsigned char)(a - 0x80 - t);
    if (v != 0) {
        if (v < 0x80) o->d8c = a - 1;
        else o->d8c = a + 1;
        o->d8c = (unsigned char)o->d8c;
    }
}

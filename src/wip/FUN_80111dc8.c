// FUNC 80111dc8 400 X000
/* score 6: only the velX sum register differs (game: addu v1,v0,v1 i.e. sum tied to s in v1; ours sum in v0 tied to the velX load). Reusing r (the table value) for the sum keeps the s=r copy in the bnez delay slot (score 27 -> 6). y update written in full in both branches (cross-jump gives the lw-first merge block). Tried: s/r/a types, s+=, inline sel(), x temps, r reused for d. */
/* b32 also tried: s/r reuse for the sum (all 3 sum targets), ternary/if-else selection forms, s/r types: none below 6. */
/* b47: -dg shows the cause: s (global) gets preference a0 from a = -s (set_preference peels the neg), and r is allocated first (v0); the game needs s in v1 with the sum. Reusing r for v (the d8c delta) keeps the copy but s takes a0 (12); local t for the sum gets tied to a (33-47); type brute force s/r/a/t: no gain. */
/* b52: with sum in s, register int s asm("$3") makes a tie to s (v1) since s dies at the neg (14); a=~s+1 (39); 0-s, -1*s, 0x10000-s fold to neg (16); a=-s before the velX store (16). Needs s free of a0 preference AND a not tied to s. */
#include "TOBJ.H"
extern short DAT_8013c984[];
extern short DAT_8007a1f0[], DAT_8007a5f0[];

static __inline__ void f(TObj *o)
{
    int s;
    char pad[4];
    int r;
    short a;
    int t;
    unsigned int v;
    int d;

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
        }
        r = (unsigned short)o->velX + s;
        a = -r;
        o->velX = r;
        o->velV = (a * DAT_8007a1f0[o->w78]) >> 12;
        o->velH = (a * DAT_8007a5f0[o->w78]) >> 12;
        o->h->raw += o->velH << 8;
        o->y.raw = o->y.raw + (o->velV << 8);
    } else {
        o->velY += 0x20;
        if (o->velY > 0x380) o->velY = 0x380;
        o->y.raw = o->y.raw + (o->velY << 8);
    }
    d = o->d8c;
    { int w = (unsigned short)o->w78; v = (unsigned char)(d - 0x80 - w); }
    if (v != 0) {
        if (v < 0x80) o->d8c = d - 1;
        else o->d8c = d + 1;
        o->d8c = (unsigned char)o->d8c;
    }
}

void FUN_80111dc8(TObj *o)
{
    f(o);
}

// FUNC 80111dc8 400 X000
/* score 27 (was 59): left: table value s loaded into v0 and copied to v1 (move v1,v0 in bnez slot), merge block of the y update starts with lw y (ours sll t), and the d8c angle diff loads w78 with lbu (game lhu, (d-0x80)-w order kept only via a char cast). char pad[4] restores the 8-byte frame. */
#include "TOBJ.H"
extern short DAT_8013c984[];
extern short DAT_8007a1f0[], DAT_8007a5f0[];

void FUN_80111dc8(TObj *o)
{
    int u;
    int s;
    char pad[4];
    short a;
    int d;
    int t;
    unsigned int v;

    if (o->b69) {
        o->b69 = 0;
        u = ((o->d38 + 0x800) & 0xfff) >> 4;
        o->w78 = u;
        if (u > 0x80) o->w78 = 0x80;
        o->velY = 0x100;
        s = DAT_8013c984[(unsigned)(o->w78 - 0x40) >> 3 & 0xf];
        if (s == 0 && (s = -4, o->velX < 0)) s = 4;
        s = (unsigned short)o->velX + s;
        o->velX = s;
        a = -s;
        o->velV = (a * DAT_8007a1f0[o->w78]) >> 12;
        o->velH = (a * DAT_8007a5f0[o->w78]) >> 12;
        o->h->raw += o->velH << 8;
        t = o->velV;
    } else {
        o->velY += 0x20;
        if (o->velY > 0x380) o->velY = 0x380;
        t = o->velY;
    }
    o->y.raw = (t << 8) + o->y.raw;
    d = o->d8c;
    v = (char)(d - 0x80 - (unsigned short)o->w78) & 0xff;
    if (v != 0) {
        if (v < 0x80) o->d8c = d - 1;
        else o->d8c = d + 1;
        o->d8c = (unsigned char)o->d8c;
    }
}

// FUNC 80116f74 1312 X003
// MATCHING 80116f74 1312
/* csv entry 80116D50 (1860) = 548 B of data (pointer table) + this function at 80116F74. */
typedef struct { unsigned short frac; short whole; } FP;
typedef struct { char pad[0x32]; short y; FP *p; } S;

extern unsigned short D_8009C962;

void func_80116F74(S *o)
{
    short x = o->p->whole;

    if (D_8009C962 == 0 || D_8009C962 == 4) {
        if (x < 0xfa) o->y = -0x78;
        else if (x < 0x1b9) o->y = -((short)(x - 0xfa) * 248) / 190 - 0x78;
        else if (x < 0x505) o->y = -((short)(x - 0x1b8) * 245) / 844 - 0x170;
        else if (x < 0x760) o->y = -((short)(x - 0x504) * 108) / 603 - 0x265;
        else if (x < 0x9c5) o->y = -((short)(x - 0x760) * 279) / 612 - 0x2d1;
        else o->y = -0x3e8;
    } else if (D_8009C962 == 1 || D_8009C962 == 5) {
        if (x < 0xc24) o->y = -0x41a;
        else if (x < 0xd2b) o->y = -((short)(x - 0xc24) * 130) / 262 - 0x41a;
        else if (x < 0xe42) o->y = -((short)(x - 0xd2a) * 182) / 279 - 0x49c;
        else if (x < 0xfbf) o->y = -((short)(x - 0xe41) * 238) / 381 - 0x552;
        else if (x < 0x10b2) o->y = (short)(x - 0xfbe) * 170 / 243 - 0x640;
        else if (x < 0x1268) o->y = -((short)(x - 0x10b1) * 470) / 438 - 0x596;
        else if (x < 0x1384) o->y = -((short)(x - 0x1267) * 270) / 284 - 0x76c;
        else if (x < 0x1548) o->y = -((short)(x - 0x1383) * 145) / 452 - 0x87a;
        else o->y = -0x90b;
    } else if (D_8009C962 == 2) {
        if (x < 0x259) o->y = -0x168;
        else if (x < 0x385) o->y = -((short)(x - 0x258) * 150) / 300 - 0x168;
        else if (x < 0x3e9) o->y = -((short)(x - 0x384) * 70) / 100 - 0x1fe;
        else if (x < 0x641) o->y = -((short)(x - 0x3e8) * 30) / 600 - 0x244;
        else o->y = -0x262;
    }
}

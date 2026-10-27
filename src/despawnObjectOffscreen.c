// FUNC 80020490 156 MAIN0
// MATCHING 80020490 156
// VERSION SLPS_01144 slps01144 80021408 156
// VERSION SLPM_80165 slpm80165 80020B78 156
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 80020490 (despawnObjectOffscreen.c): 160 -> 152.
extern short DAT_800a4582;
extern unsigned short SCREEN_ORIGIN_X, SCREEN_ORIGIN_Y;
typedef struct O { unsigned char active, visible, p2, p3, b04; char p5[0x16 - 5]; short y; char p18[0x40 - 0x18]; unsigned short *h; } O;
void despawnObjectOffscreen(O *o)
{
    char pad;
    short y;
    if (o->active != 0) {
        y = o->y;
        if (DAT_800a4582 + 0x98 <= y
            || (unsigned short)(o->h[1] - SCREEN_ORIGIN_X + 0xa0) > 0x27f
            || (unsigned short)(SCREEN_ORIGIN_Y - y + 0xa0) > 0x22f) {
            o->b04 = 3;
            o->active = 2;
            o->visible = 0;
        }
    }
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80020490 (despawnObjectOffscreen.c): bottom margin 0x98 (PAL 0xa0).
extern short DAT_800a4582;
extern unsigned short SCREEN_ORIGIN_X, SCREEN_ORIGIN_Y;
typedef struct O { unsigned char active, visible, p2, p3, b04; char p5[0x16 - 5]; short y; char p18[0x40 - 0x18]; unsigned short *h; } O;
void despawnObjectOffscreen(O *o)
{
    char pad;
    short y;
    if (o->active != 0) {
        y = o->y;
        if (DAT_800a4582 + 0x98 <= y
            || (unsigned short)(o->h[1] - SCREEN_ORIGIN_X + 0xa0) > 0x27f
            || (unsigned short)(SCREEN_ORIGIN_Y - y + 0xa0) > 0x22f) {
            o->b04 = 3;
            o->active = 2;
            o->visible = 0;
        }
    }
}
#else
extern short DAT_800a4582;
extern unsigned short SCREEN_ORIGIN_X, SCREEN_ORIGIN_Y;
typedef struct O { unsigned char active, visible, p2, p3, b04; char p5[0x16 - 5]; short y; char p18[0x40 - 0x18]; unsigned short *h; } O;
void despawnObjectOffscreen(O *o)
{
    char pad;
    short y;
    if (o->active != 0) {
        y = o->y;
        if (DAT_800a4582 + 0xa0 <= y
            || (unsigned short)(o->h[1] - SCREEN_ORIGIN_X + 0xa0) > 0x27f
            || (unsigned short)(SCREEN_ORIGIN_Y - y + 0xa0) > 0x22f) {
            o->b04 = 3;
            o->active = 2;
            o->visible = 0;
        }
    }
}
#endif

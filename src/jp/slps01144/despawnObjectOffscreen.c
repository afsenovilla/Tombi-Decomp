// JP slps01144 80021408 156
// Japanese version of PAL main0 80020490 (despawnObjectOffscreen.c): 160 -> 152.
extern short DAT_800a4582;
extern unsigned short g176, g186;
typedef struct O { unsigned char active, visible, p2, p3, b04; char p5[0x16 - 5]; short y; char p18[0x40 - 0x18]; unsigned short *h; } O;
void despawnObjectOffscreen(O *o)
{
    char pad;
    short y;
    if (o->active != 0) {
        y = o->y;
        if (DAT_800a4582 + 0x98 <= y
            || (unsigned short)(o->h[1] - g176 + 0xa0) > 0x27f
            || (unsigned short)(g186 - y + 0xa0) > 0x22f) {
            o->b04 = 3;
            o->active = 2;
            o->visible = 0;
        }
    }
}

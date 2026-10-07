// FUNC 80020490 156 MAIN0
extern short DAT_800a4582;
extern unsigned short g176, g186;
typedef struct O { unsigned char active, visible, p2, p3, b04; char p5[0x16 - 5]; short y; char p18[0x40 - 0x18]; short **h; } O;
void FUN_80020490(O *o)
{
    short y;
    if (o->active != 0) {
        y = o->y;
        if (DAT_800a4582 + 0xa0 <= y
            || (unsigned short)((*o->h)[1] - g176 + 0xa0) > 0x27f
            || (unsigned short)(g186 - y + 0xa0) > 0x22f) {
            o->b04 = 3;
            o->active = 2;
            o->visible = 0;
        }
    }
}

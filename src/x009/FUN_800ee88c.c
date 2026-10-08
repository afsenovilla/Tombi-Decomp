// FUNC 800ee88c 148 X009
// MATCHING 800ee88c 148
typedef struct O { char p0[3]; unsigned char sub; char p1[0x10]; int y; char p2[0xad-0x18]; unsigned char f; char p3[2]; short w; } O;
void FUN_800ee88c(O *o)
{
    if (o->f == 0) {
        if (o->sub != 0) {
            if ((unsigned short)(o->w + 5) > 10) o->y += 0x50000;
            else o->y += 0x30000;
        } else {
            if ((unsigned short)(o->w + 5) > 10) o->y += 0xc0000;
            else o->y += 0x80000;
        }
    }
}

// FUNC 800ee920 40 X010
// MATCHING 800ee920 40
typedef struct { char p[0x14]; int y; char q[0xad-0x18]; unsigned char f; } O;
void func_800EE920(O *o)
{
    if (o->f == 0) o->y += 0x7c000;
}

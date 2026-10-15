// JPDEMO slpm80165 80018638 16
// Demo-only helper: sets the halfwords at +0x24/+0x26/+0x28.
void func_80018638(char *o, short a, short b, short c)
{
    *(short *)(o + 0x24) = a;
    *(short *)(o + 0x26) = b;
    *(short *)(o + 0x28) = c;
}

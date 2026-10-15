// JPDEMO slpm80165 80020570 12
// Demo-only helper: copies the timer (+0x20) to +0x2c.
void func_80020570(char *o)
{
    *(short *)(o + 0x2c) = *(short *)(o + 0x20);
}

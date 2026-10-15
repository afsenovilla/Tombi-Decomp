// JPDEMO slpm80165 80018648 8
// Demo-only helper: sets the halfword at +0x28.
void func_80018648(char *o, short v)
{
    *(short *)(o + 0x28) = v;
}

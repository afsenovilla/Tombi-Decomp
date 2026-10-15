// JPDEMO slpm80165 8003C7D8 152
// Demo-only helper: probes 8 pixels to each side of the object, 12 above its y.
extern short func_8003D4C0(char *, short, short);
short func_8003C7D8(char *a)
{
    char *o = a;
    short r;
    r = func_8003D4C0(o, (*(short **)(o + 0x40))[1] + 8, *(short *)(o + 0x16) - 12);
    if (r != 0) return r;
    r = func_8003D4C0(o, (*(short **)(o + 0x40))[1] - 8, *(short *)(o + 0x16) - 12);
    if (r != 0) return r;
    return 0;
}

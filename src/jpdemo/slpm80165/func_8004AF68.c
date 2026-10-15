// JPDEMO slpm80165 8004AF68 128
// Demo-only hit handler: on contact, turns the target, plays sound 5 (not for type 1) and spawns a hit effect.
extern short func_8004AB10(unsigned char *, unsigned char *);
extern void func_8001E3D8(int);
extern void func_80020054(int, int, int, int);
void func_8004AF68(unsigned char *a, unsigned char *b)
{
    short r = func_8004AB10(a, b);
    if (r >= 0) {
        *(short *)(b + 0x2e) = r & 1;
        if (a[2] != 1)
            func_8001E3D8(5);
        b[0x68] = *(short *)(a + 0x98);
        func_80020054(1, *(short *)(a + 0x12), *(short *)(a + 0x16), *(short *)(a + 0x1a));
    }
}

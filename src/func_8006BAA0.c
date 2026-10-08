// FUNC 8006baa0 32 MAIN0
// MATCHING 8006baa0 32
extern char D_8009C038[];

char *func_8006BAA0(int a)
{
    char *p = D_8009C038;
    if (a & 0xf0) {
        p += 0xf0;
    }
    return p;
}

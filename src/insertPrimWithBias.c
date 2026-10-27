// FUNC 8004fd6c 92 MAIN0
// MATCHING 8004fd6c 92
extern int CURRENT_OT;
int insertPrimWithBias(unsigned *a, int b, int c, int d, unsigned e)
{
    unsigned t;
    c = c * 4 + ((d << 16) >> 14);
    if (c < 0)
        c = 0;
    c += b;
    if ((unsigned)(c - CURRENT_OT) >= 0xca0)
        return 1;
    t = *(unsigned *)c;
    *(unsigned *)c = (unsigned)a;
    *a = t | e;
    return 0;
}

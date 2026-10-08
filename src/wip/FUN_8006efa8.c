// FUNC 8006efa8 156 MAIN0
typedef struct { short s0, s2, s4, s6, s8, sa, sc, se; } E;
extern E *TAB;

int FUN_8006efa8(unsigned short p, short v, unsigned int f)
{
    int i = p;
    int r = 0x48;
    int ret;
    E *e;
    if (i >= 3) {
        ret = 0;
    } else {
        e = (E *)((i << 4) + (int)TAB);
        e->s4 = 0;
        e->s8 = v;
        if ((unsigned)i < 2) {
            if (f & 0x10) r = 0x49;
            if (!(f & 1)) r |= 0x100;
        } else if (i == 2) {
            if (!(f & 1)) r = 0x248;
        }
        ret = 1;
        if (f & 0x1000) r |= 0x10;
        e = (E *)((i << 4) + (int)TAB);
        e->s4 = r;
    }
    return ret;
}

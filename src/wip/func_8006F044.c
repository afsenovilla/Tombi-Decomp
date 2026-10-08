// FUNC 8006f044 56 MAIN0
typedef struct { unsigned short v; unsigned char pad[14]; } E16;
extern E16 *D_800982CC;

unsigned short func_8006F044(unsigned short i)
{
    E16 *t;
    unsigned short r;
    if ((int)i < 3) {
        t = D_800982CC;
        r = t[i].v;
    } else {
        r = 0;
    }
    return r;
}

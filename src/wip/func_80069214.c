// FUNC 80069214 212 MAIN0
typedef struct {
    int f0;
    unsigned char *p4;
    char pad[0xe9 - 8];
    unsigned char ne9;
} S80069214;

extern S80069214 *(*D_800981C0)();

int func_80069214(int a, int i, int j)
{
    S80069214 *s = D_800981C0();
    unsigned char *e;

    if (i < 0) {
        return s->ne9;
    }
    if (i < s->ne9) {
        e = s->p4 + i * 5;
        switch (j) {
        case 1: return e[0];
        case 2: return e[1];
        case 3: return e[2];
        case 4: return e[3];
        case 5: return e[4];
        }
    }
    return 0;
}

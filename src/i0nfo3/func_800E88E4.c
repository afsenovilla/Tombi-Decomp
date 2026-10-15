// FUNC 800e88e4 304 I0NFO3
// MATCHING 800e88e4 304
typedef struct {
    unsigned char pad[3];
    unsigned char n;
    char *p[22];
} PtrList;
extern unsigned short D_8009C962;
extern unsigned char D_800A60A3;

void func_800E88E4(PtrList *l, int *base)
{
    short i, s, end;
    int *tbl;
    char *b;
    register int off asm("$2");
    b = (char *)base;
    if (D_8009C962 == 3) {
        tbl = base + 1;
        if (D_800A60A3 != 0) {
            int v = D_800A60A3;
            if (v < 3) goto lo;
            if (v == 3) goto hi;
            goto def;
            do {
            lo:
                off = base[0x5C / 4];
                goto set;
            hi:
                off = base[0x60 / 4];
            set:
                l->n = 1;
            } while (0);
            l->p[0] = b + off;
            return;
        }
    def:
        s = (*(short *)0x1F800176 - 0x78) / 80;
        end = s + 8;
        if (s < 0) s = 0;
        if (end >= 0x16) end = 0x15;
        for (i = s; i < end; i++) {
            l->p[l->n] = b + tbl[i];
            l->n++;
        }
    }
}

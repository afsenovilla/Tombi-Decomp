// FUNC 800e88e4 304 I0NFO3
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
    int off;
    if (D_8009C962 == 3) {
        tbl = base + 1;
        if (D_800A60A3 != 0) {
            switch (D_800A60A3) {
            case 0 ... 2:
                off = base[0x5C / 4];
                break;
            case 3:
                off = base[0x60 / 4];
                break;
            default:
                goto def;
            }
            l->n = 1;
            l->p[0] = (char *)base + off;
            return;
        }
    def:
        s = (*(short *)0x1F800176 - 0x78) / 80;
        end = s + 8;
        if (s < 0) s = 0;
        if (end >= 0x16) end = 0x15;
        for (i = s; i < end; i++) {
            l->p[l->n] = (char *)base + tbl[i];
            l->n++;
        }
    }
}

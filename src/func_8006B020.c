// FUNC 8006b020 140 MAIN0
// WIP: only diff is library epilogue (jr ra; addiu sp in delay slot, irreproducible) + beq delay-slot fill
typedef struct {
    char pad0[0x37];
    unsigned char b37;
    char pad38[4];
    unsigned char *p3c;
} S;
extern int (*D_800981B4)(S *, int);
extern int func_80069EF8(S *, int);

int func_8006B020(S *s)
{
    int r;
    int a = 0;
    if ((*s->p3c >> 4) == 8) {
        a = s->b37 == 0;
    }
    r = func_80069EF8(s, (unsigned char)D_800981B4(s, a));
    if (r == 0x5a || r == 0) return r; if (r >= 0) return -4; return r;
}

// FUNC 800692e8 168 MAIN0
// only diff: epilogue jr ra/addiu sp (s-regs saved, guide: not reproducible)
typedef struct { unsigned char n; char p[3]; unsigned char *v; } E;
typedef struct { char p[8]; E *e; char q[0xea - 0xc]; unsigned char n; } T;
extern T *(*D_800981C0)();
int func_800692E8(int a, int i, int j)
{
    T *t = D_800981C0();
    E *e;
    if (i < 0) return t->n;
    if (i < t->n) {
        e = &t->e[i];
        if (j < 0) return e->n;
        if (j < e->n) return e->v[j];
        return 0;
    }
    return 0;
}

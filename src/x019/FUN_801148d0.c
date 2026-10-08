// FUNC 801148d0 152 X019
// MATCHING 801148d0 152
extern char *FUN_800183b8(void);
typedef struct N { char t, p1, k, b; char pad[0x10]; int y; char pad2[0x28]; int *x; int *z; char pad3[0x48]; int a; } N;
void FUN_801148d0(int a, char b, int c, int d, short e)
{
    N *n = (N *)FUN_800183b8();
    if (n != 0) {
        n->t = 1; n->k = 0x1f; n->b = b; *n->x = c << 16; n->y = d << 16; *n->z = e << 16; n->a = a;
    }
}

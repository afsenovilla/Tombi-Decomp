// VERSION SLPM_80165 slpm80165 80046C6C 144
#if defined(VERSION_SLPM_80165)
// Demo-only helper: draws the HUD block D_800B0508 entries with the scratchpad matrix.
typedef struct { unsigned char b0, b1, b2, n; int *e[1]; } H;
extern H D_800B0508;
extern char D_1F8000C0[];
extern int D_1F8001E0;
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void func_80023EC8(int *, int);
void func_80046C6C_slpm80165(void)
{
    int i;
    H *h;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0);
    h = &D_800B0508;
    for (i = 0; i < h->n; i++)
        func_80023EC8(h->e[i], D_1F8001E0 + 16);
}
#endif

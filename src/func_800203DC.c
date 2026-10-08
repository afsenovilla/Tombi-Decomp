// FUNC 800203dc 108 MAIN0
// MATCHING 800203dc 108
typedef struct P { short x; unsigned short y; } P;
typedef struct S { unsigned char on; char p[0x15]; unsigned short z; char q[0x28]; P *pos; } S;
static __inline__ int vis(void *q)
{
    S *p = q;
    if (p->on == 0) return 0;
    if ((unsigned short)(p->pos->y - *(unsigned short *)0x1F800176 + 0x40) >= 0x1c1) return 0;
    return (unsigned short)(*(unsigned short *)0x1F800186 - p->z + 0x40) < 0x171;
}
int func_800203DC(S *o)
{
    return vis(o);
}

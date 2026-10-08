// FUNC 800203dc 108 MAIN0
typedef struct P { short x; unsigned short y; } P;
typedef struct S { unsigned char on; char p[0x15]; unsigned short z; char q[0x28]; P *pos; } S;
/* score 10: game keeps a separate "return 0" block (j end; v0=0) and copies o to a1 */
int func_800203DC(S *o)
{
    if (o->on == 0 || (unsigned short)(o->pos->y - *(unsigned short *)0x1F800176 + 0x40) >= 0x1c1)
        return 0;
    return (unsigned short)(*(unsigned short *)0x1F800186 - o->z + 0x40) < 0x171;
}

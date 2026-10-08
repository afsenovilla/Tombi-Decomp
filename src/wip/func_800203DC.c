// FUNC 800203dc 108 MAIN0
typedef struct P { short x; unsigned short y; } P;
typedef struct S { unsigned char on; char p[0x15]; unsigned short z; char q[0x28]; P *pos; } S;
/* score 10: game keeps a separate "return 0" block (j end; v0=0) and copies o to a1 (move a1,a0 in the first beqz slot keeps reorg from filling the return-0 block).
   b27 tried: static inline vis(S*)/onscr(x,y)/nested inlines, int/char*/void* params cast to S*, local p=o copies,
   int/short/u8 result var, short/u8 return type, -O1/-fno-* flags (-fno-delayed-branch 9), extern symbols (16, literals 10). */
int func_800203DC(S *o)
{
    if (o->on == 0 || (unsigned short)(o->pos->y - *(unsigned short *)0x1F800176 + 0x40) >= 0x1c1)
        return 0;
    return (unsigned short)(*(unsigned short *)0x1F800186 - o->z + 0x40) < 0x171;
}

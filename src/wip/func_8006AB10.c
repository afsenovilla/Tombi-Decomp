// FUNC 8006ab10 104 MAIN0
/* score 5: epilogue only (game: jr ra; addiu sp in delay slot with s0/s1 saved) = library code built by a newer compiler. With ncheck OLDGCC=/opt/oldgcc/gcc-2.8.1-psx score 2 (only li 1 / move v1 CSE differs). Not reproducible with CC1PSX 4.3. */
typedef struct { char p0[0x14]; void *f14; void *f18; char p1[4]; int f20; char p2[0x22]; unsigned char b46; } S;
extern int (*D_800981C8)();
extern void func_8006AB78();
extern void func_8006AB94();

int func_8006AB10(S *s, int x)
{
    if (D_800981C8(s))
        return 0;
    s->b46 = 1;
    s->f14 = func_8006AB78;
    s->f20 = x;
    s->f18 = func_8006AB94;
    return 1;
}

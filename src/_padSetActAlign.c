// FUNC 8006ab10 104 MAIN0
// MATCHING 8006ab10 104
// FLAGS -O2 -G0
// AS gnu
/* pad object setup (libpad/padif.c family). Library code: GNU as fills the jr delay slot. */
typedef struct { char p0[0x14]; void *f14; void *f18; char p1[4]; int f20; char p2[0x22]; unsigned char b46; } S;
extern int (*D_800981C8)();
extern void padSetCmdActAlign();
extern void padResolveActAlign();

int _padSetActAlign(S *s, int x)
{
    if (D_800981C8(s))
        return 0;
    s->b46 = 1;
    s->f14 = padSetCmdActAlign;
    s->f20 = x;
    s->f18 = padResolveActAlign;
    return 1;
}

// FUNC 8004f490 168 MAIN0
// MATCHING 8004f490 168
extern int *PTR_8007dea0[];
extern int g298, g2a4, g2b0;
extern void SfxStopAll(int);
extern void FUN_8004f538(int);
void FUN_8004f490(int a, int b, int c)
{
    int *p;
    if (c == 0) {
        SfxStopAll(1);
    } else {
        p = PTR_8007dea0[a];
        FUN_8004f538(p[0]);
        g2a4 = g298;
    }
    p = PTR_8007dea0[a];
    FUN_8004f538(p[b + 1]);
    g2b0 = g298;
}

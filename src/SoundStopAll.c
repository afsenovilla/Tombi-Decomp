// FUNC 8001f4bc 252 MAIN0
// MATCHING 8001f4bc 252
extern short VA, VB;
extern short X1[], Y1[];
extern short Z1, Z2, Z3, Z4, Z5;
extern void g1(int a);
extern void g2(int a);
extern void g3(int a, int b);

void SoundStopAll(void)
{
    int i;
    if (VA != -1) {
        g1(VA);
        g2(VA);
        VA = -1;
    }
    if (VB != -1) {
        g1(VB);
        g2(VB);
        VB = -1;
    }
    Z1 = 0;
    Z2 = 0;
    for (i = 0; i < 24; i++) {
        Y1[i] = 0xf;
        X1[i] = -1;
    }
    Z3 = 0;
    Z4 = 0;
    Z5 = 0;
    g3(0, 0xffffff);
}

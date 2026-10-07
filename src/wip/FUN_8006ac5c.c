// FUNC 8006ac5c 152 MAIN0
typedef struct { char p0[0x14]; int w14, w18; char p1[0x46 - 0x1c]; char b46; char p2[0x51 - 0x47]; char b51, b52, b53; char p3[0xe4 - 0x54]; unsigned char be4; } TO;
extern int (*FP)(TO *o, int a, int b);
extern char A14[], A18[];

int FUN_8006ac5c(TO *o, int a, int b)
{
    int t = a;
    if (FP(o, a, b) == 0) {
        o->b46 = 1;
        o->w14 = (int)A14;
        o->w18 = (int)A18;
        o->b51 = a;
        o->b52 = b;
        o->b53 = ((t & 0xff) == o->be4);
        return 1;
    }
    return 0;
}

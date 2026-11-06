// VERSION SLPS_01144 x00 800F26AC 256
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct P { char pad[8]; unsigned char b8; char pad2[0x17]; unsigned short w20; } P;
typedef struct O {
    char p0[3]; unsigned char b3;
    char p1[0x78]; short w7c; short w7e;
    char p2[0x0c]; int i8c;
    char p3[0x0c]; unsigned char b9c;
    char p4[8]; unsigned char ba5;
    char p5[1]; unsigned char ba7;
    char p6[4]; unsigned char bac;
    char p7[3]; short wb0; short wb2;
} O;
extern P *D_8009DEE0;
extern unsigned char D_8009EAAE_[] asm("D_8009EAAE");
#define D_8009EAAE D_8009EAAE_[0]
extern unsigned char D_8009EA09;
extern unsigned char D_801145D4[];
extern void func_8001E458(int, int);
extern void func_8001C988(int, int);

void func_800F26AC_slps01144(O *o)
{
    o->ba7 = 0;
    D_8009DEE0->b8 = 0;
    func_8001E458(28, 127);
    o->b9c = 0;
    o->bac = 0;
    if (o->w7c > 0) {
        o->w7c -= 208;
        if (o->w7c < 0) o->w7c = 0;
    } else {
        o->w7c += 208;
        if (o->w7c > 0) o->w7c = 0;
    }
    o->wb2 = o->w7c;
    o->w7c = 0;
    o->w7e = 0;
    {
        int t = D_801145D4[o->wb0];
        o->ba5 = 0;
        o->i8c = t;
    }
    D_8009DEE0->w20 = 0;
    if (D_8009EAAE != 0 && o->b3 != 1) {
        o->b3 = 1;
        if (D_8009EA09 != 0xff) func_8001C988(189, 1);
    }
}
#endif

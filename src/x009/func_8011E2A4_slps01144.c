// VERSION SLPS_01144 x09 8011E2A4 260
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct O { unsigned char b0, b1, b2, b3, b4; } O;
extern unsigned char D_8009ECC1;
extern unsigned char D_80077350[];
extern void (*D_800773F0[])(O *);
extern void func_8003D740(O *);
extern int func_8002122C(O *);
extern void func_80020E38(O *);
extern void func_80017EB4(O *);

void func_8011E2A4_slps01144(O *o)
{
    switch (o->b4) {
    case 0:
        if (D_8009ECC1 != 0) {
            o->b4 = 3;
        } else {
            func_8003D740(o);
            o->b0 = 2;
            o->b4++;
        }
        break;
    case 1:
        if (func_8002122C(o) != 0) {
            func_80020E38(o);
        }
        break;
    case 2:
        D_800773F0[D_80077350[o->b3]](o);
        break;
    case 3:
        func_80017EB4(o);
        break;
    }
}
#endif

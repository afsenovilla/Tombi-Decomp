// VERSION SLPS_01144 x09 80119758 436
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    unsigned char p08[2];
    unsigned char b0a;
    unsigned char p0b[2];
    unsigned char b0d;
    unsigned char p0e;
    unsigned char b0f;
    unsigned char p10[2];
    short w12;
    unsigned char p14[2];
    short w16;
    unsigned char p18[2];
    short w1a;
    unsigned char p1c[2];
    short w1e;
    unsigned char p20[0x0e];
    short w2e;
    unsigned char p30[0x0c];
    int d3c;
    unsigned char p40[0x1c];
    int d5c, d60, d64;
    unsigned char p68[0x1c];
    int d84, d88, d8c;
} O;
extern unsigned char D_800A7B78_[] asm("D_800A7B78");
#define D_800A7B78 D_800A7B78_[0]
extern unsigned char D_8009E538, D_8009EE5B;
extern unsigned char D_8009EAAE_[] asm("D_8009EAAE");
#define D_8009EAAE D_8009EAAE_[0]
extern unsigned long *D_8012A2F0, *D_8012A304;
extern unsigned long *D_8012A2E0[], *D_8012A2F4[];
extern void *D_8012DDD0;
extern void func_80016B94(unsigned long *, int, int, int, int);
extern void func_8011920C_slps01144(O *);
extern void func_80020DE4(O *);
extern void func_80020E38(O *);
extern void func_80018278(O *);
extern void func_80017DB8(O *);

void func_80119758_slps01144(O *o)
{
    switch (o->b04) {
    case 0:
        D_800A7B78 = 5;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b0a = 0;
        o->b0f = 0;
        o->w2e = 0;
        o->d5c = o->w12;
        o->d60 = o->w16;
        o->d64 = o->w1a;
        if (D_8009EAAE) {
            func_80016B94(D_8012A2F0, 0x90, 0x1f0, 0x10, 1);
            func_80016B94(D_8012A304, 0x90, 0x1f1, 0x10, 1);
        } else {
            func_80016B94(D_8012A2E0[D_8009E538], 0x90, 0x1f0, 0x10, 1);
            func_80016B94(D_8012A2F4[D_8009EE5B], 0x90, 0x1f1, 0x10, 1);
        }
        o->w1e = 1;
        o->b0d = 0x80;
        o->b04++;
        o->d3c = ((int *)&SPAD.i2e0)[0];
        *(void **)&o->p20[4] = D_8012DDD0;
        func_80020DE4(o);
        break;
    case 1:
        func_8011920C_slps01144(o);
        func_80020E38(o);
        o->visible = 1;
        func_80018278(o);
        break;
    case 2:
    case 3:
        func_80017DB8(o);
        break;
    }
}
#endif

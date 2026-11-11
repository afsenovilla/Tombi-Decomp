// VERSION PCPX_96084 tomba 80079B68 488
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Chick object handler (states: set-up and wait until in view, drop / wander, caught: counter + area flag, free).
// The constant 6 lives in v1 from the dispatch delay slot on: a register variable is the only way found to get it.
typedef struct {
    char p0[1]; unsigned char vis; char p1[2]; unsigned char st, step; char p2[4]; unsigned short b0a; char p3[1]; unsigned char b0d; char p4; unsigned char b0f;
    char p5[0x1e - 0x10]; unsigned short w1e; char p6[2]; int anim; char p7[0x3c - 0x28]; int d3c; char p8[0x68 - 0x40]; unsigned char b68, b69, p9, b6b;
    unsigned short w6c, w6e, w70, w72;
} O;
extern volatile int D_1F80015C, D_80014050;
extern unsigned short D_800DE31C[];
extern void AnimLoadDuration(O *), func_80018550(O *), func_8007999C_pcpx96084(O *), func_80079698_pcpx96084(O *), func_800180D8_pcpx96084(O *), func_80018E74_pcpx96084(int), freeObjectLayer7(O *);
extern int isObjectInView(O *);

void func_80079B68_pcpx96084(O *o)
{
    unsigned char c = o->st;
    switch (c) {
    case 0:
        { register int k asm("$3") = 6; asm("" : "=r"(k) : "0"(k));
        *(volatile unsigned short *)&o->w6e = 0xc;
        *(volatile unsigned short *)&o->w72 = 0xc;
        *(volatile unsigned short *)&o->w6c = k;
        *(volatile unsigned short *)&o->w70 = k;
        *(volatile unsigned short *)&o->w1e = 0xb; }
        {
            int c = D_1F80015C;
            int a;
            *(volatile unsigned char *)&o->b0d = 0;
            *(volatile unsigned char *)&o->b69 = 0;
            *(volatile unsigned char *)&o->b68 = 0;
            a = D_80014050;
            *(volatile unsigned short *)&o->b0a = 0;
            *(volatile signed char *)&o->b0f = -7;
            *(volatile int *)&o->d3c = c;
            o->anim = a;
        }
        AnimLoadDuration(o);
        if (isObjectInView(o))
            o->st = o->st + 1;
        break;
    case 1:
        func_80018550(o);
        if (o->vis != 0) {
            switch (o->step) {
            case 0:
                func_8007999C_pcpx96084(o);
                break;
            case 1:
                func_80079698_pcpx96084(o);
                break;
            }
        }
        break;
    case 2:
        func_80018550(o);
        if (o->vis != 0) {
            switch (o->step) {
            case 0:
                o->step = 1;
                break;
            case 1:
                func_800180D8_pcpx96084(o);
                break;
            case 2:
                D_800DE31C[0]++;
                func_80018E74_pcpx96084(o->b6b);
                o->st = 3;
                break;
            }
        }
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}

#endif

// VERSION PCPX_96084 tomba 80031B4C 276
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of JP x000 updateBubbleDroplet (src/updateBubbleDroplet.c): a constant in place of the random call, halfword clear at +0x0A, scratchpad word 0x1F80015C.
typedef struct P { char p[0x24]; void *anim; char q[0x3c - 0x28]; int w3c; } P;
extern int D_1F80015C;
extern char D_800145F0[];
extern int func_80018550(char *);
extern void func_80018704_pcpx96084(char *);
extern int func_800180D8_pcpx96084(char *);
extern void AnimLoadDuration(char *);
extern void func_8001A87C_pcpx96084(char *);

void func_80031B4C_pcpx96084(char *o)
{
    switch ((unsigned char)o[4]) {
    case 0:
        o[4]++;
        *(short *)(o + 0x1e) = 10;
        *(short *)(o + 8) = 0x7d69;
        o[0xd] = 1;
        *(short *)(o + 0xa) = 0;
        o[3] = 0;
        *(signed char *)&o[0xf] = -13;
        *(short *)(o + 0x2e) = 0;
        ((P *)o)->anim = D_800145F0;
        ((P *)o)->w3c = D_1F80015C;
        AnimLoadDuration(o);
        break;
    case 1:
        if (func_80018550(o) == 0)
            func_80018704_pcpx96084(o);
        if (func_800180D8_pcpx96084(o) != 0)
            o[4]++;
        break;
    case 2:
        o[4]++;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif

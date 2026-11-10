// VERSION PCPX_96084 tomba 8001FCDC 200
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of gameInitState (src/gameInitState_beta980203.c): a word clear, state 2 always goes to mode 1.
typedef struct { short x, y, w, h; } RECT;
#define TASK_C D_800DE0B0
extern char *TASK_C;
extern int D_800E43D4;
extern void ClearImage(RECT *r, int a, int b, int c);
extern void func_80019910(void);
extern void func_80025944_pcpx96084(void);

void func_8001FCDC_pcpx96084(void)
{
    char *s = TASK_C;
    unsigned short u = *(unsigned short *)(s + 0x4a);
    short *q;
    short v;
    switch (u) {
    case 0:
        q = (short *)(s + 0x4a);
        *(short *)(s + 0x5a) = 1;
        *(short *)(s + 0x4a) = *q + 1;
        func_80019910();
        func_80025944_pcpx96084();
        D_800E43D4 = 0;
        break;
    case 1:
        u = *(short *)(s + 0x5a) - 1;
        *(unsigned short *)(s + 0x5a) = u;
        if ((int)((unsigned)u << 16) < 1)
            *(short *)(s + 0x4a) = *(short *)(s + 0x4a) + 1;
        break;
    case 2:
        *(short *)(s + 0x48) = 1;
        *(short *)(s + 0x4a) = 0;
        *(short *)(s + 0x4c) = 0;
        *(short *)(s + 0x4e) = 0;
    }
}
#endif

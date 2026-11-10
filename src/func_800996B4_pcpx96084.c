// VERSION PCPX_96084 tomba 800996B4 312
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
// libsnd voice reset (the voice loop of _SsVmInit, src/_SsVmInit.c): every voice up to the voice count gets its
// default state, its SPU registers are reset and it is keyed off. The voice table (0x34-byte entries at 0x800DDB94)
// is declared one field per symbol, as splat names each field address of the target. The current voice
// (_svm_cur.field_0x1a in PAL) is a one-field struct: a plain scalar store is moved above the register stores.
typedef struct { short v; char p[0x32]; } VH;
typedef struct { char v; char p[0x33]; } VB;
extern unsigned char D_800E07A0;
extern VH D_800DDB94[], D_800DDB96[], D_800DDB98[], D_800DDB9A[], D_800DDBA2[], D_800DDBA4[], D_800DDBA6[];
extern VH D_800DDBA8[];
extern VB D_800DDBAF[];
extern unsigned short *D_800C7708;
extern struct { short v; } D_800E2562;
extern void _SsVmKeyOffNow(int);

void func_800996B4_pcpx96084(void)
{
    short i;
    for (i = 0; i < D_800E07A0; i++) {
        short r = i * 8;
        D_800DDB96[i].v = 24;
        D_800DDB94[i].v = 255;
        D_800DDBAF[i].v = 0;
        D_800DDB98[i].v = 0;
        D_800DDB9A[i].v = 0;
        D_800DDBA2[i].v = 255;
        D_800DDBA4[i].v = 0;
        D_800DDBA6[i].v = 0;
        D_800DDBA8[i].v = 255;
        D_800C7708[r + 3] = 0x200;
        D_800C7708[r + 2] = 0x1000;
        D_800C7708[r + 4] = 0x80ff;
        D_800C7708[r + 0] = 0;
        D_800C7708[r + 1] = 0;
        D_800C7708[r + 5] = 0x4000;
        D_800E2562.v = i;
        _SsVmKeyOffNow(1);
    }
}
#endif

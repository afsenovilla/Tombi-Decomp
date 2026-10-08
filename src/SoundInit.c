// FUNC 8001f6b4 364 MAIN0
// MATCHING 8001f6b4 364
typedef struct { int mask; short pad0; short pad1; short mvl, mvr; short pad2[4]; short cdvl, cdvr; short pad3[2]; int cdmix; } SC;
extern char DAT_80078788;
extern void SoundShutdown(void);
extern void FUN_8006c544(void);
extern void FUN_800702c0(void *, int, int);
extern void FUN_800704a0(int);
extern void FUN_800745a8(int);
extern void FUN_80077064(int, int);
extern void FUN_800745dc(int);
extern void FUN_80077404(void *);
extern void FUN_8006eed8(void);
extern char DAT_800a2270[];
extern int DAT_8009bce0;
extern short DAT_8009bce4, DAT_8009bce6, DAT_8009bcf0, DAT_8009bcf2;
extern int DAT_8009bcf8;
extern short DAT_8009f2d0, DAT_800a3f90, DAT_8009d688, DAT_8009bd10, DAT_8009bd14, DAT_8009bd2c;
extern short DAT_8009c8c0[], DAT_800a3cc8[];
extern short DAT_1f8003a8[];
extern short DAT_800a3428, DAT_8009f0c8;

void SoundInit(void)
{
    int i;
    short m;
    int *s;
    short *q, *p;
    if (DAT_80078788 != 0)
        SoundShutdown();
    FUN_8006c544();
    FUN_800702c0(DAT_800a2270, 4, 1);
    FUN_800704a0(1);
    FUN_800745a8(0);
    FUN_80077064(0, 0xffffff);
    FUN_800745dc(0x10);
    s = &DAT_8009bce0;
    *s = 0x2c3;
    DAT_8009bce4 = 0x3fff;
    DAT_8009bce6 = 0x3fff;
    DAT_8009bcf0 = 0x7fff;
    DAT_8009bcf2 = 0x7fff;
    DAT_8009bcf8 = 1;
    FUN_80077404(s);
    FUN_8006eed8();
    i = 0;
    DAT_8009f2d0 = 0;
    DAT_800a3f90 = 0;
    DAT_8009d688 = 0;
    DAT_8009bd10 = 0;
    DAT_8009bd14 = 0;
    DAT_8009bd2c = 0;
    do {
        DAT_8009c8c0[i] = 0xf;
        DAT_800a3cc8[i] = -1;
        i++;
    } while (i < 0x18);
    m = -1;
    i = 7;
    do {
        DAT_1f8003a8[i] = m;
        i--;
    } while (i >= 0);
    DAT_800a3428 = -1;
    DAT_8009f0c8 = -1;
    DAT_80078788 = 1;
}

// VERSION SLPM_80165 slpm80165 80019384 332
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only front-end mode 0 (see func_80018BBC): display off, clear colour (0xFF, 0xFF, 0xFF), load list 1, CD seek
// to file 0 (func_8001D114), then show the loaded screen (func_800E78E8 / func_800E78F0) for 300 frames and
// switch to mode 1. Same shape as mode 1 (func_80018D8C).
typedef struct { char p0[0x48]; unsigned short w48, w4a; char p4c[0x58 - 0x4c]; short w58; } Task;
extern Task *D_1F8001D4;
extern unsigned char D_1F8001CE;
extern void SetDispMask(int);
extern void func_80015A98(int, int, int);
extern void func_8001FEDC(int);
extern void func_8001D114(short);
extern void func_800E78E8(void), func_800E78F0(void);

void func_80019384(void)
{
    switch (D_1F8001D4->w4a) {
    case 0:
        SetDispMask(0);
        func_80015A98(0xff, 0xff, 0xff);
        D_1F8001CE = 0;
        func_8001FEDC(1);
        D_1F8001D4->w4a++;
        break;
    case 1:
        if (D_1F8001CE == 0)
            break;
        D_1F8001D4->w4a++;
        func_8001D114(0);
        break;
    case 2:
        func_800E78E8();
        SetDispMask(1);
        D_1F8001D4->w58 = 300;
        D_1F8001D4->w4a++;
        break;
    case 3:
        func_800E78F0();
        if (--D_1F8001D4->w58 > 0)
            break;
        D_1F8001D4->w4a++;
        break;
    case 4:
        D_1F8001D4->w48 = 1;
        D_1F8001D4->w4a = 0;
        break;
    }
}
#endif

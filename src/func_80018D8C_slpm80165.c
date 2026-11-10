// VERSION SLPM_80165 slpm80165 80018D8C 316
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only front-end mode 1 (see func_80018BBC): display off, clear colour (0xF0, 0xF0, 0xF0), load list 1, then
// show the loaded screen (func_800E7920 / func_800E7928) for 300 frames and switch to mode 2.
typedef struct { char p0[0x48]; unsigned short w48, w4a; char p4c[0x58 - 0x4c]; short w58; } Task;
extern Task *D_1F8001D4;
extern unsigned char D_1F8001CE;
extern void SetDispMask(int);
extern void func_80015A98(int, int, int);
extern void func_8001FEDC(int);
extern void func_800E7920(void), func_800E7928(void);

void func_80018D8C(unsigned int *flag)
{
    switch (D_1F8001D4->w4a) {
    case 0:
        *flag = 0;
        SetDispMask(0);
        func_80015A98(0xf0, 0xf0, 0xf0);
        D_1F8001CE = 0;
        func_8001FEDC(1);
        D_1F8001D4->w4a++;
        break;
    case 1:
        if (D_1F8001CE == 0)
            break;
        D_1F8001D4->w4a++;
        break;
    case 2:
        func_800E7920();
        SetDispMask(1);
        D_1F8001D4->w58 = 300;
        D_1F8001D4->w4a++;
        break;
    case 3:
        func_800E7928();
        if (--D_1F8001D4->w58 > 0)
            break;
        D_1F8001D4->w4a++;
        break;
    case 4:
        D_1F8001D4->w48 = 2;
        D_1F8001D4->w4a = 0;
        break;
    }
}
#endif

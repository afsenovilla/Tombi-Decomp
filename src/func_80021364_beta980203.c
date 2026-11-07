// VERSION BETA_980203 beta980203 80021364 300
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_8001FE6C_slps01144.c (func_8001FE6C_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
typedef struct { short x, y, w, h; } RECT_;
extern unsigned short D_801FD8E0_beta980203;
typedef struct { int a, b; } E8;
extern E8 D_800725F4_beta980203[];
extern int D_800725F8_beta980203[];
extern int D_1F800290;
extern volatile unsigned D_1F800294;
#define DSTV D_1F800290
#define NSEC D_1F800294
int CdControl(int, void *, int);
int CdRead(int, void *, int);
int CdReadSync(int, void *);
int LoadImage(RECT_ *, void *);

int func_80021364_beta980203(void *dst, short *hdr)
{
    RECT_ r;
    if (D_801FD8E0_beta980203 != 0) return -1;
    {
        E8 *p = &D_800725F4_beta980203[hdr[0]];
        DSTV = (int)dst;
        if (CdControl(2, p, 0)) {
            NSEC = (unsigned)(D_800725F8_beta980203[hdr[0] * 2] + 2047) >> 11;
            CdRead(NSEC, (void *)DSTV, 0x80);
        }
        if (CdReadSync(0, 0) == -1) return -1;
        {
            if ((((unsigned char *)hdr)[3] & 0xf0) == 0x10) {
                r.x = hdr[4];
                r.y = hdr[5];
                r.w = hdr[6];
                r.h = hdr[7];
                LoadImage(&r, dst);
            }
            return D_800725F8_beta980203[hdr[0] * 2];
        }
    }
}
#endif

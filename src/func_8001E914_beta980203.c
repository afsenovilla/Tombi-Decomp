// VERSION BETA_980203 beta980203 8001E914 784
// FLAGS.BETA_980203 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_8001D5E0_slps01144.c (func_8001D5E0_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
// PAL func_8001E914_beta980203(psx_tomba movie.c name); PAL adds the 0x800000 timeout loop and DecDCTinSync.
// The &MOVIE_DEC_ENV / &f34 temps are compiler-hoisted invariants: no pointer locals (loop.c does not hoist user
// variables set after a branch and used across basic blocks); volatile f34 accesses give the second temp.
#include "spad.h"
#include "globals.h"
typedef struct { unsigned short w; unsigned short h; int pad; } Ent;
typedef struct {
    int *in[2];
    int idx;
    int *out[2];
    int oidx;
    Ent ent[2];
    int f28;
    unsigned short w2c;
    unsigned short w2e;
    short w30;
    short w32;
    int f34;
} M;
typedef struct { char p[0x48]; unsigned short w48; char p3[4]; unsigned short w4e; char p2[0x18]; unsigned char b68; } T;
typedef struct { char d[8]; } E8;
extern M MOVIE_DEC_ENV;
extern volatile int D_8009DB54_beta980203;
extern int MOVIE_DEC_IMGID;
extern Ent D_8009DB38_beta980203[];
extern Ent D_8009DB3A_beta980203[];
extern short *D_80071D70_beta980203[];
extern E8 D_800725F4_beta980203[];
extern unsigned char MOVIE_SKIP_REQUEST;
extern short FRAME_BUFFER_INDEX;
extern T *TASK_C;
extern char D_80071D6C_beta980203[];
extern void CdMix(void *);
extern void movieInitDecodeEnv(M *, int, int, int, int);
extern void func_8001DA50_beta980203(E8 *);
extern int func_8001D414_beta980203(M *);
extern void DecDCTin(int *, int);
extern void DecDCTout(int *, int);
extern void SetDispMask(int);
extern void DecDCToutCallback(void *);
extern void StUnSetRing(void);
extern void StClearRing(void);
extern int CdControlB(int, int, int);
extern void exitTask(void);
extern void ThreadWaitFrames(int);

void func_8001E914_beta980203(void)
{
    int k;
    M *e;
    SPAD.b1cc = 1;
    TASK_C->w48 = 0;
    TASK_C->w4e = 0;
    TASK_C->b68 = 0;

    for (;;) {
        if (MOVIE_SKIP_REQUEST == 1) {
            TASK_C->w48 = 3;
            CdMix(D_80071D6C_beta980203);
        }
        switch (TASK_C->w48) {
        case 0:
            movieInitDecodeEnv(&MOVIE_DEC_ENV, 0x180, 0x100, 0x2c0, 0x100);
            func_8001DA50_beta980203(&D_800725F4_beta980203[*D_80071D70_beta980203[SPAD.b1cd]]);
            TASK_C->w48++;
            while (func_8001D414_beta980203(&MOVIE_DEC_ENV) == 0)
                ;
            break;
        case 1:
            SPAD.b1cc = 2;
            TASK_C->w48 = 2;
        case 2:
            while (TASK_C->w4e == 0)
                func_8001D414_beta980203(&MOVIE_DEC_ENV);
            DecDCTin(MOVIE_DEC_ENV.in[MOVIE_DEC_ENV.idx], 2);
            k = FRAME_BUFFER_INDEX;
            MOVIE_DEC_ENV.f28 = k;
            MOVIE_DEC_ENV.w2c = D_8009DB38_beta980203[k].w;
            MOVIE_DEC_ENV.w2e = D_8009DB3A_beta980203[k].w;
            DecDCTout(MOVIE_DEC_ENV.out[MOVIE_DEC_IMGID], MOVIE_DEC_ENV.w30 * MOVIE_DEC_ENV.w32 / 2);
            TASK_C->w4e = 0;
            while (!func_8001D414_beta980203(&MOVIE_DEC_ENV)) {
                if (D_8009DB54_beta980203 == 1) break;
            }
            if (D_8009DB54_beta980203 == 0) {
                int *p = &MOVIE_DEC_ENV.f34;
                do {
                } while (*p == 0);
            }
            SetDispMask(1);
            D_8009DB54_beta980203 = 0;
            SPAD.b1cc = 3;
            SPAD.s1e8 = 0;
            break;
        case 3:
            DecDCToutCallback(0);
            StUnSetRing();
            StClearRing();
            CdControlB(9, 0, 0);
            SPAD.b1cc = 0;
            MOVIE_SKIP_REQUEST = 0;
            exitTask();
            break;
        }
        ThreadWaitFrames(1);
    }
}
#endif

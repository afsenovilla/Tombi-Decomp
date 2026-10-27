// FUNC 8001d6a4 820 MAIN0
// MATCHING 8001d6a4 820
// PAL moviePlayerTask (psx_tomba movie.c name); PAL adds the 0x800000 timeout loop and DecDCTinSync.
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
typedef struct { char p[0x48]; unsigned short w48; char p2[0x1e]; unsigned char b68; } T;
typedef struct { char d[8]; } E8;
extern M MOVIE_DEC_ENV;
extern volatile int D_8009BCDC;
extern unsigned char MOVIE_SKIP_REQUEST;
extern T *TASK_C;
extern short FRAME_BUFFER_INDEX;
extern char D_80077F0C[];
extern unsigned char D_80077F3C[];
extern E8 FILE_LINKS[];
extern void CdMix(void *);
extern void movieInitDecodeEnv(M *, int, int, int, int);
extern void startMovieStream(E8 *);
extern int MovieDecodeFrame(M *);
extern void DecDCTin(int *, int);
extern void DecDCTout(int *, int);
extern void SetDispMask(int);
extern void DecDCTinSync(int);
extern void DecDCToutCallback(void *);
extern void StUnSetRing(void);
extern void StClearRing(void);
extern int CdControlB(int, int, int);
extern void exitTask(void);
extern void ThreadWaitFrames(int);

void moviePlayerTask(void)
{
    int k;
    int cnt;
    SPAD.b1cc = 1;
    TASK_C->w48 = 0;
    TASK_C->b68 = 0;
    for (;;) {
        if (MOVIE_SKIP_REQUEST == 1) {
            TASK_C->w48 = 3;
            CdMix(D_80077F0C);
        }
        switch (TASK_C->w48) {
        case 0:
            movieInitDecodeEnv(&MOVIE_DEC_ENV, 0x180, 0x110, 0x2c0, 0x110);
            startMovieStream(&FILE_LINKS[MOVIE_FILE_IDS[D_80077F3C[SPAD.b1cd]]]);
            TASK_C->w48++;
            while (MovieDecodeFrame(&MOVIE_DEC_ENV) == -1)
                ;
        case 1:
            SPAD.b1cc = 2;
            TASK_C->w48 = 2;
        case 2:
            DecDCTin(MOVIE_DEC_ENV.in[MOVIE_DEC_ENV.idx], 2);
            k = FRAME_BUFFER_INDEX;
            MOVIE_DEC_ENV.f28 = k;
            MOVIE_DEC_ENV.w2c = MOVIE_DEC_ENV.ent[k].w;
            MOVIE_DEC_ENV.w2e = MOVIE_DEC_ENV.ent[k].h;
            DecDCTout(MOVIE_DEC_ENV.out[MOVIE_DEC_ENV.oidx], MOVIE_DEC_ENV.w30 * MOVIE_DEC_ENV.w32 / 2);
            while (MovieDecodeFrame(&MOVIE_DEC_ENV) == -1)
                ;
            if (*(volatile int *)&MOVIE_DEC_ENV.f34 == 0) {
                cnt = 0x800000;
                do {
                    cnt--;
                    if (cnt == 0) {
                        *(volatile int *)&MOVIE_DEC_ENV.f34 = 1;
                        MOVIE_DEC_ENV.f28 = FRAME_BUFFER_INDEX;
                        MOVIE_DEC_ENV.w2c = MOVIE_DEC_ENV.ent[FRAME_BUFFER_INDEX].w;
                        MOVIE_DEC_ENV.w2e = MOVIE_DEC_ENV.ent[FRAME_BUFFER_INDEX].h;
                    }
                } while (D_8009BCDC == 0);
            }
            SetDispMask(1);
            D_8009BCDC = 0;
            SPAD.b1cc = 3;
            SPAD.s1e8 = 0;
            break;
        case 3:
            DecDCTinSync(0);
            DecDCToutCallback(0);
            StUnSetRing();
            StClearRing();
            while (CdControlB(9, 0, 0) == 0)
                ;
            SPAD.b1cc = 0;
            MOVIE_SKIP_REQUEST = 0;
            exitTask();
            break;
        }
        ThreadWaitFrames(1);
    }
}

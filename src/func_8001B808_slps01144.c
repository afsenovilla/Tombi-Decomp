// VERSION SLPS_01144 slps01144 8001B808 328
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
typedef struct { char p[0x60]; unsigned short w60; short w62; short w64; short w66; } Q;
extern Q *D_1F8001D4;
#define P1D4 D_1F8001D4
extern int LOADING_SCREEN_STATE;
extern char D_80010210[];
extern void drawDebugText(int, int, int, char *);
void func_8001B808_slps01144(void)
{
    Q *q, *q2, *q3, *q4;
    switch (LOADING_SCREEN_STATE) {
    case 0:
        q = P1D4;
        q->w62 = 4;
        q->w64 = 96;
        q->w60 = 0;
        q->w66 = 30;
        drawDebugText(96, 96, 0, D_80010210);
        LOADING_SCREEN_STATE++;
        break;
    case 1:
        q2 = P1D4;
        drawDebugText(q2->w64, 96, (short)q2->w60, D_80010210);
        q3 = P1D4;
        if (--q3->w62 == 0) {
            q3->w62 = 4;
            q3->w60 = (q3->w60 + 2) & 2;
        }
        q4 = P1D4;
        if (q4->w66 == 0) {
            *(unsigned short *)&q4->w64 -= 2;
            if (q4->w64 < -88) q4->w64 = 360;
        } else {
            q4->w66--;
        }
        break;
    }
}
#endif

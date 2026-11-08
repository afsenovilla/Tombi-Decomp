// VERSION SLPS_01144 x10 80115DB8 1216
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gamestate.h"
typedef struct { char p[3]; unsigned char n; int e[1]; } Q;

extern int getScrollOffsetX(void);
extern int D_800A60BC;
extern unsigned char D_8009E50C;

#define PX (*(short*)((char*)&SPAD.i174 + 2))

void func_80115DB8_slps01144(Q *q, unsigned char *base)
{
    short lo, hi, a, b, i; int t, u;
    unsigned char cnt = base[0];
    int *off = (int *)(base + 4);

    switch (GAME.selectedSection) {
    case 0:
    case 4:
        if (D_800A60BC == 0) {
            { register int s asm("$3") = PX + 0x50; t = s / 80; }
            lo = t;
            u = t + 10;
        } else {
            t = (PX + getScrollOffsetX() + 0x50) / 80;
            lo = t;
            u = t + 12;
        }
        hi = u;
        a = t + 0x2c;
        b = t + 0x36;
        if ((short)t < 0) lo = 0;
        if (hi >= 0x2c) hi = 0x2b;
        if (a < 0x2c) a = 0x2c;
        if (b >= cnt) { if (lo) b = cnt - 1; else b = cnt - 1; }
        if (D_8009E50C == 0) {
            for (i = lo; i <= hi; i++) {
                q->e[q->n] = (int)base + off[i];
                q->n++;
            }
        }
        for (i = a; i <= b; i++) {
            q->e[q->n] = (int)base + off[i];
            q->n++;
        }
        break;
    case 1:
    case 5:
        i = 0; while (1) { if (i >= cnt) break; q->e[q->n] = (int)base + off[i]; q->n++; i++; if (i >= 0x40) break; else continue; }
        break;
    case 2:
    case 6:
        if (D_800A60BC == 0) {
            lo = (PX - 0x104) / 80;
            hi = lo + 10;
        } else {
            lo = (PX + getScrollOffsetX() - 0x104) / 80;
            hi = lo + 12;
        }
        if (lo < 0) lo = 0;
        if (hi >= cnt) hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            q->e[q->n] = (int)base + off[i];
            q->n++;
        }
        break;
    case 3:
    case 7:
        if (D_800A60BC == 0) {
            lo = (PX + 0x50) / 80;
            hi = lo + 10;
        } else {
            lo = (PX + getScrollOffsetX() + 0x50) / 80;
            hi = lo + 12;
        }
        if (lo < 0) lo = 0;
        if (hi >= cnt) hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            q->e[q->n] = (int)base + off[i];
            q->n++;
        }
        break;
    }
}
#endif

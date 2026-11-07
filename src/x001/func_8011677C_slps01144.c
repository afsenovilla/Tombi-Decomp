// JP-only (slps01144), written from the disassembly
// VERSION SLPS_01144 x01 8011677C 1072
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
typedef struct { char p[3]; unsigned char n; int e[1]; } Q;

extern unsigned short GAME_SELECTED_SECTION;
extern unsigned char D_8009E94A;
extern int D_800A60BC;
extern int getScrollOffsetX(void);
extern int getScrollOffsetY(void);

#define PX (*(short*)((char*)&SPAD.i174 + 2))

void func_8011677C_slps01144(Q *q, unsigned char *base)
{
    short lo, hi, a, b, i, t;
    int cnt = base[0];
    int *off = (int *)(base + 4);

    switch (GAME_SELECTED_SECTION) {
    case 0:
        if (D_8009E94A) {
            lo = (PX + 0x50) / 80;
            hi = lo + 8;
        } else if (D_800A60BC == 0) {
            lo = (PX + 0x78) / 80;
            hi = lo + 9;
        } else {
            lo = (PX + getScrollOffsetX() + 0x78) / 80;
            hi = lo + 9;
        }
        break;
    case 1:
        if (D_8009E94A) {
            lo = (PX - 0x50) / 80;
            hi = lo + 8;
        } else if (D_800A60BC == 0) {
            lo = (PX + 0x64) / 80;
            hi = lo + 9;
        } else {
            lo = (PX + getScrollOffsetX() + 0x64) / 80;
            hi = lo + 10;
        }
        break;
    case 2:
        if (D_8009E94A) {
            lo = (PX - 0x50) / 80;
            hi = lo + 15;
        } else if (D_800A60BC == 0) {
            lo = (PX - 0x3c) / 80;
            hi = lo + 15;
        } else {
            lo = (PX + getScrollOffsetY() - 0xdc) / 80;
            hi = lo + 15;
        }
        break;
    case 3:
        if (D_8009E94A) {
            lo = PX / 80;
            hi = lo + 9;
        } else if (D_800A60BC == 0) {
            lo = PX / 80;
            hi = lo + 9;
        } else {
            lo = (PX + getScrollOffsetX()) / 80;
            hi = lo + 9;
        }
        break;
    case 4:
        if (D_8009E94A) {
            if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0xf3d) {
                lo = (PX - 0x5dc) / 80;
                hi = lo + 8;
            } else {
                hi = (PX - 0x384) / 80;
                lo = hi - 18;
            }
        } else if (D_800A60BC == 0) {
            lo = (PX - 0x640) / 80;
            hi = lo + 12;
        } else {
            lo = (PX + getScrollOffsetX() - 0x5dc) / 80;
            hi = lo + 12;
        }
        break;
    }
    if (lo < 0) lo = 0;
    if (hi >= cnt) hi = cnt - 1;
    for (i = lo; i <= hi; i++) {
        q->e[q->n] = (int)base + off[i];
        q->n++;
    }
}
#endif

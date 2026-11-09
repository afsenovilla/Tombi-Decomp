// VERSION SLPS_01144 x04 80117938 2768
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
typedef struct { char pad[3]; unsigned char n; unsigned char *p[1]; } L;
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned char D_8009E94A;
extern unsigned char D_8012EDCC[][20];
extern unsigned char D_8012EED0[];
extern unsigned char D_8012EEE8[][20];
extern unsigned char D_8012EEDC[];
#define SX (*(short*)&SPAD.s17e)
#define SY (*(short*)((char*)&SPAD.i174 + 2))

void func_80117938(L *o, unsigned char *base)
{
    int *tbl = (int *)(base + 4);
    short r0, c0, col, r, c;
    short id;
    int rb;

    switch (GAME_SELECTED_SECTION) {
    case 0:
        if (D_8009E94A) {
            c0 = (SX - 90) / 90 - 4;
            if (c0 < 0) c0 = 0;
            if (c0 >= 20) c0 = 19;
            r0 = (SY - 450) / 90 - 2;
            if (r0 < 0) r0 = 0;
            if (r0 >= 13) r0 = 12;
            for (r = r0; r < r0 + 8 && r < 13; r++) {
                for (c = c0, col = c; c < D_8012EED0[col] + 2; c++) {
                    id = D_8012EDCC[r][c];
                    if (id != 255) {
                        o->p[o->n] = base + tbl[id];
                        o->n++;
                        if (o->n >= 64) return;
                    }
                }
            }
        } else {
            c0 = (SX - 90) / 90 - 2;
            if (c0 < 0) c0 = 0;
            if (c0 >= 20) c0 = 19;
            r0 = (SY - 450) / 90;
            if (r0 < 0) r0 = 0;
            if (r0 >= 13) r0 = 12;
            for (r = r0; r < r0 + 8 && r < 13; r++) {
                for (c = c0, col = c; c < D_8012EED0[col]; c++) {
                    id = D_8012EDCC[r][c];
                    if (id != 255) {
                        o->p[o->n] = base + tbl[id];
                        o->n++;
                        if (o->n >= 64) return;
                    }
                }
            }
        }
        break;
    case 1:
        if (D_8009E94A) {
            r0 = (SY - 90) / 90;
            if (r0 < 0) r0 = 0;
            if (r0 >= 20) r0 = 19;
            c0 = 14 - (1350 - SX) / 90;
            if (c0 < 0) c0 = 0;
            if (c0 >= 13) c0 = 12;
            for (c = r0; c < r0 + 10 && c < 20; c++) {
                col = c;
                for (r = c0, rb = r; r > rb - 8 && r > 0; r--) {
                    id = D_8012EDCC[r][col];
                    if (id != 255) {
                        o->p[o->n] = base + tbl[id];
                        o->n++;
                        if (o->n >= 64) return;
                    }
                }
            }
        } else {
            r0 = (SY - 90) / 90 - 1;
            if (r0 < 0) r0 = 0;
            if (r0 >= 20) r0 = 19;
            c0 = 12 - (1350 - SX) / 90;
            if (c0 < 0) c0 = 0;
            if (c0 >= 13) c0 = 12;
            for (c = r0; c < r0 + 10 && c < 20; c++) {
                for (r = c0; r > 0; r--) {
                    id = D_8012EDCC[r][c];
                    if (id != 255) {
                        o->p[o->n] = base + tbl[id];
                        o->n++;
                        if (o->n >= 64) return;
                    }
                }
            }
        }
        break;
    case 2:
        c0 = (SX + 90) / 90 - 2;
        if (c0 < 0) c0 = 0;
        if (c0 >= 20) c0 = 19;
        r0 = (SY - 582) / 90;
        if (r0 < 0) r0 = 0;
        if (r0 >= 13) r0 = 12;
        for (r = r0; r < r0 + 8 && r < 13; r++) {
            for (c = c0, col = c; c < D_8012EEDC[col]; c++) {
                id = D_8012EEE8[r][c];
                if (id != 255) {
                    o->p[o->n] = base + tbl[id];
                    o->n++;
                    if (o->n >= 64) return;
                }
            }
        }
        break;
    case 3:
        r0 = (SY - 90) / 90;
        if (r0 < 0) r0 = 0;
        if (r0 >= 20) r0 = 19;
        c0 = 14 - (1350 - SX) / 90;
        if (c0 < 0) c0 = 0;
        if (c0 >= 13) c0 = 12;
        { short c8 = r0;
        for (c = r0; c < c8 + 8 && c < 20; c++) {
            for (r = c0; r >= 6; r--) {
                rb = D_8012EEE8[r][c];
                id = rb;
                if (id != 255) {
                    o->p[o->n] = base + tbl[id];
                    o->n++;
                    if (o->n >= 64) return;
                }
            }
        }
        }
        break;
    default:
        {
            unsigned char *q = base + tbl[0];
            o->n = 1;
            o->p[0] = q;
        }
        break;
    }
}
#endif

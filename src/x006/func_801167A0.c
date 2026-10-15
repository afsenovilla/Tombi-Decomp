// VERSION SLPS_01144 x06 80115A54 540
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x06/nonmatchings/x06", func_80115A54_slps01144);
#else
#include "gvars.h"
/* Debt: nested do{}while(0) around the clamp (loop-weighted refs so cnt outranks offs in global alloc) and the dead `i = n` at the end (n must outlive e: cse's canonical reg of a copy pair is the longer-lived one, so the clamp compare reads n like the game instead of e). */
// MATCHING 801167a0 540
// FUNC 801167a0 540 X006
#include "TOBJ.H"
#include "gamestate.h"

typedef struct {
    unsigned char p0[3];
    unsigned char n;
    unsigned char *list[0x58];
    unsigned char p164;
    unsigned char b165;
} L801167A0;

extern unsigned char D_8011EFDC[];

void area6ScriptOp(L801167A0 *l, unsigned char *bank)
{
    int *offs = (int *)(bank + 4);
    unsigned char cnt = bank[0];
    unsigned int t;
    short i, e, n;

    if (GAME.selectedSection != 0) return;
    t = (*(unsigned int*)&GVARS.p48);
    if (t - 0xf16 < 0x709) {
        for (i = 4; i < 9; i++) {
            l->list[l->n] = bank + offs[i];
            l->n++;
        }
    }
    for (i = 0; i < 4; i++) {
        l->list[l->n] = bank + offs[i];
        l->n++;
    }
    if (t < 0x4c6) {
        i = t / 170;
        if (i < 0) i = 0;
        n = i + 6;
    } else if (t < 0x1000) {
        i = t / 180;
        if (i < 0) i = 0;
        n = i + 9;
    } else {
        i = t / 170;
        if (i < 0) i = 0;
        n = i + 8;
    }
    e = n;
    do { do { if (n >= cnt) e = cnt - 1; } while (0); } while (0);
    for (; i < e; i++) {
        if (D_8011EFDC[i] == 0xff) {
            l->b165++;
            return;
        }
        l->list[l->n] = bank + offs[D_8011EFDC[i]];
        l->n++;
    }
    i = n;
}
#endif

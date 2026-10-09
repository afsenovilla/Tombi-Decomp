/* score 2: only the clamp compare reads e (v1, the copy) instead of n (v0): `addu v1,v0; sll v0,v0` in the game. The nested do{}while(0) around the clamp (debt) adds loop-weighted refs so cnt outranks offs in global alloc (cnt t0, offs t1 like the game). Tried: n/e/i types, ternary, if/else, goto, compare spellings, clamp inline, clamping n in place. Real start 801167A0 (csv piece 80116920 is its tail). */
// FUNC 801167a0 540 X006
#include "TOBJ.H"

typedef struct {
    unsigned char p0[3];
    unsigned char n;
    unsigned char *list[0x58];
    unsigned char p164;
    unsigned char b165;
} L801167A0;

extern unsigned short D_8009C962;
extern unsigned int D_8009C948;
extern unsigned char D_8011EFDC[];

void func_801167A0(L801167A0 *l, unsigned char *bank)
{
    int *offs = (int *)(bank + 4);
    unsigned char cnt = bank[0];
    unsigned int t;
    short i, e, n;

    if (D_8009C962 != 0) return;
    t = D_8009C948;
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
}

// VERSION BETA_980203 x00 8011ED24 300
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/a00UnlayeredType0DShowMessage_scus94236.c (a00UnlayeredType0DShowMessage, twin score 1.000), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL a00UnlayeredType0DShowMessage.c (src/a00UnlayeredType0DShowMessage.c): extern unsigned char DAT_a, DAT_b, DAT_c, GAME_PLAYER_MODE, DAT_e;=>extern unsigned char D_8009BC9B, D_8009BCA7, D_8009BCAA, GAME_PLAYER_MODE, D_8009BCA6;; DAT_a =>D_8009BC9B ; DAT_b =>D_8009BCA7 ; DAT_c =>D_8009BCAA ; DAT_e =>D_8009BCA6 ; FUN_80018980=>PoolFree_1F800210.
#include "TOBJ.H"
extern unsigned char D_8009BC9B, D_8009BCA7, D_8009BCAA, GAME_PLAYER_MODE, D_8009BCA6;
extern TObj *showMessageBox(int, int, int, int);
extern void PoolFree_1F800210(TObj *);

void a00UnlayeredType0DShowMessage(TObj *o)
{
    TObj *q;
    switch (o->step) {
    case 0:
        if (o->visible != 0)
            o->step++;
        else
            o->step = 2;
        break;
    case 1:
        if (D_8009BC9B == 0) {
            D_8009BCA7 = 1;
            D_8009BCAA = 1;
            GAME_PLAYER_MODE = 0;
            o->step++;
        }
        break;
    case 2:
        o->step++;
        *(TObj **)&o->category = showMessageBox(10, 0, 100, 240);
        break;
    case 3:
        q = *(TObj **)&o->category;
        if (q->b04 == 2) {
            q->b04 = 3;
            D_8009BCA7 = 0;
            D_8009BCA6 = 0;
            D_8009BCAA = 0;
            PoolFree_1F800210(o);
        }
        break;
    }
}
#endif

// VERSION SLPS_01144 dspsub3 800ED958 392
#if defined(VERSION_SLPS_01144)
// USA version of PAL d0spsub 800eca28 (subScreenMoveGridCursor.c)
typedef struct { char p[0x14]; short row; char p16[8]; short sel; } Menu;
extern unsigned short JOYPAD_STATE;
extern unsigned short D_8009EE4C[];
#define CNT D_8009EE4C[0]
extern void playSFX(int id);

int func_800ED958(Menu *m)
{
    unsigned short pad = JOYPAD_STATE;
    if (pad & 0x1000) {
        if ((short)(m->sel % 8) > 0) {
            m->sel--;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x4000) {
        if ((short)(m->sel % 8) < 7 && m->sel < CNT - 1) {
            m->sel++;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x8000) {
        if (m->sel >= 8) {
            playSFX(8);
            m->sel -= 8;
        }
    } else {
        short old;
        if (!(pad & 0x2000)) return 0;
        old = m->sel;
        if ((m->sel = old + 8) >= CNT) m->sel = old;
        else playSFX(8);
    }
    if (m->sel / 16 != m->row) m->row = m->sel / 16;
    return 1;
}
#endif

// VERSION SLPS_01144 dspsub 800EDDA4 352
#if defined(VERSION_SLPS_01144)
// USA version of PAL d0spsub 800ecbf0 (subScreenMoveGridCursorN.c)
typedef struct { char p[0x14]; short row; char p16[8]; short sel; } Menu;
extern unsigned short JOYPAD_STATE;
extern void playSFX(int id);

int func_800EDDA4(Menu *m, int n)
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
        if ((short)(m->sel % 8) < 7 && m->sel < n - 1) {
            m->sel++;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x8000) {
        if (m->sel >= 8) {
            m->sel -= 8;
            playSFX(8);
        }
    } else {
        short old;
        if (!(pad & 0x2000)) return 0;
        old = m->sel;
        if ((m->sel = old + 8) >= n) m->sel = old;
        else playSFX(8);
    }
    if (m->sel / 16 != m->row) m->row = m->sel / 16;
    return 1;
}
#endif

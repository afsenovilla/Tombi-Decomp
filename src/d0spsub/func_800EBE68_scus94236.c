// VERSION SCUS_94236 dspsub 800EBE68 352
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub 800ecbf0 (subScreenMoveGridCursorN.c)
typedef struct { char p[0x14]; short row; char p16[8]; short sel; } Menu;
extern unsigned short JOYPAD_STATE;
extern void playSFX(int id);

int subScreenMoveGridCursorN(Menu *m, int n)
{
    unsigned short pad = JOYPAD_STATE;
    if (pad & 0x10) {
        if ((short)(m->sel % 8) > 0) {
            m->sel--;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x40) {
        if ((short)(m->sel % 8) < 7 && m->sel < n - 1) {
            m->sel++;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x80) {
        if (m->sel >= 8) {
            m->sel -= 8;
            playSFX(8);
        }
    } else {
        short old;
        if (!(pad & 0x20)) return 0;
        old = m->sel;
        if ((m->sel = old + 8) >= n) m->sel = old;
        else playSFX(8);
    }
    if (m->sel / 8 != m->row) m->row = m->sel / 8;
    return 1;
}
#endif

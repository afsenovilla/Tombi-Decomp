// FUNC 800ecd94 500 D0SPSUB
// MATCHING 800ecd94 500
typedef struct { char p[0x14]; short row; char p16[8]; short sel; } Menu;
extern unsigned short D_1F8001FC;
extern void playSFX(int id);
extern int func_800EFD10(void);

int func_800ECD94(Menu *m)
{
    int n = (func_800EFD10() + 9) / 10 * 10;
    unsigned short pad = D_1F8001FC;
    if (pad & 0x10) {
        if ((short)(m->sel % 10) > 0) {
            m->sel--;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x40) {
        if ((short)(m->sel % 10) < 9 && m->sel < n - 1) {
            m->sel++;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x80) {
        if (m->sel >= 10) {
            m->sel -= 10;
            playSFX(8);
        }
        if (m->sel / 10 != m->row) m->row = m->sel / 10;
        return 1;
    } else {
        short old;
        if (!(pad & 0x20)) return 0;
        old = m->sel;
        if ((m->sel = old + 10) >= n) m->sel = old;
        else playSFX(8);
    }
    if (m->sel / 10 != m->row) m->row = m->sel / 10;
    return 1;
}

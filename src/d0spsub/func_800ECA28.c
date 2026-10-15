// FUNC 800eca28 456 D0SPSUB
// MATCHING 800eca28 456
typedef struct { char p[0x14]; short row; char p16[8]; short sel; } Menu;
extern unsigned short D_1F8001FC;
extern unsigned short D_8009D2A4[];
#define CNT D_8009D2A4[0]
extern void playSFX(int id);

int func_800ECA28(Menu *m)
{
    unsigned short pad = D_1F8001FC;
    if (pad & 0x10) {
        if ((short)(m->sel % 10) > 0) {
            m->sel--;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x40) {
        if ((short)(m->sel % 10) < 9 && m->sel < CNT - 1) {
            m->sel++;
            playSFX(8);
        }
        return 1;
    }
    if (pad & 0x80) {
        if (m->sel >= 10) {
            playSFX(8);
            m->sel -= 10;
        }
    } else {
        short old;
        if (!(pad & 0x20)) return 0;
        old = m->sel;
        if ((m->sel = old + 10) >= CNT) m->sel = old;
        else playSFX(8);
    }
    if (m->sel / 10 != m->row) m->row = m->sel / 10;
    return 1;
}

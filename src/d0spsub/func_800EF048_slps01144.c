// VERSION SLPS_01144 dspsub 800EF048 808
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ee028 (subScreenEventListStep.c), from the USA one: 16 events per page
typedef struct { unsigned char state, sub; char p2[0x10]; short mode; short page; short cursor; char p18[6]; short sel; } Menu;
extern unsigned short JOYPAD_STATE;
extern unsigned char D_80108D0C[];
extern unsigned char D_8009E94B[];
extern unsigned char D_8010A4C4[];
extern unsigned char D_8010A4C8;
extern int func_800ED918(Menu *m, int a);
extern int func_800EDF04(Menu *m);
extern int subScreenCountEvents(void);
extern void func_800EEE88(int kind, int add, int bank);
extern void func_800F055C(void *o, int a);
extern void playSFX(int id);
extern void func_8001E910_slps01144(int a, int b);
extern void playSFXWithNote(int a, int b);

int func_800EF048(Menu *m)
{
    int pages;
    switch (m->sub) {
    case 0:
        m->cursor = 0;
        m->page = 0;
        m->sub++;
    case 1:
        if (func_800ED918(m, 1)) return 0;
        pages = (subScreenCountEvents() + 15) / 16;
        if (JOYPAD_STATE & 0x4) goto left;
        if (JOYPAD_STATE & 0x8) goto right;
        if (JOYPAD_STATE & 0x40) {
            playSFXWithNote(0x1B, 0x1C);
            return 1;
        } else if (JOYPAD_STATE & 0x820) {
            switch (m->cursor) {
            case 0:
                m->sel = m->page * 16;
                m->sub++;
                break;
            case 4:
                m->mode = 1;
                m->cursor = 0;
                m->sel = -1;
                m->sub = 0;
                break;
            case 5:
                m->mode = 2;
                m->cursor = 0;
                m->sel = -1;
                m->sub = 0;
                break;
            case 6:
                m->mode = 0;
                m->sub = 0;
                break;
            case 7:
                m->mode = 3;
                m->sub = 0;
                return 2;
            case 8:
            case 9:
            case 10:
            case 11:
                playSFXWithNote(10, 10);
                return 1;
            }
            func_8001E910_slps01144(10, 10);
        }
        return 0;
    case 2:
        if (func_800EDF04(m)) return 0;
        pages = (subScreenCountEvents() + 15) / 16;
        if (JOYPAD_STATE & 0x4) {
        left:
            if (m->page > 0) {
                playSFX(8);
                m->page--;
                m->sel -= 16;
            }
        } else if (JOYPAD_STATE & 0x8) {
        right:
            if (m->page < pages - 1) {
                playSFX(8);
                m->page++;
                m->sel += 16;
                if (m->sel >= pages * 16) m->sel = pages * 16;
            }
        } else if (JOYPAD_STATE & 0x40) {
            m->sel = -1;
            playSFXWithNote(0x1B, 0x1C);
            m->sub = 1;
        } else if (JOYPAD_STATE & 0x820) {
            int k = D_80108D0C[m->sel];
            if (D_8009E94B[k]) {
                func_800EEE88(0, k - 1, 3);
                D_8010A4C8 = 0;
                playSFXWithNote(10, 10);
                m->sub++;
            }
        }
        return 0;
    case 3:
        func_800F055C(D_8010A4C4, 3);
        if (D_8010A4C8 == 2) m->sub = 2;
        return 0;
    }
    return 0;
}
#endif

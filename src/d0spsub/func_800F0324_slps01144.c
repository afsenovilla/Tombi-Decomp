// VERSION SLPS_01144 dspsub 800F0324 568
#if defined(VERSION_SLPS_01144)
// JP only: sub-screen mode 6 (a 16-per-page list copied from D_8009ED4C, confirm opens message bank 2)
typedef struct { unsigned char state, sub; char p2[0x12]; short page; short cursor; char p18[6]; short sel; } Menu;
extern unsigned short JOYPAD_STATE;
typedef struct { unsigned short n; } Cnt;
extern Cnt D_8009EE4C;
extern unsigned char D_8009ED4C[], D_8010A38C[];
extern unsigned char D_8010A4C4[];
extern unsigned char D_8010A4C8;
extern int func_800EDDA4(Menu *m, int n);
extern void func_800EEE88(int kind, int add, int bank);
extern void func_800F055C(void *o, int a);
extern void playSFX(int id);
extern void playSFXWithNote(int a, int b);

int func_800F0324(Menu *m)
{
    int pages;
    switch (m->sub) {
    case 0: {
        Cnt *cnt = &D_8009EE4C;
        int i;
        m->cursor = 0;
        m->page = 0;
        m->sel = 0;
        for (i = 0; i < cnt->n; i++) D_8010A38C[i] = D_8009ED4C[i];
        m->sub++;
    }
    case 1: {
        Cnt *cnt = &D_8009EE4C;
        if (func_800EDDA4(m, cnt->n)) return 0;
        pages = (cnt->n + 15) / 16;
        if (JOYPAD_STATE & 0x4) {
            if (m->page > 0) {
                m->page--;
                m->sel -= 16;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x8) {
            if (m->page < pages - 1) {
                m->page++;
                m->sel += 16;
                if (m->sel >= cnt->n) m->sel = cnt->n - 1;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x40) {
            playSFXWithNote(0x1B, 0x1C);
            return 1;
        } else if (JOYPAD_STATE & 0x820) {
            func_800EEE88(1, D_8010A38C[m->sel], 2);
            D_8010A4C8 = 0;
            playSFXWithNote(10, 10);
            m->sub++;
        }
        return 0;
    }
    case 2:
        func_800F055C(D_8010A4C4, 2);
        if (D_8010A4C8 == 2) m->sub = 1;
        return 0;
    }
    return 0;
}
#endif

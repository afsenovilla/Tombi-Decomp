// VERSION SLPS_01144 dspsub 800EE084 304
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ecf88 (subScreenMainStep.c)
typedef struct {
    unsigned char b0, state;
    char p2[0x10];
    short result;
    char p14[2];
    short cursor;
    char p18[6];
    short w1e;
} Menu;
extern unsigned short JOYPAD_STATE;
extern int func_800ED918(Menu *m, int n);
extern void playSFXWithNote(int a, int b);
extern void func_8001E910_slps01144(int a, int b);

int func_800EE084(Menu *m)
{
    switch (m->state) {
    case 0:
        m->cursor = 6;
        m->state++;
    case 1:
        if (func_800ED918(m, 1)) return 0;
        if (JOYPAD_STATE & 0x40) {
            playSFXWithNote(0x1B, 0x1C);
            return 1;
        }
        if (!(JOYPAD_STATE & 0x820)) return 0;
        switch (m->cursor) {
        case 0:
            break;
        case 4:
            m->result = 1;
            m->cursor = 0;
            m->w1e = -1;
            m->state = 0;
            break;
        case 5:
            m->result = 2;
            m->cursor = 0;
            m->w1e = -1;
            m->state = 0;
            break;
        case 6:
            m->result = 0;
            m->state = 0;
            break;
        case 7:
            m->result = 3;
            m->state = 0;
            return 2;
        case 8:
        case 9:
        case 10:
        case 11:
            playSFXWithNote(0xA, 0xA);
            return 1;
        }
        func_8001E910_slps01144(0xA, 0xA);
        return 0;
    }
    return 0;
}
#endif

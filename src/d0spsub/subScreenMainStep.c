// FUNC 800ecf88 316 D0SPSUB
// MATCHING 800ecf88 316
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
extern int subScreenMoveTabCursor(Menu *m, int n);
extern void SfxPlay2(int a, int b);
extern void playSFXAndSetNote(int a, int b);

int subScreenMainStep(Menu *m)
{
    switch (m->state) {
    case 0:
        m->cursor = 6;
        m->state++;
    case 1:
        if (subScreenMoveTabCursor(m, 1)) return 0;
        if (JOYPAD_STATE & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return 1;
        }
        if (!(JOYPAD_STATE & 0x4008)) return 0;
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
            playSFXAndSetNote(0xA, 0xA);
            return 2;
        case 8:
        case 9:
        case 10:
        case 11:
            SfxPlay2(0xA, 0xA);
            return 1;
        }
        playSFXAndSetNote(0xA, 0xA);
        return 0;
    }
    return 0;
}

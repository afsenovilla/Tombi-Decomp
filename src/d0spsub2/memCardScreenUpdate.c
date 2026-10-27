// FUNC 800e9888 276 D0SPSUB2
// MATCHING 800e9888 276
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xe];
    short w12;
    char p14[8];
    short w1c;
} Menu;
extern Menu MENU_STATE;
extern unsigned short JOYPAD_STATE;
extern unsigned short PAD_PRESSED;
extern int MEMCARD_PAD_LAST, MEMCARD_PAD_REPEAT_TIMER;
extern int memCardScreenStep(int pad, Menu *m);

int memCardScreenUpdate(int a0, int a1, int ret)
{
    Menu *m = &MENU_STATE;
    int k;
    switch (m->state) {
    case 0:
        m->state++;
        m->b1 = 0;
        m->b2 = 0;
        m->b3 = 0;
    case 1:
        k = *(volatile unsigned short *)&PAD_PRESSED & 0xF0;
        if (k != MEMCARD_PAD_LAST) {
            MEMCARD_PAD_LAST = k;
            MEMCARD_PAD_REPEAT_TIMER = 0x20;
        } else if (--MEMCARD_PAD_REPEAT_TIMER == 0) {
            MEMCARD_PAD_REPEAT_TIMER = 4;
            JOYPAD_STATE |= k;
        }
        switch (m->w12) {
        case 4:
            m->w1c = 1;
            ret = memCardScreenStep(k, m);
            break;
        case 5:
            m->w1c = 0;
            ret = memCardScreenStep(k, m);
            break;
        }
        break;
    }
    return ret;
}

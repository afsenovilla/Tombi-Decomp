// FUNC 800e90d4 564 D0SPSUB2
// MATCHING 800e90d4 564
typedef struct { char p0[2]; unsigned char state; char p3; short w4, timer; char p8[0x10]; short slot, msg; } Menu;
typedef struct { char p[0x146]; short w146; } Card;
extern unsigned short JOYPAD_STATE;
extern int MEMCARD_YESNO_CURSOR;
extern char *MEMCARD_PORT_PREFIXES_2[];
extern void playSFX(int id);
extern void SfxPlay2(int a, int b);
extern int format(char *fs);

int memCardFormatStep(Menu *m, Card *c)
{
    int i;
    switch (m->state) {
    case 0:
        MEMCARD_YESNO_CURSOR = 1;
        m->msg = 0xD;
        m->state++;
        break;
    case 1:
        if (MEMCARD_YESNO_CURSOR == 0) {
            if (JOYPAD_STATE & 0x20) {
                MEMCARD_YESNO_CURSOR = 1;
                goto snd;
            }
            if (JOYPAD_STATE & 0x2000) goto cancel;
            if (JOYPAD_STATE & 0x4000) {
                m->msg = 0xB;
                m->timer = 4;
                SfxPlay2(0xA, 0xA);
                m->state++;
            }
        } else {
            if (JOYPAD_STATE & 0x80) {
                MEMCARD_YESNO_CURSOR = 0;
            snd:
                playSFX(8);
                return 0;
            }
            if (JOYPAD_STATE & 0x6000) {
            cancel:
                SfxPlay2(0x1B, 0x1C);
                return -1;
            }
        }
        break;
    case 2:
        if (--m->timer == 0) m->state++;
        break;
    case 3:
        i = 3;
        m->msg = 8;
        m->state = 5;
        do {
            if (format(MEMCARD_PORT_PREFIXES_2[m->slot]) == 1) {
                m->msg = 0xC;
                m->state = 4;
                i = 0;
            }
        } while (--i > 0);
        break;
    case 4:
        if (JOYPAD_STATE & 0x6000) {
            c->w146 = 0;
            m->state = 0;
            m->w4 = 1;
            return 1;
        }
        break;
    case 5:
        if (JOYPAD_STATE & 0x6000) return -1;
        break;
    }
    return 0;
}

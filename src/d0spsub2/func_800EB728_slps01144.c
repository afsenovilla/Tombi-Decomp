// VERSION SLPS_01144 dspsub2 800EB728 448
#if defined(VERSION_SLPS_01144)
// JP memCardFormatStep (PAL d0spsub2 800e90d4): no yes/no cursor, pad bits 0x40 / 0x10 / 0x50
typedef struct { char p0[2]; unsigned char state; char p3; short w4, timer; char p8[0x10]; short slot, msg; } Menu;
typedef struct { char p[0x146]; short w146; } Card;
asm(".set JS_,0x1f8001fc");
extern unsigned short JS_;
extern char *D_800F2A78[];
extern void func_8001E388(int a, int b); /* SfxPlay2 */
extern int func_8005853C(char *fs); /* format */

int func_800EB728(Menu *m, Card *c)
{
    int i;
    unsigned short b;
    switch (m->state) {
    case 0:
        m->msg = 0xD;
        m->state++;
        break;
    case 1:
        b = JS_;
        if (b & 0x40) {
            func_8001E388(0x1B, 0x1C);
            return -1;
        }
        if (b & 0x10) {
            m->msg = 0xB;
            m->timer = 4;
            func_8001E388(0xA, 0xA);
            m->state++;
        }
        break;
    case 2:
        if (--m->timer != 0) return 0;
        m->state++;
        break;
    case 3:
        i = 3;
        m->msg = 8;
        m->state = 5;
        do {
            if (func_8005853C(D_800F2A78[m->slot]) == 1) {
                m->msg = 0xC;
                m->state = 4;
                i = 0;
            }
        } while (--i > 0);
        break;
    case 4:
        if (JS_ & 0x50) {
            c->w146 = 0;
            m->state = 0;
            m->w4 = 1;
            return 1;
        }
        break;
    case 5:
        if (JS_ & 0x50) return -1;
        break;
    }
    return 0;
}
#endif

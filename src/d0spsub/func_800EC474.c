// FUNC 800ec474 144 D0SPSUB
// MATCHING 800ec474 144
typedef struct { char p[0x12]; short w12; char p14[2]; short w16; } Menu;

int func_800EC474(Menu *m)
{
    short n = m->w16;
    if (n < 4) {
        switch (m->w12) {
        case 0:
            m->w16 = n + 4;
            return 1;
        case 1:
            if (n >= 3) {
                m->w16 = 2;
                return 1;
            }
            break;
        case 2:
            if (n > 0) {
                m->w16 = 0;
                return 1;
            }
            break;
        case 3:
            if (n > 0) {
                m->w16 = 0;
                return 1;
            }
            break;
        }
    }
    return 0;
}

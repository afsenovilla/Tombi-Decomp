// FUNC 800e9888 276 D0SPSUB2
// MATCHING 800e9888 276
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xe];
    short w12;
    char p14[8];
    short w1c;
} Menu;
extern Menu D_800A45D8;
extern unsigned short D_1F8001FC;
extern unsigned short D_8009D670;
extern int D_800F7140, D_800F7144;
extern int memCardScreenStep(int pad, Menu *m);

int memCardScreenUpdate(int a0, int a1, int ret)
{
    Menu *m = &D_800A45D8;
    int k;
    switch (m->state) {
    case 0:
        m->state++;
        m->b1 = 0;
        m->b2 = 0;
        m->b3 = 0;
    case 1:
        k = *(volatile unsigned short *)&D_8009D670 & 0xF0;
        if (k != D_800F7140) {
            D_800F7140 = k;
            D_800F7144 = 0x20;
        } else if (--D_800F7144 == 0) {
            D_800F7144 = 4;
            D_1F8001FC |= k;
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

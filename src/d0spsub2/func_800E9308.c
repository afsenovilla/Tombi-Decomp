// FUNC 800e9308 1408 D0SPSUB2
// MATCHING 800e9308 1408
typedef struct { unsigned char state, sub, b2, b3; short timer; short x6; char p8[0x10]; short card; short x1A; short x1C; } Menu;
typedef struct { char p0[0x140]; short status, prev, p144, type; char p148[0x12]; } Card;
extern Menu D_800A45D8;
extern unsigned char D_800A45D9;
extern Card D_800F7200[];
extern short D_800F713C[];
extern int D_800F714C[2];
extern int D_800F7154, D_800F7158;
extern short D_800F70F0, D_800F70F4, D_800F70F8;
extern unsigned short D_1F8001FC;
extern int func_800EC77C(Card *c, int n);
extern int func_800E82A4(Menu *m, Card *c);
extern int func_800E90D4(Menu *m, Card *c);
extern void playSFX(int id);
extern void SfxPlay2(int a, int b);

int func_800E9308(void)
{
    Menu *m = &D_800A45D8;
    Card *c;
    int i;
    short *q;
    switch (D_800A45D9) {
    case 0:
        m->card = 0;
        for (i = 0; i < 2; i++) {
            c = &D_800F7200[i];
            c->type = 0;
            c->status = -3;
            c->prev = -3;
            D_800F713C[i] = 0;
        }
        D_800F7154 = 0;
        D_800F7158 = 0;
        m->x1A = 1;
        m->timer = 4;
        m->x6 = 1;
        m->b2 = 0;
        m->sub++;
        break;
    case 1:
        if (--m->timer == 0) {
            D_800F7158 = 1;
            m->timer = 0x1E;
            for (i = 1; i >= 0; i--) {
                c = &D_800F7200[i];
                c->status = func_800EC77C(c, i);
                if (c->status != c->prev) {
                    c->prev = c->status;
                    if (c->type == 1 || c->type == -1) {
                        D_800F714C[i] = 1;
                        D_800F7154 = i + 1;
                    } else {
                        D_800F714C[i] = 0;
                    }
                }
            }
        }
        if (D_800F714C[0]) {
            if (D_800F714C[1]) {
                if ((D_1F8001FC & 0x80) && D_800F7154 == 2) {
                    D_800F7154 = 1;
                    playSFX(8);
                } else if ((D_1F8001FC & 0x20) && D_800F7154 == 1) {
                    D_800F7154 = 2;
                    playSFX(8);
                }
            } else {
                D_800F7154 = 1;
            }
        } else if (D_800F714C[1]) {
            D_800F7154 = 2;
        } else {
            D_800F7154 = 0;
        }
        if ((D_1F8001FC & 0x4000) && D_800F7154) {
            m->card = D_800F7154 - 1;
            SfxPlay2(10, 10);
            m->x1A = 1;
            m->timer = 1;
            m->b2 = 0;
            m->sub++;
            break;
        }
        if (D_1F8001FC & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return -1;
        }
        break;
    case 2:
        c = &D_800F7200[m->card];
        if (--m->timer == 0) {
            m->timer = 0x5A;
            c->status = func_800EC77C(c, m->card);
            if (c->status != c->prev) {
                c->prev = c->status;
                m->b2 = 0;
            }
        }
        switch (c->status) {
        case 0:
            {
            switch (func_800E82A4(m, c)) {
            case -1:
                m->sub--;
                break;
            case 1:
                return 1;
            default:
                return 0;
            }
            }
            break;
        case 1:
            if (m->x1C) {
                if (func_800E90D4(m, c) == -1) m->sub--;
            } else {
                m->x1A = 3;
                if (D_1F8001FC & 0x2000) m->sub--;
            }
            break;
        case -1:
            c->prev = 0;
            m->timer = 1;
            m->sub--;
            break;
        case -2:
            m->x1A = 0;
            if (D_1F8001FC & 0x2000) {
                c->prev = 0;
                m->timer = 1;
                m->sub--;
            }
            break;
        default:
            return 0;
        }
        break;
    case 3:
        {
        int d = D_800F70F0 - D_800F70F8;
        if (d > 0x50) {
            m->sub++;
        } else if (d < 0) {
            m->sub++;
        } else {
            D_800F70F4 = D_800F70F0;
            m->sub--;
        }
        }
        break;
    case 4:
        if (D_800F70F0 != D_800F70F4) {
            if (D_800F70F0 - D_800F70F4 > 0) D_800F70F4 += 8;
            else D_800F70F4 -= 8;
            if (D_800F70F4 - D_800F70F8 > 0x28) D_800F70F8 += 8;
            if (D_800F70F4 - D_800F70F8 < 0) D_800F70F8 -= 8;
            return 0;
        }
        m->sub = 2;
        break;
    }
    return 0;
}

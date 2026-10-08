// FUNC 800ed258 1272 X003
// MATCHING 800ed258 1272
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[2];
    unsigned char b0a, b0b, b0c, b0d, b0e, b0f;
    short ax, aw, yx, yw, bx, bw;
    char p1c[4];
    short timer, w22;
    char p24[0x30 - 0x24];
    int d30;
    char p34[0x68 - 0x34];
    unsigned char b68, b69, b6a, b6b;
    short box0, box1, box2, box3;
    char p74[0x7e - 0x74];
    short velY, velH, velV;
    char p84[0xa0 - 0x84];
    int da0;
    char pa4[3];
    unsigned char ba7;
    int da8;
    short wac;
    char pae[0xb4 - 0xae];
    int db4;
    int db8;
    int dbc;
    char pc0[0xcc - 0xc0];
    short wcc;
} S;
typedef struct { int d0; int b8; int bc; char p0c[0xc]; short wcc; } E;
extern unsigned char DAT_8009c93e;
extern unsigned char DAT_800a60d4;
extern short DAT_800a604e;
extern int DAT_8009c978;
extern int DAT_8009c97c;
extern int DAT_8009c978A[];
extern unsigned char DAT_8009d079;
extern int *DAT_80114c64[];
extern char D_800E3E28[];
extern void FUN_80025aa8(int, int);
extern void FUN_80024ea0(int, int, int);
extern short FUN_8001e4f0(int);
extern void FUN_8001eaa4(int);
extern void FUN_80020d20(int, int, int, int);
extern void FUN_8005a9a4(int, int);

void FUN_800ed258(S *o)
{
    E *e;
    int *p;
    short v, t, n;
    unsigned int k;
    unsigned short w;
    short r;
    unsigned char c;

    if (DAT_8009c93e != 0)
        return;
    e = (E *)&o->db4;
    switch (o->state) {
    case 0:
        o->timer = 0;
        o->w22 = 0;
        o->velV = 0;
        o->velY = 0x4b0;
        p = DAT_80114c64[o->b0c];
        p = (int *)(*p + ((int *)*(int *volatile *)p)[1]);
        e->bc = (int)p;
        e->b8 = (int)(D_800E3E28 + o->b0c * 0x1600);
        o->da8 = e->b8;
        FUN_80025aa8(o->da0, e->b8);
        o->state = 1;
        o->b69 = 0;
        o->b0a = 0x12;
        break;
    case 1:
        if (o->visible == 0)
            goto set4;
        if (o->velV < 0x1000) {
            FUN_80025aa8(o->da0, e->b8);
            r = o->velV;
            w = r;
            if (r < 0x1000) {
                e->wcc = w;
                if (o->timer == 0)
                    o->wac = FUN_8001e4f0(0x17);
            } else {
                e->wcc = 0x1000;
            }
            t = (e->wcc * 9 >> 10) - 0x36;
            if ((o->b69 & 1) && DAT_800a60d4 == 0)
                DAT_800a604e += t + o->box2;
            o->box2 = -t;
            o->box3 = -t;
            FUN_80024ea0(e->b8, e->bc, e->wcc);
            o->timer++;
            o->velV += o->velY / o->timer;
            if (o->subtype == 0 && *(short *)((char *)o + 0x20) == 4)
                FUN_80020d20(6, o->aw, (short)(o->yw - 10), o->bw);
        } else {
            o->state = 2;
            o->b6b = 0;
            if (o->b0e != 1 && (o->b69 & 1)) {
                k = o->subtype;
                o->b0e = 1;
                o->b0d = 0x20;
                if (k < 0x20)
                    DAT_8009c978A[0] |= 1 << k;
                else
                    DAT_8009c978A[1] |= 1 << (k - 0x20);
                c = *(volatile unsigned char *)&DAT_8009d079 + 1;
                *(volatile unsigned char *)&DAT_8009d079 = c;
                if (c == 0x18)
                    FUN_8005a9a4(0x3f, 0);
            }
        }
        if (o->b69 == 0) {
            o->state = 3;
            o->w22 = 0;
            o->d30 = o->timer;
            FUN_8001eaa4(o->wac);
            o->wac = FUN_8001e4f0(0x1b);
            break;
        }
        o->b69 = 0;
        break;
    case 2:
        if (o->b6b < 5) {
            o->b6b++;
            break;
        }
        if (o->b69 == 0) {
            o->state = 3;
            o->w22 = 0;
            o->wac = FUN_8001e4f0(0x1b);
            break;
        }
        o->b69 = 0;
        o->b6b = 0;
        break;
    case 3:
        if (o->visible == 0) {
        set4:
            o->state = 4;
            break;
        }
        FUN_80025aa8(o->da0, e->b8);
        v = o->velV;
        if (o->velV < 0)
            v = 0;
        e->wcc = v;
        t = (v * 9 >> 10) - 0x36;
        o->box2 = -t;
        o->box3 = -t;
        FUN_80024ea0(e->b8, e->bc, e->wcc);
        if (o->velV > 0) {
            o->w22++;
            if (o->timer > 2)
                n = o->timer - 2;
            else
                n = 0;
            o->velV -= o->velY / (o->w22 + 1);
            o->timer = n;
        } else {
            o->state = 4;
        }
        if (o->b69 != 0) {
            o->state = 1;
            o->b69 = 0;
            FUN_8001eaa4(o->wac);
        }
        break;
    case 4:
        o->box2 = 0x36;
        o->box3 = 0x36;
        o->step = 0;
        o->state = 0;
        o->b68 = 0;
        o->b0a = o->ba7;
        break;
    }
}

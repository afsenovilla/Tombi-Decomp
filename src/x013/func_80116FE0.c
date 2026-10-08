// FUNC 80116fe0 2516 X013
// MATCHING 80116fe0 2516
typedef struct { short x, y; } XY;
typedef struct { short op, arg; } CMD;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x20 - 8];
    short timer;
    char p22[0x90 - 0x22];
    struct O *d90;
    char p94[0xb4 - 0x94];
    unsigned short wb4, wb6;
} O;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x12 - 8];
    short xw;
    char p14[0x2e - 0x14];
    unsigned short animFrame;
} PL;
typedef struct { char p[0x4a]; short w4a, w4c, w4e; } Q;

extern PL D_800A6038;
extern CMD *D_801181FC[];
extern XY D_80118200[];
extern unsigned char D_8009D088[];
extern unsigned char D_8009C93A, D_8009C93F, D_8009C975x[];
#define D_8009C975 D_8009C975x[0]
extern unsigned int D_8009C968;
extern unsigned char D_8009CDA4[];
extern int D_8011A8C0, D_8011A8C4, D_8011A8C8, D_8011A8CC;
extern unsigned char D_8009CE48, D_8009CDCB, D_8009CE0B;
extern unsigned short D_8009D2DA;
extern unsigned short D_1F8001FC;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern short D_1F8001C6;
extern Q *D_1F8001D4;
extern char D_8001D6A4[];
extern void FUN_800eea7c(PL *, int, int);
extern O *FUN_8002dc50(int, int, int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void stopBgm(int);
extern void ThreadCreate(int, char *);
extern void FUN_8001eb64(void);
extern void func_801179B4(int, int, int, int);
extern void FUN_80018790(O *);

void func_80116FE0(O *o)
{
    CMD *c;
    PL *p;
    int i;
    unsigned int t, h;
    char pad[16];

    switch (o->b04) {
    case 0:
        o->wb4 = 0;
        o->wb6 = 0;
        o->active = 2;
        o->timer = 0;
        D_8009D088[0] = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        c = D_801181FC[o->wb4];
        c += o->wb6;
        p = &D_800A6038;
        switch (c->op) {
        case 0:
            D_1F8001D4->w4a = 2;
            D_1F8001D4->w4c = 0;
            D_1F8001D4->w4e = 0;
            break;
        case 1:
            D_8009D088[0] = c->arg;
            o->wb6++;
            break;
        case 2:
            if (o->timer == 0) {
                o->timer = c->arg;
                break;
            }
            if (--o->timer != 0) break;
            o->step = 0;
            o->wb6++;
            break;
        case 3:
            switch (o->step) {
            case 0:
                if (*(unsigned short *)&p->b04 != 0x6505) break;
                D_8009C93A = 1;
                D_8009C93F = 1;
                o->timer = 1;
                o->step++;
                break;
            case 1:
                if (--o->timer != 0) break;
                D_800A6038.b04 = 5;
                D_800A6038.step = 0x64;
                D_800A6038.state = 0;
                D_800A6038.animFrame = 1;
                FUN_800eea7c(p, 0, 0);
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 4:
            o->d90 = FUN_8002dc50(2, c->arg, D_80118200[D_8009D088[0] & 0xf].x, D_80118200[D_8009D088[0] & 0xf].y);
            o->wb6++;
            break;
        case 5:
            if (o->d90->b04 < 2) break;
            o->d90->b04++;
            o->wb6++;
            break;
        case 6:
            FUN_8005a8a8(c->arg, 0, 0);
            o->wb6++;
            break;
        case 7:
            FUN_8005a9a4(c->arg, 0);
            o->wb6++;
            break;
        case 8:
            break;
        case 9:
            D_8009C975 = 3;
            D_800A6038.xw++;
            o->wb6++;
            break;
        case 10:
            D_800A6038.xw++;
            if (D_8009C975 != 1) break;
            D_800A6038.b04 = 5;
            D_800A6038.step = 0x40;
            D_800A6038.state = 0;
            D_800A6038.visible = 0;
            o->wb6++;
            break;
        case 11:
            D_8009C975 = 4;
            o->wb6++;
            break;
        case 12:
            if (D_8009C975 != 0) break;
            o->wb6++;
            break;
        case 13:
            D_1F8001CC = 1;
            D_1F8001CD = 0x14;
            D_1F8001C6 = 2;
            stopBgm(0);
            ThreadCreate(1, D_8001D6A4);
            o->wb6++;
            break;
        case 14:
            FUN_8001eb64();
            o->wb6++;
            break;
        case 15:
            switch (o->step) {
            case 0:
                D_800A6038.animFrame = 0;
                FUN_800eea7c(p, 1, 0);
                o->step++;
                break;
            case 1:
                if (++D_800A6038.xw < 0x105) break;
                D_800A6038.animFrame = 1;
                FUN_800eea7c(p, 0, 0);
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 16:
            switch (o->step) {
            case 0:
                D_800A6038.animFrame = 0;
                FUN_800eea7c(p, 1, 0);
                o->step++;
                break;
            case 1:
                if (++D_800A6038.xw < 0x14b) break;
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 17:
            switch (o->step) {
            case 0:
                t = D_8009C968;
                h = t / 180000;
                D_8011A8C8 = 0;
                D_8011A8C4 = t / 3000 - h * 60;
                D_8011A8C0 = h % 1000;
                for (i = 0; i < 0x100; i++) {
                    if (i != 0x12 && D_8009CDA4[i]) D_8011A8C8++;
                }
                D_8011A8CC = 0;
                for (i = 0; i < 0x100; i++) {
                    if (i != 0x12 && D_8009CDA4[i] == 0xff) D_8011A8CC++;
                }
                o->step++;
            case 1:
                func_801179B4(D_8011A8C0, D_8011A8C4, D_8011A8C8, D_8011A8CC);
                if (!(D_1F8001FC & 0x6008)) break;
                if (D_8009CE48 == 0xff && D_8009CDCB == 0xff && D_8009D2DA != 0 && D_8009CE0B == 0xff) {
                    D_8009C975 = 3;
                    o->step = 0xb;
                    break;
                }
                o->timer = 1;
                o->step++;
                break;
            case 2:
                if (D_8009CE48 != 0xff) {
                    o->d90 = FUN_8002dc50(3, 0, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 4;
                }
                break;
            case 3:
                if (--o->timer != 0 && o->d90->substep != 1) break;
                if (D_8009CDCB == 0xff && D_8009D2DA != 0 && D_8009CE0B == 0xff) {
                    o->timer = 1;
                    o->step = 9;
                    break;
                }
                o->d90->b04 = 3;
                o->step++;
                break;
            case 4:
                if (D_8009CDCB != 0xff) {
                    o->d90 = FUN_8002dc50(3, 1, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 6;
                }
                break;
            case 5:
                if (--o->timer != 0 && o->d90->substep != 1) break;
                if (D_8009D2DA != 0 && D_8009CE0B == 0xff) {
                    o->timer = 1;
                    o->step = 9;
                    break;
                }
                o->d90->b04 = 3;
                o->step++;
                break;
            case 6:
                if (D_8009D2DA == 0) {
                    o->d90 = FUN_8002dc50(3, 2, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 8;
                }
                break;
            case 7:
                if (--o->timer != 0 && o->d90->substep != 1) break;
                if (D_8009CE0B != 0xff) {
                    o->d90->b04 = 3;
                    o->step++;
                    break;
                }
                o->timer = 1;
                o->step = 9;
                break;
            case 8:
                if (D_8009CE0B != 0xff) {
                    o->d90 = FUN_8002dc50(3, 3, 0xa0, 0xa0);
                    o->timer = 0x168;
                } else {
                    o->timer = 1;
                }
                o->step++;
                break;
            case 9:
                if (--o->timer != 0 && o->d90->substep != 1) break;
                o->d90->substep = 1;
                D_8009C975 = 3;
                o->step++;
                break;
            case 11:
                func_801179B4(D_8011A8C0, D_8011A8C4, D_8011A8C8, D_8011A8CC);
            case 10:
                if (D_8009C975 != 1) break;
                o->wb6++;
                break;
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}

// VERSION SLPS_01144 x14 8011FB54 580
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x10 - 8];
    union { int d10; struct { short h10, h12; } s; } u;
    int d14, d18;
    char p1c[0x2e - 0x1c];
    short h2e;
    char p30[0x7c - 0x30];
    short h7c, h7e;
    char p80[0xac - 0x80];
    short hac, hae, hb0;
} O;
typedef struct { short x, y, z, w; } Pt;
extern unsigned char D_8009E4EA;
extern unsigned short D_8009E50A;
extern Pt D_801237FC[], D_8012387C[];
extern O *ObjAlloc(void);
extern int Rand(void);
extern void playSFX(int);
extern volatile unsigned short VIEW_CAMERA_X, VIEW_CAMERA_Y;
#define SPX VIEW_CAMERA_X
#define SPY VIEW_CAMERA_Y

void func_8011FB54(O *o)
{
    Pt *t;
    int i, cnt, f, s;
    O *n;
    int X, Y;

    if (D_8009E4EA == 1) return;
    t = D_801237FC;
    if (D_8009E50A == 7) t = D_8012387C;
    o->hb0 = 0;
    cnt = 0;
    for (i = 0; i < 16; i++) {
        f = 0;
        if ((unsigned short)(t->x - SPX + 158) < 317)
            f = (unsigned short)(t->y - SPY + 70) < 171;
        t++;
        cnt += f;
        o->hb0 |= f << i;
    }
    if (cnt) {
        f = 0;
        s = 0;
        do {
            i = Rand() & 0xf;
            if ((o->hb0 >> i) & 1) {
                o->hae = i;
                f = 1;
                break;
            }
            s++;
        } while (s < 8);
        if (!f) {
            for (s = 0; s < 16; s++) {
                if ((o->hb0 >> s) & 1) {
                    o->hae = s;
                    break;
                }
            }
        }
    } else
        o->hae = 0;
    t = D_801237FC;
    if (D_8009E50A == 7) t = D_8012387C;
    t += o->hae;
    o->u.s.h12 = o->h7c = t->x;
    o->h7e = t->y;
    o->hac = t->z;
    o->step++;
    X = (short)t->x;
    Y = (short)t->y;
    for (s = 0; s < 2; s++) {
        n = ObjAlloc();
        if (n) {
            n->active = 2;
            n->type = 0x25;
            n->h2e = s;
            n->u.d10 = X << 16;
            n->d14 = Y << 16;
            n->d18 = 0;
        }
    }
    playSFX(D_8009E50A == 7 ? 245 : 233);
}
#endif

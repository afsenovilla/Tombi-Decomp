// FUNC 8004bbc0 1060 MAIN0
// wip score 64: only diff = second loop: gcc hoists both 0xb and 0x21 (extra s3 save); game hoists only 0xb (s2).
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct { char p0[4]; unsigned short s4; } A;
typedef struct {
    char p0[0x16]; unsigned short s16;
    char p1[0x24 - 0x18]; A *anim;
    char p2[0x2e - 0x28]; unsigned short s2e;
    char p3[0x40 - 0x30]; H *h40;
    char p4[0x9e - 0x44]; unsigned char b9e;
    char p5[0xa9 - 0x9f]; unsigned char ba9;
    char p6[0xe8 - 0xaa]; unsigned short se8; short sea;
} TO;
extern int D_1F8003C0;
extern short D_1F80019E;
extern unsigned short D_1F800254, D_1F800250;
extern unsigned char **D_1F800268, **D_1F800260;
extern unsigned short D_8009C960;
extern short FUN_80041ca8(TO *, short, short);
extern void func_801269D0(TO *), func_8004B454(TO *, unsigned char *), func_80126940(TO *), func_8011FE38(TO *);
extern void func_80126FE4(TO *), func_80126A64(TO *), func_80126D2C(TO *), func_80126E8C(TO *), func_801212C4(TO *);
extern void func_8011DF8C(TO *), func_8012767C(TO *), func_8011EF98(TO *), func_8004B7C4(TO *, unsigned char *);
extern void func_8011F5D8(TO *), func_8011F474(TO *), func_801212D4(TO *), func_8011D3F8(TO *), func_8011F38C(TO *);
extern void func_80126F64(TO *), func_80121254(TO *);

int func_8004BBC0(TO *o, unsigned char f)
{
    short r;
    int d;
    short n;
    unsigned char **q;
    unsigned char *e;

    if (o->b9e != 0) return 0;
    D_1F8003C0 = 0;
    if (f) o->sea = o->s16 - 2;
    else o->sea = o->s16 - 8;
    d = 0x10;
    if (o->s2e & 1) d = -0x10;
    r = FUN_80041ca8(o, o->h40->s2 + d, o->sea);
    if (r != 0) {
        { int c = 6; if (r == 2) c = 5; o->b9e = c; }
        if (o->s2e & 1) o->h40->s2 -= 0xc;
        else o->h40->s2 += 0xc;
        D_1F8003C0++;
        return D_1F8003C0;
    }
    switch (o->anim->s4) {
    case 0: n = 9; break;
    case 1: n = 10; break;
    case 2: n = 0xc; break;
    default: n = 0x18; break;
    }
    if (o->ba9) n = 0xc;
    d = -n;
    if (!(o->s2e & 1)) d = n;
    q = D_1F800268;
    D_1F80019E = D_1F800254;
    o->se8 = o->h40->s2 + d;
    while (D_1F80019E != 0) {
        e = *q;
        D_1F80019E--;
        q++;
        if (*e & 1) {
            switch (e[2]) {
            case 1:
                func_801269D0(o);
                break;
            case 3:
                func_8004B454(o, e);
                break;
            case 4:
                if (D_8009C960 == 0) {
            case 2:
                    func_80126940(o);
                } else {
                    func_8011FE38(o);
                }
                break;
            case 5:
                func_80126FE4(o);
                break;
            case 6:
                func_80126A64(o);
                break;
            case 0x15:
                func_80126D2C(o);
                break;
            case 0xe:
                func_80126E8C(o);
                break;
            case 0x1c:
                func_801212C4(o);
                break;
            case 0x1d:
                func_8011DF8C(o);
                break;
            case 0x14:
                func_8012767C(o);
                break;
            case 0x34:
                func_8011EF98(o);
                break;
            case 0x10: case 0x11: case 0x19: case 0x35: case 0x37: case 0x3d: case 0x3e:
                func_8004B7C4(o, e);
                break;
            case 0x1f:
                func_8011F5D8(o);
                break;
            case 0x22:
                func_8011F474(o);
                break;
            case 0x31:
                func_801212D4(o);
                break;
            case 0x42:
                func_8011D3F8(o);
                break;
            case 0x24:
                func_8011F38C(o);
                break;
            }
        }
    }
    if (D_1F8003C0 == 0) {
        q = D_1F800260;
        D_1F80019E = D_1F800250;
        while (D_1F80019E != 0) {
            e = *q;
            D_1F80019E--;
            q++;
            if (*e & 1) {
                switch (e[2]) {
                case 0xb:
                    func_80126F64(o);
                    break;
                case 0x21:
                    func_80121254(o);
                    break;
                }
            }
        }
    }
    return D_1F8003C0;
}

// FUNC 80045f00 1808 MAIN0
// MATCHING 80045f00 1808
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern short DAT_1f80019e;
extern unsigned short DAT_1f800244;
extern short DAT_1f800246;
extern unsigned short DAT_1f800246u;
extern TObj **DAT_1f800218;
extern TObj **DAT_1f80021c;
extern int FUN_800425c4(TObj *, TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8001e4f0(int);
extern void FUN_80125f04(TObj *, TObj *);
extern void FUN_80125aac(TObj *, TObj *);
extern void FUN_80044d54(TObj *, TObj *);
extern void FUN_80120a2c(TObj *, TObj *);
extern void FUN_80044f18(TObj *, TObj *);
extern void FUN_80125c1c(TObj *, TObj *);
extern void FUN_80125da4(TObj *, TObj *);
extern void FUN_80045344(TObj *, TObj *);
extern void FUN_80125c84(TObj *, TObj *);
extern void FUN_8011f844(TObj *, TObj *);
extern void FUN_8011ddc0(TObj *, TObj *);
extern void FUN_80126004(TObj *, TObj *);
extern void FUN_80125b88(TObj *, TObj *);
extern void FUN_80120ba0(TObj *, TObj *);
extern void FUN_8011ff64(TObj *, TObj *);
extern void FUN_8011f978(TObj *, TObj *);
extern void FUN_8011f7ec(TObj *, TObj *);
extern void FUN_80120564(TObj *, TObj *);
extern void FUN_8011d2dc(TObj *, TObj *);
extern void FUN_8011cba0(TObj *, TObj *);
extern void FUN_8011cff0(TObj *, TObj *);

static __inline__ short hit(TObj *a, TObj *b)
{
    short dx;
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b)
        return -1;
    dx = U16(PTR(a, 0x40), 2) - U16(PTR(b, 0x40), 2);
    if ((unsigned short)(dx + (U16(b, 0x6c) + U16(a, 0x6c))) > S16(b, 0x6e) + S16(a, 0x6e))
        return -1;
    if ((unsigned short)((U16(a, 0x16) - U16(b, 0x16)) + (U16(a, 0x70) + U16(b, 0x70))) > S16(a, 0x72) + S16(b, 0x72))
        return -1;
    DAT_1f80019e = 0;
    if (dx < 0)
        return 0;
    return 1;
}

static __inline__ void col(TObj *a, TObj *b)
{
    int r;

    if (hit(a, b) >= 0) {
        r = 1;
        b->b68 = 1;
        b->b9e = 0;
        switch (FUN_800425c4(a, b)) {
        case 1:
        case 7:
            r = 1;
            break;
        case 4:
        case 5:
        case 10:
            r = 1;
            a->b6a = 1;
            a->active = 2;
            a->wa8 = 0x4ff;
            break;
        case 0:
        case 3:
        case 9:
            r = 2;
            break;
        case 13:
            b->b68 = 0;
            r = -1;
            break;
        case 6:
        case 11:
        case 12:
            r = 2;
            a->b6a = 1;
            a->active = 2;
            a->wa8 = 0x4ff;
            break;
        case 2:
        case 8:
            r = 0;
            b->b9e = 1;
            b->b9f = 0;
            break;
        }
        if (r >= 0) {
            FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
            if ((b->category & 0x7f) == 4)
                FUN_8001e4f0(6);
            else
                FUN_8001e4f0(7);
        }
    }
}

void FUN_80045f00(void)
{
    short n = DAT_1f800244;
    TObj **pp = DAT_1f800218;
    TObj **qq;
    TObj *a, *b;

    if (DAT_1f800246 == 0)
        return;
    while (n != 0) {
        a = *pp++;
        n--;
        if (!(a->active & 1) || a->ba5 == 0)
            continue;
        qq = DAT_1f80021c;
        for (DAT_1f80019e = DAT_1f800246u; DAT_1f80019e != 0;) {
            b = *qq++;
            DAT_1f80019e--;
            if (!(b->active & 1))
                continue;
            switch (b->type) {
            case 0:
                FUN_80125f04(a, b);
                break;
            case 1:
                FUN_80125aac(a, b);
                break;
            case 2:
                FUN_80044d54(a, b);
                break;
            case 10:
                FUN_80120a2c(a, b);
                break;
            case 11:
                col(a, b);
                break;
            case 4:
                col(a, b);
                break;
            case 14:
                FUN_80044f18(a, b);
                break;
            case 18:
                FUN_80125c1c(a, b);
                break;
            case 6:
                FUN_80125da4(a, b);
                break;
            case 7:
                FUN_80045344(a, b);
                break;
            case 8:
                FUN_80125c84(a, b);
                break;
            case 30:
                FUN_8011f844(a, b);
                break;
            case 16:
            case 26:
            case 28:
                col(a, b);
                break;
            case 39:
                FUN_8011ddc0(a, b);
                break;
            case 17:
                FUN_80126004(a, b);
                break;
            case 19:
                FUN_80125b88(a, b);
                break;
            case 33:
                FUN_80120ba0(a, b);
                break;
            case 40:
                FUN_8011ff64(a, b);
                break;
            case 41:
                FUN_8011f978(a, b);
                break;
            case 56:
                FUN_8011f7ec(a, b);
                break;
            case 58:
                FUN_80120564(a, b);
                break;
            case 59: case 60: case 61: case 62:
            case 63: case 64: case 65: case 66:
                FUN_8011d2dc(a, b);
                break;
            case 71:
                FUN_8011cba0(a, b);
                break;
            case 79:
                FUN_8011cff0(a, b);
                break;
            }
        }
    }
}

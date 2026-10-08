// FUNC 80046610 1864 MAIN0
// MATCHING 80046610 1864
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern short DAT_1f80019e;
extern unsigned short DAT_1f800244;
extern short DAT_1f80024c;
extern unsigned short DAT_1f80024cu;
extern unsigned short D_8009C960;
extern TObj **DAT_1f800218;
extern TObj **DAT_1f800224;
extern int FUN_800425c4(TObj *, TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern void FUN_8001e4f0(int);

extern void FUN_800449a0(TObj *, TObj *);
extern void FUN_800450e8(TObj *, TObj *);
extern void func_800447C0(TObj *, TObj *);
extern void func_8011DF3C(TObj *, TObj *);
extern void func_8011F330(TObj *, TObj *);
extern void func_8011F418(TObj *, TObj *);
extern void func_8011F41C(TObj *, TObj *);
extern void func_8011F518(TObj *, TObj *);
extern void func_8011F568(TObj *, TObj *);
extern void func_8011F5B4(TObj *, TObj *);
extern void func_8011FEC8(TObj *, TObj *);
extern void func_801202E4(TObj *, TObj *);
extern void func_801208C4(TObj *, TObj *);
extern void func_80120FC0(TObj *, TObj *);
extern void func_801213F0(TObj *, TObj *);
extern void func_80125868(TObj *, TObj *);
extern void func_801258E4(TObj *, TObj *);
extern void func_8012593C(TObj *, TObj *);
extern void func_80125984(TObj *, TObj *);
extern void func_80125994(TObj *, TObj *);
extern void func_80125A28(TObj *, TObj *);
extern void func_80125AE4(TObj *, TObj *);
extern void func_80125FBC(TObj *, TObj *);

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

void FUN_80046610(void)
{
    short n = DAT_1f800244;
    TObj **pp = DAT_1f800218;
    TObj **qq;
    TObj *a, *b;

    if (DAT_1f80024c == 0)
        return;
    while (n != 0) {
        a = *pp++;
        n--;
        if (!(a->active & 1) || a->ba5 == 0)
            continue;
        qq = DAT_1f800224;
        for (DAT_1f80019e = DAT_1f80024cu; DAT_1f80019e != 0;) {
            b = *qq++;
            DAT_1f80019e--;
            if (!(b->active & 1))
                continue;
            switch (b->type) {
            case 0:
                FUN_800449a0(a, b);
                break;
            case 1:
                func_80125A28(a, b);
                break;
            case 3:
                func_800447C0(a, b);
                break;
            case 4:
                if (D_8009C960 == 0) {
            case 2:
                    func_80125984(a, b);
                    break;
                }
                func_8011FEC8(a, b);
                break;
            case 5:
                func_80125AE4(a, b);
                break;
            case 0xb: case 0xc:
                func_801258E4(a, b);
                break;
            case 6: case 0x42: case 0x43:
                col(a, b);
                break;
            case 10:
                col(a, b);
                break;
            case 0x14:
                func_80125868(a, b);
                break;
            case 0x15:
                func_80125994(a, b);
                break;
            case 0xf:
                col(a, b);
                break;
            case 0x1c:
                func_801208C4(a, b);
                break;
            case 0x1d:
                func_8011DF3C(a, b);
                break;
            case 0x23:
                func_80125FBC(a, b);
                break;
            case 0x28:
                func_8011F5B4(a, b);
                break;
            case 0x1f:
                func_8011F568(a, b);
                break;
            case 0x2e:
                func_801202E4(a, b);
                break;
            case 0x2f:
                func_8012593C(a, b);
                break;
            case 0x24:
                func_8011F330(a, b);
                break;
            case 0x22:
                func_8011F418(a, b);
                break;
            case 0x33:
                func_80120FC0(a, b);
                break;
            case 0x34:
                func_8011F518(a, b);
                break;
            case 0x39:
                FUN_800450e8(a, b);
                break;
            case 0x31:
                func_801213F0(a, b);
                break;
            case 0x1e:
                func_8011F41C(a, b);
                break;
            }
        }
    }
}

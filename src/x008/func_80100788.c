// FUNC 80100788 4088 X008
// MATCHING 80100788 4088
/* Player step dispatcher. turn(): the d8c store written in every leaf (cross-jumping shares only the store
   tail, as in the game); chk() is a macro so the step constant is loaded at the compare; dirC/dirD store
   animFrame per leaf (game reloads it afterwards). */
#include "TOBJ.H"
#include "raw7.h"
extern unsigned char D_8009D2B0[];  /* [0]: keeps the store before the animFrame accesses */
extern unsigned short D_8009D670;
extern unsigned char *D_8009C330;
extern unsigned char D_801152E8[];
extern unsigned short D_1F8001FC, D_1F8003C6, D_1F8003C4;
extern int D_8009C984;
extern unsigned char D_8009C93A;
extern unsigned char D_1F8001A4;
extern void FUN_800f1478(TObj *);
extern void FUN_800f1d44(TObj *);
extern void FUN_8010e328(TObj *, int);
extern void FUN_800f2b98(TObj *);
extern void FUN_800f44b0(TObj *);
extern void FUN_800f36e0(TObj *);
extern void FUN_800f5078(TObj *);
extern void FUN_800f705c(TObj *);
extern void FUN_800f8d2c(TObj *);
extern void FUN_8010c034(TObj *);
extern void FUN_800f67bc(TObj *);
extern void FUN_800f3f3c(TObj *);
extern void FUN_800f7aac(TObj *);
extern void FUN_800f98a8(TObj *);
extern void FUN_801099b8(TObj *);
extern void FUN_80104698(TObj *);
extern void FUN_801066c4(TObj *);
extern void FUN_801075d8(TObj *);
extern void FUN_80108708(TObj *);
extern void FUN_80109038(TObj *);
extern void FUN_80106fec(TObj *);
extern void FUN_80109e50(TObj *);
extern void FUN_800f86e8(TObj *);
extern void FUN_80122f14(TObj *);
extern void FUN_8010b880(TObj *);
extern void FUN_80107c00(TObj *);
extern void FUN_801097ec(TObj *);
extern void FUN_800fa01c(TObj *);
extern void FUN_801237e4(TObj *);
extern void FUN_80121b58(TObj *);
extern void FUN_80107fcc(TObj *);
extern void FUN_80121790(TObj *);
extern void FUN_80122410(TObj *);
extern void FUN_8010ca18(TObj *);
extern void FUN_8011d7e0(TObj *);
extern void FUN_8011e688(TObj *);
extern void FUN_8011de64(TObj *);
extern void FUN_8011e0d8(TObj *);
extern void FUN_80121d6c(TObj *);
extern void FUN_80122044(TObj *);
extern void FUN_80106b50(TObj *);
extern void FUN_80106db0(TObj *);
extern void FUN_800f00ac(TObj *);
extern void FUN_80102a08(TObj *);
extern void FUN_800ffec4(TObj *);
extern void FUN_80108ad0(TObj *);
extern void FUN_800fb420(TObj *);
extern void FUN_8011e358(TObj *);
extern void FUN_8011e6b0(TObj *);
extern void FUN_80122c10(TObj *);
extern void FUN_8011eae8(TObj *);
extern void FUN_800fbdb4(TObj *, int);
extern void FUN_8011dc0c(TObj *);
extern void FUN_8011d9a4(TObj *);
extern void FUN_80106f98(TObj *);
extern void FUN_8010d390(TObj *);
extern void FUN_8010d048(TObj *);
extern void FUN_8010d888(TObj *);
extern void FUN_8011b330(TObj *);
extern void FUN_800fa91c(TObj *);
extern void FUN_8011b7d4(TObj *);
extern void FUN_8010db0c(TObj *);
extern void FUN_8010dd8c(TObj *);
extern void FUN_8010e66c(TObj *);
extern void FUN_8011beac(TObj *);
extern void FUN_8011b170(TObj *);
extern void FUN_80106a18(TObj *);
extern void FUN_8003f7cc(TObj *);
extern void FUN_800ff610(TObj *, int);
extern void FUN_8003f598(TObj *, int);

static __inline__ void dirA(TObj *o)
{
    unsigned short f = o->animFrame & 1;
    volatile unsigned short *k = &D_8009D670;
    unsigned short v;
    o->animFrame = f;
    if (*k & 0x20) {
        o->animFrame = 0;
    } else {
        if (*k & 0x80) v = 1; else v = f | 2;
        o->animFrame = v;
    }
}

static __inline__ void dirF(TObj *o)
{
    volatile unsigned short *k = &D_8009D670;
    if (*k & 0x10) o->animFrame |= 8;
    if (*k & 0x40) o->animFrame |= 4;
}

static __inline__ void dirC(TObj *o)
{
    volatile unsigned short *k = &D_8009D670;
    unsigned short f = o->animFrame & 1;
    o->animFrame = f;
    if (*k & 0x10) {
        if (*k & 0x80) o->animFrame = 5;
        else if (*k & 0x20) o->animFrame = 4;
        else if (f) o->animFrame = 7;
        else o->animFrame = 6;
    } else {
        if (*k & 0x80) o->animFrame = 3;
        else if (*k & 0x20) o->animFrame = 2;
        else if (f) o->animFrame = 3;
        else o->animFrame = 2;
    }
}

static __inline__ void dirD(TObj *o)
{
    volatile unsigned short *k = &D_8009D670;
    unsigned short f = o->animFrame & 1;
    o->animFrame = f;
    if (*k & 0x10) {
        if (*k & 0x80) o->animFrame = 5;
        else if (*k & 0x20) o->animFrame = 4;
        else if (f) o->animFrame = 7;
        else o->animFrame = 6;
    } else {
        if (*k & 0x80) o->animFrame = 1;
        else if (*k & 0x20) o->animFrame = 0;
        else if (f) o->animFrame = 3;
        else o->animFrame = 2;
    }
}

static __inline__ void turn(TObj *o)
{
    int a = o->d8c;
    unsigned int c = (unsigned char)(D_801152E8[o->wb0] - a);
    short s = c;
    unsigned short u = c;
    if (s != 0) {
        if (u < 0x80) {
            if (s >= 4) { o->d8c = a + 4; o->d8c = U8(o, 0x8c); }
            else if (s < 2) { o->d8c = a + 1; o->d8c = U8(o, 0x8c); }
            else { o->d8c = a + 2; o->d8c = U8(o, 0x8c); }
        } else {
            if (s < 0xfd) { o->d8c = a - 4; o->d8c = U8(o, 0x8c); }
            else if (s < 0xff) { o->d8c = a - 2; o->d8c = U8(o, 0x8c); }
            else { o->d8c = a - 1; o->d8c = U8(o, 0x8c); }
        }
    }
}

#define chk(o, n) \
    if (U8(o, 0xac) != 2 && o->step == n) { \
        if (o->b9e == 0) FUN_8010e328(o, 1); \
    }

void func_80100788(TObj *o, int p2)
{
    short c;

    switch (o->step) {
    case 0:
        D_8009D2B0[0] = 1;
        dirA(o);
        dirF(o);
        FUN_800f1478(o);
        turn(o);
        FUN_800f00ac(o);
        break;
    case 1:
        D_8009D2B0[0] = 1;
        dirA(o);
        dirF(o);
        FUN_800f1d44(o);
        if (D_1F8001FC & D_1F8003C6) {
            D_8009D2B0[0] = 0;
            o->b9c = 1;
            o->ba4 = 0;
            U8(o, 0xcd) = 0;
            U8(o, 0xce) = 0;
            if ((D_8009C984 & 0x40) && (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 0;
        }
        FUN_8010e328(o, 0);
        turn(o);
        FUN_800f00ac(o);
        break;
    case 2:
        dirA(o);
        FUN_800f2b98(o);
        chk(o, 2);
        break;
    case 3:
        FUN_800f44b0(o);
        turn(o);
        if (U8(o, 0xac) < 2) FUN_800f00ac(o);
        break;
    case 4:
        if (o->b9d == 0) {
            dirC(o);
            D_8009C330[7] = o->animFrame;
        }
        FUN_800f36e0(o);
        turn(o);
        break;
    case 5:
        FUN_800f5078(o);
        break;
    case 6:
        FUN_800f705c(o);
        break;
    case 7:
        FUN_800f8d2c(o);
        break;
    case 8:
        FUN_8010c034(o);
        break;
    case 9:
        FUN_800f67bc(o);
        break;
    case 10:
        dirD(o);
        FUN_800f3f3c(o);
        break;
    case 11:
        FUN_800f7aac(o);
        break;
    case 12:
        FUN_800f98a8(o);
        break;
    case 13:
        FUN_801099b8(o);
        break;
    case 14:
        FUN_80104698(o);
        break;
    case 15:
        FUN_801066c4(o);
        break;
    case 16:
        FUN_801075d8(o);
        break;
    case 17:
        FUN_80108708(o);
        break;
    case 18:
        FUN_80109038(o);
        break;
    case 19:
        D_8009D2B0[0] = 1;
        FUN_80106fec(o);
        turn(o);
        break;
    case 0x15:
        FUN_80109e50(o);
        break;
    case 0x17:
        FUN_80122f14(o);
        break;
    case 0x18:
        FUN_8010b880(o);
        turn(o);
        break;
    case 0x1b:
        FUN_80107c00(o);
        break;
    case 0x1c:
        D_8009D2B0[0] = 1;
        dirA(o);
        dirF(o);
        FUN_801097ec(o);
        if (D_1F8001FC & D_1F8003C6) {
            D_8009D2B0[0] = 0;
            o->b9c = 1;
            U8(o, 0xc3) = 0;
            o->step = 2;
            o->state = 0;
        }
        FUN_8010e328(o, 0);
        turn(o);
        FUN_800f00ac(o);
        break;
    case 0x1d:
        FUN_800fa01c(o);
        break;
    case 0x1e:
        FUN_801237e4(o);
        break;
    case 0x1f:
        D_8009D2B0[0] = 1;
        FUN_80121b58(o);
        FUN_8010e328(o, 0);
        break;
    case 0x20:
        FUN_80107fcc(o);
        break;
    case 0x21:
        FUN_80121790(o);
        break;
    case 0x22:
        FUN_80122410(o);
        break;
    case 0x24:
        FUN_8011d7e0(o);
        break;
    case 0x25:
        FUN_8011e688(o);
        break;
    case 0x26:
        FUN_8011de64(o);
        break;
    case 0x27:
        FUN_8011e0d8(o);
        break;
    case 0x28:
        FUN_80121d6c(o);
        break;
    case 0x29:
        FUN_80122044(o);
        break;
    case 0x2a:
        FUN_80106b50(o);
        turn(o);
        FUN_800f00ac(o);
        break;
    case 0x2b:
        FUN_80106db0(o);
        turn(o);
        FUN_800f00ac(o);
        break;
    case 0x2e:
        FUN_80102a08(o);
        break;
    case 0x23:
    case 0x2f:
        FUN_8010ca18(o);
        break;
    case 0x30:
        FUN_800ffec4(o);
        break;
    case 0x31:
        FUN_80108ad0(o);
        break;
    case 0x32:
        FUN_800fb420(o);
        break;
    case 0x33:
        FUN_8011e358(o);
        break;
    case 0x34:
        FUN_8011e6b0(o);
        break;
    case 0x35:
        FUN_80122c10(o);
        break;
    case 0x36:
        FUN_8011eae8(o);
        break;
    case 0x37:
        FUN_800fbdb4(o, p2);
        chk(o, 0x37);
        break;
    case 0x3a:
        FUN_8011dc0c(o);
        break;
    case 0x3b:
        FUN_8011d9a4(o);
        break;
    case 0x3c:
        FUN_80106f98(o);
        break;
    case 0x3d:
        D_8009D2B0[0] = 1;
        FUN_8010d390(o);
        break;
    case 0x3e:
        D_8009D2B0[0] = 1;
        FUN_8010d048(o);
        FUN_8010d888(o);
        break;
    case 0x3f:
        dirA(o);
        FUN_8011b330(o);
        break;
    case 0x40:
        FUN_800fa91c(o);
        break;
    case 0x41:
        dirD(o);
        FUN_8011b7d4(o);
        break;
    case 0x43:
        dirA(o);
        FUN_8010db0c(o);
        chk(o, 0x43);
        break;
    case 0x44:
        FUN_8010dd8c(o);
        break;
    case 0x45:
        FUN_8010e66c(o);
        break;
    case 0x46:
        FUN_8011beac(o);
        break;
    case 0x47:
        FUN_8011b170(o);
        break;
    case 0x48:
        FUN_80106a18(o);
        break;
    case 0x49:
    case 0x16:
        FUN_800f86e8(o);
        break;
    case 0x63:
        switch (o->state) {
        case 0:
            o->timer = 0x3c;
            o->state++;
        case 1:
            if (--o->timer <= 0) {
                o->b04 = 1;
                o->step = 0;
                o->state = 0;
                o->substep = 0;
                o->active = 1;
                D_8009D2B0[0] = 0;
                D_8009C93A = 1;
            }
            break;
        }
        break;
    }
    c = 0;
    *(unsigned char *)&o->wa8 = 0;
    o->ba6 = 0;
    if (o->b9e == 0 && o->b04 == 5) {
        c = o->step == 0x40;
        if (o->step == 0x41) c++;
        if (o->step == 0x65) c++;
    }
    if (c == 0) FUN_8003f7cc(o);
    if (D_1F8001A4 == 0) FUN_800ff610(o, 1);
    FUN_8003f598(o, 4);
}

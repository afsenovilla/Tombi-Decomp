// FUNC 801066c4 852 X014
// MATCHING 801066c4 852
#include "TOBJ.H"
typedef struct { char pad[9]; unsigned char b9; char pad2[0x20 - 10]; short w20; } P;
extern P *D_8009C330;
extern TObj *D_8009D2E8;
extern int D_8009C984;
extern unsigned short D_8009D670;
extern void FUN_8010f328(TObj *);
extern void FUN_800ef490(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_800f262c(TObj *);
extern void func_80104F88(TObj *);
extern void func_80105BCC(TObj *);
extern void func_80105694(TObj *);
extern void FUN_801064d0(TObj *);
extern void func_8011D3EC(TObj *);
extern void func_801213D0(TObj *);

void func_801066C4(TObj *o)
{
    TObj *q;
    switch (o->state) {
    case 0:
        switch (D_8009D2E8->type) {
        case 19:
            D_8009C330->w20 = 0;
            D_8009D2E8->d84 = 0;
            FUN_8010f328(o);
            break;
        case 0: case 2: case 8: case 10: case 11: case 18: case 30: case 33: case 40:
        case 56: case 58: case 59: case 60: case 61: case 62: case 63: case 64: case 65: case 66:
            D_8009D2E8->animFrame = o->animFrame & 1;
        case 4: case 26:
            q = D_8009D2E8;
            D_8009C330->w20 = 0;
            {
            int t;
            Fix16 *dst = q->h;
            t = o->h->p.whole;
            dst->p.whole = (o->animFrame & 1) ? t + 8 : t - 8;
            }
            goto tail;
        case 3: case 28: case 31: case 43:
            D_8009C330->w20 = 0;
            D_8009D2E8->animFrame = o->animFrame & 1;
            {
            int t = o->h->p.whole;
            D_8009D2E8->h->p.whole = (o->animFrame & 1) ? t + 8 : t - 8;
            }
            goto tail;
        case 41:
            if (D_8009D2E8->b6a != 1) goto e41;
        case 14:
            D_8009C330->w20 = 13;
            o->velY = -0x600;
            break;
        e41:
            D_8009C330->w20 = 0;
            {
            int t = o->h->p.whole;
            D_8009D2E8->h->p.whole = (o->animFrame & 1) ? t + 8 : t - 8;
            }
        tail:
            D_8009D2E8->y.p.whole = o->y.p.whole + 8;
            D_8009D2E8->d8c = o->d88 - 0xc0;
            FUN_8010f328(o);
            break;
        }
        FUN_800ef490(o);
        o->d88 = 0x1c0;
        o->d8c = 0;
        o->wb2 = 0;
        o->wb0 = 0;
        D_8009C330->b9 = D_8009D2E8->type;
        o->b69 = 0;
        o->velV = 0;
        PlayerSetAnimIfChanged(o, 13);
        if ((D_8009C984 & 0x40) && (*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C4)) {
            o->ba7 = 1;
        } else {
            o->ba7 = 0;
        }
        o->state = 1;
        o->substep = 0;
    case 1:
        if (D_8009C330->b9 != 14) {
            FUN_800f262c(o);
        }
    case 2:
        switch (D_8009C330->b9) {
        case 0: case 2: case 8: case 10: case 11: case 30: case 33: case 40:
        case 56: case 58: case 59: case 60: case 61: case 62: case 63: case 64: case 65: case 66:
            func_80104F88(o);
            break;
        case 3: case 18: case 28: case 31: case 43:
            func_80105BCC(o);
            break;
        case 41:
            if (D_8009D2E8->b6a == 1) {
                FUN_801064d0(o);
                break;
            }
        case 4:
            func_80105694(o);
            break;
        case 14:
            FUN_801064d0(o);
            break;
        case 26:
            func_8011D3EC(o);
            break;
        case 19:
            func_801213D0(o);
            break;
        }
        break;
    }
}

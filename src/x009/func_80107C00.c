// FUNC 80107c00 972 X009
// MATCHING 80107c00 972
#include "TOBJ.H"

#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
extern unsigned char *D_8009C330;
extern unsigned short D_8009D670;
extern int D_8009F0EC;
extern char D_80010AE8[];
void func_800EEA3C(TObj *o);
void FUN_800eea7c(TObj *o, int a, int b);
void FUN_80110eec(TObj *o);
void FUN_800ee9cc(TObj *o);
int func_8004BBC0(TObj *o, int a);
void func_800EFC8C(TObj *o, int a);
void PlayerSetAnimIfChanged(TObj *o, int a);
void FUN_800ee428(TObj *o);
void AnimJump(TObj *o, int a);
void FUN_800f00ac(TObj *o);

void func_80107C00(TObj *o)
{
    volatile unsigned short *k;
    switch (o->state) {
    case 0:
        D_8009C330[0xb] = o->animFrame;
        if ((o->animFrame &= 1) != 0) {
            if (o->wb2 > 0) o->wb2 = -o->wb2;
        } else {
            if (o->wb2 < 0) o->wb2 = -o->wb2;
        }
        B(o, 0xac) = 0;
        o->b9c = 0;
        o->b6b = 0;
        B(o, 0xc7) = 1;
        o->b9d = 0;
        B(o, 0xc6) = 0;
        B(o, 0xe3) = 0;
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        D_8009C330[0] = 0;
        o->ba4 = 1;
        func_800EEA3C(o);
        o->state++;
    case 1:
        k = &D_8009D670;
        if (*k & 0xa0) {
            if (*k & 0x80) o->animFrame = 1;
            if (*k & 0x20) o->animFrame = 0;
        }
        if (o->bbe & 1) {
            if (!(o->animFrame & 1)) {
                FUN_800eea7c(o, 8, 2);
                o->d8c = 0xe0;
            } else {
                FUN_800eea7c(o, 0x11, 0);
                o->d8c = 0;
            }
        } else {
            if (o->animFrame & 1) {
                FUN_800eea7c(o, 8, 2);
                o->d8c = 0x20;
            } else {
                FUN_800eea7c(o, 0x11, 0);
                o->d8c = 0;
            }
        }
        o->y.raw += 0x80000;
        FUN_80110eec(o);
        FUN_800ee9cc(o);
        if (o->bbe & 2) {
            o->step = 0x13;
            o->state = 0;
            o->substep = 0;
        }
        if (o->bbe == 0) {
            if (*(unsigned short *)(D_8009C330 + 0x2c) == 8 && B(o, 0xac) < 2) {
                D_8009F0EC = func_8004BBC0(o, 1);
                if (D_8009F0EC) {
                    D_8009C330[8] = 0;
                    *(short *)(D_8009C330 + 0x20) = 0;
                    B(o, 0xac) = 0;
                    o->b9c = 0;
                    o->wb2 = 0;
                    func_800EFC8C(o, D_8009F0EC == 1);
                }
            }
            if (o->animFrame != (o->bbe & 1)) o->wb2 = 0;
            o->ba4 = 0;
            o->wb6 = 0;
            PlayerSetAnimIfChanged(o, 4);
            if (D_8009C330[0] == 0) {
                o->velX = 0;
                o->velY = 0;
                B(o, 0xac) = 1;
                o->y.p.whole -= 4;
                FUN_800ee428(o);
                k = &D_8009D670;
                if (*k & 0x80) o->animFrame = 1;
                if (*k & 0x20) o->animFrame = 0;
                o->anim = D_80010AE8;
                AnimJump(o, 3);
                o->step = 2;
                o->state = 3;
                o->substep = 0;
                break;
            }
            o->velX = 0;
            o->velY = 0;
            FUN_800ee428(o);
            k = &D_8009D670;
            if (*k & 0x80) o->animFrame = 1;
            if (*k & 0x20) o->animFrame = 0;
            o->step = 4;
            o->state = 3;
            o->substep = 0;
        }
        o->velX = 0;
        o->velY = 0;
        FUN_800f00ac(o);
        break;
    }
}

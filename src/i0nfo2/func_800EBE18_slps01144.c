// VERSION SLPS_01144 info2 800EBE18 1004
// JP version of PAL i0nfo2 800ea69c (topdownPlayerItemUseStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void func_80027C18(int item, int n);
extern TObj *D_8009DEE0;
extern unsigned char D_8009E4E7[];
extern unsigned char D_8009E4E8, D_8009E4E9;
extern unsigned char D_8009E4EA[];
extern unsigned char D_8009E9FD;
extern unsigned char D_8009E9FF;
extern unsigned char D_8009EABB;
extern unsigned char D_8009EE58[];
extern unsigned char D_8009EE59;
extern unsigned char D_8009E977, D_8009E9F3, D_8009EB6E, D_8009EB6F, D_8009EB7F;
extern struct { unsigned short selectedArea; } D_8009E508;

#define CHK9(o) ({ short r = 0; \
    if (D_8009E977 != 0xFF && D_8009E508.selectedArea == 5 \
        && o->h->p.whole >= 0x90 && o->h->p.whole < 0xC0 && o->y.p.whole >= 0xB6 && o->y.p.whole < 0xCA) { \
        r = 1; \
        D_8009EB6E = 1; \
    } \
    r; })

void func_800EBE18(TObj *o)
{
    switch (o->state) {
    case 0:
        o->ba4 = 0;
        o->ba5 = 0;
        o->ba7 = 0;
        o->b9c = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        U8(D_8009DEE0, 8) = 0;
        D_8009DEE0->timer = 0;
        o->d8c = 0;
        S16(o, 0xD0) = 0;
        o->w22 = 0;
        o->animFrame |= 8;
        overheadPlayerSetMoveAnim(o);
        o->state++;
    case 1:
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        if (D_8009E4E8 == 0) break;
        switch (D_8009E4E9) {
        case 0xF:
            D_8009E9FD = 5;
            func_80027C18(D_8009E4E9, 1);
            D_8009E4E8 = 0;
            break;
        case 0x9A:
            D_8009EABB = 0;
            D_8009E4E8 = 0;
            break;
        case 0x11:
            D_8009E9FF = 4;
            func_80027C18(D_8009E4E9, 1);
            D_8009E4E8 = 0;
            break;
        case 9:
            if (!CHK9(o)) D_8009EE59 = 0;
            func_80027C18(D_8009E4E9, 1);
            D_8009E4E8 = 0;
            break;
        case 0x1E:
        case 0x1F:
            if (*(int *)&D_8009E508.selectedArea == 0x20013 && o->h->p.whole >= 6 && o->h->p.whole < 0x36
                && o->y.p.whole >= 0xE9 && o->y.p.whole < 0x149) {
                D_8009EB7F = 1;
                D_8009E4E8 = 0;
            }
            break;
        case 0x23:
            if (D_8009E9F3 != 0xFF && D_8009E508.selectedArea == 5
                && o->h->p.whole >= 0x90 && o->h->p.whole < 0xC0 && o->y.p.whole >= 0xB6 && o->y.p.whole < 0xCA) {
                D_8009EB6F = 1;
            }
            func_80027C18(D_8009E4E9, 1);
            D_8009E4E8 = 0;
            break;
        case 5:
        case 0x7C:
        case 0x97:
        case 0x98:
            D_8009E4E8 = 0;
            o->b04 = 4;
            o->step = 4;
            o->state = 0;
            o->substep = 1;
            D_8009E4E7[0] = 1;
            D_8009E4EA[0] = 1;
            D_8009EE58[0] = 3;
            break;
        case 7:
            D_8009E4E8 = 0;
            o->b04 = 4;
            o->step = 4;
            o->state = 0;
            o->substep = 0;
            D_8009E4E7[0] = 1;
            D_8009E4EA[0] = 1;
            D_8009EE58[0] = 3;
            func_80027C18(D_8009E4E9, 1);
            break;
        case 0xE:
            o->active = 4;
            o->ba4 = 0;
            o->ba5 = 0;
            o->b9c = 1;
            o->visible = 1;
            o->ba7 = 0;
            o->active = 4;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            D_8009DEE0->timer = 0;
            o->timer = 0x1E;
            o->d8c = 0;
            o->w22 = 0;
            D_8009EE58[0] = 3;
            D_8009E4E7[0] = 1;
            D_8009E4EA[0] = 1;
            o->step = 0x30;
            o->d8c = 0;
            o->b04 = 1;
            o->state = 2;
            o->substep = 0;
            break;
        case 3:
        case 0xA:
        case 0x10:
        case 0x12:
        default:
            D_8009E4E8 = 0;
            break;
        }
        break;
    }
}
#endif

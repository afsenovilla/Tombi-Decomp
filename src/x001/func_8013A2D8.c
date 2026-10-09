// FUNC 8013a2d8 460 X001
// MATCHING 8013a2d8 460
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned short D_800A6066;
extern Fix16 *D_800A6078;
extern char D_80010748[];
extern unsigned char D_8009CEAB;
extern TObj *D_800A60C8;
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern unsigned char D_800A60F8;
extern void AnimLoadDuration(TObj *o);
extern TObj *FUN_8002dc50(int, int, int, int);

void func_8013A2D8(TObj *o)
{
    TObj *p = *(TObj **)&o->category;
    int v;
    int t;

    switch (o->step) {
    case 0:
        D_800A6038.anim = D_80010748;
        AnimLoadDuration(&D_800A6038);
        switch (D_8009CEAB) {
        case 1:
            D_800A60C8 = FUN_8002dc50(2, 0x1f, 0x80, 0x6c);
            break;
        case 2:
            D_800A60C8 = FUN_8002dc50(2, 0x20, 0x80, 0x6c);
            break;
        case 3:
            D_800A60C8 = FUN_8002dc50(2, 0x21, 0x80, 0x6c);
            break;
        default:
            return;
        }
        t = *(short *)((char *)D_800A6078 + 2) > *(short *)((char *)p->h + 2);
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        D_800A6066 = t;
        o->step++;
        break;
    case 1:
        if (D_800A60C8->b04 == 2) {
            v = D_8009CEAB;
            if (v == 1 || (v != 0 && v < 4)) {
                D_800A60C8->b04 = 3;
                o->step = 0;
                o->state = 0;
                t = *(short *)((char *)D_800A6078 + 2) > *(short *)((char *)p->h + 2);
        D_800A603C = 1;
                D_800A603D = 0;
                D_800A603E = 0;
                o->animTimer = 0;
                D_8009C93F = 0;
                D_8009C93E = 0;
                D_8009C942 = 0;
                D_800A60F8 = 0;
            }
        }
        break;
    }
}

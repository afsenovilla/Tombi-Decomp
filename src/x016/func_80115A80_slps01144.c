// VERSION SLPS_01144 x16 80115A80 680
// NOTE: 93 % (register choice for the constant 1 and the case-0 load order differ); stalled.
// JP-only object handler (cutscene: player placed, camera scroll counter at +0x74 ramps to 0x4000).
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { short pad[0x26]; short s4c, s4e; } CamObj;
extern unsigned char D_800A7B78, D_800A7B79, D_800A7B7C, D_800A7B7D;
extern unsigned char D_8009E4E7, D_8009E4E6, D_8009E4EA, D_8009E4E4, D_8009E51D, D_800A609B;
extern unsigned char D_8009FF05, D_8009FF06, D_8009FF07, D_800A0C15, D_800A0C16, D_800A0C17;
extern int D_800A60B0;
extern short D_8009E93C, D_8009E93E, D_8009E948;
extern int D_1F800190, D_1F80018C;
extern CamObj *D_1F8001D4;
extern int func_801157DC_slps01144(TObj *);

void func_80115A80_slps01144(TObj *o)
{
    int v, y;

    switch (o->state) {
    case 0:
        if (o->visible == 0) break;
        if (!func_801157DC_slps01144(o)) break;
        o->state++;
        D_800A7B78 = 2;
        D_8009E4E7 = 1;
        D_8009E4E6 = 1;
        D_8009E4EA = 1;
        D_800A609B = 3;
        D_800A60B0 = 0;
        y = o->y.raw;
        v = **(int **)&o->h;
        D_1F800190 = y;
        o->timer = 10;
        D_1F80018C = v;
        break;
    case 1:
        if (--o->timer != -1) break;
        D_800A7B78 = 5;
        D_800A7B7C = 5;
        D_8009E4E6 = 0;
        D_800A7B79 = 0;
        D_800A7B7D = 0x40;
        o->state++;
        o->timer = 0x14;
        D_8009E93C = 0xe;
        D_8009E93E = 2;
        D_8009E948 = 0;
        break;
    case 2:
        if (o->timer == 0) {
            D_8009E51D = 3;
            D_8009E4E4 = 1;
            o->state++;
        } else {
            o->timer--;
        }
        o->w74 += 0x80;
        if (o->w74 > 0x4000) {
            o->w74 = 0x4000;
            o->state++;
        }
        o->w76 = o->w74;
        o->w78 = o->w74;
        break;
    case 3:
        o->w74 += 0x80;
        if (o->w74 > 0x4000) o->w74 = 0x4000;
        o->w76 = o->w74;
        o->w78 = o->w74;
        if (D_8009E51D == 1) {
            D_1F8001D4->s4c = 7;
            D_1F8001D4->s4e = 0;
            D_8009FF05 = 0xff;
            D_8009FF06 = 0xff;
            D_8009FF07 = 0xff;
            D_800A0C15 = 0xff;
            D_800A0C16 = 0xff;
            D_800A0C17 = 0xff;
        }
        break;
    }
}
#endif

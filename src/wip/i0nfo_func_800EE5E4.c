// FUNC 800ee5e4 440 I0NFO
#include "TOBJ.H"
extern TObj *func_8002DC50(int a, int b, int x, int y);
extern signed char D_8009C93F, D_8009C942, D_8009D2B0;
extern signed char D_800A603C, D_800A603D, D_800A603E;
extern unsigned short D_800A6066;
extern unsigned char D_800A60A1, D_800A60A3;
extern signed char D_800A60F8;

static inline int func_800EE5E4_start(TObj *o)
{
    int ok;
    ok = 0;
    if (D_800A60A1 == 2 && (D_800A6066 & 7) == D_800A60A1) {
        o->d90 = (int)func_8002DC50(1, 1, 0xA0, 0x8C);
        D_8009D2B0 = 2;
        D_8009C942 = 1;
        D_8009C93F = 1;
        D_800A603C = 4;
        *(short *)0x1F8001C6 = 0;
        D_800A60F8 = 0;
        D_800A603D = 6;
        D_800A603E = 0;
        o->state++;
        ok = 1;
    }
    return ok;
}

void func_800EE5E4(TObj *o)
{
    o->visible = D_800A60A3 != 0;
    switch (o->state) {
    case 0:
        if (o->b68 != 0) {
            if (!func_800EE5E4_start(o)) o->state = 2;
        }
        o->b69 = 0;
        o->b68 = 0;
        break;
    case 1:
        if (((TObj *)o->d90)->b04 >= 2) {
            ((TObj *)o->d90)->b04++;
            o->state++;
        }
        break;
    case 2:
        D_8009C942 = 0;
        D_8009C93F = 0;
        o->b68 = 0;
        o->b69 = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        *(short *)0x1F8001C6 = 0;
        D_800A60F8 = 0;
        o->state = 0;
        break;
    }
}

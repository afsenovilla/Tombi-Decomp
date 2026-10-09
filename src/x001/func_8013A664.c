// FUNC 8013a664 616 X001
// MATCHING 8013a664 616
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009CE59, D_8009CEDE, D_8009C942, D_8009C93F, D_800A60F8;
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC, D_1F8003C4;

#define W24(o) (((unsigned short *)(o))[0x12])

int func_8013A664(TObj *o)
{
    TObj *q = *(TObj **)&o->category;
    int r = 0;

    switch (D_8009CE59) {
    case 1:
        switch (D_8009CEDE) {
        case 0:
            if (D_800A6038.h->p.whole >= 0x19b)
                r = 1;
            break;
        case 1:
            r = 1;
            break;
        case 2:
            r = 1;
            break;
        }
        break;
    case 2:
        if (D_800A6038.b69 == 0)
            break;
        if ((unsigned short)(D_800A6038.h->p.whole - q->h->p.whole + 0x20) >= 0x40)
            break;
        if ((unsigned short)(D_800A6038.y.p.whole + 0x6d4) >= 4)
            break;
        if (W24(o) != 0) {
            r = 1;
            break;
        }
        if (D_8009D2B0 == 1 && (D_1F8001FC & D_1F8003C4)) {
            D_8009C942 = 1;
            D_800A60F8 = 1;
            r = 1;
            W24(o) = 1;
        }
        break;
    case 3:
        if (D_800A6038.b04 != 1)
            break;
        if (D_800A6038.b69 == 0)
            break;
        if ((unsigned short)(D_800A6038.h->p.whole - q->h->p.whole + 0x20) >= 0x40)
            break;
        if (D_800A6038.y.p.whole < -0x17c)
            break;
        if (W24(o) != 0) {
            r = 1;
            break;
        }
        if (D_8009D2B0 == 1 && (D_1F8001FC & D_1F8003C4)) {
            r = 1;
            D_8009C942 = 1;
            D_800A60F8 = 1;
            D_8009C93F = 1;
            W24(o) = 1;
        }
        break;
    case 0xff:
        r = 1;
        break;
    }
    return r;
}

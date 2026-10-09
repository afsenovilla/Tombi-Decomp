// FUNC 8012d678 468 X010
// MATCHING 8012d678 468
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009C93F;
extern void FUN_800eea7c(TObj *, int, int);

void func_8012D678(TObj *o)
{
    switch (o->state) {
    case 0:
        if (D_800A6038.y.p.whole >= -0xff && D_800A6038.h->p.whole < 0xcb) {
            *(signed char *)&D_800A6038.b0f = 8;
            D_8009C93F = 1;
            D_800A6038.y.p.whole = -0xe8;
            FUN_800eea7c(&D_800A6038, 0x1e, 0);
            D_800A6038.b04 = 5;
            D_800A6038.d8c = 0;
            D_800A6038.step = 0x65;
            D_800A6038.state = 0;
            o->state++;
        }
        break;
    case 1:
        D_800A6038.h->raw += -0x20000;
        if (D_800A6038.h->p.whole < 0x96) {
            D_800A6038.velY = 0;
            o->state++;
        }
        break;
    case 2:
        D_800A6038.h->raw += -0x20000;
        D_800A6038.y.raw += D_800A6038.velY << 8;
        D_800A6038.velY += 0x40;
        D_800A6038.d8c++;
        if (D_800A6038.y.p.whole >= -0x63) o->state = 9;
        break;
    case 9:
        D_8009C93F = 0;
        break;
    }
}

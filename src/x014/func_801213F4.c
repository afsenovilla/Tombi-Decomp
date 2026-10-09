// FUNC 801213f4 444 X014
// MATCHING 801213f4 444
#include "TOBJ.H"

extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern unsigned char D_801265BC[];
extern int Rand(void);
extern void FUN_800202b4(TObj *);

void func_801213F4(TObj *o)
{
    short v;

    switch (o->step) {
    case 0:
        o->timer = (Rand() & 0x30) + 0x5a;
        o->step++;
        o->velY = D_801265BC[o->subtype];
        if (o->b0c) {
            v = o->velH;
            if (v < 0) o->velH = v - 0x80;
            else o->velH = v + 0x80;
        }
        break;
    case 1:
        o->visible = 1;
        o->h->raw += o->velH << 8;
        o->velV += o->velY;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->animFrame) o->d8c = (o->d8c + 0x80) & 0xfff;
        else o->d8c = (o->d8c - 0x80) & 0xfff;
        if (o->timer < 0x28) {
            if ((D_1F8001F8 + D_1F800198) & 1) o->visible = 1;
            else o->visible = 0;
        }
        if (--o->timer == -1) o->b04++;
        break;
    }
    if (o->visible) FUN_800202b4(o);
}

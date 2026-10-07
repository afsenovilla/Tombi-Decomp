// FUNC 8002eddc 116 MAIN0
#include "TOBJ.H"
extern int DAT_1f8002d8;
extern void *PTR_80012208[];
extern void func_8001fe6c(TObj *o);

void FUN_8002eddc(TObj *o)
{
    o->active = 1;
    o->type = 0x32;
    o->animFrame = 1;
    *(signed char *)&o->b0f = -30;
    o->w1e = 0x14;
    o->b0d = 0x80;
    o->b0b = 0;
    o->d3c = DAT_1f8002d8;
    o->anim = PTR_80012208[*(short *)((char *)o + 0xac)];
    func_8001fe6c(o);
}

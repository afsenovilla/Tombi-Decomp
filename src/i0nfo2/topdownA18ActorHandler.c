// FUNC 800f548c 260 I0NFO2
// MATCHING 800f548c 260
#include "TOBJ.H"
extern void func_80018790(TObj *o);
extern void func_8001FE6C(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void func_800202B4(TObj *o);
extern void topdownA18ActorInit(TObj *o);
extern void **D_800FEFA8[][3];

void topdownA18ActorHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        topdownA18ActorInit(o);
        break;
    case 1:
        func_800202B4(o);
        if (*(unsigned short *)0x1F8001FC & 0x1000) {
            o->wb4 = (o->wb4 + 1) & 7;
            o->anim = D_800FEFA8[o->subtype][0][(unsigned short)o->wb4];
            func_8001FE6C(o);
        }
        func_8001FEC0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}

// VERSION SLPS_01144 x04 80127CAC 448
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern unsigned char D_8009E9CA;
extern unsigned char D_8009E4E8;
extern unsigned char D_8009E4E9;
extern unsigned char D_8009E9E5;
extern short PLAYER_Y;
extern void func_80127B28_slps01144(TObj *);
extern void ObjCullRegister(TObj *);
extern void func_801268A4(TObj *);
extern void func_80126AA0(TObj *);
extern void mermaidDivingLesson(TObj *);
extern void AnimAdvance(TObj *);
extern void freeObjectLayer2(TObj *);

void func_80127CAC_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80127B28_slps01144(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (D_8009E9CA) {
                if (D_8009E4E8 && D_8009E4E9 == 0x94) {
                    o->step = 2;
                    o->state = 0;
                }
                if (PLAYER_Y >= -168 && D_8009E9E5 && D_8009E9E5 != 255) {
                    o->step = 3;
                    o->state = 0;
                }
                if (o->b68) {
                    o->animFrame = o->b68 & 1;
                    o->step = 1;
                    o->state = 0;
                }
            }
            break;
        case 1:
            func_801268A4(o);
            break;
        case 2:
            func_80126AA0(o);
            break;
        case 3:
            mermaidDivingLesson(o);
            break;
        }
        AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif

// FUNC 800fdb34 368 I0NFO2
// MATCHING 800fdb34 368
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void spawnNpcSet0F(TObj *o);
extern void func_800FC468(TObj *o);
extern void func_800FC844(TObj *o);
extern void func_800FC9E4(TObj *o);
extern void func_800FD0E8(TObj *o);
extern void func_800FD294(TObj *o);
extern void func_800FD3C0(TObj *o);
extern void func_800FD524(TObj *o);
extern void func_800FD688(TObj *o);
extern void func_800FD8B0(TObj *o);

void updateNpcSet0F(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet0F(o);
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            func_800FD8B0(o);
            break;
        case 1:
            func_800FC468(o);
            break;
        case 2:
            func_800FC844(o);
            break;
        case 3:
            func_800FC9E4(o);
            break;
        case 4:
            func_800FD688(o);
            break;
        case 5:
        case 6:
        case 7:
            if (o->state == 0) {
                o->step = 0;
                o->state = 0;
            }
            break;
        case 8:
            func_800FD0E8(o);
            break;
        case 9:
            func_800FD294(o);
            break;
        case 10:
            func_800FD3C0(o);
            break;
        case 11:
            func_800FD524(o);
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}

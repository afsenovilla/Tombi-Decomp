// FUNC 800f1590 268 I0NFO2
// MATCHING 800f1590 268
#include "TOBJ.H"
extern void func_800187E4(TObj *o);
extern void func_80018CA4(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void func_800F13E0(TObj *o);
extern unsigned char D_800A60A3;

void func_800F1590(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_800F13E0(o);
        break;
    case 1:
        if (D_800A60A3 != 0) {
            if (D_800A60A3 == *(unsigned char *)&o->wb6) o->b6b = 0x80;
            else o->b6b = 0;
        } else {
            if ((unsigned short)o->wb6 != 0) o->b6b = 0;
            else o->b6b = 0x80;
        }
        func_8001FEC0(o);
        o->visible = 1;
        func_80018CA4(o);
        o->visible = 1;
        func_80018CA4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_800187E4(o);
        break;
    }
}

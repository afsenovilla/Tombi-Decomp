// FUNC 800f075c 100 I0NFO2
// MATCHING 800f075c 100
#include "TOBJ.H"
extern void func_80018CA4(TObj *o);
extern unsigned char D_800A60A3;

void topdownA05SceneryUpdate(TObj *o)
{
    if (D_800A60A3 != 0) {
        if (D_800A60A3 == *(unsigned char *)&o->wb6) o->b6b = 0x80;
        else o->b6b = 0;
    } else {
        if ((unsigned short)o->wb6 != 0) o->b6b = 0;
        else o->b6b = 0x80;
    }
    o->visible = 1;
    func_80018CA4(o);
}

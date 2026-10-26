// FUNC 800f6d5c 452 I0NFO2
// MATCHING 800f6d5c 452
#include "TOBJ.H"
typedef struct {
    unsigned short w1e;
    short _p2;
    short spad;
    short _p6;
    void **anims;
} ObjInit3;
extern ObjInit3 D_800FF054[];
extern int SPR_DATA[];
extern void func_8001FE6C(TObj *o);
extern void func_80018CA4(TObj *o);
extern void func_800187E4(TObj *o);
extern unsigned char D_800A60A3;

void topdownA11SceneryHandler(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = D_800FF054[o->subtype].w1e;
        o->d3c = SPR_DATA[D_800FF054[o->subtype].spad];
        o->b0d = 1;
        o->anim = D_800FF054[o->subtype].anims[(unsigned short)o->wb4];
        func_8001FE6C(o);
        o->d8c = 0;
        if ((unsigned short)o->wb6 != 0) {
            o->b6b = 0;
            o->w08 = 0x7F48;
        } else {
            o->b6b = 0x80;
            o->w08 = 0x7FC8;
        }
        *(signed char *)&o->b0f = -0xC;
        o->category |= 0x80;
        o->b04++;
        break;
    case 1:
        if (D_800A60A3 != 0) {
            if (D_800A60A3 == *(unsigned char *)&o->wb6) o->b6b = 0x80;
            else o->b6b = 0;
        } else {
            if ((unsigned short)o->wb6 != 0) o->b6b = 0;
            else o->b6b = 0x80;
        }
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

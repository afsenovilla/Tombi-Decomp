// FUNC 80039c90 204 MAIN0
// MATCHING 80039c90 204
#include "TOBJ.H"
#include "scriptctx.h"
extern TObj *SCRIPT_OBJECTS[];
extern void func_80113754(o);
extern void func_800ECBBC(o);
extern void func_800E8348(o);

void scriptOpSetObjField74(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    TObj *o;
    int b, a;
    o = SCRIPT_OBJECTS[g->arg[0]];
    a = g->arg[1];
    b = g->arg[2];
    if (o != 0) {
        o->w74 = a;
        o->w76 = b;
        switch (o->type & 0x7f) {
        case 0x18:
            func_80113754(o);
            break;
        case 0x19:
            func_800ECBBC(o);
            break;
        case 0x2e:
            func_800E8348(o);
            break;
        }
    }
    g->pc++;
}

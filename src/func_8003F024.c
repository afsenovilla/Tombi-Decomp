// FUNC 8003f024 288 MAIN0
// MATCHING 8003f024 288
#include "TOBJ.H"
typedef struct { int x, y, z; } V3;
extern TObj *FUN_80018568();
extern unsigned char D_8007B084[];
extern unsigned char *D_8007B14C[];
void func_8003F024(TObj *o, short type, short sub, short *pos, short vh, short vv)
{
    TObj *n = FUN_80018568();
    if (n != 0) {
        n->active = 4;
        n->type = 3;
        n->subtype = type;
        n->b0c = sub | 0x80;
        n->b0f = D_8007B14C[D_8007B084[type]][3];
        n->animFrame = 0;
        n->h->raw = pos[1] << 16;
        n->y.raw = pos[3] << 16;
        n->d->raw = pos[5] << 16;
        *(V3 *)&n->d30 = *(V3 *)&n->a;
        n->velH = vh;
        n->velV = vv;
        o->d94 = (int)n;
    }
}

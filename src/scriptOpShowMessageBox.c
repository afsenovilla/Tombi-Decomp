// FUNC 80039e54 848 MAIN0
// MATCHING 80039e54 848
#include "scriptctx.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern TObj *SCRIPT_OBJECTS[];
extern unsigned char D_8009D2B0, D_8009C93F, D_8009C942;
extern unsigned char D_800A60E4x[], D_8009D2B0x[], D_8009C93Fx[];
extern int showMessageBoxDirect(int, int, V6 *);
extern void func_80113754(TObj *);
extern void func_800ECBBC(TObj *);

void scriptOpShowMessageBox(void)
{
    ScriptCtx *s = SCRIPT_CTX;
    TObj *o = SCRIPT_OBJECTS[s->arg[0]];
    V6 tmp;
    unsigned int cmd = s->arg[1];

    if (o != 0) {
        switch (cmd) {
        case 1:
            o->b6a = 1;
            o->timer = s->arg[2];
        case 0:
        case 9:
            o->b6a = 0;
            o->step = cmd;
            o->state = 0;
            break;
        case 2:
            o->timer = s->arg[2];
            o->velX = s->arg[3];
            o->b6a = 1;
            o->step = cmd;
            o->state = 0;
            break;
        case 3:
            o->timer = s->arg[2];
            o->d30 = s->arg[3];
            o->d34 = s->arg[4];
            o->w98 = s->arg[5];
            o->w9a = s->arg[6];
            o->b6a = 1;
            o->step = cmd;
            o->state = 0;
            break;
        case 8:
            tmp.v[1] = s->arg[5];
            tmp.v[3] = s->arg[6];
            tmp.v[5] = s->arg[7];
        case 4:
            if (cmd == 4) tmp = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(s->arg[2], s->arg[3], &tmp);
            switch (o->type & 0x7f) {
            case 0x18:
                o->w74 = s->arg[4];
                o->w76 = 0;
                func_80113754(o);
                D_8009D2B0 = 2;
                D_8009C93F = 1;
                PLAYER.obj.b04 = 5;
                PLAYER.obj.step = 0;
                PLAYER.obj.state = 0;
                break;
            case 0x19:
                o->b68 = 2;
                D_800A60E4x[0] = 3;
                D_8009D2B0x[0] = 2;
                D_8009C93Fx[0] = 1;
                if (s->arg[4] & 0x80) {
                    o->w74 = o->animFrame + (s->arg[4] & 0x7f);
                } else {
                    o->animFrame = o->b69 - 1;
                    o->w74 = o->animFrame + s->arg[4];
                }
                o->w76 = 0;
                func_800ECBBC(o);
                {
                    unsigned short *k = &PLAYER.obj.animFrame;
                    PLAYER.obj.b04 = 1;
                    PLAYER.obj.step = 2;
                    PLAYER.obj.state = 0;
                    *k |= 8;
                }
                break;
            }
            D_8009C942 = 1;
            o->b6a = 1;
            o->step = 4;
            o->state = 0;
            break;
        case 5:
            {
            int ty = o->type & 0x7f;
            if (ty == 0x18) goto L1;
            if (ty == 0x19) {
            L1:
                o->velX = s->arg[2];
                o->velY = s->arg[3];
            }
            }
            o->b6a = 1;
            o->step = cmd;
            o->state = 0;
            break;
        case 6:
            o->b6a = 0;
            o->step = cmd;
            o->state = 0;
            break;
        case 7:
            {
            int ty = o->type & 0x7f;
            if (ty == 0x18) goto L2;
            if (ty == 0x19) {
            L2:
                o->velX = s->arg[2];
                o->velY = s->arg[3];
                o->w78 = s->arg[4];
            }
            }
            o->b6a = 1;
            o->step = cmd;
            o->state = 0;
            break;
        }
    }
    s->pc++;
}
